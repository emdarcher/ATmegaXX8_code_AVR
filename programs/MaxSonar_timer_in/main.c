

#include "uart.h"
#include "main.h"
#include "MaxSonar_EZ1.h"
/* defines */
#define DIG_TO_CHAR(d) ((char)((d) + 48))

#define DBG_PULSE 1 
#define DBG_CONS_NUM_TEST 0


const char hw_string[] PROGMEM  = "Hello, World!\n\r";
const char dist_text_str[] PROGMEM = "dist in inches: ";
const char pgm_newline[] PROGMEM = "\n\r";
#if DBG_CONS_NUM_TEST
const uint8_t test_num = 124;
const uint16_t dbg_us = 2039;
#endif

volatile uint16_t timer1_pulse_cnt = 0;
volatile uint16_t timer1_pulse_us = 0;
/* function prototypes */
static inline void init_timer1(void);
char * byteDig_to_str(uint8_t byteDig);

void main(void){

    /* init stuff */
    DDRD |= (1<<PD1);
    DDRB |= (1<<PB5);
//    DDRE |= (1<<PE1);
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    
    init_timer1();

    sei();
    /* infinite loop */
    while(1){
        uart0_puts_p(&dist_text_str);
        #if !DBG_CONS_NUM_TEST
        uart0_puts(byteDig_to_str(EZ1_US_TO_INCHES(timer1_pulse_us))); 
        #else
        uart0_puts(byteDig_to_str(EZ1_US_TO_INCHES(dbg_us))); 
        #endif
        uart0_puts_p(&pgm_newline);
    //   PORTE ^= (1<<PE1);
        PORTB ^= (1<<PB5);
        _delay_ms(1000);     
    }
}

/* function bodies */

static inline void init_timer1(void){
    DDRB &= ~(1<<PB0); //set PB0 to input
    TCCR1B |= (1<<ICES1); //sets ICP1 to use rising edge
    TCCR1B |= (1<<CS11); //sets clock to clk/8 so 2MHz from 16MHz    
    TIMSK1 |= (1<<ICIE1); //enables the input capture interrupt

}
char * byteDig_to_str(uint8_t byteDig){
    static char out_str[3]="   ";
    uint8_t digs[3];
    uint8_t val = byteDig;
    digs[0] = val / 100U;
    val -= digs[0] * 100U;
    digs[1] = val / 10U;
    val -= digs[1] * 10U;
    digs[2] = val ;
    uint8_t i=3;
    while(i--){
        out_str[i] = DIG_TO_CHAR(digs[i]);
    }
    return (char *)out_str;
}


/* Interrupt Service Routines */
ISR(TIMER1_CAPT_vect){
//isr for timer1 capture interrupt

    uint16_t icr1_store = (ICR1H<<8)|(ICR1L);  

    if(bit_is_set(TCCR1B,ICES1)){
        TCNT1 = 0; //reset cnt to 0
        TCCR1B &= ~(1<<ICES1); //clear bit so it detects falling edge       
    } else {
        timer1_pulse_cnt = icr1_store;        
        timer1_pulse_us = (timer1_pulse_cnt>>1); //divide by two for microseconds
        TCCR1B |= (1<<ICES1); //set bit so it detects rising edge
    }
}


