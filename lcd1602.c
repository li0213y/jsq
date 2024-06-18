#include <reg52.h>
#include <intrins.h>

sbit LCD_RW = P2^5;
sbit LCD_RS= P2^6;
sbit LCD_E = P2^7;

#define LCD_DATA P0

static void LCD_Delay(unsigned char ms)		  //ÑÓÊ±º¯Êý
{
  unsigned char i;
  while(ms--)
  {
    for(i = 0; i< 250; i++)
    {
      _nop_();
      _nop_();
      _nop_();
      _nop_();
    }
  }
}

bit LCD_Busy()
{
  bit result;
  LCD_RS = 0;
  LCD_RW = 1;
  LCD_E = 1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  result = (bit)(LCD_DATA & 0x80);
  LCD_E = 0;
  return result;
}

void LCD_WriteCmd(unsigned char cmd)  //LCD1602Ð´ÃüÁî
{
  while(LCD_Busy());
  LCD_RS = 0;
  LCD_RW = 0;
  LCD_E = 0;
  _nop_();
  _nop_();
  LCD_DATA = cmd;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  LCD_E = 1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  LCD_E = 0;
}

void LCD_Pos(unsigned char row,unsigned char pos)
{
  row &= 0x1;
  pos &= 0xF;
  if (row) pos |= 0x40;
  LCD_WriteCmd(pos | 0x80);
}

void LCD_WriteDat(unsigned char dat) //Ð´Êý¾Ý
{
  while(LCD_Busy());
  LCD_RS = 1;
  LCD_RW = 0;
  LCD_E = 0;
  LCD_DATA = dat;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  LCD_E = 1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  LCD_E = 0;
}

void LCD_ShowStr(unsigned char x, unsigned char y, unsigned char *dat)	  //Ð´×Ö·û´®
{
  unsigned char i=0;
  LCD_Pos(y,x);
  while(dat[i] != '\0')
  {
    LCD_WriteDat(dat[i]);
    i++;
  }
}

void LCD_Init(void)			//LCD1602ÏÔÊ¾
{
  LCD_WriteCmd(0x38);
  LCD_Delay(1);
  LCD_WriteCmd(0x0c);
  LCD_Delay(1);
  LCD_WriteCmd(0x06);
  LCD_Delay(1);
  LCD_WriteCmd(0x01);
  LCD_Delay(1);
}