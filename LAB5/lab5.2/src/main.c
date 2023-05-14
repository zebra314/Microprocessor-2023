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
#define COL_pin 6       // 6 7 8 9
#define ROW_gpio GPIOB
#define ROW_pin 3       // 3 4 5 6

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

int main(){
	// Cause we want to use floating points we need to init FPU
	FPU_init();

	#ifdef lab_modify_system_clock

	int SEG_ADDRESS_DIGIT[8] = {
		SEG_ADDRESS_DIGIT_0,
		SEG_ADDRESS_DIGIT_1,
		SEG_ADDRESS_DIGIT_2,
		SEG_ADDRESS_DIGIT_3,
		SEG_ADDRESS_DIGIT_4,
		SEG_ADDRESS_DIGIT_5,
		SEG_ADDRESS_DIGIT_6,
		SEG_ADDRESS_DIGIT_7,
		/*SEG_ADDRESS_DECODE_MODE,
		SEG_ADDRESS_ITENSITY,
		SEG_ADDRESS_SCAN_LIMIT,
		SEG_ADDRESS_SHUTDOWN,
		SEG_ADDRESS_DISPLAY_TEST*/

	};

	if(init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
			// Fail to init 7seg
		return -1;
	}
	if(init_button(BUTTON_gpio, BUTTON_pin) != 0 ){
		return -1;
	}
	// Set Decode Mode to non-decode mode
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, 0xFF);
	// Set Scan Limit to digit 0 only
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, 0xFF);
	// Wakeup 7seg
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN, 0x01);




	for(int i=0; i<8; i++){//reset display
			send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[i],SEG_DATA_DECODE_BLANK);

	}
	//begin with 0
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[0],0);



	if(init_button(BUTTON_gpio, BUTTON_pin) != 0){
		// Fail to init button
		return -1;
	}


	int state=0;
	// button_press_cycle_per_second (How many button press segments in a second)
	int button_press_cycle_per_second = 10;
	// Use to state how many cycles to check per button_press_cycle
	int debounce_cycles = 100;
	// Use to state the threshold when we consider a button press
	int debounce_threshold = debounce_cycles*0.7;
	// Used to implement negative edge trigger 0=not-presses 1=pressed
	int last_button_state=0;


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

	int sec=0, last=0;

	while(1){
		if(last!=COUNTER_timer->CNT){
			if(COUNTER_timer->CNT==0){
				// one second has pass
			sec++;
			}
			last = COUNTER_timer->CNT;
			double now_time = sec + COUNTER_timer->CNT/100.0;
			if(TIME_SEC < 0.0 || TIME_SEC >10000.0){
				display_two_decimal(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 0.0);
			}else{
			display_two_decimal(SEG_gpio, DIN_pin, CS_pin, CLK_pin, now_time);
			}
			if(now_time==TIME_SEC){
				break;
			}
		}


	}

#endif



	// Leave a empty while loop in order to stop it from
	// jumping back to startup script's LoopForever
	//
	// Useful when debugging using debugger:
	//   If jump to "LoopForever" means unexpected error happens
	//   Else, the code has ended
	while(1){}

	return 0;
}
