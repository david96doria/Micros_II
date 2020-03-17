/**********************************************************
                                                          * 
 source code: implement 16x2 LCD 4 bits initilization 	  *
 featuring freescale MC9S08QG8 microcontroller			  *
 	 ANSI-C	 	HD44780U 	 	 	 	 	 	 	 	  *
***********************************************************/
#include <hidef.h> 						/* for EnableInterrupts macro */
#include "derivative.h" 				/* include peripheral declarations */

#define RS  PTAD_PTAD0  				/*Register Select*/
#define E 	PTAD_PTAD1					/*Data Instruction Enable*/

void mcu_init(void);					/*Initialize QG8 modules, registers IRQ, etc*/
void rti_delay(void);					/*Real Time Clock timer  RTI Module set 8ms delay*/
void lcd_bit_break(unsigned char b);    /*Breaks 8 bits into 4 and 4 to LCd 4 bit data send*/
void lcd_init(unsigned char *s); 		/*Loop throught LCD start sequence array*/
void lcd_instruction(unsigned char i); 	/*Sends instructions characters to LCD*/
void lcd_character(unsigned char c);	/*Sends data characters to LCD*/
void lcd_message(unsigned char *m); 	/*Loop throught LCD data array*/

unsigned char lcdi_arr[7]={0x33,0x32,0x28,0x0f,0x06,0x01,0x00};
unsigned char lcdd_arr[7]={'S','I','M','P','L','E',0x00};
unsigned char data_message=0x00;		/*If data message is 1 means message to LCD, 0 means instructions*/

void main(void) {
	mcu_init();
	lcd_init(&lcdi_arr[0]);
	data_message=0x01;
	lcd_message(&lcdd_arr[0]);
  for(;;) {} /* loop forever */
}
void rti_delay(void){
	SRTISC_RTIS=0x01;	
	while(SRTISC_RTIF==0){}
	SRTISC_RTIACK=1;
}
void lcd_bit_break(unsigned char b){
	unsigned char data=0x00;
	switch(data_message){
	case 0x00:
		data=b&0xf0;
		data=data>>4;
		lcd_instruction(data);
		data=b&0x0f;
		lcd_instruction(data);	
		break;
	case 0x01:
		data=b&0xf0;
		data=data>>4;
		lcd_character(data);
		data=b&0x0f;
		lcd_character(data);
		break;
	}
}
void lcd_message(unsigned char *m){
	while(*m!=0){
		lcd_bit_break(*m);
		m++;}
}
void lcd_character(unsigned char c){
	RS=1;
	PTBD=c;
	E=0;
	rti_delay();
	E=1;
}
void lcd_instruction(unsigned char i){
	RS=0;
	PTBD=i;
	E=0;
	rti_delay();
	E=1;
}
void lcd_init(unsigned char *s){
	while(*s!=0){
		lcd_bit_break(*s);
		s++;}
}
void mcu_init(void){ 
	SOPT1=0x12; 						/*  Disable watchdog */
	/*EnableInterrupts;*/
	PTADD_PTADD0=1; 					/* Output bits to RS and E */
	PTADD_PTADD1=1;
	PTBDD_PTBDD0=1;						/* Output bit to LCD MSB Data pins */
	PTBDD_PTBDD1=1;                  
	PTBDD_PTBDD2=1;              
	PTBDD_PTBDD3=1;
}
