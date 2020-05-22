/**********************************************************
                                                           *
 Source code: TPM Output compare                           *
 featuring freescale MC9S08QG8 microcontroller			   *
 	 ANSI-C	 	        	 	 	 	 	 	 	 	   *
***********************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void mcu_init(void);
void tpm_oc(unsigned int trigger_value);       //TPM OUTPUT COMPARE FUNCTION
void adc_convertion(void);
unsigned int adc_value=0x00;
unsigned char output_set=0x00;

void main(void) {
  mcu_init();
  for(;;) {adc_convertion(); tpm_oc(adc_value);}
}
void mcu_init(void) {
  SOPT1=0X12;      //DISABLE WATCHSDGOG
  /**********************TPM MODULE SET UP **********************/
  PTADD_PTADD0=1;      //SET CH0 USE FOR TPM OUTPUT COMPARE
  TPMMOD=0x03FF;       //THE HIGHEST  COUNT VALUE
  TPMC0V=0x0000;       //COMPARE VALUE FOR TOGGLE
  TPMC0SC=0x14;      //OUTPUT COMPARE MODE AND TOGGLE OUTPUT ON COMPARE TPMCH0 PTA0
  TPMSC=0x0D;         //BUSCLK=4MHz , PRESCALER 2
  /**********************ADC MODULE SET UP **********************/
  ADCCFG=0x08;       /*MODE OF OPERATION, CLOCK SOURCE HIGH SPEED AND 10 BIT ADC*/
  ADCSC2=0x00;       /* CONTROL CONVERSION TRIGGER USING SOFTWARE*/
  APCTL1=0x04;       /*PIN CONTROL REGISTER PTA2 -> ADPC2*/
  ADCSC1=0x22;       /*CONTINUOUS CONVERTION AND SELECT CHANNEL ADP2*/
}
void tpm_oc(unsigned int trigger_value) {
	while(TPMC0SC_CH0F==0){}
	TPMC0SC;
	TPMC0SC_CH0F=0;
	if(trigger_value!=0 && output_set==0){TPMC0V=trigger_value; output_set=1;}
	else if(trigger_value!=0 && output_set==1){TPMC0V=0x0000; output_set=0;}
	else if(trigger_value==0 && output_set==0){TPMC0V=0x0001; output_set=1;}
	else if(trigger_value==0 && output_set==1){TPMC0V=0x0000; output_set=0;}
}
void adc_convertion(void){
	ADCSC1=ADCSC1|0x02;
	while(ADCSC1_COCO==0){}
	adc_value=ADCR;
}
