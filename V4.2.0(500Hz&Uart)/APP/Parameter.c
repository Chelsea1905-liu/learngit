/******************************************************************************
 * @file     Parameter.c
 * @brief   �����趨
 * @version  
 * @date     2017
 * @note
 * Copyright (C)  2017 ���
 *
 * @par     ���ļ������ڶ�����  
*******************************************************************************/
#include "Parameter.h"

//�豸����ģʽ
//��Ϊ�������������֣�����ģʽ�²�������
uint8_t DeviceMode = Mode_BlueTooth;
/********************
*���ڲ���
********************/
uint8_t SIMUART_Receive_Right = 0 ;  //��ȷ��ʽ�ַ�������
char SIMUART_TX_BUFFER[UARTBUF_MAX_LEN];
char SIMUART_RX_BUFFER[UARTBUF_MAX_LEN];
uint16_t  SIMUART_RX_LEN = 0;
uint8_t  SIMUART_RX_STA = 0;
uint8_t Flag_UART_Debug = 0;//DEBUG_CMD_UART;//���ڵ�����Ϣ��־

/********************
*��������
********************/
uint8_t  BlueTooth_Name[20];    //�������ƣ�Ĭ��ΪDEVICE_NAME����APP�޸�
uint8_t BlueTooth_Name_Len;    //�������Ƴ���
char APP_Order_Receive_Data[APP_RX_BUF_SIZE];    //���ڱ���app���͵�ָ���
uint8_t APP_Order_Receive_len = 0;    //APPָ�����
uint8_t APP_Order_Send_len = 0;       //��Ҫ���͵����ݳ��ȣ�������ָ���ͷ��β��ʽ����

/********************
*ʱ�����
********************/
nrf_drv_wdt_channel_id m_channel_id;
struct Time TimerSim;              //ģ��ʱ��
uint8_t  RealTime[6];              //ʵʱʱ��
uint16_t ADC_ChangeTimes = 0;
uint8_t ADC_CatchTimes = 0;
uint8_t WakeUpCount = 0;
uint16_t   Monitor_Offline_Time = 0; //�������ʱ��
uint8_t   Flag_TimeAdj = 0;          //
uint8_t   Adj_Time = 0;             //У׼ʱ���ʱ
uint16_t   GetSleepHrDataTimeCount=0;  //����˯���������ݼ�ʱ
uint16_t   GetSleepRrDataTimeCount=0;  //����˯�ߺ������ݼ�ʱ
uint16_t   GetBatteryDataTimeCount=0;  //���͵�����ݼ�ʱ
uint16_t  LedFlash = 0;			//����ed��˸

/********************
*ADC����ֵ�������
********************/
uint16_t SimpleDataNum1 = 0;   //�������ݸ���
uint16_t SimpleDataNum2 = 0;   //�������ݸ���

/********************
*˯��������ز���
********************/
uint8_t Flag_PowerOn = 1;       //�Ƿ����ϵ�
char Flag_PeopleTurnforResp = 0;
char Flag_PeopleTurnforHert = 0;            //����ֵ���嶯���
int Last_Heart = 0;    //��һ�ε����ʼ��
int Last_Resp = 0;     //��һ�εĺ�������
uint8_t NoRespTime = 0;   //�޼������ֵ��ʱ
uint8_t NoHeartTime = 0;   //�޼�������ֵ��ʱ
uint8_t SleepMode_flag = 0;

/********************
*�������
********************/
uint16_t   SleepData_SendTime=0;  //˯�����ݷ��͵�ʱ��

/********************
*ѹ������
********************/
//uint8_t Status_ExtSensorIn = 0;    //�ⲿ�������Ƿ����״̬  
uint8_t Status_ExtSensorOnbed = 0;    //�ⲿ�������Ƿ������ڴ�״̬  
uint8_t ExtSensor_OnbedStatus_Count = 0;  //�������ڴ�״̬����ͳ�ƣ��ۼƶ��״̬һ�²�ȷ���Ƿ��ڴ� 
uint8_t OnbedStatus_CountTimer = 0;   //�ⲿ�������ڴ�״̬ͳ�Ƽ��ʱ��
uint8_t BatteryCheck_CountTimer = 0;   //��ؼ����ʱ��
uint8_t Flag_SendPresensorStatus = 0;  //����ѹ��������״̬
uint8_t SendPresensorStatus_Time = 0;  //����ѹ��������ʱ��
uint8_t SendBatteryLevel_Time = 0;  //���͵�ص���ʱ��

/********************
*
********************/
uint8_t Flag_LED_Status=0;       //LED��ʾ״̬
uint8_t Flag_Check_Status = 0;
uint8_t  HR_StartTime[6];          //���ʼ��㿪ʼʱ��
uint8_t  RR_StartTime[6];          //�������㿪ʼʱ��
/********************
*�ֽ�ͨ
********************/
uint8_t Sendswitch=0;
uint8_t heart_avg=0;
uint8_t breath_avg=0;
uint8_t update_progress=0;
uint8_t Battery_level=0;

//���Ͱ���Ƿ�
uint8_t SendRrDataToServerTimes = 0;
uint8_t SendHrDataToServerTimes = 0;
uint8_t SendBatteryDataTimes = 0;
uint8_t SendPressSenserStatusTimes = 0;
uint8_t sleep_mode_timer = 0;
uint16_t writedata_timer=0;