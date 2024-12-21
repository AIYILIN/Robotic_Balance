
#ifndef __BSP_PS2_H
#define __BSP_PS2_H

#include "stdint.h"
#include "main.h"
#include "cmsis_os.h"

typedef struct __KEY_T
{
    /* 4个按键 1按下 */
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;

} key_t;




extern key_t key_line;


extern void key_line_init(key_t *key);
extern void key_line_scan(key_t *key);


 #endif
