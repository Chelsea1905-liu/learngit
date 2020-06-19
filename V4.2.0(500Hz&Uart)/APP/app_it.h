/****************************************************************************
 * @file     app_it.H
 * @brief    中断黄色
 * @version  V1.0.0
 * @date     2017.9
 * @note
 * Copyright (C) 2017 张炜
 *
 * @par     NB-IOT模块
 *  
*****************************************************************************/
#ifndef __APP_IT_H__
#define __APP_IT_H__

#include <string.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf_drv_uart.h"
#include "nrf_drv_timer.h"
#include "app_timer.h"      //定时器
#include "app_gpio_in_out.h"
#include "Parameter.h"
#include "nrf_drv_wdt.h"

void Timer1_event_handler(nrf_timer_event_t event_type, void* p_context);
void Timer2_event_handler(nrf_timer_event_t event_type, void* p_context);
//void uart_event_handle(app_uart_evt_t * p_event);
void simuart_event_handle(app_uart_evt_t * p_event);
static void ADCdata_deal(void);
#endif