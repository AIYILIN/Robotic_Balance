#ifndef __CAN_BSP_H__
#define __CAN_BSP_H__

#include "main.h"
#include "fdcan.h"

/**
************************************************************************
* @brief:      	FDCAN�˲������ýṹ��
* @details:    	����FDCAN�˲�����������Ϣ�������˲���ID�����͵�
************************************************************************
**/
extern FDCAN_FilterTypeDef sFilterConfig;

/**
************************************************************************
* @brief:      	FDCAN����ͷ���ṹ��
* @details:    	����������Ϣ��ͷ����Ϣ�������ʶ�������ݳ��ȵ�
************************************************************************
**/
extern FDCAN_TxHeaderTypeDef TxHeader;

/**
************************************************************************
* @brief:      	FDCAN����ͷ���ṹ��
* @details:    	����������Ϣ��ͷ����Ϣ�������ʶ�������ݳ��ȵ�
************************************************************************
**/
extern FDCAN_RxHeaderTypeDef FDCan1_RxHeader;
extern FDCAN_RxHeaderTypeDef FDCan2_RxHeader;
extern FDCAN_RxHeaderTypeDef FDCan3_RxHeader;

/**
************************************************************************
* @brief:      	FDCAN�������ݻ�����
* @details:    	�洢���յ�������
************************************************************************
**/
extern uint8_t FDCAN1_Rx_Data[8];
extern uint8_t FDCAN2_Rx_Data[8];
extern uint8_t FDCAN3_Rx_Data[8];

/**
************************************************************************
* @brief:      	FDCAN1������Ϣ
* @param:       msg�������͵�����
* @param:       len�����ݳ���
* @param:       CAN_ID��FDCAN��Ϣ��ʶ��
* @retval:     	0���ɹ���1��ʧ��
* @details:    	��FDCAN1������Ϣ
************************************************************************
**/
uint8_t FDCAN1_Send_Msg(uint8_t* msg, uint32_t len, uint32_t CAN_ID);

/**
************************************************************************
* @brief:      	����FDCAN�˲���
* @param:       fdcanhandle��FDCAN���
* @retval:     	void
* @details:    	��ʼ��������FDCAN�˲���
************************************************************************
**/
void canfd_filter(FDCAN_HandleTypeDef* fdcanhandle);

/**
************************************************************************
* @brief:      	FDCAN������Ϣ
* @param:       buf���������ݻ�����
* @retval:     	���յ����ݳ���
* @details:    	��FDCAN������Ϣ���洢��buf��
************************************************************************
**/
uint8_t FDCAN1_Receive_Msg(FDCAN_RxHeaderTypeDef *header,uint8_t *data);
uint8_t FDCAN2_Receive_Msg(FDCAN_RxHeaderTypeDef *header,uint8_t *data);
uint8_t FDCAN3_Receive_Msg(FDCAN_RxHeaderTypeDef *header,uint8_t *data);

/**
************************************************************************
* @brief:      	HAL_FDCAN_RxFifo0Callback
* @param:       hfdcan��FDCAN���
* @param:       RxFifo0ITs���жϱ�־λ
* @retval:     	void
* @details:    	HAL���FDCAN�жϻص���������FDCAN���յ�����Ϣʱ������
************************************************************************
**/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

/**
************************************************************************
* @brief:      	fdcan_rx_callback
* @param:      	void
* @retval:     	void
* @details:    	���û����õ�FDCAN�������ݻص�����
************************************************************************
**/
void fdcan1_rx_callback(void);
void fdcan2_rx_callback(void);
void fdcan3_rx_callback(void);

#endif /* __CAN_BSP_H__ */
