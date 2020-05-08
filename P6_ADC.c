/**********************************************************
                                                           *
 source code: 16x2 LCD 4 bits and ADC voltimeter 	     *
 featuring freescale MC9S08QG8 microcontroller			       *
 	 ANSI-C	 	HD44780U 	 	 	 	 	 	 	 	                       *
***********************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define RS  	PTAD_PTAD0  				/*Register Select*/
#define E 		PTAD_PTAD1					/*Data Instruction Enable*/

void mcu_init(void);					 /*Initialize QG8 modules, registers IRQ, etc*/
void rti_delay(void);					/*Real Time Clock timer  RTI Module set 8ms delay*/
void lcd_bit_break(unsigned char b);    /*Breaks 8 bits into 4 and 4 to LCd 4 bit data send*/
void lcd_init(unsigned char *s); 		   /*Loop throught LCD start sequence array*/
void lcd_instruction(unsigned char i); 	/*Sends instructions characters to LCD*/
void lcd_character(unsigned char c);	/*Sends data characters to LCD*/
void lcd_message(unsigned char *m); 	/*Loop throught LCD data array*/

unsigned char lcdi_arr[7]={0x33,0x32,0x28,0x0f,0x06,0x01,0x00};
unsigned char lcdd_arr[6]={'V','=','-','.','-',0x00};
unsigned char lcdd_conv[4]={'0',0x00,'0',0x00};
unsigned char lcdd_cord[4]={0x82,0x00,0x84,0x00};
unsigned char data_message=0x00;		/*If data message is 1 means message to LCD, 0 means instructions*/

void adc_convertion(unsigned char channel);
void lcd_display(unsigned char adc_convertion);
unsigned char convertion=0x00;

void main(void) {
  mcu_init();
  lcd_init(&lcdi_arr[0]);
	data_message=0x01;
	lcd_message(&lcdd_arr[0]);
  for(;;){
    adc_convertion(0x02);
    lcd_display(convertion);
    
    data_message=0x00;
    lcd_message(&lcdd_cord[0]);
    
    data_message=0x01;
    lcd_message(&lcdd_conv[0]);
    
    data_message=0x00;
    lcd_message(&lcdd_cord[2]);
    
    data_message=0x01;
    lcd_message(&lcdd_conv[2]);
    
  }
}
void mcu_init(void){
	SOPT1=0x12; 						/*DISABLE WATCHDOG*/
	PTBDD=0xF0;							/*OUTPUT FOR COLUMNS (PTB4-7), INPUT ROWS (PTB0-3)*/
	PTADD_PTADD0=1; 				/* OUTPUT BIT RS */
	PTADD_PTADD1=1;					/* OUTPUT BIT E */
                          /* ADC MODULE SET UP*/
  ADCCFG=0x00;           /*MODE OF OPERATION, CLOCK SOURCE HIGH SPEED AND 8 BIT ADC*/
  ADCSC2=0x00;          /* CONTROL CONVERSION TRIGGER USING SOFTWARE*/
  APCTL1=0x04;        /*PIN CONTROL REGISTER PTA2 -> ADPC2*/
  ADCSC1=0x22;        /*CONTINUOUS CONVERTION AND SELECT CHANNEL ADP2*/
}
/************************************** ADC READ  **************************************************/
void lcd_display(unsigned char adc_convertion){
  unsigned char h_convertion=0x00; unsigned char l_convertion=0x00; unsigned char flag=0x08;
  while (adc_convertion!=0) {
    if (flag==0) {
      if (l_convertion==0x09) {h_convertion++; l_convertion=0x00;}
      l_convertion++;
      flag=0x08;
    }
    flag--;
    adc_convertion--;
  }
  lcdd_conv[0]=h_convertion+0x30;
  lcdd_conv[2]=l_convertion+0x30;
}
void adc_convertion(unsigned char channel){
  ADCSC1=ADCSC1|channel;
  while (ADCSC1_COCO==0) {}
  convertion=ADCRL;
}
/************************************** LCD SCREEN  ************************************************/
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
