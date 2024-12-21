
#ifndef __BSP_PS2_H
#define __BSP_PS2_H

#include "stdint.h"
#include "main.h"
#include "cmsis_os.h"


#define DI()     HAL_GPIO_ReadPin(PS2_DATA_GPIO_Port,PS2_DATA_Pin)

#define CMD_H()   HAL_GPIO_WritePin(PS2_CMD_GPIO_Port,PS2_CMD_Pin,1)
#define CMD_L()   HAL_GPIO_WritePin(PS2_CMD_GPIO_Port,PS2_CMD_Pin,0)

#define CS_H()   HAL_GPIO_WritePin(PS2_CS_GPIO_Port,PS2_CS_Pin,1)
#define CS_L()   HAL_GPIO_WritePin(PS2_CS_GPIO_Port,PS2_CS_Pin,0)

#define CLK_H()  HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,1)
#define CLK_L()  HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,0)

//PS2�ֱ���ֵ���ݽṹ��	 
typedef struct			 				
{
  uint8_t mode;		    /* �ֱ��Ĺ���ģʽ */
  uint8_t btn1;         /* B0:SLCT B3:STRT B4:UP B5:R B6:DOWN B7:L   */
  uint8_t btn2;         /* B0:L2 B1:R2 B2:L1 B3:R1 B4:/\ B5:O B6:X B7:�� */
  uint8_t RJoy_LR;      /* �ұ�ҡ��  0x00 = ��    0xff = ��   */
  uint8_t RJoy_UD;      /* �ұ�ҡ��  0x00 = ��    0xff = ��   */
  uint8_t LJoy_LR;      /* ���ҡ��  0x00 = ��    0xff = ��   */
  uint8_t LJoy_UD;      /* ���ҡ��  0x00 = ��    0xff = ��   */

	uint8_t L1;
	uint8_t L2;
	uint8_t R1;
	uint8_t R2;

  uint8_t up;
  uint8_t down;
  uint8_t left;
  uint8_t right;

	uint8_t triangle;
  uint8_t rectangle;
  uint8_t circle;
  uint8_t fork;

  uint8_t select;
  uint8_t joyL;
  uint8_t joyR;
  uint8_t start;

  int LJoy_UD_0; 
  int LJoy_LR_0;
  int RJoy_UD_0;
  int RJoy_LR_0;     

}JOYSTICK_TypeDef;


/*** PS2�����ֱ��������� **********/
extern void PS2_ScanKey(JOYSTICK_TypeDef* JoystickStruct,uint8_t transmit_flag);//PS2��ȡ������ҡ����ֵ

extern uint8_t PS2_data[9];
extern JOYSTICK_TypeDef my_ps2;

 #endif
