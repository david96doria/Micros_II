/***********************************************************
 Source code:  TRAFFIC LIGHTS STATE MACHINE                	*
 featuring freescale MC9S08QG8 microcontroller			   	*
 	 ANSI-C	 	        	 	 	 	 	 	 	 	   	*
***********************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/***************DEFINE IN SECONDS TRASITION TIMES ********/
#define red_time 0x03
#define yellow_time 0x01
#define green_time 0x02
/*************** DEFINE STATES VALUE ********************/
#define Red 0x00
#define Yellow 0x01
#define Green 0x02
#define TIME 0x07		//RTI 1.024S SET UP
/************ STATE MACHINE STRUCT DEFINITION **********/
typedef struct {
	unsigned char output_light;
	unsigned char time;
	unsigned char next_state[2];
}state;

state traffic_light[3] = { {0x04, red_time, {Red, Green} },
						   {0x01, green_time, { Green, Yellow} },
						   {0x02, yellow_time, {Yellow, Red}}
						 };

unsigned char present_state = Red;
unsigned char input_time = 0x00;

void mcu_init(void);
void rti_delay_light();
void light_transition_time(unsigned char light);

void main(void){
	mcu_init();
	for(;;){
		PTBD = traffic_light[present_state].output_light;						//OUTPUT ACTIVATED
		light_transition_time(traffic_light[present_state].time);				//STATE DELAY
		if(input_time>3){input_time = 0x00;}								    //READ  INPUT
		present_state = traffic_light[present_state].next_state[input_time];    //CHANGE STATE
	}
}
void mcu_init(void) {
  SOPT1=0X12;      //DISABLE WATCH DGOG
/****************MCU PORTS CONFIGURATION ***************/
PTBDD = 0x07;		//OUTPUT FOR TRAFFIC LIGHTS  0111 - 0111
}
void rti_delay(){
	SRTISC_RTIS=TIME;
	while(SRTISC_RTIF==0){}
	SRTISC_RTIACK=1;
}
void light_transition_time(unsigned char light){
	unsigned char flag = light;
	while (flag!=0){
		rti_delay();
		flag--;}
	input_time++;
}
