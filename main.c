#include "MKL25Z4.h"
#include "Buzzer.h" 
#include "Uart.h"
#include "Adc.h"

int main(void){
	
	UART0_Init(115200);
	adcInit(0);
	init_sw(); 
	init_buzzer();	
	
	
	while(1){ // infinate loop
		adcRead(adcSelect());

		printVoltage(ADC0->R[0]);

		
	}
}