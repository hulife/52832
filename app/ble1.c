
/** @file
 *
 * @defgroup ble_sdk_uart_over_ble_main main.c
 * @{
 * @ingroup  ble_sdk_app_nus_eval
 * @brief    UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service.
 * This application uses the @ref srvlib_conn_params module.
 */

#include "ble1.h"

//#define APP_QUEUE
void ble_data_send_with_queue(void);
typedef struct {
    uint8_t * p_data;
    uint16_t length;
} buffer_t;

NRF_QUEUE_DEF(buffer_t, m_buf_queue, 30, NRF_QUEUE_MODE_NO_OVERFLOW);

APP_TIMER_DEF(m_timer_speed);
uint8_t m_data_array[6300];
uint32_t m_len_sent;
uint32_t m_cnt_7ms;

uint16_t   BLE_ALM_MAX_DATA_LEN5=BLE_GATT_ATT_MTU_DEFAULT;

 uint16_t   m_conn_handle  = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
 uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */



static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
static uint8_t states[4];
static uint8_t index = 0;
static uint8_t change_state=0;

 uint8 simpleProfileChar1[ALMINITPROFILE_CHAR1_LEN]={0,} ;;
 uint8 simpleProfileChar2[ALMINITPROFILE_CHAR2_LEN]={0} ;
 uint8 simpleProfileChar3[ALMINITPROFILE_CHAR3_LEN]={0} ;
 uint8 simpleProfileChar4[ALMINITPROFILE_CHAR4_LEN]={0} ;
 uint8 simpleProfileChar5[ALMINITPROFILE_CHAR5_LEN]={0} ;

BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                                                 /**< Advertising module instance. */
BLE_ALMINITPROFILE_DEF(m_almInitProfile);
 
 
 ble_uuid_t m_adv_uuids[]          =                                          /**< Universally unique service identifier. */
{
    {BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}
};


/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}
//定义APP定时器m_beep_timer_id（
APP_TIMER_DEF(m_beep_timer_id);                                  

//LED定时器超时时间：500ms
#define BEEP_INTERVAL         APP_TIMER_TICKS(500) 
/**@brief Function for initializing the timer module.
 */
//配置低频时钟，时钟源设置的是外部32.768KHz晶体
 void lfclk_config(void)
{
    //初始化时钟模块，设置低频时钟源
	  ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    //请求低频时钟，输入参数为NULL表示低频时钟启动后不产生事件
    nrf_drv_clock_lfclk_request(NULL);
}

//蜂鸣器定时器超时事件回调函数
 void beep_timeout_handler(void * p_context)
{
    //防止编译器警告，同时清晰地的表明p_context未使用，而不是应用程序忽略了。如果应用
	//程序需要使用p_context，则不需要这行代码
	UNUSED_PARAMETER(p_context);
	  //定时器超时后，
  //  nrf_gpio_pin_clear(LED_2 ); 
	 black_a();	
}


 void timers_init(void)//定时器初始化
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
	//创建APP定时器：单次定时器，启动后只运行一次，
	  err_code = app_timer_create(&m_beep_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                beep_timeout_handler);
	  //检查错误代码
	  APP_ERROR_CHECK(err_code);
	
}

/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
 void gap_params_init(void)//GAP初始化
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

	
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)//队列写错误操作
{
    APP_ERROR_HANDLER(nrf_error);
}




/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_evt       Nordic UART Service event.
 */
/**@snippet [Handling the data received over BLE] */
int flag;

#define block 160
int a=0;
int count=0;
uint16_t leng=0;
uint8_t buff_s[block];
uint8_t* message=buff_s;
const unsigned char *image1=gImage_7in5_V2_b;
#define OK    "OK"
static void nus_data_handler(ble_nus_evt_t * p_evt)//串口中断操作
{
	
 if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
				flag =6;
        uint32_t err_code;
				//pingmu_Init();
			 	 count++;
				NRF_LOG_INFO("length==%d",p_evt->params.rx_data.length);
				NRF_LOG_INFO("count==%d",count);
        NRF_LOG_DEBUG("Received data from BLE NUS. Writing data on UART.");
        NRF_LOG_HEXDUMP_DEBUG(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
				
				ble_nus_send((uint8 *)OK, 3);
			if(p_evt->params.rx_data.length==160)
			{
	

	 memcpy(message, p_evt->params.rx_data.p_data, block);
		if(count<=300)
		{				
   // message+=160;
		leng=160;	
		
		}	
		BOE0583CC26XX_writeBlock(message, leng);
		
		memset(message,0,leng);
	//	Epaper_Write_Command(0x13);		
  // BOE0583CC26XX_writeBlock(message, leng);		
		if(count>=300)
		{		
		EPD_7IN5B_V2_TurnOnDisplay();
		
		count=0;
		}

		
				
				
/*
				memset(simpleProfileChar5, 0, sizeof(simpleProfileChar5));
				memcpy( simpleProfileChar5,p_evt->params.rx_data.p_data,p_evt->params.rx_data.length );
				
				
				memset(epd_display_itp_buffer, 0, sizeof(epd_display_itp_buffer));	
				EpdDisplay_doReceive((uint8 *)p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);

							
				registHeightCallback(EpdDisplay_taskFxn,NULL,NULL);
				
*/	
	
/*
        for (uint32_t i = 0; i < p_evt->params.rx_data.length; i++)
        {
            do
            {
							
                err_code = app_uart_put(p_evt->params.rx_data.p_data[i]);						
                if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_BUSY))
                {
								
                    NRF_LOG_ERROR("Failed receiving NUS message. Error 0x%x. ", err_code);
                    APP_ERROR_CHECK(err_code);
                }
						
							} while (err_code == NRF_ERROR_BUSY);
						
						}
			
	*/					
        }
        if (p_evt->params.rx_data.p_data[p_evt->params.rx_data.length - 1] == 'k')
        {
					
            while (app_uart_put('\n') == NRF_ERROR_BUSY);
        }
    }
		
		else if (p_evt->type == BLE_NUS_EVT_COMM_STARTED)
		{
//			uint16_t length;	
//			length = m_ble_nus_max_data_len;
			NRF_LOG_INFO("notisfy is enable\r\n");

			
		//	ble_nus_data_send1(&m_nus, m_data_array, &length, m_conn_handle);
			
	//		APP_ERROR_CHECK(app_timer_start(m_timer_speed, APP_TIMER_TICKS(7),NULL));			
		}
//		else if (p_evt->type == BLE_NUS_EVT_TX_RDY)
		{
#define APP_QUEUE		
			
#ifndef APP_QUEUE	//测速专用		
			ret_code_t err_code;
			uint16_t length;	
			
			//sending code lines
			length = m_ble_nus_max_data_len;	
			do
			{					
				err_code = ble_nus_data_send1(&m_nus, m_data_array, &length, m_conn_handle);
				if ( (err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_ERROR_RESOURCES) &&
						 (err_code != NRF_ERROR_NOT_FOUND) )
				{
						APP_ERROR_CHECK(err_code);
				}
				if (err_code == NRF_SUCCESS)
				{
					m_len_sent += length; 	
					m_data_array[0]++;
					m_data_array[length-1]++;	
				}
			} while (err_code == NRF_SUCCESS);
#else
#define APP_DATA	
#ifndef APP_DATA	
     ble_data_send_with_queue();
#endif
#endif
		}

}
/**@snippet [Handling the data received over BLE] */


/**@brief Function for initializing services that will be used by the application.
 */


void ble_send(uint8*string,uint16 length)//发送封装
{
	uint32 err_code;
	
	err_code=	AlmInitProfile_PushNotifyData(m_conn_handle, &m_almInitProfile,string ,&length);	//(uint8 *)ITP_PACKET_REPLY_MSG_CMD_UNKNOWN

	if (err_code !=NRF_ERROR_INVALID_STATE)
		
	{
		NRF_LOG_INFO("err_code==%d\r\n",err_code);
		APP_ERROR_CHECK(err_code);
		
	}

}


void ble_send4(uint8*string,uint16 length)//发送封装
{
	uint32 err_code;
	
	err_code=	AlmInitProfile_PushNotifyData4(m_conn_handle, &m_almInitProfile,string ,&length);	//(uint8 *)ITP_PACKET_REPLY_MSG_CMD_UNKNOWN

	if (err_code !=NRF_ERROR_INVALID_STATE)
		
	{
		APP_ERROR_CHECK(err_code);
		
	}

}




	void ble_nus_send(uint8*string,uint16 length)//发送封装
{
	uint32 err_code;
	
	err_code=	ble_nus_data_send1(&m_nus, string, &length, m_conn_handle);
	if (err_code !=NRF_ERROR_INVALID_STATE)
		
	{
		APP_ERROR_CHECK(err_code);
		
	}

}










/**
 @brief 自定义初始化服务处理特征值函数
 @param connHandle -[in] 连接句柄
 @param charId -[in] 特征值ID
 @param pAlmInitProfile -[in] ALM初始化服务结构体
 @param pCharValue -[in] 写入特征值
 @param length -[in] 写入特征长度
 @return 无
*/
uint8 data[]={0};

uint16 i;
void AlmInitProfile_HandleCharValue(uint16 connHandle, uint8 charId, AlmInitProfile_t *pAlmInitProfile,
											const uint8 *pCharValue, uint16 length)
{

	m_conn_handle=connHandle;
	NRF_LOG_INFO("connHandle==%d\r\n",connHandle);
	NRF_LOG_INFO("pAlmInitProfile->handle==%d\r\n",pAlmInitProfile->char5Handle.value_handle);
	
	ret_code_t err_code;
		
	switch(charId)
    {
				
	case ALMINITPROFILE_CHAR1:																							// 通知功能 

//		if(*pCharValue!=NULL)	
//			AlmInitProfile_PushNotifyData(m_conn_handle, pAlmInitProfile, (uint8 *)str, ALMINITPROFILE_CHAR1_LEN);	//

	memset(simpleProfileChar1, 0, sizeof(simpleProfileChar1));
	memcpy( simpleProfileChar1,pCharValue,  ALMINITPROFILE_CHAR1_LEN );
	memset(epd_display_itp_buffer, 0, sizeof(epd_display_itp_buffer));
	
	EpdDisplay_doReceive((uint8 *)pCharValue, ALMINITPROFILE_CHAR1_LEN);
	
	registHeightCallback(EpdDisplay_taskFxn,NULL,NULL);
	
		break;
   case ALMINITPROFILE_CHAR2:	
	
		pingmu_Init();

		black_a();	
		
	break;
	case ALMINITPROFILE_CHAR3: 																						
		
	//启动定时器
  err_code = app_timer_start(m_beep_timer_id, BEEP_INTERVAL, NULL);
  APP_ERROR_CHECK(err_code);
	break;
	case ALMINITPROFILE_CHAR4: 
//pingmu_Init();
	flag=4;
	if(*pCharValue!=NULL)

	NRF_LOG_INFO("length4==%d",length);
		memset(simpleProfileChar4, 0, sizeof(simpleProfileChar4));
	memcpy( simpleProfileChar4,pCharValue,  ALMINITPROFILE_CHAR4_LEN );
printf("simpleProfileChar5[0]=%d\r\n",simpleProfileChar5[0]);//
	memset(epd_display_itp_buffer, 0, sizeof(epd_display_itp_buffer));
	
	EpdDisplay_doReceive((uint8 *)pCharValue, ALMINITPROFILE_CHAR4_LEN);
printf("epd_display_itp_buffer=%d\r\n",epd_display_itp_buffer[0]);

	registHeightCallback(EpdDisplay_taskFxn,NULL,NULL);
	
		
//flag=1;
	
	
//	ble_send4((uint8 *)epd_display_itp_buffer,20);

	
		break;
	
	case ALMINITPROFILE_CHAR5: 
		//uint8_t  *pCharValue = malloc(ALMINITPROFILE_CHAR5_LEN);
	
	if(*pCharValue!=NULL)
//pingmu_Init();	
	flag=5;
	NRF_LOG_INFO("length5==%d",length);
	
	printf("pCharValue=%d\r\n",pCharValue[0]);
		memset(simpleProfileChar5, 0, sizeof(simpleProfileChar5));
	memcpy( simpleProfileChar5,pCharValue,  ALMINITPROFILE_CHAR5_LEN );
	
	printf("simpleProfileChar5[0]=%d\r\n",simpleProfileChar5[0]);//0	
	
	memset(epd_display_itp_buffer, 0, sizeof(epd_display_itp_buffer));
	
	EpdDisplay_doReceive((uint8 *)pCharValue, ALMINITPROFILE_CHAR5_LEN);
	printf("epd_display_itp_buffer=%d\r\n",epd_display_itp_buffer[0]);

	registHeightCallback(EpdDisplay_taskFxn,NULL,NULL);

//	ble_send((uint8 *)epd_display_itp_reply,20);
	
	
//	flag=1;
	


	
		break;
	default:
		break;
	}
}








/**@snippet [Handling the data received over BLE] */


/**@brief Function for initializing services that will be used by the application.
 */

AlmInitProfileCallback_t  Alm_init = {0};

 void services_init(void)//服务初始化
{
    uint32_t           err_code;
    ble_nus_init_t     nus_init;
    nrf_ble_qwr_init_t qwr_init = {0};
		
		
    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;

    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
		// Initialize alm.
//		#if NRF_MODULE_ENABLED(BLE_ALMINITPROFILE)
		
//		Alm_init.almInitProfileCharWriteHandler = AlmInitProfile_HandleCharValue;	// 调用外部函数：应用层处理特征值

	// Initialize ALM Init Service.
//	err_code = AlmInitProfile_RegisterAppCallback(&m_almInitProfile, &Alm_init);
	
//	APP_ERROR_CHECK(err_code);
//	#endif
}

void Alm_init1(void)
	{
		
		memset(&Alm_init, 0, sizeof(Alm_init));
		
	Alm_init.almInitProfileCharWriteHandler = AlmInitProfile_HandleCharValue;	
	}	


/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)//连接参数事件
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)//连接参数错误
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
 void conn_params_init(void)//连接参数初始化
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
 void sleep_mode_enter(void)//随眠模式进入
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)//广播事件
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_ADV_EVT_IDLE:
            sleep_mode_enter();
            break;
        default:
            break;
    }
}


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)//蓝牙处理事件
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected");
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected");
            // LED indication will be changed when advertising starts.
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
 void ble_stack_init(void)//协议栈初始化
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


/**@brief Function for handling events from the GATT library. */
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)//GATT事件处理函数const
{
	
		//nrf_ble_gatt_eff_mtu_get(&m_gatt, p_evt->conn_handle);
	 //NRF_LOG_INFO("att_mtu_effective len is set to 0x%X(%d)", p_evt->params.att_mtu_effective, p_evt->params.att_mtu_effective);
    if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
		
			BLE_ALM_MAX_DATA_LEN5 = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
        NRF_LOG_INFO("Data len is set to 0x%X(%d)", BLE_ALM_MAX_DATA_LEN5, BLE_ALM_MAX_DATA_LEN5);
    }
    
}


/**@brief Function for initializing the GATT library. */
void gatt_init(void)//GATT初始化
{
    ret_code_t err_code;

    err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);
	
	
//	err_code = nrf_ble_gatt_att_mtu_central_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
//    APP_ERROR_CHECK(err_code);
//	
    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
void bsp_event_handler(bsp_event_t event)//板级处理事件
{
    uint32_t err_code;
    switch (event)
    {
        case BSP_EVENT_SLEEP:
            sleep_mode_enter();
            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
            {
                err_code = ble_advertising_restart_without_whitelist(&m_advertising);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;

        default:
            break;
    }
}


/**@brief   Function for handling app_uart events.
 *
 * @details This function will receive a single character from the app_uart module and append it to
 *          a string. The string will be be sent over BLE when the last character received was a
 *          'new line' '\n' (hex 0x0A) or if the string has reached the maximum data length.
 */
/**@snippet [Handling the data received over UART] */
void uart_event_handle(app_uart_evt_t * p_event)
{
    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t       err_code;

    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
            index++;

            if ((data_array[index - 1] == '\n') || (index >= (m_ble_nus_max_data_len)))
            {
                NRF_LOG_DEBUG("Ready to send data over BLE NUS");
                NRF_LOG_HEXDUMP_DEBUG(data_array, index);

                do
                {
                    uint16_t length = (uint16_t)index;
									
                  //  err_code = ble_nus_data_send(&m_nus, data_array, &length, m_conn_handle);
                    
                    err_code = ble_nus_data_send1(&m_nus, data_array, &length, m_conn_handle);
                    if ( (err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_ERROR_BUSY) &&
                         (err_code != NRF_ERROR_NOT_FOUND) )
                    {
                        APP_ERROR_CHECK(err_code);
                    }
                } while (err_code == NRF_ERROR_BUSY);

                index = 0;
            }
            break;

        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}
/**@snippet [Handling the data received over UART] */


/**@brief  Function for initializing the UART module.
 */
/**@snippet [UART Initialization] */
 void uart_init(void)
{
    uint32_t                     err_code;
    app_uart_comm_params_t const comm_params =
    {
        .rx_pin_no    = RX_PIN_NUMBER,
        .tx_pin_no    = TX_PIN_NUMBER,
        .rts_pin_no   = RTS_PIN_NUMBER,
        .cts_pin_no   = CTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity   = false,
#if defined (UART_PRESENT)
        .baud_rate    = NRF_UART_BAUDRATE_115200
#else
        .baud_rate    = NRF_UARTE_BAUDRATE_115200
#endif
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);
    APP_ERROR_CHECK(err_code);
}
/**@snippet [UART Initialization] */
void mac_set(void)
{
  ble_gap_addr_t addr;
 // uint32_t err_code;
//	
	
//	uint8_t myaddr[BLE_GAP_ADDR_LEN]={0xc0,0x15,0x85,0xbc,0xd2,0x6a};
//	ble_gap_addr_t*p_mac_addr_t=(ble_gap_addr_t*)malloc(sizeof(ble_gap_addr_t));
//	err_code=sd_ble_gap_addr_set(p_mac_addr_t);
	
	
	uint32_t err_code =sd_ble_gap_addr_get(&addr);//获取
	APP_ERROR_CHECK(err_code);
//	addr.addr[0]+=1;
	addr.addr_id_peer=1;
	addr.addr_type=BLE_GAP_ADDR_TYPE_PUBLIC;//声明pub
	  
	err_code=sd_ble_gap_addr_set(&addr);
	APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Advertising functionality.
 */
 void advertising_init(void)
{
    uint32_t               err_code;
    ble_advertising_init_t init;

//	int8_t	TX_POWER_LEVEL=1;
//	int8_t tx_power_level=TX_POWER_LEVEL;
//	init.advdata.p_tx_power_level  =  &tx_power_level;
	
    memset(&init, 0, sizeof(init));

    init.advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance = false;
    init.advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;//LIMITED GENERAL

    init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
	
//	init.config.ble_adv_primary_phy   =BLE_GAP_PHY_2MBPS;// BLE_GAP_PHY_AUTO;//BLE_GAP_PHY_2MBPS;
//	init.config.ble_adv_secondary_phy  =BLE_GAP_PHY_2MBPS;
    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}


/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
 void buttons_leds_init(bool * p_erase_bonds)
{
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}


/**@brief Function for initializing the nrf log module.
 */
 void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing power management.
 */
 void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
 void idle_state_handle(void)
{
    UNUSED_RETURN_VALUE(NRF_LOG_PROCESS());
			
    nrf_pwr_mgmt_run();
		
}


/**@brief Function for starting advertising.
 */
 void advertising_start(void)
{
    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}


 void dcdc_on(void)
{
	NRF_POWER->DCDCEN = 1;//dcdc
}
 void sys_off(void)
{
	NRF_POWER->SYSTEMOFF = 1;//system off
}



 void gpio_off(void)
{
	nrf_gpio_cfg_default(pingmu_RST_IO);
	nrf_gpio_cfg_default(pingmu_BUSY_IO);
	nrf_gpio_cfg_default(BOARD_DC_IO);
	nrf_gpio_cfg_default(BOARD_SPI0_CSN_IO);

}




buffer_t m_buf;
void ble_data_send_with_queue(void)
{
	uint32_t err_code;
	uint16_t length = 0;
	static bool retry = false;
	
	if (retry)
	{
		length = m_buf.length;
		err_code = ble_nus_data_send1(&m_nus, m_buf.p_data, &length, m_conn_handle);
		//NRF_LOG_INFO("Data2: %d", m_buf.p_data[0]);
		if ( (err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_ERROR_RESOURCES) &&
				 (err_code != NRF_ERROR_NOT_FOUND) )
		{
				APP_ERROR_CHECK(err_code);
		}
		if (err_code == NRF_SUCCESS)
		{
			m_len_sent += length;
			retry = false;
		}
	}
	
	while (!nrf_queue_is_empty(&m_buf_queue) && !retry)
	{		

		err_code = nrf_queue_pop(&m_buf_queue, &m_buf);
		APP_ERROR_CHECK(err_code);		
		length = m_buf.length;
					
		err_code = ble_nus_data_send1(&m_nus, m_buf.p_data, &length, m_conn_handle);
		//NRF_LOG_INFO("Data: %d", m_buf.p_data[0]);
		if ( (err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_ERROR_RESOURCES) &&
				 (err_code != NRF_ERROR_NOT_FOUND) )
		{
				APP_ERROR_CHECK(err_code);
		}
		if (err_code == NRF_SUCCESS)
		{
			m_len_sent += length;
			retry = false;
		}
		else
		{
			retry = true;
			break;
		}
	}			
}

static void throughput_timer_handler(void * p_context)
{
#ifndef APP_QUEUE	
	//the snippet used to test data throughput only. no queue is involved
	ret_code_t err_code;
	uint16_t length;
	m_cnt_7ms++;	
	//sending code lines
	length = m_ble_nus_max_data_len;	
	do
	{					
		err_code = ble_nus_data_send1(&m_nus, m_data_array, &length, m_conn_handle);
		if ( (err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_ERROR_RESOURCES) &&
				 (err_code != NRF_ERROR_NOT_FOUND) )
		{
				APP_ERROR_CHECK(err_code);
		}
		if (err_code == NRF_SUCCESS)
		{
			m_len_sent += length; 	
			m_data_array[0]++;
			m_data_array[length-1]++;	
		}
	} while (err_code == NRF_SUCCESS);

	//calculate speed every 1 second
	if (m_cnt_7ms == 143)
	{
		NRF_LOG_INFO("==**Speed: %d B/s**==", m_len_sent);
		m_cnt_7ms = 0;
		m_len_sent = 0;
		m_data_array[0] = 0;
		m_data_array[length-1] = 0;
	}	
	//NRF_LOG_INFO("PacketNo.: %d == Time: %d *7ms", m_data_array[0], m_cnt_7ms);	
#else
	//the snippet simulate a real application scenairo. Queue is involved.
	ret_code_t err_code1, err_code2;	
	buffer_t buf;
	static uint8_t val = 0;
	//produce the data irregard of BLE activity
	m_data_array[(m_cnt_7ms%10)*420] = val++;
	m_data_array[(m_cnt_7ms%10)*420+210] = val++;
	
	//put the data into a queue to cache them
	buf.p_data = &m_data_array[(m_cnt_7ms%10)*420];
	buf.length = MIN(m_ble_nus_max_data_len,210);
	err_code1 = nrf_queue_push(&m_buf_queue, &buf);
	//APP_ERROR_CHECK(err_code1); //it may return NRF_ERROR_NO_MEM. we skip this error
	
	buf.p_data = &m_data_array[(m_cnt_7ms%10)*420+210];
	buf.length = MIN(m_ble_nus_max_data_len,210);
	err_code2 = nrf_queue_push(&m_buf_queue, &buf);
	//APP_ERROR_CHECK(err_code2);	//it may return NRF_ERROR_NO_MEM. we skip this error
	
	ble_data_send_with_queue();
	
	if(err_code1 == NRF_ERROR_NO_MEM || err_code2 == NRF_ERROR_NO_MEM)
	{
		NRF_LOG_INFO("Drop");	
	}
	
	m_cnt_7ms++;	
	//calculate speed every 1 second
	if (m_cnt_7ms == 143)
	{
		NRF_LOG_INFO("==**Speed: %d B/s**==", m_len_sent);
		m_cnt_7ms = 0;
		m_len_sent = 0;
	}	
	//NRF_LOG_INFO("Time: %d *7ms", m_cnt_7ms);		
	
#endif	
}


void throughput_test(void)
{
	ret_code_t err_code;
	err_code = app_timer_create(&m_timer_speed, APP_TIMER_MODE_REPEATED, throughput_timer_handler);
	APP_ERROR_CHECK(err_code);

#if 0
	  ble_opt_t  opt;
    memset(&opt, 0x00, sizeof(opt));
    opt.common_opt.conn_evt_ext.enable = true;
    err_code = sd_ble_opt_set(BLE_COMMON_OPT_CONN_EVT_EXT, &opt);
    APP_ERROR_CHECK(err_code);
#endif	
	
}







void registHeightCallback(CallbackFun callback, uint32_t a0, uint32_t a1)
{
	//uint32_t a0=1, uint32_t a1=2;
	callback(NULL,NULL);
}



