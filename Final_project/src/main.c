#include "stm32l476xx.h"
#include "helper_functions.h"
#include "led_button.h"
#include "max7219.h"
#include "timer.h"
#include "animation.h"
#include <stdio.h>
#include <stdlib.h>

// Define Counter timer
#define COUNTER_timer TIM2

// Define pins for MAX7219
#define MAX7219_gpio GPIOB
#define MAX7219_DIN 3
#define MAX7219_CS 4
#define MAX7219_CLK 5

void display(int x, int y, int z){
	for(int i=1; i<6; i++){
		if(z==i){
			for(int j=1; j<6; j++){
				if(y==j){
				 	choose_layer(MAX7219_gpio, i);			
					int output = (0b10000000>>(x-1));
					send_max7219(MAX7219_gpio, MAX7219_DIN, MAX7219_CS, MAX7219_CLK, j, output);
				}else{
					send_max7219(MAX7219_gpio, MAX7219_DIN, MAX7219_CS, MAX7219_CLK, j, 0x00);
				}
			}
		}
	}
}

Point point = {	.x_pos = 5,
				.y_pos = 5,
				.z_pos = 5,
				.x_dir = -0.348,
				.y_dir = -0.384,
				.z_dir = -0.87,
			};

int j = 0;
int k = 0;
int last_pin;
int led_pattern[6] = { MAX_00000111, MAX_00001111, MAX_00010000, MAX_00100000, MAX_01000000, MAX_10000000};
void SysTick_Handler() {
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		// Toggle LED display
		#ifdef DEBUG_led
		toggle_output(LED_gpio, LED_pin_1);
		#endif
		
		int pin = 0;
		reset_gpio(LED_gpio, last_pin);
		
		if(k%5 == 0)
			pin = LED_layer_3;
		else if(k%5 == 1)
			pin = LED_layer_4;
		else if(k%5 == 2)
			pin = LED_layer_5;
		else if(k%5 == 3)
			pin = LED_layer_1;
		else if(k%5 == 4)
			pin = LED_layer_2;
		last_pin = pin;
		set_gpio(LED_gpio, pin);

		for(int i=0; i<8; i++){
			send_max7219(MAX7219_gpio, MAX7219_DIN, MAX7219_CS, MAX7219_CLK, i+1, led_pattern[j%6]);
		}
		k++;
		j++;
	}
}


int main()
{
	if(init_led(LED_gpio, LED_pin_1)!= 0 || init_led(LED_gpio, LED_layer_3)!= 0
	||init_led(LED_gpio, LED_layer_4)!= 0 || init_led(LED_gpio, LED_layer_5)!= 0
	|| init_led(LED_gpio, LED_layer_1)!= 0 || init_led(LED_gpio, LED_layer_2)!= 0){
		return -1;
	}
	if(init_max7219(MAX7219_gpio, MAX7219_DIN, MAX7219_CS, MAX7219_CLK) != 0){
		return -1;
	}

	// Configure SysTick clk to 10 Mhz and interrupt every 0.4s
	SystemClock_Config_Interrupt(10, 100000);

    while(1) {
		
    }
    return 0;
}



