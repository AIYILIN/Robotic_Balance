#include "rgb_task.h"
#include "main.h"
#include "ws2812.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Header_RgbTask_Entry */
/**
* @brief Function implementing the RgbTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RgbTask_Entry */

void RgbTask_Entry(void const * argument)
{
  /* USER CODE BEGIN RgbTask_Entry */
  /* Infinite loop */
  for(;;)
  {
    // ��ʼ������ĳ�ʼֵ
  uint8_t red = 0, green = 0, blue = 0;

  WS2812_Ctrl(0, 0, 0);
	osDelay(200);

  /* Infinite loop */
  for(;;)
  {
    // �Ӻ�ɫ���䵽��ɫ
    for (red = 0; red < RGB_LIGHT_LIMIT; red++) {
      WS2812_Ctrl(red, 0, 0);  // ��ɫ����
      osDelay(RGB_DELAY_TIME);  // ���ƽ����ٶ�
    }

    // �Ӻ�ɫ���䵽��ɫ
    for (green = 0; green < RGB_LIGHT_LIMIT; green++) {
      WS2812_Ctrl(RGB_LIGHT_LIMIT-green, green, 0);  // ��ɫ����
      osDelay(RGB_DELAY_TIME);
    }

    // ����ɫ���䵽��ɫ
    for (blue = 0; blue < RGB_LIGHT_LIMIT; blue++) {
      WS2812_Ctrl(0, RGB_LIGHT_LIMIT-blue, blue);  // ��ɫ����
      osDelay(RGB_DELAY_TIME);
    }

    // ����ɫ����غ�ɫ
    for (blue = RGB_LIGHT_LIMIT; blue > 0; blue--) {
      WS2812_Ctrl(0, 0, blue);  // ��ɫ����
      osDelay(RGB_DELAY_TIME);
    }
  }
  }
  /* USER CODE END RgbTask_Entry */
}