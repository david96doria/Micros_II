/**********************************************************
                                                          * 
 source code: implement 4x4 keyboard state machine code	  *
 featuring freescale MC9S08QG8 microcontroller			  *
 	 	 	 	 ANSI-C	 	KBI 	 	 	 	 	 	  *
***********************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void mcu_init(void);					/*Start QG8 modules, registers IRQ, etc*/
void kbi_isr(void);						/*Keyboard Interruption Service Routine*/
void key_row_check(unsigned char col);	/*Checks which row has been pressed*/
void key_col_check(void);				/*Checks which column has been pressed*/

unsigned char rows=0x00;
unsigned char key=0x00;

void main(void) {
	mcu_init();
	for(;;) {key_col_check(); PTAD=key;}
}
void mcu_init(void){
	SOPT1=0x12; 						/*DISABLE WATCHDOG*/
	PTBDD=0xF0;							/*OUTPUT FOR COLUMNS (PTB4-7), INPUT ROWS (PTB0-3)*/
	PTBPE=0x0F;							/*PTB INTERNAL PULL UP ENABLE FOR PTB0-PTB3*/
	PTADD=0x0F;							/*OUTPUT DISPLAY (PTA0-3)*/
	

	KBIPE=0xF0;							/*KBI PIN SELECTION PTB0-PTB3 ROWS*/
	KBIES=0x00;							/*FALLING EDGE*/
	KBISC=0x02;							/*KBI CONFIGURATIONS*/
	EnableInterrupts;
}
interrupt 18 void kbi_isr(void){
	DisableInterrupts;
	KBISC_KBACK=1;
	rows=PTBD&0x0F;
	EnableInterrupts;
}
void key_col_check(void){
	PTBD=0xE0; if(KBISC_KBF==0){key_row_check(0);}
	PTBD=0xD0; if(KBISC_KBF==0){key_row_check(1);}
	PTBD=0xB0; if(KBISC_KBF==0){key_row_check(2);}
	PTBD=0x70; if(KBISC_KBF==0){key_row_check(3);}
}
void key_row_check(unsigned char col){
	rows=PTBD&0x0F;
	if(rows==0x07 && col==0){key=0x0E;}
	if(rows==0x07 && col==1){key=0x00;}
	if(rows==0x07 && col==2){key=0x0F;}
	if(rows==0x07 && col==3){key=0x0D;}
	
	if(rows==0x0B && col==0){key=0x07;}
	if(rows==0x0B && col==1){key=0x08;}
	if(rows==0x0B && col==2){key=0x09;}
	if(rows==0x0B && col==3){key=0x0C;}
	
	if(rows==0x0D && col==0){key=0x04;}
	if(rows==0x0D && col==1){key=0x05;}
	if(rows==0x0D && col==2){key=0x06;}
	if(rows==0x0D && col==3){key=0x0B;}
	
	if(rows==0x0E && col==0){key=0x01;}
	if(rows==0x0E && col==1){key=0x02;}
	if(rows==0x0E && col==2){key=0x03;}
	if(rows==0x0E && col==3){key=0x0A;}
}
