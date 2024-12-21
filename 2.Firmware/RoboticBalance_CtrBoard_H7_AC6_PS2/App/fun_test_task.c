#include "fun_test_task.h"
#include "usart.h"
#include "main.h"
#include "tim.h"
#include "fdcan.h"
#include "ws2812.h"
#include "can_bsp.h"
#include <string.h>
#include "stm32h7xx_hal.h"
#include <stdio.h>

#define U1_RX_LEN   5
#define U5_RX_LEN   5
#define U7_RX_LEN   5
#define U10_RX_LEN   5

uint8_t u1Rx[U1_RX_LEN];
uint8_t u1RxFlag = 0;

uint8_t u5Rx[U5_RX_LEN];
uint8_t u5RxFlag = 0;

uint8_t u7Rx[U7_RX_LEN];
uint8_t u7RxFlag = 0;

uint8_t u10Rx[U10_RX_LEN];
uint8_t u10RxFlag = 0;


uint8_t tx_data[8] = {0,1,2,3,4,5,6,7};


/* USER CODE BEGIN Header_FunTest_Entry */
/**
* @brief Function implementing the FunTest thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FunTest_Entry */
void FunTest_Entry(void const * argument)
{
    /* USER CODE BEGIN FunTest_Entry */
    DWT_Init();
    HAL_UART_Receive_IT(&huart1, u1Rx, U1_RX_LEN);
//    HAL_UART_Receive_IT(&huart5, u5Rx, U5_RX_LEN);
    HAL_UART_Receive_IT(&huart7, u7Rx, U7_RX_LEN);
    HAL_UART_Receive_IT(&huart10, u10Rx, U10_RX_LEN);
    
//    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    
    // TIM1->CCR1 = 5000;                      // 50%占空比
    // TIM1->CCR3 = 5000;                      // 50%占空比
    // TIM1->CCR1 = 5000;                      // 50%占空比
    // TIM1->CCR3 = 5000;                      // 50%占空比
	

    /* Infinite loop */
    for(;;)
    {
//        if (u1RxFlag == 1)
//        {
            // HAL_UART_Transmit(&huart1, (const uint8_t *)"U1 Test!\n", sizeof("U1 Test!\n")-1, 0xFFFF);
            // DWT_DelayUS(1000000);
            // u1RxFlag = 0;
//        }
        osDelay(1);
    }
    /* USER CODE END FunTest_Entry */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        if (memcmp(u1Rx, "test\n", U1_RX_LEN) == 0)
        {
            u1RxFlag = 1;
        }
        HAL_UART_Receive_IT(&huart1, u1Rx, U1_RX_LEN);
    }
    else if (huart == &huart7)
    {
        if (memcmp(u7Rx, "test\n", U7_RX_LEN) == 0)
        {
            u7RxFlag = 1;
        }
        HAL_UART_Receive_IT(&huart7, u7Rx, U7_RX_LEN);
    }
    else if (huart == &huart10)
    {
        if (memcmp(u10Rx, "test\n", U10_RX_LEN) == 0)
        {
            u10RxFlag = 1;
        }
        HAL_UART_Receive_IT(&huart10, u10Rx, U10_RX_LEN);
    }
}

#define  DEM_CR      *(volatile uint32_t *)0xE000EDFC
#define  DWT_CR      *(volatile uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(volatile uint32_t *)0xE0001004
#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


void DWT_Init()
{
    DEM_CR  |=  DEM_CR_TRCENA; /*对DEMCR寄存器的位24控制，写1使能DWT外设。*/
    DWT_CYCCNT = 0;/*对于DWT的CYCCNT计数寄存器清0。*/
    DWT_CR  |=  DWT_CR_CYCCNTENA;/*对DWT控制寄存器的位0控制，写1使能CYCCNT寄存器。*/
}

void DWT_DelayUS(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
    uint32_t tStart;
           
    tStart = DWT_CYCCNT; /* 刚进入时的计数器值 */
    tCnt = 0;
    tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);
    /* 需要的节拍数 */    /*SystemCoreClock :系统时钟频率*/                 

    while(tCnt < tDelayCnt)
      {
        tCnt = DWT_CYCCNT - tStart; 
        /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */       
      }
}

// void DWT_DelayMS(uint32_t _ulDelayTime)
// {
//         bsp_DelayUS(1000*_ulDelayTime);
// }

