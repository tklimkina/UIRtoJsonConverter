/************************************************************************/
/*                                                                      */
/*             National Instruments NI-VXI Software Interface           */
/*                   (C) 1996-1999,  National Instruments               */
/*                                                                      */
/*      High Level, Low Level, and Local Resource Access Functions      */
/*                                                                      */
/************************************************************************/
/*                       DO NOT MODIFY THIS FILE                        */
/************************************************************************/
/* The following file contains all of the necessary include file
 * information for use of the National Instruments NI-VXI High Level
 * VXIbus Access functions, Low Level VXIbus Access functions,
 * and Local Resource Access functions.
 */

#if !defined(busacc_h)
#define busacc_h

#include "datasize.h"

/*************************************************************************
 *   Standard NI-VXI values for space fields in VXIin, VXIout, VXImove,
 * and MapVXIaddrress.
 */
#define LOC_SPACE             0U
#define A16_SPACE             1U
#define A24_SPACE             2U
#define A32_SPACE             3U

/*************************************************************************
 *   Standard NI-VXI values for data width parameters.
 */
#define BYTE_WIDTH            1U
#define WORD_WIDTH            2U
#define LONGWORD_WIDTH        4U

/*************************************************************************
 *   Standard NI-VXI values for privilege fields
 */
#define NonPriv_DATA          0U
#define Priv_DATA             1U
#define NonPriv_PROGRAM       2U
#define Priv_PROGRAM          3U
#define NonPriv_BLOCK         4U
#define Priv_BLOCK            5U
#define NonPrivD64_BLOCK      6U
#define PrivD64_BLOCK         7U

/*************************************************************************
 *   Standard NI-VXI values for byte order fields
 */
#define MOTOROLA_ORDER        0U
#define INTEL_ORDER           1U
/*************************************************************************
 *   Standard NI-VXI values for transfer method fields
 */
#define XFER_DMA              0U
#define XFER_PIO              1U
/*************************************************************************
 *   Standard NI-VXI values for address increment fields
 */
#define XFER_INCREMENT_ADDR    0U
#define XFER_NO_INCREMENT_ADDR 1U
/*************************************************************************
 *   Standard NI-VXI values for block transfer fields
 */
#define XFER_ENABLE_BLOCK     0U
#define XFER_DISABLE_BLOCK    1U
/*************************************************************************
 *   Standard NI-VXI values for sync block transfer fields
 */
#define XFER_ENABLE_SYNC      0U
#define XFER_DISABLE_SYNC     1U

/*************************************************************************
 *    The following function macros can be used to formulate the AccessP
 * parameters to the functions VXIin, VXIout, VXImove, and MapVXIaddress.
 *
 * NOTE:
 * DO NOT WORRY ABOUT THE EXTENSIVE MATH DONE WITHIN THESE MACROS.  IF
 * OTHER DEFINES ARE SUPPLIED AS PARAMETERS TO THESE MACROS, THE ENTIRE
 * MATH OPERATION WILL BE COMPUTED AT PRE-PROCESSOR (PRE COMPILE-TIME).
 * NO CODE WILL BE GENERATED--ONLY A SINGLE MOVE INSTRUCTION.
 *
 *    The AccessP_*() macros are used to set the particular field
 * (address space, privilege, or byte order) within the call to the
 * function.
 *
 *    The SetAccessP_*() macros are used to set the particular field
 * (address space, privilege, or byte order) in a variable to be used
 * as an AccessP parameter.  This is done in a similar manner to the
 * AccessP_* #defines except the 'c' parameter specifies the variable
 * to set the field in.
 *
 *    The GetAccessP_*() macros are used to extract the particular field
 * (address space, privilege, or byte order) from an AccessP parameter.
 *
 * Example:
 *
 *    UINT16 AccessP, value, ret;
 *    UINT32 offset;
 *
 *    offset = 0xC000;     -- logical address 0's ID register --
 *
 *    ret = VXIin(AccessP_Space(A16_SPACE)   |
 *                AccessP_Priv(NonPriv_DATA) |
 *                AccessP_BO(MOTOROLA_ORDER),
 *                offset, WORD_WIDTH, (UINT32 *)&value);
 * OR
 *    SetAccessP_Space(AccessP, A16_SPACE);
 *    SetAccessP_Priv(AccessP, NonPriv_DATA);
 *    SetAccessP_BO(AccessP, MOTOROLA_ORDER);
 *    ret = VXIin(AccessP, offset, WORD_WIDTH, (UINT32 *)&value);
 *
 */
#define AccessP_Space(p)          ( p&3     )
#define AccessP_Priv(p)           ((p&7)<<2 )
#define AccessP_Owner(p)          ((p&1)<<6 )
#define AccessP_BO(p)             ((p&1)<<7 )
#define AccessP_PIO(p)            ((p&1)<<11)
#define AccessP_NoInc(p)          ((p&1)<<12)
#define AccessP_BlkDisable(p)     ((p&1)<<13)
#define AccessP_SyncDisable(p)    ((p&1)<<14)

#define SetAccessP_Space(c,s)      (c = (c & 0xfffffffcL) | AccessP_Space(s)      )
#define SetAccessP_Priv(c,p)       (c = (c & 0xffffffe3L) | AccessP_Priv(p)       )
#define SetAccessP_Owner(c,o)      (c = (c & 0xffffffbfL) | AccessP_Owner(o)      )
#define SetAccessP_BO(c,b)         (c = (c & 0xffffff7fL) | AccessP_BO(b)         )
#define SetAccessP_PIO(c,p)        (c = (c & 0xfffff7ffL) | AccessP_PIO(p)        )
#define SetAccessP_NoInc(c,i)      (c = (c & 0xffffefffL) | AccessP_NoInc(i)      )
#define SetAccessP_BlkDisable(c,b) (c = (c & 0xffffdfffL) | AccessP_BlkDisable(b) )
#define SetAccessP_SyncDisable(c,s)(c = (c & 0xffffbfffL) | AccessP_SyncDisable(s))

#define GetAccessP_Space(c)       (UINT16)( c      & 0x0003)
#define GetAccessP_Priv(c)        (UINT16)((c>>2)  & 0x0007)
#define GetAccessP_Owner(c)       (UINT16)((c>>6)  & 0x0001)
#define GetAccessP_BO(c)          (UINT16)((c>>7)  & 0x0001)
#define GetAccessP_PIO(c)         (UINT16)((c>>11) & 0x0001)
#define GetAccessP_NoInc(c)       (UINT16)((c>>12) & 0x0001)
#define GetAccessP_BlkDisable(c)  (UINT16)((c>>13) & 0x0001)
#define GetAccessP_SyncDisable(c) (UINT16)((c>>14) & 0x0001)

/*************************************************************************
 *   The following structure definition is needed when calling the
 * functions SaveContext and RestoreContext.
 */
typedef struct ContextStruct {
    UINT32 entries[6];
} ContextStruct;

/*************************************************************************
 *   The following structure definition is needed when calling the
 * functions GetVXIbusStatus and GetVXIbusStatusInd.
 */
typedef struct BusStatus {
    INT16 BusError;
    INT16 Sysfail;
    INT16 ACfail;
    INT16 SignalIn;
    INT16 VXIints;
    INT16 ECLtrigs;
    INT16 TTLtrigs;
} BusStatus;

/*************************************************************************
 *   The following are return codes for all of the High Level, Low Level,
 * and Local Resource Access functions.
 */

/* General defines for all Bus Access functions */
#define INVALID_WINDOW              (-1)
#define INVALID_ACCESS_PARM         (-2)
#define INVALID_ACCESS_ADDRESS      (-3)
#define INVALID_ACCESS_WIDTH        (-4)
#define UNSUPPORTED_BYTEORDER       (-5)
#define UNSUPPORTED_ADDRESS         (-6)
#define UNSUPPORTED_PRIV            (-7)
#define UNSUPPORTED_WIDTH           (-9)
#define NOT_OWNER_ACCESS            (-9)

/* MapVXIaddress */
#define MAP_SUCCESSFUL              NIVXI_OK        /* zero */
#define MAP_INVPARMS                INVALID_ACCESS_PARM
#define MAP_INVADDR                 INVALID_ACCESS_ADDRESS
#define MAP_INVWIDTH                INVALID_ACCESS_WIDTH
#define MAP_UNSUP_BYTEORDER         UNSUPPORTED_BYTEORDER
#define MAP_UNSUP_ADDRESS           UNSUPPORTED_ADDRESS
#define MAP_UNSUP_PRIV              UNSUPPORTED_PRIV
#define MAP_TIMEOUT                 (-8)

/* UnmapVXIaddress */
#define UNMAP_OK_UNLOCKED           NIVXI_OK        /* zero */
#define UNMAP_ACCESS_ONLY           (1)
#define UNMAP_INVWIN                INVALID_WINDOW

/* GetWindowRange */
#define RANGE_OK                    NIVXI_OK        /* zero */
#define RANGE_INVWIN                INVALID_WINDOW

/* SaveContext, RestoreContext, GetContext, SetContext */
#define CONTEXT_OK                  NIVXI_OK        /* zero */
#define CONTEXT_INVWIN              INVALID_WINDOW
#define CONTEXT_INVALID             INVALID_ACCESS_PARM
#define CONTEXT_NOTOWNER            NOT_OWNER_ACCESS

/* SetPrivilege, GetPrivilege */
#define PRIV_OK                     NIVXI_OK       /* zero */
#define PRIV_INVWIN                 INVALID_WINDOW
#define PRIV_INVPRIV                INVALID_ACCESS_PARM
#define PRIV_UNSUP                  UNSUPPORTED_PRIV
#define PRIV_NOTOWNER               NOT_OWNER_ACCESS

/* SetByteOrdering, GetByteOrdering */
#define BYTEORDER_OK                NIVXI_OK       /* zero */
#define BYTEORDER_OK_SAMEFORALL     (1)
#define BYTEORDER_INVWIN            INVALID_WINDOW
#define BYTEORDER_INVBO             INVALID_ACCESS_PARM
#define BYTEORDER_UNSUP             UNSUPPORTED_PRIV
#define BYTEORDER_NOTOWNER          NOT_OWNER_ACCESS

/* GetVXIbusStatus, GetVXIbusStatusInd */
#define BUSSTAT_OK                  NIVXI_OK       /* zero */
#define BUSSTAT_NOTSUP              NO_HARDWARE_SUPPORT
#define BUSSTAT_INVCONT             INVALID_CONTROLLER
#define BUSSTAT_INVFIELD            (-3)

/* VXIin, VXIout, VXImove, VXIinLR, VXIoutLR, VXIinReg, VXIoutReg */
#define ACCESS_OK                   NIVXI_OK       /* zero */
#define ACCESS_BERR                 (-1)
#define ACCESS_INVPARM              INVALID_ACCESS_PARM
#define ACCESS_INVADDR              INVALID_ACCESS_ADDRESS
#define ACCESS_INVWIDTH             INVALID_ACCESS_WIDTH
#define ACCESS_UNSUP_BYTEORDER      UNSUPPORTED_BYTEORDER
#define ACCESS_UNSUP_ADDRESS        UNSUPPORTED_ADDRESS
#define ACCESS_UNSUP_PRIV           UNSUPPORTED_PRIV
#define ACCESS_UNSUP_WIDTH          UNSUPPORTED_WIDTH
#define ACCESS_DMA_ERROR            (-8)

/* SetMODID, ReadMODID */
#define MODID_OK                    NIVXI_OK       /* zero */
#define MODID_NOTSLOT0              (-1)

/* VXImemAlloc, VXImemCopy, VXImemFree */
#define MEM_OK_USE_VXIMEMCOPY       ( 1)
#define MEM_OK                      NIVXI_OK       /* zero */
#define MEM_ALLOC_FAIL              (-1)
#define MEM_A16ONLY                 (-2)

#define MEM_COPY_FAIL               (-1)
#define MEM_COPY_INVDIR             (-5)

#define MEM_DEALLOC_FAIL            (-1)

#endif

