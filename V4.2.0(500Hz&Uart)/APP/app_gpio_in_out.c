/****************************************************************************
 * @file     app_apio_in_out.c
 * @brief    оƬGPIO�����������
 * @version  V1.0.0
 * @date     2017.9
 * @note
 * Copyright (C) 2017 ���
 *
 * @par    NB-IOTģ��
 *  
*****************************************************************************/
#include "app_gpio_in_out.h"
#include "string.h"
#include "BLE_NRF.h"

#define  PRESSURESENSOR_BASEMAXVALUE   500
#define  PRESSURESENSOR_BASEMINVALUE   300

uint16_t PressureSensor_BaseValue = PRESSURESENSOR_BASEMAXVALUE;    //ѹ������������ֵ

const nrf_drv_timer_t  TIMER_CH2 = NRF_DRV_TIMER_INSTANCE(2); //��ʱ��2
nrf_saadc_value_t  adc_vdata[SAMPLES_IN_BUFFER];

//void  LED_Status(void);
	

float  ADC_VoltageValue[SAMPLES_IN_BUFFER]={0}; 

uint8_t CheckCount_ExtSensorMaxVal = 0;
uint8_t CheckCount_ExtSensorMinVal = 0;
/****************************************************************************
*	�� �� ��: LED_GPIO_CONFIG
*	����˵��: ����LED��IO����
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
void ADC_Switch_CONFIG(void) 
{
  nrf_gpio_cfg_output_pullup(IO3_PIN);   
  nrf_gpio_cfg_output_pullup(IO4_PIN);   
  IO3_OFF;
  IO4_OFF;
}
/****************************************************************************
*	�� �� ��: WIFI_GPIO_CONFIG
*	����˵��: ����WIFI��IO����
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: ADC_GPIO_CONFIG 
*	����˵��: ����ADC��IO����
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����ADC����,ʹ�ö�ʱ��2��ʱ1ms����һ������
*****************************************************************************/ 
void ADC_GPIO_CONFIG(void) 
{
	ret_code_t err_code;
//---------------------------��ʱ������----------------------------------	
	err_code = nrf_drv_timer_init(&TIMER_CH2, NULL, Timer2_event_handler);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event every 1ms */
    uint32_t ticks = nrf_drv_timer_us_to_ticks(&TIMER_CH2, 800);
    //uint32_t ticks = nrf_drv_timer_ms_to_ticks(&TIMER_CH2, 1);
    nrf_drv_timer_extended_compare(&TIMER_CH2, NRF_TIMER_CC_CHANNEL1, ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
   
    nrf_drv_timer_enable(&TIMER_CH2);

//---------------adc����--------------------------------------------------	
    	/*
				ADC0����ص���   
				ADC1������
				ADC2������
				ADC3��ѹ��
			*/
		//ADC1
    nrf_saadc_channel_config_t channel_config =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);   //����ADC��ʹ��AIN1��
    err_code = nrf_drv_saadc_channel_init(1, &channel_config);
    APP_ERROR_CHECK(err_code);

		//ADC2
		nrf_saadc_channel_config_t channel_config1 =
		NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);   //����ADC��ʹ��AIN2��
    err_code = nrf_drv_saadc_channel_init(2, &channel_config1);
    APP_ERROR_CHECK(err_code);

    //ADC3
		nrf_saadc_channel_config_t channel_config2 =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);   //����ADC��ʹ��AIN3��
    err_code = nrf_drv_saadc_channel_init(3, &channel_config2);
    APP_ERROR_CHECK(err_code);
	
    //ADC0
		nrf_saadc_channel_config_t channel_config3 =
		NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);   //����ADC��ʹ��AIN4��
    err_code = nrf_drv_saadc_channel_init(0, &channel_config3);
    APP_ERROR_CHECK(err_code);


		err_code = nrf_drv_saadc_init(NULL,saadc_callback);
    APP_ERROR_CHECK(err_code);
		
		err_code = nrf_drv_saadc_buffer_convert(adc_vdata,SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);
}	

/****************************************************************************
*	�� �� ��: saadc_callback(nrf_drv_saadc_evt_t const * p_event)
*	����˵��: ADת���жϴ�����
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: COMOrderRetuen
*	����˵��: ����ָ��״̬����
*	��    ��: data���ص����ݣ�len�������ݳ���
*	�� �� ֵ: 
* ˵    ����
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
*	�� �� ��:  SendDeviceVerInfoToUart
*	����˵��:  ͨ�����ڷ����豸�汾��Ϣ
*	��    �Σ�
*	�� �� ֵ:
* ˵    ����
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
*	�� �� ��:  SendDeviceMacToUart
*	����˵��:  ͨ������1�����豸MAC
*	��    �Σ�*mac  MAC���飬maclen MAC����
*	�� �� ֵ:
* ˵    ����
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
*	�� �� ��: LED_Status
*	����˵��: LED��ʾ  
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************//*
void  LED_Status(void)
{
	 if(Flag_BT_Linkstatus == BT_CONNECTED)   //��������
	 {
		  LED1_ON;
           // if(Flag_UART_Debug == DEBUG_CMD_UART)
            {                    
                printf("������\r\n");
            }
	 }
	 else
	 {
		 if(Flag_LED_Status == LED_WIRELEE_INIT)  //��ʼ��ģʽ��������1s
		 {
			  if(LedFlash < 500)
			     LED1_ON;
				else if(LedFlash < 1000)
					 LED1_OFF;
				else
					LedFlash = 0;
				return ;
		 }
		 if(Flag_LED_Status == LED_WIRELEE_LINK)  //����״̬�µƿ���
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
*	�� �� ��: ExtSensor_GPIO_CONFIG
*	����˵��: �ⲿѹ�����ص�IO����
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
void ExtSensor_GPIO_CONFIG(void) 
{
  nrf_gpio_cfg_input(SLEEP_MODE_PIN,NRF_GPIO_PIN_NOPULL);  
}
/****************************************************************************
*	�� �� ��: UART_CONFIG
*	����˵��: ����UART��IO����,
*	��    �Σ�UART_DEBUG  ��ӡ����  UART_NB  nbģ�鴮��
*	�� �� ֵ: ��
* ˵    �������ڴ�ӡ��NBģ��Ĵ���ʹ�õ��ǲ�ͬ����
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
						APP_UART_FLOW_CONTROL_DISABLED,  //��ʹ������
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
*	�� �� ��: ClearUSART3BUF
*	����˵��: ������ڻ���
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void ClearSIMUSARTBUF(void)
{
	uint16_t i = 0;

	memcpy(SIMUART_RX_BUFFER,0x00,UARTBUF_MAX_LEN);
	SIMUART_RX_LEN=0;
  SIMUART_Receive_Right = 0;
}
/****************************************************************************
*	�� �� ��: ClearBLEBUF
*	����˵��: ����������ջ���
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void ClearBLEBUF(void)
{
	uint16_t i = 0;

	memcpy(APP_Order_Receive_Data,0x00,APP_RX_BUF_SIZE);
	APP_Order_Receive_len=0;
}
/****************************************************************************
*	�� �� ��: ReadBTNameFromFlash()
*	����˵��: ����������
*	��    �Σ���
*	�� �� ֵ: ����1��ʾ��ȷ��ȡ������0��ʾ��һ�γ���������δ��ʼ��
* ˵    ��������������\0������
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
*	�� �� ��: SaveBTNameToFlash()
*	����˵��: ������������
*	��    �Σ���
*	�� �� ֵ: ��
* ˵    ��������������\0������
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
    //dat[2] = MAC_LEN;	  //�豸��ų���
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