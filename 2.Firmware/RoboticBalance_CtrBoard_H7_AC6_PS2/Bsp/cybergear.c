/**
 ******************************************************************************
* @file   
* @brief  
******************************************************************************
* @note

*/

/* Includes ------------------------------------------------------------------*/
#include "cybergear.h"
#include "can_bsp.h"
#include "cmsis_os.h"
#include "string.h"
/* Definitions ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t byte[4];
MI_Motor Motor1;
MI_Motor Motor2;
MI_Motor Motor3;
MI_Motor Motor4;
MI_Motor Motor5;
MI_Motor Motor6;

MotorGroup Motors;


/* Global variables ---------------------------------------------------------*/

/*Functions------------------------------------------------------------------*/

/*******************************************************************************
 * @function     : ������ת4�ֽں���
 * @param        : ������
 * @return       : 4�ֽ�����
 * @description  : IEEE 754 Э��
 *******************************************************************************/
uint8_t* Float_to_Byte(float f)
{
	unsigned long longdata = 0;
	longdata = *(unsigned long*)&f;       
	byte[0] = (longdata & 0xFF000000) >> 24;
	byte[1] = (longdata & 0x00FF0000) >> 16;
	byte[2] = (longdata & 0x0000FF00) >> 8;
	byte[3] = (longdata & 0x000000FF);
	return byte;
}

/*******************************************************************************
 * @function     : С�׵������16λ����ת����
 * @param        : 1. 16λ���� 2.��Ӧ�������� 3.��Ӧ�������� 4. ����λ��
 * @return       : ������Ӧ������
 * @description  : None
 *******************************************************************************/
float uint16_to_float(uint16_t x,float x_min,float x_max,int bits){
	uint32_t span = (1 << bits) - 1;
	float offset = x_max - x_min;
	return offset * x / span + x_min;
}

/*******************************************************************************
 * @function     : ���������ʼ��
 * @param        : 1. ����ṹ�� 2.���CANID 3.������ 4.�������ģʽ��1.�˿�ģʽ 2. λ��ģʽ 3. �ٶ�ģʽ 4. ����ģʽ��
 * @return       : None
 * @description  : �����ʼ����� CANID ������ ����Ĺ���ģʽ
 *******************************************************************************/
void Init_Motor(MotorGroup *motors ,uint8_t Can_Id ,int motor_num,float mode)
{
	if(motor_num>=1 && motor_num<=6)
	{
		MI_Motor *motor = NULL;

		if(motor_num==1) motor = &motors->Motor1;
		if(motor_num==2) motor = &motors->Motor2;
		if(motor_num==3) motor = &motors->Motor3;
		if(motor_num==4) motor = &motors->Motor4;
		if(motor_num==5) motor = &motors->Motor5;
		if(motor_num==6) motor = &motors->Motor6;

		motor->CAN_ID=Can_Id;
		motor->Motor_Nbr=motor_num;

		Set_Mode(motors,motor_num,mode);
		Enable_Motor(motors,motor_num);

		Set_Motor_Parameter(motors,motor_num,Cur_Kp,0.125,'f');
		Set_Motor_Parameter(motors,motor_num,Cur_Ki,0.0158,'f');
		Set_Motor_Parameter(motors,motor_num,Cur_Filt_Gain,0.1,'f');//�����˲�ϵ��

		// Set_Motor_Parameter(motors,motor_num,Spd_Kp,2,'f');		//spd_kp
		// Set_Motor_Parameter(motors,motor_num,Spd_Kp,0.5,'f');	//spd_ki
		// Set_Motor_Parameter(motors,motor_num,Spd_Ref,0,'f');	//�����ٶ�ֵ-30 -> 30 rad
	}
}

void Init_Motor_All(void)
{
	Init_Motor(&Motors,101,1,Speed_mode);
	Init_Motor(&Motors,102,2,Speed_mode);
	Init_Motor(&Motors,103,3,Speed_mode);
	Init_Motor(&Motors,104,4,Speed_mode);
	Init_Motor(&Motors,105,5,Speed_mode);
	Init_Motor(&Motors,106,6,Speed_mode);


}
	

/*******************************************************************************
 * @function     : ��ȡ����ظ�֡��չID�еĵ��CANID
 * @param        : ����ظ�֡�е���չCANID
 * @return       : ���ID
 * @description  : ���Ϸ���֡��ͨ������21��ȡ0-7λ����������ȡ��չID��8-15λ,
 *******************************************************************************/
uint8_t Get_Motor_ID(uint32_t CAN_ID_Frame)
{
	if (((CAN_ID_Frame>>24)&0x000000ff) == Communication_Type_ErrorFeedback)	//���Ϸ���֡��ͨ������21��
	{
		return (CAN_ID_Frame)&0x000000ff;
	}
	
	return (CAN_ID_Frame>>8)&0x000000ff;
}
/*******************************************************************************
 * @function     : ʹ�ܵ��
 * @param        : ��Ӧ���Ƶ���ṹ��
 * @return       : None
 * @description  : ʹ�ܵ��
 *******************************************************************************/
void Enable_Motor(MotorGroup *motors,uint8_t motor_num)
{
	MI_Motor *motor = NULL;
	
	if(motor_num==1) motor = &motors->Motor1;
	if(motor_num==2) motor = &motors->Motor2;
	if(motor_num==3) motor = &motors->Motor3;
	if(motor_num==4) motor = &motors->Motor4;
	if(motor_num==5) motor = &motors->Motor5;
	if(motor_num==6) motor = &motors->Motor6;

	uint32_t Send_ID;
	uint8_t temp[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	Send_ID = Communication_Type_MotorEnable<<24|Master_CAN_ID<<8|motor->CAN_ID;
	FDCAN1_Send_Msg(temp,FDCAN_DLC_BYTES_8,Send_ID);
}

/*******************************************************************************
 * @function     : ֹͣ���
 * @param        : 1.��Ӧ���Ƶ���ṹ�� 2.�������λ��0 ����� 1�����
 * @return       : None
 * @description  : ʹ�ܵ��
 *******************************************************************************/
void Stop_Motor(MotorGroup *motors,uint8_t motor_num,uint8_t clear_error)
{
	MI_Motor *motor = NULL;
	
	if(motor_num==1) motor = &motors->Motor1;
	if(motor_num==2) motor = &motors->Motor2;
	if(motor_num==3) motor = &motors->Motor3;
	if(motor_num==4) motor = &motors->Motor4;
	if(motor_num==5) motor = &motors->Motor5;
	if(motor_num==6) motor = &motors->Motor6;

	uint32_t Send_ID;
	uint8_t temp[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	temp[0]=clear_error;
	Send_ID = Communication_Type_MotorStop<<24|Master_CAN_ID<<8|motor->CAN_ID;
	FDCAN1_Send_Msg(temp,FDCAN_DLC_BYTES_8,Send_ID);
}

/*******************************************************************************
 * @function     : ����ظ�֡���ݴ�����
 * @param        : 1.��Ӧ���Ƶ���ṹ�� 2.��չID֡ 3.����֡
 * @return       : None
 * @description  : ��ȡ�Ƕ� �ٶ� Ť�� �¶� �������ȵ�
 *******************************************************************************/
void Motor_Data_Handler(MotorGroup *motors,uint32_t IDFrame,uint8_t DataFrame[8])
{
	MI_Motor *motor_temp = NULL;

	switch (GetMotorFeedbackType(IDFrame))
	{
		case  Communication_Type_GetID ://0x00��ȡ�豸��ID��64λMCUΨһ��ʶ��
			{
				if(motors->Motor1.CAN_ID == Get_Motor_ID(IDFrame))	motors->Motor1.MCU_64bit_Id = get_motor_MCU_64bit_Id(DataFrame);
				if(motors->Motor2.CAN_ID == Get_Motor_ID(IDFrame))	motors->Motor2.MCU_64bit_Id = get_motor_MCU_64bit_Id(DataFrame);
				if(motors->Motor3.CAN_ID == Get_Motor_ID(IDFrame))	motors->Motor3.MCU_64bit_Id = get_motor_MCU_64bit_Id(DataFrame);
				if(motors->Motor4.CAN_ID == Get_Motor_ID(IDFrame))	motors->Motor4.MCU_64bit_Id = get_motor_MCU_64bit_Id(DataFrame);
				if(motors->Motor5.CAN_ID == Get_Motor_ID(IDFrame))	motors->Motor5.MCU_64bit_Id = get_motor_MCU_64bit_Id(DataFrame);
				if(motors->Motor6.CAN_ID == Get_Motor_ID(IDFrame))	motors->Motor6.MCU_64bit_Id = get_motor_MCU_64bit_Id(DataFrame);
			}
			break;
		case  Communication_Type_MotorRequest ://0x02���������������������״̬
			{
				motor_temp = NULL;

				if(motors->Motor1.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor1;
				if(motors->Motor2.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor2;
				if(motors->Motor3.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor3;
				if(motors->Motor4.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor4;
				if(motors->Motor5.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor5;
				if(motors->Motor6.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor6;

				motor_temp->error_code=(IDFrame&0x1F0000)>>16;// ����������Ϣ��bit21~16��
				// ���ֹ������͵ı�־
				motor_temp->error_is_uncalibrated = (motor_temp->error_code & (1 << 21)) != 0;
				motor_temp->error_is_hall_fault =	 (motor_temp->error_code & (1 << 20)) != 0;
				motor_temp->error_is_mag_fault = 	(motor_temp->error_code & (1 << 19)) != 0;
				motor_temp->error_is_overtemp = 	(motor_temp->error_code & (1 << 18)) != 0;
				motor_temp->error_is_overcurrent = (motor_temp->error_code & (1 << 17)) != 0;
				motor_temp->error_is_undervoltage = (motor_temp->error_code & (1 << 16)) != 0;

				// ����ģʽ״̬��bit22~23��
				motor_temp->mode_status = (IDFrame & 0x00C00000) >> 22;

				// �����Ƕ� �ٶ� Ť�� �¶ȣ�data-> Byte0~Byte7 ��
				motor_temp->Angle=uint16_to_float(DataFrame[0]<<8|DataFrame[1],MIN_P,MAX_P,16);		//Byte0~1:��ǰ�Ƕ�[0~65535]��Ӧ(-4��~4��) 
				motor_temp->Speed=uint16_to_float(DataFrame[2]<<8|DataFrame[3],MIN_S,MAX_S,16);		//Byte2~3:��ǰ���ٶ�[0~65535]��Ӧ(30rad/s~30rad/s)  
				motor_temp->Torque=uint16_to_float(DataFrame[4]<<8|DataFrame[5],MIN_T,MAX_T,16);	//Byte4~5:��ǰ���� [0~65535]��Ӧ��-12Nm~12Nm��  				
				motor_temp->Temp=(DataFrame[6]<<8|DataFrame[7])*Temp_Gain;							//Byte6~7:��ǰ�¶ȣ�Temp(���϶ȣ�*10	
				
			}

			break;
		case  Communication_Type_GetSingleParameter ://0x11��ȡ��������
			{
				motor_temp = NULL;

				if(motors->Motor1.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor1;
				if(motors->Motor2.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor2;
				if(motors->Motor3.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor3;
				if(motors->Motor4.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor4;
				if(motors->Motor5.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor5;
				if(motors->Motor6.CAN_ID == Get_Motor_ID(IDFrame))	motor_temp = &motors->Motor6;

				if((DataFrame[0]==0x05)&&(DataFrame[1]==0x70)&&(DataFrame[2]==0x00)&&(DataFrame[3]==0x00))
				motor_temp->run_mode = DataFrame[7];//ò�ƶ���
				/*
				���ﻹ�кö�Ҫд��
				*/
			}
			break;
		case  Communication_Type_ErrorFeedback ://0x15���Ϸ���֡
			{
				
				
			}
			break;
		default:
			break;
	}


	

}

/*******************************************************************************
 * @function     : ��ȡ��������ķ�������
 * @param        : 1.��չID֡
 * @return       : 1.0x00:��ȡ�豸��ID��64λMCUΨһ��ʶ�� 2.0x02:���������������������״̬ 3.0x11:��ȡ�������� 4.0x15:���Ϸ���֡
 * @description  : ��ȡ��������ķ�������
 *******************************************************************************/
uint8_t GetMotorFeedbackType(uint32_t can_id_frame)
{
	return (can_id_frame>>24)&0x000000ff;
}

/*******************************************************************************
 * @function     : ��ȡ���������MCU_ID
 * @param        : 1.����֡
 * @return       : 1.MCU ID
 * @description  : ��ȡ���������MCU_ID
 *******************************************************************************/
uint64_t get_motor_MCU_64bit_Id(uint8_t *DataFrame)
{
    return ((uint64_t)DataFrame[7] << 56) | ((uint64_t)DataFrame[6] << 48) | 
           ((uint64_t)DataFrame[5] << 40) | ((uint64_t)DataFrame[4] << 32) |
           ((uint64_t)DataFrame[3] << 24) | ((uint64_t)DataFrame[1] << 16) |
           ((uint64_t)DataFrame[1] << 8)  | (uint64_t)DataFrame[0];
}


/*******************************************************************************
 * @function     : д��������
 * @param        : 1.��Ӧ���Ƶ���ṹ�� 2.д�������Ӧ��ַ 3.д�����ֵ 4.д�������������
 * @return       : None
 * @description  : None
 *******************************************************************************/
void Set_Motor_Parameter(MotorGroup *motors ,uint8_t motor_num ,uint16_t Index ,float Value ,char Value_type)
{
	uint32_t Send_ID;
	uint8_t Send_Data[8];
	if(motor_num==1)Send_ID = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|motors->Motor1.CAN_ID;
	if(motor_num==2)Send_ID = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|motors->Motor2.CAN_ID;
	if(motor_num==3)Send_ID = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|motors->Motor3.CAN_ID;
	if(motor_num==4)Send_ID = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|motors->Motor4.CAN_ID;
	if(motor_num==5)Send_ID = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|motors->Motor5.CAN_ID;
	if(motor_num==6)Send_ID = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|motors->Motor6.CAN_ID;
	Send_Data[0]=Index;
	Send_Data[1]=Index>>8;
	Send_Data[2]=0x00;
	Send_Data[3]=0x00;
	if(Value_type == 'f'){
		Float_to_Byte(Value);
		Send_Data[4]=byte[3];
		Send_Data[5]=byte[2];
		Send_Data[6]=byte[1];
		Send_Data[7]=byte[0];		
	}
	else if(Value_type == 's'){
		Send_Data[4]=(uint8_t)Value;
		Send_Data[5]=0x00;
		Send_Data[6]=0x00;
		Send_Data[7]=0x00;				
	}
	FDCAN1_Send_Msg(Send_Data,FDCAN_DLC_BYTES_8,Send_ID);	
}

/*******************************************************************************
 * @function     : ���õ������ģʽ
 * @param        : %2
 * @return       : %3
 * @description  : %4
 *******************************************************************************/
void Set_Mode(MotorGroup *motors,char motor_num,float Mode)
{
	Set_Motor_Parameter(motors,motor_num,Run_mode,Mode,'s');
}

/*******************************************************************************
 * @function     : ����ģʽ->ʹ��->���õ���ֵ
 * @param        : 1.����ṹ�� 2.������ 3.�趨����ֵ
 * @return       : NULL
 * @description  : ����ģʽ->ʹ��->���õ���ֵ
 *******************************************************************************/
void CurrentMode_Enable_Set(MotorGroup *motors,uint8_t motor_num,float iq_ref)
{
	MI_Motor *motor = NULL;
	
	if(motor_num==1) motor = &motors->Motor1;
	if(motor_num==2) motor = &motors->Motor2;
	if(motor_num==3) motor = &motors->Motor3;
	if(motor_num==4) motor = &motors->Motor4;
	if(motor_num==5) motor = &motors->Motor5;
	if(motor_num==6) motor = &motors->Motor6;

	Set_Mode(motors,motor_num,Current_mode);					//���õ���ģʽ
	Enable_Motor(motors,motor_num);			
	Set_Motor_Parameter(motors,motor_num,Iq_Ref,iq_ref,'f'); //���õ���ֵ-23 -> 23 rad
}

/*******************************************************************************
 * @function     : �ٶ�ģʽ->ʹ��->�����ٶ�ģʽ��������ֵ->�����ٶ�
 * @param        : 1.����ṹ�� 2.������ 3.�趨�ٶ�
 * @return       : NULL
 * @description  : �ٶ�ģʽ->ʹ��->�����ٶ�ģʽ��������ֵ->�����ٶ�
 *******************************************************************************/
void SpeedMode_Enable_Set(MotorGroup *motors,uint8_t motor_num,float limit_cur,float spd_ref)
{
	MI_Motor *motor_temp = NULL;
	
	if(motor_num==1) motor_temp = &motors->Motor1;
	if(motor_num==2) motor_temp = &motors->Motor2;
	if(motor_num==3) motor_temp = &motors->Motor3;
	if(motor_num==4) motor_temp = &motors->Motor4;
	if(motor_num==5) motor_temp = &motors->Motor5;
	if(motor_num==6) motor_temp = &motors->Motor6;

	Set_Mode(motors,motor_num,Speed_mode);					//�����ٶ�ģʽ
	Enable_Motor(motors,motor_num);							//���ʹ��
	Set_Motor_Parameter(motors,motor_num,Limit_Cur,limit_cur,'f');	//�����ٶ�ģʽ��������ֵ
	Set_Motor_Parameter(motors,motor_num,Spd_Ref,spd_ref,'f');	//�����ٶ�ֵ-30 -> 30 rad
	
}


/*******************************************************************************
 * @function     : λ��ģʽ->ʹ��->����λ��ģʽ������ٶ�ֵ->����λ��
 * @param        : 1.����ṹ�� 2.������ 3.�趨λ��
 * @return       : NULL
 * @description  : λ��ģʽ->ʹ��->����λ��
 *******************************************************************************/
void PositionMode_Enable_Set(MotorGroup *motors,uint8_t motor_num,float limit_speed,float pos_ref)
{
	MI_Motor *motor_temp = NULL;
	
	if(motor_num==1) motor_temp = &motors->Motor1;
	if(motor_num==2) motor_temp = &motors->Motor2;
	if(motor_num==3) motor_temp = &motors->Motor3;
	if(motor_num==4) motor_temp = &motors->Motor4;
	if(motor_num==5) motor_temp = &motors->Motor5;
	if(motor_num==6) motor_temp = &motors->Motor6;

	Set_Mode(motors,motor_num,Position_mode);					//����λ��ģʽ
	Enable_Motor(motors,motor_num);							//���ʹ��
	Set_Motor_Parameter(motors,motor_num,Limit_Spd,limit_speed,'f');//����λ��ģʽ������ٶ�ֵ
	Set_Motor_Parameter(motors,motor_num,Loc_Ref,pos_ref,'f');	//����λ�ã�float�ͣ�
	
}


/*******************************************************************************
 * @function     : ���õ����е���
 * @param        : %2
 * @return       : %3
 * @description  : %4
 *******************************************************************************/
void Set_ZeroPos(MotorGroup *motors,uint8_t motor_num)
{
	// SpeedMode_Enable_Set(motors,1,23,0);
	osDelay(1000);

	MI_Motor *motor = NULL;
	
	if(motor_num==1) motor = &motors->Motor1;
	if(motor_num==2) motor = &motors->Motor2;
	if(motor_num==3) motor = &motors->Motor3;
	if(motor_num==4) motor = &motors->Motor4;
	if(motor_num==5) motor = &motors->Motor5;
	if(motor_num==6) motor = &motors->Motor6;

	uint32_t Send_ID;
	uint8_t Send_Data[8]={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	Send_ID = Communication_Type_SetPosZero<<24|Master_CAN_ID<<8|motor->CAN_ID;
	FDCAN1_Send_Msg(Send_Data,FDCAN_DLC_BYTES_8,Send_ID);		
}

/*******************************************************************************
 * @function     : ���õ��CANID
 * @param        : 1.��Ӧ���Ƶ���ṹ�� 2.Ҫ�޸ĵĵ����� 3.Ŀ������Ҫ�޸ĳɵ�CAN_ID
 * @return       : %3
 * @description  : %4
 *******************************************************************************/
void Set_CANID(MotorGroup *motors,uint8_t motor_num,uint8_t can_id)
{

	// SpeedMode_Enable_Set(motors,1,23,0);
	osDelay(1000);


	MI_Motor *motor = NULL;
	
	if(motor_num==1) motor = &motors->Motor1;
	if(motor_num==2) motor = &motors->Motor2;
	if(motor_num==3) motor = &motors->Motor3;
	if(motor_num==4) motor = &motors->Motor4;
	if(motor_num==5) motor = &motors->Motor5;
	if(motor_num==6) motor = &motors->Motor6;

	uint32_t Send_ID;
	uint8_t Send_Data[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	// Send_ID = Communication_Type_CanID<<24|can_id<<16|Master_CAN_ID<<8|motor->CAN_ID;
	// Send_ID = Communication_Type_CanID<<24|can_id<<16|Master_CAN_ID<<8|0x7f;
	FDCAN1_Send_Msg(Send_Data,FDCAN_DLC_BYTES_8,Send_ID);
	osDelay(10);
}

/*******************************************************************************
 * @function     : ����ٶ�ģʽ����
 * @param        : 1.����ٶ� 2.�ٶ������ݶ�
 * @return       : None
 * @description  : �ٶ��ݶ�����/����
 *******************************************************************************/
void Speed_gradient_test(MotorGroup *motors,uint8_t motor_num,float max_speed,float gradient)
{
	
	if(max_speed==0 && gradient==0)
	{
		max_speed = 10;
		gradient = 0.001;
	}
	for (float speed_ref= 0.0f; speed_ref < max_speed; speed_ref+=gradient)
	{
		SpeedMode_Enable_Set(motors,motor_num,23,speed_ref);
		osDelay(1);
		
	}
	for (float speed_ref = max_speed; speed_ref > -max_speed; speed_ref-=gradient)
	{
		SpeedMode_Enable_Set(motors,motor_num,23,speed_ref);
		osDelay(1);
		
	}
	for (float speed_ref = -max_speed; speed_ref < 0; speed_ref+=gradient)
	{
		SpeedMode_Enable_Set(motors,motor_num,23,speed_ref);
		osDelay(1);
	}
}


/* Exported function declarations --------------------------------------------*/


