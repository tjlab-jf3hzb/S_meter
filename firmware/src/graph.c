/* 
 * File:   graph.cpp
 * Author: JF3HZB / T.UEBO
 *
 * Created on 2017/12/08, 00:21
 */

#include <math.h>
#include <stdio.h>
#include "app.h"
#include "display.h"
#include "graph.h"
#include "font.h"

#include "MT_img1.h"
#include "MT_img2.h"
#include "MT_img3.h"
#include "MT_img4.h"
#include "MT_img5.h"
#include "MT_img6.h"


extern uint8_t R_GRAM[Nx][Ny], G_GRAM[Nx][Ny], B_GRAM[Nx][Ny], A_GRAM[Nx][Ny];


void dot(float x, float y)
{
  int xd, yd, xu, yu;
  float Rxu, Rxd, Ryu, Ryd;
  unsigned int dat;

  xd = (int)x; yd = (int)y;

  if ( xd >= 0 && xd < Nx - 1 && yd >= 0 && yd < Ny - 1) {

    xu = xd + 1; yu = yd + 1;
    Rxd = ( (float)xu - x );
    Rxu = ( x - (float)xd );
    Ryd = ( (float)yu - y );
    Ryu = ( y - (float)yd );
    
    if(Ryd>=0 && Ryu>0 && Rxd>=0 && Rxu>=0){
        dat = (int) A_GRAM[xd][yd] + (int) (Rxd * Ryd * 256.0);
        if (dat > 0xFF) dat = 0xFF;
        A_GRAM[xd][yd] = (unsigned char) dat;

        dat = (unsigned int) A_GRAM[xu][yd] + (unsigned int) (Rxu * Ryd * 256.0);
        if (dat > 0xFF) dat = 0xFF;
        A_GRAM[xu][yd] = (unsigned char) dat;

        dat = (unsigned int) A_GRAM[xd][yu] + (unsigned int) (Rxd * Ryu * 256.0);
        if (dat > 0xFF) dat = 0xFF;
        A_GRAM[xd][yu] = (unsigned char) dat;

        dat = (unsigned int) A_GRAM[xu][yu] + (unsigned int) (Rxu * Ryu * 256.0);
        if (dat > 0xFF) dat = 0xFF;
        A_GRAM[xu][yu] = (unsigned char) dat;
    }
  }
}


void Copy_img(char img_ID, char *str){
    int xp, yp;
    
    switch(img_ID){
            
        case 0:
            for (xp = 0; xp < Nx; xp++) {
                for (yp = 0; yp < Ny; yp++) {
                    R_GRAM[xp][yp] = R_MT_img5[xp][yp];
                    G_GRAM[xp][yp] = G_MT_img5[xp][yp];
                    B_GRAM[xp][yp] = B_MT_img5[xp][yp];
                }
            }
            disp_str8(str, 38,4, 0x000000);
            break;
            
        case 1:
            for (xp = 0; xp < Nx; xp++) {
                for (yp = 0; yp < Ny; yp++) {
                    R_GRAM[xp][yp] = R_MT_img4[xp][yp];
                    G_GRAM[xp][yp] = G_MT_img4[xp][yp];
                    B_GRAM[xp][yp] = B_MT_img4[xp][yp];
                }
            }
            disp_str8(str, 38,4, 0x000000);
            break;

            
            
        case 2:
            for (xp = 0; xp < Nx; xp++) {
                for (yp = 0; yp < Ny; yp++) {
                    R_GRAM[xp][yp] = R_MT_img2[xp][yp];
                    G_GRAM[xp][yp] = G_MT_img2[xp][yp];
                    B_GRAM[xp][yp] = B_MT_img2[xp][yp];
                }
            }
            disp_str8(str, 38,4, 0x808080);
            break;


            
        case 3:
            for (xp = 0; xp < Nx; xp++) {
                for (yp = 0; yp < Ny; yp++) {
                    R_GRAM[xp][yp] = R_MT_img3[xp][yp];
                    G_GRAM[xp][yp] = G_MT_img3[xp][yp];
                    B_GRAM[xp][yp] = B_MT_img3[xp][yp];
                }
            }
            disp_str8(str, 38,4, 0x008000);
            break;


            
        case 4:
            for (xp = 0; xp < Nx; xp++) {
                for (yp = 0; yp < Ny; yp++) {
                    R_GRAM[xp][yp] = R_MT_img6[xp][yp];
                    G_GRAM[xp][yp] = G_MT_img6[xp][yp];
                    B_GRAM[xp][yp] = B_MT_img6[xp][yp];
                }
            }
            disp_str8(str, 38,4, 0x000000);
            break;
 
            
        case 5:
            for (xp = 0; xp < Nx; xp++) {
                for (yp = 0; yp < Ny; yp++) {
                    R_GRAM[xp][yp] = R_MT_img1[xp][yp];
                    G_GRAM[xp][yp] = G_MT_img1[xp][yp];
                    B_GRAM[xp][yp] = B_MT_img1[xp][yp];
                }
            }
            disp_str8(str, 38,4, 0x008080);
            break;

            
        default:
            for (xp = 0; xp < Nx; xp++) {
                for (yp = 0; yp < Ny; yp++) {
                    R_GRAM[xp][yp] = R_MT_img5[xp][yp];
                    G_GRAM[xp][yp] = G_MT_img5[xp][yp];
                    B_GRAM[xp][yp] = B_MT_img5[xp][yp];
                }
            }
            disp_str8(str, 38,4, 0x000000);
            break;
   
    }
    
    
}

void needle(int v, int fs, int wd, long cl){
    float f, angle, xo, yo, COS, SIN;
    int xp,yp;
    
    //--- 針の位置，長さ, 幅 ---------------
    int nd_length=Ny-7;
    int nd_width=2+wd;
    int rot_center=-50;
    
    
    for(xp=0; xp<Nx; xp++){
        for(yp=0; yp<Ny; yp++){
            A_GRAM[xp][yp]=0;
        }
    }
    
    angle=(float)(v)/(float)fs-0.5;
    angle*=0.9;
    
    COS=cos(angle);
    SIN=sin(angle);
    

    float x_tmp;
    int k,j;
    for(j=0; j<nd_width; j++){
        for(k=0; k<nd_length; k++){            
            x_tmp=(float)j-0.5*(float)(nd_width-1);
            xo= COS*x_tmp  -SIN*(float)(k-rot_center);
            yo=COS*(float)(k-rot_center) + SIN*x_tmp;
            dot( (float)(Nx>>1)-xo, yo+rot_center);            
        }
    }
    
//--- 針の色 -------------------------------   
    float ND_R= (float)( (cl>>16) & 0xff ); 
    float ND_G= (float)( (cl>> 8) & 0xff ); 
    float ND_B= (float)(  cl & 0xff ); 
    
// αブレンド：確定色 = 背景色 + (重ねる色 - 背景色) * (アルファ値 / 255)
    float c, a; 
    
    for(xp=0; xp<Nx; xp++){
        for(yp=0; yp<Ny; yp++){
            if(A_GRAM[xp][yp]!=0){
                
                a=(float)A_GRAM[xp][yp];
                
                c=(float)R_GRAM[xp][yp];
                R_GRAM[xp][yp]=(uint8_t)( c + (ND_R - c)*(a/255.0) );
                
                c=(float)G_GRAM[xp][yp];
                G_GRAM[xp][yp]=(uint8_t)( c + (ND_G - c)*(a/255.0) );
                
                c=(float)B_GRAM[xp][yp];
                B_GRAM[xp][yp]=(uint8_t)( c + (ND_B - c)*(a/255.0) );

            }  
        }
    }
   
    
}


// Claer GRAM ------------------------------------------------------------------
void GRAM_clr(void)
{
    boxfill(0,0,Nx-1,Ny-1,0);
}

//--------------------------------------------------------------------------
void boxfill(int x_min, int y_min, int x_max, int y_max, unsigned long color)
{
    int k, j;
	union {
		unsigned long  lw[1];
		unsigned char  byte[4];		
	} cl_data;
	cl_data.lw[0]=color;
   
    if(x_min<0) x_min=0;
    if(y_min<0) y_min=0;
    if(x_max>=Nx) x_max=Nx-1;
    if(y_max>=Ny) y_max=Ny-1;   
    
    for(k=x_min;k<=x_max; k++){
        for(j=y_min; j<=y_max; j++){
			R_GRAM[k][j]=cl_data.byte[2];
			G_GRAM[k][j]=cl_data.byte[1];
			B_GRAM[k][j]=cl_data.byte[0];
		}
    }
}

//--------------------------------------------------------------------------
void line(int xs, int ys,int xe, int ye, unsigned long color)
{
	union {
		unsigned long  lw[1];
		unsigned char  byte[4];		
	} cl_data;
	cl_data.lw[0]=color;

    
	int dx = (xe > xs ? xe - xs : xs - xe);
	int xstep =  xe > xs ? 1 : -1;
	int dy = (ye > ys ? ye - ys : ys - ye);
	int ystep =  ye > ys ? 1 : -1;
	int j;
	
	if(dx==0 && dy==0){
		R_GRAM[xs][ys]=cl_data.byte[2];
		G_GRAM[xs][ys]=cl_data.byte[1];		
		B_GRAM[xs][ys]=cl_data.byte[0];		
		
	} else if(dx==0){
		if(ystep>0) for(j=ys; j<=ye; j++){
			R_GRAM[xs][j]=cl_data.byte[2];
			G_GRAM[xs][j]=cl_data.byte[1];
			B_GRAM[xs][j]=cl_data.byte[0];
		}
		if(ystep<0) for(j=ye; j<=ys; j++){
			R_GRAM[xs][j]=cl_data.byte[2];
			G_GRAM[xs][j]=cl_data.byte[1];
			B_GRAM[xs][j]=cl_data.byte[0];			
		}
	} else if(dy==0){
		if(xstep>0) for(j=xs; j<=xe; j++){
			R_GRAM[j][ys]=cl_data.byte[2];
			G_GRAM[j][ys]=cl_data.byte[1];
			B_GRAM[j][ys]=cl_data.byte[0];						
		}
		if(xstep<0) for(j=xe; j<=xs; j++){
			R_GRAM[j][ys]=cl_data.byte[2];
			G_GRAM[j][ys]=cl_data.byte[1];			
			B_GRAM[j][ys]=cl_data.byte[0];			
		}
	} else {
		int xx=xs, yy=ys;
		if(dx>dy){
			int t = - (dx >> 1);
			while (1)
			{
				R_GRAM[xx][yy]=cl_data.byte[2];
				G_GRAM[xx][yy]=cl_data.byte[1];
				B_GRAM[xx][yy]=cl_data.byte[0];
				if (xx == xe) break;
				xx += xstep;
				t  += dy;
				if (t >= 0){ yy += ystep; t-= dx;}
			} 
		} else {
			int t = - (dy >> 1);
			while (1)
			{
				R_GRAM[xx][yy]=cl_data.byte[2];
				G_GRAM[xx][yy]=cl_data.byte[1];
				B_GRAM[xx][yy]=cl_data.byte[0];
				if (yy == ye) break;
				yy += ystep;
				t  += dx;
				if (t >= 0){ xx += xstep; t-= dy;}
			} 
		}
	}
}


//--------------------------------------------------------------------------
void box(int x_min, int y_min, int x_max, int y_max, unsigned long color)
{
    int k,j;
    
    if(x_min<0) x_min=0;
    if(y_min<0) y_min=0;
    if(x_max>=Nx) x_max=Nx-1;
    if(y_max>=Ny) y_max=Ny-1;   
    
    line(x_min, y_min, x_max, y_min, color);
    line(x_min, y_max, x_max, y_max, color);
    line(x_min, y_min, x_min, y_max, color);
    line(x_max, y_min, x_max, y_max, color);

}


//----------------------------------------------------------------------------
//    Draw String
//----------------------------------------------------------------------------

//------font 5x7----------------------------------------
void disp_str8( char *s, int x, int y, unsigned long color)
{
	unsigned char c;
	int k,N;
	
	N=x;
	for(k=0; k<128; k++)
	{
		c=*(s+k);
		if(c==0) break;
		N=disp_chr8(c, N, y, color);
		N+=1;
	}
}


/*
//------font12--------------------------------------------
void disp_str12( char *s, int x, int y, unsigned long color)
{
	unsigned char c;
	int k,N;
	
	N=x;
	for(k=0; k<128; k++)
	{
		c=*(s+k);
		if(c==0) break;
		N=disp_chr12(c,N,y,color);
		N+=1;
	}
}


//------font16--------------------------------------------
void disp_str16( char *s, int x, int y, unsigned long color)
{
	unsigned char c;
	int k,N;
	
	N=x;
	for(k=0; k<128; k++)
	{
		c=*(s+k);
		if(c==0) break;
		N=disp_chr16(c,N,y,color);
		N+=1;
	}
}

//------font20--------------------------------------------
void disp_str20( char *s, int x, int y, unsigned long color)
{
	unsigned char c;
	int k,N;
	
	N=x;
	for(k=0; k<128; k++)
	{
		c=*(s+k);
		if(c==0) break;
		N=disp_chr20(c,N,y,color);
		N+=1;
	}
}


//------font24-------------------------------------------
void disp_str24( char *s,int x, int y, unsigned long color)
{
  unsigned char c;
  int k,N;
  
  N=x;
  for(k=0; k<128; k++)
  {
    c=*(s+k);
    if(c==0) break;
    N=disp_chr24(c,N,y,color);
    N+=2;
  }
  //draw(x, y, x+N-22, y+23);
}

//----- font32-------------------------------------------
void disp_str32( char *s,int x,int y, unsigned long color)
{
  unsigned char c;
  int k,N;
  
  N=x;
  for(k=0; k<128; k++)
  {
    c=*(s+k);
    if(c==0) break;
    N=disp_chr32(c,N,y,color);
    N+=4;
  }
  //draw(x, y, x+N-32, y+31);
}

*/


//-----------------------------------------------------------------------------
//              Draw a character
//-----------------------------------------------------------------------------


//------font 5x7 -------------------------------------
int disp_chr8(char c, int x, int y, unsigned long color)
{
	union {
		unsigned long  lw[1];
		unsigned char  byte[4];		
	} cl_data;
	cl_data.lw[0]=color;
	
	int k, j;
    unsigned char f8;
	
	if(c=='\\') c=' ';
	
	for(k=0; k<5; k++){
        f8=(unsigned char)bitrev8(font[c-0x20][k]);
        if(x>=0){
            for(j=0; j<8; j++){
                if( (f8&0x01)==0x01 ){
					R_GRAM[x][y+j]=cl_data.byte[2];
					G_GRAM[x][y+j]=cl_data.byte[1];
					B_GRAM[x][y+j]=cl_data.byte[0];
				}
                f8>>=1;
            }
        }
		x++;
	}
	return(x);
}
//--------- bit reverse(8bit)-----------------
unsigned char bitrev8(unsigned char x)
	{	
		x = ((x & 0xaa) >> 1) | ((x & 0x55) << 1);
		x = ((x & 0xcc) >> 2) | ((x & 0x33) << 2);
		x = (x >> 4) | (x << 4);
		return(x);
	}



/*
//------font12-----------------------------------------
int disp_chr12(char c, int x, int y, unsigned long color)
{
	union {
		unsigned long  lw[1];
		unsigned char  byte[4];		
	} cl_data;
	cl_data.lw[0]=color;
	
	int k, j, yj;
	unsigned short int f12;
	
	if(c=='\\') c=' ';
		
	for(k=0; k<24; k++){
		f12=(unsigned short int)font12[c-0x20][k];
		if(f12==0x0fff) break;
        if(x>=0){
            for(j=0; j<12; j++){
				yj=y+j;
                if( (f12&0x0001)==0x0001){
					if( x>=0 && x<Nx && yj>=0 && yj<Ny ){
					R_GRAM[x][yj]=cl_data.byte[2];
					G_GRAM[x][yj]=cl_data.byte[1];
					B_GRAM[x][yj]=cl_data.byte[0];
					}
				}
                f12>>=1;
            }
        }
		x++;
	}
	return(x+1);
}



//------font16-----------------------------------------
int disp_chr16(char c, int x, int y, unsigned long color)
{
	union {
		unsigned long  lw[1];
		unsigned char  byte[4];		
	} cl_data;
	cl_data.lw[0]=color;
	
	int k, j, yj;
	unsigned short int f16;
	
	if(c=='\\') c=' ';
		
	for(k=0; k<24; k++){
		f16=(unsigned short int)font16[c-0x20][k];
		if(f16==0xffff) break;
        if(x>=0){
            for(j=0; j<16; j++){
				yj=y+j;
                if( (f16&0x0001)==0x0001){
					if( x>=0 && x<Nx && yj>=0 && yj<Ny ){
					R_GRAM[x][yj]=cl_data.byte[2];
					G_GRAM[x][yj]=cl_data.byte[1];
					B_GRAM[x][yj]=cl_data.byte[0];
					}
				}
                f16>>=1;
            }
        }
		x++;
	}
	return(x+1);
}

//------font20-----------------------------------------
int disp_chr20(char c, int x, int y, unsigned long color)
{
	union {
		unsigned long  lw[1];
		unsigned char  byte[4];		
	} cl_data;
	cl_data.lw[0]=color;
	
	int k, j, yj;
	unsigned long f20;
	
	if(c=='\\') c=' ';
	
	for(k=0; k<24; k++){
		f20=(unsigned long)font20[c-0x20][k];
		if(f20==0xfffff) break;
        if(x>=0){
            for(j=0; j<20; j++){
				yj=y+j;
                if( (f20&0x00001)==0x00001){
					if( x>=0 && x<Nx && yj>=0 && yj<Ny ){
					R_GRAM[x][yj]=cl_data.byte[2];
					G_GRAM[x][yj]=cl_data.byte[1];
					B_GRAM[x][yj]=cl_data.byte[0];
					}
				}
                f20>>=1;
            }
        }
		x++;
	}
	return(x+1);
}

//------font24-----------------------------------------
int disp_chr24(char c, int x, int y, unsigned long color)
{
  union {
    unsigned long  lw[1];
    unsigned char  byte[4];   
  } cl_data;
  cl_data.lw[0]=color;
  
  int k,j, yj;
  unsigned long f24;
  
  if(c=='\\') c=' ';
  
  for(k=0; k<32; k++){
    f24=(unsigned long)font24[c-0x20][k];
    if(f24==0xffffff) break;
        if(x>=0){
            for(j=0; j<24; j++){
        yj=y+j;
        if( (f24&0x000001)==0x000001){
          if( x>=0 && x<Nx && yj>=0 && yj<Ny ){         
          R_GRAM[x][yj]=cl_data.byte[2];
          G_GRAM[x][yj]=cl_data.byte[1];
          B_GRAM[x][yj]=cl_data.byte[0];
          }
        }
                f24>>=1;    
            }
        }
    x++;
  }
  return(x+1);
}

//------font32-----------------------------------------
int disp_chr32(char c, int x, int y, unsigned long color)
{
  union {
    unsigned long  lw[1];
    unsigned char  byte[4];   
  } cl_data;
  cl_data.lw[0]=color;
  
  int k,j, yj;
  unsigned long f32;
  
  if(c=='\\') c=' ';
  
  for(k=0; k<48; k++){
    f32=(unsigned long)font32[c-0x20][k];
    if(f32==0xffffffff) break;
        if(x>=0){
            for(j=0; j<32; j++){
        yj=y+j;
                if( (f32&0x00000001)==0x00000001){
          if( x>=0 && x<Nx && yj>=0 && yj<Ny ){               
          R_GRAM[x][yj]=cl_data.byte[2];
          G_GRAM[x][yj]=cl_data.byte[1];
          B_GRAM[x][yj]=cl_data.byte[0];
          }
        }
                f32>>=1;    
            }
        }
    x++;
  }
  return(x+1);
}
 */ 
