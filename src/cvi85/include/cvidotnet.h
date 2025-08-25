#ifndef _CVIDOTNET_H
#define _CVIDOTNET_H

#if defined(INCLUDE_AFTER_WINDOWS_H) && !defined(_INC_WINDOWS)
#error  This header must be included before utility.h and formatio.h
#error  because this header includes Windows SDK headers.
#endif

#include <ole2.h>
#include <cvidef.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(8)

#if defined(_CVI_) && !defined(__TPC__)
#pragma EnableLibraryRuntimeChecking
#endif

typedef void *  CDotNetHandle;
typedef int     CDotNetAssemblyHandle;

enum {
    CDotNetErrorBegin                       = -6599,
    CDotNetInternalError                    = CDotNetErrorBegin,
    CDotNetIncorrectDLLVersion,
    CDotNetCouldNotLoadDLL,
    CDotNetCouldNotCreateMutex,
    CDotNetInvalidInputError,
    CDotNetInitializationError,

    CDotNetExceptionError,
    CDotNetTypeMismatchError,
    CDotNetOutOfMemoryError,
    CDotNetSecurityError,
    CDotNetNotSupportedError,
    CDotNetMethodAccessError,
    CDotNetMemberAccessError,
    CDotNetInvalidCOMObjectError,
    CDotNetMissingMethodError,
    CDotNetMissingMemberError,
    CDotNetCOMError,
    CDotNetRemotingError,

    CDotNetInvalidPathError,
    CDotNetCouldNotLoadLibraryAssemblyError,
    CDotNetCouldNotLoadAssemblyError,
    CDotNetCouldNotGetTypeInfoError,
    CDotNetInvalidObjectHandleError,
    CDotNetNullPointerError,
    CDotNetUnknownTypeError,
    CDotNetInvalidNumParametersError,
    CDotNetInvalidParameterTypeIdsError,
    CDotNetInvalidParametersError,

    CDotNetInvocationTargetError,
    CDotNetInvocationError,
    CDotNetParameterCountMismatchError,
    CDotNetAmbiguousInvocationError,
    CDotNetCouldNotLoadTypeError,
    CDotNetInvalidInvocationKindError,

    CDotNetTypeIsNotArrayError,
    CDotNetNonArrayObjectHandleError,
    CDotNetInvalidNumberOfDimensionsError,
    CDotNetNonArrayTypeIdError,
    CDotNetInvalidElementTypeError,
    CDotNetInvalidDimensionError,
    CDotNetInvalidNumElementsError,
    CDotNetIndexOutOfRangeError,
    CDotNetCouldNotAccessElementError,

    CDotNetInvalidEnumTypeError,
    CDotNetTypeIsNotEnumError,

    CDotNetNonCOMTypeError,
    CDotNetNonCOMObjectHandleError,

    CDotNetNonExceptionObjectHandleError,

    CDotNetAppDomainUnloadedError,
    CDotNetArithmeticError,
    CDotNetArrayTypeMismatchError,
    CDotNetBadImageFormatError,
    CDotNetCannotUnloadAppDomainError,
    CDotNetContextMarshalError,
    CDotNetExecutionEngineError,
    CDotNetFormatError,
    CDotNetInvalidOperationError,
    CDotNetInvalidProgramError,
    CDotNetNullReferenceError,
    CDotNetNotImplementedError,
    CDotNetStackOverflowError,
    CDotNetTypeInitializationError,
    CDotNetTypeUnloadedError,
    CDotNetUnauthorizedAccessError,
    CDotNetLicenseError,

    CDotNetErrorEnd,
    CDotNetError4Bytes  = 0xffffffff
};

enum {

    CDOTNET_VOID        = 0,
    CDOTNET_STRING,
    CDOTNET_BOOLEAN,
    CDOTNET_BYTE,
    CDOTNET_SBYTE,
    CDOTNET_INT16,
    CDOTNET_INT32,
    CDOTNET_INT64,
    CDOTNET_UINT16,
    CDOTNET_UINT32,
    CDOTNET_UINT64,
    CDOTNET_SINGLE,
    CDOTNET_DOUBLE,
    CDOTNET_DECIMAL,
    CDOTNET_CHAR,
    CDOTNET_INTPTR,
    CDOTNET_ENUM,
    CDOTNET_OBJECT,
    CDOTNET_STRUCT,
    CDOTNET_GENERIC,

    CDOTNET_BASIC_TYPE_MASK = 0x0fff,
    CDOTNET_MODIFIER_MASK   = 0xf000,

    CDOTNET_OUT             = 0x1000,
    CDOTNET_REF             = 0x2000,
    CDOTNET_ARRAY           = 0x4000,

    CDOTNET_TYPEID_4BYTES   = 0xffffffff
};

enum {
    CDOTNET_CALL_METHOD,
    CDOTNET_GET_PROPERTY,
    CDOTNET_SET_PROPERTY,
    CDOTNET_GET_FIELD,
    CDOTNET_SET_FIELD,
    CDOTNET_INVOKE_4BYTES   = 0xffffffff
};

typedef int (CVICALLBACK * CDotNetCallback)(CDotNetHandle delegate, void * parameters[], 
                                            unsigned int numParameters, void * returnValue, 
                                            void * callbackData, void * reserved);

int             CVIFUNC     CDotNetRegisterAssemblyPath             (const char * name,
                                                                    const char * path);
int             CVIFUNC     CDotNetLoadAssembly                     (const char * name,
                                                                    CDotNetAssemblyHandle * assembly);

int             CVIFUNC     CDotNetRegisterLicense                  (CDotNetAssemblyHandle assembly,
                                                                     const char * typeName,
                                                                     const char * licenseKey);
int             CVIFUNC     CDotNetCreateInstance                   (CDotNetAssemblyHandle assembly,
                                                                    const char * name,
                                                                    void * instance,
                                                                    int numParameters,
                                                                    unsigned int parameterTypes[],
                                                                    void * parameters[],
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetInvokeMember                     (CDotNetAssemblyHandle assembly,
                                                                    const char * typeName,
                                                                    CDotNetHandle instance,
                                                                    int invokeKind,
                                                                    const char * name,
                                                                    int numParameters,
                                                                    unsigned int parameterTypes[],
                                                                    void * parameters[],
                                                                    unsigned int returnType,
                                                                    void * returnValue,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetInvokeStaticMember               (CDotNetAssemblyHandle assembly,
                                                                    const char * typeName,
                                                                    int invokeKind,
                                                                    const char * name,
                                                                    int numParameters,
                                                                    unsigned int parameterTypes[],
                                                                    void * parameters[],
                                                                    unsigned int returnType,
                                                                    void * returnValue,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetCreateGenericInstance            (CDotNetAssemblyHandle assembly,
                                                                    const char * name,
                                                                    int numGenericTypes,
                                                                    const char * genericTypes[],
                                                                    void * instance,
                                                                    int numParameters,
                                                                    const char * parameterTypeNames[],
                                                                    unsigned int parameterTypes[],
                                                                    void * parameters[],
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetInvokeGenericMember              (CDotNetAssemblyHandle assembly,
                                                                    const char * typeName,
                                                                    CDotNetHandle instance,
                                                                    int invokeKind,
                                                                    const char * name,
                                                                    int numMemberGenericTypes,
                                                                    const char * memberGenericTypes[],
                                                                    int numParameters,
                                                                    const char * parameterTypeNames[],
                                                                    unsigned int parameterTypes[],
                                                                    void * parameters[],
                                                                    unsigned int * returnType,
                                                                    void * returnValue,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetInvokeGenericStaticMember        (CDotNetAssemblyHandle assembly,
                                                                    const char * typeName,
                                                                    int numGenericTypes,
                                                                    const char * genericTypes[],
                                                                    int invokeKind,
                                                                    const char * name,
                                                                    int numMemberGenericTypes,
                                                                    const char * memberGenericTypes[],
                                                                    int numParameters,
                                                                    const char * parameterTypeNames[],
                                                                    unsigned int parameterTypes[],
                                                                    void * parameters[],
                                                                    unsigned int * returnType,
                                                                    void * returnValue,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetInvokeGenericInterfaceMember     (CDotNetAssemblyHandle assembly,
                                                                     const char * typeName,
                                                                     int numGenericTypes,
                                                                     const char * genericTypes[],
                                                                     CDotNetHandle instance,
                                                                     int invokeKind,
                                                                     const char * name,
                                                                     int numMemberGenericTypes,
                                                                     const char * memberGenericTypes[],
                                                                     int numParameters,
                                                                     const char * parameterTypeNames[],
                                                                     unsigned int parameterTypes[],
                                                                     void *parameters[],
                                                                     unsigned int * returnType,
                                                                     void * returnValue,
                                                                     CDotNetHandle * exception);
int             CVIFUNC     CDotNetCreateDelegate                   (CDotNetAssemblyHandle assembly,
                                                                     const char * delegateName,
                                                                     int numGenericTypes,
                                                                     const char * genericTypes[],
                                                                     CDotNetCallback callbackFunction,
                                                                     void * callbackData,
                                                                     void * reserved,
                                                                     void * delegate,
                                                                     CDotNetHandle * exception);
int             CVIFUNC     CDotNetDiscardHandle                    (CDotNetHandle handle);
int             CVIFUNC     CDotNetDiscardAssemblyHandle            (CDotNetAssemblyHandle assemblyHandle);

void *          CVIFUNC     CDotNetAllocateMemory                   (unsigned int size);
void            CVIFUNC     CDotNetFreeMemory                       (void * pointer);

int             CVIFUNC     CDotNetCreateArray                      (unsigned int elementType,
                                                                    int numDimensions,
                                                                    int lowerBounds[],
                                                                    int lengths[],
                                                                    void * elements,
                                                                    CDotNetHandle * array);
int             CVIFUNC     CDotNetCreateTypedArray                 (CDotNetAssemblyHandle elementTypeAssembly,
                                                                    const char * elementTypeName,
                                                                    int numDimensions,
                                                                    int lowerBounds[],
                                                                    int lengths[],
                                                                    void * elements,
                                                                    CDotNetHandle * array);
int             CVIFUNC     CDotNetGetArrayElementType              (CDotNetHandle array,
                                                                    unsigned int * typeId,
                                                                    CDotNetAssemblyHandle * assembly,
                                                                    char ** elementTypeName);
int             CVIFUNC     CDotNetGetArrayNumDimensions            (CDotNetHandle array,
                                                                    int * numDimensions);
int             CVIFUNC     CDotNetGetArrayLength                   (CDotNetHandle array,
                                                                    int dimension,
                                                                    int * length);
int             CVIFUNC     CDotNetGetArrayLowerBound               (CDotNetHandle array,
                                                                    int dimension,
                                                                    int * lowerBound);
int             CVIFUNC     CDotNetGetArrayElements                 (CDotNetHandle array,
                                                                    unsigned int elementType,
                                                                    int * numElements,
                                                                    void * elements);
int             CVIFUNC     CDotNetSetArrayElements                 (CDotNetHandle array,
                                                                    unsigned int elementType,
                                                                    int numElements,
                                                                    void * elements);
int             CVIFUNC     CDotNetGetArrayElement                  (CDotNetHandle array,
                                                                    unsigned int elementType,
                                                                    int numDimensions,
                                                                    int indices[],
                                                                    void * element);
int             CVIFUNC     CDotNetSetArrayElement                  (CDotNetHandle array,
                                                                    unsigned int elementType,
                                                                    int numDimensions,
                                                                    int indices[],
                                                                    void * element);

int             CVIFUNC     CDotNetGetTypeFromName                  (CDotNetAssemblyHandle typeAssembly,
                                                                    const char * typeName,
                                                                    unsigned int * typeId,
                                                                    CDotNetHandle * type);
int             CVIFUNC     CDotNetGetNumberOfGenericTypeParameters (CDotNetHandle instance,
                                                                     const char * typeName,
                                                                     int * numGenericParameters);
int             CVIFUNC     CDotNetGetGenericTypeParameter          (CDotNetHandle instance,
                                                                     const char * typeName,
                                                                     int index, char ** parameter);
int             CVIFUNC     CDotNetGetEnumTypeFromName              (CDotNetAssemblyHandle typeAssembly,
                                                                    const char * typeName,
                                                                    unsigned int * typeId);
int             CVIFUNC     CDotNetGetEnumType                      (CDotNetHandle handle,
                                                                    unsigned int * typeId);
int             CVIFUNC     CDotNetGetEnumValue                     (CDotNetHandle handle,
                                                                    void * value);
int             CVIFUNC     CDotNetConvertValueToHandle             (void * value,
                                                                    unsigned int typeId,
                                                                    CDotNetHandle * handle);
int             CVIFUNC     CDotNetConvertHandleToValue             (CDotNetHandle handle,
                                                                    unsigned int typeId,
                                                                    void * value);

int             CVIFUNC     CDotNetGetCOMObjectHandle               (CDotNetAssemblyHandle comInteropAssembly,
                                                                    const char * typeName,
                                                                    IUnknown * interfacePtr,
                                                                    void * handle);
int             CVIFUNC     CDotNetGetCOMInterface                  (CDotNetHandle handle,
                                                                    IUnknown ** interfacePtr);

int             CVIFUNC     CDotNetGetExceptionInfo                 (CDotNetHandle exception,
                                                                    CDotNetHandle * innerException,
                                                                    char ** message,
                                                                    char ** source,
                                                                    char ** stackTrace,
                                                                    char ** targetSite,
                                                                    char ** helpLink);

int             CVIFUNC     CDotNetObjectEquals                     (CDotNetHandle object1,
                                                                    CDotNetHandle object2,
                                                                    int * equals,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetObjectGetHashCode                (CDotNetHandle object,
                                                                    int * hashCode,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetObjectGetType                    (CDotNetHandle object,
                                                                    char ** typeName,
                                                                    CDotNetAssemblyHandle * assembly,
                                                                    CDotNetHandle * type,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetObjectToString                   (CDotNetHandle object,
                                                                    char ** objectString,
                                                                    CDotNetHandle * exception);
int             CVIFUNC     CDotNetMakeQualifiedTypeName            (CDotNetAssemblyHandle assembly,
                                                                     const char * typeName,
                                                                     const char * qualifier,
                                                                     char ** qualifiedTypeName);
int             CVIFUNC     CDotNetMakeClosedGenericTypeName        (CDotNetAssemblyHandle assembly,
                                                                     const char * typeNameFmt,
                                                                     int numTypeArguments,
                                                                     const char * typeArguments[],
                                                                     char ** closedTypeName);
int             CVIFUNC     CDotNetGetDelegateParameterType         (CDotNetHandle delegate,
                                                                     int parameterIndex,
                                                                     CDotNetAssemblyHandle * assembly,
                                                                     char ** typeName,
                                                                     unsigned int * typeId,
                                                                     int elementType);
const char *    CVIFUNC     CDotNetGetErrorDescription              (int error);
void            CVIFUNC     CDotNetTerminate                        (void);

#if defined(_CVI_) && !defined(__TPC__)
#pragma PassOrigPtrInfoForVoidPtrParam  CDotNetGetArrayElements
#endif

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif // ndef _CVIDOTNET_H
