/*
 * lab2_1.s
 *
 */

 .syntax unified
 .cpu cortex-m4
 .thumb

.data
	Leds: .byte 0

.text
	.global Init1
	.global _1111
	.global _0000
	.global _0101
	.global _1010
	.global delay
	.global show
	.equ GPIOB_MODER,	0x48000400
	.equ GPIOB_OTYPER,	0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR, 	0x4800040C
	.equ GPIOB_ODR, 	0x48000414
	.equ RCC_AHB2SMENR, 0x4002106C
	.equ RCC_AHB2ENR, 	0x4002104C

Init1:

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

show:
	movs r0, #(1<<3)
	strh r0, [r1]
	bl delay
	movs r0, #(1<<4)
	strh r0, [r1]
	bl delay
	movs r0, #(1<<5)
	strh r0, [r1]
	bl delay
	movs r0, #(1<<6)
	strh r0, [r1]
	bl delay
	movs r0, #(1<<5)
	strh r0, [r1]
	bl delay
	movs r0, #(1<<4)
	strh r0, [r1]
	bl delay
	b show

_1111:
	movs r0, #0x78 // set the 3th~6th digits to 1
	strh r0, [r1]
	b _1111

_1010:
	movs r0, #0x50
	strh r0, [r1]
	b _1010

_0101:
	movs r0, #(1<<4)
	strh r0, [r1]
	b _0101

_0000:
	movs r0, #0x0
	strh r0, [r1]
	b _0000

delay:
	mov r3, #20000
L1: mov r4, #100
L2:	subs r4, #1
	bne L2
	subs r3, #1
	bne L1
	bx LR



