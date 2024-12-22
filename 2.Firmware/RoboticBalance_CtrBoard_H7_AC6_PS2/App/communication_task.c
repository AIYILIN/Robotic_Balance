#include "communication.h"
#include "cmsis_os.h"
#include "fdcan.h"
#include "can_bsp.h"
#include "cybergear.h"
#include "motor_pid.h"
#include "bsp_user_key.h"
#include "imu_task.h"
#include "key_task.h"
#include "user_lib.h"
#include "ctr_task.h"
#include "bsp_ps2.h"
#include "usart.h"
#include <string.h>
#include "stdio.h"




/* USER CODE BEGIN Header_CommunicationTask_Entry */
/**
* @brief Function implementing the Communication thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CommunicationTask_Entry */

// uint8_t DataFrame[8] = {};

void CommunicationTask_Entry(void const * argument)
{
  /* USER CODE BEGIN CommunicationTask_Entry */
	
	canfd_filter(&hfdcan1);
  Init_Motor_All();
	
  // Set_ZeroPos(&Motors,1);
  // Set_CANID(&Motors,2,0x67);

  /* Infinite loop */
  for(;;)
  {
    if(ABS(imuAngle[INS_PITCH_ADDRESS_OFFSET]) > 50 || motor_ctrl.motor_start_flag == 0 || motor_ctrl.motor_safe_flag == 0) 
    {
      Stop_Motor(&Motors,1,0);
			osDelay(1);
      Stop_Motor(&Motors,3,0);
			osDelay(1);
      motor_ctrl.motor_safe_flag = 0;
    }
    else if(ABS(imuAngle[INS_PITCH_ADDRESS_OFFSET]) <= 50 && motor_ctrl.motor_start_flag == 1 && motor_ctrl.motor_safe_flag == 1) 
    {
      SpeedMode_Enable_Set(&Motors,1,5,pid_right_motor_out);
      osDelay(1);
      SpeedMode_Enable_Set(&Motors,3,5,pid_left_motor_out);
      osDelay(1);
    }
    osDelay(5);
  }
  /* USER CODE END CommunicationTask_Entry */
}
