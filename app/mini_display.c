#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "minilzo.h"
#include "mini_display.h"
#include "spi_ble.h"
#include "spi_lib.h"
#include "spi_lib_75.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"

#include "alm_init_profile.h"

#include "ble1.h"

 uint8_t epd_display_itp_buffer[ITP_PACKET_BUFFER_SIZE];
 EpdDISPLAY_itpState_Struct epd_display_itp_state;
 EpdDISPLAY_itpContext_Struct epd_display_itp_context;
 uint8_t epd_display_itp_reply[ITP_PACKET_REPLY_MAX_SIZE];//32
 EpdDISPLAY_itpReplyMessage_Struct epd_display_reply_message;

#if defined(USE_MALLOC_IN_DECOMPRESS)
static uint8_t *epd_display_decompress_buffer = NULL;
#else
//#pragma DATA_SECTION(dispHandle, "my_aux_section")
 uint8_t epd_display_decompress_buffer[ZIP_DECOMPRESS_BUFFER_SIZE];
#endif
	uint16_t length=20;
//Task_Struct epd_display_task;
//char epd_display_task_stack[EPD_DISPLAY_TASK_STACK_SIZE];

//static Event_Struct epd_display_event;
//static Event_Handle epd_display_event_handle;
//static Semaphore_Handle epd_display_received_mutex;
//static Semaphore_Handle epd_display_decompress_mutex;


//extern Display_Handle dispHandle;
//Display_Handle dispHandle = NULL;

//static void EpdDisplay_taskInit(void);
//static void EpdDisplay_taskFxn(uint32_t a0, uint32_t a1);

/******************** (C) COPYRIGHT 2020 Archempower **************************
 * 作者    ：Jack W. Sun
 * 库版本  ：cc2640r2 4.20
 * 描述    ：EPD 数据传输与处理代码
 * 时间    ：2020.08.12
*******************************************************************************/
 EpdDisplay_itpReplyMessageHandler epd_display_rely_message_handler = NULL;

 bool EpdDisplay_itpReplyMessage(uint8_t msg_type, const char* message)//回复
{
	// 0xAA0x550x7E |      SN       |  Length       | Command | Source SN   |  TYPE   |  Message   |  SUM   |  XOR   |  0x0D 
  EpdDISPLAY_itpReplyMessage_Struct *replyMessage = &epd_display_reply_message;
  EpdDISPLAY_itpContext_Struct *context = &epd_display_itp_context;
  EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;
  ++replyMessage->sn;
  replyMessage->msg_type=msg_type;//32-10-4
  uint8_t len = (strlen(message)>ITP_PACKET_REPLY_MAX_SIZE-ITP_BLANK_PACKET_SIZE-4) ? (ITP_PACKET_REPLY_MAX_SIZE-ITP_BLANK_PACKET_SIZE-4) : strlen(message);
  replyMessage->length = 4+len;
  replyMessage->message = (char *)message;
      
  uint8_t *p = epd_display_itp_reply;
  strcpy((char *)p, ITP_PACKET_PREFIX);
  p += strlen(ITP_PACKET_PREFIX);  
#if defined(ITP_USE_LITTE_ENDIAN)  
  *(p++) = (replyMessage->sn & 0xff);
  *(p++) = (replyMessage->sn >> 8);    
  *(p++) = (replyMessage->length & 0xff);
  *(p++) = (replyMessage->length >> 8);
  *(p++) = (uint8_t)'R';      
  *(p++) = (state->sn & 0xff);
  *(p++) = (state->sn >> 8);
#else
  *(p++) = (replyMessage->sn >> 8);
  *(p++) = (replyMessage->sn & 0xff);
  *(p++) = (replyMessage->length >> 8);
  *(p++) = (replyMessage->length & 0xff);
  *(p++) = (uint8_t)'R';  
  *(p++) = (state->sn >> 8);
  *(p++) = (state->sn & 0xff);
#endif
  *(p++) = (uint8_t)msg_type;
  memcpy(p, message, len);
  p += len;
  uint8_t sum = 0, v_xor = 0;
  uint8_t *i = epd_display_itp_reply; 
  i += 7;
  while(i<p) {
    sum += *i;
    v_xor ^= *i;
    i++;
  }
  *(p++) = sum;
  *(p++) = v_xor;
  strcpy((char *)p, ITP_PACKET_POSTFIX);///OD
  p += strlen(ITP_PACKET_POSTFIX); 
  if (epd_display_rely_message_handler != NULL) {
    epd_display_rely_message_handler(epd_display_itp_reply, replyMessage->length+ITP_BLANK_PACKET_SIZE);//10
#if defined(USE_DEBUG_MODE)
    if (context->reply_sn == state->sn) {
      NRF_LOG_INFO("%d.ITP reply message is mutil times. sn: %d.\r\n", ++context->log_no, state->sn);  
    }
#endif
    context->reply_sn = state->sn;
  }
	NRF_LOG_INFO("EpdDisplay_itpReplyMessage is ok\r\n");
		
//nrf_delay_ms(300);

if(flag==4){
	ble_send4((uint8 *)epd_display_itp_reply,20);//4
}	
//		
//	
if(flag==5){
	ble_send((uint8 *)epd_display_itp_reply,length);//5
}

	
if(flag==6){
//	black_a();
	length=sizeof(epd_display_itp_reply);
	ble_nus_send((uint8 *)epd_display_itp_reply,20);//nus
}
	
	 NRF_LOG_INFO("%s\r\n",message);
  return TRUE;
  
}

 bool EpdDisplay_itpDecompressNextBlock(void)//解压
{
  bool existNextBlock = FALSE;  
  EpdDISPLAY_itpContext_Struct *context = &epd_display_itp_context;
  EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;
//  Semaphore_pend(epd_display_decompress_mutex, ZIP_DECOMPRESS_TIME_OUT);
  state->summary &= (~ITP_STATE_HAS_DECOMPRESS);
  if ((state->summary & ITP_STATE_COMPRESS_COMPLETED) || state->offset>=state->length) {
  } else {
    uint16_t blockSize = EpdDisplay_itpPeekU16(ITP_PACKET_DATA_OFFSET+state->offset);
    if (blockSize==0 || state->offset+blockSize>=state->length) {
      state->summary |= ITP_STATE_COMPRESS_COMPLETED;
    } else {      
      memset(epd_display_decompress_buffer, 0, ZIP_DECOMPRESS_BUFFER_SIZE);
      uint8_t* srcBuffer = epd_display_itp_buffer + ITP_PACKET_DATA_OFFSET + state->offset + 2;
      lzo_uint destLength=0;
      int32_t iResult = lzo1x_decompress_safe(srcBuffer, blockSize, epd_display_decompress_buffer, &destLength, NULL); 
      if (iResult != 0 || destLength<=0) {
        NRF_LOG_INFO( "%d.Invoke lzo1x_decompress() error (%d).\r\n", ++context->log_no, iResult);  
        state->summary |= ITP_STATE_COMPRESS_COMPLETED;
      } else {
        state->block_num += 1;
        state->block_size = blockSize;
        state->raw_count = destLength;
        state->raw_subtotal += state->raw_count;
        state->summary |= ITP_STATE_HAS_DECOMPRESS;
        existNextBlock = TRUE;
        state->offset += (2 + blockSize);
        if (state->offset>=state->length) {
          state->summary |= ITP_STATE_COMPRESS_COMPLETED;
        }
      }
    }
  }   
//  Semaphore_post(epd_display_decompress_mutex);
  return existNextBlock;
}

 void EpdDisplay_itpDump(void)//空间·
{
  memset(&epd_display_itp_state, 0, sizeof(EpdDISPLAY_itpState_Struct));
}

static int16_t EpdDisplay_itpIndexOfPrefix(const uint8_t *buf)//头
{
  int32_t index = -1, i;
  EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;
  for (i=0; i<state->received_count-2; i++) {
    if (ITP_PACKET_PREFIX[0] == *(buf+i)
        && ITP_PACKET_PREFIX[1] == buf[i+1]
        && ITP_PACKET_PREFIX[2] == buf[i+2]) {
      index = i;
      break;
    }
  }
  return index;
}

 int16_t EpdDisplay_itpTrimLeft(int16_t offset)//长
{
  EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;
  if (0 == offset) {
    return 0;
  } else if (state->received_count <= offset) {
    int16_t trimBytes = state->received_count;
    state->received_count = 0;
    return trimBytes;
  } else {
    int16_t len = state->received_count - offset;
    for (int16_t i=0; i<len; i++) {
      epd_display_itp_buffer[i] = epd_display_itp_buffer[i+offset];
    }
    state->received_count = len;
    return offset;
  }
}

 uint8_t EpdDisplay_itpPeekU8(int16_t offset)//char
{
  return epd_display_itp_buffer[offset];
}

 uint16_t EpdDisplay_itpPeekU16(int16_t offset)//short
{
  uint16_t result = 0;
#if defined(ITP_USE_LITTE_ENDIAN)  
  result |= (epd_display_itp_buffer[offset++]     );
  result |= (epd_display_itp_buffer[offset]   << 8);
#else
  result |= (epd_display_itp_buffer[offset++] << 8);
  result |= (epd_display_itp_buffer[offset]       );
#endif
  return result;
}

 uint32_t EpdDisplay_itpPeekU32(int16_t offset)//int
{
  uint32_t result = 0;
#if defined(ITP_USE_LITTE_ENDIAN)  
  result |= (epd_display_itp_buffer[offset++]      );
  result |= (epd_display_itp_buffer[offset++] << 8 );
  result |= (epd_display_itp_buffer[offset++] << 16);
  result |= (epd_display_itp_buffer[offset]   << 24);
#else
  result |= (epd_display_itp_buffer[offset++] << 24);
  result |= (epd_display_itp_buffer[offset++] << 16);
  result |= (epd_display_itp_buffer[offset++] << 8 );
  result |= (epd_display_itp_buffer[offset]        );
#endif
  return result;
}

 uint16_t EpdDisplay_itpUpdateState(void) {
  EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;  
  if (state->received_count<ITP_PACKET_HEAD_SIZE+1) //8
  {
    return ITP_STATE_NONE;//0x0000
  }
  if ((state->summary & ITP_STATE_HAS_PREFIX) != ITP_STATE_HAS_PREFIX) {
    int16_t i = EpdDisplay_itpIndexOfPrefix(epd_display_itp_buffer);
    if (i != 0) {
      return ITP_STATE_NONE;
    }
    state->summary |= ITP_STATE_HAS_PREFIX;
  }
  if ((state->summary & ITP_STATE_HAS_COMMAND) != ITP_STATE_HAS_COMMAND) {
    if (state->received_count < ITP_PACKET_HEAD_SIZE+1) {
      return state->summary;
    } else {
      state->summary |= ITP_STATE_HAS_HEAD;
      state->summary |= ITP_STATE_HAS_COMMAND;
      state->sn = EpdDisplay_itpPeekU16(3);
      state->length = EpdDisplay_itpPeekU16(5);
      state->command = EpdDisplay_itpPeekU8(7);
    }
  }
  if ((state->summary & ITP_STATE_HAS_COMPLETED) != ITP_STATE_HAS_COMPLETED) {
    if (state->received_count < state->length+ITP_BLANK_PACKET_SIZE) {
      return state->summary;
    } else {
      state->summary |= ITP_STATE_HAS_COMPLETED;
    }
  }
  return state->summary;
}

 bool EpdDisplay_itpCheckSum(void) {
  EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;
  if ((state->summary & ITP_STATE_HAS_COMPLETED) != ITP_STATE_HAS_COMPLETED) {
    return FALSE;
  }
  uint8_t acc_sum = 0, acc_xor = 0;
  for (int32_t i=strlen(ITP_PACKET_PREFIX); i<ITP_PACKET_HEAD_SIZE+state->length; i++) {
    uint8_t value = epd_display_itp_buffer[i];
    acc_sum += value;
    acc_xor ^= value;
  }
  if (acc_sum != epd_display_itp_buffer[ITP_PACKET_HEAD_SIZE+state->length]
      || acc_xor != epd_display_itp_buffer[ITP_PACKET_HEAD_SIZE+state->length+1]) 
  {
    return FALSE;
  } else {
    state->summary |= ITP_STATE_HAS_CHECKED;
    return TRUE;
  }
}


void EpdDisplay_doReceive(uint8_t *pData, uint16_t len)
{
  uint16_t r_len;
  EpdDISPLAY_itpContext_Struct *context = &epd_display_itp_context;
//  Semaphore_pend(epd_display_received_mutex, ITP_PACKET_PROCESS_TIME_OUT);
  
  EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;  
	
#if defined(ITP_SUPPORT_SPLIT_PACKET)
  if (EpdDisplay_itpIndexOfPrefix(pData)) {
    memset(&epd_display_itp_state, 0, sizeof(EpdDISPLAY_itpState_Struct));
    r_len = len > ITP_PACKET_BUFFER_SIZE ? ITP_PACKET_BUFFER_SIZE : len;
    memcpy(epd_display_itp_buffer, pData, r_len);
    state->received_count = r_len;
  } else {
    r_len = len > ITP_PACKET_BUFFER_SIZE-state->received_count ? ITP_PACKET_BUFFER_SIZE-state->received_count : len;
    memcpy(epd_display_itp_buffer+state->received_count, pData, r_len);
    state->received_count += r_len;
  }
#else
  memset(&epd_display_itp_state, 0, sizeof(EpdDISPLAY_itpState_Struct));
  r_len = len > ITP_PACKET_BUFFER_SIZE ? ITP_PACKET_BUFFER_SIZE : len;
  memcpy(epd_display_itp_buffer, pData, r_len);
  state->received_count = r_len;
#endif
//  Semaphore_post(epd_display_received_mutex);
//  Event_post(epd_display_event_handle, EPD_DISPLAY_RECEIVED_EVENT);
  
#if defined(USE_DEBUG_MODE)
  NRF_LOG_INFO( "%d.Invoke epd display receive data, len: %d, r_len: %d data:(%d %d %d).\r\n", 
                 ++context->log_no, len, r_len, epd_display_itp_buffer[0], epd_display_itp_buffer[60], epd_display_itp_buffer[111]);  
#endif
}


//Semaphore_post(epd_display_received_mutex);表示发布信号量。如果任务正在等待信号量，则该调用从信号量队列
//中删除任务并将其放在就绪队列上。如果没有任务正在等待， Semaphore_post()会增加信号
//量计数并返回。对于二进制信号量，计数始终设置为 1。
//Event_post(epd_display_event_handle, EPD_DISPLAY_RECEIVED_EVENT); 
//Semaphore_pend(epd_display_decompress_mutex, ZIP_DECOMPRESS_TIME_OUT);Display_printf(dispHandle, SP_ROW_DEBUG_EPD_DISPLAY+1, 0, "%d.Invoke lzo1x_decompress() error (%d).\r\n", ++context->log_no, iResult);  



 void EpdDisplay_taskFxn(uint32_t a0, uint32_t a1)
{
  bool hasPrintStartWords = FALSE;
 
  
//  for (;;)
//  {    
////    if (!hasPrintStartWords && dispHandle != NULL) {
////      hasPrintStartWords = TRUE;
#if defined(USE_MALLOC_IN_DECOMPRESS)
      NRF_LOG_INFO( "EPD Display task start, decompress buffer malloc %s.\r\n", epd_display_decompress_buffer==NULL? "fail":"success");    
#else
 //     NRF_LOG_INFO( "EPD Display task start.\r\n");
#endif
//    }
//    
//    uint32_t events;
//    events = Event_pend(epd_display_event_handle,Event_Id_NONE, EPD_DISPLAY_EVENT_ALL, BIOS_WAIT_FOREVER);    
//    if(events & EPD_DISPLAY_RECEIVED_EVENT)
//
//    {      
//      Semaphore_pend(epd_display_received_mutex, ITP_PACKET_PROCESS_TIME_OUT); //Semaphore_pend()是一个阻塞调用，允许另一个任务在其等待信号量时运行。超时
//许任务等待直到超时、无限期等待，或不等待。返回值表示信号量是否成功发信号 

      EpdDISPLAY_itpState_Struct *state = &epd_display_itp_state;
      EpdDISPLAY_itpContext_Struct *context = &epd_display_itp_context;
#if defined(USE_DEBUG_MODE)
      NRF_LOG_INFO( "%d.EPD Display received (%d).\r\n", ++context->log_no, state->received_count);
#endif
      if (!(EpdDisplay_itpUpdateState() & ITP_STATE_HAS_COMPLETED))
      { 
#if defined(ITP_SUPPORT_SPLIT_PACKET)
        if (state->received_count >= ITP_PACKET_BUFFER_SIZE) {
          Display_printf(dispHandle, SP_ROW_DEBUG_EPD_DISPLAY+1, 0, "%d.ITP packet buffer overflow, raw total: %d.\r\n",
                         ++context->log_no, context->raw_total);
          EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_ERROR, ITP_PACKET_REPLY_MSG_BUF_OVER);
          EpdDisplay_itpDump();
        }
#else
        NRF_LOG_INFO( "%d.Invoke epd display update state, state: %d, received: %d, length: %d.\r\n",
                       ++context->log_no, state->summary, state->received_count, state->length); 
				
        EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_ERROR, ITP_PACKET_REPLY_MSG_CMD_UNKNOWN);
				NRF_LOG_INFO( "EPD data check0.\r\n");
#endif
      } 
      else {
				  NRF_LOG_INFO( "EPD data check.\r\n");
        if (!EpdDisplay_itpCheckSum()) { //校验和
          NRF_LOG_INFO( "%d.ITP checksum is not valid, sn: %d, sum: %d, xor:%d.\r\n",
                         ++context->log_no, state->sn, epd_display_itp_buffer[ITP_PACKET_HEAD_SIZE+state->length], epd_display_itp_buffer[ITP_PACKET_HEAD_SIZE+state->length+1]);          
          EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_ERROR, ITP_PACKET_REPLY_MSG_SUM_INVALID);
        } else {
         NRF_LOG_INFO( "%d.ITP received packet, sn: %d, length: %d, command: %c.\r\n", ++context->log_no, state->sn, state->length, state->command); 
             
          if (state->command == BOE0583_DISPLAY_ACTION_INIT //I
              || state->command == BOE0583_DISPLAY_ACTION_INIT_RAW ) //i
          {
//            Display_clearLines(dispHandle, SP_ROW_DEBUG_EPD_DISPLAY+1, SP_ROW_DEBUG_EPD_DISPLAY+7);
						BOE0583CC26XX_doDisplay(state->command, NULL, 0, BOE0583_DEFAULT_DISPLAY_TIMEOUT);
            memset(&epd_display_itp_context, 0x00, sizeof(EpdDISPLAY_itpContext_Struct)); 
            context->reply_sn = (-1);
            context->last_sn = (-1);
          }
          if (state->command == BOE0583_DISPLAY_ACTION_NEXT //N
              || state->command == BOE0583_DISPLAY_ACTION_NEXT_RAW) //n
          {
            context->has_red = TRUE;
            BOE0583CC26XX_doDisplay(state->command, NULL, 0, BOE0583_DEFAULT_DISPLAY_TIMEOUT);            
          }  
          
          if (state->sn == context->last_sn && state->sn != 0)
          {
            NRF_LOG_INFO( "%d.ITP packet has received, so ignored, sn: %d, length: %d.\r\n",
                         ++context->log_no, state->sn, state->length);  
            if (state->command == BOE0583_DISPLAY_ACTION_SHOW || state->command == BOE0583_DISPLAY_ACTION_SHOW_RAW)//Ss 
              EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_ERROR, ITP_PACKET_REPLY_MSG_IMG_INVALID);
            else
              EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_OK, ITP_PACKET_REPLY_MSG_OK);            
          } else if (state->sn != context->block_total) {
            context->has_invalid = TRUE;
            NRF_LOG_INFO( "%d.ITP packet invalid context, last sn: %d, pt: %d, lt: %d, bt: %d, rt: %d.\r\n",
                           ++context->log_no, context->last_sn, context->packet_total, context->length_total, context->block_total, context->raw_total); 
            EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_ERROR, ITP_PACKET_REPLY_MSG_SN_INVALID);
          } else {
            context->packet_total += 1;
            context->length_total += state->length;
            if (state->command == BOE0583_DISPLAY_ACTION_INIT_RAW //I
                || state->command == BOE0583_DISPLAY_ACTION_NEXT_RAW //L
                || state->command == BOE0583_DISPLAY_ACTION_LOAD_RAW //N
                || state->command == BOE0583_DISPLAY_ACTION_SHOW_RAW) //S
            {
#if defined(USE_DEBUG_MODE)
              NRF_LOG_INFO( "%d.ITP raw packet block, sn: %d, data length: %d.\r\n", 
                             ++context->log_no, state->sn, state->length); 
#endif
              context->last_block += 1;
              context->raw_total += state->length;
              context->block_total += 1;
              state->block_num = 1;
              state->raw_count = state->length;
              state->raw_subtotal = state->length;
              BOE0583CC26XX_doDisplay(BOE0583_DISPLAY_ACTION_LOAD_RAW, epd_display_itp_buffer+ITP_PACKET_DATA_OFFSET, state->length, BOE0583_DEFAULT_DISPLAY_TIMEOUT);      
#if defined(USE_DEBUG_MODE)
              NRF_LOG_INFO( "%d.ITP display raw block, sn: %d, block no: %d.\r\n", 
                             ++context->log_no, state->sn, context->last_block); 
#endif
            } 
            else {
              if(!EpdDisplay_itpDecompressNextBlock()) {
                NRF_LOG_INFO( "%d.ITP decompress  next block fail, sn: %d, decompressed: %d, blocks: %d, raws: %d, packets: %d.\r\n", 
                               ++context->log_no, state->sn, state->raw_count, context->block_total, context->raw_total, context->packet_total);          
              } else {
                do {
#if defined(USE_DEBUG_MODE)
                  NRF_LOG_INFO( "%d.ITP decompress  next block ok, sn: %d, decompressed: %d.\r\n", 
                                 ++context->log_no, state->sn, state->raw_count); 
#endif
                  context->last_block += 1;
                  context->raw_total += state->raw_count;
                  context->block_total += 1;
                  if (context->block_size == 0) {
                    context->block_size = state->raw_count;
                  }
      BOE0583CC26XX_doDisplay(BOE0583_DISPLAY_ACTION_LOAD, epd_display_decompress_buffer, state->raw_count, BOE0583_DEFAULT_DISPLAY_TIMEOUT);
#if defined(USE_DEBUG_MODE)
NRF_LOG_INFO("raw total: %d, packets: %d, block: %d, raw: %d.\r\n", 
             context->raw_total, context->packet_total, state->block_size, state->raw_count); 
NRF_LOG_INFO( "%d.ITP display block, sn: %d, block total: %d\r\n", 
                                 ++context->log_no, state->sn, context->block_total); 

									
#endif
                } while(EpdDisplay_itpDecompressNextBlock());
              }
            } 
            if (state->command == BOE0583_DISPLAY_ACTION_SHOW || state->command == BOE0583_DISPLAY_ACTION_SHOW_RAW) {
              if ((context->has_red == TRUE && context->raw_total == EPD_SCREEN_DATA_SIZE*2)
                  || (context->has_red == FALSE && context->raw_total == EPD_SCREEN_DATA_SIZE)) {     
                EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_OK, ITP_PACKET_REPLY_MSG_OK);
                
			BOE0583CC26XX_doDisplay(state->command, NULL, 0, BOE0583_DEFAULT_DISPLAY_TIMEOUT);                
               
										SimplePeripheral_doeInkUpdated();
              } else {
                NRF_LOG_INFO( "%d.ITP image size invalid, raw total: %d.\r\n",
                         ++context->log_no, context->raw_total);                  
                EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_ERROR, ITP_PACKET_REPLY_MSG_IMG_INVALID);
              }              
            } else {
              EpdDisplay_itpReplyMessage(ITP_PACKET_REPLY_TYPE_OK, ITP_PACKET_REPLY_MSG_OK);
            }
            context->last_sn = state->sn;
          }
        }
        EpdDisplay_itpDump();
      } 
		//Semaphore_post(epd_display_received_mutex);
//    }
    
//  }
}

void SimplePeripheral_doeInkUpdated(void)
{
  spPeriodicTaskContext.iseInkUpdating = FALSE;
  ++spSystemStates.accRefreshCount;
  spSystemStates.lastRefreshTime = spSystemSettings.systemTime + (spPeriodicTaskContext.periodicTicks-spPeriodicTaskContext.systemSettingsTicks);
}
