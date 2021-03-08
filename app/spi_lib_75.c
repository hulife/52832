//引用的C库头文件
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//Log需要引用的头文件
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"	
#include "board_spi.h"


#include "spi_ble.h"
#include "spi_lib_75.h"

#include "nrf.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"
///******************************************************************************
//function :	Software reset
//parameter:
//******************************************************************************/
//static void EPD_7IN5B_V2_Reset(void)
//{
//    DEV_Digital_Write(EPD_RST_PIN, 1);
//    DEV_Delay_ms(200);
//    DEV_Digital_Write(EPD_RST_PIN, 0);
//    DEV_Delay_ms(2);
//    DEV_Digital_Write(EPD_RST_PIN, 1);
//    DEV_Delay_ms(200);
//}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
/**
  GPIOTE中断处理
// */
//void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
//{

////	Epaper_Write_Command(0x71);

//	  EPD_7IN5B_V2_WaitUntilIdle();

//}
///**
//配置GPIOTE初始化
// */
// void gpio_init(void)
//{
//  
//	  ret_code_t err_code;

//    err_code = nrf_drv_gpiote_init();
//    APP_ERROR_CHECK(err_code);

////GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);//GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);//
//    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
//    in_config.pull = NRF_GPIO_PIN_PULLDOWN;

//    err_code = nrf_drv_gpiote_in_init(pingmu_BUSY_IO, &in_config, in_pin_handler);
//    APP_ERROR_CHECK(err_code);

//    nrf_drv_gpiote_in_event_enable(pingmu_BUSY_IO, true);
//}






void EPD_7IN5B_V2_WaitUntilIdle(void)
{
	NRF_LOG_INFO("e-Paper busy\r\n");
	unsigned char busy;
	do{
		Epaper_Write_Command(0x71);
		busy = nrf_gpio_pin_read(pingmu_BUSY_IO);
		busy =!(busy & 0x01);      
	}while(busy);//原本为1
//	nrf_delay_ms(200);      
	NRF_LOG_INFO("e-Paper busy release\r\n");
		
}





/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
void EPD_7IN5B_V2_TurnOnDisplay(void)
{
    Epaper_Write_Command(0x12);			//DISPLAY REFRESH
 //   nrf_delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!
    EPD_7IN5B_V2_WaitUntilIdle();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_7IN5B_V2_Init(void)
{
		NRF_LOG_INFO("7.5B V2");
    RESET();

    Epaper_Write_Command(0x01);			//POWER SETTING
    Epaper_Write_Data(0x07);
    Epaper_Write_Data(0x07);    //VGH=20V,VGL=-20V
    Epaper_Write_Data(0x3f);		//VDH=15V
    Epaper_Write_Data(0x3f);		//VDL=-15V

    Epaper_Write_Command(0x04); //POWER ON
   nrf_delay_ms(100);
    EPD_7IN5B_V2_WaitUntilIdle();

    Epaper_Write_Command(0X00);			//PANNEL SETTING
    Epaper_Write_Data(0x0F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    Epaper_Write_Command(0x61);        	//tres
    Epaper_Write_Data(0x03);		//source 800
    Epaper_Write_Data(0x20);
    Epaper_Write_Data(0x01);		//gate 480
    Epaper_Write_Data(0xE0);

    Epaper_Write_Command(0X15);
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0X50);			//VCOM AND DATA INTERVAL SETTING
    Epaper_Write_Data(0x11);
    Epaper_Write_Data(0x07);

    Epaper_Write_Command(0X60);			//TCON SETTING
    Epaper_Write_Data(0x22);
		
		Epaper_Write_Command(0X30);			//PLL
		Epaper_Write_Data(0x3f);
		
//		Epaper_Write_Command(0X82);			//VCOM_DC
//		Epaper_Write_Data(0x08);
//		Epaper_Write_Command(0X30);			//PLL
//		Epaper_Write_Data(0x06);

//		  UWORD i;
//    Epaper_Write_Command(0x10);
//    for(i=0; i<800 / 8 *480; i++) {
//        Epaper_Write_Data(0x00);

//    }
//    Epaper_Write_Command(0x13);
//    for(i=0; i<800 / 8 *480; i++)	{
//        Epaper_Write_Data(0x00);
//    }

//    Epaper_Write_Command(0x10);		

    return 0;
}



void wave_7in5b_epd_init(void) 
{
	
  
  // EPD 硬件复位释放    Hard  reset  release
   RESET();
	
  Epaper_Write_Command(0x40);  
  EPD_7IN5B_V2_WaitUntilIdle();
  
 // EPD_SPI_CS_UP;
	
  Epaper_Write_Command(0x61);  // Resolution setting
  Epaper_Write_Data(0x03);		 //source 800
  Epaper_Write_Data(0x20);
  Epaper_Write_Data(0x01);		 //gate 480
  Epaper_Write_Data(0xE0);
	
  Epaper_Write_Command(0x62);  // Resolution setting
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x10);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);
  
  Epaper_Write_Command(0xF8);  // Enter FITI Command
  Epaper_Write_Data(0x60); 
  Epaper_Write_Data(0xA5); 

  Epaper_Write_Command(0xF8);  // Enter FITI Command
  Epaper_Write_Data(0x89); 
  Epaper_Write_Data(0xA5); 

  Epaper_Write_Command(0xF8);  
  Epaper_Write_Data(0x76);
  Epaper_Write_Data(0x1F);
                          
  Epaper_Write_Command(0xF8);   //boost constant on time
  Epaper_Write_Data(0x7E);
  Epaper_Write_Data(0x31); 
  
  Epaper_Write_Command(0xF8);  
  Epaper_Write_Data(0xB8);  
  Epaper_Write_Data(0x80);  
  
  Epaper_Write_Command(0xF8);   //vgl=> GND:08, HZ:00[default]
  Epaper_Write_Data(0x92);  
  Epaper_Write_Data(0x00);  
  
  Epaper_Write_Command(0xF8);   //VCOM(2frme off)=> GND:01(0x88=06)[default], HZ:11  
  Epaper_Write_Data(0x87);  
  Epaper_Write_Data(0x01);
  
  Epaper_Write_Command(0xF8);   //r_vcom_init_sel
  Epaper_Write_Data(0x88);  
  Epaper_Write_Data(0x06); 
                          
  Epaper_Write_Command(0xF8);   
  Epaper_Write_Data(0xA8);  
  Epaper_Write_Data(0x30); 
  
  Epaper_Write_Command(0xF8);   
  Epaper_Write_Data(0xE8); 
  Epaper_Write_Data(0x88); 
	
  Epaper_Write_Command(0x04);  //power on  
   EPD_7IN5B_V2_WaitUntilIdle();
  
#if defined(EPD_BLANK_ON_INIT) 
	Epaper_Write_Command(0x10);
	for (uint16_t i=0; i<EPD_SCREEN_DATA_SIZE; i++) {
		Epaper_Write_Data(0x00);
	}
	Epaper_Write_Command(0x13);
	for (uint16_t i=0; i<EPD_SCREEN_DATA_SIZE; i++) {
		Epaper_Write_Data(0x00);
	}
	wave_7in5b_hal_ctx.has_red = true;
#endif   
  
#if defined(USE_DEBUG_MODE)
  SERVICE_log("Prepare Waveshare 7in5b display completed.\r\n");
#endif
}



/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5B_V2_Clear(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5B_V2_HEIGHT;

    UWORD i;
    Epaper_Write_Command(0x10);
    for(i=0; i<Width*Height; i++) {
        Epaper_Write_Data(0xff);

    }
    Epaper_Write_Command(0x13);
    for(i=0; i<Width*Height; i++)	{
        Epaper_Write_Data(0x00);

    }
    EPD_7IN5B_V2_TurnOnDisplay();
}

void EPD_7IN5B_V2_ClearRed(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5B_V2_HEIGHT;

    UWORD i;
    Epaper_Write_Command(0x10);
    for(i=0; i<Width*Height; i++) {
        Epaper_Write_Data(0xff);

    }
    Epaper_Write_Command(0x13);
    for(i=0; i<Width*Height; i++)	{
        Epaper_Write_Data(0xff);

    }
    EPD_7IN5B_V2_TurnOnDisplay();
}

void EPD_7IN5B_V2_ClearBlack(void)
{
	NRF_LOG_INFO("BLACK");
    UWORD Width, Height;
    Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5B_V2_HEIGHT;

    UWORD i;
    Epaper_Write_Command(0x10);
    for(i=0; i<Width*Height; i++) {
        Epaper_Write_Data(0x00);

    }
    Epaper_Write_Command(0x13);
    for(i=0; i<Width*Height; i++)	{
        Epaper_Write_Data(0x00);

    }
    EPD_7IN5B_V2_TurnOnDisplay();
}





/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5B_V2_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UDOUBLE Width, Height;
   Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
   Height = EPD_7IN5B_V2_HEIGHT;                        //800  480

	//	Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8): (EPD_WIDTH / 8 + 1);
   // Height = EPD_HEIGHT;
	
 //send black data
    Epaper_Write_Command(0x10);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            Epaper_Write_Data(blackimage[i + j * Width]);
        }
    }
    Epaper_Write_Command(0x92); 

    //send red data
    Epaper_Write_Command(0x13);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            Epaper_Write_Data(~ryimage[i + j * Width]);
        }
    }
   EPD_7IN5B_V2_TurnOnDisplay();
}

//0 1 is a black area and 2 3 is a red area
void EPD_7IN5B_V2_WritePicture(const UBYTE *blackimage, UBYTE Block)
{

	UDOUBLE Width, Height;
	Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
	Height = EPD_7IN5B_V2_HEIGHT;
	
	if(Block == 0){
		Epaper_Write_Command(0x10);
	}else if(Block == 2){
		Epaper_Write_Command(0x92); 
    Epaper_Write_Command(0x13);
	}
	for (UDOUBLE j = 0; j < Height/2; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
						if(Block>=2){
							Epaper_Write_Data(~blackimage[i + j * Width]);
						}else{
							Epaper_Write_Data(blackimage[i + j * Width]);
						}
        }
	}
	if(Block == 3){
		EPD_7IN5B_V2_TurnOnDisplay();
	}
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5B_V2_Sleep(void)
{
    Epaper_Write_Command(0X02);  	//power off
    EPD_7IN5B_V2_WaitUntilIdle();
    Epaper_Write_Command(0X07);  	//deep sleep
    Epaper_Write_Data(0xA5);
}


uint8_t EPD_Init(void) 
{
    RESET();
	
    Epaper_Write_Command(0x01);
    Epaper_Write_Data(0x37);
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0x00);
    Epaper_Write_Data(0xCF);
    Epaper_Write_Data(0x08);

    Epaper_Write_Command(0x30);
    Epaper_Write_Data(0x3A);

    Epaper_Write_Command(0x82);
    Epaper_Write_Data(0x28);

    Epaper_Write_Command(0x06);
    Epaper_Write_Data (0xc7);
    Epaper_Write_Data (0xcc);
    Epaper_Write_Data (0x15);

    Epaper_Write_Command(0x50);
    Epaper_Write_Data(0x77);

    Epaper_Write_Command(0x60);
    Epaper_Write_Data(0x22);

    Epaper_Write_Command(0x65);
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0x61);
		
		
		
    Epaper_Write_Data(EPD_WIDTH >> 8);
    Epaper_Write_Data(EPD_WIDTH & 0xff);
    Epaper_Write_Data(EPD_HEIGHT >> 8);
    Epaper_Write_Data(EPD_HEIGHT & 0xff);

    Epaper_Write_Command(0xe5);
    Epaper_Write_Data(0x03);

    return 0;
}


void EPD_5IN83BC_Init(void)//微雪
{
    RESET();

    Epaper_Write_Command(0x01); // POWER_SETTING
    Epaper_Write_Data(0x37);
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0x00); // PANEL_SETTING
    Epaper_Write_Data(0xCF);
    Epaper_Write_Data(0x08);

    Epaper_Write_Command(0x30); // PLL_CONTROL
    Epaper_Write_Data(0x3A); //PLL:  0-15:0x3C, 15+:0x3A

    Epaper_Write_Command(0x82); // VCM_DC_SETTING
    Epaper_Write_Data(0x28); //all temperature  range

    Epaper_Write_Command(0x06); // BOOSTER_SOFT_START
    Epaper_Write_Data (0xc7);
    Epaper_Write_Data (0xcc);
    Epaper_Write_Data (0x15);

    Epaper_Write_Command(0x50); // VCOM AND DATA INTERVAL SETTING
    Epaper_Write_Data(0x77);

    Epaper_Write_Command(0x60); // TCON_SETTING
    Epaper_Write_Data(0x22);

    Epaper_Write_Command(0x65); // FLASH CONTROL
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0x61); // TCON_RESOLUTION
    Epaper_Write_Data (0x02); // source 600
    Epaper_Write_Data (0x58);
    Epaper_Write_Data (0x01); // gate 448
    Epaper_Write_Data (0xc0);

    Epaper_Write_Command(0xe5); // FLASH MODE
    Epaper_Write_Data(0x03);

}




void EPD_Clear(void)
{
    uint16_t Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    Epaper_Write_Command(0x10);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            for(uint8_t k = 0; k < 4; k++) {
                Epaper_Write_Data(0x33);
            }
        }
    }
    EPD_7IN5B_V2_TurnOnDisplay();
}

 void BOE0583CC26XX_cmdSetOptions(void)
{
	
		RESET();
//  EPD_SPI_RST_UP;
//  DELAY_MS(10); 
//  EPD_SPI_RST_DOWN;
//  DELAY_MS(10); 
//  EPD_SPI_RST_UP;
//  DELAY_MS(10); 
	
  Epaper_Write_Command(0x40);  
  // After power on command, BUSY_N signal will drop from high to low. When finishing the power off sequence, 
  // BUSY_N signal will rise from low to high.
  EPD_7IN5B_V2_WaitUntilIdle();
  
  SPI_CS_HIGH;
	
  Epaper_Write_Command(0x61);  // Resolution setting
  Epaper_Write_Data(0x02);
  // After power off command, BUSY_N signal will drop from high to low. When finish the power off sequence,
  // BUSY_N signal will rise from low to high.
  EPD_7IN5B_V2_WaitUntilIdle();
  
  Epaper_Write_Data(0x88);
  Epaper_Write_Data(0x01);
  Epaper_Write_Data(0xE0);
  
  Epaper_Write_Command(0x62);  // Resolution setting
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x10);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);

  Epaper_Write_Command(0xF8);  // Enter FITI Command
  Epaper_Write_Data(0x60); 
  Epaper_Write_Data(0xA5); 

  Epaper_Write_Command(0xF8);  // Enter FITI Command
  Epaper_Write_Data(0x89); 
  Epaper_Write_Data(0xA5); 

  Epaper_Write_Command(0xF8);  
  Epaper_Write_Data(0x76);
  Epaper_Write_Data(0x1F);
                          
  Epaper_Write_Command(0xF8);   //boost constant on time
  Epaper_Write_Data(0x7E);
  Epaper_Write_Data(0x31); 
  
  Epaper_Write_Command(0xF8);  
  Epaper_Write_Data(0xB8);  
  Epaper_Write_Data(0x80);  
  
  Epaper_Write_Command(0xF8);   //vgl=> GND:08, HZ:00[default]
  Epaper_Write_Data(0x92);  
  Epaper_Write_Data(0x00);  
  
  Epaper_Write_Command(0xF8);   //VCOM(2frme off)=> GND:01(0x88=06)[default], HZ:11  
  Epaper_Write_Data(0x87);  
  Epaper_Write_Data(0x01);
  
  Epaper_Write_Command(0xF8);   //r_vcom_init_sel
  Epaper_Write_Data(0x88);  
  Epaper_Write_Data(0x06); 
                          
  Epaper_Write_Command(0xF8);   
  Epaper_Write_Data(0xA8);  
  Epaper_Write_Data(0x30); 
  
  Epaper_Write_Command(0xF8);   
  Epaper_Write_Data(0xE8); 
  Epaper_Write_Data(0x88); 

  Epaper_Write_Command(0x04);  //power on  
  // After power on command, BUSY_N signal will drop from high to low. When finishing the power off sequence,
  // BUSY_N signal will rise from low to high.
  EPD_7IN5B_V2_WaitUntilIdle();
#if defined(USE_DEBUG_MODE)
  Display_printf(dispHandle, SP_ROW_DEBUG_BOE_0583+2, 0, "Set BOE0583 options success.\r\n");
#endif
}

static void BOE0583CC26XX_cmdSetResolution()
{
  Epaper_Write_Command(0x61);  // Resolution setting
  Epaper_Write_Data(0x02);
  // After power off command, BUSY_N signal will drop from high to low. When finish the power off sequence,
  // BUSY_N signal will rise from low to high.
  EPD_7IN5B_V2_WaitUntilIdle();
  Epaper_Write_Data(0x88);
  Epaper_Write_Data(0x01);
  Epaper_Write_Data(0xE0);  
}