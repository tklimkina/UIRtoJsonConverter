/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       tcpsupp.h                                                     */
/* Purpose:     Include file for TCP/IP protocol support library.             */
/*                                                                            */
/*============================================================================*/

#ifndef TCPSUPP_HEADER
#define TCPSUPP_HEADER

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* constants */

#define     TCP_CONNECT       1
#define     TCP_DISCONNECT    2
#define     TCP_DATAREADY     3

#define TCP_DISCONNECT_AUTO     0
#define TCP_DISCONNECT_MANUAL   1

#define TCP_ANY_LOCAL_PORT      ((unsigned int)0)

/* enumerated errors */

enum tcpError {
    kTCP_NoError,
    kTCP_UnableToRegisterService,
    kTCP_UnableToEstablishConnection,
    kTCP_ExistingServer,
    kTCP_FailedToConnect,
    kTCP_ServerNotRegistered,
    kTCP_TooManyConnections,
    kTCP_ReadFailed,
    kTCP_WriteFailed,
    kTCP_InvalidParameter,
    kTCP_OutOfMemory,
    kTCP_TimeOutErr,
    kTCP_NoConnectionEstablished,
    kTCP_GeneralIOErr,
    kTCP_ConnectionClosed,
    kTCP_UnableToLoadWinsockDLL,
    kTCP_IncorrectWinsockDLLVersion,
    kTCP_NetworkSubsystemNotReady,
    kTCP_ConnectionsStillOpen,
    kTCP_DisconnectPending,
    kTCP_InfoNotAvailable,
    kTCP_HostAddressNotFound
};



/* type definitions */

typedef int (CVICALLBACK * tcpFuncPtr)(unsigned handle, int xType, int errCode, void *callbackData);

/* exported functions */

int CVIFUNC ClientTCPRead(unsigned convHandle, void *dataBuffer, unsigned dataSize, unsigned timeOut);
int CVIFUNC ClientTCPWrite(unsigned convHandle, const void *userData, unsigned dataSize, unsigned timeOut);
int CVIFUNC ConnectToTCPServerEx(unsigned *convHandle, unsigned serverPort, const char servHostName[],
                                 tcpFuncPtr pUserCallback, void *callbackData, unsigned timeOut,
                                 unsigned clientPort);
int CVIFUNC ConnectToTCPServer(unsigned *convHandle, unsigned portNumber, const char servHostName[],
                               tcpFuncPtr pUserCallback, void *callbackData, unsigned timeOut);
int CVIFUNC DisconnectFromTCPServer(unsigned convHandle);
int CVIFUNC RegisterTCPServerEx(unsigned portNumber, tcpFuncPtr pUserCallback,
                                void *callbackData, const char *localhostAddress);
int CVIFUNC RegisterTCPServer(unsigned portNumber, tcpFuncPtr pUserCallback, void *callbackData);
int CVIFUNC ServerTCPRead(unsigned convHandle, void *dataBuffer, unsigned dataSize, unsigned timeOut);
int CVIFUNC ServerTCPWrite(unsigned convHandle, const void *userData, unsigned dataSize, unsigned timeOut);
int CVIFUNC DisconnectTCPClient(unsigned convHandle);
int CVIFUNC UnregisterTCPServerEx(unsigned portNumber, const char *localhostAddress);
int CVIFUNC UnregisterTCPServer(unsigned portNumber);
char * CVIFUNC GetTCPErrorString(int errorNum);
char * CVIFUNC GetTCPSystemErrorString(void);
int CVIFUNC SetTCPDisconnectMode(unsigned handle, int mode);
int CVIFUNC GetHostTCPSocketHandle(unsigned handle, int *socket);
int CVIFUNC GetTCPPeerName(unsigned handle, char* buf, int bufsize);
int CVIFUNC GetTCPPeerAddr(unsigned handle, char* buf, int bufsize);
int CVIFUNC GetTCPHostName(char* buf, int bufsize);
int CVIFUNC GetTCPHostAddr(char* buf, int bufsize);
int CVIFUNC GetAllTCPHostAddresses (char ***addresses, int *numAddresses);
int CVIFUNC CloseWinsock(int unloadDLL);
int CVIFUNC ProcessTCPEvents(void);
void CVIFUNC TCPFreeMemory(void *ptr);

#ifdef __cplusplus
    }
#endif

#endif /* TCPSUPP_HEADER */
