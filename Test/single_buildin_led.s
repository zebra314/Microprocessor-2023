/*
 * assembler.s
 *
 */

 .syntax unified
 .cpu cortex-m4
 .thumb

.data
	Leds: .byte 0

.text
	.global ledInit
	.global ledHigh
	.global ledLow
	.global ledBlink
	.equ GPIOA_MODER,	0x48000000
	.equ GPIOA_OTYPER,	0x48000004
	.equ GPIOA_OSPEEDR, 0x48000008
	.equ GPIOA_PUPDR, 	0x4800000C
	.equ GPIOA_ODR, 	0x48000014
	.equ RCC_AHB2SMENR, 0x4002106C
	.equ RCC_CIER, 0x40021018
	.equ RCC_CIFR, 0x4002101C

ledInit:

	// Enable AHB2 clock
	movs r0, #0x1
	ldr r1, =RCC_AHB2SMENR
	str r0, [r1]

	// Set PA5 as output mode
	movs r0, #0x400
	ldr r1, =GPIOA_MODER
	ldr r2, [r1]
	and r2, #0xFFFFF3FF
	orrs r2, r2, r0
	str r2, [r1]

	// Default PA5 is Pull-up output, no need to set

	// Set PA5 as high speed mode
	movs r0, #0x800 // store a number(2048) to register r0
	ldr r1, =GPIOA_OSPEEDR // store an address to r1
	strh r0, [r1] // store the number in r0 to the address in r1
	ldr r1, =GPIOA_ODR // store an address to r1 // ODR: OUTPUT, IDR: INPUT

ledBlink:
	movs r0, #(1<<5) // set the fifth digits to 1
	strh r0, [r1]
	bl delay
	movs r0, #0
	strh r0, [r1]
	bl delay
	b ledBlink

ledHigh:
	movs r0, #(1<<5) // set the fifth digits to 1
	strh r0, [r1]
	bl delay
	b ledHigh

ledLow:
	movs r0, #0
	strh r0, [r1]
	bl delay
	b ledLow

delay:
	mov r3, #20000
L1: mov r4, #100
L2:	subs r4, #1
	bne L2
	subs r3, #1
	bne L1
	bx LR


