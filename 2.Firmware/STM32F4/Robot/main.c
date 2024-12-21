/**			                                                    
		   ____                    _____ _______ _____       XTARK@���˴���
		  / __ \                  / ____|__   __|  __ \ 
		 | |  | |_ __   ___ _ __ | |       | |  | |__) |
		 | |  | | '_ \ / _ \ '_ \| |       | |  |  _  / 
		 | |__| | |_) |  __/ | | | |____   | |  | | \ \ 
		  \____/| .__/ \___|_| |_|\_____|  |_|  |_|  \_\
				| |                                     
				|_|                OpenCTR   �����˿�����
									 
  ****************************************************************************** 
  *           
  * ��Ȩ���У� XTARK@���˴���  ��Ȩ���У�����ؾ�
  * ��˾��վ�� www.xtark.cn   www.tarkbot.com
  * �Ա����̣� https://xtark.taobao.com  
  * ����΢�ţ� ���˴��£���ע���ںţ���ȡ���¸�����Ѷ��
  *           
  ******************************************************************************
  * @��  ��  Musk Han@XTARK
  * @��  ��  ������
  *  
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>   

#include "ax_sys.h"    //ϵͳ����
#include "ax_delay.h"  //�����ʱ
#include "ax_led.h"    //LED�ƿ���
#include "ax_uart1.h"  //���Դ���

#include "ax_ps2.h"   //PS2�ֱ�

JOYSTICK_TypeDef my_joystick;  //�ֱ���ֵ�ṹ��

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	   
	
	//��ʱ������ʼ��
	AX_DELAY_Init();   
    
	//LED��ʼ��
	AX_LED_Init();  

	//���Դ��ڳ�ʼ��
	AX_UART1_Init(115200); //���Դ���
	
	printf("***OpenCTR PS2�����ֱ�����***\r\n\r\n");	
	
    //��������	
	AX_PS2_Init();	//PS2��ʼ��
	
	while (1)
	{
		//�ֱ���ֵɨ��
		AX_PS2_ScanKey(&my_joystick);
		AX_Delayms(30);
//		
//		//��ӡ�ֱ���ֵ
//		printf("MODE:%2x BTN1:%2x BTN2:%2x RJOY_LR:%2x RJOY_UD:%2x LJOY_LR:%2x LJOY_UD:%2x\r\n",
//		my_joystick.mode, my_joystick.btn1, my_joystick.btn2, 
//		my_joystick.RJoy_LR, my_joystick.RJoy_UD, my_joystick.LJoy_LR, my_joystick.LJoy_UD);	
		
		
//		GPIO_SetBits(GPIOC,  GPIO_Pin_6);	
//		AX_Delayus(30);		
//		GPIO_ResetBits(GPIOC,  GPIO_Pin_6);	
//		AX_Delayus(30);		
	}
}










/******************* (C) ��Ȩ 2022 XTARK *******************************/
