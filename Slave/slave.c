#include <reg51.h>
sbit RS485en=P3^2;	  //pin-i per RS485(DE-RE)
#define Stepper_Port P2 //porti per step motorin per pan
#define Stepper_Port2 P1//per tilt
unsigned char rxdata; // variabla per pranim te te dhenav
int i;
int z = 0;
int data_count=0;
bit PR_flag,PL_flag,TU_flag,TD_flag;//flag-at per pan dhe tilt
#define frame_Size 7 //madhesia e frame
//te dhenat per analizimin e frame

unsigned char tiltup = 0x08; //byte per tilt up
unsigned char panleft = 0x04; //         pan left
unsigned char tiltdown = 0x10; //        tilt down
unsigned char panright = 0x02; //        pan right
unsigned char     stop = 0x00;
unsigned char PelcoFrame[frame_Size];
unsigned char   syncByte=0xFF;  //byte per sinkronizim

unsigned char camAddress = 0x01;

unsigned int period,period2; //	perioda e delay
int sum,i;
//funk. per llogaritje te checksum
unsigned char checksum();
void clear_frame();
void delay(unsigned int count);
//Inicializimi i portit serik
void SerialInit()
{
TMOD=0x20; // zgjedhja e Timer 1, ne modin 8bit auto reload.
TH1=0xFD; // vendosja e baud rate = 9600
SCON=0x50; // vendosja e 8bit Control reg i cili kontrollon SBUF me RC5, 8 bit data, 1 stop bit , 1 start bit
TR1=1; // startimi i timerit
EA  = 1;// Enable global interrupt 
ES = 1; // Enable serial interrupt 		
}

void Serial_ISR() interrupt 4    
{
if(RI)
{
rxdata=SBUF;//pranimi i te dhenave nga regjistri SBUF
RI=0;
//analizimi i te dhenave
if(data_count==0)
{
if(rxdata==syncByte)	//frame i ri
{
PelcoFrame[data_count++]=rxdata	;
}
}
else
{
	PelcoFrame[data_count++]=rxdata;
	if(data_count>=frame_Size)
	{
		if((PelcoFrame[data_count-1] == checksum())&&(PelcoFrame[data_count-6] == camAddress))
		{
		// shpejtesia e ndrrimit te hapave te step motorit per pan
		   if(PelcoFrame[data_count-3]<=0x20)
	       {
			 period=10;		//medium speed
		   }
		   if((PelcoFrame[data_count-3]>0x20)&&(PelcoFrame[data_count-3]<=0x3F))
		   {
			 period=5;	   //high speed
		   }
		   if(PelcoFrame[data_count-3]>0x3F)
		   {
			 period=3;	   //full speed
		   }
    	// shpejtesia e ndrrimit te hapave te step motorit per tilt
		   if(PelcoFrame[data_count-2]<=0x20)
	       {
			 period2=10;		//medium speed
		   }
		   if((PelcoFrame[data_count-2]>0x20)&&(PelcoFrame[data_count-3]<=0x3F))
		   {
			 period2=5;	   //high speed
		   }
		   if(PelcoFrame[data_count-2]>0x3F)
		   {
			 period2=3;	   //full speed
		   }
		//ngasja e step motorit per pan	 
		   if(PelcoFrame[data_count-4]==panright)
		   {		
				PR_flag=1;
				PL_flag=0;
				TU_flag=0;
				TD_flag=0;
		   }
		   if(PelcoFrame[data_count-4]==panleft)
			 {
				PR_flag=0;
				PL_flag=1;
				TU_flag=0;
				TD_flag=0;
			 }
		  //ngasja e step motorit per tilt
			if(PelcoFrame[data_count-4]==tiltup)
			 {
				TU_flag=1;
				TD_flag=0;
				PR_flag=0;
				PL_flag=0;
			}
			if(PelcoFrame[data_count-4]==tiltdown)
			{
				TU_flag=0;
				TD_flag=1;
				PR_flag=0;
				PL_flag=0;
			}

			if((PelcoFrame[data_count-5]==stop)&&(PelcoFrame[data_count-4]==stop)&&(PelcoFrame[data_count-3]==stop)&&(PelcoFrame[data_count-2]==stop))
			 {
				PR_flag=0;
				PL_flag=0;
				TU_flag=0;
				TD_flag=0;
			 }

		}

    }
}			
}
if(data_count==frame_Size)	//bad frame size
{
	   clear_frame();
}

}


void main()
{

SerialInit();
clear_frame();
RS485en=0;
while(1){
if(PR_flag==1)
{
 		    Stepper_Port = 0x08;
			delay(period);
			Stepper_Port = 0x0C;
			delay(period);
			Stepper_Port = 0x04;
			delay(period);
			Stepper_Port = 0x06;
			delay(period);
			Stepper_Port = 0x02;
			delay(period);
			Stepper_Port = 0x03;
			delay(period);
			Stepper_Port = 0x01;
			delay(period);
			Stepper_Port = 0x09;
			delay(period);
}
if(PL_flag==1)
{
			Stepper_Port = 0x01;
			delay(period);
			Stepper_Port = 0x03;
			delay(period);
			Stepper_Port = 0x02;
			delay(period);
			Stepper_Port = 0x06;
			delay(period);
			Stepper_Port = 0x04;
			delay(period);
			Stepper_Port = 0x0C;
			delay(period);
			Stepper_Port = 0x08;
			delay(period);
			Stepper_Port = 0x09;
			delay(period);
}
if(TU_flag==1)
{
			Stepper_Port2 = 0x08;
			delay(period2);
			Stepper_Port2 = 0x0C;
			delay(period2);
			Stepper_Port2 = 0x04;
			delay(period2);
			Stepper_Port2 = 0x06;
			delay(period2);
			Stepper_Port2 = 0x02;
			delay(period2);
			Stepper_Port2 = 0x03;
			delay(period2);
			Stepper_Port2 = 0x01;
			delay(period2);
			Stepper_Port2 = 0x09;
			delay(period2);
}	 
if(TD_flag==1)
{
			Stepper_Port2 = 0x01;
			delay(period2);
			Stepper_Port2 = 0x03;
			delay(period2);
			Stepper_Port2 = 0x02;
			delay(period2);
			Stepper_Port2 = 0x06;
			delay(period2);
			Stepper_Port2 = 0x04;
			delay(period2);
			Stepper_Port2 = 0x0C;
			delay(period2);
			Stepper_Port2 = 0x08;
			delay(period2);
			Stepper_Port2 = 0x09;
			delay(period2);
}
}

}
//funksioni per llogaritje te checksum
unsigned char checksum()
{
	sum=0;
	for(i=5;i>=1;i--)
	{
		 sum=sum+PelcoFrame[i];
	}
	return sum%0x100;

}
//funksioni per te fshire frame i cili ka gabim
void clear_frame()
{    
    unsigned char i = 0;
    data_count = 0;
    for ( i = 0; i < frame_Size; i++ )
    {
        PelcoFrame[i] = 0;
    }
}
//funksioni per delay
void delay(unsigned int count)
{
    int i,j;
    for(i=0; i<count; i++)
	for(j=0; j<1275; j++); //1ms
}