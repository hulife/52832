
#include "spi_lib.h"
#include "spi_ble.h"
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"	
//#include "Image_583.h" 
#include "board_spi.h"
//5.83Çý¶¯

 

void EPD_5IN83B_V2_WaitUntilIdle(void)
{
  //Debug("e-Paper busy\r\n");
	uint8_t busy;
	do
	{
		Epaper_Write_Command(0x71);
		busy = nrf_gpio_pin_read(pingmu_BUSY_IO);
		busy =!(busy & 0x01);
	}
	while(busy);   
	nrf_delay_ms(200);     
  //Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_5IN83B_V2_TurnOnDisplay(void)
{
	Epaper_Write_Command(0x12);			//DISPLAY REFRESH 	
	nrf_delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!     
	EPD_5IN83B_V2_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
uint8_t EPD_5IN83B_V2_Init(void)
{
	RESET();
	EPD_5IN83B_V2_WaitUntilIdle();
	Epaper_Write_Command(0x01);			//POWER SETTING
	Epaper_Write_Data (0x07);
	Epaper_Write_Data (0x07);    //VGH=20V,VGL=-20V
	Epaper_Write_Data (0x3f);		//VDH=15V
	Epaper_Write_Data (0x3f);		//VDL=-15V

	Epaper_Write_Command(0x04); //POWER ON
	nrf_delay_ms(100);  
	EPD_5IN83B_V2_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

	Epaper_Write_Command(0X00);			//PANNEL SETTING
	Epaper_Write_Data(0x0F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

	Epaper_Write_Command(0x61);        	//tres			
	Epaper_Write_Data (0x02);		//source 648
	Epaper_Write_Data (0x88);
	Epaper_Write_Data (0x01);		//gate 480
	Epaper_Write_Data (0xe0);

	Epaper_Write_Command(0X15);		
	Epaper_Write_Data(0x00);		

	Epaper_Write_Command(0X50);			//VCOM AND DATA INTERVAL SETTING
	Epaper_Write_Data(0x11);
	Epaper_Write_Data(0x07);

	Epaper_Write_Command(0X60);			//TCON SETTING
	Epaper_Write_Data(0x22);

	return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5IN83B_V2_Clear(void)
{
	uint16_t Width, Height;
	Width =(EPD_5IN83B_V2_WIDTH % 8 == 0)?(EPD_5IN83B_V2_WIDTH / 8 ):(EPD_5IN83B_V2_WIDTH / 8 + 1);
	Height = EPD_5IN83B_V2_HEIGHT;
	uint16_t i;
	Epaper_Write_Command(0x10);
	for(i=0; i<Width*Height; i++) {
		Epaper_Write_Data(0xff);

	}
	Epaper_Write_Command(0x13);
	for(i=0; i<Width*Height; i++)	{
		Epaper_Write_Data(0x00);

	}
	EPD_5IN83B_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5IN83B_V2_Clear_C(uint8_t color)
{
	uint16_t Width, Height;
	Width =(EPD_5IN83B_V2_WIDTH % 8 == 0)?(EPD_5IN83B_V2_WIDTH / 8 ):(EPD_5IN83B_V2_WIDTH / 8 + 1);
	Height = EPD_5IN83B_V2_HEIGHT;
	uint16_t i;
	if(color == 0) {
	Epaper_Write_Command(0x10);
	for(i=0; i<Width*Height; i++) {
		Epaper_Write_Data(0xff);

	}
	Epaper_Write_Command(0x13);
	for(i=0; i<Width*Height; i++)	{
		Epaper_Write_Data(0x00);

	}
}
	else if(color == 1) {
	Epaper_Write_Command(0x10);
	for(i=0; i<Width*Height; i++) {
		Epaper_Write_Data(0xff);

	}
	Epaper_Write_Command(0x13);
	for(i=0; i<Width*Height; i++)	{
		Epaper_Write_Data(0xff);

	}
}
	else if(color == 2) {
	Epaper_Write_Command(0x10);
	for(i=0; i<Width*Height; i++) {
		Epaper_Write_Data(0x00);

	}
	Epaper_Write_Command(0x13);
	for(i=0; i<Width*Height; i++)	{
		Epaper_Write_Data(0x00);

	}
}
	EPD_5IN83B_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5IN83B_V2_Display(const uint8_t *blackimage, const uint8_t *ryimage)
{
	uint32_t Width, Height;
	Width =(EPD_5IN83B_V2_WIDTH % 8 == 0)?(EPD_5IN83B_V2_WIDTH / 8 ):(EPD_5IN83B_V2_WIDTH / 8 + 1);
	Height = EPD_5IN83B_V2_HEIGHT;
	//send black data
	Epaper_Write_Command(0x10);
	for (uint32_t j = 0; j < Height; j++) {
		for (uint32_t i = 0; i < Width; i++) {
			Epaper_Write_Data(blackimage[i + j * Width]);
		}
	}
	//send red data
	Epaper_Write_Command(0x13);
	for (uint32_t j = 0; j < Height; j++) {
		for (uint32_t i = 0; i < Width; i++) {
			Epaper_Write_Data(~ryimage[i + j * Width]);
		}
	}
	EPD_5IN83B_V2_TurnOnDisplay();
}

void EPD_5IN83B_V2_DisplayPicture(const uint8_t *blackimage,uint8_t Block)
{
	uint32_t Width, Height;
	Width =(EPD_5IN83B_V2_WIDTH % 8 == 0)?(EPD_5IN83B_V2_WIDTH / 8 ):(EPD_5IN83B_V2_WIDTH / 8 + 1);
	Height = EPD_5IN83B_V2_HEIGHT;

	if(Block == 0){
		Epaper_Write_Command(0x10);
    for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(blackimage[i + j * Width]);
        }
    }		
	}else if(Block == 1){
		for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(blackimage[i + j * Width]);
        }
    }
	}else if(Block == 2){
		for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(blackimage[i + j * Width]);
        }
    }
	}else if(Block == 3){
		for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(blackimage[i + j * Width]);
        }
    }
	}
	else if(Block == 4){
		Epaper_Write_Command(0x13);
    for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(~blackimage[i + j * Width]);
        }
    }
	}else if(Block == 5){
		for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(~blackimage[i + j * Width]);
        }
    }
	}else if(Block == 6){
		for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(~blackimage[i + j * Width]);
        }
    }
	}else if(Block == 7){
		for (uint32_t j = 0; j < Height/4; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            Epaper_Write_Data(~blackimage[i + j * Width]);
        }
    }
		EPD_5IN83B_V2_TurnOnDisplay();
	}
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5IN83B_V2_Sleep(void)
{
	Epaper_Write_Command(0X02);  	//power off
	EPD_5IN83B_V2_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal
	Epaper_Write_Command(0X07);  	//deep sleep
	Epaper_Write_Data(0xA5);
}
