#include "stm32l476xx.h"
#include "helper_functions.h"
#include "led_button.h"

// Define pins for 4 leds
#define LED_gpio GPIOA
#define LED1_pin 7
#define LED2_pin 6
#define LED3_pin 5
#define LED4_pin 4
// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13
// #define lab_led

#ifdef lab_led
int main()
{

    if (init_led(LED_gpio, LED1_pin) != 0 || init_led(LED_gpio, LED2_pin) != 0 || init_led(LED_gpio, LED3_pin) != 0 || init_led(LED_gpio, LED4_pin) != 0)
    {
        // Fail to init LEDreturn -1;
    }
    if (init_button(BUTTON_gpio, BUTTON_pin) != 0)
    {
        return -1;
    }
    // Direction to shift 0=left, 1=right
    int shift_direction = 0;
    int led_data = 0b000001;
    int leds[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};
    int state = 1;
    int button_press_persecond_cycle = 10;
    int debounce_cycle = 100;
    int debounce_threshold = 70;
    int last_botton_pos = 1;
    while (1)
    {
        if (state == 1)
        { // 1 led
            // Shift led_data
            if (shift_direction == 0)
            {
                led_data = (led_data << 1);
            }
            else
            {
                led_data = (led_data >> 1);
            }
            // Check to see if change shift direction
            if (led_data == 0b000001 || led_data == 0b100000)
            {
                shift_direction = 1 - shift_direction;
            }
        }
        if (state == 3)
        { // 2 led
            // Shift led_data
            if (shift_direction == 0)
            {
                led_data = (led_data << 1);
            }
            else
            {
                led_data = (led_data >> 1);
            }
            // Check to see if change shift direction
            if (led_data == 0b000011 || led_data == 0b110000)
            {
                shift_direction = 1 - shift_direction;
            }
        }
        if (state == 5)
        { // 3 led
            // Shift led_data
            if (shift_direction == 0)
            {
                led_data = (led_data << 1);
            }
            else
            {
                led_data = (led_data >> 1);
            }
            // Check to see if change shift direction
            if (led_data == 0b000111 || led_data == 0b111000)
            {
                shift_direction = 1 - shift_direction;
            }
        }
        for (int a = 0; a < 4; a++)
        {
            // Active
            if ((led_data >> (a + 1)) & 0x1)
            {
                set_gpio(LED_gpio, leds[a]);
            }
            else
            {
                reset_gpio(LED_gpio, leds[a]);
            }
        }
        for (int i = 0; i < button_press_persecond_cycle; i++)
        {
            int pos_cnt = 0; // count
            for (int a = 0; a < debounce_cycle; a++)
            {
                if (read_gpio(BUTTON_gpio, BUTTON_pin) == 0)
                {
                    pos_cnt++;
                }
                delay_without_interrupt(1000 / (button_press_persecond_cycle *
                                                debounce_cycle));
            }
            if (pos_cnt > debounce_threshold)
            {
                if (last_botton_pos == 1)
                {
                    last_botton_pos = 0;
                }
            }
            else
            {
                if (last_botton_pos == 0)
                {
                    state++; // state mode: no, 1, no, 2, no, 3
                    if (state == 1)
                    {
                        led_data = 0b000001;
                        shift_direction = 0;
                    }
                    else if (state == 3)
                    {
                        led_data = 0b000011;
                        shift_direction = 0;
                    }
                    else if (state == 5)
                    {
                        led_data = 0b000111;
                        shift_direction = 0;
                    }
                    else if (state == 6)
                    {
                        state = 0;
                    }
                    last_botton_pos = 1;
                }
            }
        }
    }
}
#endif