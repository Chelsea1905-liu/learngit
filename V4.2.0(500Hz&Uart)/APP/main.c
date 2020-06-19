/****************************************************************************
 * @file     mian.c
 * @brief    ������
 * @version  V4.0.0
 * @date     2020.3
 * @note
 * Copyright (C) 2020 
 *
 * @par    ������˯�ߵ�
 *         V4.0.0  ʹ��NRF52832����оƬ
*****************************************************************************/
#include "main.h"
#include "BLE_NRF.h"
#include "Parameter.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
char  *SoftVer = "Hardware:SK-B171207\r\nSoftware:V4.1.0<20180604>";

const nrf_drv_timer_t  TIMER_CH1 = NRF_DRV_TIMER_INSTANCE(1); //��ʱ��1
uint8_t close_flag = 0;
uint8_t send_data_uart[5]={0};  //lqs add
/****************************************************************************
*	�� �� ��: SIMUART_Receive_Order_Dispose
*	����˵��: ����ָ�����
*	��    �Σ�*datָ������
*	�� �� ֵ: ��
*****************************************************************************/
void SIMUART_Receive_Order_Dispose(uint8_t *dat)
{
		uint8_t i =0;
	  uint8_t len;
	  uint8_t send_data[20];
	
	  switch(dat[3])
		{
			case CMD_UART_SETBTNAME:			 //������������				 
                    BlueTooth_Name_Len = dat[2]+8;	  //�豸��ų���
                    if(BlueTooth_Name_Len > 20)
                    {
                        if(Flag_UART_Debug == DEBUG_CMD_UART)
                        {
                            printf("����������!\r\n");
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
                        printf("��д������!\r\n");
                    }

                    SaveBTNameToFlash();
                    send_data[0] = CMD_UART_SETBTNAME;
                    send_data[1] = RES_COMMANDOK;
                    COMOrderRetuen(send_data,2);
			break;
                    
            case CMD_UART_READVER:          //��ȡ�汾
				 SendDeviceVerInfoToUart(SoftVer);
			break;
						
			case CMD_UART_DEBUG:             //������Ϣ
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
                    printf("�򿪵�����Ϣ\r\n");
                }
                else
                {
                    printf("�رյ�����Ϣ\r\n");
                }
            break;
                
			default:	
            break;
			
		}
}
/****************************************************************************
*	�� �� ��: SIMUART_Receive_Order_Analys
*	����˵��: ����ָ�����
*	��    �Σ���
*	�� �� ֵ: ��
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

//�¶���Ĵ��ڴ�����  lqs add
void  UART_Receive_Analys(void)
{	
	char *pstr = NULL;
	char *pstr1 = NULL;	

	pstr = strstr(SIMUART_RX_BUFFER,"iUTMODE");
	pstr1 = strstr(SIMUART_RX_BUFFER,"iBTMODE");

	if(pstr!=NULL)    //�л�������ģʽ
	{		
		ClearSIMUSARTBUF();
		if(DeviceMode == Mode_Uart)
		{
			printf("��ǰ���Ǵ���ģʽ\r\n");
			return;
		}			
		DeviceMode = Mode_Uart;
		printf("�л�������ģʽ\r\n");
		return;
	}
	
	if(pstr1!=NULL)   //�л�������ģʽ
	{
		ClearSIMUSARTBUF();
		if(DeviceMode == Mode_BlueTooth)
		{
			printf("��ǰ��������ģʽ\r\n");
			return;
		}	
		DeviceMode = Mode_BlueTooth;
		printf("�л�������ģʽ\r\n");
	}
}
/****************************************************************************
*	�� �� ��: app_timer1_init
*	����˵��: ��ʱ��1��ʼ������ʱ1ms
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
void app_timer1_init(void)
{
	 uint32_t err_code = NRF_SUCCESS;
	 uint32_t time_ticks;
	
	 err_code =nrf_drv_timer_init(&TIMER_CH1, NULL, Timer1_event_handler); 
	 APP_ERROR_CHECK(err_code);
   time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_CH1,TIMEER1_SETTIME_MS);  //��ʱ1ms 
	
    nrf_drv_timer_extended_compare(
         &TIMER_CH1, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    
    nrf_drv_timer_enable(&TIMER_CH1);
}
/****************************************************************************
*	�� �� ��: clock_initialization
*	����˵��: �ⲿ�����ʼ��
*	��    �Σ���
*	�� �� ֵ: 
* ˵    ����
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
*	�� �� ��: WDT_Config
*	����˵��: ���Ź�����
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����WDT_CONFIG_RELOAD_VALUE  3000  ���Ź���ʱ����ʱʱ��ԼΪ3s
*****************************************************************************/
void WDT_Config(void)
{
	nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
	nrf_drv_wdt_init(&config, wdt_event_handler);  
	nrf_drv_wdt_channel_alloc(&m_channel_id);
	nrf_drv_wdt_enable();
}
/****************************************************************************
*	�� �� ��: wdt_event_handler
*	����˵��: ���Ź�����
*	��    �Σ�
*	�� �� ֵ: 
* ˵    ����
*****************************************************************************/
void wdt_event_handler(void)
{
    //NOTE: The max amount of time we can spend in WDT interrupt is two cycles of 32768[Hz] clock - after that, reset occurs
}
/****************************************************************************
*	�� �� ��: Power_ON
*	����˵��: ��������  
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void Power_ON(void)
{
	uint32_t err_code;
	uint8_t dat = 0;
	uint8_t i = 0;

	ExtClock_initialization();
    //--------------------------������ģ������--------------------------------	
	NRF_UICR->NFCPINS = 0;  //nfc������ΪGPIO	 
	app_timer1_init();      //��ʱ��1��ʼ��	
	SPI_FLASH_Init();       //�ⲿflash��ʼ��	  
	ADC_GPIO_CONFIG();       //ADC��ʼ��
	ADC_Switch_CONFIG();       //ADC���� 
	UART_CONFIG(UART_DEBUG);//���ڳ�ʼ��
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
*	�� �� ��: main
*	����˵��: ������
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
int main(void)
{
	char err_code;
	uint16_t i,j;	 
	char buf[100];
	uint32_t sFLASH_ID = 0;
	//--------------������ʼ��----------------------------
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
	Power_ON();                           //������ģ���ʼ��  
  sd_ble_gap_address_get(&device_addr1);//��ȡоƬMAC��ַ
	nrf_delay_ms(500);
  sFLASH_ID = SPI_FLASH_ReadID();
  ReadBTNameFromFlash();		
  nrf_delay_ms(1000);

  printf("���������а汾V4.2.0\r\n");
	nrf_delay_ms(500);

	WDT_Config();
  BLE_Start();                           //������������
  IO3_ON;
  IO4_ON;
			
	while (true)
	{	 
		/*
		**���ƹ���
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
						CalculateHrRrData();      //��������
					}
					if(Flag_PowerOn == 0)
					{
						Check_ExtSensor_PeopleOnBed(); //����ⲿ�������ڴ�״̬
					}
					BatteryDataCalculate();
					
					if(GetSleepHrDataTimeCount >= 3000)
					{		
						if(Flag_PowerOn == 0)
						{
							SendHrDataToServer();    //������������
						}			 
						GetSleepHrDataTimeCount=0;
					}
					
					if(GetSleepRrDataTimeCount >= 3500)
					{
							if(Flag_PowerOn == 0)
							{
									//���ͺ�������                
									SendRrDataToServer();

							}
							GetSleepRrDataTimeCount = 0;
					}
					if(GetBatteryDataTimeCount >= 4000)
					{
					  //���͵�ص��� 
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
		
		if(DeviceMode == Mode_Uart)   //����ģʽ
		{			
			sleep_mode_timer=0;            //����ģʽ������
			Check_ExtSensor_PeopleOnBed(); //����ⲿ�������ڴ�״̬
			BatteryDataCalculate();
			
			if(ADC_ChangeTimes >= 10)
			{
				ADC_ChangeTimes = 0;
				CalculateHrRrData();      //��������
			}
			
			if(GetSleepHrDataTimeCount >= 3000)
			{
				 SendHrDataToServer();    //������������
				 SendRrDataToServer();    //���ͺ�������
				//���ڷ�������
				 app_uart_put(0x55);       //֡ͷ
				 app_uart_put(send_data_uart[0]); //����
				 app_uart_put(send_data_uart[1]); //����
				 app_uart_put(move_body);         //�嶯
				 app_uart_put(send_data_uart[3]); //�ڴ�״̬
				
				 send_data_uart[4] = send_data_uart[0] + send_data_uart[1] + move_body + send_data_uart[3];
				 app_uart_put(send_data_uart[4]); //У��
				 send_data_uart[4] = 0;         
				
				 app_uart_put(0xAA);         //֡β
				 printf("\r\n");
				GetSleepHrDataTimeCount=0;
			}
		}
	
/*****************************ָ���***********************************************/				
	if((SIMUART_Receive_Right == 1)&&(SIMUART_RX_LEN > 4)) //���Դ�������
	{
		SIMUART_Receive_Right = 0 ;
//		SIMUART_Receive_Order_Analys();
		UART_Receive_Analys();
	}
/****************************************************************************/	
	}//end while		
}