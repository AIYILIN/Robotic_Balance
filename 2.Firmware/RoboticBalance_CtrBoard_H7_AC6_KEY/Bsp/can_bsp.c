#include "can_bsp.h"
#include "cybergear.h"

FDCAN_FilterTypeDef sFilterConfig;
FDCAN_TxHeaderTypeDef TxHeader;

FDCAN_RxHeaderTypeDef FDCan1_RxHeader;
FDCAN_RxHeaderTypeDef FDCan2_RxHeader;
FDCAN_RxHeaderTypeDef FDCan3_RxHeader;

uint8_t FDCAN1_Rx_Data[8] = {0};
uint8_t FDCAN2_Rx_Data[8] = {0};
uint8_t FDCAN3_Rx_Data[8] = {0};

/**
************************************************************************
* @brief:      	canfd_filter(FDCAN_HandleTypeDef* fdcanhandle)
* @param:       fdcanhandle��FDCAN���
* @retval:     	void
* @details:    	����FDCAN�˲�����������FDCAN�ӿ�
************************************************************************
**/
void canfd_filter(FDCAN_HandleTypeDef* fdcanhandle)
{
    sFilterConfig.IdType = FDCAN_EXTENDED_ID;  // ��չID
    sFilterConfig.FilterIndex = 0;  // �˲�������   
    sFilterConfig.FilterType = FDCAN_FILTER_RANGE;  // �˲�������
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;  // �˲���0������FIFO0  
    sFilterConfig.FilterID1 = 0x00000000;
    sFilterConfig.FilterID2 = 0x1FFFFFFF;

    TxHeader.IdType = FDCAN_EXTENDED_ID;  // ��չID
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;  // ����֡
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;  // �ر������л�
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;  // ��ͳ��CANģʽ
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;  // �޷����¼�
    TxHeader.MessageMarker = 0;  // ��Ϣ���

    // ����FDCAN�˲���
    if (HAL_FDCAN_ConfigFilter(fdcanhandle, &sFilterConfig) != HAL_OK)
        HAL_FDCAN_ConfigGlobalFilter(fdcanhandle, FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);  // FDCAN_ACCEPT_IN_RX_FIFO0
    
    HAL_FDCAN_ActivateNotification(fdcanhandle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    HAL_FDCAN_Start(fdcanhandle);  // ����FDCAN
}

/**
************************************************************************
* @brief:      	FDCAN1_Send_Msg(uint8_t* msg, uint32_t len, uint32_t CAN_ID)
* @param:       msg�������͵�����
* @param:       len�����ݳ���
* @param:       CAN_ID��FDCAN��Ϣ��ʶ��
* @retval:     	0���ɹ���1��ʧ��
* @details:    	��FDCAN1������Ϣ
************************************************************************
**/
uint8_t FDCAN1_Send_Msg(uint8_t* msg, uint32_t len, uint32_t CAN_ID)
{
    TxHeader.Identifier = CAN_ID;  // 32λID
    TxHeader.DataLength = len;  // ���ݳ���
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, msg) != HAL_OK) 
    {
        return 1;  // ����ʧ��
    }

    return 0;  // ���ͳɹ�
}

/**
************************************************************************
* @brief:      	FDCAN2_Send_Msg(uint8_t* msg, uint32_t len, uint32_t CAN_ID)
* @param:       msg�������͵�����
* @param:       len�����ݳ���
* @param:       CAN_ID��FDCAN��Ϣ��ʶ��
* @retval:     	0���ɹ���1��ʧ��
* @details:    	��FDCAN2������Ϣ
************************************************************************
**/
uint8_t FDCAN2_Send_Msg(uint8_t* msg, uint32_t len, uint32_t CAN_ID)
{
    TxHeader.Identifier = CAN_ID;  // 32λID
    TxHeader.DataLength = len;  // ���ݳ���
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, msg) != HAL_OK)
    {
        return 1;  // ����ʧ��
    }

    return 0;  // ���ͳɹ�
}

/**
************************************************************************
* @brief:      	FDCAN1_Receive_Msg(uint8_t *buf)
* @param:       1.header:��������λǰ��ı�ʶ�� 2.buf���������ݻ���
* @retval:     	���յ����ݳ���
* @details:    	��FDCAN1��������
************************************************************************
**/
uint8_t FDCAN1_Receive_Msg(FDCAN_RxHeaderTypeDef *header,uint8_t *data)
{
    if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, header, data) != HAL_OK)
        return 0;  // ����ʧ��
    return header->DataLength >> 16;  // �������ݳ���
}

/**
************************************************************************
* @brief:      	FDCAN2_Receive_Msg(uint8_t *buf)
* @param:       1.header:��������λǰ��ı�ʶ�� 2.buf���������ݻ���
* @retval:     	���յ����ݳ���
* @details:    	��FDCAN2��������
************************************************************************
**/
uint8_t FDCAN2_Receive_Msg(FDCAN_RxHeaderTypeDef *header,uint8_t *data)
{
    if (HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, header, data) != HAL_OK)
        return 0;  // ����ʧ��
    return header->DataLength >> 16;  // �������ݳ���
}

/**
************************************************************************
* @brief:      	FDCAN3_Receive_Msg(uint8_t *buf)
* @param:       1.header:��������λǰ��ı�ʶ�� 2.buf���������ݻ���
* @retval:     	���յ����ݳ���
* @details:    	��FDCAN3��������
************************************************************************
**/
uint8_t FDCAN3_Receive_Msg(FDCAN_RxHeaderTypeDef *header,uint8_t *data)
{
    if (HAL_FDCAN_GetRxMessage(&hfdcan3, FDCAN_RX_FIFO0, header, data) != HAL_OK)
        return 0;  // ����ʧ��
    return header->DataLength >> 16;  // �������ݳ���
}

/**
************************************************************************
* @brief:      	HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
* @param:       hfdcan��FDCAN���
* @param:       RxFifo0ITs���жϱ�־λ
* @retval:     	void
* @details:    	HAL���FDCAN�жϻص�����
************************************************************************
**/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        if (hfdcan == &hfdcan1)
        {
            fdcan1_rx_callback();  // FDCAN1���ջص�
        }
        if (hfdcan == &hfdcan2)
        {
            fdcan2_rx_callback();  // FDCAN2���ջص�
        }
        if (hfdcan == &hfdcan3)
        {
            fdcan3_rx_callback();  // FDCAN3���ջص�
        }
    }
}

/**
************************************************************************
* @brief:      	fdcan1_rx_callback(void)
* @param:      	void
* @retval:     	void
* @details:    	���û����õ�FDCAN1�������ݻص�����
************************************************************************
**/
void fdcan1_rx_callback(void)
{
    FDCAN1_Receive_Msg(&FDCan1_RxHeader,FDCAN1_Rx_Data);  // ����FDCAN1������
	// Motor_Data_Get(&Motors,&FDCan1_RxHeader,FDCAN1_Rx_Data);
    Motor_Data_Handler(&Motors,FDCan1_RxHeader.Identifier,FDCAN1_Rx_Data);
}

/**
************************************************************************
* @brief:      	fdcan2_rx_callback(void)
* @param:      	void
* @retval:     	void
* @details:    	���û����õ�FDCAN2�������ݻص�����
************************************************************************
**/
void fdcan2_rx_callback(void)
{
    FDCAN2_Receive_Msg(&FDCan2_RxHeader,FDCAN2_Rx_Data);  // ����FDCAN2������
}

/**
************************************************************************
* @brief:      	fdcan3_rx_callback(void)
* @param:      	void
* @retval:     	void
* @details:    	���û����õ�FDCAN3�������ݻص�����
************************************************************************
**/
void fdcan3_rx_callback(void)
{
    FDCAN3_Receive_Msg(&FDCan3_RxHeader,FDCAN3_Rx_Data);  // ����FDCAN3������
}
