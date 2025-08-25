/***************************************************************
                   LabWindows/CVI Internet Library
                Copyright 2003, National Instruments.
                        All Rights Reserved.
***************************************************************/
#ifndef _CVINTWRK_H
#define _CVINTWRK_H

#include "cvidef.h"

#ifdef __cplusplus
    extern "C" {
#endif

#if linux
#pragma pack(push, 8)
#else // linux
#pragma pack(push)
#pragma pack(8)
#endif // linux

#if defined(_CVI_) && !defined(__TPC__)
#pragma EnableLibraryRuntimeChecking
#endif

/* Error codes */
#define kInetNoError                        0
#define kInetInvalidParameter               -1
#define kInetOutOfMemory                    -2
#define kInetTimeOut                        -3
#define kInetSocketError                    -4
#define kInetTooManyConnections             -5
#define kInetFileError                      -6
#define kInetMailError                      -7
#define kInetFTPInvalidHandle               -8
#define kInetFTPCannotConnect               -9
#define kInetFTPLostConnection              -10
#define kInetFTPCommandFailed               -11
#define kInetFTPServerError                 -12
#define kInetFTPFileError                   -13
#define kInetFTPUnauth                      -14
#define kInetFTPNoPortFree                  -15
#define kInetFTPBufferIsEmpty               -16
#define kInetCouldNotLaunchBrowser          -17
#define kInetDllLoadError                   -18
#define kInetFunctionNotFound               -19
#define kInetCouldNotCreateMutex            -20
#define kInetTelnetInvalidHandle            -21
#define kInetTelnetCannotConnect            -22
#define kInetTelnetTooManyConnections       -23
#define kInetTelnetTimeoutNoBytesRead       -24
#define kInetTelnetTimeoutStringNotFound    -25
#define kInetIcmpDllLoadError               -26
#define kInetIcmpFunctionNotFound           -27
#define kInetIcmpError                      -28
#define kInetPop3ServerError                -29
#define kInetMailParseError                 -30
#define kInetHostNotFound                   -31
#define kInetFunctionNotSupported           -32

/* FTP */
#define INET_FTP_FILE_TYPE_ASCII  1
#define INET_FTP_FILE_TYPE_BINARY 2

int CVIFUNC InetFTPAutoSend (const char *ftpServer, const char *userName,
                             const char *password, const char *localFile,
                             const char *remoteFile, int transferType);

int CVIFUNC InetFTPAutoRetrieve (const char *ftpServer, const char *userName,
                                 const char *password, const char *localFile,
                                 const char *remoteFile, int transferType);

int CVIFUNC InetFTPLogin (const char *ftpServer, const char *userName,
                          const char *password);

int CVIFUNC InetFTPLoginEx (const char *serverName, const char *userName,
                            const char *password, int timeout);

int CVIFUNC InetFTPGetDir (int ftpHandle, char directoryName[], int length);

int CVIFUNC InetFTPChangeDir (int ftpHandle, const char *newDirectory);

int CVIFUNC InetFTPMakeDir (int ftpHandle, const char *newDirectory);

int CVIFUNC InetFTPRemoveDir (int ftpHandle, const char *directory);

int CVIFUNC InetFTPSendFile (int ftpHandle, const char *localFile,
                             const char *remoteFile, int transferType);

int CVIFUNC InetFTPRetrieveFile (int ftpHandle, const char *localFile,
                                 const char *remoteFile, int transferType);

int CVIFUNC InetFTPRename (int ftpHandle, const char *oldFileName,
                           const char *newFileName);

int CVIFUNC InetFTPDelete (int ftpHandle, const char *fileName);

int CVIFUNC InetFTPGetDirList (int ftpHandle, char ***listofFiles,
                               int *numberofFiles);

int CVIFUNC InetFTPClose (int ftpHandle);

int CVIFUNC InetFTPSetPassiveMode (int ftphandle, int passiveModeOn);

/* Telnet */
typedef struct TelnetScript {
    char *prompt;
    char *reply;
} TelnetScript;

int CVIFUNC InetTelnetOpen (const char *telnetServer, unsigned short remotePort,
                            unsigned short localPort);

int CVIFUNC InetTelnetRead (int telnetHandle, char *readBuffer, int bytesToRead,
                            int *bytesRead, int timeout);

int CVIFUNC InetTelnetWrite (int telnetHandle, const char *writeBuffer,
                             const char *endOfLine, int bytesToWrite,
                             int *bytesWritten, int timeout);

int CVIFUNC InetTelnetClose (int telnetHandle);

int CVIFUNC InetTelnetReadUntil (int telnetHandle, char *readBuffer,
                                 int readBufferLength, char *stringToMatch,
                                 int timeout);

int CVIFUNC InetTelnetRunScript (int telnetHandle, TelnetScript *script,
                                 const char *endOfLine, int itemsInScript,
                                 char *log, int logLength, int timeout);

/* Send mail */
int CVIFUNC InetSendMail (const char *server, const char *from, const char *to,
                          const char *subject, const char *messageText,
                          const char *attachments);

/* Pop3 */
int CVIFUNC InetPop3Open (const char *server, const char *userName,
                          const char *password, int *pop3Handle);

int CVIFUNC InetPop3Close (int pop3Handle);

int CVIFUNC InetPop3GetNumMessages (int pop3Handle, int *numMessages);

int CVIFUNC InetPop3GetMessageSize (int pop3Handle, int messageIndex,
                                    int *size);

int CVIFUNC InetPop3GetMessageInfo (int pop3Handle, int messageIndex,
                                    int numLines, char **from, char **to,
                                    char **cc, char **subject, char **body);

int CVIFUNC InetPop3DeleteMessage (int pop3Handle, int messageIndex);

int CVIFUNC InetPop3GetMessage (int pop3Handle, int messageIndex,
                                int numLines, char **header, char **body);

int CVIFUNC InetPop3ParseMessageHeader (const char *header,
                                        const char *fieldName,
                                        char **fieldValue);

/* Miscellaneous */
int CVIFUNC InetLaunchDefaultWebBrowser (const char *URL);

int CVIFUNC InetPing (const char *address, int *available, unsigned int timeout);

int CVIFUNC InetResolveHostname (const char *hostname, char ***addresses, int *numAddresses);

char * CVIFUNC InetGetErrorMessage (int errorCode);

int CVIFUNC InetFreeMemory (void *ptr);

#pragma pack(pop)

#ifdef __cplusplus
    }
#endif

#endif /* _CVINTWRK_H */
