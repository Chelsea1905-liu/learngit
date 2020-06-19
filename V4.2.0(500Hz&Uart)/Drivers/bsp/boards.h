/**************************************************************************//**
 * @file     boards.h
 * @brief    电路板引脚定义
 * @version  
 * @date     
 * @note
 * 
 *
 * @par      NB-IOT模块
 *   
 ******************************************************************************/
#ifndef __BOARDS_H
#define __BOARDS_H

#include "nrf_gpio.h"

//==============引脚定义===================================================
#define SIMUARTTX_PIN        23           //串口发送
#define SIMUARTRX_PIN        22           //串口接收
#define PIN_RESET            21
#define LED_1                30 
#define SLEEP_MODE_PIN       5

/* 定义FLASH连接的GPIO端口, 使用IO模拟方式控制 */
#define FLASH_SPI_SCK_PIN    28                 
#define FLASH_SPI_MISO_PIN   26                  
#define FLASH_SPI_MOSI_PIN   27                  
#define FLASH_CS_PIN         25

/*控制采集开关引脚*/
#define IO3_PIN              12                  
#define IO4_PIN              13                  


//====================================BLE相关参数定义=====================================================
/************************************************************
*  DEVICE_NAME：搜索时显示的设备名称
*  MANUFACTURE_NAME：这个是在device information模块里面的厂商名称特性的内容
*  APP_ADV_INTERVAL：广播周期，这个严重关系到搜索时间和功耗，单位是0.625mS。设置为500mS时候，功耗100uA不到，一般2-3就能搜索到设备
*  APP_ADV_TIMEOUT_IN_SECONDS：如果广播开启后这么长时间还没连接就超时
*  MIN_CONN_INTERVAL：连接之后，两次通信最小时间间隔，1.25mS为单位
*  MAX_CONN_INTERVAL：连接之后，两次通信最大时间间隔，1.25mS为单位
*                  连接时候，主机和从机会negotiation来决定具体连接时间间隔，从机至接收以上范围内的。
*  SLAVE_LATENCY：从机可以选择跳过一些传输时隙，以降低通信频率节省功耗，一般都没必要...
*  CONN_SUP_TIMEOUT：超过这个时间如果还没有收到主机信号则认为连接中断
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
//一般发现模式下，广播时间为0则为一直广播  180为有限发现模式下广播时间180s

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
