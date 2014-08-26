/*-------|---------|---------|---------|---------|---------|---------|---------|
pulsecap.c

A program to read input pulses of between 1 and 2 ms and output 0-5V signal

Typical use case is decoding RC receiver signals

by chaz miller 

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3 or any later
version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
******************************************************************/
/*
Hardware: Arduino Pro Mini with fuses purged of Arduino nonsense
Servo attached to PB0. 
Optional LEDs on PB3-5 to indicate undersize pulse, oversize pulse, 
and timer overflow respectively.
PWM is output on PD6 at ~8kHz. 
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define pulse_max 4000
#define pulse_min 2000

void delay(uint16_t);
void blink (uint8_t);

int main(){

    DDRB=0xFE;
    DDRD=0xFF;

    //set up timer for input capture to read RC signals
    TCCR1B = (1<<CS11);        //clk/8
    TIMSK1|=1<<ICE1;           //enable input capture interrupt

    //set up pwm timer0
    TCCR0A = 0b10100011;       //fast pwm, page 103
    TCCR0B |= 1<<CS01;                //fcpu / 8

    uint16_t capture_val;
    uint8_t ledcnt;

    ISR(0x0014){
        if(TCCR1B&(1<<ICES)){         //1 is rising edge;
            TCNT1=0;
            TCCR1B&=~(1<<ICES);
        }else{
            capture_val=ICP1;
            TCCR1B|=(1<<ICES);        //set back to rising edge
        }
    }

    /****************************************
    *****main loop***************************
    ****************************************/
    for(;;){  
        
        if(TIFR1&1<<TOVF1){
            PORTB|=1<<5;          //turn on LED if timer has overflowed
        }

        //at 16MHz, capture_val should be 2000-4000 ticks

        if(capture_val>pulse_max){      //trim down
            capture_val=pulse_max;
            PORTB|=1<<4;      
        }
        if(capture_val<pulse_min){      //pad up
            capture_val=pulse_min+1;
            PORTB|=1<<3;      
        }
        capture_val-=pulse_min;
        OCR0A=capture_val<<3;

        ledcnt++;
        if(!ledcnt){PORTB=0}  //ok with PB0 as input??

    } //infty
}//main
