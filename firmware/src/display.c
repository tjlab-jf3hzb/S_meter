#include "display.h"
#include "app.h"

//--------------------------------------
#define CS     LATBbits.LATB15
#define DC     LATBbits.LATB9
#define RST    LATBbits.LATB8
    

// GRAM
uint8_t R_GRAM[Nx][Ny], G_GRAM[Nx][Ny], B_GRAM[Nx][Ny], A_GRAM[Nx][Ny];

void wait_1us(long t)
{
    long k;
    for(k=0; k<t; k++) {
        _nop(); _nop();  _nop();  _nop();  _nop();
        _nop(); _nop();  _nop();  _nop();  _nop();    
        _nop(); _nop();  _nop();  _nop();  _nop();
        _nop(); _nop();  _nop();  _nop();  _nop();
        _nop(); _nop();  _nop();  _nop();  _nop();    
        _nop(); _nop();  _nop();  _nop();  _nop();
        _nop(); _nop();  _nop();  _nop();  _nop();
        _nop(); _nop();  _nop();  _nop();  _nop();    
        _nop(); _nop();  _nop();  _nop();  _nop(); 
    }
}



//---------------------------------------------------------------------------------
#ifdef SSD1331
//---------------------------------------------------------------------------------
void Transfer_Image() {   
    int xp, yp;
    uint8_t cl;
    
    for(yp=Ny-1; yp>=0; yp--) {
        for(xp=0; xp<Nx; xp++) {
            
            cl=( R_GRAM[xp][yp] & 0xF8) | (G_GRAM[xp][yp]>>5 ) ;            
            while(PLIB_SPI_IsBusy(SPI_ID_1) );
            CS=1;
            CS=0;
            PLIB_SPI_BufferWrite16bit(SPI_ID_1, cl);  //RG

            cl=( (G_GRAM[xp][yp]<<3) & 0xE0) | ( (B_GRAM[xp][yp]>>3) & 0x1F) ;            
            while(PLIB_SPI_IsBusy(SPI_ID_1) );
            CS=1;
            CS=0;
            PLIB_SPI_BufferWrite16bit(SPI_ID_1, cl);  //GB
        }
    }
    while(PLIB_SPI_IsBusy(SPI_ID_1) );
    CS=1; 
}

/*-----------------------------------------------------------------
   Initialize SSD1331
------------------------------------------------------------------ */
void display_init(void){  
 
    int xp, yp;    
    for(yp=0; yp<Ny; yp++) {
        for(xp=0; xp<Nx; xp++) {
            R_GRAM[xp][yp]=0x00;
            G_GRAM[xp][yp]=0x00;
            B_GRAM[xp][yp]=0x00;
        }
    }
       
    CS=1;
    DC=1;
  
    //Hardware Reset------------
    RST=1;
    wait_1us(10000);
    RST=0;
    wait_1us(50000);
    RST=1;
    wait_1us(50000);
  
    d_Command(0xAE); //Set Display Off
    d_Command(0xA0); //Remap & Color Depth setting@
    d_Command(0b01110010); //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format
    d_Command(0xA1); //Set Display Start Line
    d_Command(0);
    d_Command(0xA2); //Set Display Offset
    d_Command(0);
    d_Command(0xA4); //Set Display Mode (Normal)
    d_Command(0xA8); //Set Multiplex Ratio
    d_Command(63); //15-63
    d_Command(0xAD); //Set Master Configration
    d_Command(0b10001110); //a[0]=0 Select external Vcc supply, a[0]=1 Reserved(reset)
    d_Command(0xB0); //Power Save Mode
    d_Command(0x1A); //0x1A Enable power save mode. 0x00 Disable
    d_Command(0xB1); //Phase 1 and 2 period adjustment
    d_Command(0x74);
    d_Command(0xB3); //Display Clock DIV
    d_Command(0xF0);
    d_Command(0x8A); //Pre Charge A
    d_Command(0x81);
    d_Command(0x8B); //Pre Charge B
    d_Command(0x82);
    d_Command(0x8C); //Pre Charge C
    d_Command(0x83);
    d_Command(0xBB); //Set Pre-charge level
    d_Command(0x3A);
    d_Command(0xBE); //Set VcomH
    d_Command(0x3E);
    d_Command(0x87); //Set Master Current Control
    d_Command(0x06);
    d_Command(0x15); //Set Column Address
    d_Command(0);
    d_Command(95);
    d_Command(0x75); //Set Row Address
    d_Command(0);
    d_Command(63);
    d_Command(0x81); //Set Contrast for Color A
    d_Command(255);
    d_Command(0x82); //Set Contrast for Color B
    d_Command(255);
    d_Command(0x83); //Set Contrast for Color C
    d_Command(255);
    d_Command(0xAF); //Set Display On
    wait_1us(200000); // >100ms
}
#endif



/*------------------------------------------
       Command write
-------------------------------------------*/ 
void d_Command(uint8_t d){
    DC=0;
    CS=1;                 
    CS=0;                
    PLIB_SPI_BufferWrite16bit(SPI_ID_1, d);
    while(PLIB_SPI_IsBusy(SPI_ID_1) );        
    CS=1;
    DC=1;
}



