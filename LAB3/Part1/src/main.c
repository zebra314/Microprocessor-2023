#include "stm32l476xx.h"
#include "helper_functions.h"
#include "led_button.h"
#include "7seg.h"

// Define pins for 4 leds
//#define LED_gpio GPIOA
//#define LED1_pin 5
//#define LED2_pin 6
//#define LED3_pin 7
//#define LED4_pin 8

// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

// Define pins for 7seg
#define SEG_gpio GPIOB
#define DIN_pin 3
#define CS_pin 4
#define CLK_pin 5

// Use to decide which part of the code will run
// Use define & ifdef to control
//#define lab_led
//#define lab_led_button
#define lab_7seg_non_decode
//#define lab_7seg_decode

int main(){


#ifdef lab_7seg_non_decode

	if(init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
		// Fail to init 7seg
		return -1;
	}

	// Set Decode Mode to non-decode mode
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, 0xFF);
	// Set Scan Limit to digit 0 only
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, 0xFF);
	// Wakeup 7seg
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN, 0x01);

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
	int SEG_DATA_DECODE[10] = {
		SEG_DATA_DECODE_0,
		SEG_DATA_DECODE_1,
		SEG_DATA_DECODE_2,
		SEG_DATA_DECODE_3,
		SEG_DATA_DECODE_4,
		SEG_DATA_DECODE_5,
		SEG_DATA_DECODE_6,
		SEG_DATA_DECODE_7,
		SEG_DATA_DECODE_8,
		SEG_DATA_DECODE_9

	};
	int SEG_DATA_NON_DECODE_LOOP[17] = {
		SEG_DATA_NON_DECODE_0,
		SEG_DATA_NON_DECODE_1,
		SEG_DATA_NON_DECODE_2,
		SEG_DATA_NON_DECODE_3,
		SEG_DATA_NON_DECODE_4,
		SEG_DATA_NON_DECODE_5,
		SEG_DATA_NON_DECODE_6,
		SEG_DATA_NON_DECODE_7,
		SEG_DATA_NON_DECODE_8,
		SEG_DATA_NON_DECODE_9,
		SEG_DATA_NON_DECODE_0,
		SEG_DATA_NON_DECODE_A,
		SEG_DATA_NON_DECODE_B,
		SEG_DATA_NON_DECODE_C,
		SEG_DATA_NON_DECODE_D,
		SEG_DATA_NON_DECODE_E,
		SEG_DATA_NON_DECODE_F
	};

	// Loop through all elements
	int current=0;

	while(1){
		// Write to digit 0

		int student_id = 110611052;
		for(int i=0; i<8; i++){
			send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[i],student_id%10);
			student_id = (student_id-(student_id%10))/10;
		}
		delay_without_interrupt(1000);
	}

#endif

	while(1){}

	return 0;
}
