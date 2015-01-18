

#include "uart.h"
#include "main.h"
#include "MaxSonar_EZ1.h"
/* defines */


const char hw_string[] PROGMEM  = "Hello, World!\n\r";

/* function prototypes */

void main(void){

    /* init stuff */
    DDRD |= (1<<PD1);
    DDRB |= (1<<PB5);
//    DDRE |= (1<<PE1);
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));

    sei();
    /* infinite loop */
    while(1){
        uart0_puts_p(&hw_string);
    //   PORTE ^= (1<<PE1);
        PORTB ^= (1<<PB5);
        _delay_ms(1000);     
    }
}

/* function bodies */



/* Interrupt Service Routines */



