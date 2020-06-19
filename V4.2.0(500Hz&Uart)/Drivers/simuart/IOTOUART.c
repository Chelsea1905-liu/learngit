/***********************************************************************************
 * @file     IOTOUART.c
 * @brief    ʹ�õ�Ƭ��gpioģ�⴮��
 * @version  V1.0
 * @date     2017.11
 * @note
 * Copyright (C) 2017 ���
 *
 * @par   ʹ�õ�Ƭ����GPIOģ�⴮��   
************************************************************************************/
#include "string.h"
#include "IOTOUART.h"

uint16_t RX_Time=0;    //���ڽ���ʱ����ʱ

/****************************************************************************
*	�� �� ��: SIMUART_GPIO_CONFIG
*	����˵��: ģ�⴮��GPIO����
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/ 
void  SIMUART_GPIO_CONFIG(void) 
{
	ret_code_t err_code;
	
	 err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
	
  nrf_gpio_cfg_output(SIMUARTTX_PIN);   
//  nrf_gpio_cfg_input(SIMUARTRX_PIN,NRF_GPIO_PIN_NOPULL);
	
		nrf_drv_gpiote_in_config_t uartrx_in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);   //���ŵ�ƽ�½��ؽ������ж�
  uartrx_in_config.pull = NRF_GPIO_PIN_PULLUP;	
  err_code = nrf_drv_gpiote_in_init(SIMUARTRX_PIN, &uartrx_in_config, SIMUART_RX_handler);
  APP_ERROR_CHECK(err_code);
  nrf_drv_gpiote_in_event_enable(SIMUARTRX_PIN, true);
	
}
 /****************************************************************************
*	�� �� ��: SIMUART_RX_handler
*	����˵��: ģ�⴮�ڽ����ж�
*	��    �Σ�
*	�� �� ֵ: ��
*****************************************************************************/
void SIMUART_RX_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	 char buf[50];
	
	 if(action)
	{
		 if(!nrf_gpio_pin_read(SIMUARTRX_PIN)) //������ʼλ
			{		
					nrf_drv_gpiote_in_event_disable(SIMUARTRX_PIN);
				  UART_RX_BUFFER[UART_RX_LEN++]=SIMUART_RXByte();
				  nrf_drv_gpiote_in_event_enable(SIMUARTRX_PIN, true);
			
			}
		
	}
	
}
/****************************************************************************
*	�� �� ��: SIMUART_TXBYTE
*	����˵��: ����һ���ֽ�
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void SIMUART_TXByte(uint8_t dat)
{
	 uint8_t i;
	
	 UARTTXPIN_L();
	 nrf_delay_us(BPSTXDELAYTIME);
	
	 for(i=0;i<8;i++)
	{
		 if((dat>>i)&0x01)
		 {
			 UARTTXPIN_H();
		 }
		 else
		 {
			  UARTTXPIN_L();
		 }
		 nrf_delay_us(BPSTXDELAYTIME);
	}
	UARTTXPIN_H();
	nrf_delay_us(BPSTXDELAYTIME);
}
/****************************************************************************
*	�� �� ��: SIMUART_RXByte
*	����˵��: ����һ���ֽ�
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
uint8_t  SIMUART_RXByte(void)
{
	 uint8_t i;
	 uint8_t dat=0;

	 nrf_delay_us(BPSRXDELAYTIME-1);

	for(i=0;i<8;i++)
	{
		 dat>>=1;
		 if(nrf_gpio_pin_read(SIMUARTRX_PIN))
		 {
			 dat |= 0x80;
		 }    
		 nrf_delay_us(BPSRXDELAYTIME);
		 SIMUART_Delay(1);
	}

	while(1)
	{		
		if(nrf_gpio_pin_read(SIMUARTRX_PIN))
		{
			 return dat;
		}
		nrf_delay_us(1);
	}
	
}
/****************************************************************************
*	�� �� ��: SIMUART_TXBYTE
*	����˵��: �����ַ���
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void SIMUART_TXString(char *str)
{
	 while(*str !='\0')
	{
		SIMUART_TXByte(*str++);
	}
	
}
/****************************************************************************
*	�� �� ��: SIMUART_TXBUF
*	����˵��: ��������
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void SIMUART_TXBUF(char *buf,uint16_t len)
{
	uint16_t i;
	 for(i=0;i<len;i++)
	{
		SIMUART_TXByte(buf[i]);
	}
	
}
/****************************************************************************
*	�� �� ��: SIMUART_Delay
*	����˵��: ��ʱ
*	��    �Σ���
*	�� �� ֵ: ��
*****************************************************************************/
void SIMUART_Delay(uint16_t t)
{
	 uint16_t i,j;
	
	 for(i=0;i<t;i++)
	{
	   for(j=0;j<10;j++)
		{
			__nop();
		}
	}
	
}