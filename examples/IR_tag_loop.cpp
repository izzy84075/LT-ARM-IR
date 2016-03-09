#include "stm32f30x.h"

#include "LTTO_IRRX.hpp"
#include "LTTO_IRTX.hpp"

int main(void);
void IRON(void);
void IROFF(void);

LTTO_IRRX dome;
LTTO_IRTX barrel(&IRON, &IROFF);
unsigned int delay;
bool fire;

GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
TIM_OCInitTypeDef         TIM_OCInitStructure;

extern "C" {
	void SysTick_Handler(void) {
		barrel.LTTO_IRTX_1msTick();
		
		if(delay) {
			delay--;
		}
		if(!delay) {
			fire = true;
		}
		
	}
}

int main(void) {
	//SystemInit();
	SysTick_Config(SystemCoreClock / 1000);
	
	//Turn on the GPIO clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//Turn on TIM4's clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//Set the AF mode on PB6
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2);
	
	//Set up our GPIO pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
	
	unsigned int Period  = (SystemCoreClock / 38000 ) - 1;
	unsigned int Channel1Pulse = (uint16_t) (((uint32_t) 5 * (Period - 1)) / 10);
	
	//Set up the timer
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_PrescalerConfig(TIM4, 0, TIM_PSCReloadMode_Immediate);
	
	//Set up the timer's output channel
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
	
	/* TIM4 counter enable */
  TIM_Cmd(TIM4, ENABLE);

  /* TIM4 Main Output Enable */
  //TIM_CtrlPWMOutputs(TIM4, ENABLE);
	
	GPIOA->BRR = 0xFFFF;
	
	dome.newSample(12, 24);
	dome.newSample(12, 8);
	dome.newSample(4, 8);
	dome.newSample(4, 8);
	dome.newSample(4, 8);
	dome.newSample(4, 8);
	dome.newSample(4, 8);
	dome.newSample(4, 8);
	dome.newSample(4, 200);
	
	LTTO_IR temp = dome.getSignature();
	
	while(1) {
		if(fire) {
			fire = false;
			if(!barrel.signatureInProgress()) {
				barrel.queueSignature(temp);
			}
			delay = 1000;
		}
	}
}

void IRON(void) {
	GPIOB->BSRR = GPIO_Pin_15;
}

void IROFF(void) {
	GPIOB->BRR = GPIO_Pin_15;
}
