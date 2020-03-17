/**********************************************************
                                                          * 
 source code: implement a full step in a stepper motor    *
 control timing between steps with MTIM timer module	  *
 featuring freescale MC9S08QG8 microcontroller			  *
 	 ANSI-C	  	 	 	 	 	 	 	 	 	 	 	  *
***********************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

unsigned char m_speed=0xff;   		/* Motor speed starting at max*/
void wait(unsigned char m_speed);	/* MTIM delay function*/
void speed_change(void);			/*Changes m_speed value*/

void main(void) {
	SOPT1=0x12; //diable watchdog
	//motor outputs
	PTBDD_PTBDD0=1;            
	PTBDD_PTBDD1=1;                  
	PTBDD_PTBDD2=1;              
	PTBDD_PTBDD3=1;
	//control inputs
	PTAPE=0x0C;
	PTADD_PTADD2=0;  //increment speed
	PTADD_PTADD3=0; //decrement speed
	//EnableInterrupts;
	//init MTIM module
	MTIMCLK=0x08;
  for(;;) {
	  speed_change();
	  PTBD=0X05;
	  wait(m_speed);
	  speed_change();
	  PTBD=0X06;
	  wait(m_speed);
	  speed_change();
	  PTBD=0X0A;
	  wait(m_speed);
	  speed_change();
	  PTBD=0X09;
	  wait(m_speed);
  } /* loop forever */
}
void speed_change(void){
	  //SW1 & SW2  polling
	  if(PTAD_PTAD2==0){
		  unpress: if(PTAD_PTAD2==0){goto unpress;}
		  m_speed=m_speed+0x0F;}
	  if(PTAD_PTAD3==0){
	  		  unpress1: if(PTAD_PTAD3==0){goto unpress1;}
	  		  m_speed=m_speed-0x0F;}
}
void wait(unsigned char m_speed){
	MTIMMOD=m_speed;
	MTIMSC_TRST=0;
	MTIMSC_TSTP=0;
	while(MTIMSC_TOF==0){}
	MTIMSC_TSTP=1;
	MTIMSC_TRST=1;
}
