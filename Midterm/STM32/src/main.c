// For 112/5/22 Microprocessor Midterm
// LABs topics reference

//----LAB 2----//
// Part 1 : 設定 GPIOB PIN8
// Part 2.1 : LED 顯示學號
// Part 2.2 : 換 pin 腳
// Part 3 : 用 switch 和 LED 做隱藏密碼 

//----LAB 2.2----//
// C++ GPIO 跑馬燈
// 沒有紀錄 

//----LAB 3----//
// Part 1 : 使用 decode mode 控制 7-Seg LED 顯示學號
// Part 2 : 顯示 Fibonacci 數

//----LAB 4----//
// Part 1 : 3位數計算機
// Part 3 : Calculator 先乘除後加減
// Part 4 : Multiple press

//----LAB 5----//
// Part 1 : 20 MHz system clock LED 閃爍
// Part 2 : Timer 7-Seg LED 計時器
// Part 3 : Timer 蜂鳴器 keypad 電子琴

//----LAB 6----//
// Part 1 : Timer interrupt change LED sparkling rate
// Part 2 : Keypad external interrupt
// Part 3 : 鬧鐘

//----LAB 7----// 
// Part 1 : ADC 取樣光敏電阻值 7-segmemt
// Part 2 : ADC 變壓輸入
// Part 3 : SysTick timer 和 SG90 servo

//**********Setup************//
#include "stm32l476xx.h"
#include "timer.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"
#include "led_button.h"

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

// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

// Define pins for led (default use on-board led PA5)
#define LED_gpio GPIOA
#define LED_pin 5


void display(int num, int num_digit){
	display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, num, num_digit);
}

int check_press(int i, int j){
	return check_keypad_input_one(ROW_gpio, COL_gpio, ROW_pin, COL_pin, i, j);
}

int keypad_space[16][2]= { {0,0}, {0,1}, {0,2}, {0,3},
						   {1,0}, {1,1}, {1,2}, {1,3},
						   {2,0}, {2,1}, {2,2}, {2,3},
						   {3,0}, {3,1}, {3,2}, {3,3}};

// Return one value if input
// Return -1 if no input
int get_keyboard_input(){
	for(int i=0;i<16;i++){
		int a = keypad_space[i][0];
		int b = keypad_space[i][1];
		if(check_press(a,b)){
			while(check_press(a,b)!=0) continue;
			return keypad[a][b];
		}
	}
	return -1;
}

/************End Setup**********/

// Used to implement negative edge trigger 0=not-presses 1=pressed
int last_button_state=0;

void E(void){
	// send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 2, 10);
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 2, 11);
}

void HAHA(void){
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin,4, 12); // H
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin,3, 14); // A
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin,3, 1); // A
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin,2, 12); // H
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin,1, 14); // A
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin,1, 1); // A
	display(1,1);
}

int led_state = 0;
// User button 外部中斷處理函數
void EXTI15_10_IRQHandler(void){
	// button_press_cycle_per_second (How many button press segments in a second)
	int button_press_cycle_per_second = 20;
	// Use to state how many cycles to check per button_press_cycle
	int debounce_cycles = 50;
	// Use to state the threshold when we consider a button press
	int debounce_threshold = debounce_cycles*0.7;

	if(EXTI->PR1 & EXTI_PR1_PIF13_Msk){
		for(int a=0;a<button_press_cycle_per_second;a++){
		    // Simple Debounce without interrupt
		   	int pos_cnt=0;
		    for(int a=0;a<debounce_cycles;a++){
			    // If button press add count
		        if(read_gpio(BUTTON_gpio, BUTTON_pin)==0){
					pos_cnt++;
			    }
			    delay_without_interrupt(1000/(button_press_cycle_per_second*debounce_cycles));
		    }
		    // Check if need to change state
		    if(pos_cnt>debounce_threshold){
				// Pressed 
				int loop_cnt = 500*3000;
				while(loop_cnt){
					HAHA();
					loop_cnt--;
				}
				led_state = 1;
		    }
		    else{
				last_button_state = 0;
		    }
		}
	}
}

/*
void past_105_1(){
	int num_1 = 0;
	int num_2 = 0;
	int result = 0;
	int operator = 0;
	// 1 : +
	// 2 : -
	// 3 : *
	// 4 : /

	int state = 0;
	// 0 : input the first number
	// 1 : input the operator
	// 2 : input the second number
	// 3 : wait for the '='
	// 4 : display the result

	while(1){
		int input = get_keyboard_input();
		if (state == 0){
			// 0 : input the first number
			display(num_1, num_digits(num_1));
			if(input == 14){
				// 歸零
				num_1 = 0;
			}else if(input != -1){
				if(input == 10 || input == 11 || input == 13 || input == 14){
				// + - * /
				operator = input;
				state = 2;
				}else {
					num_1 = num_1*10 + input;
				}
			}
			if(num_digits(num_1) == 2){
				state = 1;
			}
		}else if(state == 1){
			// 1 : input the operator
			display(num_1, num_digits(num_1));
			if(input == 14){
				// 歸零 回到state 0
				num_1 = 0;
				state = 0;
			}else if(input == 10 || input == 11 || input == 13 || input == 14){
				// + - * /
				operator = input;
				state = 2;
			}
		}else if(state == 2){
			// 2 : input the second number
			display(num_2, num_digits(num_2));
			if(input == 14){
				// 歸零 
				num_2 = 0;
			}else if(input != -1){
				if(input == 15){
					// =
					if(operator == 10){
						result = num_1 + num_2;
					}else if(operator == 11){
						result = num_1 - num_2;
					}else if(operator == 12){
						result = num_1 * num_2;
					}else if(operator == 13){
						result = num_1 / num_2;
					}
					state = 4;
				}else {
					num_2 = num_2*10 + input;
				}
			}
			if(num_digits(num_2) == 2){
				state = 3;
			}
		}else if(state == 3){
			// 3 : wait for the '='
			display(num_2, num_digits(num_2));
			if(input == 14){
				// 歸零 回到state 2
				num_2 = 0;
				state = 2;
			}else if(input == 15){
				// =
				if(operator == 10){
					result = num_1 + num_2;
				}else if(operator == 11){
					result = num_1 - num_2;
				}else if(operator == 12){
					result = num_1 * num_2;
				}else if(operator == 13){
					result = num_1 / num_2;
				}
				state = 4;
			}
		}else if (state == 4){
			// 4 : display the result
			display(result, num_digits(result));
			if(input == 14){
				// 歸零 回到state 0
				num_1 = 0;
				num_2 = 0;
				result = 0;
				state = 0;
			}
		}	
	}
}
*/


// Q6
void SysTick_Handler() {
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk && led_state == 1){
		// Toggle LED display
		int loop_cnt = 500*3000;
		while(loop_cnt){
			loop_cnt--;
		}
		toggle_output(LED_gpio, LED_pin);
	}
}

void timer_countdown(){
	double TIME_SEC = 123.45;
	if(TIME_SEC < 0.0 || TIME_SEC >10000.0){
		display_two_decimal(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 0.0);
	}else{
		//Enable timer
		timer_enable(COUNTER_timer);
		// init the timer
		timer_init(COUNTER_timer, 40000, 100);
		// Start the timer
		timer_start(COUNTER_timer);
	}
	int sec=204, last=0;
	while(1){
		if(last!=COUNTER_timer->CNT){
			if(COUNTER_timer->CNT==0){
				// one second has pass
				sec--;
			}
			last = COUNTER_timer->CNT;
			double now_time = sec - COUNTER_timer->CNT/100.0;
			if(TIME_SEC < 0.0 || TIME_SEC >10000.0){
				display_two_decimal(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 0.0);
			}else{
				int min = (int)now_time/60;
				int sec = (int)now_time%60;
				now_time = min*100 + sec;
				display_two_decimal(SEG_gpio, DIN_pin, CS_pin, CLK_pin, now_time);
			}
			if(now_time==0){
				break;
			}
		}
	}
}

void NVIC_Configuration() {
	// NVIC_SetPriority(EXTI15_10_IRQn, 1); // 次高優先順序
  	// NVIC_SetPriority(SysTick_IRQn, 0);  // 最高優先順序

	// Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Select output bits
	SYSCFG-> EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_Msk;
	SYSCFG-> EXTICR[3] |= (2 << SYSCFG_EXTICR4_EXTI13_Pos);

	// Enable interrupt
	EXTI->IMR1 |= EXTI_IMR1_IM13;

	// Enable Falling Edge
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;

	// Enable NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Q6(){
	timer_enable(TIM2);
	NVIC_Configuration();
	SystemClock_Config_Interrupt(10, 1000000);
	while(1){
		HAHA();
	}
}

/***************** Q7 ***********************/
int tohex(int num){
	int out = 0;
	while(num!=0){
		if(num >=10 && num <=15){
			num = 0;
		}

		if(num<16){
			out = out*10 + num;
			break;
		}
		out = out*10 + num/16;
		if(num == 16){
			out = out*10;
			break;
		}
			
		num = num % 16;
	}
	return out;
}
void Q7(){
	int num_1 = 0;
	int num_2 = 0;
	int dis;
	int result = 0;
	int operator = 0;
	// 1 : +
	// 2 : -
	// 3 : *
	// 4 : /

	int state = 0;
	// 0 : input the first number
	// 1 : input the operator
	// 2 : input the second number
	// 3 : wait for the '='
	// 4 : display the result

	while(1){
		int input = get_keyboard_input();
		if (state == 0){
			// 0 : input the first number
			dis = tohex(num_1);
			// display(num_1, num_digits(num_1));
			display(dis, num_digits(dis));
			if(input == 14){
				// 歸零
				num_1 = 0;
			}else if(input != -1){
				if(input == 10 || input == 11 || input == 13 || input == 14){
				// + - * /
				operator = input;
				state = 2;
				}else {
					num_1 = num_1*10 + input;
				}
			}
			if(num_digits(num_1) == 2){
				state = 1;
			}
		}else if(state == 1){
			// 1 : input the operator
			dis = tohex(num_1);
			// display(num_1, num_digits(num_1)); 
			display(dis, num_digits(dis));
			if(input == 14){
				// 歸零 回到state 0
				num_1 = 0;
				state = 0;
			}else if(input == 10 || input == 11 || input == 13 || input == 14){
				// + - * /
				operator = input;
				state = 2;
			}
		}else if(state == 2){
			// 2 : input the second number
			dis = tohex(num_2);
			// display(num_2, num_digits(num_2));
			display(dis, num_digits(dis));
			if(input == 14){
				// 歸零 
				num_2 = 0;
			}else if(input != -1){
				if(input == 15){
					// =
					if(operator == 10){
						result = num_1 + num_2;
					}else if(operator == 11){
						result = num_1 - num_2;
					}else if(operator == 12){
						result = num_1 * num_2;
					}else if(operator == 13){
						result = num_1 / num_2;
					}
					state = 4;
				}else {
					num_2 = num_2*10 + input;
				}
			}
			if(num_digits(num_2) == 2){
				state = 3;
			}
		}else if(state == 3){
			// 3 : wait for the '='
			dis = tohex(num_2);
			// display(num_2, num_digits(num_2));
			display(dis, num_digits(dis));
			if(input == 14){
				// 歸零 回到state 2
				num_2 = 0;
				state = 2;
			}else if(input == 15){
				// =
				if(operator == 10){
					result = num_1 + num_2;
				}else if(operator == 11){
					result = num_1 - num_2;
				}else if(operator == 12){
					result = num_1 * num_2;
				}else if(operator == 13){
					result = num_1 / num_2;
				}
				state = 4;
			}
		}else if (state == 4){
			// 4 : display the result
			dis = tohex(result);
			// display(result, num_digits(result));
			display(dis, num_digits(dis));
			if(input == 14){
				// 歸零 回到state 0
				num_1 = 0;
				num_2 = 0;
				result = 0;
				state = 0;
			}
		}	
	}
}

/****************End Q7**********************/

/*
void do_something_with_button(){
	// button_press_cycle_per_second (How many button press segments in a second)
	int button_press_cycle_per_second = 20;
	// Use to state how many cycles to check per button_press_cycle
	int debounce_cycles = 50;
	// Use to state the threshold when we consider a button press
	int debounce_threshold = debounce_cycles*0.7;
	for(int a=0;a<button_press_cycle_per_second;a++){
		// Simple Debounce without interrupt
		int pos_cnt=0;
		for(int a=0;a<debounce_cycles;a++){
			// If button press add count
		    if(read_gpio(BUTTON_gpio, BUTTON_pin)==0){
			    pos_cnt++;
			}
			delay_without_interrupt(1000/(button_press_cycle_per_second*debounce_cycles));
		}
		// Check if need to change state
		if(pos_cnt>debounce_threshold){
			if(last_button_state==0){
			    // Pressed button - Pos edge
				// Do something
				timer_countdown();
			}
			last_button_state = 1;
		}
		else{
			last_button_state = 0;
		}
	}
}
*/

//************Q8*****************//
int now_col = 3;
int keyCnt = 0, keyValue = -1;

// void SysTick_Handler() {
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

void Q8(){
	timer_enable(TIM2);
	int score = 0;
	for(int i = 0; i<9; i++){
		// Configure SysTick clk to 10 Mhz and interrupt every 0.1s
		SystemClock_Config_Interrupt(10, 10000);
		// Init Interrupts
		EXTI_Setup();
	}
	while(1);
	return 0;
}
//***********End Q8*****************//

int main() {
	// Cause we want to use floating points we need to init FPU
	FPU_init();
	if (init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
		// Fail to init 7seg
		return -1;
	}
	if (init_keypad(ROW_gpio, COL_gpio, ROW_pin, COL_pin) != 0){
		// Fail to init keypad
		return -1;
	}
	if(init_button(BUTTON_gpio, BUTTON_pin) != 0){
		// Fail to init button
		return -1;
	}
	// Set Decode Mode to non-decode mode
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, 0xFF);
	// Set Scan Limit to digit 0 only
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, 0xFF);
	// Wakeup 7seg
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN, 0x01);


	while(1){
		// send_7seg_a(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 3, 0);
		Q6();
		// Q7();
		// Q8();
		// display(0, num_digits(0));
		// past_105_1();
		// do_something_with_button();
	}
}