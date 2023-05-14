#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"
#include "led_button.h"
#include "timer.h"

// Define pins for 7seg
#define SEG_gpio GPIOC
#define DIN_pin 3
#define CS_pin 4
#define CLK_pin 5

// Define pins for keypad
#define COL_gpio GPIOA
#define COL_pin 5// 6 7 8 9
#define ROW_gpio GPIOB
#define ROW_pin 3// 3 4 5 6

// Define pins for led (default use on-board led PA5)
#define LED_gpio GPIOA
#define LED_pin 5

// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

// Define Counter timer
#define COUNTER_timer TIM2

// Buzzer is fixed to PA0 due to its need for PWM signal
// Can change to other ports if needed, but need to look up the reference

// Use to decide which part of the code will run
// Use define & ifdef to control
#define lab_modify_system_clock
//#define lab_counter
//#define lab_music_keyboard
//#define lab_music_song

int key_last = 0;

int main(){


	if(init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
		// Fail to init 7seg
		return -1;
	}

	// Set Decode Mode to Code B decode mode
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, 0xFF);
	// Set Scan Limit to all digits
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, 0x07);
	// Wakeup 7seg
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN, 0x01);

	if(init_keypad(ROW_gpio, COL_gpio, ROW_pin, COL_pin) != 0){
		// Fail to init keypad
		return -1;
	}
	GPIO_init_AF();
	timer_enable(TIM2);

	PWM_channel_init();
	//timer_init(TIM2,383,20); //170.3 383
	timer_start(TIM2);
	while(1){
		int input = 0;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				if(check_keypad_input_one(ROW_gpio, COL_gpio, ROW_pin, COL_pin, i, j)){
					input = 1;
					display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, keypad[i][j], num_digits(keypad[i][j]));
					sound(keypad[i][j]);
				}else{

				}
			}
		}
		if(input == 0){
			display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 0, 0);
			sound(0);
		}

	}



	while(1){}

	return 0;
}

void sound(int key){
	if(key!=key_last){
		key_last = key;
		timer_stop(TIM2);
		switch(key){
			case 0:
				timer_init(TIM2,0,0);
				break;
			case 1:
				timer_init(TIM2,383,20);
				break;
			case 2:
				timer_init(TIM2,341,20);
				break;
			case 3:
				timer_init(TIM2,303,20);
				break;
			case 4:
				timer_init(TIM2,286,20);
				break;
			case 5:
				timer_init(TIM2,255,20);
				break;
			case 6:
				timer_init(TIM2,227,20);
				break;
			case 7:
				timer_init(TIM2,202,20);
				break;
			case 8:
				timer_init(TIM2,191,20);
				break;
		}
		timer_start(TIM2);
	}

}

