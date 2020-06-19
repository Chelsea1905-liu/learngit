/******************************************************************************
 * @file     Parameter.c
 * @brief   参数设定
 * @version  
 * @date     2017
 * @note
 * Copyright (C)  2017 张炜
 *
 * @par     此文件仅用于定参数  
*******************************************************************************/
#include "Parameter.h"

//设备运行模式
//分为蓝牙跟串口两种，串口模式下不会休眠
uint8_t DeviceMode = Mode_BlueTooth;
/********************
*串口参数
********************/
uint8_t SIMUART_Receive_Right = 0 ;  //正确格式字符串接收
char SIMUART_TX_BUFFER[UARTBUF_MAX_LEN];
char SIMUART_RX_BUFFER[UARTBUF_MAX_LEN];
uint16_t  SIMUART_RX_LEN = 0;
uint8_t  SIMUART_RX_STA = 0;
uint8_t Flag_UART_Debug = 0;//DEBUG_CMD_UART;//串口调试信息标志

/********************
*蓝牙参数
********************/
uint8_t  BlueTooth_Name[20];    //蓝牙名称，默认为DEVICE_NAME，可APP修改
uint8_t BlueTooth_Name_Len;    //蓝牙名称长度
char APP_Order_Receive_Data[APP_RX_BUF_SIZE];    //用于保存app发送的指令串，
uint8_t APP_Order_Receive_len = 0;    //APP指令串长度
uint8_t APP_Order_Send_len = 0;       //需要发送的数据长度，不包括指令及包头包尾格式数据

/********************
*时间参数
********************/
nrf_drv_wdt_channel_id m_channel_id;
struct Time TimerSim;              //模拟时钟
uint8_t  RealTime[6];              //实时时间
uint16_t ADC_ChangeTimes = 0;
uint8_t ADC_CatchTimes = 0;
uint8_t WakeUpCount = 0;
uint16_t   Monitor_Offline_Time = 0; //脱离监测计时，
uint8_t   Flag_TimeAdj = 0;          //
uint8_t   Adj_Time = 0;             //校准时间计时
uint16_t   GetSleepHrDataTimeCount=0;  //发送睡眠心率数据计时
uint16_t   GetSleepRrDataTimeCount=0;  //发送睡眠呼吸数据计时
uint16_t   GetBatteryDataTimeCount=0;  //发送电池数据计时
uint16_t  LedFlash = 0;			//控制ed闪烁

/********************
*ADC采样值处理参数
********************/
uint16_t SimpleDataNum1 = 0;   //采样数据个数
uint16_t SimpleDataNum2 = 0;   //采样数据个数

/********************
*睡眠数据相关参数
********************/
uint8_t Flag_PowerOn = 1;       //是否是上电
char Flag_PeopleTurnforResp = 0;
char Flag_PeopleTurnforHert = 0;            //心率值中体动标记
int Last_Heart = 0;    //上一次的心率间隔
int Last_Resp = 0;     //上一次的呼吸心率
uint8_t NoRespTime = 0;   //无计算呼吸值计时
uint8_t NoHeartTime = 0;   //无计算心率值计时
uint8_t SleepMode_flag = 0;

/********************
*网络参数
********************/
uint16_t   SleepData_SendTime=0;  //睡眠数据发送的时间

/********************
*压力开关
********************/
//uint8_t Status_ExtSensorIn = 0;    //外部传感器是否插入状态  
uint8_t Status_ExtSensorOnbed = 0;    //外部传感器是否有人在床状态  
uint8_t ExtSensor_OnbedStatus_Count = 0;  //传感器在床状态次数统计，累计多次状态一致才确定是否在床 
uint8_t OnbedStatus_CountTimer = 0;   //外部传感器在床状态统计间隔时间
uint8_t BatteryCheck_CountTimer = 0;   //电池检测间隔时间
uint8_t Flag_SendPresensorStatus = 0;  //发送压力传感器状态
uint8_t SendPresensorStatus_Time = 0;  //发送压力传感器时间
uint8_t SendBatteryLevel_Time = 0;  //发送电池电量时间

/********************
*
********************/
uint8_t Flag_LED_Status=0;       //LED显示状态
uint8_t Flag_Check_Status = 0;
uint8_t  HR_StartTime[6];          //心率计算开始时间
uint8_t  RR_StartTime[6];          //呼吸计算开始时间
/********************
*字节通
********************/
uint8_t Sendswitch=0;
uint8_t heart_avg=0;
uint8_t breath_avg=0;
uint8_t update_progress=0;
uint8_t Battery_level=0;

//发送包标记符
uint8_t SendRrDataToServerTimes = 0;
uint8_t SendHrDataToServerTimes = 0;
uint8_t SendBatteryDataTimes = 0;
uint8_t SendPressSenserStatusTimes = 0;
uint8_t sleep_mode_timer = 0;
uint16_t writedata_timer=0;