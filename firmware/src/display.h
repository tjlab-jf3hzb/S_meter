/* 
 * File:   display.h
 * Author: t_uebo
 *
 * Created on 2019/09/18, 02:00
 */

#ifndef ____display____
#define	____display____

#ifdef	__cplusplus
extern "C" {
#endif

    
/*----------------------------------------
 Select Display controller
   ST7735 : LCD  (1.8inch 128x160 TFT)
   SEPS525: OLED ( NHD-1.69-160128UGC3 ) 
-----------------------------------------*/

//#define SEPS525
//#define ST7735
#define SSD1331

/*---------------------------------------*/

#define Nx 96
#define Ny 64

void display_init(void);
void Transfer_Image(void);
void d_Command(unsigned char d);
void wait_1us(long);

#ifdef	__cplusplus
}
#endif

#endif	/* ____display____ */

