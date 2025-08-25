/************************************************************************/
/*                                                                      */
/*            National Instruments NI-VXI Software Interface            */
/*                   (C) 1998,  National Instruments                    */
/*                                                                      */
/*                     NI-VXI Data Type Definitions                     */
/*                                                                      */
/************************************************************************/
/*                       DO NOT MODIFY THIS FILE                        */
/************************************************************************/
/* The following file contains all of the necessary typedef information
 * for use of the National Instruments NI-VXI Software Interface
 * functions.
 */

#if !defined(datasize_h)
#define datasize_h

/* === Target dependencies -- MUST define one of following !!! ======== */

#if defined(VXIWIN)
#if !defined(NIVXI_FSPEC)
#define NIVXI_FSPEC                 _far _pascal
#endif
#elif defined(VXIDOS)
#define NIVXI_FSPEC                 _far _cdecl
#elif defined(VXINT)
#define NIVXI_FSPEC                 __stdcall
#elif defined(VXISOLARIS)
#define NIVXI_FSPEC
#elif defined(VXIMAC)
#define NIVXI_FSPEC
#elif defined(VXIHPUX)
#define NIVXI_FSPEC
#elif defined(VXIVXWORKS)
#define NIVXI_FSPEC
#elif defined(VXIWIN32)
#define NIVXI_FSPEC
#elif defined(_NI_mswin32_)
#define VXINT
#define NIVXI_FSPEC                 __stdcall
#elif defined(_NI_mswin16_) || defined(_NI_i386_)
#define VXIWIN32
#define NIVXI_FSPEC
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define VXINT
#define NIVXI_FSPEC                 __stdcall
#elif defined(_WINDOWS) || defined(_Windows)
#define VXIWIN
#if !defined(NIVXI_FSPEC)
#define NIVXI_FSPEC                 _far _pascal
#endif
#elif defined(__sparc) || defined(_NI_sparc_)
#define VXISOLARIS
#define NIVXI_FSPEC
#elif defined(__hp9000s700) || defined(__hppa__) || defined(__hpux) || defined(hpux) || defined(_NI_hpux_)
#define VXIHPUX
#define NIVXI_FSPEC
#else
#error Undefined target macro:  define VXI<OS>.
#error See INCLUDE\DATASIZE.H or ...\EXAMPLES\MAKEFILE for details.
#endif

#if defined(VXINT)
#define NIVXI_HSPEC                 NIVXI_FSPEC
#elif defined(_M_I86) || defined(M_I86)
#define NIVXI_HSPEC                 _loadds NIVXI_FSPEC
#elif defined(__TURBOC__)
#define NIVXI_HSPEC                 _saveregs _loadds NIVXI_FSPEC
#else
#define NIVXI_HSPEC                 NIVXI_FSPEC
#endif

#if defined(VXIDOS) || defined(VXIWIN)
#define NIVXI_DSPEC                 _far
#else
#define NIVXI_DSPEC
#endif

#define NIVXI_HQUAL

/* === Data Sizes -- machine specific ================================= */

#if (defined(_MSC_VER) && (_MSC_VER >= 1200))
/* new SDK defines some of the same types that NIVXI defines below */
#include <basetsd.h>
#elif defined(VXIVXWORKS)
/* vxWorks already defines these */
#include <vxWorks.h>
#ifndef INT_TYPES
#define INT_TYPES
#endif
#ifndef UINT_TYPES
#define UINT_TYPES
#endif
#endif

#ifndef INT_TYPES
#define INT_TYPES
typedef          char   INT8;
typedef          short  INT16;
#if !defined(_BASETSD_H_) || (((!defined(_MSC_VER)) || (_MSC_VER < 1200)) && ((!defined(_CVI_)) || (_CVI_ < 550)))
typedef          long   INT32;
#endif
#endif

#ifndef SINT_TYPES
#define SINT_TYPES
#if defined(VXIHPUX) && (defined(__cplusplus) || defined(__cplusplus__))
typedef          char   SINT8;
typedef          short  SINT16;
typedef          long   SINT32;
#else
typedef signed   char   SINT8;
typedef signed   short  SINT16;
typedef signed   long   SINT32;
#endif
#endif

#ifndef UINT_TYPES
#define UINT_TYPES
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
#if !defined(_BASETSD_H_) || (((!defined(_MSC_VER)) || (_MSC_VER < 1200)) && ((!defined(_CVI_)) || (_CVI_ < 550)))
typedef unsigned long   UINT32;
#endif
#endif

typedef INT16           NIVXI_STATUS;

/* following provided for backward compatability */

#ifdef NIVXI_PROVIDE_DATATYPE_COMPAT
#define int8            INT8
#define uint8           UINT8
#define int16           INT16
#define uint16          UINT16
#define int32           INT32
#define uint32          UINT32
#endif

#define NIVXI_DT        NIVXI_DSPEC
#define NIVXI_FN        NIVXI_FSPEC
#define NIVXI_HANDLER   NIVXI_HSPEC

typedef NIVXI_HQUAL void (NIVXI_HSPEC *FN_PTR)(void);
typedef void NIVXI_DSPEC *lpvoid;

/* === Interrupt Handler Function Types =============================== */

#if !defined(__NO_PROTOTYPES__)

typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HACFAIL   (INT16  controller);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HBUSERROR (void);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HCLOCK    (void);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HVXIINT   (INT16  controller,
                                                      UINT16 level,
                                                      UINT32 statusId);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSIGNAL   (UINT16 signal);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSOFTRESET(void);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSYSFAIL  (INT16  controller);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSYSRESET (INT16  controller);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HTRIG     (INT16  controller,
                                                      UINT16 line,
                                                      UINT16 type);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSCMD   (UINT16 cmd);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSECMD  (UINT16 cmdExt,
                                                      UINT32 cmd);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSLCMD  (UINT32 cmd);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSRD    (INT16  status,
                                                      UINT32 count);
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSWRT   (INT16  status,
                                                      UINT32 count);

#else

typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HACFAIL   ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HBUSERROR ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HCLOCK    ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HVXIINT   ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSIGNAL   ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSOFTRESET();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSYSFAIL  ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HSYSRESET ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HTRIG     ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSCMD   ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSECMD  ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSLCMD  ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSRD    ();
typedef NIVXI_HQUAL void NIVXI_HSPEC NIVXI_HWSSWRT   ();

#endif

/* === Version specific information =================================== */

#define NIVXI_EXTERNAL        (0x100)
#define NIVXI_EMBEDDED        (0x200)

#define NIVXI_RETRY           (0x100)
#define NIVXI_NO_RETRY        (0x200)

/* Note:  The macros NIVXI_DEVNAME, NIVXI_DEVLOCATION, and NIVXI_DEVRETRY
   are no longer defined.  Users should use GetDevInfo with DI_NIVXI_DEVNAME,
   DI_NIVXI_DEVLOCATION, and DI_NIVXI_RETRY instead. */

#endif
