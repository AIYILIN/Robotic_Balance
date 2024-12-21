 /**
   ******************************************************************************
   * @file   
   * @brief  
   ******************************************************************************
   * @note

  */

 #ifndef __MI_MOTOR_H
 #define __MI_MOTOR_H



 /* Includes ------------------------------------------------------------------*/
 // #include "System_Config.h"
 #include "FDCAN.h"
 #include "cybergear.h"
 #include "can_bsp.h"
 #include <stdbool.h>

 /* Definitions ---------------------------------------------------------*/
 


	
	
 typedef struct{

	uint64_t MCU_64bit_Id;//64λMCUΨһ��ʶ��
 	uint8_t CAN_ID;	//Ŀ����CAN_ID
 	int Motor_Nbr;	//Ŀ�������
	
 	float Angle;	//��ǰ�Ƕ�[0~65535]��Ӧ(-4��~4��) 
 	float Speed;	//��ǰ���ٶ�[0~65535]��Ӧ(30rad/s~30rad/s)  
 	float Torque;	//��ǰ���� [0~65535]��Ӧ��-12Nm~12Nm��  
 	float Temp;		//��ǰ�¶ȣ�Temp(���϶ȣ�*10
	
 	uint16_t set_current;
 	uint16_t SpeedMode_Enable_Set;
 	uint16_t set_position;
	
 	uint8_t error_code;	//������Ϣ��0��,1�У�
	bool error_is_uncalibrated;//δ�궨 
	bool error_is_hall_fault;//HALL������� 
	bool error_is_mag_fault;//�ű������
	bool error_is_overtemp;//����
	bool error_is_overcurrent;//����
	bool error_is_undervoltage;//Ƿѹ����
	uint8_t mode_status;//���ģʽ״̬ (0: Reset[��λ], 1: Cali[�궨], 2: Motor[����])
	
	//�ɶ�д���������б� (7019-701DΪ���°汾�̼��ɶ�)
	uint8_t run_mode;//0: �˿�ģʽ 1: λ��ģʽ  2: �ٶ�ģʽ  3: ����ģʽ 
	float iq_ref;//����ģʽIqָ�� (-23~23A )
	float spd_ref;//ת��ģʽת��ָ�� (-30~30rad/s )
	float imit_torque;//ת������(0~12Nm)
	float cur_kp;//������Kp (Ĭ��ֵ0.125)
	float cui_ki;//������Ki (Ĭ��ֵ0.0158)

	float spd_kp_medium;//�ٶȵ�Kp (Ĭ��ֵ2)
	float spd_ki_medium;//�ٶȵ�Ki (Ĭ��ֵ0.021)
	float spd_kp_low;//�ٶȵ�Kp (Ĭ��ֵ2)
	float spd_ki_low;//�ٶȵ�Ki (Ĭ��ֵ0.5)


	float cur_filt_gain;//�����˲�ϵ��(0~1.0��Ĭ��ֵ0.1)
	float loc_ref;//λ��ģʽ�Ƕ�ָ�� (��λrad)
	float limit_spd;//λ��ģʽ�ٶ����� (0~30rad/s)
	float limit_cur;//�ٶ�λ��ģʽ��������(0~23A)
	float mechPos;//���ض˼�Ȧ��е�Ƕ�(��λrad)
	float iqf;//iq�˲�ֵ(-23~23A )
	float mechVel;//���ض�ת��(-30~30rad/s )
	float VBUS;//ĸ�ߵ�ѹ (��λ:V)
	int16_t rotation;//Ȧ��




 	float Angle_Bias;

	// uint32_t Id_Frame;
	// uint8_t  Data_Frame[8];
	
	
 }MI_Motor;
 

extern MI_Motor Motor1;
extern MI_Motor Motor2;
extern MI_Motor Motor3;
extern MI_Motor Motor4;
extern MI_Motor Motor5;
extern MI_Motor Motor6;
 

typedef struct 
{
	MI_Motor Motor1;
	MI_Motor Motor2;
	MI_Motor Motor3;
	MI_Motor Motor4;
	MI_Motor Motor5;
	MI_Motor Motor6;

}MotorGroup;

extern MotorGroup Motors;

#define Master_CAN_ID 0x00

#define Communication_Type_GetID 0x00     //��ȡ�豸��ID��64λMCUΨһ��ʶ��
#define Communication_Type_MotionControl 0x01 	//��������������Ϳ���ָ��
#define Communication_Type_MotorRequest 0x02	//���������������������״̬
#define Communication_Type_MotorEnable 0x03	//���ʹ������
#define Communication_Type_MotorStop 0x04	//���ֹͣ����
#define Communication_Type_SetPosZero 0x06	//���õ����е��λ
#define Communication_Type_CanID 0x07	//���ĵ�ǰ���CAN_ID

#define Communication_Type_GetSingleParameter 0x11	//��ȡ��������

#define Communication_Type_Control_Mode 0x12
#define Communication_Type_SetSingleParameter 0x12	//�趨��������

#define Communication_Type_ErrorFeedback 0x15	//���Ϸ���֡

#define Run_mode 0x7005	
#define Iq_Ref 0x7006
#define Spd_Ref 0x700A
#define Limit_Torque 0x700B
#define Cur_Kp 0x7010
#define Cur_Ki 0x7011

#define Spd_Kp 0x2014
#define Spd_Ki 0x2015

#define Cur_Filt_Gain 0x7014
#define Loc_Ref 0x7016
#define Limit_Spd 0x7017
#define Limit_Cur 0x7018

#define Gain_Angle 720/32767.0
#define Bias_Angle 0x8000
#define Gain_Speed 30/32767.0
#define Bias_Speed 0x8000
#define Gain_Torque 12/32767.0
#define Bias_Torque 0x8000
#define Temp_Gain 0.1

#define MAX_P 720
#define MIN_P -720
#define MAX_S 30
#define MIN_S -30
#define MAX_T 12
#define MIN_T -12

#define Motor_Error 0x00
#define Motor_OK 0X01

#define Current_mode 3
#define Position_mode 1
#define Motion_mode 0
#define Speed_mode 2
 /* Private variables ---------------------------------------------------------*/
 
 /* Global variables ---------------------------------------------------------*/

 /*Functions------------------------------------------------------------------*/
extern void Init_Motor(MotorGroup *motors,uint8_t Can_Id,int Motor_Num,float mode);
extern void Init_Motor_All(void);
extern void Enable_Motor(MotorGroup *motors,uint8_t motor_num);
extern void Stop_Motor(MotorGroup *motors,uint8_t motor_num,uint8_t clear_error);
extern void Motor_Data_Handler(MotorGroup *motors,uint32_t IDFrame,uint8_t DataFrame[8]);
// extern void Motor_Data_Get(MotorGroup *motors,FDCAN_RxHeaderTypeDef *header ,uint8_t *DataFrame);
extern uint8_t Get_Motor_ID(uint32_t CAN_ID_Frame);
extern uint8_t GetMotorFeedbackType(uint32_t can_id_frame);
extern uint64_t get_motor_MCU_64bit_Id(uint8_t *DataFrame);
extern float uint16_to_float(uint16_t x,float x_min,float x_max,int bits);
extern void Set_Mode(MotorGroup *motors,char motor_num,float Mode);
extern void CurrentMode_Enable_Set(MotorGroup *motors,uint8_t motor_num,float iq_ref);
extern void SpeedMode_Enable_Set(MotorGroup *motors,uint8_t motor_num,float limit_cur,float spd_ref);
extern void PositionMode_Enable_Set(MotorGroup *motors,uint8_t motor_num,float limit_speed,float pos_ref);
extern void Set_Motor_Parameter(MotorGroup *motors ,uint8_t motor_num ,uint16_t Index ,float Value ,char Value_type);
extern void Set_ZeroPos(MotorGroup *motors,uint8_t motor_num);
extern void Set_CANID(MotorGroup *motors,uint8_t motor_num,uint8_t can_id);
extern void Speed_gradient_test(MotorGroup *motors,uint8_t motor_num,float max_speed,float gradient);
 /* Exported function declarations --------------------------------------------*/

#endif
