/***********************************************************************/
/*헤더 파일 포함*/
/***********************************************************************/
#include "Driver_Can.h"  // CAN 드라이버를 위한 헤더 파일 포함.


/*********************************************************************************************************************/
/*-------------------------------------------------전역 변수--------------------------------------------------*/
/*********************************************************************************************************************/
AppLedType g_led;  // LED 구성 및 제어를 위한 전역 변수.


/***********************************************************************/
/*매크로 정의*/
/***********************************************************************/


/***********************************************************************/
/*타입 정의*/
/***********************************************************************/


/***********************************************************************/
/*정적 함수 프로토타입*/
/***********************************************************************/




/* 인터럽트 서비스 루틴을 정의하는 매크로.*/

IFX_INTERRUPT(canIsrTxHandler, 0, ISR_PRIORITY_CAN_TX);   //

/***********************************************************************/
/*변수*/
/***********************************************************************/

App_MulticanBasic g_MulticanBasic;          // CAN 구성을 위한 구조체
volatile CanRxMsg rec;                      // 수신된 CAN 메시지를 저장하는 변수
int a[8]={0,0x1,0x2,0x3,0x4,0x5,0x6,0x7};   // 데이터 값의 배열


/***********************************************************************/
/*함수*/
/***********************************************************************/

// CAN 드라이버를 초기화하는 함수.
void Driver_Can_Init(void)
{
    // 모듈 설정 생성
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

    // 인터럽트 설정
    canConfig.nodePointer[TX_INTERRUPT_SRC_ID].priority = ISR_PRIORITY_CAN_TX;

    // 모듈 초기화
    IfxMultican_Can_initModule(&g_MulticanBasic.drivers.can, &canConfig);

    // CAN 노드 설정 생성
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_MulticanBasic.drivers.can);

    canNodeConfig.baudrate = 500000UL;  // 전송 속도를 500,000bps 로 설정
    {
        canNodeConfig.nodeId    = IfxMultican_NodeId_0;         // CAN 노드 ID를 0으로 설정
        canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;  // 수신 핀 설정
        canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;     // 핀 모드를 풀업으로 설정
        canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;  // 송신 핀 설정
        canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;  // 핀 모드를 푸시-풀로 설정

        IfxMultican_Can_Node_init(&g_MulticanBasic.drivers.canSrcNode, &canNodeConfig);
    }

    // 메시지 객체 설정 생성
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;           //구조체 변수 canMsgObjConfig를 선언
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_MulticanBasic.drivers.canSrcNode);   //구조체 변수 canMsgObjConfig를 초기화하는 함수 호출

    canMsgObjConfig.msgObjId              = 0;                               // 메시지 객체 ID를 0으로 설정
    canMsgObjConfig.messageId             = 0x100;                           // 메시지 ID를 0x100으로 설정
    canMsgObjConfig.acceptanceMask        = 0x7FFFFFFFUL;                    // 수신 허용 마스크를 0x7FF로 설정
    canMsgObjConfig.frame                 = IfxMultican_Frame_transmit;      // 프레임 설정을 송신으로 설정
    canMsgObjConfig.control.messageLen    = IfxMultican_DataLengthCode_8;    // 데이터 길이를 8로 설정
    canMsgObjConfig.control.extendedFrame = FALSE;                           // 메시지 객체의 확장 프레임(Extended Frame)설정을 비활성화
    canMsgObjConfig.control.matchingId    = TRUE;                            // 메세지 ID가 수신된 CAN 메세지 ID 와 일치시 수신

    /*
    canMsgObjConfig.control.transmitMode  = IfxMultican_TransmitMode_singleShot;  // 송신 모드를 단일 발사로 설정
    canMsgObjConfig.control.messageMarker = 0;  // 메시지 마커를 0으로 설정
    canMsgObjConfig.dataLength             = IfxMultican_DataLengthCode_8;  // 데이터 길이를 8로 설정
    */

    canMsgObjConfig.txInterrupt.enabled    = TRUE;  // 송신 인터럽트 활성화
    canMsgObjConfig.txInterrupt.srcId      = TX_INTERRUPT_SRC_ID;  // 송신 인터럽트 소스 ID 설정

    // 메시지 객체 초기화
    IfxMultican_Can_MsgObj_init(&g_MulticanBasic.drivers.canSrcMsgObj, &canMsgObjConfig);

    // IO 포트 초기화
    IfxPort_setPinModeOutput(STB, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    // CAN 칩의 STB 핀 설정 (Low 레벨 활성화)
    IfxPort_setPinLow(STB);
}


// CAN 전송 테스트를 수행하는 함수.
void Driver_Can_TxTest(void)
{
    const uint32 dataLow  = 0x12340000;     // 데이터의 하위 16비트 설정
    const uint32 dataHigh = 0x9abc0000;     // 데이터의 상위 16비트 설정

    // 데이터 전송
    {
        IfxMultican_Message msg;            // IfxMultican_Message 구조체 변수 msg 선언

        // msg 구조체를 초기화하고, ID, 데이터 로우, 데이터 하이, 데이터 길이(DLC)를 설정
        IfxMultican_Message_init(&msg, 0x100, dataLow, dataHigh, IfxMultican_DataLengthCode_8);

        // g_MulticanBasic.drivers.canSrcMsgObj를 통해 msg를 전송하고, 전송이 완료될 때까지 반복
        while (IfxMultican_Can_MsgObj_sendMessage(&g_MulticanBasic.drivers.canSrcMsgObj, &msg) == IfxMultican_Status_notSentBusy)
        {}
    }
}


// CAN 메시지를 전송하는 함수.
void CAN_send(CanRxMsg *message)
{
    IfxMultican_Message msg;        // IfxMultican_Message 구조체 변수 msg 선언

    // message 구조체의 Data 배열 요소를 이용하여 32비트 데이터의 하위 16비트를 구성
    const unsigned dataLow = message->Data[0] | (message->Data[1] << 8) | (message->Data[2] << 16) | (message->Data[3] << 24);
    const unsigned dataHigh = message->Data[4] | (message->Data[5] << 8) | (message->Data[6] << 16) | (message->Data[7] << 24);

    // msg 구조체를 초기화하고, ID, 데이터 로우, 데이터 하이, 데이터 길이(DLC)를 설정
    IfxMultican_Message_init(&msg, message->ID, dataLow, dataHigh, message->DLC);

    // g_MulticanBasic.drivers.canSrcMsgObj를 통해 msg를 전송하고, 전송이 완료될 때까지 반복
    while (IfxMultican_Can_MsgObj_sendMessage(&g_MulticanBasic.drivers.canSrcMsgObj, &msg) == IfxMultican_Status_notSentBusy)
    {

    }
}


// CAN 테스트를 수행하는 함수.
void CAN_TEST(void)
{
    CanRxMsg MES;       // CanRxMsg 구조체 변수 MES 선언
    int i=0;
    MES.ID = 0x890;     // MES 구조체의 ID 멤버에 0x890 값 할당
    MES.IDE = 0;        // MES 구조체의 IDE 멤버에 0 할당 (표준 ID 사용)
    MES.DLC = 8;        // MES 구조체의 DLC 멤버에 8 할당 (데이터 길이)
    for(i=0; i<8; i++)
    {
        MES.Data[i] = a[i];  // MES 구조체의 Data 배열에 a배열의 i번째 값 할당
    }
    CAN_send(&MES);          // MES 구조체를 send 함수로 전송
}


// TX 인터럽트가 발생했을 때 실행되는 함수.
void canIsrTxHandler(void)
{
    /* Just to indicate that the CAN message has been transmitted by turning on LED1 */
    blinkLED1();

    //IfxPort_setPinLow(g_led.led1.port, g_led.led1.pinIndex);
}


void initLed(void)
{
    /* ======================================================================
     * Configuration of the pins connected to the LEDs:
     * ======================================================================
     *  - define the GPIO port
     *  - define the GPIO pin that is the connected to the LED
     *  - define the general GPIO pin usage (no alternate function used)
     *  - define the pad driver strength
     * ======================================================================
     */
    // LED1에 연결된 핀 설정
    g_led.led1.port      = &MODULE_P00;                             // LED1이 연결된 GPIO 포트를 지정
    g_led.led1.pinIndex  = PIN5;                                    // LED1이 연결된 GPIO 핀을 지정
    g_led.led1.mode      = IfxPort_OutputIdx_general;               // 일반 GPIO 핀 사용 (대체 기능 없음)
    g_led.led1.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;  // 패드 드라이버 강도 설정

    // LED2에 연결된 핀 설정
    g_led.led2.port      = &MODULE_P00;                             // LED2가 연결된 GPIO 포트를 지정
    g_led.led2.pinIndex  = PIN6;                                    // LED2가 연결된 GPIO 핀을 지정
    g_led.led2.mode      = IfxPort_OutputIdx_general;               // 일반 GPIO 핀 사용 (대체 기능 없음)
    g_led.led2.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;  // 패드 드라이버 강도 설정

    /* Initialize the pins connected to LEDs to level "HIGH"; will keep the LEDs turned off as default state */
    // LED 핀 초기화 및 상태 설정 (초기에 끄기 위해 HIGH로 설정)
    IfxPort_setPinHigh(g_led.led1.port, g_led.led1.pinIndex);       // LED1 핀을 HIGH로 설정하여 LED를 끔
    IfxPort_setPinHigh(g_led.led2.port, g_led.led2.pinIndex);       // LED2 핀을 HIGH로 설정하여 LED를 끔

    /* Set the pin input/output mode for both pins connected to the LEDs */
    // LED에 연결된 핀의 입출력 모드 설정
    IfxPort_setPinModeOutput(g_led.led1.port, g_led.led1.pinIndex, IfxPort_OutputMode_pushPull, g_led.led1.mode);  // LED1 핀을 출력 모드로 설정
    IfxPort_setPinModeOutput(g_led.led2.port, g_led.led2.pinIndex, IfxPort_OutputMode_pushPull, g_led.led2.mode);  // LED2 핀을 출력 모드로 설정

    /* Set the pad driver mode for both pins connected to the LEDs */
    // LED에 연결된 핀의 패드 드라이버 모드 설정
    IfxPort_setPinPadDriver(g_led.led1.port, g_led.led1.pinIndex, g_led.led1.padDriver);  // LED1 핀의 패드 드라이버 모드 설정
    IfxPort_setPinPadDriver(g_led.led2.port, g_led.led2.pinIndex, g_led.led2.padDriver);  // LED2 핀의 패드 드라이버 모드 설정
}

void blinkLED1(void)
{
    //IfxPort_togglePin(LED1);                                                     /* Toggle the state of the LED      */

    // TX 인터럽트 발생시 LED 키고 끔
    IfxPort_setPinHigh(LED1);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_1ms));    /* Wait 1 milliseconds            */
    IfxPort_setPinLow(LED1);
}
void blinkLED2(void)
{
    IfxPort_togglePin(LED2);                                                     /* Toggle the state of the LED      */
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_10ms));    /* Wait 500 milliseconds            */
}
