#include "Buzzer.h"
#include "Uart.h"
#include <stdbool.h>
#define SW1 (12) // pin-ul 12 de pe portul A pentru switch 
#define MASK(x) (1UL << (x))
#define SWITCH_PRESSED_A(x) ((PTA->PDIR & MASK(x))>>x) // un macro care verifica daca switch-ul este apasat
#define buzzer (4) // pin 4 pentru buzzer
#define MASK(x) (1UL << (x))
int val;
void init_buzzer(void){ 
	
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; // activez portul D 	
		PORTD->PCR[buzzer] &= ~PORT_PCR_MUX_MASK;
		PORTD->PCR[buzzer] |= PORT_PCR_MUX(1); // GPIO
		PTD->PDDR |= MASK(buzzer); // buzzer ca output
	
}
void delay(volatile unsigned int time){
	while(time--){ //o simpla decrementare, pentru delay
		;
	}
}
void play_buzzer(void){
	
	uint32_t frequency = 5000;
	int duration = 0; // valoare ce va contoriza cat timp buzzerul va functiona
	if(ADC0->R[0] > 0 && ADC0->R[0] < 200) { // in functie de tensiunea transmisa de catre potentiometru, se modifica durata
		duration = 6000;
	}
	else if(ADC0->R[0] > 200 && ADC0->R[0] < 400) {
		duration = 5000;
	}
	else if(ADC0->R[0] > 400 && ADC0->R[0] < 600) {
		duration = 4000;
	}
	else if(ADC0->R[0] > 600 && ADC0->R[0] < 800) {
		duration = 3000;
	}
	else if(ADC0->R[0] > 800 && ADC0->R[0] < 1000){
		duration = 2000;
	}
	else {
		duration = 1000;
	}
	int cnt = 0;
	while(cnt != duration){ 
		PTD->PTOR = MASK(buzzer); // buzzerul va incepe sa bazaie
		delay(frequency); // delay-ul stabilit mai sus
		cnt++;
	}
	val = 0;
	return;   
}



void init_sw(void){ 
	
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //activez portul A
		
		PORTA->PCR[SW1] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10);  // setez switch ul ca GPIO
																																																																																														//the interrupt to treigger on the falling adge
		NVIC_SetPriority(PORTA_IRQn, 2); // prioritatea intreruperii este 2  
		NVIC_ClearPendingIRQ(PORTA_IRQn); // sterge flag ul de intrerupere pentru IRQn 
		NVIC_EnableIRQ(PORTA_IRQn); // activeaza intreruperea 	
		__enable_irq(); 

}


void PORTA_IRQHandler(void){ 

		if((PORTA->ISFR & MASK(SW1))){ // intreruperea se activeaza daca switch ul este apasat
			if(!(SWITCH_PRESSED_A(SW1))){
				play_buzzer(); // functia care va face ca buzzerul sa porneasca
			}
		
	}
	PORTA->ISFR = 0xffffffff; // sterge flagul de status
}