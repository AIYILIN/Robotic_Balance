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
  * @内  容  LED灯控制
  ******************************************************************************
  * @说  明
  *
  * 
  ******************************************************************************
  */

#include "ax_led.h"

/**
  * @简  述  LED 初始化
  * @参  数  无
  * @返回值  无
  */
void AX_LED_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO配置 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE); //启动GPIO时钟 
	
	//配置红色LED指示灯GPIO  推挽输出模式 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,  GPIO_Pin_3);	

	//配置绿色LED指示灯GPIO  推挽输出模式  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,  GPIO_Pin_4);	
}

/******************* (C) 版权 2022 XTARK **************************************/
