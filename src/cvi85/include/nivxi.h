/************************************************************************/
/*                                                                      */
/*             National Instruments NI-VXI Software Interface           */
/*                   (C) 1996-1999,  National Instruments               */
/*                                                                      */
/*                         Prototype Definitions                        */
/*                                                                      */
/************************************************************************/
/*                       DO NOT MODIFY THIS FILE                        */
/************************************************************************/
/* The following file contains all of the prototype definitions
 * for the NI-VXI software specification interface.
 */

#if !defined(nivxi_h)
#define nivxi_h

#if defined(__cplusplus) || defined(__cplusplus__)
   extern "C" {
#endif

#include "datasize.h"

#if defined(VXINT)
#pragma pack(push)
#pragma pack(2)
#endif

#include "busacc.h"
#include "devinfo.h"
#include "sysint.h"
#include "trig.h"
#include "vxiint.h"
#include "ws.h"

/* === Return codes common to all NI-VXI functions ==================== */

#if !defined(NIVXI_OK)
#define NIVXI_OK              (NIVXI_STATUS)( 0)
#endif

#define NO_HARDWARE_SUPPORT   (NIVXI_STATUS)(-1)
#define INVALID_MAINFRAME     (NIVXI_STATUS)(-2)
#define NIVXI_SYSTEM_ERROR    (NIVXI_STATUS)(0x8000)

#if defined(_CVI_)
#pragma EnableLibraryRuntimeChecking
#endif

/* === Global Variables =============================================== */

#if defined(VXIWIN) || defined(VXIWIN32) || defined(VXINT)

typedef struct tag_global_struct
{
            INT32  d_MyResmanTmo;
            UINT16 d_MyServArea;
            UINT16 d_MyProtReg;
            UINT16 d_MyReadProtResp;
            UINT16 d_MyNumHandlers;
            UINT16 d_MyNumInterrupters;
   volatile UINT16 d_MyStatusByte;
   volatile INT16  d_VXIintController;
   volatile UINT16 d_VXIintLevel;
   volatile UINT32 d_VXIintStatusId;
   volatile UINT32 d_ClockRecv;
   volatile UINT32 d_SoftResetRecv;
   volatile UINT32 d_SysfailRecv;
   volatile UINT32 d_ACfailRecv;
   volatile UINT32 d_BusErrorRecv;
   volatile UINT32 d_SysresetRecv;
   volatile UINT16 d_WSSwrtDone;
   volatile INT16  d_WSSwrtDoneStatus;
   volatile UINT32 d_WSSwrtDoneCount;
   volatile UINT16 d_WSSrdDone;
   volatile INT16  d_WSSrdDoneStatus;
   volatile UINT32 d_WSSrdDoneCount;
            UINT16 d_VXI_type;
   volatile UINT16 d_Bus_Error_Flag;
   volatile UINT16 d_Retry_Interrupt_Flag;
            UINT16 d_Board_Status_Loc;
            UINT16 d_Int_Status_Loc;
   volatile UINT16 d_INTCNTRL_COPY;
   volatile UINT16 d_MyNormalOperation;
            UINT16 d_MyHandlerToVME1;
            UINT16 d_MyHandlerToVME2;
            UINT16 d_MyHandlerToVME3;
            UINT16 d_MyHandlerToVME4;
            UINT16 d_MyHandlerToVME5;
            UINT16 d_MyHandlerToVME6;
            UINT16 d_MyHandlerToVME7;
            UINT16 d_MyInterrupterToVME1;
            UINT16 d_MyInterrupterToVME2;
            UINT16 d_MyInterrupterToVME3;
            UINT16 d_MyInterrupterToVME4;
            UINT16 d_MyInterrupterToVME5;
            UINT16 d_MyInterrupterToVME6;
            UINT16 d_MyInterrupterToVME7;
   volatile UINT32 d_MyEvents00_1F;
   volatile UINT32 d_MyEvents20_3F;
   volatile UINT32 d_MyEvents40_5F;
   volatile UINT32 d_MyEvents60_7F;
   volatile UINT32 d_WSStrgRecv;
            INT8   d_nivxi_path[256];
} global_struct;

#define MyResmanTmo          global_vars->d_MyResmanTmo
#define MyServArea           global_vars->d_MyServArea
#define MyProtReg            global_vars->d_MyProtReg
#define MyReadProtResp       global_vars->d_MyReadProtResp
#define MyNumHandlers        global_vars->d_MyNumHandlers
#define MyNumInterrupters    global_vars->d_MyNumInterrupters
#define MyStatusByte         global_vars->d_MyStatusByte
#define VXIintController     global_vars->d_VXIintController
#define VXIintLevel          global_vars->d_VXIintLevel
#define VXIintStatusId       global_vars->d_VXIintStatusId
#define ClockRecv            global_vars->d_ClockRecv
#define SoftResetRecv        global_vars->d_SoftResetRecv
#define SysfailRecv          global_vars->d_SysfailRecv
#define ACfailRecv           global_vars->d_ACfailRecv
#define BusErrorRecv         global_vars->d_BusErrorRecv
#define SysresetRecv         global_vars->d_SysresetRecv
#define WSSwrtDone           global_vars->d_WSSwrtDone
#define WSSwrtDoneStatus     global_vars->d_WSSwrtDoneStatus
#define WSSwrtDoneCount      global_vars->d_WSSwrtDoneCount
#define WSSrdDone            global_vars->d_WSSrdDone
#define WSSrdDoneStatus      global_vars->d_WSSrdDoneStatus
#define WSSrdDoneCount       global_vars->d_WSSrdDoneCount
#define VXI_type             global_vars->d_VXI_type
#define Bus_Error_Flag       global_vars->d_Bus_Error_Flag
#define Retry_Interrupt_Flag global_vars->d_Retry_Interrupt_Flag
#define Board_Status_Loc     global_vars->d_Board_Status_Loc
#define Int_Status_Loc       global_vars->d_Int_Status_Loc
#define INTCNTRL_COPY        global_vars->d_INTCNTRL_COPY
#define MyNormalOperation    global_vars->d_MyNormalOperation
#define MyHandlerToVME1      global_vars->d_MyHandlerToVME1
#define MyHandlerToVME2      global_vars->d_MyHandlerToVME2
#define MyHandlerToVME3      global_vars->d_MyHandlerToVME3
#define MyHandlerToVME4      global_vars->d_MyHandlerToVME4
#define MyHandlerToVME5      global_vars->d_MyHandlerToVME5
#define MyHandlerToVME6      global_vars->d_MyHandlerToVME6
#define MyHandlerToVME7      global_vars->d_MyHandlerToVME7
#define MyInterrupterToVME1  global_vars->d_MyInterrupterToVME1
#define MyInterrupterToVME2  global_vars->d_MyInterrupterToVME2
#define MyInterrupterToVME3  global_vars->d_MyInterrupterToVME3
#define MyInterrupterToVME4  global_vars->d_MyInterrupterToVME4
#define MyInterrupterToVME5  global_vars->d_MyInterrupterToVME5
#define MyInterrupterToVME6  global_vars->d_MyInterrupterToVME6
#define MyInterrupterToVME7  global_vars->d_MyInterrupterToVME7
#define MyEvents00_1F        global_vars->d_MyEvents00_1F
#define MyEvents20_3F        global_vars->d_MyEvents20_3F
#define MyEvents40_5F        global_vars->d_MyEvents40_5F
#define MyEvents60_7F        global_vars->d_MyEvents60_7F
#define WSStrgRecv           global_vars->d_WSStrgRecv
#define nivxi_path           global_vars->d_nivxi_path

extern global_struct NIVXI_DT *global_vars;

#else

extern          INT32  MyResmanTmo;
extern          UINT16 MyServArea;
extern          UINT16 MyProtReg;
extern          UINT16 MyReadProtResp;
extern          UINT16 MyNumHandlers;
extern          UINT16 MyNumInterrupters;
extern volatile UINT16 MyStatusByte;
extern volatile INT16  VXIintController;
extern volatile UINT16 VXIintLevel;
extern volatile UINT32 VXIintStatusId;
extern volatile UINT32 ClockRecv;
extern volatile UINT32 SoftResetRecv;
extern volatile UINT32 SysfailRecv;
extern volatile UINT32 ACfailRecv;
extern volatile UINT32 BusErrorRecv;
extern volatile UINT32 SysresetRecv;
extern volatile UINT16 WSSwrtDone;
extern volatile INT16  WSSwrtDoneStatus;
extern volatile UINT32 WSSwrtDoneCount;
extern volatile UINT16 WSSrdDone;
extern volatile INT16  WSSrdDoneStatus;
extern volatile UINT32 WSSrdDoneCount;
extern          UINT16 VXI_type;
extern volatile UINT16 Bus_Error_Flag;
extern volatile UINT16 Retry_Interrupt_Flag;
extern          UINT16 Board_Status_Loc;
extern          UINT16 Int_Status_Loc;
extern volatile UINT16 INTCNTRL_COPY;
extern volatile UINT16 MyNormalOperation;
extern          UINT16 MyHandlerToVME1;
extern          UINT16 MyHandlerToVME2;
extern          UINT16 MyHandlerToVME3;
extern          UINT16 MyHandlerToVME4;
extern          UINT16 MyHandlerToVME5;
extern          UINT16 MyHandlerToVME6;
extern          UINT16 MyHandlerToVME7;
extern          UINT16 MyInterrupterToVME1;
extern          UINT16 MyInterrupterToVME2;
extern          UINT16 MyInterrupterToVME3;
extern          UINT16 MyInterrupterToVME4;
extern          UINT16 MyInterrupterToVME5;
extern          UINT16 MyInterrupterToVME6;
extern          UINT16 MyInterrupterToVME7;
extern volatile UINT32 MyEvents00_1F;
extern volatile UINT32 MyEvents20_3F;
extern volatile UINT32 MyEvents40_5F;
extern volatile UINT32 MyEvents60_7F;
extern volatile UINT32 WSStrgRecv;
extern          INT8   nivxi_path[256];

#endif

#if defined(VXINT)
#pragma pack(pop)
#endif

/* === Main NI-VXI Functions ============================================= */

#if !defined(__NO_PROTOTYPES__)

/* === Begin With-Prototype Function Declarations Section =============== */

/* system configuration functions */

extern NIVXI_STATUS       NIVXI_FN CloseVXIlibrary(void);
extern NIVXI_STATUS       NIVXI_FN CreateDevInfo(INT16 la);
extern NIVXI_STATUS       NIVXI_FN FindDevLA(INT8 NIVXI_DT *namepat,
                                             INT16 manid,
                                             INT16 modelcode,
                                             INT16 devclass,
                                             INT16 slot,
                                             INT16 mainframe,
                                             INT16 cmdrla,
                                             INT16 NIVXI_DT *la);
extern NIVXI_STATUS       NIVXI_FN GetDevInfo(INT16 la,
                                              UINT16 field,
                                              void NIVXI_DT *fieldvalue);
extern NIVXI_STATUS       NIVXI_FN GetDevInfoLong(INT16 la,
                                                  UINT16 field,
                                                  UINT32 NIVXI_DT *longvalue);
extern NIVXI_STATUS       NIVXI_FN GetDevInfoShort(INT16 la,
                                                   UINT16 field,
                                                   UINT16 NIVXI_DT *shortvalue);
extern NIVXI_STATUS       NIVXI_FN GetDevInfoStr(INT16 la,
                                                 UINT16 field,
                                                 UINT8 NIVXI_DT *stringvalue);
extern NIVXI_STATUS       NIVXI_FN InitVXIlibrary(void);
extern NIVXI_STATUS       NIVXI_FN SetDevInfo(INT16 la,
                                              UINT16 field,
                                              void NIVXI_DT *fieldvalue);
extern NIVXI_STATUS       NIVXI_FN SetDevInfoLong(INT16 la,
                                                  UINT16 field,
                                                  UINT32 longvalue);
extern NIVXI_STATUS       NIVXI_FN SetDevInfoShort(INT16 la,
                                                   UINT16 field,
                                                   UINT16 shortvalue);
extern NIVXI_STATUS       NIVXI_FN SetDevInfoStr(INT16 la,
                                                 UINT16 field,
                                                 UINT8 NIVXI_DT *stringvalue);

/* word serial protocol functions -- commander and servant */

extern NIVXI_STATUS       NIVXI_FN GenProtError(UINT16 proterr);
extern NIVXI_HWSSCMD    * NIVXI_FN GetWSScmdHandler(void);
extern NIVXI_HWSSECMD   * NIVXI_FN GetWSSEcmdHandler(void);
extern NIVXI_HWSSLCMD   * NIVXI_FN GetWSSLcmdHandler(void);
extern NIVXI_HWSSRD     * NIVXI_FN GetWSSrdHandler(void);
extern NIVXI_HWSSWRT    * NIVXI_FN GetWSSwrtHandler(void);
extern NIVXI_STATUS       NIVXI_FN RespProtError(void);
extern NIVXI_STATUS       NIVXI_FN SetWSScmdHandler(NIVXI_HWSSCMD *func);
extern NIVXI_STATUS       NIVXI_FN SetWSSEcmdHandler(NIVXI_HWSSECMD *func);
extern NIVXI_STATUS       NIVXI_FN SetWSSLcmdHandler(NIVXI_HWSSLCMD *func);
extern NIVXI_STATUS       NIVXI_FN SetWSSrdHandler(NIVXI_HWSSRD *func);
extern NIVXI_STATUS       NIVXI_FN SetWSSwrtHandler(NIVXI_HWSSWRT *func);
extern NIVXI_STATUS       NIVXI_FN WSabort(INT16 la,
                                           UINT16 abortop);
extern NIVXI_STATUS       NIVXI_FN WSclr(INT16 la);
extern NIVXI_STATUS       NIVXI_FN WScmd(INT16 la,
                                         UINT16 cmd,
                                         UINT16 respflag,
                                         UINT16 NIVXI_DT *response);
extern NIVXI_STATUS       NIVXI_FN WSEcmd(INT16 la,
                                          UINT16 cmdExt,
                                          UINT32 cmd,
                                          UINT16 respflag,
                                          UINT32 NIVXI_DT *response);
extern NIVXI_STATUS       NIVXI_FN WSgetTmo(INT32 NIVXI_DT *actualtimo);
extern NIVXI_STATUS       NIVXI_FN WSLcmd(INT16 la,
                                          UINT32 cmd,
                                          UINT16 respflag,
                                          UINT32 NIVXI_DT *response);
extern NIVXI_STATUS       NIVXI_FN WSLresp(INT16 la,
                                           UINT32 NIVXI_DT *response);
extern NIVXI_STATUS       NIVXI_FN WSrd(INT16 la,
                                        UINT8 NIVXI_DT *buf,
                                        UINT32 count,
                                        UINT16 mode,
                                        UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSrdf(INT16 la,
                                         INT8 NIVXI_DT *filename,
                                         UINT32 count,
                                         UINT16 mode,
                                         UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSrdi(INT16 la,
                                         UINT16 NIVXI_DT *buf,
                                         UINT32 count,
                                         UINT16 mode,
                                         UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSrdl(INT16 la,
                                         UINT32 NIVXI_DT *buf,
                                         UINT32 count,
                                         UINT16 mode,
                                         UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSresp(INT16 la,
                                          UINT16 NIVXI_DT *response);
extern NIVXI_STATUS       NIVXI_FN WSsetTmo(INT32 timo,
                                            INT32 NIVXI_DT *actualtimo);
extern NIVXI_STATUS       NIVXI_FN WStrg(INT16 la);
extern NIVXI_STATUS       NIVXI_FN WSwrt(INT16 la,
                                         UINT8 NIVXI_DT *buf,
                                         UINT32 count,
                                         UINT16 mode,
                                         UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSwrtf(INT16 la,
                                          INT8 NIVXI_DT *filename,
                                          UINT32 count,
                                          UINT16 mode,
                                          UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSwrti(INT16 la,
                                          UINT16 NIVXI_DT *buf,
                                          UINT32 count,
                                          UINT16 mode,
                                          UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSwrtl(INT16 la,
                                          UINT32 NIVXI_DT *buf,
                                          UINT32 count,
                                          UINT16 mode,
                                          UINT32 NIVXI_DT *retcount);
extern NIVXI_STATUS       NIVXI_FN WSSabort(UINT16 abortop);
extern NIVXI_STATUS       NIVXI_FN WSSdisable(void);
extern NIVXI_STATUS       NIVXI_FN WSSenable(void);
extern NIVXI_STATUS       NIVXI_FN WSSLnoResp(void);
extern NIVXI_STATUS       NIVXI_FN WSSLsendResp(UINT32 response);
extern NIVXI_STATUS       NIVXI_FN WSSnoResp(void);
extern NIVXI_STATUS       NIVXI_FN WSSrd(UINT8 NIVXI_DT *buf,
                                         UINT32 count,
                                         UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN WSSrdi(UINT16 NIVXI_DT *buf,
                                          UINT32 count,
                                          UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN WSSrdl(UINT32 NIVXI_DT *buf,
                                          UINT32 count,
                                          UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN WSSsendResp(UINT16 response);
extern NIVXI_STATUS       NIVXI_FN WSSwrt(UINT8 NIVXI_DT *buf,
                                          UINT32 count,
                                          UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN WSSwrti(UINT16 NIVXI_DT *buf,
                                           UINT32 count,
                                           UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN WSSwrtl(UINT32 NIVXI_DT *buf,
                                           UINT32 count,
                                           UINT16 mode);

extern NIVXI_HWSSCMD               DefaultWSScmdHandler;
extern NIVXI_HWSSECMD              DefaultWSSEcmdHandler;
extern NIVXI_HWSSLCMD              DefaultWSSLcmdHandler;
extern NIVXI_HWSSRD                DefaultWSSrdHandler;
extern NIVXI_HWSSWRT               DefaultWSSwrtHandler;

/* VXIbus access functions -- high/low level and local resource access */

extern NIVXI_STATUS       NIVXI_FN ClearBusError(void);
extern NIVXI_STATUS       NIVXI_FN GetByteOrder(UINT32 window,
                                                UINT16 NIVXI_DT *ordermode);
extern NIVXI_STATUS       NIVXI_FN GetContext(UINT32 window,
                                              UINT32 NIVXI_DT *context);
extern INT16              NIVXI_FN GetMyLA(void);
extern NIVXI_STATUS       NIVXI_FN GetPrivilege(UINT32 window,
                                                UINT16 NIVXI_DT *priv);
extern NIVXI_STATUS       NIVXI_FN GetVXIbusStatus(INT16 controller,
                                                   BusStatus NIVXI_DT *status);
extern NIVXI_STATUS       NIVXI_FN GetVXIbusStatusInd(INT16 controller,
                                                      UINT16 field,
                                                      INT16 NIVXI_DT *status);
extern NIVXI_STATUS       NIVXI_FN GetWindowRange(UINT32 window,
                                                  UINT32 NIVXI_DT *windowbase,
                                                  UINT32 NIVXI_DT *windowend);
extern void NIVXI_DT    * NIVXI_FN MapVXIAddress(UINT16 accessparms,
                                                 UINT32 address,
                                                 INT32 timo,
                                                 UINT32 NIVXI_DT *window,
                                                 NIVXI_STATUS NIVXI_DT *ret);
extern NIVXI_STATUS       NIVXI_FN MapVXIAddressSize(UINT32 size);
extern NIVXI_STATUS       NIVXI_FN ReadMODID(UINT16 NIVXI_DT *modid);
extern NIVXI_STATUS       NIVXI_FN RestoreContext(ContextStruct NIVXI_DT *contextlist);
extern NIVXI_STATUS       NIVXI_FN SaveContext(ContextStruct NIVXI_DT *contextlist);
extern NIVXI_STATUS       NIVXI_FN SetByteOrder(UINT32 window,
                                                UINT16 ordermode);
extern NIVXI_STATUS       NIVXI_FN SetContext(UINT32 window,
                                              UINT32 context);
extern NIVXI_STATUS       NIVXI_FN SetMODID(UINT16 enable,
                                            UINT16 modid);
extern NIVXI_STATUS       NIVXI_FN SetPrivilege(UINT32 window,
                                                UINT16 priv);
extern NIVXI_STATUS       NIVXI_FN UnMapVXIAddress(UINT32 window);
extern NIVXI_STATUS       NIVXI_FN VXIin(UINT16 accessparms,
                                         UINT32 address,
                                         UINT16 width,
                                         void NIVXI_DT *value);
extern NIVXI_STATUS       NIVXI_FN VXIinLR(UINT16 reg,
                                           UINT16 width,
                                           void NIVXI_DT *value);
extern NIVXI_STATUS       NIVXI_FN VXIinReg(INT16 la,
                                            UINT16 reg,
                                            UINT16 NIVXI_DT *value);
extern NIVXI_STATUS       NIVXI_FN VXImemAlloc(UINT32 size,
                                               void NIVXI_DT * NIVXI_DT *useraddr,
                                               UINT32 NIVXI_DT *vxiaddr);
extern NIVXI_STATUS       NIVXI_FN VXImemCopy(void NIVXI_DT *useraddr,
                                              void NIVXI_DT *bufaddr,
                                              UINT32 size,
                                              UINT16 dir);
extern NIVXI_STATUS       NIVXI_FN VXImemFree(void NIVXI_DT *useraddr);
extern NIVXI_STATUS       NIVXI_FN VXImove(UINT16 srcparms,
                                           UINT32 srcaddr,
                                           UINT16 destparms,
                                           UINT32 destaddr,
                                           UINT32 length,
                                           UINT16 width);
extern NIVXI_STATUS       NIVXI_FN VXIout(UINT16 accessparms,
                                          UINT32 address,
                                          UINT16 width,
                                          UINT32 value);
extern NIVXI_STATUS       NIVXI_FN VXIoutLR(UINT16 reg,
                                            UINT16 width,
                                            UINT32 value);
extern NIVXI_STATUS       NIVXI_FN VXIoutReg(INT16 la,
                                             UINT16 reg,
                                             UINT16 value);
extern void               NIVXI_FN VXIpeek(void NIVXI_DT *addressptr,
                                           UINT16 width,
                                           void NIVXI_DT *value);
extern void               NIVXI_FN VXIpoke(void NIVXI_DT *addressptr,
                                           UINT16 width,
                                           UINT32 value);

/* signal functions */

extern NIVXI_STATUS       NIVXI_FN DisableSignalInt(void);
extern NIVXI_STATUS       NIVXI_FN EnableSignalInt(void);
extern NIVXI_HSIGNAL    * NIVXI_FN GetSignalHandler(INT16 la);
extern NIVXI_STATUS       NIVXI_FN RouteSignal(INT16 la,
                                               UINT32 modemask);
extern NIVXI_STATUS       NIVXI_FN SetSignalHandler(INT16 la,
                                                    NIVXI_HSIGNAL *func);
extern NIVXI_STATUS       NIVXI_FN SignalDeq(INT16 la,
                                             UINT32 signalmask,
                                             UINT16 NIVXI_DT *signal);
extern NIVXI_STATUS       NIVXI_FN SignalEnq(UINT16 signal);
extern NIVXI_STATUS       NIVXI_FN SignalJam(UINT16 signal);
extern NIVXI_STATUS       NIVXI_FN WaitForSignal(INT16 la,
                                                 UINT32 signalmask,
                                                 INT32 timeout,
                                                 UINT16 NIVXI_DT *retsignal,
                                                 UINT32 NIVXI_DT *retsignalmask);

extern NIVXI_HSIGNAL               DefaultSignalHandler;

/* interrupt functions */

extern NIVXI_STATUS       NIVXI_FN AcknowledgeVXIint(INT16 controller,
                                                     UINT16 level,
                                                     UINT32 NIVXI_DT *statusId);
extern NIVXI_STATUS       NIVXI_FN AssertVXIint(INT16 controller,
                                                UINT16 level,
                                                UINT32 statusId);
extern NIVXI_STATUS       NIVXI_FN DeAssertVXIint(INT16 controller,
                                                  UINT16 level);
extern NIVXI_STATUS       NIVXI_FN DisableVXIint(INT16 controller,
                                                 UINT16 levels);
extern NIVXI_STATUS       NIVXI_FN DisableVXItoSignalInt(INT16 controller,
                                                         UINT16 levels);
extern NIVXI_STATUS       NIVXI_FN EnableVXIint(INT16 controller,
                                                UINT16 levels);
extern NIVXI_STATUS       NIVXI_FN EnableVXItoSignalInt(INT16 controller,
                                                        UINT16 levels);
extern NIVXI_HVXIINT    * NIVXI_FN GetVXIintHandler(UINT16 level);
extern NIVXI_HVXIINT    * NIVXI_FN GetVXIintHandlerController(INT16 controller,
                                  UINT16 level);
extern NIVXI_STATUS       NIVXI_FN RouteVXIint(INT16 controller,
                                               UINT16 Sroute);
extern NIVXI_STATUS       NIVXI_FN SetVXIintHandler(UINT16 levels,
                                                    NIVXI_HVXIINT *func);
extern NIVXI_STATUS       NIVXI_FN SetVXIintHandlerController(INT16 controller,
                                  UINT16 levels,
                                  NIVXI_HVXIINT *func);
extern NIVXI_STATUS       NIVXI_FN VXIintAcknowledgeMode(INT16 controller,
                                                         UINT16 modes);

extern NIVXI_HVXIINT               DefaultVXIintHandler;

/* trigger functions -- all functions with ECL or TTL in the name are
 * outdated,  and are provided for backward compatability only -- new
 * functions with similar names  (without ECL or TTL)  should be used
 * instead, since the old style MAY NOT be supported in the future!!! */

extern NIVXI_STATUS       NIVXI_FN AcknowledgeECLtrig(INT16 controller,
                                                      UINT16 line);
extern NIVXI_STATUS       NIVXI_FN AcknowledgeTrig(INT16 controller,
                                                   UINT16 line);
extern NIVXI_STATUS       NIVXI_FN AcknowledgeTTLtrig(INT16 controller,
                                                      UINT16 line);
extern NIVXI_STATUS       NIVXI_FN DisableECLsense(INT16 controller,
                                                   UINT16 line);
extern NIVXI_STATUS       NIVXI_FN DisableTrigSense(INT16 controller,
                                                   UINT16 line);
extern NIVXI_STATUS       NIVXI_FN DisableTTLsense(INT16 controller,
                                                   UINT16 line);
extern NIVXI_STATUS       NIVXI_FN EnableECLsense(INT16 controller,
                                                  UINT16 line,
                                                  UINT16 prot);
extern NIVXI_STATUS       NIVXI_FN EnableTrigSense(INT16 controller,
                                                   UINT16 line,
                                                   UINT16 prot);
extern NIVXI_STATUS       NIVXI_FN EnableTTLsense(INT16 controller,
                                                  UINT16 line,
                                                  UINT16 prot);
extern NIVXI_HTRIG      * NIVXI_FN GetECLtrigHandler(UINT16 line);
extern NIVXI_HTRIG      * NIVXI_FN GetTrigHandler(UINT16 line);
extern NIVXI_HTRIG      * NIVXI_FN GetTrigHandlerController(INT16 controller,
                                UINT16 line);
extern NIVXI_HTRIG      * NIVXI_FN GetTTLtrigHandler(UINT16 line);
extern NIVXI_STATUS       NIVXI_FN MapTrigToTrig(INT16 controller,
                                                 UINT16 srcTrig,
                                                 UINT16 destTrig,
                                                 UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN SetECLtrigHandler(UINT16 lines,
                                                     NIVXI_HTRIG *func);
extern NIVXI_STATUS       NIVXI_FN SetTrigHandler(UINT16 lines,
                                                  NIVXI_HTRIG *func);
extern NIVXI_STATUS       NIVXI_FN SetTrigHandlerController(INT16 controller,
                                UINT16 lines,
                                NIVXI_HTRIG *func);
extern NIVXI_STATUS       NIVXI_FN SetTTLtrigHandler(UINT16 lines,
                                                     NIVXI_HTRIG *func);
extern NIVXI_STATUS       NIVXI_FN SrcECLtrig(INT16 controller,
                                              UINT16 line,
                                              UINT16 prot,
                                              INT32 timeout);
extern NIVXI_STATUS       NIVXI_FN SrcTrig(INT16 controller,
                                           UINT16 line,
                                           UINT16 prot,
                                           INT32 timeout);
extern NIVXI_STATUS       NIVXI_FN SrcTTLtrig(INT16 controller,
                                              UINT16 line,
                                              UINT16 prot,
                                              INT32 timeout);
extern NIVXI_STATUS       NIVXI_FN TrigAssertConfig(INT16 controller,
                                                    UINT16 line,
                                                    UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN TrigCntrConfig(INT16 controller,
                                                  UINT16 mode,
                                                  UINT16 source,
                                                  UINT16 count);
extern NIVXI_STATUS       NIVXI_FN TrigExtConfig(INT16 controller,
                                                 UINT16 extline,
                                                 UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN TrigTickConfig(INT16 controller,
                                                  UINT16 mode,
                                                  UINT16 source,
                                                  UINT16 tcount1,
                                                  UINT16 tcount2);
extern NIVXI_STATUS       NIVXI_FN UnMapTrigToTrig(INT16 controller,
                                                   UINT16 srcTrig,
                                                   UINT16 destTrig);
extern NIVXI_STATUS       NIVXI_FN WaitForECLtrig(INT16 controller,
                                                  UINT16 line,
                                                  INT32 timeout);
extern NIVXI_STATUS       NIVXI_FN WaitForTrig(INT16 controller,
                                               UINT16 line,
                                               INT32 timeout);
extern NIVXI_STATUS       NIVXI_FN WaitForTTLtrig(INT16 controller,
                                                  UINT16 line,
                                                  INT32 timeout);

extern NIVXI_HTRIG                 DefaultECLtrigHandler;
extern NIVXI_HTRIG                 DefaultTrigHandler;
extern NIVXI_HTRIG                 DefaultTrigHandler2;
extern NIVXI_HTRIG                 DefaultTTLtrigHandler;

/* system interrupt handler functions */

extern NIVXI_STATUS       NIVXI_FN AssertSysreset(INT16 controller,
                                                  UINT16 mode);
extern NIVXI_STATUS       NIVXI_FN DisableACfail(INT16 controller);
extern NIVXI_STATUS       NIVXI_FN DisableSoftReset(void);
extern NIVXI_STATUS       NIVXI_FN DisableSysfail(INT16 controller);
extern NIVXI_STATUS       NIVXI_FN DisableSysreset(INT16 controller);
extern NIVXI_STATUS       NIVXI_FN EnableACfail(INT16 controller);
extern NIVXI_STATUS       NIVXI_FN EnableSoftReset(void);
extern NIVXI_STATUS       NIVXI_FN EnableSysfail(INT16 controller);
extern NIVXI_STATUS       NIVXI_FN EnableSysreset(INT16 controller);
extern NIVXI_HACFAIL    * NIVXI_FN GetACfailHandler(void);
extern NIVXI_HACFAIL    * NIVXI_FN GetACfailHandlerController(INT16 controller);
extern NIVXI_HBUSERROR  * NIVXI_FN GetBusErrorHandler(void);
extern NIVXI_HSOFTRESET * NIVXI_FN GetSoftResetHandler(void);
extern NIVXI_HSYSFAIL   * NIVXI_FN GetSysfailHandler(void);
extern NIVXI_HSYSFAIL   * NIVXI_FN GetSysfailHandlerController(INT16 controller);
extern NIVXI_HSYSRESET  * NIVXI_FN GetSysresetHandler(void);
extern NIVXI_HSYSRESET  * NIVXI_FN GetSysresetHandlerController(INT16 controller);
extern NIVXI_STATUS       NIVXI_FN SetACfailHandler(NIVXI_HACFAIL *func);
extern NIVXI_STATUS       NIVXI_FN SetACfailHandlerController(INT16 controller,
                                  NIVXI_HACFAIL *func);
extern NIVXI_STATUS       NIVXI_FN SetBusErrorHandler(NIVXI_HBUSERROR *func);
extern NIVXI_STATUS       NIVXI_FN SetSoftResetHandler(NIVXI_HSOFTRESET *func);

extern NIVXI_STATUS       NIVXI_FN SetSysfailHandler(NIVXI_HSYSFAIL *func);
extern NIVXI_STATUS       NIVXI_FN SetSysfailHandlerController(INT16 controller,
                                   NIVXI_HSYSFAIL *func);
extern NIVXI_STATUS       NIVXI_FN SetSysresetHandler(NIVXI_HSYSRESET *func);
extern NIVXI_STATUS       NIVXI_FN SetSysresetHandlerController(INT16 controller,
                                NIVXI_HSYSRESET *func);

extern NIVXI_HACFAIL               DefaultACfailHandler;
extern NIVXI_HBUSERROR             DefaultBusErrorHandler;
extern NIVXI_HSOFTRESET            DefaultSoftResetHandler;
extern NIVXI_HSYSFAIL              DefaultSysfailHandler;
extern NIVXI_HSYSRESET             DefaultSysresetHandler;

/* VXIbus extender functions */

extern NIVXI_STATUS       NIVXI_FN MapECLtrig(INT16 extender,
                                              UINT16 lines,
                                              UINT16 directions);
extern NIVXI_STATUS       NIVXI_FN MapTTLtrig(INT16 extender,
                                              UINT16 lines,
                                              UINT16 directions);
extern NIVXI_STATUS       NIVXI_FN MapUtilBus(INT16 extender,
                                              UINT16 modes);
extern NIVXI_STATUS       NIVXI_FN MapVXIint(INT16 extender,
                                             UINT16 levels,
                                             UINT16 directions);

/* other functions available which are not NI-VXI documented */

extern NIVXI_STATUS       NIVXI_FN DisableBlockMode(void);
extern NIVXI_STATUS       NIVXI_FN EnableBlockMode(void);
extern NIVXI_HCLOCK     * NIVXI_FN GetClockHandler(void);
extern NIVXI_STATUS       NIVXI_FN LPeek(UINT32 address,
                                         UINT16 width,
                                         void NIVXI_DT *value);
extern NIVXI_STATUS       NIVXI_FN LPoke(UINT32 address,
                                         UINT16 width,
                                         UINT32 value);
extern NIVXI_STATUS       NIVXI_FN RMCloseVXIlibrary(void);
extern NIVXI_STATUS       NIVXI_FN RMInitVXIlibrary(void);
extern NIVXI_STATUS       NIVXI_FN SetClockHandler(NIVXI_HCLOCK *func);

extern NIVXI_HCLOCK                DefaultClockHandler;

/* === End With-Prototype Function Declarations Section =============== */

#else

/* === Begin No-Prototype Function Declarations Section =============== */

/* system configuration functions */

extern NIVXI_STATUS       NIVXI_FN CloseVXIlibrary();
extern NIVXI_STATUS       NIVXI_FN CreateDevInfo();
extern NIVXI_STATUS       NIVXI_FN FindDevLA();
extern NIVXI_STATUS       NIVXI_FN GetDevInfo();
extern NIVXI_STATUS       NIVXI_FN GetDevInfoLong();
extern NIVXI_STATUS       NIVXI_FN GetDevInfoShort();
extern NIVXI_STATUS       NIVXI_FN GetDevInfoStr();
extern NIVXI_STATUS       NIVXI_FN InitVXIlibrary();
extern NIVXI_STATUS       NIVXI_FN SetDevInfo();
extern NIVXI_STATUS       NIVXI_FN SetDevInfoLong();
extern NIVXI_STATUS       NIVXI_FN SetDevInfoShort();
extern NIVXI_STATUS       NIVXI_FN SetDevInfoStr();

/* word serial protocol functions -- commander and servant */

extern NIVXI_STATUS       NIVXI_FN GenProtError();
extern NIVXI_HWSSCMD    * NIVXI_FN GetWSScmdHandler();
extern NIVXI_HWSSECMD   * NIVXI_FN GetWSSEcmdHandler();
extern NIVXI_HWSSLCMD   * NIVXI_FN GetWSSLcmdHandler();
extern NIVXI_HWSSRD     * NIVXI_FN GetWSSrdHandler();
extern NIVXI_HWSSWRT    * NIVXI_FN GetWSSwrtHandler();
extern NIVXI_STATUS       NIVXI_FN RespProtError();
extern NIVXI_STATUS       NIVXI_FN SetWSScmdHandler();
extern NIVXI_STATUS       NIVXI_FN SetWSSEcmdHandler();
extern NIVXI_STATUS       NIVXI_FN SetWSSLcmdHandler();
extern NIVXI_STATUS       NIVXI_FN SetWSSrdHandler();
extern NIVXI_STATUS       NIVXI_FN SetWSSwrtHandler();
extern NIVXI_STATUS       NIVXI_FN WSabort();
extern NIVXI_STATUS       NIVXI_FN WSclr();
extern NIVXI_STATUS       NIVXI_FN WScmd();
extern NIVXI_STATUS       NIVXI_FN WSEcmd();
extern NIVXI_STATUS       NIVXI_FN WSgetTmo();
extern NIVXI_STATUS       NIVXI_FN WSLcmd();
extern NIVXI_STATUS       NIVXI_FN WSLresp();
extern NIVXI_STATUS       NIVXI_FN WSrd();
extern NIVXI_STATUS       NIVXI_FN WSrdf();
extern NIVXI_STATUS       NIVXI_FN WSrdi();
extern NIVXI_STATUS       NIVXI_FN WSrdl();
extern NIVXI_STATUS       NIVXI_FN WSresp();
extern NIVXI_STATUS       NIVXI_FN WSsetTmo();
extern NIVXI_STATUS       NIVXI_FN WStrg();
extern NIVXI_STATUS       NIVXI_FN WSwrt();
extern NIVXI_STATUS       NIVXI_FN WSwrtf();
extern NIVXI_STATUS       NIVXI_FN WSwrti();
extern NIVXI_STATUS       NIVXI_FN WSwrtl();
extern NIVXI_STATUS       NIVXI_FN WSSabort();
extern NIVXI_STATUS       NIVXI_FN WSSdisable();
extern NIVXI_STATUS       NIVXI_FN WSSenable();
extern NIVXI_STATUS       NIVXI_FN WSSLnoResp();
extern NIVXI_STATUS       NIVXI_FN WSSLsendResp();
extern NIVXI_STATUS       NIVXI_FN WSSnoResp();
extern NIVXI_STATUS       NIVXI_FN WSSrd();
extern NIVXI_STATUS       NIVXI_FN WSSrdi();
extern NIVXI_STATUS       NIVXI_FN WSSrdl();
extern NIVXI_STATUS       NIVXI_FN WSSsendResp();
extern NIVXI_STATUS       NIVXI_FN WSSwrt();
extern NIVXI_STATUS       NIVXI_FN WSSwrti();
extern NIVXI_STATUS       NIVXI_FN WSSwrtl();

extern NIVXI_HWSSCMD               DefaultWSScmdHandler;
extern NIVXI_HWSSECMD              DefaultWSSEcmdHandler;
extern NIVXI_HWSSLCMD              DefaultWSSLcmdHandler;
extern NIVXI_HWSSRD                DefaultWSSrdHandler;
extern NIVXI_HWSSWRT               DefaultWSSwrtHandler;

/* VXIbus access functions -- high/low level and local resource access */

extern NIVXI_STATUS       NIVXI_FN ClearBusError();
extern NIVXI_STATUS       NIVXI_FN GetByteOrder();
extern NIVXI_STATUS       NIVXI_FN GetContext();
extern INT16              NIVXI_FN GetMyLA();
extern NIVXI_STATUS       NIVXI_FN GetPrivilege();
extern NIVXI_STATUS       NIVXI_FN GetVXIbusStatus();
extern NIVXI_STATUS       NIVXI_FN GetVXIbusStatusInd();
extern NIVXI_STATUS       NIVXI_FN GetWindowRange();
extern void NIVXI_DT    * NIVXI_FN MapVXIAddress();
extern NIVXI_STATUS       NIVXI_FN MapVXIAddressSize();
extern NIVXI_STATUS       NIVXI_FN ReadMODID();
extern NIVXI_STATUS       NIVXI_FN RestoreContext();
extern NIVXI_STATUS       NIVXI_FN SaveContext();
extern NIVXI_STATUS       NIVXI_FN SetByteOrder();
extern NIVXI_STATUS       NIVXI_FN SetContext();
extern NIVXI_STATUS       NIVXI_FN SetMODID();
extern NIVXI_STATUS       NIVXI_FN SetPrivilege();
extern NIVXI_STATUS       NIVXI_FN UnMapVXIAddress();
extern NIVXI_STATUS       NIVXI_FN VXIin();
extern NIVXI_STATUS       NIVXI_FN VXIinLR();
extern NIVXI_STATUS       NIVXI_FN VXIinReg();
extern NIVXI_STATUS       NIVXI_FN VXImemAlloc();
extern NIVXI_STATUS       NIVXI_FN VXImemCopy();
extern NIVXI_STATUS       NIVXI_FN VXImemFree();
extern NIVXI_STATUS       NIVXI_FN VXImove();
extern NIVXI_STATUS       NIVXI_FN VXIout();
extern NIVXI_STATUS       NIVXI_FN VXIoutLR();
extern NIVXI_STATUS       NIVXI_FN VXIoutReg();
extern void               NIVXI_FN VXIpeek();
extern void               NIVXI_FN VXIpoke();

/* signal functions */

extern NIVXI_STATUS       NIVXI_FN DisableSignalInt();
extern NIVXI_STATUS       NIVXI_FN EnableSignalInt();
extern NIVXI_HSIGNAL    * NIVXI_FN GetSignalHandler();
extern NIVXI_STATUS       NIVXI_FN RouteSignal();
extern NIVXI_STATUS       NIVXI_FN SetSignalHandler();
extern NIVXI_STATUS       NIVXI_FN SignalDeq();
extern NIVXI_STATUS       NIVXI_FN SignalEnq();
extern NIVXI_STATUS       NIVXI_FN SignalJam();
extern NIVXI_STATUS       NIVXI_FN WaitForSignal();

extern NIVXI_HSIGNAL               DefaultSignalHandler;

/* interrupt functions */

extern NIVXI_STATUS       NIVXI_FN AcknowledgeVXIint();
extern NIVXI_STATUS       NIVXI_FN AssertVXIint();
extern NIVXI_STATUS       NIVXI_FN DeAssertVXIint();
extern NIVXI_STATUS       NIVXI_FN DisableVXIint();
extern NIVXI_STATUS       NIVXI_FN DisableVXItoSignalInt();
extern NIVXI_STATUS       NIVXI_FN EnableVXIint();
extern NIVXI_STATUS       NIVXI_FN EnableVXItoSignalInt();
extern NIVXI_HVXIINT    * NIVXI_FN GetVXIintHandler();
extern NIVXI_STATUS       NIVXI_FN RouteVXIint();
extern NIVXI_STATUS       NIVXI_FN SetVXIintHandler();
extern NIVXI_STATUS       NIVXI_FN VXIintAcknowledgeMode();

extern NIVXI_HVXIINT               DefaultVXIintHandler;

/* trigger functions -- all functions with ECL or TTL in the name are
 * outdated,  and are provided for backward compatability only -- new
 * functions with similar names  (without ECL or TTL)  should be used
 * instead, since the old style MAY NOT be supported in the future!!! */

extern NIVXI_STATUS       NIVXI_FN AcknowledgeECLtrig();
extern NIVXI_STATUS       NIVXI_FN AcknowledgeTrig();
extern NIVXI_STATUS       NIVXI_FN AcknowledgeTTLtrig();
extern NIVXI_STATUS       NIVXI_FN DisableECLsense();
extern NIVXI_STATUS       NIVXI_FN DisableTrigSense();
extern NIVXI_STATUS       NIVXI_FN DisableTTLsense();
extern NIVXI_STATUS       NIVXI_FN EnableECLsense();
extern NIVXI_STATUS       NIVXI_FN EnableTrigSense();
extern NIVXI_STATUS       NIVXI_FN EnableTTLsense();
extern NIVXI_HTRIG      * NIVXI_FN GetECLtrigHandler();
extern NIVXI_HTRIG      * NIVXI_FN GetTrigHandler();
extern NIVXI_HTRIG      * NIVXI_FN GetTTLtrigHandler();
extern NIVXI_STATUS       NIVXI_FN MapTrigToTrig();
extern NIVXI_STATUS       NIVXI_FN SetECLtrigHandler();
extern NIVXI_STATUS       NIVXI_FN SetTrigHandler();
extern NIVXI_STATUS       NIVXI_FN SetTTLtrigHandler();
extern NIVXI_STATUS       NIVXI_FN SrcECLtrig();
extern NIVXI_STATUS       NIVXI_FN SrcTrig();
extern NIVXI_STATUS       NIVXI_FN SrcTTLtrig();
extern NIVXI_STATUS       NIVXI_FN TrigAssertConfig();
extern NIVXI_STATUS       NIVXI_FN TrigCntrConfig();
extern NIVXI_STATUS       NIVXI_FN TrigExtConfig();
extern NIVXI_STATUS       NIVXI_FN TrigTickConfig();
extern NIVXI_STATUS       NIVXI_FN UnMapTrigToTrig();
extern NIVXI_STATUS       NIVXI_FN WaitForECLtrig();
extern NIVXI_STATUS       NIVXI_FN WaitForTrig();
extern NIVXI_STATUS       NIVXI_FN WaitForTTLtrig();

extern NIVXI_HTRIG                 DefaultECLtrigHandler;
extern NIVXI_HTRIG                 DefaultTrigHandler;
extern NIVXI_HTRIG                 DefaultTrigHandler2;
extern NIVXI_HTRIG                 DefaultTTLtrigHandler;

/* system interrupt handler functions */

extern NIVXI_STATUS       NIVXI_FN AssertSysreset();
extern NIVXI_STATUS       NIVXI_FN DisableACfail();
extern NIVXI_STATUS       NIVXI_FN DisableSoftReset();
extern NIVXI_STATUS       NIVXI_FN DisableSysfail();
extern NIVXI_STATUS       NIVXI_FN DisableSysreset();
extern NIVXI_STATUS       NIVXI_FN EnableACfail();
extern NIVXI_STATUS       NIVXI_FN EnableSoftReset();
extern NIVXI_STATUS       NIVXI_FN EnableSysfail();
extern NIVXI_STATUS       NIVXI_FN EnableSysreset();
extern NIVXI_HACFAIL    * NIVXI_FN GetACfailHandler();
extern NIVXI_HBUSERROR  * NIVXI_FN GetBusErrorHandler();
extern NIVXI_HSOFTRESET * NIVXI_FN GetSoftResetHandler();
extern NIVXI_HSYSFAIL   * NIVXI_FN GetSysfailHandler();
extern NIVXI_HSYSRESET  * NIVXI_FN GetSysresetHandler();
extern NIVXI_STATUS       NIVXI_FN SetACfailHandler();
extern NIVXI_STATUS       NIVXI_FN SetBusErrorHandler();
extern NIVXI_STATUS       NIVXI_FN SetSoftResetHandler();
extern NIVXI_STATUS       NIVXI_FN SetSysfailHandler();
extern NIVXI_STATUS       NIVXI_FN SetSysresetHandler();

extern NIVXI_HACFAIL               DefaultACfailHandler;
extern NIVXI_HBUSERROR             DefaultBusErrorHandler;
extern NIVXI_HSOFTRESET            DefaultSoftResetHandler;
extern NIVXI_HSYSFAIL              DefaultSysfailHandler;
extern NIVXI_HSYSRESET             DefaultSysresetHandler;

/* VXIbus extender functions */

extern NIVXI_STATUS       NIVXI_FN MapECLtrig();
extern NIVXI_STATUS       NIVXI_FN MapTTLtrig();
extern NIVXI_STATUS       NIVXI_FN MapUtilBus();
extern NIVXI_STATUS       NIVXI_FN MapVXIint();

/* other functions available which are not NI-VXI documented */

extern NIVXI_STATUS       NIVXI_FN DisableBlockMode();
extern NIVXI_STATUS       NIVXI_FN EnableBlockMode();
extern NIVXI_HCLOCK     * NIVXI_FN GetClockHandler();
extern NIVXI_STATUS       NIVXI_FN LPeek();
extern NIVXI_STATUS       NIVXI_FN LPoke();
extern NIVXI_STATUS       NIVXI_FN RMCloseVXIlibrary();
extern NIVXI_STATUS       NIVXI_FN RMInitVXIlibrary();
extern NIVXI_STATUS       NIVXI_FN SetClockHandler();

extern NIVXI_HCLOCK                DefaultClockHandler;

/* === End No-Prototype Function Declarations Section =============== */

#endif

/* === Macro definitions for VXIpeek/VXIpoke, override with () ======== */

#if defined(VXIDOS) || defined(VXIWIN)

#if (defined(BINARY_COMPATIBLE)) || (NIVXI_DEVLOCATION == NIVXI_EXTERNAL)
#define MXI1AND2_COMPATIBLE
#endif

#if defined(MXI1_COMPATIBLE)

extern UINT8 GetBoardStatus (void);

#if defined(M_I386) || defined(_M_I386) || defined(__I386__)

#define VXIpeek(p,w,v)                                                     \
   {                                                                       \
      register UINT8 board_status;                                         \
                                                                           \
      *(v) = *(p);                                                         \
                                                                           \
      if (board_status = GetBoardStatus ())                                \
      {                                                                    \
         while (board_status & 0x05)                                       \
         {                                                                 \
            *(v) = *(p);                                                   \
            board_status = GetBoardStatus ();                              \
         }                                                                 \
                                                                           \
         if (board_status & 0x0A)                                          \
            (*GetBusErrorHandler())();                                     \
      }                                                                    \
   }

#define VXIpoke(p,w,v)                                                     \
   {                                                                       \
      register UINT8 board_status;                                         \
                                                                           \
      *(p) = (v);                                                          \
                                                                           \
      if (board_status = GetBoardStatus ())                                \
      {                                                                    \
         while (board_status & 0x05)                                       \
         {                                                                 \
            *(p) = (v);                                                    \
            board_status = GetBoardStatus ();                              \
         }                                                                 \
                                                                           \
         if (board_status & 0x0A)                                          \
            (*GetBusErrorHandler())();                                     \
      }                                                                    \
   }

#else

#define VXIpeek(p,w,v)                                                     \
   {                                                                       \
      if ((w)==4)                                                          \
         (VXIpeek)(p,w,v);                                                 \
      else                                                                 \
      {                                                                    \
         register UINT8 board_status;                                      \
                                                                           \
         *(v) = *(p);                                                      \
                                                                           \
         if (board_status = GetBoardStatus ())                             \
         {                                                                 \
            while (board_status & 0x05)                                    \
            {                                                              \
               *(v) = *(p);                                                \
               board_status = GetBoardStatus ();                           \
            }                                                              \
                                                                           \
            if (board_status & 0x0A)                                       \
               (*GetBusErrorHandler())();                                  \
         }                                                                 \
      }                                                                    \
   }

#define VXIpoke(p,w,v)                                                     \
   {                                                                       \
      if ((w)==4)                                                          \
         (VXIpoke)(p,w,v);                                                 \
      else                                                                 \
      {                                                                    \
         register UINT8 board_status;                                      \
                                                                           \
         *(p) = (v);                                                       \
                                                                           \
         if (board_status = GetBoardStatus ())                             \
         {                                                                 \
            while (board_status & 0x05)                                    \
            {                                                              \
               *(p) = (v);                                                 \
               board_status = GetBoardStatus ();                           \
            }                                                              \
                                                                           \
            if (board_status & 0x0A)                                       \
               (*GetBusErrorHandler())();                                  \
         }                                                                 \
      }                                                                    \
   }

#endif

#elif defined(MXI1AND2_COMPATIBLE)

extern UINT8 GetBoardStatus12 (void);

#if defined(M_I386) || defined(_M_I386) || defined(__I386__)

#define VXIpeek(p,w,v)                                                     \
   {                                                                       \
      register UINT8 board_status;                                         \
                                                                           \
      *(v) = *(p);                                                         \
                                                                           \
      if (board_status = GetBoardStatus12 ())                              \
      {                                                                    \
         while (board_status & 0x25)                                       \
         {                                                                 \
            *(v) = *(p);                                                   \
            board_status = GetBoardStatus12 ();                            \
         }                                                                 \
                                                                           \
         if (board_status & 0x1A)                                          \
            (*GetBusErrorHandler())();                                     \
      }                                                                    \
   }

#define VXIpoke(p,w,v)                                                     \
   {                                                                       \
      register UINT8 board_status;                                         \
                                                                           \
      *(p) = (v);                                                          \
                                                                           \
      if (board_status = GetBoardStatus12 ())                              \
      {                                                                    \
         while (board_status & 0x25)                                       \
         {                                                                 \
            *(p) = (v);                                                    \
            board_status = GetBoardStatus12 ();                            \
         }                                                                 \
                                                                           \
         if (board_status & 0x1A)                                          \
            (*GetBusErrorHandler())();                                     \
      }                                                                    \
   }

#else

#define VXIpeek(p,w,v)                                                     \
   {                                                                       \
      if ((w)==4)                                                          \
         (VXIpeek)(p,w,v);                                                 \
      else                                                                 \
      {                                                                    \
         register UINT8 board_status;                                      \
                                                                           \
         *(v) = *(p);                                                      \
                                                                           \
         if (board_status = GetBoardStatus12 ())                           \
         {                                                                 \
            while (board_status & 0x25)                                    \
            {                                                              \
               *(v) = *(p);                                                \
               board_status = GetBoardStatus12 ();                         \
            }                                                              \
                                                                           \
            if (board_status & 0x1A)                                       \
               (*GetBusErrorHandler())();                                  \
         }                                                                 \
      }                                                                    \
   }

#define VXIpoke(p,w,v)                                                     \
   {                                                                       \
      if ((w)==4)                                                          \
         (VXIpoke)(p,w,v);                                                 \
      else                                                                 \
      {                                                                    \
         register UINT8 board_status;                                      \
                                                                           \
         *(p) = (v);                                                       \
                                                                           \
         if (board_status = GetBoardStatus12 ())                           \
         {                                                                 \
            while (board_status & 0x25)                                    \
            {                                                              \
               *(p) = (v);                                                 \
               board_status = GetBoardStatus12 ();                         \
            }                                                              \
                                                                           \
            if (board_status & 0x1A)                                       \
               (*GetBusErrorHandler())();                                  \
         }                                                                 \
      }                                                                    \
   }

#endif

#else /* code specifically for NIVXI_EMBEDDED without binary compatibility */

#if defined(M_I386) || defined(_M_I386) || defined(__I386__)

#define VXIpeek(p,w,v)  ( *(v) = *(p) )

#define VXIpoke(p,w,v)  ( *(p) =  (v) )

#else

#define VXIpeek(p,w,v)  if (w==4) (VXIpeek)(p,w,v);                        \
                        else      *(v) = *(p);

#define VXIpoke(p,w,v)  if (w==4) (VXIpoke)(p,w,v);                        \
                        else      *(p) = (v);

#endif

#endif

#endif

/* === End of Declarations =============================================== */

#if defined(__cplusplus) || defined(__cplusplus__)
   }
#endif

#endif

