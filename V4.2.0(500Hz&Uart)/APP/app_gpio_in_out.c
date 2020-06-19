/****************************************************************************
 * @file     app_apio_in_out.c
 * @brief    芯片GPIO输入输出设置
 * @version  V1.0.0
 * @date     2017.9
 * @note
 * Copyright (C) 2017 张炜
 *
 * @par    NB-IOT模块
 *  
*****************************************************************************/
#include "app_gpio_in_out.h"
#include "string.h"
#include "BLE_NRF.h"

#define  PRESSURESENSOR_BASEMAXVALUE   500
#define  PRESSURESENSOR_BASEMINVALUE   300

uint16_t PressureSensor_BaseValue = PRESSURESENSOR_BASEMAXVALUE;    //压力传感器基础值

const nrf_drv_timer_t  TIMER_CH2 = NRF_DRV_TIMER_INSTANCE(2); //定时器2
nrf_saadc_value_t  adc_vdata[SAMPLES_IN_BUFFER];

//void  LED_Status(void);
	

float  ADC_VoltageValue[SAMPLES_IN_BUFFER]={0}; 

uint8_t CheckCount_ExtSensorMaxVal = 0;
uint8_t CheckCount_ExtSensorMinVal = 0;
/****************************************************************************
*	函 数 名: LED_GPIO_CONFIG
*	功能说明: 配置LED的IO引脚
*	形    参：无
*	返 回 值: 无
*****************************************************************************/ 
void ADC_Switch_CONFIG(void) 
{
  nrf_gpio_cfg_output_pullup(IO3_PIN);   
  nrf_gpio_cfg_output_pullup(IO4_PIN);   
  IO3_OFF;
  IO4_OFF;
}
/****************************************************************************
*	函 数 名: WIFI_GPIO_CONFIG
*	功能说明: 配置WIFI的IO引脚
*	形    参：无
*	返 回 值: 无
*****************************************************************************/ 
void OSC_GPIO_CONFIG(void) 
{
    nrf_gpio_cfg_output_pullup(0);   
    nrf_gpio_cfg_output_pullup(1);
    nrf_gpio_cfg_output_pullup(12);
    nrf_gpio_pin_clear(0);
    nrf_gpio_pin_clear(1);
    nrf_gpio_pin_clear(12);
}
/****************************************************************************
*	函 数 名: ADC_GPIO_CONFIG 
*	功能说明: 配置ADC的IO引脚
*	形    参：无
*	返 回 值: 
* 说    明：ADC配置,使用定时器2定时1ms采样一次数据
*****************************************************************************/ 
void ADC_GPIO_CONFIG(void) 
{
	ret_code_t err_code;
//---------------------------定时器配置----------------------------------	
	err_code = nrf_drv_timer_init(&TIMER_CH2, NULL, Timer2_event_handler);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event every 1ms */
    uint32_t ticks = nrf_drv_timer_us_to_ticks(&TIMER_CH2, 800);
    //uint32_t ticks = nrf_drv_timer_ms_to_ticks(&TIMER_CH2, 1);
    nrf_drv_timer_extended_compare(&TIMER_CH2, NRF_TIMER_CC_CHANNEL1, ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
   
    nrf_drv_timer_enable(&TIMER_CH2);

//---------------adc配置--------------------------------------------------	
    	/*
				ADC0：电池电量   
				ADC1：呼吸
				ADC2：心率
				ADC3：压阻
			*/
		//ADC1
    nrf_saadc_channel_config_t channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);   //配置ADC，使用AIN1，
    err_code = nrf_drv_saadc_channel_init(1, &channel_config);
    APP_ERROR_CHECK(err_code);

		//ADC2
		nrf_saadc_channel_config_t channel_config1 =
		NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);   //配置ADC，使用AIN2，
    err_code = nrf_drv_saadc_channel_init(2, &channel_config1);
    APP_ERROR_CHECK(err_code);

    //ADC3
		nrf_saadc_channel_config_t channel_config2 =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);   //配置ADC，使用AIN3，
    err_code = nrf_drv_saadc_channel_init(3, &channel_config2);
    APP_ERROR_CHECK(err_code);
	
    //ADC0
		nrf_saadc_channel_config_t channel_config3 =
		NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);   //配置ADC，使用AIN4，
    err_code = nrf_drv_saadc_channel_init(0, &channel_config3);
    APP_ERROR_CHECK(err_code);


		err_code = nrf_drv_saadc_init(NULL,saadc_callback);
    APP_ERROR_CHECK(err_code);
		
		err_code = nrf_drv_saadc_buffer_convert(adc_vdata,SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);
}	

/****************************************************************************
*	函 数 名: saadc_callback(nrf_drv_saadc_evt_t const * p_event)
*	功能说明: AD转换中断处理函数
*	形    参：无
*	返 回 值: 无
*****************************************************************************/
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
	uint16_t adc_data;
	uint8_t i = 0;
	ret_code_t err_code;
	
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
//		   err_code = nrf_drv_saadc_buffer_convert(adc_vdata,SAMPLES_IN_BUFFER);
//			 nrf_drv_saadc_sample();
    }
}
/****************************************************************************
*	函 数 名: COMOrderRetuen
*	功能说明: 串口指令状态返回
*	形    参: data返回的数据，len返回数据长度
*	返 回 值: 
* 说    明：
*****************************************************************************/
void COMOrderRetuen(uint8_t *data,uint8_t len)
{	
	 uint8_t i = 0;
	
   app_uart_put(CMD_START);
	 app_uart_put(0x01 );
	 app_uart_put(len+1);
	 for(i=0;i<len;i++)
	 {
		app_uart_put(data[i]);
	 }
	 app_uart_put(0xFF );
	 app_uart_put(CMD_LF1 );
	 app_uart_put(CMD_LF2 );
}

/****************************************************************************
*	函 数 名:  SendDeviceVerInfoToUart
*	功能说明:  通过串口发送设备版本信息
*	形    参：
*	返 回 值:
* 说    明：
*****************************************************************************/
void SendDeviceVerInfoToUart(char *str)
{
	
     uint8_t len;
     len = strlen(str);	
	 app_uart_put(CMD_START);
	 app_uart_put(0x01 );
	 app_uart_put(len+2);
	 app_uart_put(CMD_UART_READVER);
	 while(*str != '\0')
	 {
	    app_uart_put(*str++);
	 }
	 app_uart_put(0xFF );
	 app_uart_put(CMD_LF1 );
	 app_uart_put(CMD_LF2 );

    printf("%s\r\n",str);
}
/****************************************************************************
*	函 数 名:  SendDeviceMacToUart
*	功能说明:  通过串口1发送设备MAC
*	形    参：*mac  MAC数组，maclen MAC长度
*	返 回 值:
* 说    明：
*****************************************************************************//*
void SendDeviceMacToUart(uint8_t *mac,uint8_t maclen)
{
    uint8_t i = 0;
	
	 app_uart_put(CMD_START);
	 app_uart_put(0x01 );
	 app_uart_put(maclen+2);
	 app_uart_put(CMD_READMAC);
	 for(i=0;i<maclen;i++)
	 {
			app_uart_put(mac[i]);
	 }
	 app_uart_put(0xFF );
	 app_uart_put(CMD_LF1 );
	 app_uart_put(CMD_LF2 );
			
}*/
/****************************************************************************
*	函 数 名: LED_Status
*	功能说明: LED显示  
*	形    参：无
*	返 回 值: 无
*****************************************************************************//*
void  LED_Status(void)
{
	 if(Flag_BT_Linkstatus == BT_CONNECTED)   //蓝牙连接
	 {
		  LED1_ON;
           // if(Flag_UART_Debug == DEBUG_CMD_UART)
            {                    
                printf("已连上\r\n");
            }
	 }
	 else
	 {
		 if(Flag_LED_Status == LED_WIRELEE_INIT)  //初始化模式下慢闪，1s
		 {
			  if(LedFlash < 500)
			     LED1_ON;
				else if(LedFlash < 1000)
					 LED1_OFF;
				else
					LedFlash = 0;
				return ;
		 }
		 if(Flag_LED_Status == LED_WIRELEE_LINK)  //连接状态下灯快闪
		 {
       if(LedFlash <= 100)
			     LED1_ON;
				else if(LedFlash <= 200)
					 LED1_OFF;
				else
					LedFlash = 0;
				return ;
		 }		 
	 }
}*/
/****************************************************************************
*	函 数 名: ExtSensor_GPIO_CONFIG
*	功能说明: 外部压力开关的IO引脚
*	形    参：无
*	返 回 值: 无
*****************************************************************************/ 
void ExtSensor_GPIO_CONFIG(void) 
{
  nrf_gpio_cfg_input(SLEEP_MODE_PIN,NRF_GPIO_PIN_NOPULL);  
}
/****************************************************************************
*	函 数 名: UART_CONFIG
*	功能说明: 配置UART的IO引脚,
*	形    参：UART_DEBUG  打印串口  UART_NB  nb模块串口
*	返 回 值: 无
* 说    明：串口打印和NB模块的串口使用的是不同引脚
*****************************************************************************/ 
void UART_CONFIG(uint8_t uart_type)
{
	 uint32_t err_code;
	
	 if(uart_type == UART_DEBUG)
	 {
		  const app_uart_comm_params_t comm_params =
				{
						SIMUARTRX_PIN,
						SIMUARTTX_PIN,				
						0xFF,
						0xFF,
						APP_UART_FLOW_CONTROL_DISABLED,  //不使用流控
						false,
						UART_BAUDRATE_BAUDRATE_Baud115200
				};
			APP_UART_FIFO_INIT(&comm_params,
							 UART_RX_BUF_SIZE,
							 UART_TX_BUF_SIZE*2,
							 simuart_event_handle,
							 APP_IRQ_PRIORITY_MID,
							 err_code);
            //NRF_UART0->PSELRTS = 0xFFFFFFFF;
            //NRF_UART0->PSELCTS = 0xFFFFFFFF;
            APP_ERROR_CHECK(err_code);	
	 }	
}
/****************************************************************************
*	函 数 名: ClearUSART3BUF
*	功能说明: 清除串口缓存
*	形    参：无
*	返 回 值: 
* 说    明：
*****************************************************************************/
void ClearSIMUSARTBUF(void)
{
	uint16_t i = 0;

	memcpy(SIMUART_RX_BUFFER,0x00,UARTBUF_MAX_LEN);
	SIMUART_RX_LEN=0;
  SIMUART_Receive_Right = 0;
}
/****************************************************************************
*	函 数 名: ClearBLEBUF
*	功能说明: 清除蓝牙接收缓存
*	形    参：无
*	返 回 值: 
* 说    明：
*****************************************************************************/
void ClearBLEBUF(void)
{
	uint16_t i = 0;

	memcpy(APP_Order_Receive_Data,0x00,APP_RX_BUF_SIZE);
	APP_Order_Receive_len=0;
}
/****************************************************************************
*	函 数 名: ReadBTNameFromFlash()
*	功能说明: 读蓝牙名称
*	形    参：无
*	返 回 值: 返回1表示正确读取，返回0表示第一次程序启动，未初始化
* 说    明：蓝牙名称以\0结束，
*****************************************************************************/
char ReadBTNameFromFlash(void)	
{
	uint8_t dat[12];
	
    SPI_FLASH_BufferRead(dat, FLASH_EquipmentInfoAddress, 2);
    if(dat[0] == Flag_FirstRun)
    {
      BlueTooth_Name_Len=dat[1];
      memset(BlueTooth_Name,0,sizeof(BlueTooth_Name));
      SPI_FLASH_BufferRead(BlueTooth_Name, FLASH_EquipmentInfoAddress+2, BlueTooth_Name_Len);			
      return 1;
    }
    if(dat[0] != Flag_FirstRun)
    {		
       return 0;
    }
}
/****************************************************************************
*	函 数 名: SaveBTNameToFlash()
*	功能说明: 保存蓝牙名称
*	形    参：无
*	返 回 值: 无
* 说    明：蓝牙名称以\0结束，
*****************************************************************************/
char SaveBTNameToFlash(void)	
{     
    uint8_t dat[60];
    uint8_t i;
	uint8_t err = 0;
	memset(dat,0,sizeof(dat));  
    SPI_FLASH_SectorErase(FLASH_EquipmentInfoAddress);
    dat[0] = Flag_FirstRun; 
    dat[1] = BlueTooth_Name_Len;	
    //dat[2] = MAC_LEN;	  //设备编号长度
    for(i=0; i<dat[1];i++)
    {
        dat[i+2] = BlueTooth_Name[i];
    }
    //for(i=0; i<dat[2];i++)
    //{
    // dat[i+3+BlueTooth_Name_Len] = MAC_ID[i];
    //}			 
    SPI_FLASH_BufferWrite(dat,FLASH_EquipmentInfoAddress, 60);	
    return 1;
 	
}