
/*********************************************************************
 * INCLUDES
 */
 


#include "ble.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
 
#include "sdk_common.h"
#if NRF_MODULE_ENABLED(BLE_ALMINITPROFILE)
#include "ble_srv_common.h"

#include "alm_init_profile.h"
//#include "common.h"
#include "crc16.h"
 void almInitProfile_writeAttrCallback(AlmInitProfile_t *pAlmInitProfile, ble_evt_t const *pBleEvent);

/*********************************************************************
 * LOCAL VARIABLES
 */
//BLE_ALMINITPROFILE_DEF(m_almInitProfile);

/*********************************************************************
 * EXTERN FUNCTIONS
 */
// 在主函数定义

// extern void AlmInitProfile_HandleCharValue( uint8 charId, 
//											const uint8 *pCharValue, uint16 length);
 
/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/**
 @brief 添加ALM Init Profile服务
 @param 无
 @return NRF_SUCCESS - 成功；其他值 - 失败
*/
//uint32 AlmInitProfile_AddService(void)
//{	
//	AlmInitProfileCallback_t almInitProfileCallback = {0};
//	
//	return AlmInitProfile_RegisterAppCallback(&m_almInitProfile, &almInitProfileCallback);
//}


uint16_t crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc)
{
    uint16_t crc = (p_crc == NULL) ? 0xFFFF : *p_crc;

    for (uint32_t i = 0; i < size; i++)
    {
        crc  = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (uint8_t)(crc & 0xFF) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xFF) << 4) << 1;
    }

    return crc;
}

/**@brief Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the S110 SoftDevice.
 *
 * @param[in] p_nus     Nordic UART Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
//修改
static void on_connect(AlmInitProfile_t *pAlmInitProfile, ble_evt_t const * pBleEvent)
{
    uint32_t err_code;
    uint16_t buffer[4];
	
	 ble_gatts_value_t          gatts_val;
    uint8_t                    cccd_value[2];
	
	
	  ble_alm_client_context_t * p_client = NULL;


	  memset(&gatts_val, 0, sizeof(ble_gatts_value_t));
    gatts_val.p_value = cccd_value;
    gatts_val.len     = sizeof(cccd_value);
    gatts_val.offset  = 0;
	
 NRF_LOG_INFO("link##################");
    pAlmInitProfile->conn_handle = pBleEvent->evt.gap_evt.conn_handle;
    
    buffer[0] = pAlmInitProfile->char5Handle.cccd_handle;      // cccd handle
    buffer[1] = 2;       // cccd attribute size = 2
    buffer[2] = 1;       // 1 = enable notifications
    buffer[3] = crc16_compute((uint8_t *)buffer, 6,NULL);  // CRC-CCITT (0xFFFF)
    err_code = sd_ble_gatts_sys_attr_set(pAlmInitProfile->conn_handle, (uint8_t*)(buffer), 8, 1<<1); 
    NRF_LOG_INFO("err_code=%d",err_code);
	 NRF_LOG_INFO(" buffer[3]=%d", buffer[3]);
	 NRF_LOG_INFO(" buffer[0]=%d", buffer[0]);

	
	
    if(err_code == NRF_SUCCESS)
    {   
			  //使能通知
			  pAlmInitProfile->is_notification_enabled = true;
			 NRF_LOG_INFO("使能通知##################");
        //翻转开发板上的指示灯D3，指示通知使能
        //nrf_gpio_pin_toggle(LED_3);
    }
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_hrs       Heart Rate Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(AlmInitProfile_t *pAlmInitProfile, ble_evt_t const * pBleEvent)
{
    UNUSED_PARAMETER(pBleEvent);
    pAlmInitProfile->conn_handle = BLE_CONN_HANDLE_INVALID;
}
/*
static void on_hvx_tx_complete(AlmInitProfile_t *pAlmInitProfile, ble_evt_t const * pBleEven)
{
   

    if (!pAlmInitProfile->is_notification_enabled)
    {
        NRF_LOG_INFO("notification OVER!");
    }
}
*/

/**
 @brief 注册应用程序回调函数。只调用这个函数一次
 @param pAlmInitProfile -[out] ALM初始化服务结构体
 @param pAppCallback -[in] 指向应用程序的回调
 @return NRF_SUCCESS - 成功；其他值 - 失败
*/
uint32 AlmInitProfile_RegisterAppCallback(AlmInitProfile_t *pAlmInitProfile, const AlmInitProfileCallback_t *pAppCallback)
{
    uint32 errCode;
    ble_uuid_t bleUuid;
    ble_add_char_params_t addCharParams;

		ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
	 ble_gatts_attr_md_t attr_md;
	
		

	
	
    // 初始化服务结构体
    pAlmInitProfile->almInitProfileCharWriteHandler = pAppCallback->almInitProfileCharWriteHandler;

    /*--------------------- 服务 ---------------------*/
    ble_uuid128_t baseUuid = {ALMINITPROFILE_UUID_BASE};
    errCode = sd_ble_uuid_vs_add(&baseUuid, &pAlmInitProfile->uuidType);
    VERIFY_SUCCESS(errCode);

    bleUuid.type = pAlmInitProfile->uuidType;
    bleUuid.uuid = ALMINITPROFILE_UUID_SERVICE;

    errCode = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &bleUuid, &pAlmInitProfile->serviceHandle);
    VERIFY_SUCCESS(errCode);

    /*--------------------- 特征1 ---------------------*/
    memset(&addCharParams, 0, sizeof(addCharParams));
    addCharParams.uuid              = ALMINITPROFILE_UUID_CHAR1;
    addCharParams.uuid_type         = pAlmInitProfile->uuidType;
    addCharParams.init_len          = sizeof(uint8_t);
    addCharParams.max_len           = ALMINITPROFILE_CHAR1_LEN;								// 特征长度
    addCharParams.char_props.read   = 1;													// 可读
			addCharParams.is_var_len       =true;												//kebian
		addCharParams.char_props.write  = 1;													// 可写
    addCharParams.char_props.notify = 1;													// 可通知								

    addCharParams.read_access       = SEC_OPEN;
	addCharParams.write_access      = SEC_OPEN;
    addCharParams.cccd_write_access = SEC_OPEN;

    errCode = characteristic_add(pAlmInitProfile->serviceHandle, &addCharParams, &pAlmInitProfile->char1Handle);
    if(errCode != NRF_SUCCESS)
    {
        return errCode;
    }

    /*--------------------- 特征2 ---------------------*/
    memset(&addCharParams, 0, sizeof(addCharParams));
    addCharParams.uuid             = ALMINITPROFILE_UUID_CHAR2;
    addCharParams.uuid_type        = pAlmInitProfile->uuidType;
    addCharParams.init_len         = sizeof(uint8_t);
    addCharParams.max_len          = ALMINITPROFILE_CHAR2_LEN;								// 特征长度
    addCharParams.char_props.read  = 1;														// 可读
    addCharParams.char_props.write = 1;														// 可写

    addCharParams.read_access  = SEC_OPEN;
    addCharParams.write_access = SEC_OPEN;

    errCode = characteristic_add(pAlmInitProfile->serviceHandle, &addCharParams, &pAlmInitProfile->char2Handle);
	if(errCode != NRF_SUCCESS)
    {
        return errCode;
    }
	
	/*--------------------- 特征3 ---------------------*/
    memset(&addCharParams, 0, sizeof(addCharParams));
    addCharParams.uuid             = ALMINITPROFILE_UUID_CHAR3;
    addCharParams.uuid_type        = pAlmInitProfile->uuidType;
    addCharParams.init_len         = sizeof(uint8_t);
    addCharParams.max_len          = ALMINITPROFILE_CHAR3_LEN;								// 特征长度
    addCharParams.char_props.read  = 1;														// 可读
    addCharParams.char_props.write = 1;														// 可写

    addCharParams.read_access  = SEC_OPEN;
    addCharParams.write_access = SEC_OPEN;

    errCode = characteristic_add(pAlmInitProfile->serviceHandle, &addCharParams, &pAlmInitProfile->char3Handle);
	if(errCode != NRF_SUCCESS)
    {
        return errCode;
    }
	
	/*--------------------- 特征4 ---------------------*/
    memset(&addCharParams, 0, sizeof(addCharParams));
    addCharParams.uuid             = ALMINITPROFILE_UUID_CHAR4;
    addCharParams.uuid_type        = pAlmInitProfile->uuidType;
    addCharParams.init_len         = sizeof(uint8_t);
    addCharParams.max_len          = ALMINITPROFILE_CHAR4_LEN;								// 特征长度
    addCharParams.char_props.read  = 1;														// 可读
    addCharParams.char_props.write = 1;														// 可写
 addCharParams.char_props.notify = 1;	
		
		
		addCharParams.is_var_len       =true;	
		
    addCharParams.read_access  = SEC_OPEN;
    addCharParams.write_access = SEC_OPEN;
 addCharParams.cccd_write_access = SEC_OPEN;
    errCode = characteristic_add(pAlmInitProfile->serviceHandle, &addCharParams, &pAlmInitProfile->char4Handle);
	if(errCode != NRF_SUCCESS)
    {
        return errCode;
    }
	 /*--------------------- 特征5 ---------------------*/
		
		memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));
	 char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;
		
		
		memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;
		
    memset(&addCharParams, 0, sizeof(addCharParams));
    addCharParams.uuid              = ALMINITPROFILE_UUID_CHAR5;
    addCharParams.uuid_type         = pAlmInitProfile->uuidType;
    addCharParams.init_len          = sizeof(uint8_t);
    addCharParams.max_len           = ALMINITPROFILE_CHAR5_LEN;								// 特征长度
    addCharParams.char_props.read   = 1;													// 可读
	addCharParams.char_props.write  = 1;													// 可写
    addCharParams.char_props.notify = 1;													// 可通知								
addCharParams.is_var_len       =true;	
    addCharParams.read_access       = SEC_OPEN;
	addCharParams.write_access      = SEC_OPEN;
    addCharParams.cccd_write_access = SEC_OPEN;

		
    errCode = characteristic_add(pAlmInitProfile->serviceHandle, &addCharParams, &pAlmInitProfile->char5Handle);
    if(errCode != NRF_SUCCESS)
    {
        return errCode;
    }

	return errCode;
}
 



/**
 @brief 处理来自蓝牙协议栈的应用事件
 @param pBleEvent -[in] 来自蓝牙协议栈的事件
 @param pContext -[in] ALM初始化服务结构体
 @return 无
*/
//uint16_t   m_conn_handle ; 
void HandleAlmInitProfileOnBleStackEvent(ble_evt_t const *pBleEvent, void *pContext)//const
{
    AlmInitProfile_t *pAlmInitProfile = (AlmInitProfile_t *)pContext;
uint32_t err_code;
    switch(pBleEvent->header.evt_id)
    {
	case BLE_GATTS_EVT_WRITE:
		almInitProfile_writeAttrCallback(pAlmInitProfile, pBleEvent);
		break;
	
/*	
	 case BLE_GATTS_EVT_HVN_TX_COMPLETE:
            on_hvx_tx_complete(pAlmInitProfile, pBleEvent);
            break;
*/	
	  case BLE_GAP_EVT_CONNECTED:
            on_connect(pAlmInitProfile, pBleEvent);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(pAlmInitProfile, pBleEvent);
            break;
	
	default:
		// No implementation needed.
		break;
    }
}

/**
 @brief 推送通知数据
 @param connHandle -[in] 连接句柄
 @param pAlmInitProfile -[in] ALM初始化服务结构体
 @param pData -[in] 通知内容
 @param dataLen -[in] 通知内容长度
 @return NRF_SUCCESS - 成功；其他值 - 失败
*/
uint32 AlmInitProfile_PushNotifyData(uint16 connHandle, AlmInitProfile_t *pAlmInitProfile, uint8 *pData, uint16 *dataLen)
{
    ble_gatts_hvx_params_t params;
	 VERIFY_PARAM_NOT_NULL(pAlmInitProfile);

	if(pAlmInitProfile->conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_NOT_FOUND;
    }
		
		  if (!pAlmInitProfile->is_notification_enabled)//通知使能了
    {
        return NRF_ERROR_INVALID_STATE;
    }
		
	if(*dataLen >ALMINITPROFILE_CHAR5_LEN)//ALMINITPROFILE_CHAR5_LEN
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = pAlmInitProfile->char5Handle.value_handle;
    params.p_data = pData;
    params.p_len  = dataLen;
		
		
		

    return sd_ble_gatts_hvx(pAlmInitProfile->conn_handle, &params);
}

uint32 AlmInitProfile_PushNotifyData4(uint16 connHandle, AlmInitProfile_t *pAlmInitProfile, uint8 *pData, uint16 *dataLen)
{
    ble_gatts_hvx_params_t params;
	
	if(connHandle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_NOT_FOUND;
    }
		
		
	if(*dataLen >ALMINITPROFILE_CHAR4_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&params, 0, sizeof(params));
    params.type   = BLE_GATT_HVX_NOTIFICATION;
    params.handle = pAlmInitProfile->char4Handle.value_handle;
    params.p_data = pData;
    params.p_len  = dataLen;
		
		
		

    return sd_ble_gatts_hvx(pAlmInitProfile->conn_handle, &params);
	}








/*********************************************************************
 * LOCAL FUNCTIONS
 */
/**
 @brief 写属性，在写入之前验证属性数据
 @param pAlmInitProfile -[in] ALM初始化服务结构体
 @param pBleEvent -[in] 来自蓝牙协议栈的事件
 @return 无
*/
static void almInitProfile_writeAttrCallback(AlmInitProfile_t *pAlmInitProfile, ble_evt_t const *pBleEvent)//const
{
    ble_gatts_evt_write_t  const*pEventWrite = &pBleEvent->evt.gatts_evt.params.write;//
	uint8 characteristicId;
	
//NRF_LOG_INFO("pAlmInitProfile->char5Handle.cccd_handle=%d",pAlmInitProfile->char5Handle.cccd_handle);
//NRF_LOG_INFO("pEventWrite->handle=%d",pEventWrite->handle);
//NRF_LOG_INFO("pAlmInitProfile->char5Handle.value_handle=%d",pAlmInitProfile->char5Handle.value_handle);

    if (
        (pEventWrite->handle == pAlmInitProfile->char5Handle.cccd_handle)
        &&
        (pEventWrite->len == 2)
       )
    {
        if (ble_srv_is_notification_enabled(pEventWrite->data))
        {
            pAlmInitProfile->is_notification_enabled = true;
        }
        else
        {
            pAlmInitProfile->is_notification_enabled = false;
        }
    }


//    else if (
//             (pEventWrite->handle == p_nus->tx_handles.value_handle)
//             &&
//             (p_nus->data_handler != NULL)
//            )
//    {
//        p_nus->data_handler(p_nus, pEventWrite->data, pEventWrite->len);
//    }
//    else
//    {
//        // Do Nothing. This event is not relevant for this service.
//    }

//else if(pEventWrite->handle != pAlmInitProfile->char5Handle.cccd_handle)
//	{
	/*--------------------- 特征1 ---------------------*/
	 if((pEventWrite->handle == pAlmInitProfile->char1Handle.value_handle)
		//&& (pEventWrite->len == ALMINITPROFILE_CHAR1_LEN)	
		&& (pAlmInitProfile->almInitProfileCharWriteHandler != NULL))
    {
		characteristicId = ALMINITPROFILE_CHAR1;
        pAlmInitProfile->almInitProfileCharWriteHandler(pBleEvent->evt.gap_evt.conn_handle, characteristicId,
														pAlmInitProfile, pEventWrite->data, pEventWrite->len);
    }	
	/*--------------------- 特征2 ---------------------*/
	else if((pEventWrite->handle == pAlmInitProfile->char2Handle.value_handle)
		//&& (pEventWrite->len == ALMINITPROFILE_CHAR2_LEN)	
		&& (pAlmInitProfile->almInitProfileCharWriteHandler != NULL))
    {
		characteristicId = ALMINITPROFILE_CHAR2;
        pAlmInitProfile->almInitProfileCharWriteHandler(pBleEvent->evt.gap_evt.conn_handle, characteristicId,
														pAlmInitProfile, pEventWrite->data, pEventWrite->len);
    }
	/*--------------------- 特征3 ---------------------*/
	else if((pEventWrite->handle == pAlmInitProfile->char3Handle.value_handle)
		//&& (pEventWrite->len == ALMINITPROFILE_CHAR3_LEN)	
		&& (pAlmInitProfile->almInitProfileCharWriteHandler != NULL))
    {
		characteristicId = ALMINITPROFILE_CHAR3;
        pAlmInitProfile->almInitProfileCharWriteHandler(pBleEvent->evt.gap_evt.conn_handle, characteristicId,
														pAlmInitProfile, pEventWrite->data, pEventWrite->len);
    }
	/*--------------------- 特征4 ---------------------*/
	else if((pEventWrite->handle == pAlmInitProfile->char4Handle.value_handle)
		//&& (pEventWrite->len==2 )	
		&& (pAlmInitProfile->almInitProfileCharWriteHandler != NULL))
    {
		characteristicId = ALMINITPROFILE_CHAR4;
        pAlmInitProfile->almInitProfileCharWriteHandler(pBleEvent->evt.gap_evt.conn_handle, characteristicId,
														pAlmInitProfile, pEventWrite->data, pEventWrite->len);
    }
		
		/*--------------------- 特征5 ---------------------*/
	else if((pEventWrite->handle == pAlmInitProfile->char5Handle.value_handle)
		//	&& (pEventWrite->len==2 )	
		&& (pAlmInitProfile->almInitProfileCharWriteHandler != NULL))
    {
		characteristicId = ALMINITPROFILE_CHAR5;
	 
			
        pAlmInitProfile->almInitProfileCharWriteHandler(pBleEvent->evt.gap_evt.conn_handle, characteristicId,
														pAlmInitProfile, pEventWrite->data, pEventWrite->len);
		}
		
//	}
}

#endif // NRF_MODULE_ENABLED(BLE_ALMINITPROFILE)

/****************************************************END OF FILE****************************************************/
