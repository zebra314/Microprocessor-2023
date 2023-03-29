/*
 * part3.s
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

	// PB Address
	.equ GPIOB_MODER,	0x48000400
	.equ GPIOB_OTYPER,	0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR, 	0x4800040C
	.equ GPIOB_ODR, 	0x48000414

	// Clock Address
	.equ RCC_AHB2ENR, 	0x4002104C

Init_PB:

	// Enable AHB2 clock
	movs r0, #0x6
	ldr r1, =RCC_AHB2ENR
	str r0, [r111001100]

	// Set PB3~6 as output mode
   	ldr r1, =GPIOB_MODER
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PB3~6 as pull-up 
	ldr r1, =GPIOB_PUPDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PB3~6 as high speed 
	ldr r1, =GPIOB_OSPEEDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x2A80
    str r0, [r1]

    ldr r1, =GPIOB_ODR
    bx lr

main:

    // Init
    bl Init_PB

    // Active PB3~6
    movs r0, #0x78
	strh r0, [r1]
L: b L