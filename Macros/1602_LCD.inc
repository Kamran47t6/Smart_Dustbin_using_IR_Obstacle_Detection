; Macros to send Data/Characters and commands to the 16x2 Character LCD


; Macro to send a command to control the LCD
.macro LCD_send_a_command
	PUSH r16
	LDI r16,@0 ; receives the command
	OUT PORTD,r16 ; Set the PD0 to PD7 pins according to command bits
	CBI PORTB, PB0 ; set RS pin to LOW (set LCD mode to "Command Mode")
	SBI PORTB, PB1 ; set E pin to HIGH (set LCD to receive the command)
	;delay 50
	delay 10
	CBI PORTB, PB1 ; set E pin to LOW (set LCD to process the command)
	LDI r16,0x00
	OUT PORTD,r16 ; clear the PD0 to PD7 pins after sending the command
	POP r16
.endmacro

; Macro to send a single character to display on the LCD
.macro LCD_send_a_character
	PUSH r16
	LDI r16,@0 ; receives the character
	OUT PORTD,r16 ; Set the PD0 to PD7 pins according to command bits
	SBI PORTB, PB0 ; set RS pin to HIGH (set LCD mode to "Data Mode")
	SBI PORTB, PB1 ; set E pin to HIGH (set LCD to receive the command)
	;delay 30
	delay 10
	CBI PORTB, PB1 ; set E pin to LOW (set LCD to process the command)
	LDI r16,0x00
	OUT PORTD,r16 ; clear the PD0 to PD7 pins after sending the command
	POP r16
.endmacro


; Macro to send a string (<256) to display on the LCD
; the Z register pair should have the address of the buffer
; R20 should have the length of the buffer
;	For example:	
;	LDI             ZL, LOW (2 * hello_string)
;	LDI             ZH, HIGH (2 * hello_string)
;	LDI             R20, string_len
;	
	;it is recommanded to define the strings at the end of the code segment
;	hello_string:	.db	"Hello World!",10,0
;	len: .equ	string_len   = 2 * (len - hello_string)

.macro LCD_send_a_string
	; save the states of R16 and R17
	PUSH            R16
	PUSH            R17
	LCD_writeBuffer_LOOP:
	; load the current byte/character pointed to be Z and increment the Z pointer
	LPM             R16, Z+
	; check if the remaining size of the string is non-zero and return if it is
	CPI             R20, 0
	BREQ            LCD_writeBuffer_END
	LCD_writeBuffer_CHAR:
	OUT PORTD, r16 ; Set the PD0 to PD7 pins according to command bits
	SBI PORTB, PB0 ; set RS pin to HIGH (set LCD mode to "Data Mode")
	SBI PORTB, PB1 ; set E pin to HIGH (set LCD to receive the command)
	;delay 30
	delay 10
	CBI PORTB, PB1 ; set E pin to LOW (set LCD to process the command)
	LDI r16,0x00
	OUT PORTD,r16 ; clear the PD0 to PD7 pins after sending the command
	DEC             R20
	RJMP            LCD_writeBuffer_LOOP
	LCD_writeBuffer_END:
	; restore the states of R16 and R17 and return
	POP             R17
	POP             R16
.endmacro


; Macro to send a register to display its value on the LCD
.macro LCD_send_a_register
	PUSH r16
	MOV r16,@0 ; receives the character
	OUT PORTD,r16 ; Set the PD0 to PD7 pins according to command bits
	SBI PORTB, PB0 ; set RS pin to HIGH (set LCD mode to "Data Mode")
	SBI PORTB, PB1 ; set E pin to HIGH (set LCD to receive the command)
	;delay 30
	delay 10
	CBI PORTB, PB1 ; set E pin to LOW (set LCD to process the command)
	LDI r16,0x00
	OUT PORTD,r16 ; clear the PD0 to PD7 pins after sending the command
	POP r16
.endmacro