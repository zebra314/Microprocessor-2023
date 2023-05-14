#include <stdio.h>
#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"
#include "led_button.h"
#include "timer.h"

// Define pins for led (default use on-board led PA5)
#define LED_gpio GPIOA
#define LED_pin 5

// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

// Define Counter timer
#define COUNTER_timer TIM2

void SysTick_Handler() {
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		// Toggle LED display
		toggle_output(LED_gpio, LED_pin);
	}
}

int main()
{
	// Cause we want to use floating points we need to init FPU
	FPU_init();
	if(init_led(LED_gpio, LED_pin) != 0){
		// Fail to init led
		return -1;
	}
	if(init_button(BUTTON_gpio, BUTTON_pin) != 0){
		// Fail to init button
		return -1;
	}
	// Configure SysTick clk to 10 Mhz and interrupt every 0.5s
	SystemClock_Config_Interrupt(10, 5000000);

	// button_press_cycle_per_second (How many button press segments in a second)
	int button_press_cycle_per_second = 20;
	// Use to state how many cycles to check per button_press_cycle
	int debounce_cycles = 50;
	// Use to state the threshold when we consider a button press
	int debounce_threshold = debounce_cycles*0.7;
	// Used to implement negative edge trigger 0=not-presses 1=pressed
	int last_button_state=0;


	int blink_frequencies[] = {5000000, 1000000, 3000000};
	int num_blink_frequencies = sizeof(blink_frequencies) / sizeof(blink_frequencies[0]);
	int current_blink_frequency_index = 0;
	int blink_frequency = blink_frequencies[1];
    while(1) {
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
					current_blink_frequency_index = (current_blink_frequency_index + 1) % num_blink_frequencies;
    				blink_frequency = blink_frequencies[current_blink_frequency_index];
    				SystemClock_Config_Interrupt(10, blink_frequency);
			    }
			    else{
				    // Pressed button - Continued pressing
				    // Do nothing
			    }
			    last_button_state = 1;
		    }
		    else{
			    if(last_button_state==0){
				    // Released button - Not pressing
				    // Do nothing
			    }
			    else{
				    // Released button - Neg edge
				     // Do nothing
			    }
				last_button_state = 0;
		    }
	    }
    }
    while(1);
    return 0;
}