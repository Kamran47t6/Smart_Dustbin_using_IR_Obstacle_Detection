 ; Code by CS 53, 54, 11, 32
.include "m328pdef.inc"
.include "delay.inc"
.include "motors.inc"

.equ sensorPin = PB4
.equ buzzerPin = PB5
.equ motorPin = PB1

.cseg

.org 0x0000

    CBI DDRB,sensorPin  ; input IR
	SBI DDRB,buzzerPin  ; buzzer output
	SBI DDRB, motorPin  ; motor Pin

	; Timer 1 setup PWM
	; Set OC1A and WMG11.
	LDI r16, 0b10100010
	STS TCCR1A, r16
	; Set WMG12 and WMG13 and Prescalar to 8
	LDI r16, 0b00011010
	STS TCCR1B, r16
	; clear counter
	LDI r16, 0
	STSw TCNT1H, r16, r16
	; count of 40000 for a 20ms period or 50 Hz cycle
	LDI r16, LOW(40000)
	LDI r17, HIGH(40000)
	STSw ICR1H,r17,r16

	loop:
			;0 degree
	LDI r16, LOW(900)
	LDI r17, HIGH(900)
	STSw OCR1AH,r17,r16

	SBIS PINB,sensorPin  ;compare sensor input
	CALL L1        ;jump to L1 label below
	rjmp loop

	L1:
	CALL motor
	
	ret
	
	
	motor:
		
		sbi PORTB,buzzerPin   ; set buzzer as ON

		; 90 degree
		LDI r16, LOW(2900)
		LDI r17, HIGH(2900)
		STSw OCR1AH,r17,r16

		delay 2000
		
		CBI PORTB,buzzerPin		;set buzzer as OFF
	ret