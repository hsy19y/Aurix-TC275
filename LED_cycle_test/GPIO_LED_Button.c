/**********************************************************************************************************************
 * \file GPIO_LED_Button.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "IfxPort.h"

/*********************************************************************************************************************/
/*-----------------------------------------------------Macros--------------------------------------------------------*/
/*********************************************************************************************************************/
#define LED_A     &MODULE_P00,5   /* Port pin for the LED     */
#define LED_B     &MODULE_P00,6

#define BUTTON  &MODULE_P00,7   /* Port pin for the button  */

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* Function to configure the port pins for the push button and the LED */
void init_GPIOs(void)
{
    /* Setup the port pin connected to the LED to general output mode push-pull. This function can be used
     * to initialize any port pin by specifying the port number, pin number and port pin mode.
     */
    IfxPort_setPinMode(LED_A, IfxPort_Mode_outputPushPullGeneral); //(outputPushpull)    출력으로 설정
    IfxPort_setPinMode(LED_B, IfxPort_Mode_outputPushPullGeneral);

    /* Setup the port pin connected to the push button to input mode. This function can be used to initialize any
     * port to input mode by just specifying the port number as illustrated.
     */
    IfxPort_setPinMode(BUTTON, IfxPort_Mode_inputPullUp); //버튼을 입력으로 설정, Pull up 저항으로 연결 -> 스위치가 입력시 GND로 접지(0V)
}

/* Depending on the the state of the "BUTTON" port pin, the LED is turned on or off */
void control_LED(void)
{
    /* With the routine getPinState() the value of a particular pin can be retrieved. This
     * function can be used to retrieve any port state by just specifying the port number
     * as illustrated.
     */
    if(IfxPort_getPinState(BUTTON) == 0) //입력이 low 이면(누름)
    {
        /* With the routine setPinState() the state of the port can be set to drive either
         * LOW or HIGH. This function can be used to retrieve any port state by just
         * specifying the port number as illustrated.
         */
        IfxPort_setPinState(LED_A, IfxPort_State_low); // 출력을 low 로 설정
        IfxPort_setPinState(LED_B, IfxPort_State_high);
    }
    else
    {
        IfxPort_setPinState(LED_A, IfxPort_State_high); //아니면 출력을 high 로 설정
        IfxPort_setPinState(LED_B, IfxPort_State_low);
    }
}

