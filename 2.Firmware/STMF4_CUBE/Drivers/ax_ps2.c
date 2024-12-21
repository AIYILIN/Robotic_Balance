#include "ax_ps2.h"
#include "ax_delay.h"
#include "ax_sys.h"
#include "main.h"
#include "string.h"
#include "stm32f4xx_hal_uart.h"
#include <stdio.h>
//PS2�ֱ������������
#define DI()     HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)

#define CMD_H()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1)
#define CMD_L()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0)

#define CS_H()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1)
#define CS_L()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0)

#define CLK_H()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1)
#define CLK_L()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0)


const  uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   //�����ȡ��������                         
uint8_t PS2_data[9] = {0};  //���յ�����


/**
  * @��  ��  PS2���ݶ�д����
  * @��  ��  data:Ҫд�������
  * @����ֵ  ��������
  */
char ps2_temp[100];
uint8_t ref,res;
static uint8_t PS2_ReadWriteData(uint8_t data)
{
	res = 0;
	for(ref = 0x01; ref > 0x00; ref <<= 1)
	{
		CLK_L();
		if(ref&data)
			CMD_H();
		else
			CMD_L();	
		
		HAL_Delay_us(16);
		
		CLK_H();

		if(DI())
			res |= ref; 
		
		HAL_Delay_us(16);	
		
	}
	
	CMD_H();

	// sprintf(ps2_temp,"%d\n",res);	
	// HAL_UART_Transmit(&huart1, (uint8_t *)ps2_temp, strlen(ps2_temp), 0xFFFF);

	//���ض�������
    return res;	
}

/**
  * @��  ��  PS2��ȡ������ҡ����ֵ��
  * @��  ��  *JoystickStruct �ֱ���ֵ�ṹ��
  * @����ֵ  ��
  */
void AX_PS2_ScanKey(JOYSTICK_TypeDef *JoystickStruct)
{
	uint8_t i;
	
	//ʹ���ֱ�
	CS_L();
	
	//��ȡPS2����
	for(i=0; i<9; i++)
	{
		PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
		HAL_Delay_us(16);
	}
	
	//�ر�ʹ��
	CS_H();

	//��ֵ����
	JoystickStruct->mode = PS2_data[1];
	JoystickStruct->btn1 = ~PS2_data[3];
	JoystickStruct->btn2 = ~PS2_data[4];
	JoystickStruct->RJoy_LR = PS2_data[5];
	JoystickStruct->RJoy_UD = PS2_data[6];
	JoystickStruct->LJoy_LR = PS2_data[7];
	JoystickStruct->LJoy_UD = PS2_data[8];

	// for(i=0; i<9; i++)
	// {
	// 	sprintf(ps2_temp,"i[%d]=%d\n",i,PS2_data[i]);	
	// 	HAL_UART_Transmit(&huart1, (uint8_t *)ps2_temp, strlen(ps2_temp), 0xFFFF);
	// 	HAL_Delay_us(100);
	// }

	// for(i=0; i<9; i++)
	// {
	// 	sprintf(ps2_temp, 
    //         "MODE: %02X BTN1: %02X BTN2: %02X RJOY_LR: %02X RJOY_UD: %02X LJOY_LR: %02X LJOY_UD: %02X\r\n", 
    //         JoystickStruct->mode,
    //         JoystickStruct->btn1,
    //         JoystickStruct->btn2,
    //         JoystickStruct->RJoy_LR,
    //         JoystickStruct->RJoy_UD,
    //         JoystickStruct->LJoy_LR,
    //         JoystickStruct->LJoy_UD);
    
    // // ͨ�� UART ��������
    // 	HAL_UART_Transmit(&huart1, (uint8_t *)ps2_temp, strlen(ps2_temp), 0xFFFF);
	// 	HAL_Delay_us(100);
	// }
	

}
