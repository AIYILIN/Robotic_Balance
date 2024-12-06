
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


float motor_pid_out[2];

void CtrTask_Entry(void const * argument)
{

  PID_struct_init(&pid_pit,DELTA_PID,5,5,0.55,0.12,3); 
	
  for(;;)
  {
    float pid_out_temp ;
    pid_out_temp = pid_calc(&pid_pit,imuAngle[1],0);
    osDelay(5);
    motor_pid_out[0] = pid_out_temp;
    motor_pid_out[1] = -pid_out_temp;  


    osDelay(1);
	}
    /* USER CODE END CtrTask_Entry */
}




