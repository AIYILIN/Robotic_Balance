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
	osDelay(500);
	canfd_filter(&hfdcan1);
  Init_Motor_All();
	
  // Set_ZeroPos(&Motors,1);
  // Set_CANID(&Motors,2,0x67);

  /* Infinite loop */
  for(;;)
  {
    if(ABS(imuAngle[1]) > 60 || key_status == 0) 
    {
      Stop_Motor(&Motors,1,0);
			osDelay(1);
      Stop_Motor(&Motors,3,0);
			osDelay(1);
    }
    else if(ABS(imuAngle[1]) <= 60 && key_status == 1) 
    {
      CurrentMode_Enable_Set(&Motors,1,pid_right_motor_out);
      osDelay(1);
      CurrentMode_Enable_Set(&Motors,3,pid_left_motor_out);
      osDelay(1);
    // Speed_gradient_test(&Motors,3,5,0.001);//速度正弦测试
    // CurrentMode_Enable_Set(&Motors,1,0.5);//电流模式测试
    // PositionMode_Enable_Set(&Motors,1,5,0);
    }

    PS2_ScanKey(&my_ps2,0);

    osDelay(5);
  }
  /* USER CODE END CommunicationTask_Entry */
}
