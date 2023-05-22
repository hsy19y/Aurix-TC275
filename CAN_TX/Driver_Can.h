#ifndef DRIVER_CAN
#define DRIVER_CAN

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"               // Ifx 데이터 형식을 포함합니다.
#include "IfxCpu.h"                  // Ifx CPU 기능을 포함합니다.
#include "IfxMultican_Can.h"         // Ifx MultiCAN 드라이버를 포함합니다.
#include "IfxPort.h"                 // Ifx 포트 기능을 포함합니다.
#include "Bsp.h"                     // 보드 지원 패키지(BSP)를 포함합니다.


/***********************************************************************/
/*Define*/
/***********************************************************************/

#define STB         &MODULE_P20,6     // STB를 P20.6 정의

#define TX_INTERRUPT_SRC_ID         IfxMultican_SrcId_0         /* 송신 인터럽트 서비스 요청 ID를 정의합니다. */
#define ISR_PRIORITY_CAN_TX         2                           /* CAN TX 인터럽트 우선순위를 정의합니다. */
#define PIN5                        5                           /* TX ISR에서 사용되는 LED1이 연결된 핀입니다. */
#define PIN6                        6                           /* RX ISR에서 사용되는 LED2가 연결된 핀입니다. */


#define WAIT_TIME_1ms               1       // 1밀리초의 대기 시간
#define WAIT_TIME_5ms               5       // 5밀리초의 대기 시간
#define WAIT_TIME_10ms              10      // 10밀리초의 대기 시간
#define WAIT_TIME_20ms              20      // 20밀리초의 대기 시간
#define WAIT_TIME_50ms              50      // 50밀리초의 대기 시간
#define WAIT_TIME_100ms             100     // 100밀리초의 대기 시간

#define LED1         &MODULE_P00,5     // LED1을 P00.5 핀으로 정의
#define LED2         &MODULE_P00,6     // LED2를 P00.6 핀으로 정의


/***********************************************************************/
/*Typedef*/
/***********************************************************************/
typedef struct
{
    struct
    {
        IfxMultican_Can        can;          /**< \brief CAN 드라이버 핸들 */
        IfxMultican_Can_Node   canSrcNode;   /**< \brief CAN 소스 송신 노드 */
        IfxMultican_Can_MsgObj canSrcMsgObj; /**< \brief CAN 소스 송신 메시지 객체 */
    }drivers;
} App_MulticanBasic;


typedef struct
{
    unsigned long ID;                      // CAN 메시지의 식별자 ID
    unsigned char IDE;                     // 식별자 확장 (IDE) 플래그
    unsigned char DLC;                     // 데이터 길이 코드 (DLC)
    unsigned char Data[8];                 // CAN 메시지의 데이터 배열
} CanRxMsg;

typedef struct
{
    IfxPort_Pin_Config              led1;   // LED1의 설정 구조체
    IfxPort_Pin_Config              led2;   // LED2의 설정 구조체
} AppLedType;


/***********************************************************************/
/*External Variable*/
/***********************************************************************/
IFX_EXTERN App_MulticanBasic g_MulticanBasic;   // 외부 변수 g_MulticanBasic의 선언


/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/
extern void Driver_Can_Init(void);          // CAN 드라이버를 초기화하기 위한 함수 원형
extern void Driver_Can_TxTest(void);        // CAN 송신 테스트를 위한 함수 원형
extern void CAN_send(CanRxMsg *message);    // CAN 메시지를 전송하기 위한 함수 원형
extern void CAN_TEST(void);                 // CAN 테스트를 위한 함수 원형
void initLed(void);                         // LED를 초기화하기 위한 함수 원형
void blinkLED1(void);                       // LED1을 깜빡이기 위한 함수 원형
void blinkLED2(void);                       // LED2를 깜빡이기 위한 함수 원형
#endif /* DRIVER_STM */
