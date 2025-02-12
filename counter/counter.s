.title "EE308 PIC24F Curiosity Board"
.sbttl "Initialization Code"
.equ __24FJ128GA204, 1
.include "p24FJ128GA204.inc"

config __CONFIG1, JTAGEN_OFF    
;config __CONFIG2, FNOSC_FRCPLL
;config __CONFIG4, PLLDIV_PLL4X
    
.global __reset          ;The label for the first line of code. 
.global __T1Interrupt    ;Declare Timer 1 ISR name global

.bss
    count:	.space 2

.section .const,psv
    lookUp:	.ascii "0123456789ABCDEF"
    line1:	.ascii "PIC24 Board v2.2"
    line2:	.ascii "Counter   -->  0"

 .text		  ;Start of Code section
__reset:
    mov	    #__SP_init, W15	; Initalize the Stack Pointer
    mov	    #__SPLIM_init, W0	; Initialize the Stack Pointer Limit Register
    mov	    W0, SPLIM
    nop				; Add NOP to follow SPLIM initialization
    
    mov	    #edspage(lookUp), W0
    mov	    W0, DSRPAG
    
    bclr    LATA, #9
    bclr    LATA, #10
    bclr    TRISA, #9
    bclr    TRISA, #10
    
    call    init_timer
    call    init_I2C
    call    init_LCD

    mov	    #0, W0
    mov	    W0, count
    
    
MainLoop:
    clrwdt
    
    mov	    count, W0
    inc	    W0, W0
    mov	    W0, count
    
    mov     #0xFFFF, W6
    mov	    #0x000F, W1
    and	    W0, W1, W2
    mov	    #edsoffset(lookUp), W3
    mov.b   [W3+W2], W4
    mov.b   #0x3A, W5
    cp      W4, W5
    bra     GEU, no_display
    mov	    #0xCF, W12
    call    send_LCD_CMD
    mov.b   W4, W12
    call    send_LCD_DATA
    mov	    #0xCE, W12
    call    send_LCD_CMD
    mov.b   #0x20, W12
    call    send_LCD_DATA
    call    dlyloop
    bra     MainLoop
   
no_display:
    sub.b   #10, W0
    and	    W0, W1, W2
    mov.b   [W3+W2], W12
    call    send_LCD_DATA
    mov	    #0xCE, W12
    call    send_LCD_CMD
    mov.b   #0x31, W12
    call    send_LCD_DATA

dlyloop:
    repeat  #40
    nop
    sub	    W6, #1, W6
    bra	    NZ, dlyloop
    return
    
    

init_timer:
    bclr    T1CON, #TON		; turn timer1 OFF

    bset    T1CON, #TCKPS1
    bset    T1CON, #TCKPS0	; set prescaler to 256

    bclr    T1CON, #TCS		; select internal clock

    mov	    #0x0000, W0 
    mov	    W0, TMR1		; clear TMR1 register
    mov	    #3905, W0
    mov	    W0, PR1		; set timer1 period to 3906 -> f = 2e6/256/3096 = 2Hz
    ;mov	    W0, PR1		; set timer1 period to 15625 -> f = 8e6/256/15625 = 2Hz

    bclr    IFS0, #T1IF		; clear timer1 interrupt status flag
    bset    IEC0, #T1IE		; enable timer1 interrupts

    bset    T1CON, #TON	; turn timer1 ON

    return

  
__T1Interrupt:
    bclr    IFS0, #T1IF
    
    btg	    LATA, #9
    ;btg	    LATA, #10

    retfie			; Return from Interrupt Service routine
    
init_I2C:
    bclr    ANSB, #2
    bclr    ANSB, #3
    mov	    #0x27, W0
    mov	    W0, I2C2BRG                     ; set BAUD rate
    bclr    I2C2CONL, #A10M          ; 7-bit slave address
    bset    I2C2CONL, #I2CEN         ; enable I2C module #1
    return
    
init_LCD:
    mov.b #0x08, W12
    call send_1_LCD
    call delay
    mov.b #0x3C, W12
    call send_1_LCD
    call delay
    mov.b #0x3C, W12
    call send_1_LCD
    call delay
    mov.b #0x3C, W12
    call send_1_LCD
    call delay
    mov.b #0x2C, W12
    call send_1_LCD
    call delay
    
    mov.b #0x28, W12
    call send_LCD_CMD
    call delay 
    mov.b #0x0E, W12
    call send_LCD_CMD
    call delay 
    mov.b #0x01, W12
    call send_LCD_CMD
    call delay
    
    mov	    #0, W0
    mov	    #edsoffset(line1), W1
init_LCD_loop1:
    mov.b   [W0+W1], W12
    call    send_LCD_DATA
    inc	    W0, W0
    cp	    W0, #16
    bra	    NZ, init_LCD_loop1
    mov	    #0xC0, W12
    call    send_LCD_CMD
    mov	    #0, W0
    mov	    #edsoffset(line2), W1
init_LCD_loop2:
    mov.b   [W0+W1], W12
    call    send_LCD_DATA
    inc	    W0, W0
    cp	    W0, #16
    bra	    NZ, init_LCD_loop2   
;    mov.b #0x41, W12
;    call send_LCD_DATA
;    mov.b #0x41, W12
;    call send_LCD_DATA
    return

send_LCD_CMD:
    mov.b #0x0C, W13
    call  send_2_LCD
    return
send_LCD_DATA:
    mov.b #0x0D, W13
    call  send_2_LCD
    return   

send_1_LCD:
    mov.b #0x4E, W11
    call I2C_start
    call I2C_write
    mov.b W12, W11
    call I2C_write
    bclr W11, #2
    call I2C_write
    call I2C_stop
;   call delay
    return 
    
send_2_LCD:
    mov.b #0x4E, W11
    call I2C_start
    call I2C_write
    mov.b #0xF0, W11
    and.b W11, W12, W11
    ior.b W11, W13, W11
    call I2C_write
    bclr W11, #2
    call I2C_write
    sl   W12, #4, W12
    mov.b #0xF0, W11
    and.b W11, W12, W11
    ior.b W11, W13, W11
    call I2C_write
    bclr W11, #2
    call I2C_write
;    mov #0, W11
;    call I2C_write
    call I2C_stop
;    call delay
    return

delay:
    repeat #16000
    nop
    return
    
    
; :::::::::::::
; I2C Functions
; :::::::::::::

I2C_start:
    bset	I2C2CONL, #SEN		; start condition
wait_start:
    btsc	I2C2CONL, #SEN		; start condition in progress?
    bra		wait_start			; yes, wait
    return

I2C_stop:
    bset	I2C2CONL, #PEN		; stop condition
wait_stop:
    btsc	I2C2CONL, #PEN		; stop condition in progress?
    bra		wait_stop			; yes, wait
    return

I2C_restart:
    bset	I2C2CONL, #RSEN		; repeated start condition
wait_rstart:
    btsc	I2C2CONL, #RSEN		; repeated start condition in progress?
    bra		wait_rstart			; yes, wait
    return

I2C_write:
    mov		W11, I2C2TRN
wait_write:
    btsc	I2C2STAT, #TBF
    bra		wait_write
wait_slave_ack:
    btsc	I2C2STAT, #TRSTAT
    bra		wait_slave_ack
    return

I2C_read:
    bset	I2C2CONL, #RCEN		; master receive enable
wait_read:
    btsc	I2C2CONL, #RCEN
    bra		wait_read
    mov		I2C2RCV, W11
    return

I2C_ack:
    bset	I2C2CONL, #ACKEN		; send ack
wait_ack:
    btsc	I2C2CONL, #ACKEN
    bra		wait_ack
    return

I2C_nack:
    bset	I2C2CONL, #ACKDT
    bset	I2C2CONL, #ACKEN		; send nack
wait_nack:
    btsc	I2C2CONL, #ACKEN
    bra		wait_nack
    bclr	I2C2CONL, #ACKDT
    return
    
    .end
    