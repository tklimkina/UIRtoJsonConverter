/**************************************************************************/
/*                                                                        */
/*             National Instruments NI-VXI Software Interface             */
/*                   (C) 1996-1999,  National Instruments                 */
/*                                                                        */
/*                        Trigger Include File                            */
/*                                                                        */
/**************************************************************************/
/*                       DO NOT MODIFY THIS FILE                          */
/**************************************************************************/
/* The following file contains all the necessary trigger information
 * for use of the National Instruments NI-VXI Software Interface functions.
 */

#if !defined(trig_h)
#define trig_h

/*****************************************************************************
 *   The following are parameter values for the NI-VXI Trigger functions.
 */

/* "line" parameter for all functions as applicable */
#define TRIG_LINE_TTL_BASE         (0)
#define TRIG_LINE_ECL_BASE         (8)

#define TRIG_LINE_STARX_BASE       (14)   /* currently unavailable */
#define TRIG_LINE_STARY_BASE       (27)   /* currently unavailable */

#define TRIG_LINE_GPIO_BASE        (40)
#define TRIG_LINE_GPIO(g)          (TRIG_LINE_GPIO_BASE+g)
#define TRIG_LINE_FPIN             (40)
#define TRIG_LINE_FPOUT            (41)

#define TRIG_LINE_TCNTR            (50)   /* TIC counter output    */
#define TRIG_LINE_GCNTR            (51)   /* TIC finished output   */
#define TRIG_LINE_TICK1            (60)   /* tick timer 1 output   */
#define TRIG_LINE_TICK2            (61)   /* tick timer 2 output   */
#define TRIG_LINE_CLK10            (70)   /* 10 MHz clock          */
#define TRIG_LINE_EXTCLK           (71)   /* current EXTCLK in TIC */

/* "line" for CNTR and TICK for SetTrigHandler */
#define TABLE_ENTRY_CNTR           (14)
#define TABLE_ENTRY_TICK           (15)

/* "prot" in SrcTrig, EnableTrigSense */
#define TRIG_PROT_ON               (0)
#define TRIG_PROT_OFF              (1)
#define TRIG_PROT_START            (2)
#define TRIG_PROT_STOP             (3)
#define TRIG_PROT_SYNC             (4)
#define TRIG_PROT_SEMISYNC         (5)
#define TRIG_PROT_ASYNC            (6)
#define TRIG_PROT_SEMISYNC_ACK     (7)
#define TRIG_PROT_ASYNC_ACK        (8)
#define TRIG_PROT_ABORT_ACK        (0xFFFF)

/* additional "prot" in EnableTrigSense */
#define TRIG_PROT_PSOVERRUN        (0x8000)

/* "srcTrig", "destTrig" in MapTrigToTrig and UnMapTrigToTrig */
#define TRIG_MAP_TTLBASE           (0)
#define TRIG_MAP_ECLBASE           (8)
#define TRIG_MAP_STARXBASE         (14)
#define TRIG_MAP_STARYBASE         (27)
#define TRIG_MAP_EXTBASE           (40)
#define TRIG_MAP_FPIN              (40)
#define TRIG_MAP_FPOUT             (41)

/* "mode" in MapTrigToTrig */
#define MAP_MODE_SYNC              (1U<<0)
#define MAP_MODE_INVERT            (1U<<1)
#define MAP_MODE_1CLK              (1U<<2)
#define MAP_MODE_EXTCLK            (1U<<3)

/* "TrigType" definitions for TrigHandler, DefaultTrigHandler */
#define TRIG_TYPE_SENSED           (1U<<0)
#define TRIG_TYPE_ACK_RECV         (0)    /* opposite of SENSED: !(1U<<0) */
#define TRIG_TYPE_ERROR            (1U<<15)
#define TRIG_TYPE_ASSERT_OVERRUN   (TRIG_TYPE_ERROR | (1U<<2))
#define TRIG_TYPE_UNASSERT_OVERRUN (TRIG_TYPE_ERROR | (1U<<3))
#define TRIG_TYPE_PSTRECTH_OVERRUN (TRIG_TYPE_ERROR | (1U<<4))

/* "mode" in TrigAssertConfig (general: line = -1) */
#define ASSERT_MODE_CLK10_FALLING   (1U<<0)
#define ASSERT_MODE_CLK10_RISING    (0)

/* "mode" in TrigAssertConfig (specific line number) */
#define ASSERT_MODE_CLK10_NOSYNC    (1U<<0)
#define ASSERT_MODE_CLK10_SYNC      (0)
#define ASSERT_MODE_EXTSSARM        (1U<<1)

/* "mode" in TrigExtConfig */
#define EXT_MODE_NOFEEDBACK         (0)
#define EXT_MODE_FEEDBACK           (1U<<0)
#define EXT_MODE_TRISTATE           (0)
#define EXT_MODE_ASSERT             (1U<<1)
#define EXT_MODE_ASSERT_LOW         (1U<<2)
#define EXT_MODE_ASSERT_HIGH        (0)
#define EXT_MODE_INVERT_INP         (1U<<3)

/* "mode" in TrigCntrConfig */
#define CNTR_MODE_INIT              (0)
#define CNTR_MODE_RELOAD            (2)
#define CNTR_MODE_DISABLE           (3)

/* "mode" in TrigTickConfig */
#define TICK_MODE_INIT_ROLL         (0)
#define TICK_MODE_INIT_NOROLL       (1)
#define TICK_MODE_RELOAD            (2)
#define TICK_MODE_DISABLE           (3)

/*****************************************************************************
 *   The following are return codes for all of the NI-VXI Trigger functions
 */

/* General defines for all NI-VXI Trigger functions */
#define NIVXI_OK_NO_NEED_TO_CALL    ( 1)
#define NIVXI_OK                    (0)
#define TRIGS_NOT_SUPPORTED         NO_HARDWARE_SUPPORT
#define INVALID_CONTROLLER          (-2)
#define INVALID_LINE_OR_PROT        (-3)
#define LINE_NOT_SUPPORTED          (-4)
#define PROT_NOT_SUPPORTED          (-5)
#define TRIG_WAIT_TIMEOUT           (-6)
#define LINE_ALREADY_CONFIGURED     (-7)
#define UNSUPPORTED_SOURCE_LINE     (-8)
#define UNSUPPORTED_DEST_LINE       (-9)
#define INVALID_CONFIG_MODE         (-10)
#define LINE_ALREADY_MAPPED         (-11)
#define LINE_NOT_CONFIGURED         (-12)
#define INVALID_COUNT               (-13)
#define INVALID_MAP_MODE            (-14)
#define OPERATION_PENDING           (-15)
#define ACKNOWLEDGE_PENDING         (-16)
#define NO_TRIGGER_SENSED           (-17)

/* AcknowledgeTrig */
#define TRIG_ACK_OK_NO_NEED         NIVXI_OK_NO_NEED_TO_CALL
#define TRIG_ACK_OK                 NIVXI_OK       /* zero */
#define TRIG_ACK_UNSUP              TRIGS_NOT_SUPPORTED
#define TRIG_ACK_INVCONT            INVALID_CONTROLLER
#define TRIG_ACK_INVLINE            INVALID_LINE_OR_PROT
#define TRIG_ACK_UNSUP_LINE         LINE_NOT_SUPPORTED
#define TRIG_ACK_NOT_CONFIGURED     LINE_NOT_CONFIGURED
#define TRIG_ACK_NOT_SENSED         NO_TRIGGER_SENSED
#define TRIG_ACK_NOT_EXT_SEMISYNC   (-18)

/* EnableTrigSense, DisableTrigSense */
#define TRIG_SENSE_OK               NIVXI_OK       /* zero */
#define TRIG_SENSE_UNSUP            TRIGS_NOT_SUPPORTED
#define TRIG_SENSE_INVCONT          INVALID_CONTROLLER
#define TRIG_SENSE_INVLINE          INVALID_LINE_OR_PROT
#define TRIG_SENSE_UNSUP_LINE       LINE_NOT_SUPPORTED
#define TRIG_SENSE_NOT_CONFIGURED   LINE_NOT_CONFIGURED

/* additional for EnableTrigSense */
#define TRIG_SENSE_UNSUP_PROT       PROT_NOT_SUPPORTED
#define TRIG_SENSE_PREV_CONF        LINE_ALREADY_CONFIGURED
#define TRIG_SENSE_CNTR_PEND        OPERATION_PENDING

/* GetTrigHandler "func" return codes */
#define TRIG_GET_INVLINE            (0)

/* SetTrigHandler */
#define TRIG_SET_OK                 NIVXI_OK       /* zero */
#define TRIG_SET_UNSUP              TRIGS_NOT_SUPPORTED

/* MapTrigToTrig, UnMapTrigToTrig */
#define TRIG_MAP_OK                 NIVXI_OK       /* zero */
#define TRIG_MAP_UNSUP              TRIGS_NOT_SUPPORTED
#define TRIG_MAP_INVCONT            INVALID_CONTROLLER
#define TRIG_MAP_UNSUP_SRC          UNSUPPORTED_SOURCE_LINE
#define TRIG_MAP_UNSUP_DEST         UNSUPPORTED_DEST_LINE
#define TRIG_MAP_UNSUP_MODE         INVALID_CONFIG_MODE
#define TRIG_MAP_PREV_MAP           LINE_ALREADY_MAPPED
#define TRIG_MAP_NPREV_MAP          LINE_NOT_CONFIGURED

/* SrcTrig */
#define TRIG_SRC_OK                 NIVXI_OK       /* zero */
#define TRIG_SRC_UNSUP              TRIGS_NOT_SUPPORTED
#define TRIG_SRC_INVCONT            INVALID_CONTROLLER
#define TRIG_SRC_INVLINE            INVALID_LINE_OR_PROT
#define TRIG_SRC_INVPROT            INVALID_LINE_OR_PROT
#define TRIG_SRC_UNSUP_LINE         LINE_NOT_SUPPORTED
#define TRIG_SRC_UNSUP_PROT         PROT_NOT_SUPPORTED
#define TRIG_SRC_TIMEOUT_ACK        WAIT_TIMEOUT
#define TRIG_SRC_PREV_CONF          LINE_ALREADY_CONFIGURED
#define TRIG_SRC_ACK_PEND           ACKNOWLEDGE_PENDING
#define TRIG_SRC_CNTR_PEND          OPERATION_PENDING
#define TRIG_SRC_NOT_CONFIGURED     LINE_NOT_CONFIGURED

/* TrigAssertConfig, TrigExtConfig, TrigCntrConfig, TrigTickConfig */
#define TRIG_CONFIG_OK_DISABLE      (3)
#define TRIG_CONFIG_OK_RELOAD       (2)
#define TRIG_CONFIG_OK_INIT_NOROLL  (1)
#define TRIG_CONFIG_OK_INIT         NIVXI_OK       /* zero */
#define TRIG_CONFIG_UNSUP           TRIGS_NOT_SUPPORTED
#define TRIG_CONFIG_INVCONT         INVALID_CONTROLLER
#define TRIG_CONFIG_INVMODE         INVALID_CONFIG_MODE
#define TRIG_CONFIG_INVLINE         INVALID_LINE_OR_PROT
#define TRIG_CONFIG_NOT_CONFIGURED  LINE_NOT_CONFIGURED
#define TRIG_CONFIG_CNTR_PEND       OPERATION_PENDING
#define TRIG_CONFIG_UNSUP_SOURCE    UNSUPPORTED_SOURCE_LINE

/* WaitForTrig */
#define TRIG_WAIT_OK                NIVXI_OK       /* zero */
#define TRIG_WAIT_UNSUP             TRIGS_NOT_SUPPORTED
#define TRIG_WAIT_INVCONT           INVALID_CONTROLLER
#define TRIG_WAIT_INVLINE           INVALID_LINE_OR_PROT
#define TRIG_WAIT_TIMEOUT_ACK       WAIT_TIMEOUT
#define TRIG_WAIT_NOT_CONFIGURED    LINE_NOT_CONFIGURED

#endif

