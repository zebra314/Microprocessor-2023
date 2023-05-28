#include "max7219.h"
#include "helper_functions.h"

int init_max7219(GPIO_TypeDef *gpio, int DIN, int CS, int CLK)
{
    // Enable AHB2 Clock
    if (gpio == GPIOA)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    }
    else if (gpio == GPIOB)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    }
    else if (gpio == GPIOC)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    }
    else
    {
        // Error! Add other cases to suit other GPIO pins
        return -1;
    }

    // Set GPIO pins to output mode (01)
    // First Clear bits(&) then set bits(|)
    gpio->MODER &= ~(0b11 << (2 * DIN));
    gpio->MODER |= (0b01 << (2 * DIN));
    gpio->MODER &= ~(0b11 << (2 * CS));
    gpio->MODER |= (0b01 << (2 * CS));
    gpio->MODER &= ~(0b11 << (2 * CLK));
    gpio->MODER |= (0b01 << (2 * CLK));

    // Set Decode Mode to non-decode mode
    send_max7219(gpio, DIN, CS, CLK, MAX_ADDRESS_DECODE_MODE, 0x00);
    // Set Intensity to 0x05
    send_max7219(gpio, DIN, CS, CLK, MAX_ADDRESS_ITENSITY, 0x01);
    // Set Scan Limit 
    send_max7219(gpio, DIN, CS, CLK, MAX_ADDRESS_SCAN_LIMIT, 0x07);
    // Wakeup max7219
    send_max7219(gpio, DIN, CS, CLK, MAX_ADDRESS_SHUTDOWN, 0x01);
    // Close display test
    send_max7219(gpio, DIN, CS, CLK, MAX_ADDRESS_DISPLAY_TEST, 0x00);
    
    return 0;
}

void send_max7219(GPIO_TypeDef *gpio, int DIN, int CS, int CLK, int address, int data)
{
    // The payload to send
    int payload = ((address & 0xFF) << 8) | (data & 0xFF);

    // Start the sending cycles
    // 16 data-bits + 1 CS signal
    int total_cycles = 16 + 1;

    for (int a = 1; a <= total_cycles; a++)
    {
        // Reset CLK when enter
        reset_gpio(gpio, CLK);

        // Set DIN according to data except for last cycle(CS)
        if (((payload >> (total_cycles - 1 - a)) & 0x1) && a != total_cycles)
        {
            set_gpio(gpio, DIN);
        }
        else
        {
            reset_gpio(gpio, DIN);
        }
        // Set CS at last cycle
        if (a == total_cycles)
        {
            set_gpio(gpio, CS);
        }
        else
        {
            reset_gpio(gpio, CS);
        }

        // Set CLK when leaving (7seg set data at rising edge)
        set_gpio(gpio, CLK);
    }
    return;
}

void write_byte(GPIO_TypeDef *gpio, int DIN, int CS, int CLK, uint8_t byte)
{
    // write byte
    for (int i = 0; i < 8; i++)
    {
        // Reset CLK when enter
        reset_gpio(gpio, CLK);

        write_gpio(gpio, DIN, byte & 0x80); // write the MSB bit to the data pin
        byte = byte << 1;                   // shift left

        set_gpio(gpio, CLK); // set the clock pin HIGH
    }
}

void write_matrix(GPIO_TypeDef *gpio, int DIN, int CS, int CLK, uint8_t address, uint8_t data)
{
    reset_gpio(gpio, CS); // pull the CS pin LOW
    write_byte(gpio, DIN, CS, CLK, address);
    write_byte(gpio, DIN, CS, CLK, data);
    set_gpio(gpio, CS); // pull the CS pin HIGH
}

void write_string(GPIO_TypeDef *gpio, int DIN, int CS, int CLK, char *str, int8_t led_array[][8])
{
    while (*str)
    {
        for (int i = 1; i < 9; i++)
        {
            write_matrix(gpio, DIN, CS, CLK, i, led_array[(*str - 55)][i - 1]);
        }
        str++;
        delay_without_interrupt(500);
    }
}