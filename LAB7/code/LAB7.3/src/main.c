#include "stm32l476xx.h"
#include "timer.h"

int times = 1;
void SysTick_Handler() {
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
        times ++;
        //-------- timer_init(TIM2, 1000, 20000); ---------//
        TIM2->PSC = (uint32_t)(1000-1);          // PreScalser
	    TIM2->ARR = (uint32_t)(150000-1);          // Reload value
	    TIM2->EGR |= TIM_EGR_UG;                // Reinitialize the counter

        //-------- timer_start(TIM2); ---------//
        TIM2->CR1 |= TIM_CR1_CEN;
        if (times % 4 == 1 || times % 4 == 2)
            TIM2->CCR1 = 20;
        else
            TIM2->CCR1 = (uint32_t)(1);
	}
}

int main()
{
    // Configure SysTick clk to 10 Mhz and interrupt every 4s
	SystemClock_Config_Interrupt(10, 40000000);

    //-------- GPIO_init_AF(); ---------//
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Set to Alternate function mode
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
	GPIOA->MODER |= (2 << GPIO_MODER_MODE0_Pos);
	// Set AFRL
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0_Msk;
	GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL0_Pos);

    //-------- timer_enable(TIM2); ---------//
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    // -------- timer_init(TIM2, 1000, 20000); ---------//
    TIM2->PSC = (uint32_t)(10000-1);          // PreScalser
	TIM2->ARR = (uint32_t)(1000-1);          // Reload value
	TIM2->EGR |= TIM_EGR_UG;                // Reinitialize the counter

    //-------- sg90_init(); ---------//
    // PA0 for PWM
	// PWM mode 1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM2->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
	// OCPreload_Enable
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1PE_Msk;
	TIM2->CCMR1 |= (1 << TIM_CCMR1_OC1PE_Pos);
	// Active high for channel 1 polarity
	TIM2->CCER &= ~TIM_CCER_CC1P_Msk;
	TIM2->CCER |= (0 << TIM_CCER_CC1P_Pos);
	// Enable for channel 1 output
	TIM2->CCER &= ~TIM_CCER_CC1E_Msk;
	TIM2->CCER |= (1 << TIM_CCER_CC1E_Pos);

    //-------- timer_init(TIM2, 1000, 20000); ---------//
    TIM2->PSC = (uint32_t)(10000-1);          // PreScalser
	TIM2->ARR = (uint32_t)(1000-1);          // Reload value
	TIM2->EGR |= TIM_EGR_UG;                // Reinitialize the counter

    //-------- timer_start(TIM2); ---------//
    TIM2->CR1 |= TIM_CR1_CEN;

    while (1){
        
    }
    return 0;
}