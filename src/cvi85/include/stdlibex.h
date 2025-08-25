/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       stdlibex.h                                                    */
/* Purpose:     Include file for CVI specific extensions of ANSI Standard C   */
/*              library functions and macros of general purpose utility.      */
/*                                                                            */
/*============================================================================*/

#ifndef _STDLIBEX
#define _STDLIBEX

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

#if defined (_NI_mswin32_) && _NI_mswin32_
int CVIANSI _putenv(const char *);

__int64 CVIANSI atoll(const char *);
__int64 CVIANSI strtoll(const char *, char **, int);
unsigned __int64 CVIANSI strtoull(const char *, char **, int);

void * CVIANSI alloca (size_t);

typedef struct {
    __int64 quot;
    __int64 rem;
} lldiv_t;

__int64 CVIANSI llabs(__int64);
lldiv_t CVIANSI lldiv(__int64, __int64);

#elif defined (_NI_linux_) && _NI_linux_
#define _putenv   putenv

#include <alloca.h>
#endif

#define atoi64    atoll
#define strtol64  strtoll
#define strtoul64 strtoull

#ifdef __cplusplus
    }
#endif

#endif

