#include "Adc.h"

#define PIN_Rotation (0) //PORT B
int channel_pin;

void printVoltage(uint16_t value){ 
	uint8_t nr1=value /1000; 
	uint8_t nr2=value %1000 /100; 
	uint8_t nr3=value %100 /10; 
	uint8_t nr4=value %10;  
	UART0_Transmit(nr1+48);
	UART0_Transmit(nr2+48);
	UART0_Transmit(nr3+48);
	UART0_Transmit(nr4+48);
	UART0_Transmit(0x0D);
	UART0_Transmit(0x0A);
	
} 

void adcInit(int power){
    
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // activez CLK ADC0

    if(power==1)
        ADC0->CFG1 = ADC_CFG1_ADLPC_MASK;
    
    ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK;        
    ADC0->CFG2 = ADC_CFG2_ADLSTS(0x12 & 3);
    

    ADC0->CFG1 |=ADC_CFG1_ADIV(1)    // Clock Divide Select
               | ADC_CFG1_MODE(2)              
               | ADC_CFG1_ADICLK(0);    // Input Clock: (Bus Clock=24MHz)

    ADC0->CFG2 = (uint32_t)((ADC0->CFG2 & (uint32_t)~(uint32_t)(
               ADC_CFG2_ADHSC_MASK |
               ADC_CFG2_ADLSTS(0x03)
              )) | (uint32_t)(
               ADC_CFG2_ADACKEN_MASK
              ));

    
    ADC0->SC3 = ADC_SC3_AVGE_MASK       
              | ADC_SC3_AVGS(0x10 & 3); 
}


int adcSelect(){
	
    channel_pin=-1;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;// activez clock ul
    PORTB->PCR[PIN_Rotation] &= ~PORT_PCR_MUX_MASK;// analog input
    channel_pin=8;  
    return channel_pin;
}

void adcRead(int channel) {
    // conversia de la analog la digital
    if(channel>=0){
    ADC0->SC1[0] = ADC_SC1_ADCH(channel & ~(1 << 5));
    }
}


