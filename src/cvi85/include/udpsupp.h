/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-2007.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       udpsupp.h                                                     */
/* Purpose:     Include file for UDP protocol support library.                */
/*                                                                            */
/*============================================================================*/

#ifndef UDPSUPP_HEADER
#define UDPSUPP_HEADER

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* constants */

#ifndef NULL
#define NULL 0
#endif

#define UDP_DATAREADY              3

#define UDP_DO_NOT_WAIT            0
#define UDP_DEFAULT_TIMEOUT        5000
#define UDP_WAIT_FOREVER           ((unsigned int)-1)

#define UDP_LAN_BROADCAST_ADDR     "255.255.255.255"

#define UDP_ANY_LOCAL_PORT         ((unsigned int)0)
#define UDP_ANY_ADDRESS            NULL

/* enumerated errors */

enum UDPError
{
    kUDP_NoError                            = 0,
    kUDP_InvalidChannel                     = -6800,
    kUDP_InvalidPortNumber                  = -6801,
    kUDP_InvalidAddress                     = -6802,
    kUDP_InvalidWriteSize                   = -6803,
    kUDP_InvalidArgument                    = -6804,
    kUDP_NullPointer                        = -6805,
    kUDP_InvalidAttribute                   = -6806,
    kUDP_InvalidAttributeValue              = -6807,
    kUDP_ReadTimeout                        = -6808,
    kUDP_DataLost                           = -6809,
    kUDP_ChannelClosed                      = -6810,
    kUDP_PortInUse                          = -6811,
    kUDP_GeneralNetworkError                = -6812,
    kUDP_HostAddressNotFound                = -6813,
    kUDP_InvalidMulticastAddress            = -6814,
    kUDP_AlreadySubscribedOnInterface       = -6815,
    kUDP_AlreadySubscribedToSource          = -6816,
    kUDP_NotSubscribed                      = -6817,
    kUDP_NotSubscribedToSource              = -6818,
    kUDP_WinsockUnavailable                 = -6819,
    kUDP_LibraryInitializationFailure       = -6820,
    kUDP_LibraryShuttingDown                = -6821,
    kUDP_UnexpectedWinsockError             = -6822,
    kUDP_NotSupportedOnPlatform             = -6823,
    kUDP_SystemError                        = -6824,
    kUDP_OutOfMemory                        = -6825,
    kUDP_OutOfSystemResources               = -6826,
    kUDP_InternalError                      = -6827,
    
    kUDP_ForceSizeTo32Bits                  = 0xffffffff
};

/* type definitions */

typedef int (CVICALLBACK * UDPCallbackPtr)(unsigned channel, int eventType, int errCode, void *callbackData);

/* attributes */

enum UDPAttribute
{
    ATTR_UDP_PORT = 0,
    ATTR_UDP_CALLBACK,
    ATTR_UDP_CALLBACK_DATA,
    ATTR_UDP_MULTICAST_OUTPUT_INTERFACE,
    ATTR_UDP_MULTICAST_TTL,
    ATTR_UDP_MULTICAST_LOOPBACK
};

/* exported functions */

int CVIFUNC CreateUDPChannel(unsigned localPort, unsigned *channel);
int CVIFUNC CreateUDPChannelConfig(unsigned localPort, const char *localAddr, unsigned exclusive, UDPCallbackPtr callback, void *callbackData, unsigned *channel);
int CVIFUNC DisposeUDPChannel(unsigned channel);
int CVIFUNC UDPRead(unsigned channel, void *buf, unsigned bufsize, unsigned timeout, unsigned *sourcePort, char *sourceAddr);
int CVIFUNC UDPWrite(unsigned channel, unsigned destinationPort, const char *destinationAddr, void *buf, unsigned numBytes);
int CVIFUNC UDPMulticastSubscribe(unsigned channel, const char *multicastAddr, const char *sourceAddr);
int CVIFUNC UDPMulticastUnsubscribe(unsigned channel, const char *multicastAddr, const char *sourceAddr);
int CVIFUNC GetUDPAttribute(unsigned channel, int attribute, void *value);
int CVIFUNC_C SetUDPAttribute(unsigned channel, int attribute, ...);
int CVIFUNC GetUDPSocketHandle(unsigned channel, int *socketHandle);
char * CVIFUNC GetUDPErrorString(int errCode);

#ifdef __cplusplus
    }
#endif

#endif /* UDPSUPP_HEADER */
