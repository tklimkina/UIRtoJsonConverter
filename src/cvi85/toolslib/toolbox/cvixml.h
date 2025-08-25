/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       cvixml.h                                                      */
/* Purpose:     provides functions to create and edit XML documents           */
/*                                                                            */
/*============================================================================*/

#ifndef _CVIXML_HEADER_
#define _CVIXML_HEADER_

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "msxmldom.h"
#include <toolbox.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Constants and macros
//-----------------------------------------------------------------------------
#define E_CVIXML_INVALID_INDEX      (E_CVIAUTO_USER_ERROR_BASE + (0x100))
#define E_CVIXML_INVALID_PATH       (E_CVIAUTO_USER_ERROR_BASE + (0x101))
#define E_CVIXML_INVALID_DOCUMENT   (E_CVIAUTO_USER_ERROR_BASE + (0x102))

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef int         CVIXMLDocument;
typedef int         CVIXMLElement;
typedef int         CVIXMLAttribute;
typedef HRESULT     CVIXMLStatus;

//-----------------------------------------------------------------------------
// Document functions
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLLoadDocument (const char *fullPath,
                                         CVIXMLDocument *doc);
CVIXMLStatus CVIFUNC CVIXMLNewDocument (const char *rootElementName,
                                        CVIXMLDocument *doc);
CVIXMLStatus CVIFUNC CVIXMLSaveDocument (CVIXMLDocument doc,
                                         int formatDoc,
                                         const char *fullPath);
void CVIFUNC CVIXMLDiscardDocument (CVIXMLDocument doc);
CVIXMLStatus CVIFUNC CVIXMLGetRootElement (CVIXMLDocument doc,
                                           CVIXMLElement *elem);
void CVIFUNC CVIXMLLineFormatXMLDocument (const char *fullPath);

//-----------------------------------------------------------------------------
// Element functions
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLNewElement (CVIXMLElement parent, int index,
                                       const char *tag, CVIXMLElement *elem);
CVIXMLStatus CVIFUNC CVIXMLSetElementValue (CVIXMLElement elem,
                                            const char *value);
CVIXMLStatus CVIFUNC CVIXMLGetElementTagLength (CVIXMLElement elem,
                                                int *length);
CVIXMLStatus CVIFUNC CVIXMLGetElementTag (CVIXMLElement elem, char tag[]);
CVIXMLStatus CVIFUNC CVIXMLGetElementValueLength (CVIXMLElement elem,
                                                  int *length);
CVIXMLStatus CVIFUNC CVIXMLGetElementValue (CVIXMLElement elem, char value[]);
CVIXMLStatus CVIFUNC CVIXMLCopyElement (CVIXMLElement srcElem,
                                        int copyChildElements,
                                        CVIXMLElement destParent,
                                        int index,
                                        CVIXMLElement *destElem);
CVIXMLStatus CVIFUNC CVIXMLFindElements (CVIXMLElement parent,
                                         const char *tag, ListType *elemList);
CVIXMLStatus CVIFUNC CVIXMLRemoveElement (CVIXMLElement elem);
void CVIFUNC CVIXMLDiscardElement (CVIXMLElement elem);

//-----------------------------------------------------------------------------
// Child Element functions
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetNumChildElements (CVIXMLElement parent,
                                                int *numChildren);
CVIXMLStatus CVIFUNC CVIXMLGetChildElementByIndex (CVIXMLElement parent,
                                                   int index,
                                                   CVIXMLElement *child);
CVIXMLStatus CVIFUNC CVIXMLGetChildElementByTag (CVIXMLElement parent,
                                                 const char *tag,
                                                 CVIXMLElement *child);
CVIXMLStatus CVIFUNC CVIXMLGetNextElement (CVIXMLElement elem,
                                           CVIXMLElement *sibElem);
CVIXMLStatus CVIFUNC CVIXMLGetPreviousElement (CVIXMLElement elem,
                                               CVIXMLElement *sibElem);
CVIXMLStatus CVIFUNC CVIXMLGetParentElement (CVIXMLElement child,
                                             CVIXMLElement *parent);

//-----------------------------------------------------------------------------
// Attribute functions
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLAddAttribute (CVIXMLElement elem, const char *name,
                                         const char *value);
CVIXMLStatus CVIFUNC CVIXMLGetNumAttributes (CVIXMLElement elem, int *numAttrs);
CVIXMLStatus CVIFUNC CVIXMLGetAttributeByIndex (CVIXMLElement elem, int index,
                                                CVIXMLAttribute *attr);
CVIXMLStatus CVIFUNC CVIXMLGetAttributeByName (CVIXMLElement elem,
                                               const char *name,
                                               CVIXMLAttribute *attr);
CVIXMLStatus CVIFUNC CVIXMLGetNextAttribute (CVIXMLAttribute attr,
                                             CVIXMLAttribute *sibAttr);
CVIXMLStatus CVIFUNC CVIXMLGetPreviousAttribute (CVIXMLAttribute attr,
                                                 CVIXMLAttribute *sibAttr);
CVIXMLStatus CVIFUNC CVIXMLSetAttributeValue (CVIXMLAttribute attr,
                                              const char *value);
CVIXMLStatus CVIFUNC CVIXMLGetAttributeNameLength (CVIXMLAttribute attr,
                                                   int *length);
CVIXMLStatus CVIFUNC CVIXMLGetAttributeName (CVIXMLAttribute attr, char name[]);
CVIXMLStatus CVIFUNC CVIXMLGetAttributeValueLength (CVIXMLAttribute attr,
                                                    int *length);
CVIXMLStatus CVIFUNC CVIXMLGetAttributeValue (CVIXMLAttribute attr,
                                              char value[]);
CVIXMLStatus CVIFUNC CVIXMLRemoveAttribute (CVIXMLAttribute attr);
void CVIFUNC CVIXMLDiscardAttribute (CVIXMLAttribute attr);

//-----------------------------------------------------------------------------
// Error functions
//-----------------------------------------------------------------------------
void CVIFUNC CVIXMLGetErrorString (CVIXMLStatus error, char buffer[],
                                   int bufferLength);
HRESULT CVIFUNC CVIXMLGetLastErrorInfo (ERRORINFO *errorInfo);

//-----------------------------------------------------------------------------
// Handle functions
//-----------------------------------------------------------------------------
CVIXMLStatus CVIFUNC CVIXMLGetDocumentActiveXHandle (CVIXMLDocument doc,
                                                     MSXMLObj_IXMLDOMDocument *xmlHdl);
CVIXMLStatus CVIFUNC CVIXMLGetElementActiveXHandle (CVIXMLElement elem,
                                                    MSXMLObj_IXMLDOMElement *elemHdl);
CVIXMLStatus CVIFUNC CVIXMLGetAttributeActiveXHandle (CVIXMLAttribute attr,
                                                      MSXMLObj_IXMLDOMAttribute *attrHdl);

#ifdef __cplusplus
}
#endif

#endif // _CVIXML_HEADER_
