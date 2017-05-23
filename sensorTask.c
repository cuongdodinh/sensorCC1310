//07.05.17 Avkhadiev Rustem

#include <sce/scif.h>
#include "sensorTask.h"

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>

#include <ti/display/Display.h>
#include <math.h>

#include "radioTask.h"

#define BV(x)    (1 << (x))


/***** Defines *****/
#define SENSOR_TASK_STACK_SIZE 1024
#define SENSOR_TASK_PRIORITY   3

#define SENSOR_EVENT_ALL                  0xFFFFFFFF
#define SENSOR_EVENT_NEW_SENSOR_VALUE    (uint32_t)(1 << 0)

/***** Variable declarations *****/
static Task_Params sensorTaskParams;
Task_Struct sensorTask;    /* not static so you can see in ROV */
static uint8_t sensorTaskStack[SENSOR_TASK_STACK_SIZE];
Event_Struct sensorEvent;  /* not static so you can see in ROV */
static Event_Handle sensorEventHandle;

static uint8_t temperatureValue;
static uint8_t humidityValue;

static Display_Handle hDisplaySerial;

/***** Prototypes *****/
static void sensorTaskFunction(UArg arg0, UArg arg1);
static void newDataFromSensorCallback(void);

static void scCtrlReadyCallback(void)
{
} // scCtrlReadyCallback


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SensorTaskInit(void)
{

    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&sensorEvent, &eventParam);
    sensorEventHandle = Event_handle(&sensorEvent);

    /* Create the node task */
    Task_Params_init(&sensorTaskParams);
    sensorTaskParams.stackSize = SENSOR_TASK_STACK_SIZE;
    sensorTaskParams.priority = SENSOR_TASK_PRIORITY;
    sensorTaskParams.stack = &sensorTaskStack;
    Task_construct(&sensorTask, sensorTaskFunction, &sensorTaskParams, NULL);
}

int a = 0;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void sensorTaskFunction(UArg arg0, UArg arg1)
{

    // Initialize the Sensor Controller
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(scCtrlReadyCallback);
    scifOsalRegisterTaskAlertCallback(newDataFromSensorCallback);
    scifInit(&scifDriverSetup);

    // Set the Sensor Controller task tick interval to 1 second
    scifStartRtcTicksNow(0x00040000);

    // Start Sensor Controller task
    scifStartTasksNbl(BV(SCIF_TEMPERATURE_TASK_ID));

    return;

/*    Display_Params params;
    Display_Params_init(&params);
//    params.lineClearMode = DISPLAY_CLEAR_BOTH;

    hDisplaySerial = Display_open(Display_Type_UART, &params);

    Display_printf(hDisplaySerial, 0, 0, "Waiting for SCE reading...");*/



    while (1)
    {
        sleep (4);
        /* Wait for event */



 /*       uint32_t events = Event_pend(sensorEventHandle, 0, SENSOR_EVENT_NEW_SENSOR_VALUE, BIOS_WAIT_FOREVER);
        a++;
        Display_printf(hDisplaySerial, 0, 0, "Temperature: %02d,   Humidity: %12d", temperatureValue, humidityValue);*/


            /* Send ADC value to concentrator */
            //NodeRadioTask_sendAdcData(latestAdcValue);

            /* update display */
  //          updateLcd();


    }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void newDataFromSensorCallback(void)
{
    /* Clear the ALERT interrupt source */
    scifClearAlertIntSource();

    float buff = scifTaskData.temperature.output.resultTemperatureRaw * 175.72 / 65536 - 46.85;

    temperatureValue = (char) round (buff);

    buff = scifTaskData.temperature.output.resultHumidityRaw * 125 / 65536 - 6;

    humidityValue = (char) round (buff);

    /* Post event */
   // Event_post(sensorEventHandle, SENSOR_EVENT_NEW_SENSOR_VALUE);

    /* Acknowledge the alert event */
    scifAckAlertEvents();

    radioTaskSendValue (temperatureValue, humidityValue);

}
