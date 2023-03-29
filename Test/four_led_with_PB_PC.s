/*
 * lab2_1.s
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
	.global Init_PC
	.global ID_show_with_PB
	.global ID_show_with_PC

	// PB setup
	.equ GPIOB_MODER,	0x48000400
	.equ GPIOB_OTYPER,	0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR, 	0x4800040C
	.equ GPIOB_ODR, 	0x48000414

	//PC setup
	.equ GPIOC_MODER,	0x48000800
	.equ GPIOC_OTYPER,	0x48000804
	.equ GPIOC_PUPDR, 	0x4800080C
	.equ GPIOC_ODR, 	0x48000814
	.equ GPIOC_OSPEEDR, 0x48000808

	// Clock setup
	.equ RCC_AHB2ENR, 	0x4002104C


Init_PB:

	// Enable AHB2 clock
	movs r0, #0x6
	ldr r1, =RCC_AHB2ENR
	str r0, [r1]

	// Set PB3~6 as output mode
   	ldr r1, =GPIOB_MODER
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PB3~6  as Pull-up output
	ldr r1, =GPIOB_PUPDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PB3~6  as high speed mode
	ldr r1, =GPIOB_OSPEEDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x2A80
    str r0, [r1]

    ldr r1, =GPIOB_ODR
    bx lr


Init_PC:

	// Enable AHB2 clock
	movs r0, #0x6
	ldr r1, =RCC_AHB2ENR
	str r0, [r1]

	// Set PC3~6 as output mode
   	ldr r1, =GPIOC_MODER
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PC3~6  as Pull-up output
	ldr r1, =GPIOC_PUPDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]

	// Set PC3~6  as high speed mode
	ldr r1, =GPIOC_OSPEEDR
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x2A80
    str r0, [r1]

    ldr r1, =GPIOC_ODR
    bx lr

ID_show_with_PB:
	movs r0, #0x18 // 110611063 -> bin(24) -> hex(18)
	strh r0, [r1]
	b ID_show_with_PB

ID_show_with_PC:
	movs r0, #0x24 // 110611063 -> bin(24) -> hex(18)
	strh r0, [r1]
	b ID_show_with_PC
