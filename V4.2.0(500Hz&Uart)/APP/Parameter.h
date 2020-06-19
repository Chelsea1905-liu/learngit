/******************************************************************************
 * @file     Parameter.h
 * @brief   �����趨
 * @version  
 * @date     
 * @note
 * 
 *
 * @par     ���ļ������ڶ�����   
*******************************************************************************/
#ifndef __PARAMETER_H_
#define __PARAMETER_H_

#include "boards.h"
#include "nrf.h"
#include "string.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_wdt.h"
#include "FilterPeak.h"

//����ģʽ
#define Mode_BlueTooth   1
#define Mode_Uart        2
/************************************************************************
*����״̬
*************************************************************************/
#define STATUS_NOPEOPEL    0
#define STATUS_NOHR        1
#define STATUS_NORR        2
#define STATUS_APNOEA      3   //������ͣ
#define STATUS_RRCANNOTCAL 4   //�����޷�����
#define STATUS_HRCANNOTCAL 5   //�����޷�����
#define STATUS_OBSAPNOEA   6   //�����Ժ�����ͣ


/************************************************************************
*��Э��ָ��CMD
*************************************************************************/
#define  CMD_START                0x24  //ָ��ͷ
#define  CMD_LF1                  0x69  //ָ������ַ�
#define  CMD_LF2                  0x42  //ָ������ַ�
#define  HREARTRATE_DATA          0x01  //��������
#define  BREATHE_DATA             0x02  //��������
#define  BATTERY_DATA             0x10  //��صȼ�����
/********************
*����ר��
********************/
#define CMD_UART_DEBUG            0x32  //������Ϣ
#define CMD_UART_SETBTNAME        0x33  //������������
#define CMD_UART_READVER          0x34  //��ȡ����汾
#define CMD_UART_REQMODULESTATUS  0x35  //��ѯ�豸��ǰ״̬
#define  DEVICE_ABNORMAL_MESSAGE  0xF1  //�豸�쳣��Ϣ��δʹ�ã�
#define  CMD_PRESSSENSER          0xF4  //ѹ��������״̬��δʹ�ã�
#define  PRESSSENSER_OFF          0X01  //ѹ��������δѹ�£�δʹ�ã�
#define  PRESSSENSER_ON           0X02  //ѹ��������ѹ�£�δʹ�ã�
#define CMD_DELFLASHDATA          0XD1  //ɾ��FLASH���ݣ�δʹ�ã�
#define CMD_DELSAVENUM            0x01  //ɾ������ķ������ݱ�ţ�δʹ�ã�
#define CMD_DELALLDATA            0XFF  //ɾ�����е����ݣ�δʹ�ã�
#define RES_COMMANDOK             0x01  //ָ���ѽ���
#define RES_COMMANDERR            0x00  //ָ���ظ��������ٷ��ͣ�δʹ�ã�
#define ENABLE_SENDPRESSSERSONSTATUS    //ʹ�ܷ���ѹ��״̬
/*********************************************************************************/

/********************
*FLASHоƬ��������
********************/
#define  FLASH_EquipmentInfoAddress       0x000000     //�豸��Ϣ����λ�ã��豸ID��
#define  FLASH_WIFIInfoAddress            0x001000     //WIFIģ�����ӵ�·�������ƺ�����
#define  FLASH_ServerInfoAddress          0x002000     //���������IP��������������Ϣ��
#define  FLASH_VerifyInfoAddress          0x003000     //���������tokenУ����Ϣ
#define  FLASH_SaveSleepDataStartAddress  0x010000     /*����ͳ�����ݵĿ�ʼλ�ã���λ�ô洢����������Ϣ*/
    
                                                   /*��һ��λ�ÿ�ʼ�洢���ݣ����ɱ���2032��*4K������*/                                            
#define TRUE 1
#define FALSE 0

/********************
*ADC����ֵ�������
********************/
extern uint8_t  SentWaveTime;   //���Ͳ���
extern  uint16_t SimpleDataNum1;   //�������ݸ���
extern  uint16_t SimpleDataNum2;   //�������ݸ���



/********************
*���ڲ���
********************/
#define UARTBUF_MAX_LEN  200
#define UART_DEBUG  1
#define UART_WIFI     2
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */
#define DEBUG_COM      1
#define DEBUG_BLE      2
#define DEBUG_CMD_UART 3
extern char  SIMUART_RX_BUFFER[UARTBUF_MAX_LEN];
extern char  SIMUART_TX_BUFFER[UARTBUF_MAX_LEN];
extern uint16_t  SIMUART_RX_LEN;
extern uint8_t  SMINUART_RX_STA;
extern uint8_t  SIMUART_Receive_Right;  //��ȷ��ʽ�ַ�������
extern uint8_t Flag_ReceivedServerOrder;
extern uint8_t Flag_UART_Debug;//���ڵ�����Ϣ��־

/********************
*ʱ�����
********************/
#define  TIMEER1_SETTIME_MS       1   //100        //��ʱ����ʱʱ�䣬��λms
#define  GETNETWORKSTATUSTIME     30     //30s��ѯһ������״̬
#define  SLEEPDATA_CHANGETIME     300    //������ʱ��ı����ݴ�������
#define  SLEEPDATA_SENDTIME_1MIN  60     //1���ӷ���һ��˯������
#define  SLEEPDATA_SENDTIME_20S   20     //20S����һ��˯������
#define  SLEEPDATA_SENDTIME_3S    3      //3S����һ��˯������
#define  NETWORKLINK_TIMEOUT_30S  30     //�������ӳ�ʱʱ��
#define  NETWORKLINK_TIMEOUT_10S  10
#define  NETWORKLINK_TIMEOUT_60S  60
#define  NETWORKLINK_TIMEOUT_120S 120
#define  SENDLINKSTATUS_TIME_5S    5    //��������״̬ʱ����5s
extern const nrf_drv_timer_t  TIMER_CH1 ; //��ʱ��1
extern const nrf_drv_timer_t  TIMER_CH2;
extern nrf_drv_wdt_channel_id m_channel_id;
struct Time   //
{
	 uint16_t Msec;
	 uint8_t Sec;
	 uint8_t Min;
	 uint8_t Hour;
	 uint16_t Day;
};
extern struct   Time TimerSim;              //ģ��ʱ��
extern uint8_t  RealTime[6];              //ʵʱʱ��
extern uint16_t   Monitor_Offline_Time;    //�������ʱ��
extern uint8_t   Flag_TimeAdj;          //����ʱ��У׼��1����֮�ڲ��ͷ�������������У׼
extern uint8_t   Adj_Time ;             //У׼ʱ���ʱ
extern uint16_t   GetSleepHrDataTimeCount;  //����˯���������ݼ�ʱ
extern uint16_t   GetSleepRrDataTimeCount;  //����˯�ߺ������ݼ�ʱ
extern uint16_t   GetBatteryDataTimeCount;  //���͵�����ݼ�ʱ

extern uint16_t  LedFlash;			//����ed��˸
extern uint16_t ADC_ChangeTimes;
extern uint8_t ADC_CatchTimes;
extern uint8_t WakeUpCount;
//extern uint16_t HrDataSendTimeCount;
//extern uint16_t RrDataSendTimeCount;
//extern uint16_t HrDataSendTimeCount;

/********************
*��������
********************/
#define  BT_NoCONNECT  0
#define  BT_CONNECTED  1
#define  Flag_FirstRun   0x55       //�����һ�����У����Flash������Ĭ������
#define  APP_RX_BUF_SIZE   100
#define  ENABLESENDDATAAPP  1
#define  DISABLESENDDATAAPP  0
extern char APP_Order_Receive_Data[APP_RX_BUF_SIZE];   //���ڱ���app���͵�ָ���
extern uint8_t APP_Order_Receive_len ;    //APPָ�����
extern uint8_t APP_Order_Send_len;       //��Ҫ���͵����ݳ��ȣ�������ָ���ͷ��β��ʽ����
extern uint8_t  BlueTooth_Name[20];    //�������ƣ�Ĭ��ΪDEVICE_NAME����APP�޸�
extern uint8_t BlueTooth_Name_Len;    //�������Ƴ���

/********************
*˯��������ز���
********************/
#define SLEEPDTATABUF   100
#define STEP_SEND          1     //��������
#define STEP_RECEIVE       2     // ��������
extern int Position_LAC;
extern int Position_CID;
extern uint8_t Socket;
extern unsigned char SIMCard_IMSI[20];
extern uint8_t SIMCard_IMSI_Len;
extern uint8_t Command_SendCount;
extern char IP2[30];
extern char Port2[5];
extern char Token2[15]; 
extern uint8_t Token2_Len;
extern uint8_t LinkStep;         //���������Ӳ���
extern uint8_t LinkVerfyDataSendOver; //У�������ѷ���
extern uint8_t    Flag_Binding_Users ;  //�豸�Ƿ���û�,
extern uint16_t   SleepData_SendTime;  //˯�����ݷ��͵�ʱ��
extern uint16_t   No_Binding_Users_Time ; //δ�����û���ʱ��
extern uint8_t    Flag_No_Binding_Users_Time;//δ�����û���ʱ��־��
extern char Flag_PeopleTurnforResp;            //����ֵ���嶯���
extern char Flag_PeopleTurnforHert;            //����ֵ���嶯���
extern int Last_Heart ;    //��һ�ε����ʼ��
extern int Last_Resp;     //��һ�εĺ�������
extern uint8_t NoHeartTime;   //�޼�������ֵ��ʱ
extern uint8_t NoRespTime;   //�޼������ֵ��ʱ
extern uint8_t SleepMode_flag;

/********************
*ѹ������
********************/
#define EXTSENSOR_ISIN            1
#define EXTSENSOR_ISOUT           0
#define EXTSENSOR_PEOPLEOFFBED    0	
#define EXTSENSOR_PEOPLEONBED     1
#define EXTSENSOR_CANNOTCAL       2 //�޷�����
#define ARITH_PEOPLEONBED         1  
#define ARITH_PEOPLEOFFBED        0
extern uint8_t Status_ExtSensorOnbed;    //�ⲿ�������Ƿ������ڴ�״̬  
extern uint8_t ExtSensor_OnbedStatus_Count;  //�������ڴ�״̬����ͳ�ƣ��ۼƶ��״̬һ�²�ȷ���Ƿ��ڴ� 
extern uint8_t OnbedStatus_CountTimer;   //�ⲿ�������ڴ�״̬ͳ�Ƽ��ʱ��
extern uint8_t BatteryCheck_CountTimer;   //��ؼ����ʱ��
extern uint8_t Flag_SendPresensorStatus;  //����ѹ��������״̬
extern uint8_t SendPresensorStatus_Time;  //����ѹ��������ʱ��
extern uint8_t SendBatteryLevel_Time;  //���͵�ص���ʱ��
/********************
*
********************/

#define  ERROR_N0_MAC             0X01    //�豸�ޱ��
#define  ERROR_NO_IP2            0X02    //�豸��IP2
#define ADC_SAMPLERAGE      500   //������
extern char  *SoftVer;
extern uint8_t Flag_LED_Status;       //LED��ʾ״̬
extern uint8_t Flag_Check_Status ;
extern uint8_t Flag_PowerOn;       //�Ƿ����ϵ�
extern uint8_t  HR_StartTime[6];          //���ʼ��㿪ʼʱ��
extern uint8_t  RR_StartTime[6];          //�������㿪ʼʱ��


/********************
*�㷨/˯������
********************/
#define CALCULATE_TIME    1     //1s�����˲�һ��
#define NUM_CALCULATE    ADC_SAMPLERAGE*CALCULATE_TIME  // NUM_CALCULATE/500 s ����һ��
#define NUM_ADCSAMBUF    NUM_CALCULATE*10     //ADC���������С
extern uint8_t Sendswitch;
extern uint8_t heart_avg;
extern uint8_t breath_avg;
/********************
*��ص���
********************/
#define Battery_level_100  1 //����Ϊ100%ʱ
#define Battery_level_80   2 //����Ϊ80%ʱ
#define Battery_level_60   3 //����Ϊ60%ʱ
#define Battery_level_40   4 //����Ϊ40%ʱ
#define Battery_level_20   5 //����Ϊ20%ʱ
extern uint8_t Battery_level;
/********************
*���Ͱ���Ƿ�
********************/
extern uint8_t SendRrDataToServerTimes;
extern uint8_t SendHrDataToServerTimes;
extern uint8_t SendBatteryDataTimes;
extern uint8_t SendPressSenserStatusTimes;

extern uint8_t sleep_mode_timer;
#define FPU_EXCEPTION_MASK 0x0000009F

extern uint16_t writedata_timer;
extern uint8_t DeviceMode;  //lqs add
#endif