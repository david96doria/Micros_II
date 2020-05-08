/**********************************************************
                                                          *
 source code: 16x2 LCD 4 bits and matrix keyboard   	  *
 featuring freescale MC9S08QG8 microcontroller			  *
 	 ANSI-C	 	HD44780U 	 	 	 	 	 	 	 	  *
***********************************************************/

#include <hidef.h> 						/* for EnableInterrupts macro */
#include "derivative.h" 				/* include peripheral declarations */

#define RS  	PTAD_PTAD0  				/*Register Select*/
#define E 		PTAD_PTAD1					/*Data Instruction Enable*/

void mcu_init(void);					/*Start QG8 modules, registers IRQ, etc*/
void kbi_isr(void);						/*Keyboard Interruption Service Routine*/
void key_row_check(unsigned char col);	/*Checks which row has been pressed*/
void key_col_check(void);				/*Checks which column has been pressed*/

unsigned char rows=0x00;
unsigned char key=0x00;
unsigned char key_c='A';

void mcu_init(void);					/*Initialize QG8 modules, registers IRQ, etc*/
void rti_delay(void);					/*Real Time Clock timer  RTI Module set 8ms delay*/
void lcd_bit_break(unsigned char b);    /*Breaks 8 bits into 4 and 4 to LCd 4 bit data send*/
void lcd_init(unsigned char *s); 		/*Loop throught LCD start sequence array*/
void lcd_instruction(unsigned char i); 	/*Sends instructions characters to LCD*/
void lcd_character(unsigned char c);	/*Sends data characters to LCD*/
void lcd_message(unsigned char *m); 	/*Loop throught LCD data array*/
void key_rti_delay(unsigned char time);

unsigned char lcdi_arr[7]={0x33,0x32,0x28,0x0f,0x06,0x01,0x00};
unsigned char lcdd_arr[2]={0x33, 0x00};
unsigned char data_message=0x00;		/*If data message is 1 means message to LCD, 0 means instructions*/
unsigned char keypad[4][4]={'1','2','3','A',
						    '4','5','6','B',
						    '7','8','9','C',
						    '*','0','#','D',};
unsigned char key_press=0x01;

void main(void) {
 	mcu_init();
	lcd_init(&lcdi_arr[0]);
	data_message=0x01;
	PTBD=0x00;
	EnableInterrupts;
	for(;;) {key_col_check();}
	}
void mcu_init(void){
	SOPT1=0x12; 						/*DISABLE WATCHDOG*/
	PTBDD=0xF0;							/*OUTPUT FOR COLUMNS (PTB4-7), INPUT ROWS (PTB0-3)*/
	PTBPE=0x0F;							/*PTB INTERNAL PULL UP ENABLE FOR PTB0-PTB3*/
	PTADD_PTADD0=1; 					/* OUTPUT BIT RS */
	PTADD_PTADD1=1;						/* OUTPUT BIT E */

	KBIPE=0xF0;							/*KBI PIN SELECTION PTB0-PTB3 ROWS*/
	KBIES=0x00;							/*FALLING EDGE*/
	KBISC=0x02;							/*KBI CONFIGURATIONS*/

}
/************************************** KEYPAD  ************************************************/
interrupt 18 void kbi_isr(void){
	DisableInterrupts;
	KBISC_KBACK=1;
	key_press=0x00;
	rows=PTBD&0x0F;
	EnableInterrupts;
}
void key_col_check(void){
	rows=PTBD&0x0F;
	if(key_press!=0x01){PTBD=0x7F; if(PTBD!=0x7F){key_row_check(3);}}
	if(key_press!=0x01){PTBD=0xBF; if(PTBD!=0xBF){key_row_check(2);}}
	if(key_press!=0x01){PTBD=0xDF; if(PTBD!=0xDF){key_row_check(1);}}
	if(key_press!=0x01){PTBD=0xEF; if(PTBD!=0xEF){key_row_check(0);}}
}
void key_row_check(unsigned char col){
	if(key_press!=0x01){if((rows&0x01)==0){rows=0; key_press=0x01;}}
	if(key_press!=0x01){if((rows&0x02)==0){rows=1; key_press=0x01;}}
	if(key_press!=0x01){if((rows&0x04)==0){rows=2; key_press=0x01;}}
	if(key_press!=0x01){if((rows&0x08)==0){rows=3; key_press=0x01;}}


	key=keypad[rows][col];
	key_c=keypad[rows][col];

	DisableInterrupts;
	lcdd_arr[0]=key_c;
	key_rti_delay(0x06);
	lcd_message(&lcdd_arr[0]);
	key_rti_delay(0x06);
	PTBD=0x00;
	EnableInterrupts;
}
/************************************** LCD SCREEN  ************************************************/
void key_rti_delay(unsigned char time){
	SRTISC_RTIS=time;
	while(SRTISC_RTIF==0){}
	SRTISC_RTIACK=1;
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
		lcd_instruction(data);
		data=b&0x0f;
		data=data<<4;
		lcd_instruction(data);
		break;
	case 0x01:
		data=b&0xf0;
		lcd_character(data);
		data=b&0x0f;
		data=data<<4;
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
	E=0;
}
void lcd_instruction(unsigned char i){
	RS=0;
	PTBD=i;
	E=0;
	rti_delay();
	E=1;
	E=0;
}
void lcd_init(unsigned char *s){
	while(*s!=0){
		lcd_bit_break(*s);
		s++;}
}
