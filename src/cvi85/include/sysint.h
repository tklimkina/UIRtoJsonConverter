/**************************************************************************/
/*                                                                        */
/*             National Instruments NI-VXI Software Interface             */
/*                   (C) 1996-1999,  National Instruments                 */
/*                                                                        */
/*          System Interrupt and VXIbus Extender Include File             */
/*                                                                        */
/**************************************************************************/
/*                       DO NOT MODIFY THIS FILE                          */
/**************************************************************************/
/* The following file contains all the necessary System Interrupt and
 * VXIbus Extender information for use of the National Instruments NI-VXI
 * Software Interface functions.
 */

#if !defined(sysint_h)
#define sysint_h

/*****************************************************************************
 *   The following are return codes for all of the System Interrupt
 * functions.
 */

/* General defines for all VXI interrupt and signal functions */
#define INT_SRC_NOT_SUPPORTED       NO_HARDWARE_SUPPORT

/**************
     ACfail
 **************/

/* EnableACfail, DisableACfail */
#define ACFAIL_OK                   NIVXI_OK       /* zero */
#define ACFAIL_UNSUP                INT_SRC_NOT_SUPPORTED
#define ACFAIL_INVCONT              INVALID_CONTROLLER

/* SetACfailHandler return codes */
#define ACFAIL_SET_OK               NIVXI_OK       /* zero */
#define ACFAIL_SET_UNSUP            INT_SRC_NOT_SUPPORTED

/* GetACfailHandler "func" return codes */
#define ACFAIL_GET_INVLA            0L

/**************
   Soft Reset
 **************/

/* EnableSoftReset, DisableSoftReset */
#define SOFTR_OK                    NIVXI_OK       /* zero */
#define SOFTR_UNSUP                 INT_SRC_NOT_SUPPORTED

/* SetSoftResetHandler return codes */
#define SOFTR_SET_OK                NIVXI_OK       /* zero */
#define SOFTR_SET_UNSUP             INT_SRC_NOT_SUPPORTED

/* GetSoftResetHandler "func" return codes */
#define SOFTR_GET_INVLA             0L

/**************
     Sysfail
 **************/

/* EnableSysfail, DisableSysfail */
#define SYSF_OK                     NIVXI_OK       /* zero */
#define SYSF_UNSUP                  INT_SRC_NOT_SUPPORTED
#define SYSF_INVCONT                INVALID_CONTROLLER

/* SetSysfailHandler return codes */
#define SYSF_SET_OK                 NIVXI_OK       /* zero */
#define SYSF_SET_UNSUP              INT_SRC_NOT_SUPPORTED

/* GetSysfailHandler "func" return codes */
#define SYSF_GET_INVLA              0L

/**************
    Sysreset
 **************/

/* EnableSysreset, DisableSysreset */
#define SYSR_OK                     NIVXI_OK       /* zero */
#define SYSR_UNSUP                  INT_SRC_NOT_SUPPORTED

/* SetSysresetHandler return codes */
#define SYSR_SET_OK                 NIVXI_OK       /* zero */
#define SYSR_SET_UNSUP              INT_SRC_NOT_SUPPORTED

/* GetSysresetHandler "func" return codes */
#define SYSR_GET_INVLA              0L

/* AssertSysreset mode parameters */
#define SYSR_RST_ASSERT             0U
#define SYSR_RST_ALL                1U
#define SYSR_RST_OTHERS             2U

/* AssertSysreset return codes */
#define SYSR_RST_OK                 NIVXI_OK       /* zero */
#define SYSR_RST_UNSUP              INT_SRC_NOT_SUPPORTED
#define SYSR_RST_INVCONT            INVALID_CONTROLLER
#define SYSR_RST_INVMODE            (-10)

/*****************************************************************************
 *   The following are return codes for all of the VXIbus Extender functions
 */

/* MapECLtrig, MapTTLtrig, MapUtilBus, MapVXIint */
#define MAP_OK                     NIVXI_OK       /* zero */
#define MAP_UNSUP                  (-1)
#define MAP_INV_EXTENDER           (-2)

#endif

