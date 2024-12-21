#include "cmsis_os.h"
#include "main.h"
#include "bsp_ps2.h"


key_t key_line;

void key_line_init(key_t *key)
{
    key->up = 0;
    key->down = 0;
    key->left = 0;
    key->right = 0;
}


void key_line_scan(key_t *key)
{
    key->up = !HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
    key->down = !HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin);
    key->left = !HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin);
    key->right = !HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin);
}

