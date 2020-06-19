/****************************************************************************
 * @file     app_it.c
 * @brief    �ж�
 * @version  
 * @date    
 * @note
 * 
 *
 * @par     NB-IOTģ��
 *  
*****************************************************************************/
#include "app_it.h"
#include "Fun.h"
#include "app_gpio_in_out.h"
#include "ring_buffer.h"
#include "Parameter.h"

/****************************************************************************
*	�� �� ��: timer1_event_handler
*	����˵��: ��ʱ���жϴ�����
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
void Timer1_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    
	char printfstr[50];

	switch(event_type)
	{
		case NRF_TIMER_EVENT_COMPARE0:
			TimerSim.Msec+=TIMEER1_SETTIME_MS;		
			//LedFlash	+=TIMEER1_SETTIME_MS;	
            ADC_CatchTimes+=TIMEER1_SETTIME_MS; //����ʱ�����
            ADC_ChangeTimes+=TIMEER1_SETTIME_MS;//����ʱ�����	
            GetSleepHrDataTimeCount+=TIMEER1_SETTIME_MS;
            GetSleepRrDataTimeCount +=TIMEER1_SETTIME_MS;
            GetBatteryDataTimeCount +=TIMEER1_SETTIME_MS;
            if(Flag_PowerOn == 0)
				OnbedStatus_CountTimer+=TIMEER1_SETTIME_MS;
            BatteryCheck_CountTimer+=TIMEER1_SETTIME_MS;        
		break;

		default:

		break;
	}  
    
    if(ADC_CatchTimes >= 2)   //lqs test
    {
        ADC_CatchTimes = 0;
        ADCdata_deal();
    }
//    if(ADC_ChangeTimes >= 10)
//    {
//        ADC_ChangeTimes = 0;
//        if(SleepMode_flag == 0)
//          CalculateHrRrData();
//    }

	if(TimerSim.Msec >= 1000)   //1sʱ�䵽
	{
		TimerSim.Msec = 0;
		TimerSim.Sec++;
        
		NoRespTime++;   //�޺���ֵ��ʱ
		NoHeartTime++;//������ֵ��ʱ
        WakeUpCount++;
        


		if(TimerSim.Sec == 60)
		{
			TimerSim.Sec = 0;
			TimerSim.Min++;
		}
		if(TimerSim.Min == 60)
		{
			TimerSim.Min = 0;
			TimerSim.Hour++;
		}	
		if(TimerSim.Hour == 24)
		{
			TimerSim.Hour = 0;
			TimerSim.Day++;
		}	
		
        

//        if(Status_ExtSensorOnbed == EXTSENSOR_PEOPLEOFFBED)
		    if((gPeopleFlag == EXTSENSOR_PEOPLEOFFBED)&&(DeviceMode == Mode_BlueTooth))   //lqs add
            sleep_mode_timer++;
        else
            sleep_mode_timer=0;  
      
        RenewRealTime();
		nrf_drv_wdt_channel_feed(m_channel_id);
        
		if(Flag_TimeAdj != 0)
		{
			Adj_Time++;
			if( Adj_Time == 60)
			{
				Adj_Time = 0;
				if(Flag_TimeAdj == 2)
					Flag_TimeAdj = 1;
				if(Flag_TimeAdj == 3)
					Flag_TimeAdj = 0; 					
			}			
		}		 

        

	}
}
/****************************************************************************
*	�� �� ��: Timer2_event_handler 
*	����˵��: ��ʱ��2�жϴ�����
*	��    �Σ���
*	�� �� ֵ: 
*   setup m_timer for compare event every 1ms 
*****************************************************************************/ 
void Timer2_event_handler(nrf_timer_event_t event_type, void* p_context)
{
	
	switch(event_type)
	{
		case NRF_TIMER_EVENT_COMPARE1:
			nrf_drv_saadc_buffer_convert(adc_vdata,SAMPLES_IN_BUFFER);
			nrf_drv_saadc_sample();
		break;

		default:

		break;
	}	
}
/****************************************************************************
*	�� �� ��: simuart_error_handle
*	����˵��: ģ�⴮���жϺ���
*	��    �Σ�app_uart_evt_t * p_event�ж��¼�
*	�� �� ֵ: ��
*****************************************************************************/ 
void simuart_event_handle(app_uart_evt_t * p_event)
{
    uint32_t       err_code; 
    uint8_t i =0;  
    switch (p_event->evt_type)          
    {			
        case APP_UART_DATA_READY:      //���ڽ�������
           app_uart_get(&SIMUART_RX_BUFFER[SIMUART_RX_LEN]);				
			SIMUART_Receive_Right = 1;
			SIMUART_RX_LEN++;
			if(SIMUART_RX_LEN > UARTBUF_MAX_LEN)
			{
				SIMUART_RX_LEN = 0;
			}
          break;

        case APP_UART_COMMUNICATION_ERROR:

            break;

        case APP_UART_FIFO_ERROR:

            break;

        default:
					
            break;
    }
}
/***************************************************************************************
*	�� �� ��: ADCdata_deal
*	����˵��: ADC �������ݴ���
*	��    �Σ���
*	�� �� ֵ: ��
* 	˵    ����
****************************************************************************************/

static void ADCdata_deal(void)
{
	uint8_t len = 0;
	static char IS_read_over=0;
    char readbuffer[30];
    uint32_t data_table[2]={0};
    uint8_t value1[20] = {0};
    char read_res_len=0;
    static uint16_t read_count=0;
    float val;
    uint32_t adc_breathe = adc_vdata[1];
    uint32_t adc_heart  = adc_vdata[2];
		
    if(adc_breathe>4096)
        adc_breathe=4096;
    
    if(adc_heart>4096)
        adc_heart=4096;

    Write_RingBuff(&ringBuff_ori,&adc_breathe, &adc_heart);
    writedata_timer++;
//	if(SimpleDataNum2>=NUM_ADCSAMBUF)
//	{
//		SimpleDataNum2 = NUM_ADCSAMBUF-1;
//	}	
//	if(SimpleDataNum1>=NUM_ADCSAMBUF)
//	{
//		SimpleDataNum1 = NUM_ADCSAMBUF-1;
//	}
}