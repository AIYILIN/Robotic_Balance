/**			                                                    
		   ____                    _____ _______ _____       XTARK@塔克创新
		  / __ \                  / ____|__   __|  __ \ 
		 | |  | |_ __   ___ _ __ | |       | |  | |__) |
		 | |  | | '_ \ / _ \ '_ \| |       | |  |  _  / 
		 | |__| | |_) |  __/ | | | |____   | |  | | \ \ 
		  \____/| .__/ \___|_| |_|\_____|  |_|  |_|  \_\
				| |                                     
				|_|                OpenCTR   机器人控制器
									 
  ****************************************************************************** 
  *           
  * 版权所有： XTARK@塔克创新  版权所有，盗版必究
  * 公司网站： www.xtark.cn   www.tarkbot.com
  * 淘宝店铺： https://xtark.taobao.com  
  * 塔克微信： 塔克创新（关注公众号，获取最新更新资讯）
  *           
  ******************************************************************************
  * @作  者  Musk Han@XTARK
  * @内  容  主程序
  *  
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>   

#include "ax_sys.h"    //系统设置
#include "ax_delay.h"  //软件延时
#include "ax_led.h"    //LED灯控制
#include "ax_uart1.h"  //调试串口

#include "ax_ps2.h"   //PS2手柄

JOYSTICK_TypeDef my_joystick;  //手柄键值结构体

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	   
	
	//延时函数初始化
	AX_DELAY_Init();   
    
	//LED初始化
	AX_LED_Init();  

	//调试串口初始化
	AX_UART1_Init(115200); //调试串口
	
	printf("***OpenCTR PS2无线手柄例程***\r\n\r\n");	
	
    //功能配置	
	AX_PS2_Init();	//PS2初始化
	
	while (1)
	{
		//手柄键值扫描
		AX_PS2_ScanKey(&my_joystick);
		AX_Delayms(30);
//		
//		//打印手柄键值
//		printf("MODE:%2x BTN1:%2x BTN2:%2x RJOY_LR:%2x RJOY_UD:%2x LJOY_LR:%2x LJOY_UD:%2x\r\n",
//		my_joystick.mode, my_joystick.btn1, my_joystick.btn2, 
//		my_joystick.RJoy_LR, my_joystick.RJoy_UD, my_joystick.LJoy_LR, my_joystick.LJoy_UD);	
		
		
//		GPIO_SetBits(GPIOC,  GPIO_Pin_6);	
//		AX_Delayus(30);		
//		GPIO_ResetBits(GPIOC,  GPIO_Pin_6);	
//		AX_Delayus(30);		
	}
}










/******************* (C) 版权 2022 XTARK *******************************/
