#ifndef __CTR_TASK_H__
#define __CTR_TASK_H__


#include "motor_pid.h"

typedef struct __MOTOR_CTRL_T
{
    float mechanism_zero_angel;
    float yaw_zero_angel;
    float forward_speed;
    
} motor_ctrl_t;

extern motor_ctrl_t motor_ctrl;
extern float motor_pid_out[2];

extern void motor_para_init(motor_ctrl_t *motor_ctrl);

#endif
