/**************************************************************************/
/*                                                                        */
/*             National Instruments NI-VXI Software Interface             */
/*                   (C) 1996-1999,  National Instruments                 */
/*                                                                        */
/*                   Device Information Include File                      */
/*                                                                        */
/**************************************************************************/
/*                       DO NOT MODIFY THIS FILE                          */
/**************************************************************************/
/* The following file contains all of the necessary include file
 * information for use of the National Instruments NI-VXI Device Access
 * functions.
 */

#if !defined(devinfo_h)
#define devinfo_h

#include "datasize.h"

/*************************************************************************
 * The following is the structure definition for the structure
 * returned from GetDevInfo.  Following the structure is definitions
 * of fields which are bit encoded.
 */

typedef struct UserLAEntry {
    INT16   la;            /* Logical Address of this entry        */
    INT8    DevName[14];   /* Device Name (assigned by RM)         */
    INT16   Cmdr;          /* Logical Address of commander         */
    INT16   Mainframe;     /* MainFrame number (extended or CPU)   */
    INT16   Slot;          /* physical slot in which it resides    */
    UINT16  ManId;         /* manufacturer identification number   */
    INT8    ManName[14];   /* manufacturer name (from mfnameid.tbl)*/
    UINT16  ModelCode;     /* manufacturer model number            */
    INT8    ModelName[14]; /* model name (from model.tbl)          */
    UINT16  DevClass;      /* Device Class 0=MEM,1=EXT,2=MBD,3=RBD */
    UINT16  SubClass;      /* Subclass for Extended Class devices  */
    UINT16  MemSpace;      /* Address Space: 0=A24, 1=A32, 3=A16   */
    UINT32  MemBase;       /* address of A24 or A32 address space  */
    UINT32  MemSize;       /* address space size in actual bytes   */
    UINT16  MemType;       /* memory type (attributes)             */
    UINT16  IrqLevel;      /* VXI interrupter levels assigned      */
    UINT16  IrqHandle;     /* VXI interrupt handler levels assigned*/
    UINT16  ExtInfo;       /* Controller/extender information      */
    UINT16  AsyncMode;     /* Asynchronous Mode Control Response   */
    UINT16  RespEnable;    /* Response Interrupts Enabled          */
    UINT16  Prot;          /* protcols supported (from Read Prot)  */
    UINT16  Flags;         /* various flags                        */
    UINT16  StatusState;   /* 0=failed,1=extended,2=diag,3=passed  */
} UserLAEntry;

/* UserLAEntry.DevClass definitions */
#define DevClass_MEM        0    /* Memory Class device                    */
#define DevClass_EXT        1    /* Extended Class device (see .SubClass   */
#define DevClass_MBD        2    /* Message-Based device                   */
#define DevClass_RBD        3    /* Register Based device                  */

/* UserLAEntry.ExtInfo definitions */
#define EXT_REMOTE      (1U<<13) /* 1=Remote Controller */
#define EXT_CHILD       (1U<<12) /* 1=Child side extender, 0=Parent side   */
#define EXT_FRAME_EXT   (1U<<11) /* 1=device is a mainframe extender       */
#define EXT_EXTENDED    (1U<<10) /* 1=device is an Extended controller     */
#define EXT_EMBEDDED    (1U<<9)  /* 1=device is an Embedded controller     */
#define EXT_EXTERNAL    (1U<<8)  /* 1=device is an External controller     */
#define EXT_TOWARD_ROOT 0x00FF   /* mask: LA of extender toward root frame */

/* UserLAEntry.flags definitions */
#define LA_ALIVE        (1U<<0)  /* 1=was alive at one time, 0=never alive */
#define LA_DEAD         (1U<<1)  /* 1=was forced offline,0=currently alive */
#define LA_READ_STB     (1U<<2)  /* 1=Device can respond to WS Read STB    */
#define LA_MultiplePH   (1U<<3)  /* 1=device was mult. programable handler */
#define LA_DC_Device    (1U<<4)  /* 1=device was Dynamically config device */

/* UserLAEntry.StatusState definitions (copy of PASSED,READY bits)*/
#define LA_PASSED           3    /* VXI Specification 1.2 definitions      */
#define LA_EXTEND           1
#define LA_FAILED           0
#define LA_DIAG             2
#define LA_PASSED_READY     3    /* VXI Specification 1.3 definitions      */
#define LA_PASSED_NREADY    1
#define LA_NPASSED_NREADY   0
#define LA_CONFIG_REG_INIT  2

/*************************************************************************
 * The following are #define's for use with the GetDevInfo,
 * GetDevInfoStr, GetDevInfoShort, and GetDevInfoLong functions.  The
 * defines specify the value of the 'field' parameter.  These #define's
 * may also be used with the SetDevInfo functions.  Great care should be
 * used when using the SetDevInfo functions.  Changing values of these
 * fields has a direct effect on the execution of the NI-VXI software.
 */

#define DI_DIENTRY          0    /* get/set entire UserLAEntry structure    */
#define DI_DEVNAME          1    /* get/set UserLAEntry.DevName     INT8[14]*/
#define DI_CMDR             2    /* get/set UserLAEntry.Cmdr        INT16   */
#define DI_FRAME            3    /* get/set UserLAEntry.Mainframe   INT16   */
#define DI_SLOT             4    /* get/set UserLAEntry.Slot        INT16   */
#define DI_MANID            5    /* get/set UserLAEntry.ManId       UINT16  */
#define DI_MANNAME          6    /* get/set UserLAEntry.ManName     INT8[14]*/
#define DI_MODELCODE        7    /* get/set UserLAEntry.ModelCode   UINT16  */
#define DI_MODELNAME        8    /* get/set UserLAEntry.ModelName   INT8[14]*/
#define DI_DEVCLASS         9    /* get/set UserLAEntry.DevClass    UINT16  */
#define DI_SUBCLASS         10   /* get/set UserLAEntry.SubClass    UINT16  */
#define DI_MEMSPACE         11   /* get/set UserLAEntry.MemSpace    UINT16  */
#define DI_MEMBASE          12   /* get/set UserLAEntry.MemBase     UINT32  */
#define DI_MEMSIZE          13   /* get/set UserLAEntry.MemSize     UINT32  */
#define DI_MEMTYPE          14   /* get/set UserLAEntry.MemType     UINT16  */
#define DI_IRQLEVEL         15   /* get/set UserLAEntry.IrqLevel    UINT16  */
#define DI_IRQHANDLE        16   /* get/set UserLAEntry.IrqHandle   UINT16  */
#define DI_EXTINFO          17   /* get/set UserLAEntry.ExtInfo     UINT16  */
#define DI_ASYNCMODE        18   /* get/set UserLAEntry.AsyncMode   UINT16  */
#define DI_RESPENA          19   /* get/set UserLAEntry.RespEnable  UINT16  */
#define DI_PROT             20   /* get/set UserLAEntry.Prot        UINT16  */
#define DI_FLAGS            21   /* get/set UserLAEntry.Flags       UINT16  */
#define DI_STATUSSTATE      22   /* get/set UserLAEntry.StatusState UINT16  */

#define DI_MYNORMALOPERATION     (0x8000)
#define DI_MYRESMANTMO           (0x8001)
#define DI_MYSTATUSBYTE          (0x8002)
#define DI_MYSERVAREA            (0x8003)
#define DI_MYPROTREG             (0x8004)
#define DI_MYREADPROTRESP        (0x8005)
#define DI_MYNUMHANDLERS         (0x8010)
#define DI_MYHANDLERTOVME1       (0x8011)
#define DI_MYHANDLERTOVME2       (0x8012)
#define DI_MYHANDLERTOVME3       (0x8013)
#define DI_MYHANDLERTOVME4       (0x8014)
#define DI_MYHANDLERTOVME5       (0x8015)
#define DI_MYHANDLERTOVME6       (0x8016)
#define DI_MYHANDLERTOVME7       (0x8017)
#define DI_MYNUMINTERRUPTERS     (0x8018)
#define DI_MYINTERRUPTERTOVME1   (0x8019)
#define DI_MYINTERRUPTERTOVME2   (0x801a)
#define DI_MYINTERRUPTERTOVME3   (0x801b)
#define DI_MYINTERRUPTERTOVME4   (0x801c)
#define DI_MYINTERRUPTERTOVME5   (0x801d)
#define DI_MYINTERRUPTERTOVME6   (0x801e)
#define DI_MYINTERRUPTERTOVME7   (0x801f)
#define DI_MYEVENTS00_1F         (0x8020)
#define DI_MYEVENTS20_3F         (0x8021)
#define DI_MYEVENTS40_5F         (0x8022)
#define DI_MYEVENTS60_7F         (0x8023)
#define DI_WSSTRGRECV            (0x8030)
#define DI_WSSWRTDONE            (0x8031)
#define DI_WSSWRTDONESTATUS      (0x8032)
#define DI_WSSWRTDONECOUNT       (0x8033)
#define DI_WSSRDDONE             (0x8034)
#define DI_WSSRDDONESTATUS       (0x8035)
#define DI_WSSRDDONECOUNT        (0x8036)
#define DI_VXIINTCONTROLLER      (0x8040)
#define DI_VXIINTLEVEL           (0x8041)
#define DI_VXIINTSTATUSID        (0x8042)
#define DI_CLOCKRECV             (0x8050)
#define DI_SOFTRESETRECV         (0x8051)
#define DI_SYSFAILRECV           (0x8052)
#define DI_ACFAILRECV            (0x8053)
#define DI_BUSERRORRECV          (0x8054)
#define DI_NIVXI_SWREV           (0x8100)
#define DI_NIVXI_HWREV           (0x8101)
#define DI_NIVXI_DEVNAME         (0x8102)
#define DI_NIVXI_RETRY           (0x8103)
#define DI_NIVXI_MITESWRETRY     (0x8104)
#define DI_NIVXI_DEVLOCATION     (0x8105)
#define DI_NIVXI_VXIMOVE_SUPPORT (0x8106)

/*************************************************************************
 * The following are return codes for all of the System Configuration
 * functions.
 */

/* InitVXIlibrary */
#define INIT_RET_OK              NIVXI_OK    /* zero */
#define INIT_RET_OK_REPEATED     ( 1)
#define INIT_RET_ERROR           (-1)

/* CloseVXIlibrary */
#define CLOSE_RET_OK             NIVXI_OK    /* zero */
#define CLOSE_RET_OK_STILL_OPEN  ( 1)
#define CLOSE_RET_ERROR          (-1)

/* FindDevLA, GetDevInfo, GetDevInfoStr, GetDevInfoShort, GetDevInfoLong,
 * SetDevInfo, SetDevinfoStr, SetDevInfoShort, and SetDevInfoLong
 */
#define DI_RET_OK                NIVXI_OK    /* zero */
#define DI_RET_DEVNF             (-1)
#define DI_RET_INVFLD            (-2)

/* CreateDevInfo */
#define DI_CREATE_OK             NIVXI_OK    /* zero */
#define DI_CREATE_DEVEXISTS      (-1)
#define DI_CREATE_LARANGE        (-2)
#define DI_CREATE_ALLOCFAIL      (-3)

#endif

