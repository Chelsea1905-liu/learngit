/**************************************************************************//**
 * @file     boards.h
 * @brief    ��·�����Ŷ���
 * @version  
 * @date     
 * @note
 * 
 *
 * @par      NB-IOTģ��
 *   
 ******************************************************************************/
#ifndef __BOARDS_H
#define __BOARDS_H

#include "nrf_gpio.h"

//==============���Ŷ���===================================================
#define SIMUARTTX_PIN        23           //���ڷ���
#define SIMUARTRX_PIN        22           //���ڽ���
#define PIN_RESET            21
#define LED_1                30 
#define SLEEP_MODE_PIN       5

/* ����FLASH���ӵ�GPIO�˿�, ʹ��IOģ�ⷽʽ���� */
#define FLASH_SPI_SCK_PIN    28                 
#define FLASH_SPI_MISO_PIN   26                  
#define FLASH_SPI_MOSI_PIN   27                  
#define FLASH_CS_PIN         25

/*���Ʋɼ���������*/
#define IO3_PIN              12                  
#define IO4_PIN              13                  


//====================================BLE��ز�������=====================================================
/************************************************************
*  DEVICE_NAME������ʱ��ʾ���豸����
*  MANUFACTURE_NAME���������device informationģ������ĳ����������Ե�����
*  APP_ADV_INTERVAL���㲥���ڣ�������ع�ϵ������ʱ��͹��ģ���λ��0.625mS������Ϊ500mSʱ�򣬹���100uA������һ��2-3�����������豸
*  APP_ADV_TIMEOUT_IN_SECONDS������㲥��������ô��ʱ�仹û���Ӿͳ�ʱ
*  MIN_CONN_INTERVAL������֮������ͨ����Сʱ������1.25mSΪ��λ
*  MAX_CONN_INTERVAL������֮������ͨ�����ʱ������1.25mSΪ��λ
*                  ����ʱ�������ʹӻ���negotiation��������������ʱ�������ӻ����������Ϸ�Χ�ڵġ�
*  SLAVE_LATENCY���ӻ�����ѡ������һЩ����ʱ϶���Խ���ͨ��Ƶ�ʽ�ʡ���ģ�һ�㶼û��Ҫ...
*  CONN_SUP_TIMEOUT���������ʱ�������û���յ������ź�����Ϊ�����ж�
*****************************************************************/
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                           /**< Include the service_changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

#define CENTRAL_LINK_COUNT              0                                           /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           1                                           /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define DEVICE_NAME                     "i029"                                    /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME               "iBr"                                    /**< Manufacturer. Will be passed to Device Information Service. */
#define DEVICE_VER                      "_V1"                                    /**< Device version. Will be after to BlueTooth name */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_ADV_INTERVAL                4800                                     /**< The advertising interval (in units of 0.625 ms. This value corresponds to 300 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      0                                        /**< The advertising timeout (in units of seconds). */
//һ�㷢��ģʽ�£��㲥ʱ��Ϊ0��Ϊһֱ�㲥  180Ϊ���޷���ģʽ�¹㲥ʱ��180s

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(10, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (10 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Size of timer operation queues. */


//--------------------------------------------------------
// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC,            \
                                 .rc_ctiv       = 16,                                \
                                 .rc_temp_ctiv  = 2,                                \
                                 NULL}																					

//#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
//                                 .rc_ctiv       = 0,                                \
//                                 .rc_temp_ctiv  = 0,                                \
//                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}																		 
#endif
