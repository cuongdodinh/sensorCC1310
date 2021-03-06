/// \addtogroup module_scif_driver_setup
//@{
#include "scif.h"
#include "scif_framework.h"
#ifdef DEVICE_FAMILY
    #define DEVICE_FAMILY_PATH(x) <ti/devices/DEVICE_FAMILY/x>
#else
    #define DEVICE_FAMILY_PATH(x) <x>
#endif
#include DEVICE_FAMILY_PATH(inc/hw_types.h)
#include DEVICE_FAMILY_PATH(inc/hw_memmap.h)
#include DEVICE_FAMILY_PATH(inc/hw_aon_event.h)
#include DEVICE_FAMILY_PATH(inc/hw_aon_rtc.h)
#include DEVICE_FAMILY_PATH(inc/hw_aon_wuc.h)
#include DEVICE_FAMILY_PATH(inc/hw_aux_sce.h)
#include DEVICE_FAMILY_PATH(inc/hw_aux_smph.h)
#include DEVICE_FAMILY_PATH(inc/hw_aux_evctl.h)
#include DEVICE_FAMILY_PATH(inc/hw_aux_aiodio.h)
#include DEVICE_FAMILY_PATH(inc/hw_aux_timer.h)
#include DEVICE_FAMILY_PATH(inc/hw_aux_wuc.h)
#include DEVICE_FAMILY_PATH(inc/hw_event.h)
#include DEVICE_FAMILY_PATH(inc/hw_ints.h)
#include DEVICE_FAMILY_PATH(inc/hw_ioc.h)
#include <string.h>
#if defined(__IAR_SYSTEMS_ICC__)
    #include <intrinsics.h>
#endif


// OSAL function prototypes
uint32_t scifOsalEnterCriticalSection(void);
void scifOsalLeaveCriticalSection(uint32_t key);




/// Firmware image to be uploaded to the AUX RAM
static const uint16_t pAuxRamImage[] = {
    /*0x0000*/ 0x1408, 0x040C, 0x1408, 0x042C, 0x1408, 0x0447, 0x1408, 0x044D, 0x4436, 0x2437, 0xAEFE, 0xADB7, 0x6442, 0x7000, 0x7C6B, 0x6871, 
    /*0x0020*/ 0x0068, 0x1425, 0x6872, 0x0069, 0x1425, 0x6873, 0x006A, 0x1425, 0x786B, 0xF801, 0xFA01, 0xBEF2, 0x786F, 0x6871, 0xFD0E, 0x6873, 
    /*0x0040*/ 0xED92, 0xFD06, 0x7C6F, 0x642D, 0x0450, 0x786B, 0x8F1F, 0xED8F, 0xEC01, 0xBE01, 0xADB7, 0x8DB7, 0x6630, 0x6542, 0x0000, 0x186F, 
    /*0x0060*/ 0x9D88, 0x9C01, 0xB60D, 0x1067, 0xAF19, 0xAA00, 0xB609, 0xA8FF, 0xAF39, 0xBE06, 0x0C6B, 0x8869, 0x8F08, 0xFD47, 0x9DB7, 0x086B, 
    /*0x0080*/ 0x8801, 0x8A01, 0xBEEC, 0x262F, 0xAEFE, 0x4630, 0x0450, 0x5527, 0x6642, 0x0000, 0x0C6B, 0x1499, 0x0450, 0x6742, 0x03FF, 0x0C6D, 
    /*0x00A0*/ 0x786C, 0x686D, 0xED37, 0xB605, 0x0000, 0x0C6C, 0x7C70, 0x652D, 0x0C6D, 0x786D, 0x686E, 0xFD0E, 0xF801, 0xE92B, 0xFD0E, 0xBE01, 
    /*0x00C0*/ 0x6436, 0xBDB7, 0x241A, 0xA6FE, 0xADB7, 0x641A, 0xADB7, 0x0000, 0x0078, 0x007B, 0x00FE, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 
    /*0x00E0*/ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0C67, 0xADB7, 0x0877, 0x8A00, 0xBE1A, 0x640F, 0x702C, 
    /*0x0100*/ 0x600E, 0x1501, 0x6000, 0x150D, 0x6C76, 0x7080, 0x6876, 0x1524, 0x6C76, 0x70F3, 0x6876, 0x1524, 0x6C76, 0x6876, 0x1542, 0x6C76, 
    /*0x0120*/ 0x0055, 0x8B0D, 0x8608, 0x7022, 0xFB0A, 0x7025, 0x154D, 0x6444, 0xADB7, 0x0877, 0x8A00, 0xBE39, 0x6000, 0x150D, 0x6C76, 0x7081, 
    /*0x0140*/ 0x6876, 0x1524, 0x6C76, 0x0876, 0x8A00, 0xBE13, 0x7000, 0x6876, 0x1553, 0x6C76, 0x8D47, 0x7000, 0x6876, 0x1553, 0x6C76, 0x9D47, 
    /*0x0160*/ 0x7001, 0x6876, 0x1553, 0x6C76, 0x86FF, 0x90FC, 0x8DA0, 0x8D09, 0x0C75, 0x6876, 0x1542, 0x6C76, 0x0001, 0x0C77, 0x6000, 0x150D, 
    /*0x0180*/ 0x6C76, 0x7080, 0x6876, 0x1524, 0x6C76, 0x70F5, 0x6876, 0x1524, 0x6C76, 0x6876, 0x1542, 0x6C76, 0x001D, 0x8B0D, 0x8608, 0x7022, 
    /*0x01A0*/ 0xFB0A, 0x7025, 0x154D, 0x6444, 0x04FD, 0x6000, 0x150D, 0x6C76, 0x7081, 0x6876, 0x1524, 0x6C76, 0x0876, 0x8A00, 0xBE13, 0x7000, 
    /*0x01C0*/ 0x6876, 0x1553, 0x6C76, 0x8D47, 0x7000, 0x6876, 0x1553, 0x6C76, 0x9D47, 0x7001, 0x6876, 0x1553, 0x6C76, 0x86FF, 0x90FC, 0x8DA0, 
    /*0x01E0*/ 0x8D09, 0x0C74, 0x6876, 0x1542, 0x6C76, 0x0000, 0x0C77, 0x440F, 0x086C, 0x8201, 0x0C6C, 0x0001, 0x0C67, 0xADB7, 0x5527, 0x6642, 
    /*0x0200*/ 0xADB7, 0xFB0C, 0xEDA4, 0xEB09, 0x640B, 0xCDB1, 0xADB7, 0x53F4, 0x1512, 0x750F, 0x1515, 0x53F4, 0x1512, 0x540F, 0x53F8, 0x1512, 
    /*0x0220*/ 0x550F, 0xADB7, 0xD802, 0xDEFE, 0xADB7, 0x53F6, 0x3513, 0xAE0B, 0x3513, 0xAE09, 0x3513, 0xAE07, 0x3513, 0xAE05, 0x3513, 0xAE03, 
    /*0x0240*/ 0xD801, 0xBEF4, 0xE202, 0xADB7, 0xEA00, 0xBE1B, 0xB50E, 0xFDA1, 0x8601, 0xFC00, 0xB602, 0x740F, 0x8E02, 0x540F, 0xFD47, 0x53FA, 
    /*0x0260*/ 0x1512, 0x750F, 0x1515, 0x53FD, 0x1512, 0x550F, 0x740F, 0x53F4, 0x1512, 0x750F, 0x1515, 0x53FA, 0x1512, 0x3413, 0xA601, 0xE201, 
    /*0x0280*/ 0x550F, 0xADB7, 0x540F, 0x53F4, 0x1512, 0x750F, 0x1515, 0x53FD, 0x1512, 0x740F, 0x53F5, 0x1512, 0xADB7, 0x5527, 0x6642, 0x862B, 
    /*0x02A0*/ 0xF200, 0xFB27, 0xADB7, 0xEA00, 0xBE1C, 0xB50B, 0x740F, 0x53F5, 0x1512, 0x750F, 0x1515, 0x53FF, 0x1512, 0xFDA1, 0x3413, 0xA601, 
    /*0x02C0*/ 0xF201, 0x550F, 0x8601, 0xFC00, 0xB602, 0x740F, 0x8E02, 0x540F, 0xFD47, 0x53F9, 0x1512, 0x750F, 0x1515, 0x53FD, 0x1512, 0x550F, 
    /*0x02E0*/ 0xF0FF, 0xADB7
};


/// Look-up table that converts from AUX I/O index to MCU IOCFG offset
static const uint8_t pAuxIoIndexToMcuIocfgOffsetLut[] = {
    120, 116, 112, 108, 104, 100, 96, 92, 28, 24, 20, 16, 12, 8, 4, 0
};


/** \brief Look-up table of data structure information for each task
  *
  * There is one entry per data structure (\c cfg, \c input, \c output and \c state) per task:
  * - [31:20] Data structure size (number of 16-bit words)
  * - [19:12] Buffer count (when 2+, first data structure is preceded by buffering control variables)
  * - [11:0] Address of the first data structure
  */
static const uint32_t pScifTaskDataStructInfoLut[] = {
//  cfg         input       output      state       
    0x00000000, 0x00000000, 0x002010E8, 0x002010EC  // Temperature
};




// No task-specific initialization functions




// No task-specific uninitialization functions




/** \brief Initilializes task resource hardware dependencies
  *
  * This function is called by the internal driver initialization function, \ref scifInit().
  */
static void scifTaskResourceInit(void) {
    scifInitIo(8, AUXIOMODE_OUTPUT, -1, 0);
    scifInitIo(13, AUXIOMODE_OPEN_DRAIN_WITH_INPUT, -1, 1);
    HWREG((IOC_BASE + IOC_O_IOCFG0) + pAuxIoIndexToMcuIocfgOffsetLut[13]) |= IOC_IOCFG0_HYST_EN_M;
    scifInitIo(12, AUXIOMODE_OPEN_DRAIN_WITH_INPUT, -1, 1);
    HWREG((IOC_BASE + IOC_O_IOCFG0) + pAuxIoIndexToMcuIocfgOffsetLut[12]) |= IOC_IOCFG0_HYST_EN_M;
    HWREG(AON_WUC_BASE + AON_WUC_O_AUXCLK) = (AON_WUC_AUXCLK_SRC_SCLK_HF | AON_WUC_AUXCLK_SCLK_HF_DIV_DIV2) | AON_WUC_AUXCLK_PWR_DWN_SRC_SCLK_LF;
    HWREG(AON_RTC_BASE + AON_RTC_O_CTL) |= AON_RTC_CTL_RTC_4KHZ_EN;
} // scifTaskResourceInit




/** \brief Uninitilializes task resource hardware dependencies
  *
  * This function is called by the internal driver uninitialization function, \ref scifUninit().
  */
static void scifTaskResourceUninit(void) {
    scifUninitIo(8, 0);
    scifUninitIo(13, -1);
    scifUninitIo(12, -1);
    HWREG(AON_WUC_BASE + AON_WUC_O_AUXCLK) = (AON_WUC_AUXCLK_SRC_SCLK_HF | AON_WUC_AUXCLK_SCLK_HF_DIV_DIV2) | AON_WUC_AUXCLK_PWR_DWN_SRC_NONE;
    HWREG(AON_RTC_BASE + AON_RTC_O_CTL) &= ~AON_RTC_CTL_RTC_4KHZ_EN;
} // scifTaskResourceUninit




/** \brief Re-initializes I/O pins used by the specified tasks
  *
  * It is possible to stop a Sensor Controller task and let the System CPU borrow and operate its I/O
  * pins. For example, the Sensor Controller can operate an SPI interface in one application state while
  * the System CPU with SSI operates the SPI interface in another application state.
  *
  * This function must be called before \ref scifExecuteTasksOnceNbl() or \ref scifStartTasksNbl() if
  * I/O pins belonging to Sensor Controller tasks have been borrowed System CPU peripherals.
  *
  * \param[in]      bvTaskIds
  *     Bit-vector of task IDs for the task I/Os to be re-initialized
  */
void scifReinitTaskIo(uint32_t bvTaskIds) {
    if (bvTaskIds & (1 << SCIF_TEMPERATURE_TASK_ID)) {
        scifReinitIo(8, -1);
        scifReinitIo(13, -1);
        HWREG((IOC_BASE + IOC_O_IOCFG0) + pAuxIoIndexToMcuIocfgOffsetLut[13]) |= IOC_IOCFG0_HYST_EN_M;
        scifReinitIo(12, -1);
        HWREG((IOC_BASE + IOC_O_IOCFG0) + pAuxIoIndexToMcuIocfgOffsetLut[12]) |= IOC_IOCFG0_HYST_EN_M;
    }
} // scifReinitTaskIo




/// Driver setup data, to be used in the call to \ref scifInit()
const SCIF_DATA_T scifDriverSetup = {
    (volatile SCIF_INT_DATA_T*) 0x400E00D6,
    (volatile SCIF_TASK_CTRL_T*) 0x400E00DE,
    (volatile uint16_t*) 0x400E00CE,
    0x0000,
    sizeof(pAuxRamImage),
    pAuxRamImage,
    pScifTaskDataStructInfoLut,
    pAuxIoIndexToMcuIocfgOffsetLut,
    scifTaskResourceInit,
    scifTaskResourceUninit
};




/** \brief Starts or modifies RTC-based task scheduling tick generation
  *
  * RTC-based tick generation will wake up the Sensor Controller first at the specified value of the RTC
  * and then periodically at the specified interval. The application must call this function after
  * calling \ref scifInit().
  *
  * The application must ensure that:
  * - \a tickStart is not in the past (prefer using \ref scifStartRtcTicksNow() to avoid this)
  * - \a tickPeriod is not too short. RTC ticks will be skipped silently if the Sensor Controller does
  *   not complete its tasks within a single tick interval.
  *
  * \param[in]      tickStart
  *     RTC value when the first tick is generated:
  *     - Bits 31:16 = seconds
  *     - Bits 15:0 = 1/65536 of a second
  * \param[in]      tickPeriod
  *     Interval at which subsequent ticks are generated:
  *     - Bits 31:16 = seconds
  *     - Bits 15:0 = 1/65536 of a second
  */
void scifStartRtcTicks(uint32_t tickStart, uint32_t tickPeriod) {
    HWREG(AON_RTC_BASE + AON_RTC_O_CH2CMP) = tickStart;
    HWREG(AON_RTC_BASE + AON_RTC_O_CH2CMPINC) = tickPeriod;
    HWREG(AON_RTC_BASE + AON_RTC_O_CHCTL) |= AON_RTC_CHCTL_CH2_EN_M | AON_RTC_CHCTL_CH2_CONT_EN_M;
    HWREG(AON_EVENT_BASE + AON_EVENT_O_AUXWUSEL) =
        (HWREG(AON_EVENT_BASE + AON_EVENT_O_AUXWUSEL) & ~AON_EVENT_AUXWUSEL_WU0_EV_M) |
        (AON_EVENT_AUXWUSEL_WU0_EV_RTC_CH2);
} // scifStartRtcTicks




/** \brief Starts or modifies RTC-based task scheduling tick generation
  *
  * RTC-based tick generation will wake up the Sensor Controller first after approximately 128 us and
  * then periodically at the specified interval. The application must call this function after calling
  * \ref scifInit().
  *
  * The application must ensure that \a tickPeriod is not too short. RTC ticks will be skipped silently
  * if the Sensor Controller does not complete its tasks within a single tick interval.
  *
  * \param[in]      tickPeriod
  *     Interval at which subsequent ticks are generated:
  *     - Bits 31:16 = seconds
  *     - Bits 15:0 = 1/65536 of a second
  */
void scifStartRtcTicksNow(uint32_t tickPeriod) {
    uint32_t key, sec, subsec;
    key = scifOsalEnterCriticalSection();
    sec = HWREG(AON_RTC_BASE + AON_RTC_O_SEC);
    subsec = HWREG(AON_RTC_BASE + AON_RTC_O_SUBSEC);
    scifStartRtcTicks(((sec << 16) | (subsec >> 16)) + 8, tickPeriod);
    scifOsalLeaveCriticalSection(key);
} // scifStartRtcTicksNow




/** \brief Stops RTC-based task scheduling tick generation
  *
  * The application must call this function before calling \ref scifUninit().
  */
void scifStopRtcTicks(void) {
    HWREG(AON_RTC_BASE + AON_RTC_O_CHCTL) &= ~(AON_RTC_CHCTL_CH2_EN_M | AON_RTC_CHCTL_CH2_CONT_EN_M);
    HWREG(AON_EVENT_BASE + AON_EVENT_O_AUXWUSEL) =
        (HWREG(AON_EVENT_BASE + AON_EVENT_O_AUXWUSEL) & ~AON_EVENT_AUXWUSEL_WU0_EV_M) |
        (AON_EVENT_AUXWUSEL_WU0_EV_NONE);
    HWREG(AON_RTC_BASE + AON_RTC_O_SYNC);
} // scifStopRtcTicks


//@}


// Generated by PSILON-VAIO at 2017-05-25 13:13:50.846
