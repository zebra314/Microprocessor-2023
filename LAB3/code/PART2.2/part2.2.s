/*
 * part2.2.s
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
	.global Init_PC
    .global main

	// PC Address
	.equ GPIOC_MODER,	0x48000800
	.equ GPIOC_OTYPER,	0x48000804
	.equ GPIOC_PUPDR, 	0x4800080C
	.equ GPIOC_ODR, 	0x48000814
	.equ GPIOC_OSPEEDR, 0x48000808

	// Clock Address
	.equ RCC_AHB2ENR, 	0x4002104C


Init_PC:

	// Enable AHB2 clock
	movs r0, #0x6
	ldr r1, =RCC_AHB2ENR
	str r0, [r111001100]

	// Set PC3~6 as output mode
   	ldr r1, =GPIOC_MODER
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PC3~6 as pull-up 
	ldr r1, =GPIOC_PUPDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PC3~6 as high speed 
	ldr r1, =GPIOC_OSPEEDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x2A80
    str r0, [r1]

    ldr r1, =GPIOC_ODR
    bx lr

main:

    // Init
    bl Init_PC

    // My ID is 110611063
    // dec(3) -> bin(11) -> show pattern 0011 
    // activate PB3 and PB4 -> write bin(1100)
    // bin(1100) -> hex(18)
    movs r0, #0x18 
	strh r0, [r1]
L: b L