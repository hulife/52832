/*   J202                   J201


BSI					GND      BS2					GND  电源地
BUSY1				03			 BUSY2				16	忙状态输出引脚（表示忙碌）	
RST1				02       RST2         11	复位，低电平有效
DC1					05       DC2          12	数据/命令，低电平表示命令，高电平表示数据
CS1					07       CS2          15	片选，低电平有效
SCK1				06       SCK2         14	SPI的CLK，时钟信号输入
DIN1				04       DIN2         13	SPI的MOSI，数据输入
nrf_delay_us(200);
*/
#include "string.h"
#include "stddef.h"
#include "spi_ble.h"
#include "spi_lib.h"
#include "spi_lib_75.h"
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"	
//#include "Image_583.h" 
#include "board_spi.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "mini_display.h"
//#include "flower-bwr.png.h"
#include "location-card-bwr.png.h"
#include "png_75.h"
#include "ar.h"
#define EPD_FILL_RED_CHANNEL  TRUE


uint8_t  temp1,temp2;
uint16_t  Tsensor_C=0;
float Temperature_Value;
uint8_t Epaper_border;
extern uint8_t  DIS_EN_FLAG;
unsigned char Temp_Value_as_ICFormat[75];
unsigned char data1,data2;	
unsigned char buffer[1976];
unsigned char Test_buffer[1976];
unsigned char Display_buffer[ALLSCREEN_BYTES];


void EpaperIO_Init(void)
{		
	NRF_LOG_INFO("spi Init");
	//DC
	nrf_gpio_cfg_output(BOARD_DC_IO); //输出
	
	//cs
	nrf_gpio_cfg_output(BOARD_SPI0_CSN_IO); //输出
	
	//sck
	nrf_gpio_cfg_output(BOARD_SPI0_CLK_IO); //输出
	
	//din
	nrf_gpio_cfg_output(BOARD_SPI0_MOSI_IO); //输出
	
	
	//Epaper_BS=0;
}
void pingmuRstPinInit(void)//RST引脚初始化
{
	NRF_LOG_INFO("pingmuRstPinInit");    //配置 PWM 输出管脚
	nrf_gpio_cfg_output(pingmu_RST_IO);    //配置GPIO输出管脚
	//SPI_RST_SET;//
	pingmu_RST_HIGH;
}


//RST引脚设置

void pingmuRstPinSet(bool pinState)
{
	NRF_LOG_INFO("pingmuRstPinSet");   //配置 PWM 输出管脚
    if(pinState == pingmu_RST_OFF)
    {
		   nrf_gpio_pin_write (pingmu_RST_IO, 1);  //配置为写引脚，并写1
    }
    else if(pinState == pingmu_RST_ON)
    {
		nrf_gpio_pin_write(pingmu_RST_IO, 0);       //配置为写引脚，并写0
    }

}

//写死

//void BS1_Init(void)
//{
//	nrf_gpio_cfg_output(BOARD_BS1_IO);
//	nrf_gpio_pin_write(BOARD_BS1_IO, 0);     //配置BS1为低电平，则设置为4线SPI
//}

void BUSY_Init(void)
{
	NRF_LOG_INFO("busy Init");
	nrf_gpio_cfg_input(pingmu_BUSY_IO, NRF_GPIO_PIN_NOPULL);//拉输入
}


/*
 @brief DC的初始化函数
 @param 无
 @return 无
*/
void DC_Init(void)
{
	nrf_gpio_cfg_output(BOARD_DC_IO);

}


void DC_L(void)//命令
{
	nrf_gpio_pin_write(BOARD_DC_IO, 0);
}


void DC_H(void)//数据
{
	nrf_gpio_pin_write(BOARD_DC_IO, 1);
}



void RESET(void)  //复位
{
	
	 nrf_gpio_pin_write(pingmu_RST_IO, 1);
    nrf_delay_ms(20);
    nrf_gpio_pin_write(pingmu_RST_IO, 0);
    nrf_delay_ms(20);
    nrf_gpio_pin_write(pingmu_RST_IO, 1);
    nrf_delay_ms(20);
	
//	NRF_LOG_INFO("RESET Init");
//	//SPI_RST_CLEAR;
//	pingmu_RST_LOW;
//	nrf_delay_ms(1);								
// 	//SPI_RST_SET;
//	pingmu_RST_HIGH;
//  nrf_delay_ms(1);
}


/*
 * 函数名：SPI1_WriteByte
 * 描述  ：SPIx 写一个字节
 * 输入  ：TxData:要写入的字节
 * 输出  ：返回值:读取到的字节
 */

void spi_clk(void)
{
	nrf_gpio_cfg_output(BOARD_SPI0_CLK_IO ); 
	
}
  
void SPI_DATA_IO_Init(uint8_t mode)
{

if(mode==SPI_OUT)
 {
	nrf_gpio_cfg_output(BOARD_SPI0_MOSI_IO);
 }

 if(mode==SPI_IN)
 {
	nrf_gpio_cfg_input(BOARD_SPI0_MOSI_IO, NRF_GPIO_PIN_PULLDOWN);//拉输入
 }
}


/*
 * 函数名：HalLcd_READBUSY
 * 描述  ：等待电子纸busy信号由高变低 
 * 输入  ：无
 * 输出  ：无
 */	
void Epaper_READBUSY(void)
{ 
  while(1)
  {	 //=1 BUSY
	//nrf_gpio_cfg_input(pingmu_BUSY_IO,NRF_GPIO_PIN_NOPULL);
		 if(nrf_gpio_pin_read(pingmu_BUSY_IO)==0) //3
			 break;

		
  }  
	// nrf_delay_us(100) ;
}




void Epaper_Update(void)
{     
	
  Epaper_Write_Command(0x12);        //display      
  Epaper_READBUSY();		   
 
}
void Epaper_DeepSleep(void)
{
	NRF_LOG_INFO("sleep");
	Epaper_Write_Command(0x02);        //power off 
  Epaper_READBUSY();
	
  Epaper_Write_Command(0x07); //deep sleep
  Epaper_Write_Data(0xa5);  
  nrf_delay_ms(500); 

}

/*
 * 函数名：Epaper_Load_Image
 * 描述  ：加载图片
 * 输入  ：datas 图片数据
           num   数据大小
           mode  黑白 or 红色         三色屏幕 图片分两种 黑白+红色
 * 输出  ：无
 */	

void Epaper_Load_Image(const uint8_t *datas,uint32_t num,uint8_t color_mode)
{
  uint32_t i; 
  uint8_t tempOriginal;      
  uint32_t tempcol=0;
  uint32_t templine=0;
//  		Epaper_Write_Command(0x61);  // Resolution setting
//		Epaper_Write_Data(0x02);
//		Epaper_Write_Data(0x88);
//		Epaper_Write_Data(0x01);
//		Epaper_Write_Data(0xE0);
			//Epaper_READBUSY();
	if(color_mode==MONO)
    Epaper_Write_Command(0x10); 
	 
	if(color_mode==RED)
    Epaper_Write_Command(0x13);   //write RAM for black(0)/white (1)
  
	
	for(i=0;i<num;i++)
   {          
     tempOriginal=*(datas+templine*MAX_COLUMN_BYTES+tempcol);
     templine++;
     if(templine>=MAX_LINE_BYTES)
     {
       tempcol++;
       templine=0;
     }     
     Epaper_Write_Data(tempOriginal);
   } 
}





/*@brief 写命令寄存器
 @param registerAddr -[in] 寄存器地址
 @param writeData -[in] 写入数据
 @return 无
*/
void Epaper_Write_Command(unsigned char INIT_COM)
{
  
//	SPI_CsnSet(1);
//	SPI_CsnSet(0);
	SPI_CS_HIGH;
	SPI_CS_LOW;
	DC_L();

    SPI_ReadWriteData(&INIT_COM, NULL, 1);

}


// 写数据 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void Epaper_Write_Data(unsigned char INIT_DATA)
{
	//SPI_CS_HIGH;
//	SPI_CS_LOW;
	DC_H();
	
	
	 SPI_ReadWriteData(&INIT_DATA, NULL, 1);
}




/***666666666666666666666666666666666666666666666666666666666*********/
//#define Epaper_Write_Data(x)  BOE0583CC26XX_writeData(x);





 void BOE0583CC26XX_writeBlock(const uint8_t* block, uint16_t len)
{
	
  if (block == NULL) {
  } else {
		
    NRF_LOG_INFO( "BOE0583 write block, len: %d.\r\n", len);
    for (uint16_t i=0; i<len; i++)
    {      
      Epaper_Write_Data(*(block+i));
    }
  }
}


typedef struct BOE0583CC26XX_context_Struct {
  uint8_t action;
  uint8_t* block;
  uint16_t length;
#if defined(USE_DEBUG_MODE)
  uint32_t pushed_count;
#endif
#if defined(EPD_FILL_RED_CHANNEL)
  bool has_red;
#endif
} BOE0583CC26XX_context_Struct;

static BOE0583CC26XX_context_Struct boe_0583_context;

#define EPD_BLANK_ON_INIT 
 void BOE0583CC26XX_epdInit(void) 
{
	SPI_CS_HIGH;
	SPI_CS_LOW;
	EPD_7IN5B_V2_Init();
	
 // BOE0583CC26XX_cmdSetOptions(); 
 // BOE0583CC26XX_cmdSetResolution(); 
  
#if defined(EPD_BLANK_ON_INIT)  
  Epaper_Write_Command(0x10);
  for (uint16_t i=0; i<EPD_SCREEN_DATA_SIZE; i++) {
    Epaper_Write_Data(0x00);
  }
  Epaper_Write_Command(0x13);
  for (uint16_t i=0; i<EPD_SCREEN_DATA_SIZE; i++) {
    Epaper_Write_Data(0x00);
  }
#endif    
  
#if defined(USE_DEBUG_MODE)
  NRF_LOG_INFO( "Prepare BOE0583 display completed.\r\n");
#endif
}


 void BOE0583CC26XX_epdUninit(void) 
{
  EPD_7IN5B_V2_TurnOnDisplay();
  Epaper_DeepSleep();
//  EPD_SPI_LED_DOWN;
#if defined(USE_DEBUG_MODE)
  NRF_LOG_INFO( "Update BOE0583 display completed.\r\n");
#endif
}


  void BOE0583CC26XX_display(uint8_t action,  uint8_t* block, uint16_t len)
{
  BOE0583CC26XX_context_Struct *context = &boe_0583_context;
#if defined(USE_DEBUG_MODE)  
  if (action != BOE0583_DISPLAY_ACTION_INIT && action != BOE0583_DISPLAY_ACTION_INIT_RAW) {
    context->pushed_count += len;
  } else {
    context->pushed_count = 0;
  }
#endif
  switch(action) {
  case BOE0583_DISPLAY_ACTION_INIT:
  case BOE0583_DISPLAY_ACTION_INIT_RAW:
  {
    NRF_LOG_INFO( "BOE0583 display() init.\r\n");
#if defined(EPD_FILL_RED_CHANNEL)
    context->has_red = FALSE;
#endif
   BOE0583CC26XX_epdInit();   
		
    Epaper_Write_Command(0x10);      
    BOE0583CC26XX_writeBlock(block, len);
		NRF_LOG_INFO( "%d\r\n",len);
		 EPD_7IN5B_V2_TurnOnDisplay();
		
		
    break;
  }
  case BOE0583_DISPLAY_ACTION_LOAD:
  case BOE0583_DISPLAY_ACTION_LOAD_RAW:
  {
    BOE0583CC26XX_writeBlock(block, len);
    break;
  }
  case BOE0583_DISPLAY_ACTION_NEXT:
  case BOE0583_DISPLAY_ACTION_NEXT_RAW:
  {
#if defined(EPD_FILL_RED_CHANNEL)
    context->has_red = TRUE;
#endif
    Epaper_Write_Command(0x13);
    BOE0583CC26XX_writeBlock(block, len);
    break;
  }
  case BOE0583_DISPLAY_ACTION_SHOW:
  case BOE0583_DISPLAY_ACTION_SHOW_RAW:
  {
#if defined(USE_DEBUG_MODE)
    NRF_LOG_INFO( "BOE0583 display() write total: %d.\r\n", context->pushed_count);
#else
    NRF_LOG_INFO( "BOE0583 display() completed.\r\n");
#endif
    BOE0583CC26XX_writeBlock(block, len);
#if defined(EPD_FILL_RED_CHANNEL)
    if (!context->has_red) {
      Epaper_Write_Command(0x13);
      for (uint16_t i=0; i<EPD_SCREEN_DATA_SIZE; i++) {
        Epaper_Write_Data(0x00);
      }
    }
#endif
    BOE0583CC26XX_epdUninit();
    break;
  }
  default:
    break;
  }
}


void BOE0583CC26XX_doDisplay(uint8_t action,  uint8_t* block, uint16_t len, uint32_t timeout)
{
#if defined(USE_DEBUG_MODE)
  NRF_LOG_INFO( "BOE0583 display() action: %c, length: %d.\r\n", (char)action, len);
#endif
  BOE0583CC26XX_context_Struct *context = &boe_0583_context;
  context->action = action;
  context->block = block;
  context->length = len;    
#if defined( USE_EPD_TASK_BASED )
  Event_post(boe_0583_display_event_handle, BIOS_WAIT_FOREVER);
  Semaphore_pend(boe_0583_display_critical, timeout);
#else
  BOE0583CC26XX_display(context->action, context->block, context->length); 
#endif // USE_EPD_TASK_BASED

}



void spi_t(void)
{
	pingmuRstPinInit();//RST初始化
  BUSY_Init();       //BUSY初始化
	DC_Init();         //DC初始化
	SPI_Init();      //spi
}
bool pingmu_Init(void)
{
	pingmuRstPinInit();//RST初始化
//	SPI_CS_CLEAR;			//CS
  BUSY_Init();       //BUSY初始化
	DC_Init();         //DC初始化
												//硬件SPI0初始化
	SPI_Init();      //spi
	EPD_7IN5B_V2_Init();
	
	  return TRUE;
	}

const unsigned char gImage_160_0[160]={
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

	
const unsigned char gImage_160[160]={
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};
const unsigned char gImage_100_f[100]={
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};
		
const unsigned char gImage_100[100]={
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
	
	
	
int block1=100;
int a1=0;
int count1=0;
uint16_t leng1=0;
uint8_t buff_s1[100];
uint8_t* message11=buff_s1;
const unsigned char *image11=gImage_ar;
	
int block2=100;
int a2=0;
int count2=0;
uint16_t leng2=0;
uint8_t buff_s2[100];
uint8_t* message12=buff_s2;
const unsigned char *image12=gImage_ar;
	
void test_screen(void)
{
	EPD_7IN5B_V2_Init();

	Epaper_Write_Command(0x13);
			while(1)
	{
		count1++;
	
		if(count1<=480)
		{
	 image11 += block1;//picture use
		block1=100;	
		}
 memcpy(message11, image11, block1);
		
		      
    BOE0583CC26XX_writeBlock(message11, block1);
		NRF_LOG_INFO( "%d\r\n",count1);
			
		memset(message11,0,block1);
		if(count1>480)
		{
	
		
		break;
		}
	}
	
	Epaper_Write_Command(0x13);
		while(1)
	{
			count2++;
	
		if(count2<=480)
		{
	 image12 += block2;//picture use
		block2=100;	
		}
	 memcpy(message12, image12, block2);
		      
    BOE0583CC26XX_writeBlock(message12, block2);
		NRF_LOG_INFO( "%d\r\n",count2);
			
		memset(message12,0,block2);
		if(count2>480)
		{
		break;
		}
		
	}
	
	 EPD_7IN5B_V2_TurnOnDisplay();	
}


int block3=160;
int a3=0;
int count3=0;
uint16_t leng3=0;
uint8_t buff_s3[160];
uint8_t* message13=buff_s3;
const unsigned char *image13=gImage_160;//bw_image;
	
int block4=160;
int a4=0;
int count4=0;
uint16_t leng4=0;
uint8_t buff_s4[160];
uint8_t* message14=buff_s4;
const unsigned char *image14=gImage_160;//red_image;
//48000-38880=9120=160*57
//38880=160*243
uint8_t buff_s5[160];
uint8_t* message15=buff_s5;
const unsigned char *image15=gImage_160_0;

void test_screen1(void)
{
	EPD_7IN5B_V2_Init();
	//wave_7in5b_epd_init();
	Epaper_Write_Command(0x10);
			while(1)
	{
		count3++;
	 
		if(count3<=243)
		{
	// image13 += block3;
		block3=160;	

		memcpy(message13, image13, block3);		      
    BOE0583CC26XX_writeBlock(message13, block3);
		NRF_LOG_INFO( "%d\r\n",count3);		
		memset(message13,0,block3);
		}

		
		if(count3>243)
		{	
		memcpy(message15, image15, 160);
		BOE0583CC26XX_writeBlock(message15, 160);
		NRF_LOG_INFO( "%d\r\n",count3);		
		memset(message15,0,160);
		
		}
		if(count3>300)
		{			
		break;
		count3=0;	
		}
	}
	
	Epaper_Write_Command(0x13);
		while(1)
	{
		count4++;
		if(count4<=243)
		{
	// image14 += block4;
		block4=160;	
			
		memcpy(message14, image14, block4);      
    BOE0583CC26XX_writeBlock(message14, block4);
		NRF_LOG_INFO( "%d\r\n",count4);			
		memset(message14,0,block4);
		}

		
		if(count4>243)
		{
		memcpy(message15, image15, 160);
		BOE0583CC26XX_writeBlock(message15, 160);
		NRF_LOG_INFO( "%d\r\n",count3);		
		memset(message15,0,160);
		}
		
		if(count4>300)
		{		
		break;
		count4=0;	
		}
		
	}
	
	 EPD_7IN5B_V2_TurnOnDisplay();	
	 
}


	
	
void black_a(void)//驱动
{	

//	BOE0583CC26XX_cmdSetOptions();	//BOE	5.83							
// EPD_Init();											//微雪5.83
// EPD_Clear();
	
EPD_7IN5B_V2_Clear();           //微雪7.5	
//EPD_7IN5B_V2_ClearBlack();
//EPD_7IN5B_V2_ClearRed();

//EPD_7IN5B_V2_Display(bw_image,red_image);	
//EPD_7IN5B_V2_Display(gImage_7in5_V2_b,gImage_7in5_V2_b);
	
//Epaper_Load_Image(gImage_7in5_V2_b,48000,1);
	

	Epaper_DeepSleep();									//睡眠

//	SPI_Disable();											//spi失能
	
}




