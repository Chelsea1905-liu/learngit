/****************************************************************************
 * @file     mian.c
 * @brief    主函数
 * @version  V4.0.0
 * @date     2020.3
 * @note
 * Copyright (C) 2020 
 *
 * @par    蓝牙版睡眠垫
 *         V4.0.0  使用NRF52832主控芯片
*****************************************************************************/
#include "main.h"
#include "BLE_NRF.h"
#include "Parameter.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
char  *SoftVer = "Hardware:SK-B171207\r\nSoftware:V4.1.0<20180604>";

const nrf_drv_timer_t  TIMER_CH1 = NRF_DRV_TIMER_INSTANCE(1); //定时器1
uint8_t close_flag = 0;
uint8_t send_data_uart[5]={0};  //lqs add
/****************************************************************************
*	函 数 名: SIMUART_Receive_Order_Dispose
*	功能说明: 串口指令处理函数
*	形    参：*dat指令数组
*	返 回 值: 无
*****************************************************************************/
void SIMUART_Receive_Order_Dispose(uint8_t *dat)
{
		uint8_t i =0;
	  uint8_t len;
	  uint8_t send_data[20];
	
	  switch(dat[3])
		{
			case CMD_UART_SETBTNAME:			 //设置蓝牙名称				 
                    BlueTooth_Name_Len = dat[2]+8;	  //设备编号长度
                    if(BlueTooth_Name_Len > 20)
                    {
                        if(Flag_UART_Debug == DEBUG_CMD_UART)
                        {
                            printf("请重新输入!\r\n");
                        }
                        BlueTooth_Name_Len = 0;
                        break;
                    }
                    len=strlen(MANUFACTURER_NAME);
                    for(i=0;i<len;i++)
                        BlueTooth_Name[i] = MANUFACTURER_NAME[i];
                    BlueTooth_Name[len] = '_';

                    for(i=0; i<(dat[2]-1);i++)
                    {
                        BlueTooth_Name[len+1+i] = dat[4+i];
                    }	    
                    if(Flag_UART_Debug == DEBUG_CMD_UART)
                    {                    
                        printf("已写入名字!\r\n");
                    }

                    SaveBTNameToFlash();
                    send_data[0] = CMD_UART_SETBTNAME;
                    send_data[1] = RES_COMMANDOK;
                    COMOrderRetuen(send_data,2);
			break;
                    
            case CMD_UART_READVER:          //读取版本
				 SendDeviceVerInfoToUart(SoftVer);
			break;
						
			case CMD_UART_DEBUG:             //调试信息
                if(Flag_UART_Debug == 0)
                {
                    Flag_UART_Debug = DEBUG_CMD_UART;
                }
                else
                {
                    Flag_UART_Debug = 0;
                }	
                if(Flag_UART_Debug == DEBUG_CMD_UART)
                {
                    printf("打开调试信息\r\n");
                }
                else
                {
                    printf("关闭调试信息\r\n");
                }
            break;
                
			default:	
            break;
			
		}
}
/****************************************************************************
*	函 数 名: SIMUART_Receive_Order_Analys
*	功能说明: 串口指令处理函数
*	形    参：无
*	返 回 值: 无
*****************************************************************************/ 
void  SIMUART_Receive_Order_Analys(void)
{
	 uint8_t i ,j; 
   uint8_t order_data[50];
	 uint8_t order_len = 0; 	
	 char *pstr = NULL;
	 char *pstr1 = NULL;	
	
	  pstr = strstr(SIMUART_RX_BUFFER,"$");
	  pstr1 = strstr(SIMUART_RX_BUFFER,"iB");
	
	  if((pstr!=NULL)&&(pstr1!=NULL))
		{
			if(pstr[1] == 0x01)
			{
				 order_len = pstr[2];
				 for(i=0;i<order_len+5;i++)
				 {
					 order_data[i]=pstr[i];
				 }
				  ClearSIMUSARTBUF();
				  SIMUART_Receive_Order_Dispose(order_data);			
				}				
		}
}

//新定义的串口处理函数  lqs add
void  UART_Receive_Analys(void)
{	
	char *pstr = NULL;
	char *pstr1 = NULL;	

	pstr = strstr(SIMUART_RX_BUFFER,"iUTMODE");
	pstr1 = strstr(SIMUART_RX_BUFFER,"iBTMODE");

	if(pstr!=NULL)    //切换到串口模式
	{		
		ClearSIMUSARTBUF();
		if(DeviceMode == Mode_Uart)
		{
			printf("当前已是串口模式\r\n");
			return;
		}			
		DeviceMode = Mode_Uart;
		printf("切换到串口模式\r\n");
		return;
	}
	
	if(pstr1!=NULL)   //切换到蓝牙模式
	{
		ClearSIMUSARTBUF();
		if(DeviceMode == Mode_BlueTooth)
		{
			printf("当前已是蓝牙模式\r\n");
			return;
		}	
		DeviceMode = Mode_BlueTooth;
		printf("切换到蓝牙模式\r\n");
	}
}
/****************************************************************************
*	函 数 名: app_timer1_init
*	功能说明: 定时器1初始化，定时1ms
*	形    参：无
*	返 回 值: 无
*****************************************************************************/ 
void app_timer1_init(void)
{
	 uint32_t err_code = NRF_SUCCESS;
	 uint32_t time_ticks;
	
	 err_code =nrf_drv_timer_init(&TIMER_CH1, NULL, Timer1_event_handler); 
	 APP_ERROR_CHECK(err_code);
   time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_CH1,TIMEER1_SETTIME_MS);  //定时1ms 
	
    nrf_drv_timer_extended_compare(
         &TIMER_CH1, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    
    nrf_drv_timer_enable(&TIMER_CH1);
}
/****************************************************************************
*	函 数 名: clock_initialization
*	功能说明: 外部晶振初始化
*	形    参：无
*	返 回 值: 
* 说    明：
*****************************************************************************/
void ExtClock_initialization(void)
{
    /* Start 32 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }

//    /* Start low frequency crystal oscillator for app_timer(used by bsp)*/
//    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
//    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
//    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

//    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
//    {
//        // Do nothing.
//    }
}
/****************************************************************************
*	函 数 名: WDT_Config
*	功能说明: 看门狗配置
*	形    参：
*	返 回 值: 
* 说    明：WDT_CONFIG_RELOAD_VALUE  3000  看门狗定时器超时时间约为3s
*****************************************************************************/
void WDT_Config(void)
{
	nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
	nrf_drv_wdt_init(&config, wdt_event_handler);  
	nrf_drv_wdt_channel_alloc(&m_channel_id);
	nrf_drv_wdt_enable();
}
/****************************************************************************
*	函 数 名: wdt_event_handler
*	功能说明: 看门狗配置
*	形    参：
*	返 回 值: 
* 说    明：
*****************************************************************************/
void wdt_event_handler(void)
{
    //NOTE: The max amount of time we can spend in WDT interrupt is two cycles of 32768[Hz] clock - after that, reset occurs
}
/****************************************************************************
*	函 数 名: Power_ON
*	功能说明: 开机启动  
*	形    参：无
*	返 回 值: 无
*****************************************************************************/
void Power_ON(void)
{
	uint32_t err_code;
	uint8_t dat = 0;
	uint8_t i = 0;

	ExtClock_initialization();
    //--------------------------各功能模块配置--------------------------------	
	NRF_UICR->NFCPINS = 0;  //nfc引脚作为GPIO	 
	app_timer1_init();      //定时器1初始化	
	SPI_FLASH_Init();       //外部flash初始化	  
	ADC_GPIO_CONFIG();       //ADC初始化
	ADC_Switch_CONFIG();       //ADC开关 
	UART_CONFIG(UART_DEBUG);//串口初始化
}

//void PeripheralsPowerClose(void)
//{
///* Clear exceptions and PendingIRQ from the FPU unit */
//    sd_ble_gap_adv_stop();
////    __set_FPSCR(__get_FPSCR() & ~(FPU_EXCEPTION_MASK));
////    (void) __get_FPSCR();
////    NVIC_ClearPendingIRQ(FPU_IRQn);

////    IO3_OFF;
////    IO4_OFF;
//    uint32_t err_code = sd_app_evt_wait();
//    APP_ERROR_CHECK(err_code);
//}
//void PeripheralsPowerOpen(void)
//{
//    ble_advertising_start(BLE_ADV_MODE_FAST);
////    IO3_ON;
////    IO4_ON;
//}

/****************************************************************************
*	函 数 名: main
*	功能说明: 主函数
*	形    参：无
*	返 回 值: 无
*****************************************************************************/
int main(void)
{
	char err_code;
	uint16_t i,j;	 
	char buf[100];
	uint32_t sFLASH_ID = 0;
	//--------------参数初始化----------------------------
	uint8_t Time_RM = 0;
	uint8_t send_data[5];
  ble_gap_addr_t device_addr1;
	Flag_PowerOn = 1;
	GetSleepHrDataTimeCount = 0;
  GetSleepRrDataTimeCount = 0;
	Flag_TimeAdj = 0;
  SleepMode_flag=0;
	ClearSIMUSARTBUF();
	SleepData_SendTime = SLEEPDATA_SENDTIME_3S;
	Status_ExtSensorOnbed = EXTSENSOR_PEOPLEOFFBED; 
	ExtSensor_OnbedStatus_Count = 0;
  WakeUpCount = 0;
	OnbedStatus_CountTimer = 0;
  BatteryCheck_CountTimer = 0;
	Flag_SendPresensorStatus = 0; 
	RingBuff_Init(&ringBuff_ori,0);
	//-----------------------------------------------------	
	Power_ON();                           //各功能模块初始化  
  sd_ble_gap_address_get(&device_addr1);//获取芯片MAC地址
	nrf_delay_ms(500);
  sFLASH_ID = SPI_FLASH_ReadID();
  ReadBTNameFromFlash();		
  nrf_delay_ms(1000);

  printf("开机，运行版本V4.2.0\r\n");
	nrf_delay_ms(500);

	WDT_Config();
  BLE_Start();                           //启动蓝牙服务
  IO3_ON;
  IO4_ON;
			
	while (true)
	{	 
		/*
		**控制功耗
		*/
		if(DeviceMode == Mode_BlueTooth)
		{
				if(sleep_mode_timer >= 30)
				{
						SleepMode_flag = 1;
						SendRrDataToServerTimes = 0;
						SendHrDataToServerTimes = 0;
						SendBatteryDataTimes = 0;
						SendPressSenserStatusTimes = 0;
						WakeUpCount = 0;
						sleep_mode_timer=0;
					 // PeripheralsPowerClose();
						sleep_mode_enter();
				}
				else 
				{
						SleepMode_flag = 0; 
				}		 

				if(SleepMode_flag == 0)
				{
					if(ADC_ChangeTimes >= 1)   //lqs test
					{
						ADC_ChangeTimes = 0;
						CalculateHrRrData();      //计算心率
					}
					if(Flag_PowerOn == 0)
					{
						Check_ExtSensor_PeopleOnBed(); //检查外部传感器在床状态
					}
					BatteryDataCalculate();
					
					if(GetSleepHrDataTimeCount >= 3000)
					{		
						if(Flag_PowerOn == 0)
						{
							SendHrDataToServer();    //发送心率数据
						}			 
						GetSleepHrDataTimeCount=0;
					}
					
					if(GetSleepRrDataTimeCount >= 3500)
					{
							if(Flag_PowerOn == 0)
							{
									//发送呼吸数据                
									SendRrDataToServer();

							}
							GetSleepRrDataTimeCount = 0;
					}
					if(GetBatteryDataTimeCount >= 4000)
					{
					  //发送电池电量 
						if(Battery_level != 0)
							SendBatteryData(Battery_level);

							GetBatteryDataTimeCount = 0;
					}
				}
				else
				{
						GetSleepHrDataTimeCount=0;
				}
		}
		
		if(DeviceMode == Mode_Uart)   //串口模式
		{			
			sleep_mode_timer=0;            //串口模式不休眠
			Check_ExtSensor_PeopleOnBed(); //检查外部传感器在床状态
			BatteryDataCalculate();
			
			if(ADC_ChangeTimes >= 10)
			{
				ADC_ChangeTimes = 0;
				CalculateHrRrData();      //计算心率
			}
			
			if(GetSleepHrDataTimeCount >= 3000)
			{
				 SendHrDataToServer();    //发送心率数据
				 SendRrDataToServer();    //发送呼吸数据
				//串口发送数据
				 app_uart_put(0x55);       //帧头
				 app_uart_put(send_data_uart[0]); //心率
				 app_uart_put(send_data_uart[1]); //呼吸
				 app_uart_put(move_body);         //体动
				 app_uart_put(send_data_uart[3]); //在床状态
				
				 send_data_uart[4] = send_data_uart[0] + send_data_uart[1] + move_body + send_data_uart[3];
				 app_uart_put(send_data_uart[4]); //校验
				 send_data_uart[4] = 0;         
				
				 app_uart_put(0xAA);         //帧尾
				 printf("\r\n");
				GetSleepHrDataTimeCount=0;
			}
		}
	
/*****************************指令处理***********************************************/				
	if((SIMUART_Receive_Right == 1)&&(SIMUART_RX_LEN > 4)) //调试串口数据
	{
		SIMUART_Receive_Right = 0 ;
//		SIMUART_Receive_Order_Analys();
		UART_Receive_Analys();
	}
/****************************************************************************/	
	}//end while		
}