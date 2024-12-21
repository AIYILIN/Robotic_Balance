
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


float pid_speed_out;
float pid_balance_out = 0; 
float pid_yaw_out = 0; 

float pid_left_motor_out = 0; 
float pid_right_motor_out = 0;

motor_ctrl_t motor_ctrl;


void CtrTask_Entry(void const * argument)
{
  motor_ctrl_para_init(&motor_ctrl);
  Power_Enable(); 
	osDelay(500);

  PID_struct_init(&pid_speed,DELTA_PID,50,5,1,0.1,5); 
  PID_struct_init(&pid_pit,DELTA_PID,10000,0,3,0,1);//参数放大到10000倍，方便调整 
  PID_struct_init(&pid_yaw,DELTA_PID,10000,0,0.6,0,4); 
  
	
  for(;;)
  {
    motor_ctrl_para_set(&motor_ctrl,&my_ps2);

    pid_speed_out   = pid_calc(&pid_speed, Motors.Motor3.Speed - Motors.Motor1.Speed, motor_ctrl.forward_speed);//3->left,1->right 正负不确定
    pid_balance_out = pid_calc(&pid_pit, imuAngle[1], pid_speed_out);
    pid_yaw_out     = pid_calc(&pid_yaw, imuAngle[0], motor_ctrl.yaw_zero_angel); 

    pid_left_motor_out  = (-pid_balance_out - pid_yaw_out)/2000.0; 
    pid_right_motor_out = (-pid_balance_out + pid_yaw_out)/2000.0; 

    osDelay(5);
	}
    /* USER CODE END CtrTask_Entry */
}


void motor_ctrl_para_init(motor_ctrl_t *motor_ctrl)
{
  motor_ctrl->forward_speed = 0;
  motor_ctrl->yaw_zero_angel = 0;
  motor_ctrl->mechanism_zero_angel = 0;
}

void motor_ctrl_para_set(motor_ctrl_t *motor_ctrl,JOYSTICK_TypeDef *ps2)
{
  if (ps2->mode == 0x73)
  {
    motor_ctrl->forward_speed = ps2->LJoy_UD_0/127.0;
    motor_ctrl->yaw_zero_angel = ps2->RJoy_LR_0/127.0;
    motor_ctrl->mechanism_zero_angel = 0;
  }
  else if(ps2->mode == 0x41) 
  {
    motor_ctrl_para_init(motor_ctrl);
  }
}