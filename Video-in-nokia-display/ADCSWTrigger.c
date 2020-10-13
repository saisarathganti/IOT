
#include "tm4c123gh6pm.h"

void ADC0_InitSWTriggerSeq3_Ch1(void){ volatile unsigned long delay;
	SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
	delay = SYSCTL_RCGCGPIO_R;
	SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  GPIO_PORTE_DIR_R &= ~0x0C;      // 2) make PE2,PE3 input
  GPIO_PORTE_AFSEL_R |= 0x0C;     // 3) enable alternate function on PE2,PE3
  GPIO_PORTE_DEN_R &= ~0x0C;      // 4) disable digital I/O on PE2,PE3
  GPIO_PORTE_AMSEL_R |= 0x0C;     // 5) enable analog function on PE2,PE3
  //SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
  //delay = SYSCTL_RCGC2_R;         
  ADC0_PC_R &= ~0xF;
	ADC0_PC_R |= 0x1; 
	//SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x3210;          // 8) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0x0F00;         // 10) seq2 is software trigger
	
  ADC0_SSMUX2_R = ((ADC0_SSMUX2_R&~ADC_SSMUX2_MUX1_M)+(0<<ADC_SSMUX2_MUX1_S)) |
                  ((ADC0_SSMUX2_R&~ADC_SSMUX2_MUX0_M)+(1<<ADC_SSMUX2_MUX0_S));
	//ADC0_SSMUX2_R = 0x0001;         // 12) set channels for SS2
  //ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+0; // 11) channel Ain1,Ain0 (PE2,PE3)
  ADC0_SSCTL2_R = 0x0060;         // 12) no TS0 D0, yes IE0 END0
	ADC0_IM_R &= ~0x0004;
  ADC0_ACTSS_R |= 0x0004;         // 13) enable sample sequencer 2
}

void ADC_In89(unsigned long data[2]){ 
  ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  data[1] = ADC0_SSFIFO2_R&0xFFF;  // 3A) read first result
  data[0] = ADC0_SSFIFO2_R&0xFFF;  // 3B) read second result
  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
}

