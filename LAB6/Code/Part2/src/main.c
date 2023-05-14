#include <stdio.h>
#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"
#include "led_button.h"
#include "timer.h"

// Define Counter timer
#define COUNTER_timer TIM2

// Define pins for 7seg
#define SEG_gpio GPIOC
#define DIN_pin 3
#define CS_pin 2
#define CLK_pin 1

// Define pins for keypad
#define COL_gpio GPIOA
#define COL_pin 6       // 6 7 8 9
#define ROW_gpio GPIOB
#define ROW_pin 3       // 3 4 5 6

int now_col = 3;
int keyCnt = 0, keyValue = -1;

void SysTick_Handler() {
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		reset_push(COL_gpio, now_col+COL_pin);
		now_col = (now_col+1)%4;
		set_push(COL_gpio, now_col+COL_pin);
	}
}

void EXTIKeyPad_Handler(int r){
	int nowKey = keypad[r][(now_col+3)%4];
	if(nowKey == keyValue){
		keyCnt++;
	}else{
		keyCnt = 0;
	}
	keyValue = nowKey;
	if(keyCnt >=5){
		keyCnt = 5;
		display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, keyValue, 2);
	}
}

void EXTI_Setup(){
	// Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Select output bits
	SYSCFG-> EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3_Msk;
	SYSCFG-> EXTICR[0] |= (1 << SYSCFG_EXTICR1_EXTI3_Pos);
	SYSCFG-> EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI4_Msk;
	SYSCFG-> EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI4_Pos);
	SYSCFG-> EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5_Msk;
	SYSCFG-> EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI5_Pos);
	SYSCFG-> EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6_Msk;
	SYSCFG-> EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI6_Pos);

	// Enable interrupt
	EXTI->IMR1 |= EXTI_IMR1_IM3;
	EXTI->IMR1 |= EXTI_IMR1_IM4;
	EXTI->IMR1 |= EXTI_IMR1_IM5;
	EXTI->IMR1 |= EXTI_IMR1_IM6;

	// Enable Falling Edge
	EXTI->FTSR1 |= EXTI_FTSR1_FT3;
	EXTI->FTSR1 |= EXTI_FTSR1_FT4;
	EXTI->FTSR1 |= EXTI_FTSR1_FT5;
	EXTI->FTSR1 |= EXTI_FTSR1_FT6;

	// Enable NVIC
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI3_IRQHandler(){
	if(EXTI->PR1 & EXTI_PR1_PIF3_Msk){
		EXTIKeyPad_Handler(0);
		EXTI->PR1 |= EXTI_PR1_PIF3_Msk;
	}
}

void EXTI4_IRQHandler(){
	if(EXTI->PR1 & EXTI_PR1_PIF4_Msk){
		EXTIKeyPad_Handler(1);
		EXTI->PR1 |= EXTI_PR1_PIF4_Msk;
	}
}

void EXTI9_5_IRQHandler(){
	if(EXTI->PR1 & EXTI_PR1_PIF5_Msk){
		EXTIKeyPad_Handler(2);
		EXTI->PR1 |= EXTI_PR1_PIF5_Msk;
	}
	if(EXTI->PR1 & EXTI_PR1_PIF6_Msk){
		EXTIKeyPad_Handler(3);
		EXTI->PR1 |= EXTI_PR1_PIF6_Msk;
	}
}

int main()
{
	// Cause we want to use floating points we need to init FPU
	FPU_init();
	if(init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
		// Fail to init 7seg
		return -1;
	}
	if(init_keypad(ROW_gpio, COL_gpio, ROW_pin, COL_pin) != 0){
		// Fail to init keypad
		return -1;
	}

	// Set Decode Mode to non-decode mode
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE,0xFF);
	 // Set Scan Limit to digit 0 only
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT,0xFF);
	 // Wakeup 7seg
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN,0x01);

	// Configure SysTick clk to 10 Mhz and interrupt every 0.1s
	SystemClock_Config_Interrupt(10, 10000);
	// Init Interrupts
	EXTI_Setup();

    while(1);
    return 0;
}
