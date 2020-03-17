/**********************************************************
                                                          * 
 source code: implement a full step in a stepper motor    *
 RTI module											      *
 featuring freescale MC9S08QG8 microcontroller			  *
 	 ANSI-C	 	 	 	 	 	 	 	 	 	 	  	  *
 **********************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void wait(unsigned char time);  //function prototype
#define SEC  0x02 //defines 1 second for register RTI

void main(void){
  SOPT1=0x12; //diable watchdog
  EnableInterrupts;
  //config PTB OUTPUTS
  PTBDD_PTBDD0=1;            
  PTBDD_PTBDD1=1;                  
  PTBDD_PTBDD2=1;              
  PTBDD_PTBDD3=1;                   

  for(;;) {
	  //ful step sequence
	PTBD=0X05;
	wait(SEC);
	PTBD=0X06;
	wait(SEC);
	PTBD=0X0A;
	wait(SEC);
	PTBD=0X09;
	wait(SEC);
	
  } /* loop forever */
}

void wait(unsigned char time){
	SRTISC=time;	//rti clock 1khz
	while(SRTISC_RTIF==0){}
	SRTISC_RTIACK=1; //delete flag
}
