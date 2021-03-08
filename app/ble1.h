#ifndef BLE1_H__
#define BLE1_H__


#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "app_timer.h"
#include "ble_nus.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp_btn_ble.h"
#include "nrf_pwr_mgmt.h"


#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"	
//#include "Image_583.h" 
#include "board_spi.h"
#include "spi_lib.h"
#include "spi_ble.h"
#include "spi_lib_75.h"
#include "alm_init_profile.h"
#include "mini_display.h"
#include "app_scheduler.h"




#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"
#include "nrf_queue.h"



extern uint8 simpleProfileChar1[ALMINITPROFILE_CHAR1_LEN] ;
extern uint8 simpleProfileChar2[ALMINITPROFILE_CHAR2_LEN] ;
extern uint8 simpleProfileChar3[ALMINITPROFILE_CHAR3_LEN] ;
extern uint8 simpleProfileChar4[ALMINITPROFILE_CHAR4_LEN] ;
extern  uint8 simpleProfileChar5[ALMINITPROFILE_CHAR5_LEN] ;
#define VOID (void)

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

#define DEVICE_NAME                     "TEST_52832"                               /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_DURATION                0//18000                                      /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(30, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(30, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */


void Alm_init1(void);

 void lfclk_config(void);
 void timers_init(void);//定时器初始化
 void gap_params_init(void);//GAP初始化
 void services_init(void);//服务初始化
 void conn_params_init(void);//连接参数初始化
 void sleep_mode_enter(void);//随眠模式进入
 void ble_stack_init(void);//协议栈初始化
void gatt_init(void);//GATT初始化
 void uart_init(void);
 void advertising_init(void);
 void buttons_leds_init(bool * p_erase_bonds);
 void log_init(void);
 void power_management_init(void);
 void advertising_start(void);
 void dcdc_on(void);
 void sys_off(void);
 void gpio_off(void);
 void idle_state_handle(void);


void ble_send(uint8*string,uint16 length);
void ble_send4(uint8*string,uint16 length);
void ble_nus_send(uint8*string,uint16 length);


void mac_set(void);


//定义函数的原型
//@CallbackFun 指向函数的指针类型
//@      回调参数，当有多个参数时，可以定义一个结构体
//@      回调上下文，在C中一般传入nullptr，在C++中可传入对象指针
typedef void (*CallbackFun)(uint32_t a0, uint32_t a1);
void registHeightCallback(CallbackFun callback, uint32_t a0, uint32_t a1);


typedef struct
{
    uint8_t *pData;
    uint8_t flag;
    
}custome_event;


 void ble_data_send_with_queue(void);

static void throughput_timer_handler(void * p_context);
void throughput_test(void);


extern int flag;

#endif /* BLE1_H__ */


