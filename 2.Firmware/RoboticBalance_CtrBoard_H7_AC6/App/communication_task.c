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

  Set_ZeroPos(&Motors,1);

  // Set_CANID(&Motors,2,0x66);
  

  // Set_CANID(&Motors,1,0x66);
  
  /* Infinite loop */
  for(;;)
  {
    
    // Speed_gradient_test(&Motors,1,5,0.001);//�ٶ����Ҳ���
    // CurrentMode_Enable_Set(&Motors,1,0.5);//����ģʽ����
    // PositionMode_Enable_Set(&Motors,1,5,0);
    // osDelay(2000);
    // PositionMode_Enable_Set(&Motors,1,5,2);
    // osDelay(2000);
    // PositionMode_Enable_Set(&Motors,1,5,0);
    // osDelay(2000);
    // PositionMode_Enable_Set(&Motors,1,5,-2);
    // osDelay(2000);
    // SpeedMode_Enable_Set(&Motors,1,5,0.1);

    if(ABS(imuAngle[1]) > 40 || key_status == 0) 
    {
      Stop_Motor(&Motors,1,0);
      Stop_Motor(&Motors,2,0);
    }
    else if(ABS(imuAngle[1]) <= 40 && key_status == 1) 
    {
      SpeedMode_Enable_Set(&Motors,1,5,motor_pid_out[0]);
      osDelay(5);
      SpeedMode_Enable_Set(&Motors,2,5,motor_pid_out[1]);
      osDelay(5);
    }

      

    osDelay(1);
  }
  /* USER CODE END CommunicationTask_Entry */
}
