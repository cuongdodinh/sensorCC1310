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

static uint8_t temperatureValue;
static uint8_t humidityValue;

static Display_Handle hDisplaySerial;

/***** Prototypes *****/
static void newDataFromSensorCallback(void);

static void scCtrlReadyCallback(void)
{
} // scCtrlReadyCallback


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SensorTaskInit(void)
{
    // Initialize the Sensor Controller
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(scCtrlReadyCallback);
    scifOsalRegisterTaskAlertCallback(newDataFromSensorCallback);
    scifInit(&scifDriverSetup);

    // Set the Sensor Controller task tick interval to 1 second
    scifStartRtcTicksNow(0x02580000);
//    scifStartRtcTicksNow(0x00050000);

    // Start Sensor Controller task
    scifStartTasksNbl(BV(SCIF_TEMPERATURE_TASK_ID));
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

    sendTaskInit (temperatureValue, humidityValue);

}
