/**********************************************************                                                   *
 Source code: SCI / UART                       			   *
 featuring freescale MC9S08QG8 microcontroller			   *
 	 ANSI-C	 	        	 	 	 	 	 	 	 	   *
***********************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


void mcu_init(void);
void byte_data_transfer (unsigned char data);
void byte_data_recive(void);
void rti_delay(void);

unsigned char flag_0 = 0x00;
unsigned char flag_1 = 0x01;
unsigned char data_r_buffer = 0x01;
unsigned char data_t_buffer[28] = {" Luis David Blanco 17110038"};
unsigned char buffer_size = 0x1B;

void main(void) {
mcu_init();
//EnableInterrupts;
  for(;;) {
    flag_1 = 0x00;
    while (flag_1<buffer_size) {
      byte_data_transfer(data_t_buffer[flag_1]);
      rti_delay();
      flag_1++;
    }
  }
} /*End main*/
void mcu_init(void) {
  SOPT1=0X12;      //DISABLE WATCHSDGOG
  /****************SCI UART CONFIGURATION ***************/
  SCIBDH = 0x00;    //BAUD RATE CONFIGURATION 9600
  SCIBDL = 0x1A;    //BAUD RATE CONFIGURATION 9600
  SCIC1 = 0x00;     //NO PARITY NO WAKE UP
  SCIC2 = 0X2C;     //RECEIVE  INTERRUPT, TRANSMISION AND RECEPTION ENEABLE. POLLING TX.
  SCIC3 = 0x00;     //ONLY 8-BIT NOT ERROR CHECKING
  (void)SCIS1;      //DUMMY READ
  SCID = 0x11;      //TURN OFF FLAGS
}
void byte_data_transfer (unsigned char data){
  SCIC2_TE = 1;
  SCID = data;
  while(!SCIS1_TC){}
  SCIC2_TE = 0;
}
interrupt 15 void byte_data_recive(void){
  if(flag_0==8){ flag_0 = 0x00;}
  data_r_buffer = SCID;

(void)SCIS1;
SCID = 0x00;

flag_0++;
}
void rti_delay(void){
	SRTISC_RTIS=0x07;
	while(SRTISC_RTIF==0){}
	SRTISC_RTIACK=1;
}

/******************ARDUINO RECEIVER*******************
char byte_data_receive;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()>0){
    byte_data_receive = Serial.read();
  }

  Serial.print(byte_data_receive);
  delay(1000);
}
*****************************************************/
