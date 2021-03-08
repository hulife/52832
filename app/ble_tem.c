///** @file
// *
// * @defgroup ble_sdk_app_template_main main.c
// * @{
// * @ingroup ble_sdk_app_template
// * @brief Template project main file.
// *
// * This file contains a template for creating a new application. It has the code necessary to wakeup
// * from button, advertise, get a connection restart advertising on disconnect and if no new
// * connection created go back to system-off mode.
// * It can easily be used as a starting point for creating a new application, the comments identified
// * with 'YOUR_JOB' indicates where and how you can customize.
// */
//#include <ble_tem.h>

//#include <stdbool.h>
//#include <stdint.h>
//#include <string.h>

//#include "nordic_common.h"
//#include "nrf.h"
//#include "app_error.h"
//#include "ble.h"
//#include "ble_hci.h"
//#include "ble_srv_common.h"
//#include "ble_advdata.h"
//#include "ble_advertising.h"
//#include "ble_conn_params.h"
//#include "nrf_sdh.h"
//#include "nrf_sdh_soc.h"
//#include "nrf_sdh_ble.h"
//#include "app_timer.h"
//#include "fds.h"
//#include "peer_manager.h"
//#include "peer_manager_handler.h"
//#include "bsp_btn_ble.h"
//#include "sensorsim.h"
//#include "ble_conn_state.h"
//#include "nrf_ble_gatt.h"
//#include "nrf_ble_qwr.h"
//#include "nrf_pwr_mgmt.h"
////99
//#include "boards.h"
//#include "app_button.h"
//#include "ble_lbs.h"

//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
//#include "nrf_log_default_backends.h"

//#include "alm_init_profile.h"
////99
//#define ADVERTISING_LED                 BSP_BOARD_LED_0                        /**< Is on when device is advertising. */
//#define CONNECTED_LED                   BSP_BOARD_LED_1                        /**< Is on when device has connected. */
//#define LEDBUTTON_LED                   BSP_BOARD_LED_2                       /**< LED to be toggled with the help of the LED Button Service. */
//#define LEDBUTTON_BUTTON                BSP_BUTTON_0                            /**< Button that will trigger the notification event with the LED Button Service */
//#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50)                     /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */
//BLE_LBS_DEF(m_lbs);                                                             /**< LED Button Service instance. */



//#define DEVICE_NAME                     "Nordic_Template1"                       /**< Name of device. Will be included in the advertising data. */
//#define MANUFACTURER_NAME               "NordicSemiconductor"                   /**< Manufacturer. Will be passed to Device Information Service. */
//#define APP_ADV_INTERVAL                300                                     /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */

//#define APP_ADV_DURATION                18000                                   /**< The advertising duration (180 seconds) in units of 10 milliseconds. */
//#define APP_BLE_OBSERVER_PRIO           3                                       /**< Application's BLE observer priority. You shouldn't need to modify this value. */
//#define APP_BLE_CONN_CFG_TAG            1                                       /**< A tag identifying the SoftDevice BLE configuration. */

//#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)        /**< Minimum acceptable connection interval (0.1 seconds). */
//#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)        /**< Maximum acceptable connection interval (0.2 second). */
//#define SLAVE_LATENCY                   0                                       /**< Slave latency. */
//#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)         /**< Connection supervisory timeout (4 seconds). */

//#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                   /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
//#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                  /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
//#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                       /**< Number of attempts before giving up the connection parameter negotiation. */

//#define SEC_PARAM_BOND                  1                                       /**< Perform bonding. */
//#define SEC_PARAM_MITM                  0                                       /**< Man In The Middle protection not required. */
//#define SEC_PARAM_LESC                  0                                       /**< LE Secure Connections not enabled. */
//#define SEC_PARAM_KEYPRESS              0                                       /**< Keypress notifications not enabled. */
//#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                    /**< No I/O capabilities. */
//#define SEC_PARAM_OOB                   0                                       /**< Out Of Band data not available. */
//#define SEC_PARAM_MIN_KEY_SIZE          7                                       /**< Minimum encryption key size. */
//#define SEC_PARAM_MAX_KEY_SIZE          16                                      /**< Maximum encryption key size. */

//#define DEAD_BEEF                       0xDEADBEEF                              /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */


//NRF_BLE_GATT_DEF(m_gatt);                                                       /**< GATT module instance. */
//NRF_BLE_QWR_DEF(m_qwr);                                                         /**< Context for the Queued Write module.*/
//BLE_ADVERTISING_DEF(m_advertising);                                             /**< Advertising module instance. */

//static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                        /**< Handle of the current connection. */

///* YOUR_JOB: Declare all services structure your application is using
// *  BLE_XYZ_DEF(m_xyz);
// */
// //99
//static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;                   /**< Advertising handle used to identify an advertising set. */
//static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                    /**< Buffer for storing an encoded advertising set. */
//static uint8_t m_enc_scan_response_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX];         /**< Buffer for storing an encoded scan data. */

//// YOUR_JOB: Use UUIDs for service(s) used in your application.
//static ble_uuid_t m_adv_uuids[] =                                               /**< Universally unique service identifiers. */
//{
//    {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}
//};

////99


///**@brief Struct that contains pointers to the encoded advertising data. */
//static ble_gap_adv_data_t m_adv_data =
//{
//    .adv_data =
//    {
//        .p_data = m_enc_advdata,
//        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
//    },
//    .scan_rsp_data =
//    {
//        .p_data = m_enc_scan_response_data,
//        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX

//    }
//};

//static void advertising_start(bool erase_bonds);


///**@brief Callback function for asserts in the SoftDevice.
// *
// * @details This function will be called in case of an assert in the SoftDevice.
// *
// * @warning This handler is an example only and does not fit a final product. You need to analyze
// *          how your product is supposed to react in case of Assert.
// * @warning On assert from the SoftDevice, the system can only recover on reset.
// *
// * @param[in] line_num   Line number of the failing ASSERT call.
// * @param[in] file_name  File name of the failing ASSERT call.
// */

//static void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
//{
//    app_error_handler(DEAD_BEEF, line_num, p_file_name);
//}


////99
///**@brief Function for the LEDs initialization.
// *
// * @details Initializes all LEDs used by the application.
// */
//static void leds_init(void)
//{
//    bsp_board_init(BSP_INIT_LEDS);
//}


///**@brief Function for handling Peer Manager events.
// *
// * @param[in] p_evt  Peer Manager event.
// */
//static void pm_evt_handler(pm_evt_t const * p_evt)
//{
//    pm_handler_on_pm_evt(p_evt);
//    pm_handler_flash_clean(p_evt);

//    switch (p_evt->evt_id)
//    {
//        case PM_EVT_PEERS_DELETE_SUCCEEDED:
//            advertising_start(false);
//            break;

//        default:
//            break;
//    }
//}


///**@brief Function for the Timer initialization.
// *
// * @details Initializes the timer module. This creates and starts application timers.
// */
//static void timers_init(void)
//{
//    // Initialize timer module.
//    ret_code_t err_code = app_timer_init();
//    APP_ERROR_CHECK(err_code);

//    // Create timers.

//    /* YOUR_JOB: Create any timers to be used by the application.
//                 Below is an example of how to create a timer.
//                 For every new timer needed, increase the value of the macro APP_TIMER_MAX_TIMERS by
//                 one.
//       ret_code_t err_code;
//       err_code = app_timer_create(&m_app_timer_id, APP_TIMER_MODE_REPEATED, timer_timeout_handler);
//       APP_ERROR_CHECK(err_code); */
//}


///**@brief Function for the GAP initialization.
// *
// * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
// *          device including the device name, appearance, and the preferred connection parameters.
// */
//static void gap_params_init(void)
//{
//    ret_code_t              err_code;
//    ble_gap_conn_params_t   gap_conn_params;
//    ble_gap_conn_sec_mode_t sec_mode;

//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

//    err_code = sd_ble_gap_device_name_set(&sec_mode,
//                                          (const uint8_t *)DEVICE_NAME,
//                                          strlen(DEVICE_NAME));
//    APP_ERROR_CHECK(err_code);

//    /* YOUR_JOB: Use an appearance value matching the application's use case.
//       err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_);
//       APP_ERROR_CHECK(err_code); */

//    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

//    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
//    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
//    gap_conn_params.slave_latency     = SLAVE_LATENCY;
//    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

//    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
//    APP_ERROR_CHECK(err_code);
//}


///**@brief Function for initializing the GATT module.
// */
//static void gatt_init(void)
//{
//    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
//    APP_ERROR_CHECK(err_code);
//}

////99

///**@brief Function for handling write events to the LED characteristic.
// *
// * @param[in] p_lbs     Instance of LED Button Service to which the write applies.
// * @param[in] led_state Written/desired state of the LED.
// */
//static void led_write_handler(uint16_t conn_handle, ble_lbs_t * p_lbs, uint8_t led_state)
//{
//    if (led_state)
//    {
//        bsp_board_led_on(LEDBUTTON_LED);
//        NRF_LOG_INFO("Received LED ON!");
//    }
//    else
//    {
//        bsp_board_led_off(LEDBUTTON_LED);
//        NRF_LOG_INFO("Received LED OFF!");
//    }
//}



///**@brief Function for handling Queued Write Module errors.
// *
// * @details A pointer to this function will be passed to each service which may need to inform the
// *          application about an error.
// *
// * @param[in]   nrf_error   Error code containing information about what went wrong.
// */
//static void nrf_qwr_error_handler(uint32_t nrf_error)
//{
//    APP_ERROR_HANDLER(nrf_error);
//}


///**@brief Function for handling the YYY Service events.
// * YOUR_JOB implement a service handler function depending on the event the service you are using can generate
// *
// * @details This function will be called for all YY Service events which are passed to
// *          the application.
// *
// * @param[in]   p_yy_service   YY Service structure.
// * @param[in]   p_evt          Event received from the YY Service.
// *
// *
//static void on_yys_evt(ble_yy_service_t     * p_yy_service,
//                       ble_yy_service_evt_t * p_evt)
//{
//    switch (p_evt->evt_type)
//    {
//        case BLE_YY_NAME_EVT_WRITE:
//            APPL_LOG("[APPL]: charact written with value %s. ", p_evt->params.char_xx.value.p_str);
//            break;

//        default:
//            // No implementation needed.
//            break;
//    }
//}
//*/

///**@brief Function for initializing services that will be used by the application.
// */
//static void services_init(void)
//{
//    ret_code_t         err_code;
//    nrf_ble_qwr_init_t qwr_init = {0};
////99
////		ble_lbs_init_t     init     = {0};
//		
//    // Initialize Queued Write Module.
//    qwr_init.error_handler = nrf_qwr_error_handler;

//    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
//    APP_ERROR_CHECK(err_code);
////99
//		  // Initialize LBS.
////    init.led_write_handler = led_write_handler;

////    err_code = ble_lbs_init(&m_lbs, &init);
////    APP_ERROR_CHECK(err_code);
//		
////自定义服务		
//		#if NRF_MODULE_ENABLED(BLE_ALMINITPROFILE)
//	// Initialize ALM Init Service.
//		err_code = AlmInitProfile_AddService();
//		APP_ERROR_CHECK(err_code);
//		#endif
//    /* YOUR_JOB: Add code to initialize the services used by the application.
//       ble_xxs_init_t                     xxs_init;
//       ble_yys_init_t                     yys_init;

//       // Initialize XXX Service.
//       memset(&xxs_init, 0, sizeof(xxs_init));

//       xxs_init.evt_handler                = NULL;
//       xxs_init.is_xxx_notify_supported    = true;
//       xxs_init.ble_xx_initial_value.level = 100;

//       err_code = ble_bas_init(&m_xxs, &xxs_init);
//       APP_ERROR_CHECK(err_code);

//       // Initialize YYY Service.
//       memset(&yys_init, 0, sizeof(yys_init));
//       yys_init.evt_handler                  = on_yys_evt;
//       yys_init.ble_yy_initial_value.counter = 0;

//       err_code = ble_yy_service_init(&yys_init, &yy_init);
//       APP_ERROR_CHECK(err_code);
//     */
//}


///**@brief Function for handling the Connection Parameters Module.
// *
// * @details This function will be called for all events in the Connection Parameters Module which
// *          are passed to the application.
// *          @note All this function does is to disconnect. This could have been done by simply
// *                setting the disconnect_on_fail config parameter, but instead we use the event
// *                handler mechanism to demonstrate its use.
// *
// * @param[in] p_evt  Event received from the Connection Parameters Module.
// */
//static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
//{
//    ret_code_t err_code;

//    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
//    {
//        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
//        APP_ERROR_CHECK(err_code);
//    }
//}


///**@brief Function for handling a Connection Parameters error.
// *
// * @param[in] nrf_error  Error code containing information about what went wrong.
// */
//static void conn_params_error_handler(uint32_t nrf_error)
//{
//    APP_ERROR_HANDLER(nrf_error);
//}


///**@brief Function for initializing the Connection Parameters module.
// */
//static void conn_params_init(void)
//{
//    ret_code_t             err_code;
//    ble_conn_params_init_t cp_init;

//    memset(&cp_init, 0, sizeof(cp_init));

//    cp_init.p_conn_params                  = NULL;
//    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
//    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
//    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
//    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
//    cp_init.disconnect_on_fail             = false;
//    cp_init.evt_handler                    = on_conn_params_evt;
//    cp_init.error_handler                  = conn_params_error_handler;

//    err_code = ble_conn_params_init(&cp_init);
//    APP_ERROR_CHECK(err_code);
//}


///**@brief Function for starting timers.
// */
//static void application_timers_start(void)
//{
//    /* YOUR_JOB: Start your timers. below is an example of how to start a timer.
//       ret_code_t err_code;
//       err_code = app_timer_start(m_app_timer_id, TIMER_INTERVAL, NULL);
//       APP_ERROR_CHECK(err_code); */

//}


///**@brief Function for putting the chip into sleep mode.
// *
// * @note This function will not return.
// */
//static void sleep_mode_enter(void)
//{
//    ret_code_t err_code;

//    err_code = bsp_indication_set(BSP_INDICATE_IDLE);
//    APP_ERROR_CHECK(err_code);

//    // Prepare wakeup buttons.
//    err_code = bsp_btn_ble_sleep_mode_prepare();
//    APP_ERROR_CHECK(err_code);

//    // Go to system-off mode (this function will not return; wakeup will cause a reset).
//    err_code = sd_power_system_off();
//    APP_ERROR_CHECK(err_code);
//}


///**@brief Function for handling advertising events.
// *
// * @details This function will be called for advertising events which are passed to the application.
// *
// * @param[in] ble_adv_evt  Advertising event.
// */
//static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
//{
//    ret_code_t err_code;

//    switch (ble_adv_evt)
//    {
//        case BLE_ADV_EVT_FAST:
//            NRF_LOG_INFO("Fast advertising.");
//            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
//            APP_ERROR_CHECK(err_code);
//            break;

//        case BLE_ADV_EVT_IDLE:
//            sleep_mode_enter();
//            break;

//        default:
//            break;
//    }
//}


///**@brief Function for handling BLE events.
// *
// * @param[in]   p_ble_evt   Bluetooth stack event.
// * @param[in]   p_context   Unused.
// */
//static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
//{
//    ret_code_t err_code = NRF_SUCCESS;

//    switch (p_ble_evt->header.evt_id)
//    {
//        case BLE_GAP_EVT_DISCONNECTED:
//            NRF_LOG_INFO("Disconnected.");
//            //99
//				
////				bsp_board_led_off(CONNECTED_LED);
////            m_conn_handle = BLE_CONN_HANDLE_INVALID;
////            err_code = app_button_disable();
////            APP_ERROR_CHECK(err_code);
////            advertising_start(0);
//				// LED indication will be changed when advertising starts.
//            break;

//        case BLE_GAP_EVT_CONNECTED:
//            NRF_LOG_INFO("Connected.");
//            //99
////		 bsp_board_led_off(ADVERTISING_LED);
//				err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
//            APP_ERROR_CHECK(err_code);
//            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
//            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
//            APP_ERROR_CHECK(err_code);
//				
//				//99
//      //     APP_ERROR_CHECK(err_code);
//            break;

//        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
//        {
//            NRF_LOG_DEBUG("PHY update request.");
//            ble_gap_phys_t const phys =
//            {
//                .rx_phys = BLE_GAP_PHY_AUTO,
//                .tx_phys = BLE_GAP_PHY_AUTO,
//            };
//            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
//            APP_ERROR_CHECK(err_code);
//        } break;

//        case BLE_GATTC_EVT_TIMEOUT:
//            // Disconnect on GATT Client timeout event.
//            NRF_LOG_DEBUG("GATT Client Timeout.");
//            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
//                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
//            APP_ERROR_CHECK(err_code);
//            break;

//        case BLE_GATTS_EVT_TIMEOUT:
//            // Disconnect on GATT Server timeout event.
//            NRF_LOG_DEBUG("GATT Server Timeout.");
//            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
//                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
//            APP_ERROR_CHECK(err_code);
//            break;

//        default:
//            // No implementation needed.
//            break;
//    }
//}


///**@brief Function for initializing the BLE stack.
// *
// * @details Initializes the SoftDevice and the BLE event interrupt.
// */
//static void ble_stack_init(void)
//{
//    ret_code_t err_code;

//    err_code = nrf_sdh_enable_request();
//    APP_ERROR_CHECK(err_code);

//    // Configure the BLE stack using the default settings.
//    // Fetch the start address of the application RAM.
//    uint32_t ram_start = 0;
//    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
//    APP_ERROR_CHECK(err_code);

//    // Enable BLE stack.
//    err_code = nrf_sdh_ble_enable(&ram_start);
//    APP_ERROR_CHECK(err_code);

//    // Register a handler for BLE events.
//    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
//}


///**@brief Function for the Peer Manager initialization.
// */
//static void peer_manager_init(void)
//{
//    ble_gap_sec_params_t sec_param;
//    ret_code_t           err_code;

//    err_code = pm_init();
//    APP_ERROR_CHECK(err_code);

//    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

//    // Security parameters to be used for all security procedures.
//    sec_param.bond           = SEC_PARAM_BOND;
//    sec_param.mitm           = SEC_PARAM_MITM;
//    sec_param.lesc           = SEC_PARAM_LESC;
//    sec_param.keypress       = SEC_PARAM_KEYPRESS;
//    sec_param.io_caps        = SEC_PARAM_IO_CAPABILITIES;
//    sec_param.oob            = SEC_PARAM_OOB;
//    sec_param.min_key_size   = SEC_PARAM_MIN_KEY_SIZE;
//    sec_param.max_key_size   = SEC_PARAM_MAX_KEY_SIZE;
//    sec_param.kdist_own.enc  = 1;
//    sec_param.kdist_own.id   = 1;
//    sec_param.kdist_peer.enc = 1;
//    sec_param.kdist_peer.id  = 1;

//    err_code = pm_sec_params_set(&sec_param);
//    APP_ERROR_CHECK(err_code);

//    err_code = pm_register(pm_evt_handler);
//    APP_ERROR_CHECK(err_code);
//}


///**@brief Clear bond information from persistent storage.
// */
//static void delete_bonds(void)
//{
//    ret_code_t err_code;

//    NRF_LOG_INFO("Erase bonds!");

//    err_code = pm_peers_delete();
//    APP_ERROR_CHECK(err_code);
//}


///**@brief Function for handling events from the BSP module.
// *
// * @param[in]   event   Event generated when button is pressed.
// */
//static void bsp_event_handler(bsp_event_t event)
//{
//    ret_code_t err_code;

//    switch (event)
//    {
//        case BSP_EVENT_SLEEP:
//            sleep_mode_enter();
//            break; // BSP_EVENT_SLEEP

//        case BSP_EVENT_DISCONNECT:
//            err_code = sd_ble_gap_disconnect(m_conn_handle,
//                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
//            if (err_code != NRF_ERROR_INVALID_STATE)
//            {
//                APP_ERROR_CHECK(err_code);
//            }
//            break; // BSP_EVENT_DISCONNECT

//        case BSP_EVENT_WHITELIST_OFF:
//            if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
//            {
//                err_code = ble_advertising_restart_without_whitelist(&m_advertising);
//                if (err_code != NRF_ERROR_INVALID_STATE)
//                {
//                    APP_ERROR_CHECK(err_code);
//                }
//            }
//            break; // BSP_EVENT_KEY_0

//        default:
//            break;
//    }
//}


///**@brief Function for initializing the Advertising functionality.
// */
//static void advertising_init(void)
//{
//    ret_code_t             err_code;
//	
//	ble_advertising_init_t init;

//    memset(&init, 0, sizeof(init));

//    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
//    init.advdata.include_appearance      = true;
//    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
//    init.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
//    init.advdata.uuids_complete.p_uuids  = m_adv_uuids;

//    init.config.ble_adv_fast_enabled  = true;
//    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
//    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;

//    init.evt_handler = on_adv_evt;

//    err_code = ble_advertising_init(&m_advertising, &init);
//    APP_ERROR_CHECK(err_code);

////99
//// ble_gap_adv_params_t adv_params;

//    // Set advertising parameters.
////    memset(&adv_params, 0, sizeof(adv_params));

////    adv_params.primary_phy     = BLE_GAP_PHY_1MBPS;
////    adv_params.duration        = APP_ADV_DURATION;
////    adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
////    adv_params.p_peer_addr     = NULL;
////    adv_params.filter_policy   = BLE_GAP_ADV_FP_ANY;
////    adv_params.interval        = APP_ADV_INTERVAL;

////    err_code = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &adv_params);
////    APP_ERROR_CHECK(err_code);

//    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
//}


///**@brief Function for initializing buttons and leds.
// *
// * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
// */
//static void buttons_leds_init(bool * p_erase_bonds)
//{
//    ret_code_t err_code;
//    bsp_event_t startup_event;

//    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
//    APP_ERROR_CHECK(err_code);

//    err_code = bsp_btn_ble_init(NULL, &startup_event);
//    APP_ERROR_CHECK(err_code);

//    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
//}


///**@brief Function for handling events from the button handler module.
// *
// * @param[in] pin_no        The pin that the event applies to.
// * @param[in] button_action The button action (press/release).
// */
//static void button_event_handler(uint8_t pin_no, uint8_t button_action)
//{
//    ret_code_t err_code;

//    switch (pin_no)
//    {
//        case LEDBUTTON_BUTTON:
//            NRF_LOG_INFO("Send button state change.");
//            err_code = ble_lbs_on_button_change(m_conn_handle, &m_lbs, button_action);
//            if (err_code != NRF_SUCCESS &&
//                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
//                err_code != NRF_ERROR_INVALID_STATE &&
//                err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
//            {
//                APP_ERROR_CHECK(err_code);
//            }
//            break;

//        default:
//            APP_ERROR_HANDLER(pin_no);
//            break;
//    }
//}


///**@brief Function for initializing the button handler module.
// */
//static void buttons_init(void)
//{
//    ret_code_t err_code;

//    //The array must be static because a pointer to it will be saved in the button handler module.
//    static app_button_cfg_t buttons[] =
//    {
//        {LEDBUTTON_BUTTON, false, BUTTON_PULL, button_event_handler}
//    };

//    err_code = app_button_init(buttons, ARRAY_SIZE(buttons),
//                               BUTTON_DETECTION_DELAY);
//    APP_ERROR_CHECK(err_code);
//}



///**@brief Function for initializing the nrf log module.
// */
//static void log_init(void)
//{
//    ret_code_t err_code = NRF_LOG_INIT(NULL);
//    APP_ERROR_CHECK(err_code);

//    NRF_LOG_DEFAULT_BACKENDS_INIT();
//}


///**@brief Function for initializing power management.
// */
//static void power_management_init(void)
//{
//    ret_code_t err_code;
//    err_code = nrf_pwr_mgmt_init();
//    APP_ERROR_CHECK(err_code);
//}


///**@brief Function for handling the idle state (main loop).
// *
// * @details If there is no pending log operation, then sleep until next the next event occurs.
// */
//static void idle_state_handle(void)
//{
//    if (NRF_LOG_PROCESS() == false)
//    {
//        nrf_pwr_mgmt_run();
//    }
//}


///**@brief Function for starting advertising.
// */
//static void advertising_start(bool erase_bonds)
//{
//    if (erase_bonds == true)
//    {
//        delete_bonds();
//        // Advertising is started by PM_EVT_PEERS_DELETED_SUCEEDED event
//    }
//    else
//    {
//        ret_code_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);

//        APP_ERROR_CHECK(err_code);
//			
//			//99
////			bsp_board_led_on(ADVERTISING_LED);
//    }
//}



///**
// @brief 自定义初始化服务处理特征值函数
// @param connHandle -[in] 连接句柄
// @param charId -[in] 特征值ID
// @param pAlmInitProfile -[in] ALM初始化服务结构体
// @param pCharValue -[in] 写入特征值
// @param length -[in] 写入特征长度
// @return 无
//*/
//void AlmInitProfile_HandleCharValue(uint16 connHandle, uint8 charId, AlmInitProfile_t *pAlmInitProfile,
//									const uint8 *pCharValue, uint16 length)
//{
//	switch(charId)
//    {
//	case ALMINITPROFILE_CHAR1:																							// 通知功能 
//		AlmInitProfile_PushNotifyData(m_conn_handle, pAlmInitProfile, (uint8 *)pCharValue, ALMINITPROFILE_CHAR1_LEN);	// 通知
//		break;
//    case ALMINITPROFILE_CHAR2:																							
//		NRF_LOG_INFO("ss2");
//		break;
//	case ALMINITPROFILE_CHAR3: 																						
//		AlmInitProfile_PushNotifyData(m_conn_handle, pAlmInitProfile, (uint8 *)pCharValue, ALMINITPROFILE_CHAR3_LEN);	// 通知
//		break;
//	case ALMINITPROFILE_CHAR4: 																							
//		break;
//	default:
//		break;
//	}
//}


///**@brief Function for application main entry.
// */
//void ble_main(void)
//{
//    bool erase_bonds;

//    // Initialize.
//    log_init();			//log打印初始化
//    timers_init();	//定时器初始化

//    buttons_leds_init(&erase_bonds);//按键和LED初始化
//		//99
////		leds_init();
////    buttons_init();
//	
//    power_management_init(); //电源管理初始化
//    ble_stack_init();//BLE协议栈初始化
//    gap_params_init();//GAP参数初始化
//    gatt_init();//GATT初始化
//    advertising_init();//广播初始化
//    services_init();//服务初始化
//    conn_params_init();//连接参数初始化
//    peer_manager_init();//设备管理初始化

//    // Start execution.
//    NRF_LOG_INFO("Template example started.");
//    application_timers_start();//应用定时器启动

//    advertising_start(erase_bonds); //广播开始


//    // Enter main loop.
//    for (;;)
//    {
//        idle_state_handle();
//    }
//}


///**
// * @}
// */
