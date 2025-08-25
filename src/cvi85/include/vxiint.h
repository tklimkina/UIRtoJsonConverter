/**************************************************************************/
/*                                                                        */
/*             National Instruments NI-VXI Software Interface             */
/*                   (C) 1996-1999,  National Instruments                 */
/*                                                                        */
/*                VXI Interrupt and Signal Include File                   */
/*                                                                        */
/**************************************************************************/
/*                       DO NOT MODIFY THIS FILE                          */
/**************************************************************************/
/* The following file contains all of the necessary typedef information
 * for use of the National Instruments NI-VXI Software Interface
 * functions.
 */

#if !defined(vxiint_h)
#define vxiint_h

/*****************************************************************************
 *   The following are parameter values and useful defines for VXI interrupt
 * and VXI signal functions.
 */

/* "mask" for Message-Based device in RouteSignal, SignalDeq, and
 * WaitForSignal functions (other bits are reserved)
 */

/* Event Signals */
#define UDE_SIGNAL               (1U<<14)
#define VRE_SIGNAL               (1U<<13)
#define SME_SIGNAL               (1U<<12)
#define URC_SIGNAL               (1U<<11) /* MUST ALWAYS ROUTE TO HANDLER */
#define REQF_SIGNAL              (1U<<10)
#define REQT_SIGNAL              (1U<<9)
#define NCG_SIGNAL               (1U<<8)

/* Response signals */
#define NOT_USED                 (1U<<7)
#define B14_SIGNAL               (1U<<6)
#define DOR_SIGNAL               (1U<<5)
#define DIR_SIGNAL               (1U<<4)
#define ERR_SIGNAL               (1U<<3)
#define RR_SIGNAL                (1U<<2)
#define WR_SIGNAL                (1U<<1)
#define FHS_SIGNAL               (1U<<0)

/* define's for use in Signal interrupt handler to case signal values */

/* bit 15=0 signifies a Response signal */
#define Response_B14             (1U<<14)
#define Response_DOR             (1U<<13)
#define Response_DIR             (1U<<12)
#define Response_ERR             (1U<<11)
#define Response_RR              (1U<<10)
#define Response_WR              (1U<<9)
#define Response_FHS             (1U<<8)

/* bit 15=1 signifies an Event signal */
#define SIG_REQT                 0xFD00
#define SIG_REQF                 0xFC00
#define SIG_UNSUP_CMD            0xEE00
#define SIG_NO_CAUSE             0xFF00
#define SIG_RES_EVENT            (1U<<14)       /* bit 14 = 1 */
#define SIG_USER_EVENT           (1U<<14)       /* bit 14 = 0 */

#define SIG_UNREC_CMD            SIG_UNSUP_CMD  /* backwards compatability */

/*****************************************************************************
 *   The following are return codes for all of the VXI interrupt and
 * VXI signal functions.
 */

/* General defines for all VXI interrupt and signal functions */
#define INT_SRC_NOT_SUPPORTED    NO_HARDWARE_SUPPORT
#define ENABLE_OK_QUEUE_FULL     ( 1)
#define INVALID_LA               (-1)
#define INVALID_LEVEL            (-3)

/* EnableSignalInt, DisableSignalInt */
#define SIGINT_ENA_OK            NIVXI_OK       /* zero */
#define SIGINT_DIS_OK            NIVXI_OK       /* zero */
#define SIGINT_ENA_QUEUE_FULL    ENABLE_OK_QUEUE_FULL

/* GetSignalHandler LA parameter */
#define SIGH_GET_UNKNOWN         -2             /* Get unknown LA handler */

/* GetSignalHandler "func" return codes */
#define SIGH_GET_INVLA           0L

/* SetSignalHandler LA parameter */
#define SIGH_SET_UNKNOWN         -2             /* Set unknown LA handler */

/* SetSignalHandler return codes */
#define SIGH_SET_OK              NIVXI_OK
#define SIGH_SET_INVLA           INVALID_LA

/* RouteSignal */
#define ROUTES_OK                NIVXI_OK       /* zero */
#define ROUTES_INVLA             INVALID_LA

/* SignalDeq, SignalEnq, SignalJam */
#define SIGQ_OK                  NIVXI_OK       /* zero */
#define SIGQ_EMPTY               (-1)
#define SIGQ_FULL                (-1)
#define SIGQ_NOMATCH             (-1)

/* WaitForSignal */
#define WAITS_OK                 NIVXI_OK       /* zero */
#define WAITS_INVLA              INVALID_LA
#define WAITS_TIMEOUT            (-2)

/* AcknowledgeVXIint */
#define ACKINT_OK                NIVXI_OK       /* zero */
#define ACKINT_UNSUP             INT_SRC_NOT_SUPPORTED
#define ACKINT_INVCONT           INVALID_CONTROLLER
#define ACKINT_INVLEV            INVALID_LEVEL
#define ACKINT_BERR              (-4)

/* AssertVXIint, DeAssertVXIint */
#define VXIINT_OK                NIVXI_OK       /* zero */
#define VXIINT_UNSUP             INT_SRC_NOT_SUPPORTED
#define VXIINT_INVCONT           INVALID_CONTROLLER
#define VXIINT_INVLEV            INVALID_LEVEL
#define VXIINT_PENDING           (-5)

/* EnableVXIint, DisableVXIint, EnableVXItoSignalInt, DisableVXItoSignalInt */
#define VXIINT_ENA_OK            NIVXI_OK       /* zero */
#define VXIINT_ENA_UNSUP         INT_SRC_NOT_SUPPORTED
#define VXIINT_ENA_INVCONT       INVALID_CONTROLLER
#define VXIINT_DIS_OK            NIVXI_OK       /* zero */
#define VXIINT_DIS_UNSUP         INT_SRC_NOT_SUPPORTED
#define VXIINT_DIS_INVCONT       INVALID_CONTROLLER
#define VXIINT_ENA_QUEUE_FULL    ENABLE_OK_QUEUE_FULL

/* GetVXIintHandler "func" return codes */
#define VXIINT_GET_INVLEV        0L

/* SetVXIintHandler */
#define VXIINT_SET_OK            NIVXI_OK       /* zero */

/* RouteVXIint */
#define ROUTEI_OK                NIVXI_OK       /* zero */
#define ROUTEI_UNSUP             INT_SRC_NOT_SUPPORTED
#define ROUTEI_INVCONT           INVALID_CONTROLLER

/* VXIintAcknowledgeMode */
#define ACKMODE_OK               NIVXI_OK      /* zero */
#define ACKMODE_UNSUP            INT_SRC_NOT_SUPPORTED
#define ACKMODE_INVCONT          INVALID_CONTROLLER

#endif

