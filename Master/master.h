#include <reg51.h>


/*void delay(unsigned int ms)
{
int count=0;
 while(count!=ms)
  {
   TMOD=0x01;  //16-bit timer0 selected
   TH0=0xFC;   // Loading high byte in TH
   TL0=0x66;   // Loaded low byte in TL
   TR0=1;      // Running the timer
    while(!TF0);   //Checking the timer flag register if it is not equal to 1 
   TR0 = 0;      // If TF0=1 stop the timer
   TF0 = 0;      // Clear the Timer Flag bit for next calculation

   count++;
  }

}*/
