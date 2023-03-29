/*
 * part1.s
 *
 * Created on : 3/24 Fri
 * 		Author: alfonso
 */

.syntax unified
.cpu cortex-m4
.thumb

.data
	Leds: .byte 0

.text
	.global Init_PB
    .global main
    
	// PB setup
	.equ GPIOB_MODER,	0x48000400
	.equ GPIOB_OTYPER,	0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR, 	0x4800040C
	.equ GPIOB_ODR, 	0x48000414

	// Clock setup
	.equ RCC_AHB2ENR, 	0x4002104C

Init_PB:

	// Enable AHB2 clock
	movs r0, #0x6
	ldr r1, =RCC_AHB2ENR
	str r0, [r1]

	// Set PB8 as output mode
   	ldr r1, =GPIOB_MODER
    ldr r0, [r1]
    ldr r3, =0xFFFCFFFF // 1111 1111 1111 1100 1111 1111 1111 1111
    ldr r4, =0x10000    // 0000 0000 0000 0001 0000 0000 0000 0000
    and r0, r3
    orr r0, r4
    str r0, [r1]

	// Set PB8 as pull-down
	ldr r1, =GPIOB_PUPDR
    ldr r0, [r1]
    ldr r3, =0xFFFCFFFF // 1111 1111 1111 1100 1111 1111 1111 1111
    ldr r4, =0x20000    // 0000 0000 0000 0010 0000 0000 0000 0000
    and r0, r3
    orr r0, r4
    str r0, [r1]   

	// Set PB8 as open-drain
	ldr r1, =GPIOB_OTYPER
    ldr r0, [r1]
    ldr r3, =0xFFFFFF7F // 1111 1111 1111 1111 1111 1111 0111 1111
    ldr r4, =0x80       // 0000 0000 0000 0000 0000 0000 1000 0000
    and r0, r3
    orr r0, r4
    str r0, [r1]    

    // Set PB8 as medium speed
	ldr r1, =GPIOB_OSPEEDR
    ldr r0, [r1]
    ldr r3, =0xFFFCFFFF // 1111 1111 1111 1100 1111 1111 1111 1111
    ldr r4, =0x10000    // 0000 0000 0000 0001 0000 0000 0000 0000
    and r0, r3
    orr r0, r4
    str r0, [r1] 

    ldr r1, =GPIOB_ODR
    bx lr

main:
    Init_PB
    /* Do something with PB8 */ 
L: b L