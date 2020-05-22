/**********************************************************
                                                           *
 Source code: TPM PWM                         *
 featuring freescale MC9S08QG8 microcontroller			   *
 	 ANSI-C	 	        	 	 	 	 	 	 	 	   *
***********************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void mcu_init(void);
void pwm(unsigned int pulse_width);       //TPM OUTPUT COMPARE FUNCTION
#define pwm_period 0x04FF
void adc_convertion(void);
unsigned int adc_value=0x00;


void main(void) {
  mcu_init();
  for(;;) {adc_convertion(); pwm(adc_value);}
}
void mcu_init(void) {
  SOPT1=0X12;      //DISABLE WATCHSDGOG
  /**********************TPM MODULE SET UP **********************/
  PTADD_PTADD0=1;      //SET CH0 USE FOR TPM OUTPUT COMPARE
  /*TPMMOD=0x03FF;       //THE HIGHEST  COUNT VALUE
  TPMC0V=0x0000;      //COMPARE VALUE FOR TOGGLE*/

  /**********************ADC MODULE SET UP **********************/
  ADCCFG=0x08;       /*MODE OF OPERATION, CLOCK SOURCE HIGH SPEED AND 10 BIT ADC*/
  ADCSC2=0x00;       /* CONTROL CONVERSION TRIGGER USING SOFTWARE*/
  APCTL1=0x04;       /*PIN CONTROL REGISTER PTA2 -> ADPC2*/
  ADCSC1=0x22;       /*CONTINUOUS CONVERTION AND SELECT CHANNEL ADP2*/
}
void pwm(unsigned int width) {
	if(width==0){width=0x0001;}
	TPMC0SC=0x28;       //PWM  MODE TPMCH0 PTA0
	TPMMOD=pwm_period;
	TPMC0V=width;
	TPMSC=0x0D;         //BUSCLK=4MHz , PRESCALER 32
	while(!TPMC0SC_CH0F);
	TPMC0SC;
	TPMC0SC_CH0F=0;
	TPMCNT;
}
void adc_convertion(void){
	ADCSC1=ADCSC1|0x02;
	while(ADCSC1_COCO==0){}
	adc_value=ADCR;
}
