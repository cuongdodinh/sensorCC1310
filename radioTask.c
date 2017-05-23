//16.05.17 Avkhadiev Rustem

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

/* EasyLink API Header files */
#include <ti/drivers/PIN.h>
#include "easylink/EasyLink.h"

/* Board Header files */
#include "Board.h"

#include "radioTask.h"

/* XDCtools Header files */
#include <stdlib.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

#include <ti/devices/cc13x0/driverlib/aon_batmon.h>

/* POSIX Header files */
//#include <pthread.h>


/* Board Header files */
#include "Board.h"

#include <unistd.h>

/* Undefine to not use async mode */
#define RFEASYLINKTX_ASYNC

#define RFEASYLINKTX_TASK_STACK_SIZE    1024
#define RFEASYLINKTX_TASK_PRIORITY      2

#define RFEASYLINKTXPAYLOAD_LENGTH      13

Task_Struct radioTask;    /* not static so you can see in ROV */
static Task_Params radioTaskParams;
static uint8_t radioTaskStack[RFEASYLINKTX_TASK_STACK_SIZE];


static uint16_t seqNumber;

/* Pin driver handle */
static PIN_Handle pinHandle;

uint8_t temperature;
uint8_t humidity;

#ifdef RFEASYLINKTX_ASYNC
static Semaphore_Handle txDoneSem;
#endif //RFEASYLINKTX_ASYNC

#ifdef RFEASYLINKTX_ASYNC

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void txDoneCb(EasyLink_Status status)
{
 //       PIN_setOutputValue(pinHandle, Board_PIN_LED2,!PIN_getOutputValue(Board_PIN_LED2));
}
#endif //RFEASYLINKTX_ASYNC

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void radioTaskFxn (UArg arg0, UArg arg1)
{
  //  PIN_setOutputValue(pinHandle, Board_PIN_LED1, 1);

#ifdef RFEASYLINKTX_ASYNC
    /* Create a semaphore for Async */
    Semaphore_Params params;
    Error_Block eb;

    /* Init params */
    Semaphore_Params_init(&params);
    Error_init(&eb);

    /* Create semaphore instance */
    txDoneSem = Semaphore_create(0, &params, &eb);
#endif //TX_ASYNC

    EasyLink_init(EasyLink_Phy_Custom);
//    EasyLink_init(EasyLink_Phy_625bpsLrm);

    /*
     * If you wish to use a frequency other than the default, use
     * the following API:

     */
//    EasyLink_setFrequency(433000000);

    /* Set output power to 12dBm */
    EasyLink_setRfPwr(12);

    while(1) {
        sleep (3600);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void radioTaskInit (PIN_Handle inPinHandle) {
    pinHandle = inPinHandle;

    Task_Params_init(&radioTaskParams);
    radioTaskParams.stackSize = RFEASYLINKTX_TASK_STACK_SIZE;
    radioTaskParams.priority = RFEASYLINKTX_TASK_PRIORITY;
    radioTaskParams.stack = &radioTaskStack;
    radioTaskParams.arg0 = (UInt)1000000;

    Task_construct(&radioTask, radioTaskFxn, &radioTaskParams, NULL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void radioTaskSendValue (uint8_t _temperature, uint8_t _humidity)
{
 //   PIN_setOutputValue(pinHandle, Board_PIN_LED1,!PIN_getOutputValue(Board_PIN_LED1));
 //   return;
  //  EasyLink_init(EasyLink_Phy_Custom);
//    EasyLink_init(EasyLink_Phy_625bpsLrm);

    /*
     * If you wish to use a frequency other than the default, use
     * the following API:

     */
//    EasyLink_setFrequency(433000000);

    /* Set output power to 12dBm */
//    EasyLink_setRfPwr(12);

    temperature = _temperature;
    humidity = _humidity;


    EasyLink_TxPacket txPacket =  { {0}, 0, 0, {0} };

    seqNumber++;

    txPacket.payload[0] = (uint8_t)(seqNumber >> 8);
    txPacket.payload[1] = (uint8_t)(seqNumber);

    EasyLink_getIeeeAddr(&txPacket.payload[2]);

    txPacket.payload[10] = temperature;
    txPacket.payload[11] = humidity;

    txPacket.payload[12] = (uint8_t)((AONBatMonBatteryVoltageGet()) >> 3);

    txPacket.len = RFEASYLINKTXPAYLOAD_LENGTH;
    txPacket.dstAddr[0] = 0xaa;

    /* Create a semaphore for Async */
    Semaphore_Params params;
    Error_Block eb;

    /* Init params */
    Semaphore_Params_init(&params);
    Error_init(&eb);

    /* Create semaphore instance */
    txDoneSem = Semaphore_create(0, &params, &eb);

    EasyLink_transmitAsync(&txPacket, txDoneCb);

}
