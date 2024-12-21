#ifndef __CTR_TASK_H__
#define __CTR_TASK_H__


#include "motor_pid.h"
#include "bsp_ps2.h"

typedef struct __MOTOR_CTRL_T
{
    float mechanism_zero_angel;
    float yaw_zero_angel;
    float forward_speed;
    
} motor_ctrl_t;

extern motor_ctrl_t motor_ctrl;

extern float pid_left_motor_out; 
extern float pid_right_motor_out;

extern void motor_ctrl_para_init(motor_ctrl_t *motor_ctrl);
extern void motor_ctrl_para_set(motor_ctrl_t *motor_ctrl,JOYSTICK_TypeDef *ps2);

#endif
