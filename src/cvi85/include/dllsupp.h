/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       dllsupp.h                                                     */
/* Purpose:     Function declarations which must be used when using the       */
/*              WATCOM function DefineUserProc16(). Since there are only      */
/*              16 of the GETPROC_USEDEFINED_X handles, they are managed      */
/*              by these functions.                                           */
/*                                                                            */
/*============================================================================*/

#include "cvidef.h"
#include "cvirte.h"

#ifndef _DLLSUPP_H_
#define _DLLSUPP_H_

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef _NI_mswin16_
int CVIFUNC GetProcUserDefinedHandle(void);
void CVIFUNC FreeProcUserDefinedHandle(int handle);

/* This function should be used to free the handle allocated by               */
/* GetIndirectFunctionHandle().                                               */

void CVIFUNC FreeIndirectHandle(void* handle);
#endif /* def _NI_mswin16_ */

/* These functions get/set the 80x87 control word.                         */
/* DLL's that use floating point operations need to save the control       */
/* control word on entry to each function (only if it uses floating        */
/* point operations) and restore it on exit                                */
unsigned short CVIFUNC_C Get387CW(void);
void CVIFUNC_C Set387CW(unsigned short);

#ifdef _NI_mswin16_
/*  This function returns the hInstance parameter to WinMain for CVI       */
unsigned CVIFUNC GetTaskInstance(void);

/* This function allocate a 16:16 alias for the 32 bit pointer 'p',        */
/* capable of addressing 'size' bytes.                                     */
/* If either p or size is zero, alias will be set to zero.                 */
/* It returns 0 on success, -1 on failure.                                 */
/* It works on buffers of size greater than 64K                            */
int CVIFUNC Alloc16BitAlias (void *p, unsigned long size, unsigned long *alias);

/* This function deallocs an alias allocated with Alloc16BitAlias. It      */
/* works on any alias from a successful call to Alloc16BitAlias.           */

void  CVIFUNC Free16BitAlias (unsigned long alias, unsigned long size);

#endif /* _NI_mswin16_ */

#define WEM_NOOPENFILEERRORBOX  0x8000

int CVIFUNC SetWindowsErrorMode(int errorMode);

#ifdef __cplusplus
    }
#endif

#endif /* _DLLSUPP_H_ */
