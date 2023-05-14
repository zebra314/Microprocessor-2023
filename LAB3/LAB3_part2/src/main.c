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
	if(init_button(BUTTON_gpio, BUTTON_pin) != 0 ){
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
	for(int i=0; i<8; i++){//reset display
			send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[i],SEG_DATA_DECODE_BLANK);

	}
	//begin with 0
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[0],0);
	// Loop through all elements
	int a=0;
	int b=1;
	int c=0;
	int number = 0;
	int state = 0;
	int ch_state = 0;
	int button_press_persecond_cycle = 10;
	int debounce_cycle = 100;
	int debounce_threshold = 70;
	int last_botton_pos =1;
	while(1){
		// Write to digit 0

	for(int i=0; i<button_press_persecond_cycle; i++){
					int pos_cnt = 0; //count
					for(int a=0; a<debounce_cycle; a++){
						if(read_gpio(BUTTON_gpio, BUTTON_pin) == 0){
							pos_cnt++;
						}
						delay_without_interrupt(1000/(button_press_persecond_cycle*debounce_cycle));
					}
					if(pos_cnt > debounce_threshold){
						if(last_botton_pos == 1){
							last_botton_pos = 0;
						}

					}else{

						if(last_botton_pos == 0){
							if(a>100000000){

								for(int i=0; i<8; i++){//reset display
									send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[i],SEG_DATA_DECODE_BLANK);

								}
								send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[1],SEG_DATA_DECODE_DASH);
								send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[0],1);
							}else{
								number = a;
								c = a + b;
								a = b;
								b = c;

								for(int i=0; i<8; i++){//display
									if(number!=0){
									send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT[i],number%10);
									}
									number = (number-(number%10))/10;
								}

							last_botton_pos = 1;
						}
					}

			}

		}


	}

#endif

	while(1){}

	return 0;
}
