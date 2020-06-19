/******************************************************************************
 * @file     Parameter.h
 * @brief   参数设定
 * @version  
 * @date     
 * @note
 * 
 *
 * @par     此文件仅用于定参数   
*******************************************************************************/
#ifndef __PARAMETER_H_
#define __PARAMETER_H_

#include "boards.h"
#include "nrf.h"
#include "string.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_wdt.h"
#include "FilterPeak.h"

//运行模式
#define Mode_BlueTooth   1
#define Mode_Uart        2
/************************************************************************
*呼吸状态
*************************************************************************/
#define STATUS_NOPEOPEL    0
#define STATUS_NOHR        1
#define STATUS_NORR        2
#define STATUS_APNOEA      3   //呼吸暂停
#define STATUS_RRCANNOTCAL 4   //呼吸无法计算
#define STATUS_HRCANNOTCAL 5   //心率无法计算
#define STATUS_OBSAPNOEA   6   //阻塞性呼吸暂停


/************************************************************************
*新协议指令CMD
*************************************************************************/
#define  CMD_START                0x24  //指令头
#define  CMD_LF1                  0x69  //指令结束字符
#define  CMD_LF2                  0x42  //指令结束字符
#define  HREARTRATE_DATA          0x01  //心率数据
#define  BREATHE_DATA             0x02  //呼吸数据
#define  BATTERY_DATA             0x10  //电池等级数据
/********************
*串口专用
********************/
#define CMD_UART_DEBUG            0x32  //调试信息
#define CMD_UART_SETBTNAME        0x33  //设置蓝牙名称
#define CMD_UART_READVER          0x34  //读取软件版本
#define CMD_UART_REQMODULESTATUS  0x35  //查询设备当前状态
#define  DEVICE_ABNORMAL_MESSAGE  0xF1  //设备异常信息（未使用）
#define  CMD_PRESSSENSER          0xF4  //压力传感器状态（未使用）
#define  PRESSSENSER_OFF          0X01  //压力传感器未压下（未使用）
#define  PRESSSENSER_ON           0X02  //压力传感器压下（未使用）
#define CMD_DELFLASHDATA          0XD1  //删除FLASH数据（未使用）
#define CMD_DELSAVENUM            0x01  //删除保存的发送数据编号（未使用）
#define CMD_DELALLDATA            0XFF  //删除所有的数据（未使用）
#define RES_COMMANDOK             0x01  //指令已接收
#define RES_COMMANDERR            0x00  //指令重复，不可再发送（未使用）
#define ENABLE_SENDPRESSSERSONSTATUS    //使能发送压力状态
/*********************************************************************************/

/********************
*FLASH芯片参数定义
********************/
#define  FLASH_EquipmentInfoAddress       0x000000     //设备信息保存位置，设备ID，
#define  FLASH_WIFIInfoAddress            0x001000     //WIFI模块连接的路由器名称和密码
#define  FLASH_ServerInfoAddress          0x002000     //保存服务器IP、服务器设置信息等
#define  FLASH_VerifyInfoAddress          0x003000     //保存服务器token校验信息
#define  FLASH_SaveSleepDataStartAddress  0x010000     /*保存统计数据的开始位置，该位置存储数据索引信息*/
    
                                                   /*下一个位置开始存储数据，最大可保存2032组*4K的数据*/                                            
#define TRUE 1
#define FALSE 0

/********************
*ADC采样值处理参数
********************/
extern uint8_t  SentWaveTime;   //发送波形
extern  uint16_t SimpleDataNum1;   //采样数据个数
extern  uint16_t SimpleDataNum2;   //采样数据个数



/********************
*串口参数
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
extern uint8_t  SIMUART_Receive_Right;  //正确格式字符串接收
extern uint8_t Flag_ReceivedServerOrder;
extern uint8_t Flag_UART_Debug;//串口调试信息标志

/********************
*时间参数
********************/
#define  TIMEER1_SETTIME_MS       1   //100        //定时器定时时间，单位ms
#define  GETNETWORKSTATUSTIME     30     //30s查询一次网络状态
#define  SLEEPDATA_CHANGETIME     300    //脱离监测时间改变数据传输速率
#define  SLEEPDATA_SENDTIME_1MIN  60     //1分钟发送一次睡眠数据
#define  SLEEPDATA_SENDTIME_20S   20     //20S发送一次睡眠数据
#define  SLEEPDATA_SENDTIME_3S    3      //3S发送一次睡眠数据
#define  NETWORKLINK_TIMEOUT_30S  30     //网络连接超时时间
#define  NETWORKLINK_TIMEOUT_10S  10
#define  NETWORKLINK_TIMEOUT_60S  60
#define  NETWORKLINK_TIMEOUT_120S 120
#define  SENDLINKSTATUS_TIME_5S    5    //发送连接状态时间间隔5s
extern const nrf_drv_timer_t  TIMER_CH1 ; //定时器1
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
extern struct   Time TimerSim;              //模拟时钟
extern uint8_t  RealTime[6];              //实时时间
extern uint16_t   Monitor_Offline_Time;    //脱离监测计时，
extern uint8_t   Flag_TimeAdj;          //用于时间校准，1分钟之内不和服务器进行两次校准
extern uint8_t   Adj_Time ;             //校准时间计时
extern uint16_t   GetSleepHrDataTimeCount;  //发送睡眠心率数据计时
extern uint16_t   GetSleepRrDataTimeCount;  //发送睡眠呼吸数据计时
extern uint16_t   GetBatteryDataTimeCount;  //发送电池数据计时

extern uint16_t  LedFlash;			//控制ed闪烁
extern uint16_t ADC_ChangeTimes;
extern uint8_t ADC_CatchTimes;
extern uint8_t WakeUpCount;
//extern uint16_t HrDataSendTimeCount;
//extern uint16_t RrDataSendTimeCount;
//extern uint16_t HrDataSendTimeCount;

/********************
*蓝牙参数
********************/
#define  BT_NoCONNECT  0
#define  BT_CONNECTED  1
#define  Flag_FirstRun   0x55       //程序第一次运行，清除Flash，保存默认数据
#define  APP_RX_BUF_SIZE   100
#define  ENABLESENDDATAAPP  1
#define  DISABLESENDDATAAPP  0
extern char APP_Order_Receive_Data[APP_RX_BUF_SIZE];   //用于保存app发送的指令串，
extern uint8_t APP_Order_Receive_len ;    //APP指令串长度
extern uint8_t APP_Order_Send_len;       //需要发送的数据长度，不包括指令及包头包尾格式数据
extern uint8_t  BlueTooth_Name[20];    //蓝牙名称，默认为DEVICE_NAME，可APP修改
extern uint8_t BlueTooth_Name_Len;    //蓝牙名称长度

/********************
*睡眠数据相关参数
********************/
#define SLEEPDTATABUF   100
#define STEP_SEND          1     //发送数据
#define STEP_RECEIVE       2     // 接收数据
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
extern uint8_t LinkStep;         //服务器连接步骤
extern uint8_t LinkVerfyDataSendOver; //校验数据已发送
extern uint8_t    Flag_Binding_Users ;  //设备是否绑定用户,
extern uint16_t   SleepData_SendTime;  //睡眠数据发送的时间
extern uint16_t   No_Binding_Users_Time ; //未关联用户计时，
extern uint8_t    Flag_No_Binding_Users_Time;//未关联用户计时标志，
extern char Flag_PeopleTurnforResp;            //呼吸值中体动标记
extern char Flag_PeopleTurnforHert;            //心率值中体动标记
extern int Last_Heart ;    //上一次的心率间隔
extern int Last_Resp;     //上一次的呼吸心率
extern uint8_t NoHeartTime;   //无计算心率值计时
extern uint8_t NoRespTime;   //无计算呼吸值计时
extern uint8_t SleepMode_flag;

/********************
*压力开关
********************/
#define EXTSENSOR_ISIN            1
#define EXTSENSOR_ISOUT           0
#define EXTSENSOR_PEOPLEOFFBED    0	
#define EXTSENSOR_PEOPLEONBED     1
#define EXTSENSOR_CANNOTCAL       2 //无法计算
#define ARITH_PEOPLEONBED         1  
#define ARITH_PEOPLEOFFBED        0
extern uint8_t Status_ExtSensorOnbed;    //外部传感器是否有人在床状态  
extern uint8_t ExtSensor_OnbedStatus_Count;  //传感器在床状态次数统计，累计多次状态一致才确定是否在床 
extern uint8_t OnbedStatus_CountTimer;   //外部传感器在床状态统计间隔时间
extern uint8_t BatteryCheck_CountTimer;   //电池检测间隔时间
extern uint8_t Flag_SendPresensorStatus;  //发送压力传感器状态
extern uint8_t SendPresensorStatus_Time;  //发送压力传感器时间
extern uint8_t SendBatteryLevel_Time;  //发送电池电量时间
/********************
*
********************/

#define  ERROR_N0_MAC             0X01    //设备无编号
#define  ERROR_NO_IP2            0X02    //设备无IP2
#define ADC_SAMPLERAGE      500   //采样率
extern char  *SoftVer;
extern uint8_t Flag_LED_Status;       //LED显示状态
extern uint8_t Flag_Check_Status ;
extern uint8_t Flag_PowerOn;       //是否是上电
extern uint8_t  HR_StartTime[6];          //心率计算开始时间
extern uint8_t  RR_StartTime[6];          //呼吸计算开始时间


/********************
*算法/睡眠数据
********************/
#define CALCULATE_TIME    1     //1s计算滤波一次
#define NUM_CALCULATE    ADC_SAMPLERAGE*CALCULATE_TIME  // NUM_CALCULATE/500 s 计算一次
#define NUM_ADCSAMBUF    NUM_CALCULATE*10     //ADC采样数组大小
extern uint8_t Sendswitch;
extern uint8_t heart_avg;
extern uint8_t breath_avg;
/********************
*电池电量
********************/
#define Battery_level_100  1 //电量为100%时
#define Battery_level_80   2 //电量为80%时
#define Battery_level_60   3 //电量为60%时
#define Battery_level_40   4 //电量为40%时
#define Battery_level_20   5 //电量为20%时
extern uint8_t Battery_level;
/********************
*发送包标记符
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