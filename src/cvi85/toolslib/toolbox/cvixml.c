/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       cvixml.c                                                      */
/* Purpose:     provides functions to create and edit XML documents           */
/*                                                                            */
/*============================================================================*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#define boolean rpcndr_boolean
#include "cvixml.h"
#undef boolean
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>

// Check if multi-threading protection is not needed
#ifdef DISABLE_CVIXML_MULTITHREADING
#define MULTI_THREADED 0
#define CmtGetLock(a) 0
#define CmtReleaseLock(a) 0
#define CmtNewLock(a, b, c) 0
#define CmtDiscardLock(a) 0
#else
#define MULTI_THREADED 1
#endif

//-----------------------------------------------------------------------------
// Constants and macros
//-----------------------------------------------------------------------------
#define TMP_PREFIX          "CVI"
#define TAG_OPEN            '<'
#define TAG_CLOSE           '>'
#define TAG_END             '/'
#define NEWLINE             '\n'
#define XML_PROC_INSTR_TARG "xml"
#define XML_PROC_INSTR_DATA "version=\"1.0\""
#define eofChk(fcall) if (error = (fcall), error == EOF) goto Error;
#define libErrChk(fcall) if (error = (fcall), error < 0) {__result = E_UNEXPECTED; goto Error;} else
#define LOCK(l, c) {libErrChk(CmtGetLock(l)); c++;}
#define UNLOCK(l, c) {while(c--) CmtReleaseLock(l);}

enum {
    kNoEndFound = 0,
    kEndFound = 1,
};

//-----------------------------------------------------------------------------
// User-defined types
//-----------------------------------------------------------------------------
typedef struct _CVIXMLDoc
{
    CAObjHandle docHdl;
    char        *path;
    int         lock;
} CVIXMLDoc;

typedef struct _CVIXMLElem
{
    CAObjHandle             docHdl;
    MSXMLObj_IXMLDOMElement elem;
    int                     lock;
} CVIXMLElem;

typedef struct _CVIXMLAttr
{
    CAObjHandle                 docHdl;
    MSXMLObj_IXMLDOMElement     elemHdl;
    MSXMLObj_IXMLDOMAttribute   attr;
    int                         lock;
} CVIXMLAttr;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
static CVIXMLStatus FindNthElement (MSXMLObj_IXMLDOMNodeList nodeList,
                                    int index, MSXMLObj_IXMLDOMNode *foundElem);
static CVIXMLStatus GetAttributeIndex (MSXMLObj_IXMLDOMNamedNodeMap list,
                                       MSXMLObj_IXMLDOMAttribute attr,
                                       int *index);
static CVIXMLStatus GetNodeText (MSXMLObj_IXMLDOMNode elem,
                                 MSXMLObj_IXMLDOMText *foundElem);
static CVIXMLStatus GetNumElements (MSXMLObj_IXMLDOMNode elem,
                                    int *numChildren);
static CVIXMLStatus FormatXMLDocument (CAObjHandle doc,
                                       MSXMLObj_IXMLDOMNode elem);
static int FindTagClose (FILE *rf, FILE *wf);
static int FindTagOpen (FILE *rf, FILE *wf);
static int FormatNode (FILE *rf, FILE *wf);
static int fgetcMb (FILE *pf);
static int fputcMb (int c, FILE *pf);

//-----------------------------------------------------------------------------
// DOCUMENT FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose:     This function loads an existing XML document.
// Parameters:  fullPath - Location of the document.
//              doc - Handle of the document object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLLoadDocument (const char *fullPath, CVIXMLDocument *doc)
{
    int                     error;
    char                    *newPath = NULL;
    VBOOL                   success = VFALSE;
    VARIANT                 variantPath;
    CVIXMLDoc               *docPtr = NULL;
    CAObjHandle             xmlHdl = 0;
    HRESULT                 __result = S_OK;

    __caErrChk(CA_VariantSetEmpty(&variantPath));
    if((fullPath == NULL) || (fullPath[0] == '\0'))
    {
        __caErrChk(E_CVIXML_INVALID_PATH);
    }
    if(doc == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    *doc = 0;

    docPtr = (CVIXMLDoc *)calloc(1, sizeof(CVIXMLDoc));
    newPath = calloc(1, strlen(fullPath) + 1);
    if((docPtr == NULL) || (newPath == NULL))
    {
        __caErrChk(E_OUTOFMEMORY);
    }
    libErrChk(CmtNewLock(0, 0, &docPtr->lock));
    _mbscpy((unsigned char*)newPath, (const unsigned char*)fullPath);

    __caErrChk(MSXML_NewDOMDocumentIXMLDOMDocument(NULL, MULTI_THREADED, LOCALE_NEUTRAL, 0, &xmlHdl));
    __caErrChk(MSXML_IXMLDOMDocumentSetasync(xmlHdl, NULL, VFALSE));
    __caErrChk(MSXML_IXMLDOMDocumentSetpreserveWhiteSpace(xmlHdl, NULL, VFALSE));
    __caErrChk(CA_VariantSetCString(&variantPath, fullPath));
    __caErrChk(MSXML_IXMLDOMDocumentload(xmlHdl, NULL, variantPath, &success));
    if(!success)
    {
        __caErrChk(E_CVIXML_INVALID_DOCUMENT);
    }

    // Set up the document's structure information.
    // Cannot fail after this.
    docPtr->docHdl = xmlHdl;
    docPtr->path = newPath;
    *doc = (CVIXMLDocument)docPtr;
    xmlHdl = 0;
    newPath = 0;
    docPtr = NULL;

Error:
    CA_DiscardObjHandle(xmlHdl);
    CA_VariantClear(&variantPath);
    CVIXMLDiscardDocument((CVIXMLDocument)docPtr);
    free(newPath);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function creates a new XML document.  A root element name
//              must be specified in order for it to be created.
// Parameters:  rootElementName - Tag for the root element.
//              doc - Handle of the document object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLNewDocument (const char *rootElementName,
                                        CVIXMLDocument *doc)
{
    CVIXMLDoc                            *docPtr = NULL;
    CAObjHandle                           xmlHdl = 0;
    MSXMLObj_IXMLDOMElement               rootElement = 0;
    HRESULT                               __result = S_OK;
    MSXMLObj_IXMLDOMProcessingInstruction processingInstr = 0;
    MSXMLObj_IXMLDOMElement               outNewChild = 0;
    int                                   error;

    if((rootElementName == NULL) || (rootElementName[0] == '\0'))
    {
        __caErrChk(E_INVALIDARG);
    }
    if(doc == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    *doc = 0;

    docPtr = (CVIXMLDoc *)calloc(1, sizeof(CVIXMLDoc));
    if(docPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }
    libErrChk(CmtNewLock(0, 0, &docPtr->lock));

    __caErrChk(MSXML_NewDOMDocumentIXMLDOMDocument(NULL, MULTI_THREADED, LOCALE_NEUTRAL, 0, &xmlHdl));
    __caErrChk(MSXML_IXMLDOMDocumentSetasync(xmlHdl, NULL, VFALSE));
    __caErrChk(MSXML_IXMLDOMDocumentSetpreserveWhiteSpace(xmlHdl, NULL, VFALSE));
    __caErrChk(MSXML_IXMLDOMDocumentcreateElement(xmlHdl, NULL, rootElementName, &rootElement));
    __caErrChk(MSXML_IXMLDOMDocumentcreateProcessingInstruction(xmlHdl, NULL, XML_PROC_INSTR_TARG, XML_PROC_INSTR_DATA, &processingInstr));
    __caErrChk(MSXML_IXMLDOMDocumentappendChild(xmlHdl, NULL, processingInstr, &outNewChild));
    __caErrChk(MSXML_IXMLDOMDocumentSetByRefdocumentElement(xmlHdl, NULL, rootElement));
    // Set up the document's structure information.
    // Cannot fail after this.
    docPtr->docHdl = xmlHdl;
    docPtr->path = NULL;
    *doc = (CVIXMLDocument)docPtr;
    xmlHdl = 0;
    docPtr = NULL;

Error:
    CA_DiscardObjHandle(xmlHdl);
    CA_DiscardObjHandle(rootElement);
    CA_DiscardObjHandle(processingInstr);
    CA_DiscardObjHandle(outNewChild);
    CVIXMLDiscardDocument((CVIXMLDocument)docPtr);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function saves an XML document.  If a path is not passed in,
//              the document's structure is checked for an existing path.  If one
//              doesn't exist then an error is returned.
// Parameters:  doc - Handle of the document object.
//              fullPath - Location of the document.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLSaveDocument (CVIXMLDocument doc, int formatDoc,
                                         const char *fullPath)
{
    char                    *newPath = NULL;
    VARIANT                 variantPath;
    CVIXMLDoc               *docPtr = (CVIXMLDoc *)doc;
    HRESULT                 __result = S_OK;
    int                     error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&variantPath));

    if(docPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }

    LOCK(docPtr->lock, locked);

    if((fullPath == NULL) || (fullPath[0] == '\0'))
    {
        if((docPtr->path == NULL) || (docPtr->path[0] == '\0'))
        {
            __caErrChk (E_CVIXML_INVALID_PATH);
        }
        else
        {
            fullPath = docPtr->path;
        }
    }
    else
    {
        newPath = calloc(1, strlen(fullPath) + 1);
        if(newPath == NULL)
        {
            __caErrChk(E_OUTOFMEMORY);
        }
        _mbscpy((unsigned char*)newPath, (const unsigned char*)fullPath);
        free(docPtr->path);
        docPtr->path = newPath;
        newPath = 0;
    }

    __caErrChk(CA_VariantSetCString(&variantPath, fullPath));
    __caErrChk(MSXML_IXMLDOMDocumentsave(docPtr->docHdl, NULL, variantPath));

    if(formatDoc)
    {
        CVIXMLLineFormatXMLDocument ((char *)fullPath);
    }

Error:
    CA_VariantClear(&variantPath);
    free(newPath);
    UNLOCK(docPtr->lock, locked);
    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function discards an XML document.
// Parameters:  doc - Handle of the document object.
// Return:      None.
//-----------------------------------------------------------------------------
void CVIFUNC CVIXMLDiscardDocument (CVIXMLDocument doc)
{
    CVIXMLDoc *docPtr = (CVIXMLDoc *)doc;

    if(docPtr)
    {
        int     lock = docPtr->lock;
        int     locked = 0;

        if(lock && CmtGetLock(lock) >= 0)
            locked = 1;
        docPtr->lock = 0;
        CA_DiscardObjHandle(docPtr->docHdl);
        free(docPtr->path);
        free(docPtr);
        if(locked)
            CmtReleaseLock(lock);
        if(lock)
            CmtDiscardLock(lock);
    }
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the root element of a document.
// Parameters:  doc - Handle of the document object.
//              elem - Handle of an element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetRootElement (CVIXMLDocument doc, CVIXMLElement *elem)
{
    CVIXMLDoc               *docPtr = (CVIXMLDoc *)doc;
    CVIXMLElem              *elemPtr = NULL;
    MSXMLObj_IXMLDOMElement rootElement = 0;
    HRESULT                 __result = S_OK;
    int                     error, locked = 0;

    if((docPtr == NULL) || (elem == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *elem = 0;

    LOCK(docPtr->lock, locked);

    elemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
    if(elemPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }
    libErrChk(CmtNewLock(0, 0, &elemPtr->lock));

    __caErrChk(MSXML_IXMLDOMDocumentGetdocumentElement(docPtr->docHdl, NULL,
                                                       &rootElement));
    if(!rootElement)
    {
        __caErrChk(E_CVIXML_INVALID_DOCUMENT);
    }

    // Set up the document's structure information.
    __caErrChk(CA_DuplicateObjHandle(docPtr->docHdl, 0, &elemPtr->docHdl));
    // Cannot fail after this.
    elemPtr->elem = rootElement;
    *elem = (CVIXMLElement)elemPtr;
    rootElement = 0;
    elemPtr = NULL;

Error:
    CA_DiscardObjHandle(rootElement);
    CVIXMLDiscardElement((CVIXMLElement)elemPtr);
    UNLOCK(docPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// ELEMENT FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose:     This function creates a new element.  It inserts the element at the
//              index of the *element* where it wants it.  That is, it ignores text
//              nodes and does not treat them like children.
// Parameters:  parent - Parent of the element.
//              index - Index to place it at (-1 means always
//                      append to end - has BIG PERFORMANCE GAIN)
//              tag - Tag of the element.
//              elem - Handle of an element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLNewElement (CVIXMLElement parent, int index,
                                       const char *tag, CVIXMLElement *elem)
{
    int                         numChildren = 0;
    VARIANT                     sibVariant;
    CVIXMLElem                  *parentPtr = (CVIXMLElem *)parent;
    CVIXMLElem                  *elemPtr = NULL;
    MSXMLObj_IXMLDOMElement     newElement = 0;
    MSXMLObj_IXMLDOMElement     newElementAdded = 0;
    MSXMLObj_IXMLDOMElement     sibElement = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&sibVariant));
    if((tag == NULL) || (tag[0] == '\0'))
    {
        __caErrChk(E_INVALIDARG);
    }
    if((parentPtr == NULL) || (elem == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *elem = 0;

    LOCK(parentPtr->lock, locked);

    if(index != -1) // then we need to find out how many child elements that we have
    {
        __caErrChk(GetNumElements(parentPtr->elem, &numChildren));
        if((index > numChildren) || (index < -1))
        {
            __caErrChk(E_CVIXML_INVALID_INDEX);
        }
    }

    elemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
    if(elemPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }
    libErrChk(CmtNewLock(0, 0, &elemPtr->lock));

    __caErrChk(MSXML_IXMLDOMDocumentcreateElement(parentPtr->docHdl, NULL, tag,
                                                  &newElement));

    if( (numChildren) && (index != -1) )
    {
        __caErrChk(FindNthElement(parentPtr->elem, index, &sibElement));
        if(!sibElement)
        {
            __caErrChk(MSXML_IXMLDOMNodeappendChild(parentPtr->elem, NULL,
                                                    newElement, &newElementAdded));
        }
        else
        {
            __caErrChk(CA_VariantSetObjHandle(&sibVariant, sibElement, CAVT_UNKNOWN));
            __caErrChk(MSXML_IXMLDOMNodeinsertBefore(parentPtr->elem, NULL,
                                                     newElement, sibVariant,
                                                     &newElementAdded));
        }
    }
    else
    {
        __caErrChk(MSXML_IXMLDOMNodeappendChild(parentPtr->elem, NULL, newElement,
                                                &newElementAdded));
    }

    // Set up the document's structure information.
    __caErrChk(CA_DuplicateObjHandle(parentPtr->docHdl, 0, &elemPtr->docHdl));
    // Cannot fail after this.
    elemPtr->elem = newElementAdded;
    *elem = (CVIXMLElement)elemPtr;
    newElementAdded = 0;
    elemPtr = NULL;

Error:
    CA_DiscardObjHandle(sibElement);
    CA_DiscardObjHandle(newElement);
    CA_DiscardObjHandle(newElementAdded);
    CA_VariantClear(&sibVariant);
    CVIXMLDiscardElement((CVIXMLElement)elemPtr);
    UNLOCK(parentPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function sets the value of an element.  Even though an
//              element can have multiple text nodes, this function sets the
//              first one that it finds.
// Parameters:  elem - Handle of an element object.
//              value - New value for the element.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLSetElementValue (CVIXMLElement elem, const char *value)
{
    VARIANT                 variantVal;
    CVIXMLElem              *elemPtr = (CVIXMLElem *)elem;
    MSXMLObj_IXMLDOMText    textChild = 0;
    MSXMLObj_IXMLDOMText    textChildAdded = 0;
    MSXMLObj_IXMLDOMNode    firstChild = 0;
    HRESULT                 __result = S_OK;
    int                     error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&variantVal));
    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(value == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }

    LOCK(elemPtr->lock, locked);

    __caErrChk(GetNodeText(elemPtr->elem, &textChild));
    if(textChild)
    {
        __caErrChk(CA_VariantSetCString(&variantVal, value));
        __caErrChk(MSXML_IXMLDOMNodeSetnodeTypedValue(textChild, NULL, variantVal));
    }
    else
    {
        __caErrChk(MSXML_IXMLDOMDocumentcreateTextNode(elemPtr->docHdl, NULL,
                                                       value, &textChild));
        __caErrChk(MSXML_IXMLDOMNodeGetfirstChild(elemPtr->elem, NULL, &firstChild));
        if(firstChild)
        {
            __caErrChk(CA_VariantSetObjHandle(&variantVal, firstChild, CAVT_UNKNOWN));
        }
        __caErrChk(MSXML_IXMLDOMNodeinsertBefore(elemPtr->elem, NULL, textChild,
            firstChild ? variantVal : CA_VariantNULL(), &textChildAdded));
    }

Error:
    CA_DiscardObjHandle(textChild);
    CA_DiscardObjHandle(textChildAdded);
    CA_VariantClear(&variantVal);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the length of the tag of an element.
// Parameters:  elem - Handle of an element object.
//              length - Length of the tag.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetElementTagLength (CVIXMLElement elem, int *length)
{
    char        *tag = NULL;
    CVIXMLElem  *elemPtr = (CVIXMLElem *)elem;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(length == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *length = 0;

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMElementGetnodeName(elemPtr->elem, NULL, &tag));
    *length = strlen(tag);

Error:
    CA_FreeMemory(tag);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the tag of an element.
// Parameters:  elem - Handle of an element object.
//              tag - Tag of the element.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetElementTag (CVIXMLElement elem, char *tag)
{
    char        *tempTag = NULL;
    CVIXMLElem  *elemPtr = (CVIXMLElem *)elem;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(tag == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *tag = '\0';

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMElementGetnodeName(elemPtr->elem, NULL, &tempTag));
    _mbscpy((unsigned char*)tag, (const unsigned char*)tempTag);

Error:
    CA_FreeMemory(tempTag);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the length of the value of an element.  It gets
//              the length of the first value it finds.  If the value has whitespace
//              characters in it, it counts these too.
// Parameters:  elem - Handle of an element object.
//              length - Length of the tag.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetElementValueLength (CVIXMLElement elem, int *length)
{
    char                    *value = NULL;
    VBOOL                   hasChildren = VFALSE;
    VARIANT                 variantVal;
    CVIXMLElem              *elemPtr = (CVIXMLElem *)elem;
    MSXMLObj_IXMLDOMText    textChild = 0;
    HRESULT                 __result = S_OK;
    int                     error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&variantVal));
    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(length == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *length = 0;

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMNodehasChildNodes(elemPtr->elem, NULL, &hasChildren));
    if(hasChildren)
    {
        __caErrChk(GetNodeText(elemPtr->elem, &textChild));
        if(textChild)
        {
            __caErrChk(MSXML_IXMLDOMNodeGetnodeTypedValue(textChild, NULL, &variantVal));
            __caErrChk(CA_VariantConvertToType(&variantVal, CAVT_CSTRING, &value));
            *length = strlen(value);
        }
    }

Error:
    CA_DiscardObjHandle(textChild);
    CA_VariantClear(&variantVal);
    CA_FreeMemory(value);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the value of an element.  It returns the
//              first value it finds.
// Parameters:  elem - Handle of an element object.
//              value - Value of the element.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetElementValue (CVIXMLElement elem, char *value)
{
    char                    *tempValue = NULL;
    VBOOL                   hasChildren = VFALSE;
    VARIANT                 variantVal;
    CVIXMLElem              *elemPtr = (CVIXMLElem *)elem;
    MSXMLObj_IXMLDOMText    textChild = 0;
    HRESULT                 __result = S_OK;
    int                     error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&variantVal));
    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(value == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *value = '\0';

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMNodehasChildNodes(elemPtr->elem, NULL, &hasChildren));
    if(hasChildren)
    {
        __caErrChk(GetNodeText(elemPtr->elem, &textChild));
        if(textChild)
        {
            __caErrChk(MSXML_IXMLDOMNodeGetnodeTypedValue(textChild, NULL, &variantVal));
            __caErrChk(CA_VariantConvertToType(&variantVal, CAVT_CSTRING, &tempValue));
            _mbscpy((unsigned char*)value, (const unsigned char*)tempValue);
        }
    }

Error:
    CA_DiscardObjHandle(textChild);
    CA_VariantClear(&variantVal);
    CA_FreeMemory(tempValue);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function copies an element.  It inserts the element at the
//              index of the *element* where it wants it.  That is, it ignores
//              text nodes and does not treat them like children.
// Parameters:  srcElem - Handle of an element object to copy.
//              copyChildElements - Specifies whether to copy a node's children
//                                  or not.
//              destParent - Handle of an element that will be the parent of
//                           the copy.
//              index - Index where we want the element inserted.
//              destElem - Handle of an element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLCopyElement (CVIXMLElement srcElem,
                                        int copyChildElements,
                                        CVIXMLElement destParent, int index,
                                        CVIXMLElement *destElem)
{
    int                     numChildren = 0;
    VARIANT                 sibVariant;
    CVIXMLElem              *elemPtr = (CVIXMLElem *)srcElem;
    CVIXMLElem              *parentPtr = (CVIXMLElem *)destParent;
    CVIXMLElem              *newElemPtr = NULL;
    MSXMLObj_IXMLDOMElement sibElement = 0;
    MSXMLObj_IXMLDOMNode    newElement = 0;
    MSXMLObj_IXMLDOMNode    newElementAdded = 0;
    MSXMLObj_IXMLDOMText    textChild = 0;
    MSXMLObj_IXMLDOMText    textChildCopy = 0;
    MSXMLObj_IXMLDOMText    textChildAdded = 0;
    HRESULT                 __result = S_OK;
    int                     error, elemLocked = 0, parentLocked = 0;

    __caErrChk(CA_VariantSetEmpty(&sibVariant));
    if((elemPtr == NULL) || (parentPtr == NULL) || (destElem == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *destElem = 0;

    LOCK(elemPtr->lock, elemLocked);
    LOCK(parentPtr->lock, parentLocked);

    __caErrChk(GetNumElements(parentPtr->elem, &numChildren));
    if((index > numChildren) || (index < -1))
    {
        __caErrChk(E_CVIXML_INVALID_INDEX);
    }

    newElemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
    if(newElemPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }

    libErrChk(CmtNewLock(0, 0, &newElemPtr->lock));

    if(copyChildElements)
    {
        __caErrChk(MSXML_IXMLDOMNodecloneNode(elemPtr->elem, NULL, VTRUE,
                                              &newElement));
    }
    else
    {
        __caErrChk(MSXML_IXMLDOMNodecloneNode(elemPtr->elem, NULL, VFALSE,
                                              &newElement));
        __caErrChk(GetNodeText(elemPtr->elem, &textChild));
        if(textChild)
        {
            __caErrChk(MSXML_IXMLDOMNodecloneNode(textChild, NULL, VTRUE,
                                                  &textChildCopy));
            __caErrChk(MSXML_IXMLDOMNodeappendChild(newElement, NULL,
                                                    textChildCopy,
                                                    &textChildAdded));
        }
    }

    if(numChildren)
    {
        __caErrChk(FindNthElement(parentPtr->elem, index, &sibElement));
        if(!sibElement)
        {
            __caErrChk(MSXML_IXMLDOMNodeappendChild(parentPtr->elem, NULL,
                                                    newElement,
                                                    &newElementAdded));
        }
        else
        {
            __caErrChk(CA_VariantSetObjHandle(&sibVariant, sibElement, CAVT_UNKNOWN));
            __caErrChk(MSXML_IXMLDOMNodeinsertBefore(parentPtr->elem, NULL,
                                                     newElement, sibVariant,
                                                     &newElementAdded));
        }
    }
    else
    {
        __caErrChk(MSXML_IXMLDOMNodeappendChild(parentPtr->elem, NULL,
                                                newElement, &newElementAdded));
    }

    // Set up the document's structure information.
    __caErrChk(CA_DuplicateObjHandle(parentPtr->docHdl, 0, &newElemPtr->docHdl));
    // Cannot fail after this.
    newElemPtr->elem = newElementAdded;
    *destElem = (CVIXMLElement)newElemPtr;
    newElementAdded = 0;
    newElemPtr = NULL;

Error:
    CA_DiscardObjHandle(sibElement);
    CA_DiscardObjHandle(newElement);
    CA_DiscardObjHandle(newElementAdded);
    CA_DiscardObjHandle(textChild);
    CA_DiscardObjHandle(textChildCopy);
    CA_DiscardObjHandle(textChildAdded);
    CA_VariantClear(&sibVariant);
    CVIXMLDiscardElement((CVIXMLElement)newElemPtr);
    UNLOCK(parentPtr->lock, parentLocked);
    UNLOCK(elemPtr->lock, elemLocked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function discards an XML element.
// Parameters:  elem - Handle of the element object.
// Return:      None.
//-----------------------------------------------------------------------------
void CVIFUNC CVIXMLDiscardElement (CVIXMLElement elem)
{
    CVIXMLElem *elemPtr = (CVIXMLElem *)elem;
    if(elemPtr)
    {
        int     lock = elemPtr->lock, locked = 0;

        if (lock && CmtGetLock(lock) >= 0)
            locked = 1;
        elemPtr->lock = 0;
        CA_DiscardObjHandle(elemPtr->elem);
        CA_DiscardObjHandle(elemPtr->docHdl);
        free(elemPtr);
        if(locked)
            CmtReleaseLock(lock);
        if(lock)
            CmtDiscardLock(lock);
    }
}

//-----------------------------------------------------------------------------
// Purpose:     This function finds all elements with a certain tag in the document.
// Parameters:  parent - Handle of an element object to search under.
//              tag - Tag to search for.
//              elemList - Handle of an element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLFindElements (CVIXMLElement parent, const char *tag,
                                         ListType *elemList)
{
    CVIXMLElem                  *elemPtr = NULL;
    CVIXMLElem                  *parentPtr = (CVIXMLElem *)parent;
    ListType                    retNodeList = 0;
    MSXMLObj_IXMLDOMNode        tempNode = 0;
    MSXMLObj_IXMLDOMNodeList    nodeList = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    if(parentPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if((tag == NULL) || (tag[0] == '\0') || (elemList == NULL))
    {
        __caErrChk(E_INVALIDARG);
    }
    *elemList = 0;

    retNodeList = ListCreate(sizeof(CVIXMLElement));
    if(retNodeList == 0)
        __caErrChk (E_OUTOFMEMORY);

    LOCK(parentPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMElementgetElementsByTagName(parentPtr->elem, NULL,
                                                        tag, &nodeList));
    __caErrChk(MSXML_IXMLDOMNodeListnextNode(nodeList, NULL, &tempNode));
    while(tempNode)
    {
        elemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
        if(elemPtr == NULL)
        {
            __caErrChk(E_OUTOFMEMORY);
        }
        libErrChk(CmtNewLock(0, 0, &elemPtr->lock));
        __caErrChk(CA_DuplicateObjHandle(tempNode, 0, &elemPtr->elem));
        __caErrChk(CA_DuplicateObjHandle(parentPtr->docHdl, 0, &elemPtr->docHdl));
        if(ListInsertItem(retNodeList, &elemPtr, END_OF_LIST) == 0)
            __caErrChk(E_OUTOFMEMORY);
        elemPtr = NULL;
        CA_DiscardObjHandle(tempNode);
        tempNode = 0;
        __caErrChk(MSXML_IXMLDOMNodeListnextNode(nodeList, NULL, &tempNode));
    }
    *elemList = retNodeList;
    retNodeList = 0;

Error:
    if(retNodeList)
    {
        int i, numItems = ListNumItems (retNodeList);
        for (i = 0; i < numItems; ++i) {
            CVIXMLElement   item = 0;
            ListRemoveItem(retNodeList, &item, FRONT_OF_LIST);
            CVIXMLDiscardElement(item);
        }
        ListDispose(retNodeList);
    }
    CA_DiscardObjHandle(tempNode);
    CA_DiscardObjHandle(nodeList);
    CVIXMLDiscardElement((CVIXMLElement)elemPtr);
    UNLOCK(parentPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function removes an element and its children from the document.
// Parameters:  elem - Handle of the element object to delete.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLRemoveElement (CVIXMLElement elem)
{
    CVIXMLElem              *elemPtr = (CVIXMLElem *)elem;
    MSXMLType_DOMNodeType   nodeType = 0;
    MSXMLObj_IXMLDOMNode    parentNode = 0;
    MSXMLObj_IXMLDOMNode    removedChild = 0;
    HRESULT                 __result = S_OK;
    int                     error, locked = 0;

    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMNodeGetparentNode(elemPtr->elem, NULL, &parentNode));
    if(parentNode)
    {
        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(parentNode, NULL, &nodeType));
        if(nodeType == MSXMLConst_NODE_DOCUMENT)
        {
            __caErrChk(E_INVALIDARG);
        }
        __caErrChk(MSXML_IXMLDOMNoderemoveChild(parentNode, NULL,
                                                elemPtr->elem, &removedChild));
    }

Error:
    CA_DiscardObjHandle(parentNode);
    CA_DiscardObjHandle(removedChild);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// CHILD ELEMENT FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose:     This function gets the number of children an element has.
//              It does not count text nodes as children.
// Parameters:  parent - Handle of the element object.
//              numChildren - Number of children the element has.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetNumChildElements (CVIXMLElement parent, int *numChildren)
{
    CVIXMLElem  *elemPtr = (CVIXMLElem *)parent;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(numChildren == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *numChildren = 0;

    LOCK(elemPtr->lock, locked);
    __caErrChk(GetNumElements(elemPtr->elem, numChildren));

Error:
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the child of an element by its index.
//              It gets the element at the index of the *element* index specified.
//              That is, it ignores text nodes and does not treat them like
//              children when counting where the index is.
// Parameters:  parent - Handle of the element object.
//              index - Index where the child is at.
//              child - Handle of the child element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetChildElementByIndex (CVIXMLElement parent,
                                                   int index,
                                                   CVIXMLElement *child)
{
    int                         numChildren = 0;
    CVIXMLElem                  *elemPtr = (CVIXMLElem *)parent;
    CVIXMLElem                  *childElemPtr = NULL;
    MSXMLObj_IXMLDOMNode        childElem = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    if((elemPtr == NULL) || (child == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *child = 0;

    LOCK(elemPtr->lock, locked);
    __caErrChk(GetNumElements(elemPtr->elem, &numChildren));
    if((index > numChildren - 1) || (index < -1))
    {
        __caErrChk(E_CVIXML_INVALID_INDEX);
    }

    childElemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
    if(childElemPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }

    libErrChk(CmtNewLock(0, 0, &childElemPtr->lock));

    if(index == -1)
    {
        index = numChildren - 1;
    }
    __caErrChk(FindNthElement(elemPtr->elem, index, &childElem));

    // Set up the document's structure information.
    __caErrChk(CA_DuplicateObjHandle(elemPtr->docHdl, 0, &childElemPtr->docHdl));
    // Cannot fail after this.
    childElemPtr->elem = childElem;
    *child = (CVIXMLElement)childElemPtr;
    childElem = 0;
    childElemPtr = NULL;

Error:
    CA_DiscardObjHandle(childElem);
    CVIXMLDiscardElement((CVIXMLElement)childElemPtr);
    UNLOCK(elemPtr->lock, locked);
    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the child of an element by its tag.
// Parameters:  parent - Handle of the element object.
//              tag - Tag to search for the child by.
//              child - Handle of the child element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetChildElementByTag (CVIXMLElement parent, const char *tag,
                                                 CVIXMLElement *child)
{
    char                        *compTag = NULL;
    CVIXMLElem                  *elemPtr = (CVIXMLElem *)parent;
    CVIXMLElem                  *childElemPtr = NULL;
    MSXMLObj_IXMLDOMNode        childElem = 0;
    MSXMLType_DOMNodeType       nodeType = 0;
    MSXMLObj_IXMLDOMNodeList    childList = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    if((elemPtr == NULL) || (child == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    if((tag == NULL) || (tag[0] == '\0'))
    {
        __caErrChk(E_INVALIDARG);
    }
    *child = 0;

    LOCK(elemPtr->lock, locked);

    childElemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
    if(childElemPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }

    libErrChk(CmtNewLock(0, 0, &childElemPtr->lock));

    __caErrChk(MSXML_IXMLDOMNodeGetchildNodes(elemPtr->elem, NULL, &childList));
    __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &childElem));
    while(childElem)
    {
        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(childElem, NULL, &nodeType));
        if(nodeType == MSXMLConst_NODE_ELEMENT)
        {
            __caErrChk(MSXML_IXMLDOMNodeGetnodeName(childElem, NULL, &compTag));
            if(!_mbscmp((const unsigned char*)tag, (const unsigned char*)compTag))
            {
                // Set up the document's structure information.
                __caErrChk(CA_DuplicateObjHandle(elemPtr->docHdl, 0, &childElemPtr->docHdl));
                // Cannot fail after this.
                childElemPtr->elem = childElem;
                childElem = 0;
                *child = (CVIXMLElement)childElemPtr;
                childElemPtr = NULL;
                break;
            }
            CA_FreeMemory(compTag);
            compTag = NULL;
        }
        CA_DiscardObjHandle(childElem);
        childElem = 0;
        __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &childElem));
    }

Error:
    CA_FreeMemory(compTag);
    CA_DiscardObjHandle(childElem);
    CA_DiscardObjHandle(childList);
    CVIXMLDiscardElement((CVIXMLElement)childElemPtr);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the element to the right of the current element.
// Parameters:  elem - Handle of the element object.
//              sibElem - Handle of the sibling element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetNextElement (CVIXMLElement elem, CVIXMLElement *sibElem)
{
    CVIXMLElem                  *elemPtr = (CVIXMLElem *)elem;
    CVIXMLElem                  *sibElemPtr = NULL;
    MSXMLObj_IXMLDOMNode        rightSib = 0;
    MSXMLType_DOMNodeType       nodeType = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    if((elemPtr == NULL) || (sibElem == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *sibElem = 0;

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMNodeGetnextSibling(elemPtr->elem, NULL, &rightSib));
    while(rightSib)
    {
        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(rightSib, NULL, &nodeType));
        if(nodeType == MSXMLConst_NODE_ELEMENT)
        {
            sibElemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
            if(sibElemPtr == NULL)
            {
                __caErrChk(E_OUTOFMEMORY);
            }
            libErrChk(CmtNewLock(0, 0, &sibElemPtr->lock));
            // Set up the document's structure information.
            __caErrChk(CA_DuplicateObjHandle(elemPtr->docHdl, 0, &sibElemPtr->docHdl));
            // Cannot fail after this.
            sibElemPtr->elem = rightSib;
            rightSib = 0;
            *sibElem = (CVIXMLElement)sibElemPtr;
            sibElemPtr = NULL;
            break;
        }
        __caErrChk(MSXML_IXMLDOMNodeGetnextSibling(rightSib, NULL, &rightSib));
    }

Error:
    CA_DiscardObjHandle(rightSib);
    CVIXMLDiscardElement((CVIXMLElement)sibElemPtr);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the element to the left of the current element.
// Parameters:  elem - Handle of the element object.
//              sibElem - Handle of the sibling element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetPreviousElement (CVIXMLElement elem, CVIXMLElement *sibElem)
{
    CVIXMLElem                  *elemPtr = (CVIXMLElem *)elem;
    CVIXMLElem                  *sibElemPtr = NULL;
    MSXMLObj_IXMLDOMNode        leftSib = 0;
    MSXMLType_DOMNodeType       nodeType = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    if((elemPtr == NULL) || (sibElem == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *sibElem = 0;

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMNodeGetpreviousSibling(elemPtr->elem, NULL, &leftSib));
    while(leftSib)
    {
        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(leftSib, NULL, &nodeType));
        if(nodeType == MSXMLConst_NODE_ELEMENT)
        {
            sibElemPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
            if(sibElemPtr == NULL)
            {
                __caErrChk(E_OUTOFMEMORY);
            }
            libErrChk(CmtNewLock(0, 0, &sibElemPtr->lock));
            // Set up the document's structure information.
            __caErrChk(CA_DuplicateObjHandle(elemPtr->docHdl, 0, &sibElemPtr->docHdl));
            // Cannot fail after this.
            sibElemPtr->elem = leftSib;
            leftSib = 0;
            *sibElem = (CVIXMLElement)sibElemPtr;
            sibElemPtr = NULL;
            break;
        }
        __caErrChk(MSXML_IXMLDOMNodeGetpreviousSibling(leftSib, NULL, &leftSib));
    }

Error:
    CA_DiscardObjHandle(leftSib);
    CVIXMLDiscardElement((CVIXMLElement)sibElemPtr);
    UNLOCK(elemPtr->lock, locked);
    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the parent of an element.  Of course if it
//              is the root of the document then it returns an error.
// Parameters:  child - Handle of the element object.
//              parent - Handle of the parent element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetParentElement (CVIXMLElement child, CVIXMLElement *parent)
{
    CVIXMLElem              *elemPtr = (CVIXMLElem *)child;
    CVIXMLElem              *parentPtr = NULL;
    MSXMLObj_IXMLDOMNode    parentElem = 0;
    MSXMLType_DOMNodeType   nodeType = 0;
    HRESULT                 __result = S_OK;
    int                     error, locked = 0;

    if((elemPtr == NULL) || (parent == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *parent = 0;

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMNodeGetparentNode(elemPtr->elem, NULL, &parentElem));
    if(parentElem)
    {
        parentPtr = (CVIXMLElem *)calloc(1, sizeof(CVIXMLElem));
        if(parentPtr == NULL)
        {
            __caErrChk(E_OUTOFMEMORY);
        }
        libErrChk(CmtNewLock(0, 0, &parentPtr->lock));

        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(parentElem, NULL, &nodeType));
        if(nodeType == MSXMLConst_NODE_DOCUMENT)
        {
            __result = S_FALSE;
            *parent = 0;
            goto Error;
        }

        // Set up the document's structure information.
        __caErrChk(CA_DuplicateObjHandle(elemPtr->docHdl, 0, &parentPtr->docHdl));
        // Cannot fail after this.
        parentPtr->elem = parentElem;
        *parent = (CVIXMLElement)parentPtr;
        parentElem = 0;
        parentPtr = NULL;
    }

Error:
    CA_DiscardObjHandle(parentElem);
    CVIXMLDiscardElement((CVIXMLElement)parentPtr);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// ATTRIBUTE FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose:     This function gets the number of attributes an element has.
// Parameters:  elem - Handle of the element object.
//              numAttrs - Number of attributes the element has.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetNumAttributes (CVIXMLElement elem, int *numAttrs)
{
    CVIXMLElem                      *elemPtr = (CVIXMLElem *)elem;
    MSXMLObj_IXMLDOMNamedNodeMap    attrList = 0;
    HRESULT                         __result = S_OK;
    int                             error, locked = 0;

    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(numAttrs == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *numAttrs = 0;

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMNodeGetattributes(elemPtr->elem, NULL, &attrList));
    __caErrChk(MSXML_IXMLDOMNamedNodeMapGetlength(attrList, NULL, numAttrs));

Error:
    CA_DiscardObjHandle(attrList);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the attribute of an element by its index.
// Parameters:  elem - Handle of the element object.
//              index - Index where the attribute is at.
//              attr - Handle of the attribute element object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetAttributeByIndex (CVIXMLElement elem, int index,
                                                CVIXMLAttribute *attr)
{
    int                             numAttr = 0;
    CVIXMLElem                      *elemPtr = (CVIXMLElem *)elem;
    CVIXMLAttr                      *attrPtr = NULL;
    MSXMLObj_IXMLDOMNode            foundAttr = 0;
    MSXMLObj_IXMLDOMNamedNodeMap    attrList = 0;
    HRESULT                         __result = S_OK;
    int                             error, locked = 0;

    if((elemPtr == NULL) || (attr == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *attr = 0;

    LOCK(elemPtr->lock, locked);

    attrPtr = (CVIXMLAttr *)calloc(1, sizeof(CVIXMLAttr));
    if(attrPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }
    libErrChk(CmtNewLock(0, 0, &attrPtr->lock));

    __caErrChk(MSXML_IXMLDOMNodeGetattributes(elemPtr->elem, NULL, &attrList));
    __caErrChk(MSXML_IXMLDOMNamedNodeMapGetlength(attrList, NULL, &numAttr));
    if((index > numAttr - 1) || (index < -1))
    {
        __caErrChk(E_CVIXML_INVALID_INDEX);
    }

    if(index == -1)
    {
        index = numAttr - 1;
    }
    __caErrChk(MSXML_IXMLDOMNamedNodeMapGetitem(attrList, NULL, index, &foundAttr));

    // Set up the document's structure information.
    __caErrChk(CA_DuplicateObjHandle(elemPtr->docHdl, 0, &attrPtr->docHdl));
    __caErrChk(CA_DuplicateObjHandle(elemPtr->elem, 0, &attrPtr->elemHdl));
    // Cannot fail after this.
    attrPtr->attr = foundAttr;
    *attr = (CVIXMLAttribute)attrPtr;
    foundAttr = 0;
    attrPtr = NULL;

Error:
    CA_DiscardObjHandle(foundAttr);
    CA_DiscardObjHandle(attrList);
    CVIXMLDiscardAttribute ((CVIXMLAttribute)attrPtr);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose: This function gets the attribute of an element by its name.
// Parameters: elem - Handle of the element object.
//          name - Name of the attribute to find.
//          attr - Handle of the attribute element object.
// Return: Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetAttributeByName (CVIXMLElement elem, const char *name,
                                               CVIXMLAttribute *attr)
{
    CVIXMLElem                      *elemPtr = (CVIXMLElem *)elem;
    CVIXMLAttr                      *attrPtr = NULL;
    MSXMLObj_IXMLDOMNode            foundAttr = 0;
    MSXMLObj_IXMLDOMNamedNodeMap    attrList = 0;
    HRESULT                         __result = S_OK;
    int                             error, locked = 0;

    if((elemPtr == NULL) || (attr == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    if((name == NULL) || (name[0] == '\0'))
    {
        __caErrChk(E_INVALIDARG);
    }
    *attr = 0;

    LOCK(elemPtr->lock, locked);

    attrPtr = (CVIXMLAttr *)calloc(1, sizeof(CVIXMLAttr));
    if(attrPtr == NULL)
    {
        __caErrChk(E_OUTOFMEMORY);
    }
    libErrChk(CmtNewLock(0, 0, &attrPtr->lock));

    __caErrChk(MSXML_IXMLDOMNodeGetattributes(elemPtr->elem, NULL, &attrList));
    __caErrChk(MSXML_IXMLDOMNamedNodeMapgetNamedItem(attrList, NULL, name, &foundAttr));

    if (foundAttr) // Then we found the requested attribute
    {
        // Set up the document's structure information.
        __caErrChk(CA_DuplicateObjHandle(elemPtr->docHdl, 0, &attrPtr->docHdl));
        __caErrChk(CA_DuplicateObjHandle(elemPtr->elem, 0, &attrPtr->elemHdl));
        // Cannot fail after this.
        attrPtr->attr = foundAttr;
        *attr = (CVIXMLAttribute)attrPtr;
        foundAttr = 0;
        attrPtr = NULL;
    }

Error:
    if(FAILED(__result))
    {
        __result = S_FALSE;
        *attr = 0;
    }
    CA_DiscardObjHandle(foundAttr);
    CA_DiscardObjHandle(attrList);
    CVIXMLDiscardAttribute ((CVIXMLAttribute)attrPtr);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the attribute to the right of the
//              current attribute.
// Parameters:  attr - Handle of the attribute object.
//              sibAttr - Handle of the sibling attribute object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetNextAttribute (CVIXMLAttribute attr,
                                             CVIXMLAttribute *sibAttr)
{
    int                             index = 0;
    CVIXMLAttr                      *attrPtr = (CVIXMLAttr *)attr;
    CVIXMLAttr                      *sibAttrPtr = NULL;
    MSXMLObj_IXMLDOMNode            rightSib = 0;
    MSXMLObj_IXMLDOMNamedNodeMap    attrList = 0;
    HRESULT                         __result = S_OK;
    int                             error, locked = 0;

    if((attrPtr == NULL) || (sibAttr == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *sibAttr = 0;

    LOCK(attrPtr->lock, locked);

    if(attrPtr->elemHdl == 0)
        goto Error;

    __caErrChk(MSXML_IXMLDOMNodeGetattributes(attrPtr->elemHdl, NULL, &attrList));
    __caErrChk(GetAttributeIndex(attrList, attrPtr->attr, &index));
    if(index == -1)
    {
        __caErrChk(E_INVALIDARG);
    }
    __caErrChk(MSXML_IXMLDOMNamedNodeMapGetitem(attrList, NULL, index + 1, &rightSib));
    if(rightSib)
    {
        sibAttrPtr = (CVIXMLAttr *)calloc(1, sizeof(CVIXMLAttr));
        if(sibAttrPtr == NULL)
        {
            __caErrChk(E_OUTOFMEMORY);
        }
        libErrChk(CmtNewLock(0, 0, &sibAttrPtr->lock));
        // Set up the document's structure information.
        __caErrChk(CA_DuplicateObjHandle(attrPtr->docHdl, 0, &sibAttrPtr->docHdl));
        __caErrChk(CA_DuplicateObjHandle(attrPtr->elemHdl, 0, &sibAttrPtr->elemHdl));
        // Cannot fail after this.
        sibAttrPtr->attr = rightSib;
        rightSib = 0;
        *sibAttr = (CVIXMLAttribute)sibAttrPtr;
        sibAttrPtr = NULL;
    }

Error:
    CA_DiscardObjHandle(rightSib);
    CA_DiscardObjHandle(attrList);
    CVIXMLDiscardAttribute((CVIXMLAttribute)sibAttrPtr);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose: This function gets the attribute to the left of the current attribute.
// Parameters: attr - Handle of the attribute object.
//          sibAttr - Handle of the sibling attribute object.
// Return: Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetPreviousAttribute (CVIXMLAttribute attr,
                                                 CVIXMLAttribute *sibAttr)
{
    int                             index = 0;
    CVIXMLAttr                      *attrPtr = (CVIXMLAttr *)attr;
    CVIXMLAttr                      *sibAttrPtr = NULL;
    MSXMLObj_IXMLDOMNode            leftSib = 0;
    MSXMLObj_IXMLDOMNamedNodeMap    attrList = 0;
    HRESULT                         __result = S_OK;
    int                             error, locked = 0;

    if((attrPtr == NULL) || (sibAttr == NULL))
    {
        __caErrChk(E_HANDLE);
    }
    *sibAttr = 0;

    LOCK(attrPtr->lock, locked);

    if(attrPtr->elemHdl == 0)
        goto Error;

    __caErrChk(MSXML_IXMLDOMNodeGetattributes(attrPtr->elemHdl, NULL, &attrList));
    __caErrChk(GetAttributeIndex(attrList, attrPtr->attr, &index));
    if(index == -1)
    {
        __caErrChk(E_INVALIDARG);
    }
    __caErrChk(MSXML_IXMLDOMNamedNodeMapGetitem(attrList, NULL, index - 1, &leftSib));
    if(leftSib)
    {
        sibAttrPtr = (CVIXMLAttr *)calloc(1, sizeof(CVIXMLAttr));
        if(sibAttrPtr == NULL)
        {
            __caErrChk(E_OUTOFMEMORY);
        }
        libErrChk(CmtNewLock(0, 0, &sibAttrPtr->lock));
        // Set up the document's structure information.
        __caErrChk(CA_DuplicateObjHandle(attrPtr->docHdl, 0, &sibAttrPtr->docHdl));
        __caErrChk(CA_DuplicateObjHandle(attrPtr->elemHdl, 0, &sibAttrPtr->elemHdl));
        // Cannot fail after this.
        sibAttrPtr->attr = leftSib;
        leftSib = 0;
        *sibAttr = (CVIXMLAttribute)sibAttrPtr;
        sibAttrPtr = NULL;
    }

Error:
    CA_DiscardObjHandle(leftSib);
    CA_DiscardObjHandle(attrList);
    CVIXMLDiscardAttribute((CVIXMLAttribute)sibAttrPtr);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function adds an attribute to an element.
// Parameters:  elem - Handle of an element object.
//              name - Name of the new attribute.
//              value - Value of the new attribute.
//              attr - Handle of an attribute object.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLAddAttribute (CVIXMLElement elem, const char *name,
                                         const char *value)
{
    VARIANT                     tempVar;
    CVIXMLElem                  *elemPtr = (CVIXMLElem *)elem;
    MSXMLObj_IXMLDOMAttribute   newAttr = 0;
    MSXMLObj_IXMLDOMAttribute   newAttrAdded = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&tempVar));
    if(elemPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if((name == NULL) || (name[0] == '\0') || (value == NULL))
    {
        __caErrChk(E_INVALIDARG);
    }

    LOCK(elemPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMDocumentcreateAttribute(elemPtr->docHdl, NULL,
                                                    name, &newAttr));
    __caErrChk(CA_VariantSetCString(&tempVar, value));
    __caErrChk(MSXML_IXMLDOMAttributeSetvalue(newAttr, NULL, tempVar));
    __caErrChk(MSXML_IXMLDOMElementsetAttributeNode(elemPtr->elem, NULL,
                                                    newAttr, &newAttrAdded));

Error:
    CA_VariantClear (&tempVar);
    CA_DiscardObjHandle(newAttr);
    CA_DiscardObjHandle(newAttrAdded);
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function sets the value of an attribute.
// Parameters:  attr - Handle of an attribute object.
//              value - Value to set the attribute to.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLSetAttributeValue (CVIXMLAttribute attr, const char *value)
{
    VARIANT     tempVar;
    CVIXMLAttr  *attrPtr = (CVIXMLAttr *)attr;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&tempVar));
    if(attrPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(value == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }

    LOCK(attrPtr->lock, locked);

    __caErrChk(CA_VariantSetCString(&tempVar, value));
    __caErrChk(MSXML_IXMLDOMAttributeSetvalue(attrPtr->attr, NULL, tempVar));

Error:
    CA_VariantClear(&tempVar);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the length of the name of an attribute.
// Parameters:  attr - Handle of an attribute object.
//              length - Length of the name of the attribute.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetAttributeNameLength (CVIXMLAttribute attr, int *length)
{
    char        *name = NULL;
    CVIXMLAttr  *attrPtr = (CVIXMLAttr *)attr;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if(attrPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(length == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *length = 0;

    LOCK(attrPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMAttributeGetname(attrPtr->attr, NULL, &name));
    *length = strlen(name);

Error:
    CA_FreeMemory(name);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the name of an attribute.
// Parameters:  attr - Handle of an attribute object.
//              name - Name of the attribute.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetAttributeName (CVIXMLAttribute attr, char *name)
{
    char        *tempName = NULL;
    CVIXMLAttr  *attrPtr = (CVIXMLAttr *)attr;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if(attrPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(name == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *name = '\0';

    LOCK(attrPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMAttributeGetname(attrPtr->attr, NULL, &tempName));
    _mbscpy((unsigned char*)name, (const unsigned char*)tempName);

Error:
    CA_FreeMemory(tempName);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the length of the value of an attribute.
// Parameters:  attr - Handle of an attribute object.
//              length - Length of the value of the attribute.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetAttributeValueLength (CVIXMLAttribute attr, int *length)
{
    char        *value = NULL;
    VARIANT     tempVal;
    CVIXMLAttr  *attrPtr = (CVIXMLAttr *)attr;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&tempVal));
    if(attrPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(length == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *length = 0;

    LOCK(attrPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMAttributeGetnodeValue(attrPtr->attr, NULL, &tempVal));
    __caErrChk(CA_VariantConvertToType(&tempVal, CAVT_CSTRING, &value));
    *length = strlen(value);

Error:
    CA_VariantClear(&tempVal);
    CA_FreeMemory(value);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the value of an attribute.
// Parameters:  attr - Handle of an attribute object.
//              value - Value of the attribute.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetAttributeValue (CVIXMLAttribute attr, char *value)
{
    char        *tempValue = NULL;
    VARIANT     tempVar;
    CVIXMLAttr  *attrPtr = (CVIXMLAttr *)attr;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    __caErrChk(CA_VariantSetEmpty(&tempVar));
    if(attrPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    if(value == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *value = '\0';

    LOCK(attrPtr->lock, locked);

    __caErrChk(MSXML_IXMLDOMAttributeGetnodeValue(attrPtr->attr, NULL, &tempVar));
    __caErrChk(CA_VariantConvertToType(&tempVar, CAVT_CSTRING, &tempValue));
    _mbscpy((unsigned char*)value, (const unsigned char*)tempValue);

Error:
    CA_VariantClear(&tempVar);
    CA_FreeMemory(tempValue);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function removes an attribute from an element.
// Parameters:  elem - Handle of an element object.
//              attr - Handle of the attribute object.
//              removedAttr - The removed attribute.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLRemoveAttribute (CVIXMLAttribute attr)
{
    CVIXMLAttr                  *attrPtr = (CVIXMLAttr *)attr;
    MSXMLObj_IXMLDOMAttribute   oldAttr = 0;
    HRESULT                     __result = S_OK;
    int                         error, locked = 0;

    if(attrPtr == NULL)
    {
        __caErrChk(E_HANDLE);
    }

    LOCK(attrPtr->lock, locked);

    if(attrPtr->elemHdl)
    {
        __caErrChk(MSXML_IXMLDOMElementremoveAttributeNode(attrPtr->elemHdl,
                                                           NULL, attrPtr->attr,
                                                           &oldAttr));
        CA_DiscardObjHandle (attrPtr->elemHdl);
        attrPtr->elemHdl = 0;
    }

Error:
    CA_DiscardObjHandle(oldAttr);
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function discards an XML attribute.
// Parameters:  attr - Handle of the attribute object.
// Return:      None.
//-----------------------------------------------------------------------------
void CVIFUNC CVIXMLDiscardAttribute (CVIXMLAttribute attr)
{
    CVIXMLAttr *attrPtr = (CVIXMLAttr *)attr;

    if(attrPtr)
    {
        int lock = attrPtr->lock, locked = 0;

        if(lock && CmtGetLock(lock) >= 0)
            locked = 1;
        attrPtr->lock = 0;
        CA_DiscardObjHandle(attrPtr->attr);
        CA_DiscardObjHandle(attrPtr->elemHdl);
        CA_DiscardObjHandle(attrPtr->docHdl);
        free(attrPtr);
        if(locked)
            CmtReleaseLock(lock);
        if(lock)
            CmtDiscardLock(lock);
    }
}

//-----------------------------------------------------------------------------
// ERROR FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose:     This function returns an error message based on the error number.
// Parameters:  error - The number of the error.
//              buffer - Holds the error message.
//              bufferLength - Size of the buffer needed.
// Return:      None.
//-----------------------------------------------------------------------------
void CVIFUNC CVIXMLGetErrorString (CVIXMLStatus error, char *buffer, int bufferLength)
{
    if (buffer != NULL && bufferLength > 0)
    {
        switch((unsigned int)error)
        {
            case E_CVIXML_INVALID_INDEX:
                _mbsnbcpy((unsigned char*)buffer,
                    (const unsigned char*)"The index passed in is invalid.",
                    (size_t)(bufferLength - 1));
                buffer[bufferLength - 1] = '\0';
                break;
            case E_CVIXML_INVALID_PATH:
                _mbsnbcpy((unsigned char*)buffer,
                    (const unsigned char*)"The path passed in is invalid.",
                    (size_t)(bufferLength - 1));
                buffer[bufferLength - 1] = '\0';
                break;
            case E_CVIXML_INVALID_DOCUMENT:
                _mbsnbcpy((unsigned char*)buffer,
                    (const unsigned char*)"The XML document is invalid.",
                    (size_t)(bufferLength - 1));
                buffer[bufferLength - 1] = '\0';
                break;
            default:
                CA_GetAutomationErrorString(error, buffer, bufferLength);
                break;
        }
    }
}

//-----------------------------------------------------------------------------
// Purpose:     This function gets the ERRORINFO last set
// Parameters:  errorInfo - address of ERRORINFO structure to fill
// Return:      Returns S_OK if error info is present, else S_FALSE, or error
//-----------------------------------------------------------------------------
HRESULT CVIFUNC CVIXMLGetLastErrorInfo (ERRORINFO *errorInfo)
{
    HRESULT     hr;
    int         errorInfoPresent;

    hr = CA_FillErrorInfo (DISP_E_EXCEPTION, errorInfo, &errorInfoPresent);
    if (SUCCEEDED (hr) && !errorInfoPresent)
        hr = S_FALSE;
    return hr;
}

//-----------------------------------------------------------------------------
// HANDLE FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose:     This function returns the DOM document handle from one of our
//              document objects.
// Parameters:  doc - Handle of the document object.
//              xmlHdl - DOM document handle.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetDocumentActiveXHandle (CVIXMLDocument doc,
                                                     MSXMLObj_IXMLDOMDocument *xmlHdl)
{
    CVIXMLDoc   *docPtr = (CVIXMLDoc *)doc;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if((docPtr == NULL) || (xmlHdl == NULL))
    {
        __caErrChk(E_HANDLE);
    }

    LOCK(docPtr->lock, locked);

    __caErrChk(CA_DuplicateObjHandle(docPtr->docHdl, 0, xmlHdl));

Error:
    UNLOCK(docPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function returns the DOM element handle from one of
//              our element objects.
// Parameters:  elem - Handle of the element object.
//              elemHdl - DOM element handle.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetElementActiveXHandle (CVIXMLElement elem,
                                                    MSXMLObj_IXMLDOMElement *elemHdl)
{
    CVIXMLElem  *elemPtr = (CVIXMLElem *)elem;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if((elemPtr == NULL) || (elemHdl == NULL))
    {
        __caErrChk(E_HANDLE);
    }

    LOCK(elemPtr->lock, locked);

    __caErrChk(CA_DuplicateObjHandle(elemPtr->elem, 0, elemHdl));

Error:
    UNLOCK(elemPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function returns the DOM attribute handle from one of our
//              attribute objects.
// Parameters:  attr - Handle of the attribute object.
//              attrHdl - DOM attribute handle.
// Return:      Error code.
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetAttributeActiveXHandle (CVIXMLAttribute attr,
                                                      MSXMLObj_IXMLDOMAttribute *attrHdl)
{
    CVIXMLAttr  *attrPtr = (CVIXMLAttr *)attr;
    HRESULT     __result = S_OK;
    int         error, locked = 0;

    if((attrPtr == NULL) || (attrHdl == NULL))
    {
        __caErrChk(E_HANDLE);
    }

    LOCK(attrPtr->lock, locked);

    __caErrChk(CA_DuplicateObjHandle(attrPtr->attr, 0, attrHdl));

Error:
    UNLOCK(attrPtr->lock, locked);

    return __result;
}

//-----------------------------------------------------------------------------
// INTERNAL FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose:     This function finds the nth element in a list of nodes.
// Parameters:  parent - The node whose children we will search.
//              index - The index where the child is at.
//              foundElem - Handle of the element we find.
// Return:      Error code.
//-----------------------------------------------------------------------------
static CVIXMLStatus FindNthElement (MSXMLObj_IXMLDOMNode parent, int index,
                                    MSXMLObj_IXMLDOMNode *foundElem)
{
    int                         count = 0;
    MSXMLObj_IXMLDOMNode        tempElem = 0;
    MSXMLType_DOMNodeType       tempType = 0;
    MSXMLObj_IXMLDOMNodeList    childList = 0;
    HRESULT                     __result = S_OK;

    if(foundElem == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    *foundElem = 0;

    __caErrChk(MSXML_IXMLDOMNodeGetchildNodes(parent, NULL, &childList));
    __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &tempElem));
    while(tempElem)
    {
        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(tempElem, NULL, &tempType));
        if(tempType == MSXMLConst_NODE_ELEMENT)
        {
            if(count == index)
            {
                *foundElem = tempElem;
                tempElem = 0;
                break;
            }
            count++;
        }
        CA_DiscardObjHandle(tempElem);
        tempElem = 0;
        __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &tempElem));
    }

Error:
    CA_DiscardObjHandle(tempElem);
    CA_DiscardObjHandle(childList);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function finds the nth attribute in a list of attributes.
// Parameters:  elem - The node whose attributes we will search.
//              index - The index where the child is at.
//              attr - Handle of the element we find.
// Return:      Error code.
//-----------------------------------------------------------------------------
static CVIXMLStatus GetAttributeIndex (MSXMLObj_IXMLDOMNamedNodeMap list,
                                       MSXMLObj_IXMLDOMAttribute attr, int *index)
{
    int                     i = 0;
    long                    length = 0;
    char                    *name = NULL;
    char                    *tempName = NULL;
    MSXMLObj_IXMLDOMNode    attrSib = 0;
    HRESULT                 __result = S_OK;

    if(index == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *index = -1;

    __caErrChk(MSXML_IXMLDOMAttributeGetnodeName(attr, NULL, &name));
    __caErrChk(MSXML_IXMLDOMNamedNodeMapGetlength(list, NULL, &length));
    for(i = 0; i < length; i++)
    {
        __caErrChk(MSXML_IXMLDOMNamedNodeMapGetitem(list, NULL, i, &attrSib));
        __caErrChk(MSXML_IXMLDOMAttributeGetname(attrSib, NULL, &tempName));
        if(!_mbscmp((const unsigned char*)name, (const unsigned char*)tempName))
        {
                *index = i;
                break;
        }
        CA_DiscardObjHandle(attrSib);
        attrSib = 0;
        CA_FreeMemory(tempName);
        tempName = NULL;
    }

Error:
    CA_DiscardObjHandle(attrSib);
    CA_FreeMemory(name);
    CA_FreeMemory(tempName);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function returns the text node of an element.
// Parameters:  elem - The node whose children we will search.
//              foundElem - Handle of the text node we find.
// Return:      Error code.
//-----------------------------------------------------------------------------
static CVIXMLStatus GetNodeText(MSXMLObj_IXMLDOMNode elem,
                                MSXMLObj_IXMLDOMText *foundElem)
{
    MSXMLObj_IXMLDOMNode        tempElem = 0;
    MSXMLType_DOMNodeType       tempType = 0;
    MSXMLObj_IXMLDOMNodeList    childList = 0;
    HRESULT                     __result = S_OK;

    if(foundElem == NULL)
    {
        __caErrChk(E_HANDLE);
    }
    *foundElem = 0;

    __caErrChk(MSXML_IXMLDOMNodeGetchildNodes(elem, NULL, &childList));
    __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &tempElem));
    while(tempElem)
    {
        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(tempElem, NULL, &tempType));
        if(tempType == MSXMLConst_NODE_TEXT)
        {
            *foundElem = tempElem;
            tempElem = 0;
            break;
        }
        CA_DiscardObjHandle(tempElem);
        tempElem = 0;
        __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &tempElem));
    }

Error:
    CA_DiscardObjHandle(tempElem);
    CA_DiscardObjHandle(childList);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     This function returns the number of elements a node has.
// Parameters:  elem - The node whose children we will search.
//              numChildren - Number of children the node has.
// Return:      Error code.
//-----------------------------------------------------------------------------
static CVIXMLStatus GetNumElements(MSXMLObj_IXMLDOMNode elem, int *numChildren)
{
    int                         count = 0;
    MSXMLObj_IXMLDOMNode        tempElem = 0;
    MSXMLType_DOMNodeType       tempType = 0;
    MSXMLObj_IXMLDOMNodeList    childList = 0;
    HRESULT                     __result = S_OK;

    if(numChildren == NULL)
    {
        __caErrChk(E_INVALIDARG);
    }
    *numChildren = 0;

    __caErrChk(MSXML_IXMLDOMNodeGetchildNodes(elem, NULL, &childList));
    __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &tempElem));
    while(tempElem)
    {
        __caErrChk(MSXML_IXMLDOMNodeGetnodeType(tempElem, NULL, &tempType));
        if(tempType == MSXMLConst_NODE_ELEMENT)
        {
            count++;
        }
        CA_DiscardObjHandle(tempElem);
        tempElem = 0;
        __caErrChk(MSXML_IXMLDOMNodeListnextNode(childList, NULL, &tempElem));
    }
    *numChildren = count;

Error:
    CA_DiscardObjHandle(tempElem);
    CA_DiscardObjHandle(childList);

    return __result;
}

//-----------------------------------------------------------------------------
// Purpose:     Formats the fullPath file
//-----------------------------------------------------------------------------
void CVIFUNC CVIXMLLineFormatXMLDocument (const char *fullPath)
{
    int                 error = 0;
    FILE                *rf = NULL, *wf = NULL;
    char                tempDir [MAX_PATH];
    char                tempFile [MAX_PATH];
    int                 tmpFileCreated = 0;

    assert (fullPath && fullPath[0]);

    // Get temp path
    error = GetTempPath (sizeof (tempDir), tempDir);
    assert (error > 0 && error <= sizeof (tempDir));
    // Create temp file
    if (GetTempFileName (tempDir, TMP_PREFIX, 0, tempFile)) {
        tmpFileCreated = 1;

        // Open files
        rf = fopen (fullPath, "r");
        nullChk ((void*)rf);
        wf = fopen (tempFile, "w");
        nullChk ((void*)wf);

        // Format the XML
        errChk (FormatNode (rf, wf));

        // Save files
        eofChk (fclose (rf));
        rf = NULL;
        eofChk (fclose (wf));
        wf = NULL;

        // Copy formatted XML over the original contents
        errChk (CopyFile (tempFile, (char *)fullPath));
    }

Error:
    if (rf)
        fclose (rf);
    if (wf)
        fclose (wf);
    if (tmpFileCreated)
        DeleteFile (tempFile);
    assert (error == 0);    // For debug purposes only
    return;
}

//-----------------------------------------------------------------------------
// On Input - File ptrs should be pointing to TAG_OPEN of an XML tag
//            i.e., ^<Foo>, ^</Foo>, or ^<Foo/>.
// Lets call
//      <Foo>   - begin tag of a non-empty node
//      </Foo>  - end tag of a non-empty node
//      </Foo>  - empty node tag
//-----------------------------------------------------------------------------
static int FormatNode (FILE *rf, FILE *wf)
{
    int         error = 0;
    int         c;
    long        pos = 0;
    int         isEndTag;
    int         hasValue;

    assert (rf && wf);

    while (1) { // Loop will terminate on end of file (eofChk)

        errChk (FindTagOpen (rf, wf));
        isEndTag = (error == kEndFound);        // Check if end tag of a non-empty node

        errChk (FindTagClose (rf, wf));

        if (isEndTag) {
            assert (error != kEndFound);
            eofChk (fputcMb (NEWLINE, wf));
        }
        else if (error == kEndFound) {          // Tag of an empty node
            eofChk (fputcMb (NEWLINE, wf));
        }
        else {                                  // Inside non-empty node.
            hasValue = 0;
            while (1) {
                errChk (pos = ftell (rf));
                eofChk (c = fgetcMb (rf));
                if (c == TAG_OPEN)
                    break;
                eofChk (fputcMb (c, wf));       // Node Value
                hasValue = 1;
            }
            if (0 != fseek (rf, pos, SEEK_SET)) // Reset read-file position to TAG_OPEN
                goto Error;
            if (!hasValue) {
                eofChk (fputcMb (NEWLINE, wf));
            }
        }
    }

Error:
    if (error < 0) {
        if (0 != (error = -abs (ferror (rf))))
            return error;
        if (0 != (error = -abs (ferror (wf))))
            return error;
    }
    return error;
}

//-----------------------------------------------------------------------------
// Copies till it finds a TAG_OPEN. It then copies the TAG_OPEN and the next byte.
//-----------------------------------------------------------------------------
static int FindTagOpen (FILE *rf, FILE *wf)
{
    int         error = 0;
    int         c;
    int         retval = 0;

    assert (rf && wf);

    do {
        eofChk (c = fgetcMb (rf));
        eofChk (fputcMb (c, wf));
    } while (c != TAG_OPEN);

    eofChk (c = fgetcMb (rf));
    if (c == TAG_END) {
        retval = kEndFound;         // We are at an end tag </^xyz>
    }
    else {
        assert (c != TAG_CLOSE);    // Cannot have empty tags, i.e. <>
        retval = kNoEndFound;       // We are at a begin tag <x^yz>
    }
    eofChk (fputcMb (c, wf));

Error:
    return (error < 0) ? error : retval;
}

//-----------------------------------------------------------------------------
// On Input -   File ptrs should be after a TAG_OPEN, i.e. <^
//              If this is an end tag, then should be after the TAG_END, i.e. </^
// Copies till TAG_CLOSE is found. TAG_CLOSE is also copied.
//-----------------------------------------------------------------------------
static int FindTagClose (FILE *rf, FILE *wf)
{
    int     c;
    int     error = 0;
    int     retval = 0;
    int     closeFound = 0;

    assert (rf && wf);

    do {
        eofChk (c = fgetcMb (rf));
        assert (c != TAG_OPEN);
        eofChk (fputcMb (c, wf));
        if (c == TAG_END) {
            eofChk (c = fgetcMb (rf));
            eofChk (fputcMb (c, wf));
            if (c == TAG_CLOSE) {
                closeFound = 1;
                retval = kEndFound;     // i.e., xyz/>^
            }
        }
        else if (c == TAG_CLOSE) {
            retval = kNoEndFound;   // i.e., xyz>^
            closeFound = 1;
        }
    } while (!closeFound);

Error:
    return (error < 0) ? error : retval;
}

//-----------------------------------------------------------------------------
// MB version of fgetc
// On input, the file position should be on a character boundary (cannot point
//  to trail byte).
//-----------------------------------------------------------------------------
static int fgetcMb (FILE *pf)
{
    int c = fgetc (pf);

    if (c != EOF && _ismbblead (c)) {
        int trail = fgetc (pf);
        return (trail == EOF) ? EOF : (((c << 8) & 0xff00) | (trail & 0x00ff));
    }
    return c;
}

//-----------------------------------------------------------------------------
// MB version of fputc
// On input, the file position should be on a character boundary (cannot point
//  to trail byte).
// On a non-mb system only the lower byte in c will be put.
// NOTE:    This function can fail after putting the lead byte, but before
//          putting the trail byte. So ALWAYS check the error returned, and
//          take appropriate action on failure.
//-----------------------------------------------------------------------------
static int fputcMb (int c, FILE *pf)
{
    int leadByte = (c & 0xff00) >> 8;
    if (_ismbblead (leadByte))
        if (EOF == fputc (leadByte, pf))   // Put lead byte
            return EOF;
    return fputc (c & 0x00ff, pf);         // Put trail byte or single byte
}

