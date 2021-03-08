#ifndef _ALM_INIT_PROFILE_H_
#define _ALM_INIT_PROFILE_H_

#define uint8		uint8_t
#define uint16		uint16_t
#define uint32		uint32_t
/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

//#include "ble_nus.h"

/*********************************************************************
 * DEFINITIONS
 */
#ifdef __cplusplus
extern "C" {
#endif
 

#define BLE_ALMINITPROFILE_DEF(_name)						\
static AlmInitProfile_t _name;								\
NRF_SDH_BLE_OBSERVER(_name ## _obs,							\
                     BLE_ALMINITPROFILE_BLE_OBSERVER_PRIO,	\
                     HandleAlmInitProfileOnBleStackEvent, &_name)

#define ALMINITPROFILE_UUID_BASE		{0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
										0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ALMINITPROFILE_UUID_SERVICE		0xFEF0
#define ALMINITPROFILE_UUID_CHAR1		0xFEFF
#define ALMINITPROFILE_UUID_CHAR2		0xFE00
#define ALMINITPROFILE_UUID_CHAR3		0xFE10
#define ALMINITPROFILE_UUID_CHAR4		0xFE31
#define ALMINITPROFILE_UUID_CHAR5		0xFE32

// Profile Parameters
#define ALMINITPROFILE_CHAR1			0        						// uint8 - Profile Characteristic 1 value
#define ALMINITPROFILE_CHAR2			1        						// uint8 - Profile Characteristic 2 value
#define ALMINITPROFILE_CHAR3			2        						// uint8 - Profile Characteristic 3 value
#define ALMINITPROFILE_CHAR4			3        						// uint8 - Profile Characteristic 4 value
#define ALMINITPROFILE_CHAR5			4        						// uint8 - Profile Characteristic 5 value
	




										
#if defined(NRF_SDH_BLE_GATT_MAX_MTU_SIZE) && (NRF_SDH_BLE_GATT_MAX_MTU_SIZE != 0)
    #define BLE_ALM_MAX_DATA_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - 0 - 0)
			#define BLE_ALM_MAX_DATA_LEN4 (100 - 0 - 0)							
#else
    #define BLE_NUS_MAX_DATA_LEN (BLE_GATT_MTU_SIZE_DEFAULT - OPCODE_LENGTH - HANDLE_LENGTH)
    #warning NRF_SDH_BLE_GATT_MAX_MTU_SIZE is not defined.
#endif										
// Length of Characteristic in bytes
#define ALMINITPROFILE_CHAR1_LEN		2       						// CHAR1 LEN
#define ALMINITPROFILE_CHAR2_LEN		2       						// CHAR2 LEN
#define ALMINITPROFILE_CHAR3_LEN		2      						// CHAR3 LEN
#define ALMINITPROFILE_CHAR4_LEN		126     						// CHAR4 LEN
#define ALMINITPROFILE_CHAR5_LEN		BLE_ALM_MAX_DATA_LEN       						// CHAR5 LEN										

/*********************************************************************
 * TYPEDEFS
 */
// Forward declaration of the AlmInitProfile_t type.

typedef struct
{
    uint8_t const * p_data; /**< A pointer to the buffer with received data. */
    uint16_t        length; /**< Length of received data. */
} ble_alm_data_t;





typedef struct almInitProfileService_s AlmInitProfile_t;

typedef void (*AlmInitProfileCharWriteHandler_t)(uint16 connHandle, uint8 charId, AlmInitProfile_t *pAlmInitProfile,
												const uint8 *pCharValue, uint16 length);

/* //const uint8 *pCharValue
 ALM Init Service init structure.
 This structure contains all options 
 and data needed for initialization of the service.
*/
 typedef   struct
{
    bool is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
} ble_alm_client_context_t;



typedef struct
{
    AlmInitProfileCharWriteHandler_t almInitProfileCharWriteHandler;	// Event handler to be called when the Characteristic is written.
} AlmInitProfileCallback_t;

/*
 ALM Init Service structure.
 This structure contains various status information for the service.
*/
struct almInitProfileService_s
{
    uint16 serviceHandle;      											// Handle of ALM Init Service (as provided by the BLE stack).
	uint16_t                   conn_handle; /**< Connection handle. */
	ble_gatts_char_handles_t char1Handle; 								// Handles related to the Characteristic 1.
   ble_gatts_char_handles_t char2Handle;								// Handles related to the Characteristic 2.
	ble_gatts_char_handles_t char3Handle;								// Handles related to the Characteristic 3.
	ble_gatts_char_handles_t char4Handle;								// Handles related to the Characteristic 4.
	ble_gatts_char_handles_t char5Handle;

	 
	
 bool                     is_notification_enabled;
	
    uint8 uuidType;           											// UUID type for the ALM Init Service.  
	AlmInitProfileCharWriteHandler_t almInitProfileCharWriteHandler;	// Event handler to be called when the Characteristic is written.


};

/*********************************************************************
 * API FUNCTIONS
 */

extern AlmInitProfileCallback_t  Alm_init ;




extern void AlmInitProfile_HandleCharValue(uint16 connHandle, uint8 charId, AlmInitProfile_t *pAlmInitProfile,
											const uint8 *pCharValue, uint16 length);

uint32 AlmInitProfile_AddService(void);
uint32 AlmInitProfile_RegisterAppCallback(AlmInitProfile_t *pAlmInitProfile, const AlmInitProfileCallback_t *pAppCallback);

void HandleAlmInitProfileOnBleStackEvent(ble_evt_t const *pBleEvent, void *pContext);//

uint32 AlmInitProfile_PushNotifyData(uint16 connHandle, AlmInitProfile_t *pAlmInitProfile, uint8 *pData, uint16 *dataLen);
uint32 AlmInitProfile_PushNotifyData4(uint16 connHandle, AlmInitProfile_t *pAlmInitProfile, uint8 *pData, uint16 *dataLen);

#ifdef __cplusplus
}
#endif

#endif /* _ALM_INIT_PROFILE_H_ */
