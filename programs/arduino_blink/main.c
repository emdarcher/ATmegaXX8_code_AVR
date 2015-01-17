
#include "main.h"

/* defines */
#define PIN13_LED PB5
/* function prototypes */

void main(void){

    /* init stuff */
    DDRB |= (1<<PIN13_LED); 
    /* infinite loop */
    while(1){
        PORTB ^= (1<<PIN13_LED);
        _delay_ms(500);     
    }
}

/* function bodies */



/* Interrupt Service Routines */



