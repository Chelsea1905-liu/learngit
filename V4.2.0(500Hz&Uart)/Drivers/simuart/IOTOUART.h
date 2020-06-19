/***********************************************************************************
 * @file     IOTOUART.h
 * @brief    ʹ�õ�Ƭ��gpioģ�⴮��
 * @version  V1.0
 * @date     2017.11
 * @note
 * Copyright (C) 2017 ���
 *
 * @par   ʹ�õ�Ƭ����GPIOģ�⴮��   
************************************************************************************/
#ifndef __IOTOUART_H__
#define __IOTOUART_H__

#include "boards.h"
#include "nrf.h"
#include "string.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"
#include "Parameter.h"
#include <stdio.h>



//#define   UART_BAUD_9600        9600        //ģ������Ĳ�����
#define   UART_BAUD_115200      115200

#ifdef   UART_BAUD_9600   
#define  BPSTXDELAYTIME    107
#define  BPSRXDELAYTIME    110
#elif defined  UART_BAUD_115200
#define  BPSTXDELAYTIME     9
#define  BPSRXDELAYTIME     7
#endif


#define UARTTXPIN_H()   nrf_gpio_pin_set(SIMUARTTX_PIN)
#define UARTTXPIN_L()   nrf_gpio_pin_clear(SIMUARTTX_PIN)


extern uint16_t RX_Time;    //���ڽ���ʱ����ʱ

void SIMUART_GPIO_CONFIG(void);
void SIMUART_TXByte(uint8_t dat);
void SIMUART_TXString(char *str);
void SIMUART_TXBUF(char *buf,uint16_t len);
void SIMUART_RX_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
uint8_t  SIMUART_RXByte(void);
void SIMUART_Delay(uint16_t t);



#endif