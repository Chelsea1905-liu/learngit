/****************************************************************************
 * @file     app_apio_in_out.h
 * @brief    芯片GPIO输入输出设置
 * @version  V1.0.0
 * @date     2017.9
 * @note
 * Copyright (C) 2017 张炜
 *
 * @par     NB-IOT模块
 *  
*****************************************************************************/
#ifndef __APP_GPIO_IN_OUT_H__ 
#define __APP_GPIO_IN_OUT_H__

#include "boards.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_saadc.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "app_pwm.h"
#include "nrf_drv_ppi.h"
#include "app_uart.h"
#include "nrf_drv_uart.h"
#include "Parameter.h"
#include "Fun.h"
#include "app_it.h"


#define ON   1
#define OFF  0

//#define ADC2_GAIN1_HIGH  nrf_gpio_pin_set(ADC2_GAIN1_PIN)
//#define ADC2_GAIN1_LOW  nrf_gpio_pin_clear(ADC2_GAIN1_PIN)
//#define ADC2_GAIN2_HIGH  nrf_gpio_pin_set(ADC2_GAIN2_PIN)
//#define ADC2_GAIN2_LOW  nrf_gpio_pin_clear(ADC2_GAIN2_PIN)


#define IO3_OFF  nrf_gpio_pin_set(IO3_PIN)
#define IO3_ON   nrf_gpio_pin_clear(IO3_PIN)
#define IO4_OFF  nrf_gpio_pin_set(IO4_PIN)
#define IO4_ON   nrf_gpio_pin_clear(IO4_PIN)



//-------------------------------指示灯-----------------------------------------------
#define LED1_OFF  nrf_gpio_pin_set(LED_1)
#define LED1_ON   nrf_gpio_pin_clear(LED_1)
#define LED1_INVERT do { uint32_t gpio_state = NRF_GPIO->OUT;      \
                              NRF_GPIO->OUTSET = ((1<<LED_1) & ~gpio_state); \
                              NRF_GPIO->OUTCLR = ((1<<LED_1) & gpio_state); } while (0)

//#define  WIFI_POWER(a) if (a)	\
					nrf_gpio_pin_set(WIFI_POWER_PIN);\
					else		\
					nrf_gpio_pin_clear(WIFI_POWER_PIN)

//-----------------------------------ADC---------------------------------------------
#define SAMPLES_IN_BUFFER      4
extern   nrf_saadc_value_t   adc_vdata[SAMPLES_IN_BUFFER];
					
extern float  ADC_VoltageValue[SAMPLES_IN_BUFFER];   //adc电压值
															
//-------------------------------函数声明-----------------------------------------------															
extern void ADC_Switch_CONFIG(void); 
void UART_CONFIG(uint8_t uart_type);					
void COMOrderRetuen(uint8_t *data,uint8_t len);
void SendDeviceVerInfoToUart(char *str);
void ADC_GPIO_CONFIG(void);
//void LED_Status(void);															
void ClearSIMUSARTBUF(void);															
void ClearBLEBUF(void);
void ClearUSARTBUF(void);
void Timer2_handler(nrf_timer_event_t event_type, void* p_context);															
void saadc_callback(nrf_drv_saadc_evt_t const * p_event);
char ReadBTNameFromFlash(void);
char SaveBTNameToFlash(void);
void ExtSensor_GPIO_CONFIG(void);
void OSC_GPIO_CONFIG(void);

#endif