#ifndef _EPD_7IN5B_V2_H_
#define _EPD_7IN5B_V2_H_

#include <stdint.h>
#define UBYTE uint8_t
#define UWORD uint16_t
#define UDOUBLE uint32_t

// Display resolution
#define EPD_7IN5B_V2_WIDTH       800
#define EPD_7IN5B_V2_HEIGHT      480

#define EPD_WIDTH       640
#define EPD_HEIGHT      384

void EPD_7IN5B_V2_WaitUntilIdle(void);
 void EPD_7IN5B_V2_TurnOnDisplay(void);

UBYTE EPD_7IN5B_V2_Init(void);
void EPD_7IN5B_V2_Clear(void);
void EPD_7IN5B_V2_ClearRed(void);
void EPD_7IN5B_V2_ClearBlack(void);
void EPD_7IN5B_V2_Display(const UBYTE *blackimage, const UBYTE *ryimage);
void EPD_7IN5B_V2_Sleep(void);
void EPD_7IN5B_V2_WritePicture(const UBYTE *blackimage, UBYTE Block);
void wave_7in5b_epd_init(void);
uint8_t EPD_Init(void);

void EPD_5IN83BC_Init(void);
void EPD_Clear(void);


void BOE0583CC26XX_cmdSetOptions(void);



void gpio_init(void);







#endif
