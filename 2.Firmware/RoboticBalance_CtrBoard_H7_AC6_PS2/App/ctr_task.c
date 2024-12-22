
#include "ctr_task.h"
#include "cmsis_os.h"
#include "ws2812.h"
#include "fdcan.h"
#include "can_bsp.h"
#include "string.h"
#include "can_bsp.h"
#include "cybergear.h"
#include "motor_pid.h"
#include "bsp_power_enable.h"
#include "imu_task.h"
#include "user_lib.h"
#include "key_task.h"
#include "bsp_ps2.h"


float pid_speed_out_left = 0;
float pid_speed_out_right = 0;

float pid_balance_out_left = 0;
float pid_balance_out_right = 0;

float pid_yaw_out = 0; 

float pid_left_motor_out = 0; 
float pid_right_motor_out = 0;

motor_ctrl_t motor_ctrl;


void CtrTask_Entry(void const * argument)
{
  motor_ctrl_para_init(&motor_ctrl);
  Power_Enable(); 
	osDelay(500);

  PID_struct_init(&pid_speed,POSITION_PID,50,0,0.8,0,0); 
  PID_struct_init(&pid_pit,POSITION_PID,30,0,2,0,0);//�����Ŵ�10000����������� 
  PID_struct_init(&pid_yaw,POSITION_PID,30,0,0.5,0,0); 
  
	
  for(;;)
  {
    motor_ctrl_para_set(&motor_ctrl,&my_ps2);

    pid_speed_out_left   = pid_calc(&pid_speed, Motors.Motor3.Speed, -motor_ctrl.forward_speed);//3->left,1->right ������ȷ��
    pid_speed_out_right   = pid_calc(&pid_speed, -Motors.Motor1.Speed, -motor_ctrl.forward_speed);//3->left,1->right ������ȷ��

    pid_balance_out_left = pid_calc(&pid_pit, -imuAngle[INS_PITCH_ADDRESS_OFFSET], pid_speed_out_left);
    pid_balance_out_right = pid_calc(&pid_pit, -imuAngle[INS_PITCH_ADDRESS_OFFSET], pid_speed_out_right);

    pid_yaw_out     = pid_calc(&pid_yaw, imuAngle[INS_YAW_NORMALIZE], -motor_ctrl.yaw_zero_angel); 

    pid_left_motor_out  = (pid_balance_out_left + pid_yaw_out); 
    pid_right_motor_out = (-pid_balance_out_right + pid_yaw_out); 

    osDelay(1);
	}
    /* USER CODE END CtrTask_Entry */
}


void motor_ctrl_para_init(motor_ctrl_t *motor_ctrl)
{
  motor_ctrl->forward_speed = 0;
  motor_ctrl->yaw_zero_angel = 0;
  motor_ctrl->mechanism_zero_angel = 0;
  motor_ctrl->motor_start_flag = 0;//1:���� 0:ֹͣ
  motor_ctrl->motor_safe_flag = 1;//1:��ȫ 0:����ȫ
}

void motor_ctrl_para_set(motor_ctrl_t *motor_ctrl,JOYSTICK_TypeDef *ps2)
{
  if (ps2->mode == 0x73)
  {
    motor_ctrl->forward_speed = (ps2->LJoy_UD_0/127.0)*15;
    motor_ctrl->yaw_zero_angel += (ps2->RJoy_LR_0/1270.0);
    motor_ctrl->mechanism_zero_angel = 0;
  }
  else if(ps2->mode == 0x41) 
  {
    motor_ctrl_para_init(motor_ctrl);
  }
}