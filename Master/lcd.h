#include <reg51.h>
//Lidhjet e LCD Modulit
sbit RS = P0^0;                                                                   
sbit EN = P0^1;                            
sbit D0 = P2^0;
sbit D1 = P2^1;
sbit D2 = P2^2;
sbit D3 = P2^3;
sbit D4 = P2^4;
sbit D5 = P2^5;
sbit D6 = P2^6;
sbit D7 = P2^7;

void Lcd_Delay(unsigned int itime) {
  unsigned int i,j;
  for (i=0; i<itime; i++)
    for (j=0; j<1275; j++); // 1ms delay
}

//LCD 8 Bit Interfacing Functions
void Lcd8_Port(char a)
{
	if(a & 1)
		D0 = 1;
	else 
		D0 = 0;
	
	if(a & 2)
		D1 = 1;
	else
		D1 = 0;
	
	if(a & 4)
		D2 = 1;
	else
		D2 = 0;
	
	if(a & 8)
		D3 = 1;
	else
		D3 = 0;
	
	if(a & 16)
		D4 = 1;
	else
		D4 = 0;

	if(a & 32)
		D5 = 1;
	else
		D5 = 0;
	
	if(a & 64)
		D6 = 1;
	else 
		D6 = 0;
	
	if(a & 128)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd8_Cmd(char a)
{ 
  RS = 0;             // => RS = 0
  Lcd8_Port(a);             //Data transfer
  EN  = 1;             // => E = 1
  Lcd_Delay(5);
  EN  = 0;             // => E = 0
}

void Lcd8_Clear()
{
	  Lcd8_Cmd(1);
}

void Lcd8_Set_Cursor(char a, char b)
{
	if(a == 1)
	  Lcd8_Cmd(0x80 + b);
	else if(a == 2)
		Lcd8_Cmd(0xC0 + b);
}

void Lcd8_Init()
{
  Lcd8_Port(0x00);
  RS = 0;
  ///////////// Reset process from datasheet /////////
  Lcd8_Cmd(0x30);
  /////////////////////////////////////////////////////
  Lcd8_Cmd(0x38);    //function set
  Lcd8_Cmd(0x0C);    //display on,cursor off,blink off
  Lcd8_Cmd(0x01);    //clear display
  Lcd8_Cmd(0x06);    //entry mode, set increment
}

void Lcd8_Write_Char(char a)
{
   RS = 1;             // => RS = 1
   Lcd8_Port(a);             //Data transfer
   EN  = 1;             // => E = 1
  //Lcd_Delay(5);
   EN  = 0;             // => E = 04
}

void Lcd8_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	 Lcd8_Write_Char(a[i]);
}

void Lcd8_Shift_Right()
{
	Lcd8_Cmd(0x1C);
}

void Lcd8_Shift_Left()
{
	Lcd8_Cmd(0x18);
}
//End LCD 8 Bit Interfacing Functions
