
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

float motor_pid_out[2];

float balance_pid_out = 0; 
float yaw_pid_out = 0; 
float speed_left_pid_out = 0;
float speed_right_pid_out = 0;

float left_motor_pid_out = 0; 
float right_motor_pid_out = 0;

motor_ctrl_t motor_ctrl;
uint8_t turn_flag = 0;

void motor_para_init(motor_ctrl_t *motor_ctrl)
{
  motor_ctrl->forward_speed = 0;
  motor_ctrl->yaw_zero_angel = 0;
  motor_ctrl->mechanism_zero_angel = 6.3;
}

void CtrTask_Entry(void const * argument)
{
  motor_para_init(&motor_ctrl);
  Power_Enable(); 

  PID_struct_init(&pid_pit,DELTA_PID,20,0,3,0,1); 
  PID_struct_init(&pid_yaw,DELTA_PID,20,0,0.6,0,4); 
  PID_struct_init(&pid_spd[0],DELTA_PID,20,5,1,0.1,5); //left
  PID_struct_init(&pid_spd[1],DELTA_PID,20,5,1,0.1,5); //right
	
  for(;;)
  {
    if(key_line.up == 1)  
    {
      motor_ctrl.forward_speed += 0.03;
      motor_ctrl.mechanism_zero_angel += 0.1;
    }
    if(key_line.down == 1)  
    {
      motor_ctrl.forward_speed -= 0.03;
      motor_ctrl.mechanism_zero_angel -= 0.1;
    }
    if(key_line.left == 1)  
    {
      if(turn_flag == 0)
      {
        motor_ctrl.yaw_zero_angel -= 0.3;
        if(motor_ctrl.yaw_zero_angel <= -170)turn_flag = 1;
      }
      
      if ( turn_flag == 1)
      {
        motor_ctrl.yaw_zero_angel += 0.3;
        if(motor_ctrl.yaw_zero_angel >= 170)turn_flag = 0;
      }
      
    }
    
    balance_pid_out = pid_calc(&pid_pit,imuAngle[1],motor_ctrl.mechanism_zero_angel);
    yaw_pid_out = pid_calc(&pid_yaw,imuAngle[0],motor_ctrl.yaw_zero_angel); 
    speed_left_pid_out  =  pid_calc(&pid_spd[0],Motors.Motor3.Speed,-motor_ctrl.forward_speed);//left
    speed_right_pid_out =  pid_calc(&pid_spd[1],Motors.Motor1.Speed,motor_ctrl.forward_speed); //right

    left_motor_pid_out  = -balance_pid_out - yaw_pid_out - speed_left_pid_out;
    right_motor_pid_out = -balance_pid_out + yaw_pid_out + speed_right_pid_out;

    motor_pid_out[1] = float_constrain(-left_motor_pid_out,-15,15);//left
    motor_pid_out[0] = float_constrain(right_motor_pid_out,-15,15);//right
    

    osDelay(5);
	}
    /* USER CODE END CtrTask_Entry */
}




    // balance_pid_out = pid_calc(&pid_pit,imuAngle[1],2.3);
    // yaw_pid_out = pid_calc(&pid_yaw,imuAngle[0],0); 

    // // left_motor_pid_out = balance_pid_out - yaw_pid_out ;
    // // right_motor_pid_out = balance_pid_out + yaw_pid_out ;

    // speed_left_pid_out =  pid_calc(&pid_spd[1],Motors.Motor2.Speed,1);//left
    // speed_right_pid_out =  pid_calc(&pid_spd[0],Motors.Motor1.Speed,-1);//rightu


    // osDelay(5);
    // motor_pid_out[0] = balance_pid_out + yaw_pid_out + speed_right_pid_out;   //right
    // motor_pid_out[1] = -balance_pid_out + yaw_pid_out + speed_left_pid_out;   //left

    // osDelay(1);