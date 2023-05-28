#ifndef __max7219__H__
#define __max7219__H__

#include "stm32l476xx.h"

#define MAX_ADDRESS_DECODE_MODE		9
#define MAX_ADDRESS_ITENSITY		10
#define MAX_ADDRESS_SCAN_LIMIT		11
#define MAX_ADDRESS_SHUTDOWN		12
#define MAX_ADDRESS_DISPLAY_TEST	15

int init_max7219(GPIO_TypeDef* gpio, int DIN, int CS, int CLK);
void send_max7219(GPIO_TypeDef* gpio, int DIN, int CS, int CLK, int address, int data);
void write_byte(GPIO_TypeDef* gpio, int DIN, int CS, int CLK, uint8_t byte);
void write_matrix(GPIO_TypeDef* gpio, int DIN, int CS, int CLK, uint8_t address, uint8_t data);
void write_string(GPIO_TypeDef* gpio, int DIN, int CS, int CLK, char* str, int8_t led_array[38][8]);
#endif