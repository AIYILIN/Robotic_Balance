#include "key_task.h"
#include "adc.h"
#include "cmsis_os.h"
#include "vofa.h"
#include "bsp_user_key.h"
#include "bsp_buzzer.h"
#include "cybergear.h"
#include "ws2812.h"
#include "bsp_power_enable.h"
#include "ctr_task.h"
#include "bsp_ps2.h"

float vbus = 0;
volatile uint16_t adc_val[2];
uint8_t key_status = 0;

void KeyTask_Entry(void const * argument)
{
    /* USER CODE BEGIN KeyTask_Entry */
    
    BSP_Buzzer_Init();
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_val,2);
    osDelay(500);
    
    /* Infinite loop */
    for(;;)
    {
        if (BSP_UserKey_Detect() == BUTTON_PRESSED)
        {   
            motor_ctrl.motor_start_flag =! motor_ctrl.motor_start_flag;
            motor_ctrl.motor_safe_flag = 1;
            // BSP_Buzzer_Toggle();
        }
        vbus = (adc_val[0]*3.3f/65535)*11.0f;

        PS2_ScanKey(&my_ps2,0);

        osDelay(10);
    }
    /* USER CODE END KeyTask_Entry */
}



