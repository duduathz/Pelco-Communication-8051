#include <reg51.h>
#include <lcd.h>
//buttons
sbit panleft = P1^3;
sbit panright = P1^2;
sbit tiltdown = P1^1;
sbit tiltup = P1^0;
sbit stop = P1^4;
sbit RS485en=P3^2;

int data_count=0;
int frameSize=7; //madhesia e frame-it. Pelco-D 7-byte

//Frames 
unsigned char leftFrame[]={0xFF,0x01,0x00,0x04,0x3F,0x00}; // sinjali per transmetim. pan left high speed
unsigned char rightFrame[]={0xFF,0x01,0x00,0x02,0x3F,0x00}; // 						    pan right high speed
unsigned char stopFrame[]={0xFF,0x01,0x00,0x00,0x00,0x00};	 // Stop all actions (Pan / Tilt / Zoom / Iris etj.)
unsigned char upFrame[]={0xFF,0x01,0x00,0x08,0x00,0x20};	 //						    tilt up medium speed
unsigned char downFrame[]={0xFF,0x01,0x00,0x10,0x00,0x20};	 //						    tilt down medium speed
//funksioni per llogaritje te checksum
unsigned char checksum(unsigned char frame[]);
//funksioni per dergim te frame-it
void sendframe(unsigned char frame[]);
//funksioni per delay
void Delay(unsigned int itime);
// Inicializimi i portit serik
void SerialInit()
{
TMOD=0x20; // zgjedhja e Timer 1, ne modin 8bit auto reload.
TH1=0xFD; // vendosja e baud rate = 9600
SCON=0x50; // vendosja e 8bit Control reg i cili kontrollon SBUF me RC5, 8 bit data, 1 stop bit , 1 start bit
TR1=1; // startimi i timerit
//EA  = 1;// Enable global interrupt 
//ES = 1; // Enable serial interrupt
}

void main()
{
SerialInit();
Lcd8_Init();     
while(1)
{
if(panleft==0)
{
	sendframe(leftFrame); 
	Lcd8_Clear();	 //fshirja e LCD pas cdo marrje te informatave
	Lcd8_Set_Cursor(1,1);
	Lcd8_Write_String("Left");
}
else if(panright==0)
{
	sendframe(rightFrame);
	Lcd8_Clear();
	Lcd8_Set_Cursor(1,1);
	Lcd8_Write_String("Right");
}
else if(tiltup==0)
{
	sendframe(upFrame);
	Lcd8_Clear();
	Lcd8_Set_Cursor(1,1);
	Lcd8_Write_String("Up");
}
else if(tiltdown==0)
{
	sendframe(downFrame);
	Lcd8_Clear();
	Lcd8_Set_Cursor(1,1);
	Lcd8_Write_String("Down");
}

else if(stop==0)
{
	sendframe(stopFrame);
	Lcd8_Clear();
	Lcd8_Set_Cursor(1,1);
	Lcd8_Write_String("Stopped");
} 
}
}
//funksioni per llogaritje te checksum
 unsigned char checksum(unsigned char frame[])
{
	unsigned char sum;
	int i;
	sum=0x00;
	for(i=frameSize-2;i>=frameSize-6;i--)
	{
		 sum=sum+frame[i];
	}
	return sum%0x100;

}
//funksioni per dergim te paketit
 void sendframe(unsigned char frame[])
{
	 while(data_count<frameSize-1)//frameSize-1=6
	 {
	 SBUF=frame[data_count++];//ngarkimi i byte ne regjistrin SBUF
	 while(TI==0);			  //pret derisa te transmetohet stop bit-i
     TI=0;					  //e fshijme TI flag
	 }
	 SBUF=checksum(frame);
	 while(TI==0);
     TI=0;
	 data_count=0;
}
//funksioni per delay
void Delay(unsigned int itime) {
  unsigned int i,j;
  for (i=0; i<itime; i++)
    for (j=0; j<1275; j++); // 1ms delay
}