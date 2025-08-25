/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       ddesupp.h                                                     */
/* Purpose:     Include file for Windows (TM) DDE support library.            */
/*                                                                            */
/*============================================================================*/


#ifndef DDESUPP_HEADER
#define DDESUPP_HEADER

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* constants */

#define DDE_CONNECT         1
#define DDE_DISCONNECT      2
#define DDE_DATAREADY       3
#define DDE_REQUESTDATA     4
#define DDE_ADVISELOOP      5
#define DDE_ADVISESTOP      6
#define DDE_EXECUTE         7

/* Windows provides serveral predefined formats for use in data interchange */

#define CF_TEXT             1
#define CF_BITMAP           2
#define CF_METAFILEPICT     3
#define CF_SYLK             4
#define CF_DIF              5
#define CF_TIFF             6
#define CF_OEMTEXT          7
#define CF_DIB              8
#define CF_PALETTE          9
#define CF_PENDATA          10
#define CF_RIFF             11
#define CF_WAVE             12

#define CF_OWNERDISPLAY     0x0080
#define CF_DSPTEXT          0x0081
#define CF_DSPBITMAP        0x0082
#define CF_DSPMETAFILEPICT  0x0083

/* the absolute values of the error codes returned by the DDE functions */
#define kDDE_NoError                            0
#define kDDE_UnableToRegisterService            1
#define kDDE_ExistingServer                     2
#define kDDE_FailedToConnect                    3
#define kDDE_ServerNotRegistered                4
#define kDDE_TooManyConversations               5
#define kDDE_InvalidParameter                   9
#define kDDE_OutOfMemory                       10
#define kDDE_NoConnectionEstablished           12
#define kDDE_NotThreadOfServer                 13
#define kDDE_NotThreadOfClient                 14
#define kDDE_AdvAckTimeOut                     16
#define kDDE_Busy                              17
#define kDDE_DataAckTimeOut                    18
#define kDDE_DllNotInitialized                 19
#define kDDE_DllUsage                          20
#define kDDE_ExecAckTimeOut                    21
#define kDDE_DataMismatch                      22
#define kDDE_LowMemory                         23
#define kDDE_MemoryError                       24
#define kDDE_NotProcessed                      25
#define kDDE_NoConvEstablished                 26
#define kDDE_PokeAckTimeOut                    27
#define kDDE_PostMsgFailed                     28
#define kDDE_ServerDied                        30
#define kDDE_SysError                          31
#define kDDE_UnadvAckTimeOut                   32
#define kDDE_UnfoundQueueId                    33


/* typedefs */

typedef int (CVICALLBACK  * ddeFuncPtr) (unsigned handle, char *topicName, char *itemName,
                                         int xType, int dataFmt, int dataSize,
                                         void *dataPtr, void *callbackData);

/* exported functions */

int CVIFUNC AdviseDDEDataReady(unsigned convHandle, const char userItemName[],
                               unsigned dataFmt, const void *userData,
                               unsigned dataSize, unsigned timeOut);
int CVIFUNC BroadcastDDEDataReady(const char userServName[], const char userTopicName[],
                                  const char userItemName[], unsigned dataFmt,
                                  const void *userData, unsigned dataSize);
int CVIFUNC ClientDDEExecute(unsigned convHandle, const char commandString[],unsigned timeOut);
int CVIFUNC ClientDDERead(unsigned convHandle, const char userItemName[], unsigned dataFmt,
                          void *dataBuffer, unsigned dataSize, unsigned timeOut);
int CVIFUNC ClientDDEWrite(unsigned convHandle, const char userItemName[], unsigned dataFmt,
                           const void *userData, unsigned dataSize, unsigned timeOut);
int CVIFUNC ConnectToDDEServer(unsigned *convHandle, const char userServName[],
                               const char userTopicName[], ddeFuncPtr pUserCallback,
                               void *callbackData);
int CVIFUNC DisconnectFromDDEServer(unsigned convHandle);
int CVIFUNC RegisterDDEServer(const char userServName[], ddeFuncPtr pUserCallback,
                              void *callbackData);
int CVIFUNC ServerDDEWrite(unsigned convHandle, const char userItemName[], unsigned dataFmt,
                           const void *userData, unsigned dataSize, unsigned timeOut);
int CVIFUNC SetUpDDEHotLink(unsigned  convHandle, const char userItemName[],
                            unsigned dataFmt, unsigned timeOut);
int CVIFUNC SetUpDDEWarmLink(unsigned convHandle, const char userItemName[],
                             unsigned dataFmt, unsigned timeOut);
int CVIFUNC TerminateDDELink(unsigned convHandle, const char userItemName[],
                             unsigned dataFmt, unsigned timeOut);
int CVIFUNC UnregisterDDEServer(const char userServName[]);

char * CVIFUNC GetDDEErrorString(int errorNum);

#ifdef __cplusplus
    }
#endif

#endif /* DDESUPP_HEADER */
