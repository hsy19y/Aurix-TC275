#include "IfxPort.h"
#include "Bsp.h"


#define LED_1         &MODULE_P00,8
#define LED_2         &MODULE_P00,10
#define LED_3         &MODULE_P00,12

#define WAIT_TIME   5  
                                                 
int time = 0;


void initLED(void)
{
    
    IfxPort_setPinModeOutput(LED_1, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(LED_2, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(LED_3, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    IfxPort_setPinHigh(LED_1);
    IfxPort_setPinHigh(LED_2);
    IfxPort_setPinHigh(LED_3);
}


void blinkLED(void)
{
    time++;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME));
     
    if(time%1==0){IfxPort_togglePin(LED_1);};
    if(time%2==0){IfxPort_togglePin(LED_2);};
    if(time%4==0){IfxPort_togglePin(LED_3);};


}