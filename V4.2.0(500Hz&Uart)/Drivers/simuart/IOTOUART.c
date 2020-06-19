/***********************************************************************************
 * @file     IOTOUART.c
 * @brief    使用单片机gpio模拟串口
 * @version  V1.0
 * @date     2017.11
 * @note
 * Copyright (C) 2017 张炜
 *
 * @par   使用单片机的GPIO模拟串口   
************************************************************************************/
#include "string.h"
#include "IOTOUART.h"

uint16_t RX_Time=0;    //串口接收时间延时

/****************************************************************************
*	函 数 名: SIMUART_GPIO_CONFIG
*	功能说明: 模拟串口GPIO配置
*	形    参：无
*	返 回 值: 无
*****************************************************************************/ 
void  SIMUART_GPIO_CONFIG(void) 
{
	ret_code_t err_code;
	
	 err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
	
  nrf_gpio_cfg_output(SIMUARTTX_PIN);   
//  nrf_gpio_cfg_input(SIMUARTRX_PIN,NRF_GPIO_PIN_NOPULL);
	
		nrf_drv_gpiote_in_config_t uartrx_in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);   //引脚电平下降沿将引起中断
  uartrx_in_config.pull = NRF_GPIO_PIN_PULLUP;	
  err_code = nrf_drv_gpiote_in_init(SIMUARTRX_PIN, &uartrx_in_config, SIMUART_RX_handler);
  APP_ERROR_CHECK(err_code);
  nrf_drv_gpiote_in_event_enable(SIMUARTRX_PIN, true);
	
}
 /****************************************************************************
*	函 数 名: SIMUART_RX_handler
*	功能说明: 模拟串口接收中断
*	形    参：
*	返 回 值: 无
*****************************************************************************/
void SIMUART_RX_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	 char buf[50];
	
	 if(action)
	{
		 if(!nrf_gpio_pin_read(SIMUARTRX_PIN)) //串口起始位
			{		
					nrf_drv_gpiote_in_event_disable(SIMUARTRX_PIN);
				  UART_RX_BUFFER[UART_RX_LEN++]=SIMUART_RXByte();
				  nrf_drv_gpiote_in_event_enable(SIMUARTRX_PIN, true);
			
			}
		
	}
	
}
/****************************************************************************
*	函 数 名: SIMUART_TXBYTE
*	功能说明: 发送一个字节
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: SIMUART_RXByte
*	功能说明: 接收一个字节
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: SIMUART_TXBYTE
*	功能说明: 发送字符串
*	形    参：无
*	返 回 值: 无
*****************************************************************************/
void SIMUART_TXString(char *str)
{
	 while(*str !='\0')
	{
		SIMUART_TXByte(*str++);
	}
	
}
/****************************************************************************
*	函 数 名: SIMUART_TXBUF
*	功能说明: 发送数组
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: SIMUART_Delay
*	功能说明: 延时
*	形    参：无
*	返 回 值: 无
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