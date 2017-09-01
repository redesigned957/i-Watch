#include "includes.h"
#include "font.h"

// OLED_RES PB13
// OLED_DC  PA15
// OLED_CS  PB7
//初始化SSD1306					    
void OLED_Init(void)
{ 	 
	OLED_pinInit();	
  OLED_deSelect();  //不选择OLED
	OLED_reset();		
	
	OLED_writeByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_writeByte(0x02,OLED_CMD);//---SET low column address
	OLED_writeByte(0x10,OLED_CMD);//---SET high column address
	OLED_writeByte(0x40,OLED_CMD);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
	OLED_writeByte(0x81,OLED_CMD);//--SET contrast control register
	OLED_writeByte(0xCF,OLED_CMD); // SET SEG Output Current Brightness
	OLED_writeByte(0xA1,OLED_CMD);//--SET SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_writeByte(0xC8,OLED_CMD);//SET COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_writeByte(0xA6,OLED_CMD);//--SET normal display
	OLED_writeByte(0xA8,OLED_CMD);//--SET multiplex ratio(1 to 64)
	OLED_writeByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_writeByte(0xD3,OLED_CMD);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_writeByte(0x00,OLED_CMD);//-not offSET
	OLED_writeByte(0xd5,OLED_CMD);//--SET display clock divide ratio/oscillator frequency
	OLED_writeByte(0x80,OLED_CMD);//--SET divide ratio, SET Clock as 100 Frames/Sec
	OLED_writeByte(0xD9,OLED_CMD);//--SET pre-charge period
	OLED_writeByte(0xF1,OLED_CMD);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_writeByte(0xDA,OLED_CMD);//--SET com pins hardware configuration
	OLED_writeByte(0x12,OLED_CMD);
	OLED_writeByte(0xDB,OLED_CMD);//--SET vcomh
	OLED_writeByte(0x40,OLED_CMD);//SET VCOM Deselect Level
	OLED_writeByte(0x20,OLED_CMD);//-SET Page Addressing Mode (0x00/0x01/0x02)
	OLED_writeByte(0x02,OLED_CMD);//
	OLED_writeByte(0x8D,OLED_CMD);//--SET Charge Pump enable/disable
	OLED_writeByte(0x14,OLED_CMD);//--SET(0x10) disable
	OLED_writeByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_writeByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_writeByte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_writeByte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_clear();
	OLED_setPos(0,0); 	
}  
/**********************************************************************************************************
* 函数名    ：   OLED_Reset
* 功能描述  ：   oled复位
* 作者      ：   Jahol Fan  
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
u8 OLED_reset(void)
{
	OLED_RST_CLR;
	Delay_ms(25);   //Jahol Fan ：奇怪的问题，延时改长了，就不能兼容HP-6
	OLED_RST_SET;
	return 0;       //返回0，表示复位成功
}
/**********************************************************************************************************
* 函数名    ：   OLED_resetPinInit
* 功能描述  ：   oled复位管脚初始化
* 作者      ：   Jahol Fan  
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
								OLED_RES  	PB13
								OLED_DC 		PA15
								OLED_CS  		PB7
**********************************************************************************************************/
u8 OLED_pinInit(void)
{
	RCC->AHB1ENR |= 3<<0;//PA  PB
	
	//OLED_RES  	PB13
	GPIOB->MODER &= ~(3<<26);
	GPIOB->MODER |= 1<<26;
	
	GPIOB->OTYPER &= ~(1<<13);
	GPIOB->OSPEEDR &= ~(3<<26);
	GPIOB->OSPEEDR |= 2<<26;
	
	//OLED_DC 		PA15
	GPIOA->MODER &= ~(3ul<<30);
	GPIOA->MODER |= 1ul<<30;
	
	GPIOA->OTYPER &= ~(1<<15);
	GPIOA->OSPEEDR &= ~(3ul<<30);
	GPIOA->OSPEEDR |= 2ul<<30;
	
	//OLED_CS  		PB7
	GPIOB->MODER &= ~(3<<14);
	GPIOB->MODER |= 1<<14;
	
	GPIOB->OTYPER &= ~(1<<7);
	GPIOB->OSPEEDR &= ~(3<<14);
	GPIOB->OSPEEDR |= 2<<14;	
	
	return 0;
}
/**********************************************************************************************************
* 函数名    ：   OLED_select
* 功能描述  ：   oled片选
* 作者      ：   Jahol Fan  
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_select(void)
{
   OLED_CS_CLR;//拉低使能端
}
/**********************************************************************************************************
* 函数名    ：   OLED_deSelect
* 功能描述  ：   oled取消片选
* 作者      ：   Jahol Fan  
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_deSelect(void)
{  
	 OLED_CS_SET;//拉高使能端
}


/**********************************************************************************************************
* 函数名    ：   OLED_writeByte
* 功能描述  ：   oled字节写，向SSH1106写入一个字节。
* 作者      ：   Jahol Fan  
* 参数说明  ：   dat:要写入的数据/命令
*                cmd:数据/命令标志 0,表示命令;1,表示数据;
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_writeByte(u8 dat, u8 cmd)
{	
	cmd ? OLED_DC_SET : OLED_DC_CLR;	
		
	OLED_select();		 
	
 SPI_ReceiveByte(dat);
	
	OLED_deSelect();
} 

/**********************************************************************************************************
* 函数名    ：   OLED_clear
* 功能描述  ：   清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	
* 作者      ：   Jahol Fan  
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_writeByte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_writeByte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_writeByte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0; n<128; n++)
		{
			OLED_writeByte(0,OLED_DATA); 
		}
	} //更新显示
}
/**********************************************************************************************************
* 函数名    ：   OLED_setPos
* 功能描述  ：   清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	
* 作者      ：   Jahol Fan  
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_setPos(unsigned char x, unsigned char y) 
{ 
	x += 2;//SSH1106 要加这个
	
	OLED_writeByte(0xb0+y,OLED_CMD);
	OLED_writeByte(((x&0xf0)>>4)|0x10, OLED_CMD);
	OLED_writeByte((x&0x0f)|0x01,OLED_CMD); 
}  

//显示一个汉字
void OLED_showCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t;
	OLED_setPos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_writeByte(Hzk[2*no][t],OLED_DATA);

     }	
		OLED_setPos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_writeByte(Hzk[2*no+1][t],OLED_DATA);

      }					
}

//显示一个汉字字符串
void OLED_showHzString(u8 x,u8 y,char *buf)
{
	   int i;
				
			 //在自定义的汉字库中对比找到要显示的汉字
			 for(i=0;i<strlen((char*)hz_index);i++)
			 {
				 if(*buf ==hz_index[i])
				 {
					 //调取汉字显示函数
					 OLED_showCHinese(x+16*i,y,i);	
					  
			   }	
		      buf++;	 
			 }			 	
}

void OLED_showChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
  u8 c=0,i=0;	
  c=chr-' ';//得到偏移后的值			
  if(x>MAX_COLUMNC-1)
  {
    x=0;
    y=y+2;  //这个要优化，根据字体的不同来换行
  }
  if(Char_Size == 24)
  {
    OLED_setPos(x,y);	
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i],OLED_DATA);
    OLED_setPos(x,y+1);
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i+16],OLED_DATA);
		OLED_setPos(x,y+2);
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i+16*2],OLED_DATA);
  }
  else if(Char_Size ==16)
  {
    OLED_setPos(x,y);	
    for(i=0;i<8;i++)
      OLED_writeByte(F8X16[c*16+i],OLED_DATA); //一个字符用16个字节来控制
    OLED_setPos(x,y+1); //y+1 表示换行，
    for(i=0;i<8;i++)
      OLED_writeByte(F8X16[c*16+i+8],OLED_DATA);
  }
	
  else 
  {	
    OLED_setPos(x,y);
    for(i=0;i<6;i++)
      OLED_writeByte(F6x8[c][i],OLED_DATA); 
  }
}
 //显示一个字符字符串
void OLED_showChString(u8 x,u8 y,u8 *chr,u8 Char_Size)

{


  u8 j=0;
  while (chr[j]!='\0')
  {		
    OLED_showChar(x,y,chr[j],Char_Size);
		if(24 == Char_Size)
		{
		  x+=16;
		}
		else
      x+=8;
    if(x>120){x=0;y+=2;}
    j++;

 }	
}
