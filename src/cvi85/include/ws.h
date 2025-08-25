/****************************************************************************/
/*                                                                          */
/*            National Instruments NI-VXI Software Interface                */
/*                   (C) 1996-1999, National Instruments                    */
/*                                                                          */
/*                       Word Serial Include File                           */
/*                                                                          */
/****************************************************************************/
/*                        DO NOT MODIFY THIS FILE                           */
/****************************************************************************/
/*  The following file contains all of the necessary include file
 * information for use of the National Instruments NI-VXI Commander Word
 * Serial and Servant Word Serial functions.
 */

#if !defined(ws_h)
#define ws_h

    /******************************/
    /* WORD SERIAL COMMAND VALUES */
    /******************************/

#define WS_BYTEAV_0          0xbc00    /* Byte Available without END           */
#define WS_BYTEAV_1          0xbd00    /* Byte Available with END              */
#define WS_BYTEAV_END        (1U<<8)   /* Byte Available END position          */
#define WS_BYTEAV_MASK       0xfe00    /* Upper 8 bits minus END bit           */
#define WS_BYTERQ            0xdeff    /* Byte Request command                 */
#define WS_BYTERQ_END        (1U<<8)   /* Byte Request END position            */

#define WS_CLEAR             0xffff    /* Device Clear                         */
#define WS_TRIGGER           0xedff    /* Trigger                              */
#define WS_READ_SERV         0xceff    /* Read Servant Area                    */
#define WS_DEVGRANT          0xbf00    /* Device Grant (upper 8 bits)          */
#define WS_REL_DEVICE        0x8e00    /* Release device (upper 8 bits)        */
#define WS_ID_COMMANDER      0xbe00    /* Identify commander (upper 8 bits)    */
#define WS_READ_STB          0xcfff    /* Read Status Byte                     */

#define WS_ASSIGN_HNDLR      0xa900    /* Assign Handler (VXI Interrupts)      */
#define WS_READ_HNDLRS       0xc7ff    /* Read Handlers (Number Int Handlers)  */
#define WS_READ_LINE         0x8c00    /* Read Handler Line                    */

#define WS_ASSIGN_INT        0xaa00    /* Assign Interrupter (VXI Interrupts)  */
#define WS_READ_INTS         0xcaff    /* Read Interrupters (Num Interrupters) */
#define WS_READ_INTLINE      0x8d00    /* Read Interrupter Line                */

#define WS_READ_MODID        0xccff    /* Read MODID                           */
#define WS_LOWER_MODID       0xae00    /* Set Lower MODID                      */
#define WS_UPPER_MODID       0xad00    /* Set Upper MODID                      */
#define WS_MODID_ENA         0x0080    /* Modid Enable bit for set upper/lower */
#define M_ENABLE             (1U<<13)  /* Modid Enable bit for RBD and extender*/

#define WS_BEGIN_NORM_A      0xfdff    /* Begin Normal Operation-- AUTO        */
#define WS_BEGIN_NORM        0xfcff    /* Begin Normal Operation-- no AUTO     */
#define WS_ENO               0xc9ff    /* End Normal Operation                 */
#define WS_ANO               0xc8ff    /* Abort Normal Operation               */

#define WS_ASYNC_MODE        0xa800    /* Asynchronous Mode Control            */
#define ASYNC_RESP_ENA       0x0008    /* 0 = Response generation enabled      */
#define ASYNC_EVENT_ENA      0x0004    /* 0 = Event generation enabled         */
#define ASYNC_RESP_INT       0x0002    /* 0 = Response via ints, 1 = signals   */
#define ASYNC_EVENT_INT      0x0001    /* 0 = Event via ints, 1 = signals      */

#define WS_CTRL_EVENT        0xaf00    /* Control Event (ena/dis event gen)    */
#define WS_RESP_ENA          0x8f00    /* Response enable (Control Response)   */
#define WS_CONT_RESP         0x8f00    /* Response enable (Control Response)   */
#define RESP_DOR             0x0020    /* Enable Data Out Ready interrupt      */
#define RESP_DIR             0x0010    /* Enable Data In Ready interrupt       */
#define RESP_ERR             0x0008    /* Enable ERR bit interrupt             */
#define RESP_RR              0x0004    /* Enable Read Ready interrupt          */
#define RESP_WR              0x0002    /* Enable Write Ready interrupt         */
#define RESP_FHS             0x0001    /* Enable Fast Handshake Active inter   */

#define WS_RD_PROT_ERR       0xcdff    /* Read Protocol Error (Error query)    */
#define PROT_E_NO_ERROR      0xffff    /* No error Protocol Error              */
#define PROT_E_GENERIC       0xfffe    /* Generic error (not in Spec 1.3)      */
#define PROT_E_MQE           0xfffd    /* Multiple Query Error                 */
#define PROT_E_UNSUP_COM     0xfffc    /* Unsupported command                  */
#define PROT_E_DIR_VIOL      0xfffb    /* DIR violation (BAV sent when not DIR)*/
#define PROT_E_DOR_VIOL      0xfffa    /* DOR violation (BRQ sent when not DOR)*/
#define PROT_E_RR_VIOL       0xfff9    /* RR violation (resp read when not RR) */
#define PROT_E_WR_VIOL       0xfff8    /* WR violation (Cmd wrote when not WR) */

#define WS_READ_PROT         0xdfff    /* Read Protocols                       */
#define PROT_DEVDEP          0x87ff    /* Mask for Device dependent            */
#define PROT_RESERVED        0xf8ff    /* Mask for reserved                    */
#define PROT_SPEC_1_2        (1U<<15)  /* 0 = VXI Spec 1.2 device              */
#define PROT_RG              (1U<<9)   /* 0 = Response generation              */
#define PROT_EG              (1U<<8)   /* 0 = Event generation                 */
#define PROT_E               (1U<<7)   /* 0 = ERR bit supported                */
#define PROT_PI              (1U<<6)   /* 0 = WS Interrupt commands            */
#define PROT_PH              (1U<<5)   /* 0 = WS Int Handler commands          */
#define PROT_TRG             (1U<<4)   /* 0 = WS Trigger command               */
#define PROT_I4              (1U<<3)   /* 0 = 488.2 supported                  */
#define PROT_I               (1U<<2)   /* 0 = VXI Instrument prot              */
#define PROT_ELW             (1U<<1)   /* 0 = WS Extended Long word            */
#define PROT_LW              (1U<<0)   /* 0 = WS Long word protocol            */

    /**************************************/
    /* Word Serial command fields         */
    /**************************************/

/* value definitions for 'respflag' field */
#define WS_RESPONSE          1         /* Command requires a reponse           */
#define WS_NORESPONSE        0         /* Command has no response              */

    /*************************************/
    /* Word Serial command RETURN VALUES */
    /*************************************/

/* Bit definitions for 'ret' return code for commander driver */

/* successful return codes (ERRORFLAG cleared, IODONE set) */
#define WS_RET_IODONE        (1U<<0)   /* I/O successfully completed           */

/* unsuccessful return codes (ERRORFLAG set) */
#define WS_RET_TIMO_SEND     (1U<<1)   /* I/O timed out on SEND of command     */
#define WS_RET_TIMO_REC      (1U<<2)   /* I/O timed out on RESPONSE for command*/
#define WS_RET_ForcedAbort   (1U<<4)   /* WSabort called with ForcedAbort      */
#define WS_RET_InvalidLA     (1U<<5)   /* Invalid Logical Address to talk to   */
#define WS_RET_MQE           (1U<<6)   /* Mulitple Query Error                 */
#define WS_RET_BERR          (1U<<7)   /* Bus error                            */
#define WS_RET_UnSupCom      (1U<<9)   /* Unsupported Cmd event/protocol error */
#define WS_RET_RDERR_NSUP    (1U<<10)  /* Read Protocol Error (not supported)  */
#define WS_RET_DIR_VIOL      (1U<<11)  /* DIR viol (BAV sent when not DIR)     */
#define WS_RET_DOR_VIOL      (1U<<12)  /* DOR viol (BRQ sent when not DOR)     */
#define WS_RET_RR_VIOL       (1U<<13)  /* Response was read when not RR        */
#define WS_RET_WR_VIOL       (1U<<14)  /* Command was written while not WR     */
#define WS_ERRORFLAG         (1U<<15)  /* Error Summary flag                   */

    /***************************/
    /* Word Serial ABORT codes */
    /***************************/

/* WSabort function 'abortop' parameter codes */
#define WS_ABORT_ForcedAbort     1     /* Forced Abort: WSwrt,WSrd,WStrg       */
#define WS_ABORT_UnRecCom        2     /* Unrecognized: WScmd/WSLcmd/WSEcmd    */
#define WS_ABORT_ForcedAbortCom  3     /* Forced Abort: WScmd/WSLcmd/WSEcmd    */
#define WS_ABORT_ForcedAbortAll  4     /* Forced Abort: All WS operations      */
#define WS_ABORT_AsyncAbortAll   5     /* Forced Abort: All WS ops immediately */

/* WSabort function return codes */
#define WS_ABORT_OK          NIVXI_OK
#define WS_ABORT_INVLA       (-1)
#define WS_ABORT_INVOP       (-2)

    /************************************************/
    /* Word Serial Buffer MODE for Sending/Receving */
    /************************************************/

/* Bit definitions for 'mode' field */
#define WSb_MODE_NO_DirDorAbort (1U<<0)/* Poll while !DIR(writes), !DOR(reads)*/
#define WSb_MODE_SendEnd     (1U<<1)   /* Send end with last byte of transfer */
#define WSb_MODE_NoEndEnd    (1U<<1)   /* 0 = end transfer on END bit         */
#define WSb_MODE_EndLF       (1U<<2)   /* End transfer on LF (linefeed)       */
#define WSb_MODE_EndCR       (1U<<3)   /* End transfer on CR (carraige return)*/
#define WSb_MODE_EndEOS      (1U<<4)   /* End transfer on EOS character       */

    /*********************************************************/
    /* Word Serial Buffer RETURN VALUES for Sending/Receving */
    /*********************************************************/

/* Bit definitions for 'ret' return code for commander driver */

/* successful (ERRORFLAG cleared, IODONE set) */
#define WSb_RET_IODONE       (1U<<0)   /* I/O successfully completed           */
#define WSb_RET_END          (1U<<1)   /* END was received on Read             */
#define WSb_RET_TC           (1U<<2)   /* Terminal Count--buffer was filled    */
#define WSb_RET_DirDorAbort  (1U<<3)   /* Device was !DIR(writes), !DOR(reads) */

/* unsuccessful (ERRORFLAG set) */
#define WSb_RET_FopenError   (1U<<0)   /* Error opening file                   */
#define WSb_RET_FwriteError  (1U<<1)   /* Error writing to file                */
#define WSb_RET_ForcedAbort  (1U<<4)   /* WSabort called with ForcedAbort      */
#define WSb_RET_InvalidLA    (1U<<5)   /* Invalid Logical Address to talk to   */
#define WSb_RET_MQE          (1U<<6)   /* Mulitple Query Error                 */
#define WSb_RET_BERR         (1U<<7)   /* Bus error occurred during transfer   */
#define WSb_RET_TIMO         (1U<<8)   /* A timeout occurred during transfer   */
#define WSb_RET_UnSupCom     (1U<<9)   /* Unsupported Cmd event/protocol error */
#define WSb_RET_RDERR_NSUP   (1U<<10)  /* Read Protocol Error (not supported)  */
#define WSb_RET_DIR_VIOL     (1U<<11)  /* DIR viol (BAV sent when not DIR)     */
#define WSb_RET_DOR_VIOL     (1U<<12)  /* DOR viol (BRQ sent when not DOR)     */
#define WSb_RET_RR_VIOL      (1U<<13)  /* Response was read when not RR        */
#define WSb_RET_WR_VIOL      (1U<<14)  /* Command was written while not WR     */
#define WSb_ERRORFLAG        (1U<<15)  /* Error Summary flag                   */

/******************************************************************************/
/****************  S E R V A N T   W O R D   S E R I A L  *********************/
/******************************************************************************/

    /****************************************/
    /* Word Serial Servant Response Sending */
    /****************************************/

/* General */
#define WSS_NOT_SUPPORTED    NO_HARDWARE_SUPPORT

/* MODE parameter */
#define WSS_MODE_DIR         (1U<<0)   /* Send DIR (reads) Response int/signal */
#define WSS_MODE_DOR         (1U<<0)   /* Send DOR (writes) Response int/signal*/
#define WSS_MODE_END         (1U<<1)   /* Send END bit with last byte (writes) */

/* GenProtError return values */
#define WSS_RET_ERR_PENDING  1         /* Previous error is pending            */

/* WSSrd, WSSwrt, WSSsendResp, WSSLsendResp return values */
#define WSS_RET_SUCCESSFUL   NIVXI_OK  /* I/O successfully completed           */
#define WSS_RET_OK_NOT_ENABLED 1       /* Must call WSSenable to start IO      */
#define WSS_RET_MQE          -2        /* Multiple Query Error--reponse pending*/

    /****************************************/
    /* Word Serial Servant ABORT type codes */
    /****************************************/

/* WSSabort function 'abortop' parameter codes */
#define WSS_ABORT_Write      (1U<<0)   /* Abort WSSwrt                         */
#define WSS_ABORT_Read       (1U<<1)   /* Abort WSSrd                          */
#define WSS_ABORT_Response   (1U<<2)   /* Abort WSSsendResp,WSSLsendResp       */
#define WSS_ABORT_All        (1U<<15)  /* Abort all WS/WSS operations          */

    /*****************************************************************/
    /* Word Serial Servant Buffer RETURN VALUES for Sending/Receving */
    /****************************************************************/

/* Bit definitions for 'ret' return code for servant driver */

/* successful (ERRORFLAG cleared, IODONE set) */
#define WSSb_RET_IODONE      (1U<<0)   /* I/O successfully completed           */
#define WSSb_RET_END         (1U<<1)   /* END was received on Read             */
#define WSSb_RET_TC          (1U<<2)   /* Terminal Count--buffer was filled    */

/* unsuccessful (ERRORFLAG set) */
#define WSSb_RET_ForcedAbort (1U<<4)   /* WSabort called with ForcedAbort      */
#define WSSb_RET_DIR_VIOL    (1U<<11)  /* DIR viol (BAV sent when not DIR)     */
#define WSSb_RET_DOR_VIOL    (1U<<12)  /* DOR viol (BRQ sent when not DOR)     */
#define WSSb_RET_RR_VIOL     (1U<<13)  /* Response was read when not RR        */
#define WSSb_RET_WR_VIOL     (1U<<14)  /* Command was written while not WR     */
#define WSSb_ERRORFLAG       (1U<<15)  /* Error Summary flag                   */

#endif

