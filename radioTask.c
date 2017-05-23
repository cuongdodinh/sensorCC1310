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

/* POSIX Header files */
#include <pthread.h>

/* Files needed to turn off the external flash on 2650, 1310, 1350, 2640R2 LaunchPads */
#if defined(CC2650_LAUNCHXL) || defined(CC2640R2_LAUNCHXL) || defined(CC1310_LAUNCHXL) || defined(CC1350_LAUNCHXL)
    #include <ti/drivers/SPI.h>
    #include <ti/mw/extflash/ExtFlash.h>
#endif

/* Board Header files */
#include "Board.h"

#include <unistd.h>

/* Undefine to not use async mode */
#define RFEASYLINKTX_ASYNC

#define RFEASYLINKTX_TASK_STACK_SIZE    1024
#define RFEASYLINKTX_TASK_PRIORITY      2

#define RFEASYLINKTXPAYLOAD_LENGTH      12

Task_Struct radioTask;    /* not static so you can see in ROV */
static Task_Params radioTaskParams;
static uint8_t radioTaskStack[RFEASYLINKTX_TASK_STACK_SIZE];

Event_Struct radioEvent;  /* not static so you can see in ROV */
static Event_Handle radioEventHandle;
#define RADIO_EVENT_SEND_VALUE    (uint32_t)(1 << 0)

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

  /*  if (status == EasyLink_Status_Success)
    {

        PIN_setOutputValue(pinHandle, Board_PIN_LED1,!PIN_getOutputValue(Board_PIN_LED1));
    }
*/

    Semaphore_post(txDoneSem);
}
#endif //RFEASYLINKTX_ASYNC

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void *radioTaskFxn (void *arg0)
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

//    return;

    while(1) {

 //       PIN_setOutputValue(pinHandle, Board_PIN_LED1, 1);

//        uint32_t events = Event_pend(radioEventHandle, 0, RADIO_EVENT_SEND_VALUE, BIOS_WAIT_FOREVER);

 /*       EasyLink_TxPacket txPacket =  { {0}, 0, 0, {0} };

        seqNumber++;

        txPacket.payload[0] = (uint8_t)(seqNumber >> 8);
        txPacket.payload[1] = (uint8_t)(seqNumber);

        EasyLink_getIeeeAddr(&txPacket.payload[2]);

        txPacket.payload[10] = temperature;
        txPacket.payload[11] = humidity;

        txPacket.len = RFEASYLINKTXPAYLOAD_LENGTH;
        txPacket.dstAddr[0] = 0xaa;


        EasyLink_transmitAsync(&txPacket, txDoneCb);

        if(Semaphore_pend(txDoneSem, (2700000 / Clock_tickPeriod)) == FALSE)
        {

        }

 //       PIN_setOutputValue(pinHandle, Board_PIN_LED1, 0);*/

 //       radioTaskSendValue (temperature, humidity);

        Task_sleep (3600);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void radioTaskInit (PIN_Handle inPinHandle) {
    pinHandle = inPinHandle;

    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&radioEvent, &eventParam);
    radioEventHandle = Event_handle(&radioEvent);

    Task_Params_init(&radioTaskParams);
    radioTaskParams.stackSize = RFEASYLINKTX_TASK_STACK_SIZE;
    radioTaskParams.priority = RFEASYLINKTX_TASK_PRIORITY;
    radioTaskParams.stack = &radioTaskStack;
    radioTaskParams.arg0 = (UInt)1000000;

    Task_construct(&radioTask, radioTaskFxn, &radioTaskParams, NULL);


    pthread_t           thread0;
    pthread_attr_t      pAttrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Shut down external flash on LaunchPads. It is powered on by default
     * but can be shut down through SPI
     */
#if defined(CC2650_LAUNCHXL) || defined(CC2640R2_LAUNCHXL) || defined(CC1310_LAUNCHXL) || defined(CC1350_LAUNCHXL)
//    SPI_init();
//    bool extFlashOpened = ExtFlash_open();
//    if (extFlashOpened) {
//        ExtFlash_close();
//    }
#endif

//    EasyLink_init(EasyLink_Phy_Custom);
//    EasyLink_init(EasyLink_Phy_625bpsLrm);

    /*
     * If you wish to use a frequency other than the default, use
     * the following API:

     */
//    EasyLink_setFrequency(433000000);

    /* Set output power to 12dBm */
//    EasyLink_setRfPwr(12);

  //  radioTaskSendValue (1, 0);

    return;

//    PIN_setOutputValue(pinHandle, Board_PIN_LED1, 1);

    /* Create application thread */
    pthread_attr_init(&pAttrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    retc |= pthread_attr_setstacksize(&pAttrs, RFEASYLINKTX_TASK_STACK_SIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    /* Create threadFxn0 thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc = pthread_create(&thread0, &pAttrs, radioTaskFxn, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void radioTaskSendValue (uint8_t _temperature, uint8_t _humidity)
{
    PIN_setOutputValue(pinHandle, Board_PIN_LED1,!PIN_getOutputValue(Board_PIN_LED1));
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
