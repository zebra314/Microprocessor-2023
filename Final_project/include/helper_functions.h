#ifndef __helper_function__H__
#define __helper_function__H__

#include "stm32l476xx.h"

// Define pins for led (default use on-board led PA5)
#define LED_gpio GPIOA
#define LED_pin_1 5
#define LED_layer_4 10
#define LED_layer_5 9
#define LED_layer_3 6
#define LED_layer_2 7
#define LED_layer_1 8

void choose_layer(GPIO_TypeDef* gpio, int pin);
int read_gpio(GPIO_TypeDef* gpio, int pin);
void write_gpio(GPIO_TypeDef* gpio, int pin, int data);
void set_gpio(GPIO_TypeDef* gpio, int pin);
void reset_gpio(GPIO_TypeDef* gpio, int pin);
void toggle_output(GPIO_TypeDef* gpio, int pin);

void delay_without_interrupt(int msec);

int num_digits(int x);

int gcd(int a, int b);
int lcm(int a, int b);

void FPU_init();
#endif
