#ifndef EPD_DISPLAY_H__
#define EPD_DISPLAY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdint.h>  
#include <stdbool.h>  
#define FALSE 0
#define TRUE  1

//#define ITP_SUPPORT_SPLIT_PACKET  1
#ifndef SP_ROW_DEBUG
#define SP_ROW_DEBUG         (TBM_ROW_APP + 8)
#endif
#define SP_ROW_DEBUG_EPD_DISPLAY   (SP_ROW_DEBUG+3)

#define ZIP_MAX_BLOCK_SIZE         512
#define ZIP_BLOCK_HEAD_SIZE        2
#define ZIP_DECOMPRESS_BUFFER_SIZE 2048
#define ZIP_DECOMPRESS_TIME_OUT    100000
//#define ITP_PLAINTEXT_BLOCK_SIZE   (ZIP_MAX_BLOCK_SIZE-20)
#define ITP_MIN_PACKET_SIZE        11
#define ITP_BLANK_PACKET_SIZE      10
#define ITP_PACKET_HEAD_SIZE       7
#define ITP_PACKET_COMMAND_OFFSET  ITP_PACKET_HEAD_SIZE
#define ITP_PACKET_DATA_OFFSET     (ITP_PACKET_HEAD_SIZE+1)//8
#define ITP_PACKET_BUFFER_SIZE     (ZIP_MAX_BLOCK_SIZE+ZIP_BLOCK_HEAD_SIZE+ITP_MIN_PACKET_SIZE)//512+2+11
#define ITP_STATE_NONE             0x0000
#define ITP_STATE_HAS_PREFIX       0x0001
#define ITP_STATE_HAS_HEAD         0x0002
#define ITP_STATE_HAS_COMMAND      0x0004
#define ITP_STATE_HAS_COMPLETED    0x0008
#define ITP_STATE_HAS_CHECKED      0x0010
#define ITP_STATE_HAS_DECOMPRESS   0x0020
#define ITP_STATE_COMPRESS_COMPLETED 0x0040
#define ITP_PACKET_PREFIX          "\xAA\x55\x7E"
#define ITP_PACKET_POSTFIX         "\x0D"
#define ITP_PACKET_REPLY_MAX_SIZE  32
#define ITP_PACKET_REPLY_CMD       0x52  // 'R'
#define ITP_PACKET_REPLY_TYPE_OK   0x4F  // 'O',
#define ITP_PACKET_REPLY_TYPE_ERROR 0x45 // 'E'  
#define ITP_PACKET_REPLY_TYPE_ARQ  0x41  // 'A'
#define ITP_PACKET_REPLY_MSG_OK    "OK"
#define ITP_PACKET_REPLY_MSG_ERROR "ERROR"
#define ITP_PACKET_REPLY_MSG_ARQ   "ARQ"
#define ITP_PACKET_REPLY_MSG_SUM_INVALID "SUM-NEQ"
#define ITP_PACKET_REPLY_MSG_CMD_UNKNOWN "CMD-XX"



#define ITP_PACKET_REPLY_MSG_WORN_OUT "WORN-OUT"
#define ITP_PACKET_REPLY_MSG_SN_INVALID "SN-NEQ"
#define ITP_PACKET_REPLY_MSG_IMG_INVALID "IMG-INV"
#define ITP_PACKET_REPLY_MSG_BUF_OVER "BUF-OVER"
#define ITP_PACKET_SN_KEPT_SIZE    10
#define ITP_PACKET_PROCESS_TIME_OUT    (30000000 / Clock_tickPeriod)

#define EPD_DISPLAY_TASK_PRIORITY   2
#define EPD_DISPLAY_TASK_STACK_SIZE 644

#define EPD_DISPLAY_RECEIVED_EVENT      Event_Id_00 
#define EPD_DISPLAY_EVENT_ALL ( EPD_DISPLAY_RECEIVED_EVENT )
#define EPD_DISPLAY_EVENT_TIME_OUT 100000
#define EPD_SCREEN_DATA_SIZE       38880


extern uint8_t epd_display_itp_buffer[ITP_PACKET_BUFFER_SIZE];
// EpdDISPLAY_itpState_Struct epd_display_itp_state;
// EpdDISPLAY_itpContext_Struct epd_display_itp_context;
extern uint8_t epd_display_itp_reply[ITP_PACKET_REPLY_MAX_SIZE];//32
// EpdDISPLAY_itpReplyMessage_Struct epd_display_reply_message;

#if defined(USE_MALLOC_IN_DECOMPRESS)
static uint8_t *epd_display_decompress_buffer = NULL;
#else
//#pragma DATA_SECTION(dispHandle, "my_aux_section")
extern uint8_t epd_display_decompress_buffer[ZIP_DECOMPRESS_BUFFER_SIZE];
#endif

//#define ITP_USE_LITTE_ENDIAN       TRUE
#define USE_DEBUG_MODE             TRUE



typedef struct EpdDISPLAY_itpState_Struct {
  uint16_t summary;
  uint16_t received_count;
  uint16_t sn;
  uint16_t length;
  uint8_t command;
  uint16_t offset;
  uint16_t block_size;
  uint16_t raw_count;
  uint8_t block_num;
  uint16_t raw_subtotal;
} EpdDISPLAY_itpState_Struct;

typedef struct EpdDISPLAY_itpContext_Struct {
  uint16_t last_sn;
  uint16_t reply_sn;
  uint32_t raw_total;
  uint32_t last_block;
  uint32_t block_total;
  uint32_t length_total;
  uint16_t packet_total;  
  uint16_t block_size;
  bool has_invalid;
  uint16_t log_no;
  bool has_red;
#if defined(USE_DEBUG_MODE)
  //uint16_t pre_sn_set[ITP_PACKET_SN_KEPT_SIZE];
  //uint16_t pre_length_set[ITP_PACKET_SN_KEPT_SIZE];
#endif  
} EpdDISPLAY_itpContext_Struct;

typedef struct EpdDISPLAY_itpReplyMessage_Struct {
  uint16_t sn;
  uint16_t length;
  uint8_t msg_type;
  char* message;
} EpdDISPLAY_itpReplyMessage_Struct;



 uint8_t EpdDisplay_itpPeekU8(int16_t offset);
 uint16_t EpdDisplay_itpPeekU16(int16_t offset);
 uint32_t EpdDisplay_itpPeekU32(int16_t offset);
 int16_t EpdDisplay_itpIndexOf(const uint8_t *prefix, size_t len);
 int16_t EpdDisplay_itpTrimLeft(int16_t offset);
 uint16_t EpdDisplay_itpUpdateState(void);
 bool EpdDisplay_itpCheckSum(void);
 void EpdDisplay_itpDump(void);
 bool EpdDisplay_itpDecompressNextBlock(void);
 bool EpdDisplay_itpReplyMessage(uint8_t msg_type, const char* message);
  int16_t EpdDisplay_itpIndexOfPrefix(const uint8_t *buf);

//void EpdDisplay_setLogHandle(Display_Handle logHandle)
//{
//  dispHandle = logHandle;    
//  BOE0583CC26XX_setLogHandle(logHandle);
//}
















// Periodic Task Context
typedef struct
{
  uint32_t systemSettingsTicks;
  uint32_t periodicTicks;
  uint8_t conns;
  uint32_t disconnectTicks;  
  uint32_t advertTransferTicks;
  uint32_t collectBatteryTicks;
  uint32_t backupTicks;
  bool isAdvertising;  
  bool iseInkUpdating;
} spPeriodicTaskContext_t;

// 系统状态：[上次刷新时间(4 bytes)][累计刷新次数(4 bytes)][芯片温度(1 bytes)][电池电量(1 bytes)][电池电压(2 bytes)]
typedef struct
{
  uint32_t lastRefreshTime;
  uint32_t accRefreshCount;
  uint8_t mcuTemperature;
  uint8_t batteryLevel;
  uint16_t batteryVoltage;
} spSystemStates_t;
// 关于可以配置的参数：[工作窗口时间(5秒)(2 bytes)][睡眠窗口时间(5秒)(2 bytes)][电压采样时间间隔(30秒)(2 bytes)][断线保持时间(10秒)(2 bytes)]
//     [广播间隔(500毫秒)(2 bytes)][系统时钟(4 bytes)][配置与状态备份间隔(1800秒)]
typedef struct
{
  uint16_t workWindowSegments;
  uint16_t sleepWindowSegments;
  uint16_t collectBatterySegments;
  uint16_t disconnectKeepSegments;  
  uint16_t advertInterval;
  uint32_t systemTime;
  uint16_t backupInterval;
} spSystemSettings_t;

static spSystemSettings_t spSystemSettings = {
  .workWindowSegments = 5,
  .sleepWindowSegments = 5,
  .collectBatterySegments = 30,
  .disconnectKeepSegments = 10,
  .advertInterval = 500,
  .systemTime = 0,
  .backupInterval = 1800
};


static spSystemStates_t spSystemStates;
static spPeriodicTaskContext_t spPeriodicTaskContext;

void SimplePeripheral_doeInkUpdated(void);



//typedef struct
//{
//  uint8_t *pBuf;
//  uint16_t length;
//} spReplyMessage_t;

//#define HEAPMGR_MALLOC         ICall_heapMalloc


//void *ICall_heapMalloc(uint32_t size);

//void *ICall_malloc(uint_least16_t size)
//{
//  return (ICall_heapMalloc(size));
//}

//#define HEAPMGR_FREE           ICall_heapFree
//void ICall_heapFree(void *blk);

/**
 * Frees an allocated memory block.
 * @param msg  pointer to a memory block to free.
 */
//void ICall_free(void *msg)
//{
//  ICall_heapFree(msg);
//}
#define SP_REPLY_NOTIFICATION_EVT            10
#define SUCCESS                               0

//void SimplePeripheral_doReplyMessage(uint8_t *pBuf, uint16_t length);

//void SimplePeripheral_doReplyMessage(uint8_t *pBuf, uint16_t length)
//{
//  spReplyMessage_t *pData ;
//	pData=calloc( 0, sizeof(spReplyMessage_t));
//  if (pData)
//  {
//    pData->pBuf = pBuf;
//    pData->length = length;

////    if(SimplePeripheral_enqueueMsg(SP_REPLY_NOTIFICATION_EVT, pData) != SUCCESS)
////    {
////      ICall_free(pData);
////    }
//  }
//	free(pData);
//}





typedef void (*EpdDisplay_itpReplyMessageHandler)(uint8_t * buf, uint16_t length);

void EpdDisplay_createTask(EpdDisplay_itpReplyMessageHandler replyMessageHandler);  
void EpdDisplay_doReceive(uint8_t *pData, uint16_t len);  
//void EpdDisplay_setLogHandle(Display_Handle logHandle);  


void EpdDisplay_taskFxn(uint32_t a0, uint32_t a1);

#ifdef __cplusplus
}
#endif
#endif //EPD_DISPLAY_H__

