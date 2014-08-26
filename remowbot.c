/*-------|---------|---------|---------|---------|---------|---------|---------|
mowerc.c	

a controller for radio control lawn mower

by chaz miller for ATMEGAxx8 set at 1MHz running at 5V. 

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3 or any later
version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
******************************************************************/
/*
Hardware: two rear wheels driven by stepper motors and casters in front
Steering is differential
There is a relay to enable ignition and a button for human input.
Also there is an R/C receiver which is signaling on 3 channels with
RC-type PPM. Channel 1 is throttle, CH2 is steer, CH3 is enable. 
*/

#include <avr/io.h>

void delay(uint16_t);
void blink (uint8_t);


int main(){

    //input for enable button
    //set up timer for input capture to read RC signals
    //set up timer(s) for output pulseage to wheel motors
    //watchdog
    //stall waiting for enable
    //enable 

    PORTB|=1<<5;                     //turn on LED

    /****************************************
    *****main loop***************************
    ****************************************/
    for(;;){  
        //kick watchdog
        //read RCs
        /************
        RC signal is read with input capture interrupt to measure 
        RC pulse width of all pulses in an RC PPM frame. 
        Values for first three pulses are stored in CH1 CH2 and CH3 bytes.
        *************/
        //filter RCs
        /***********
        If RC pulses are not within spec, it could mean radio interference
        or something else bad. 
        ***********/
        if((CH1>pulsemin) || (CH1<pulsemax)){die(1)}
        if((CH2>pulsemin) || (CH2<pulsemax)){die(2)}
        if((CH3>pulsemin) || (CH3<pulsemax)){die(3)}
        /***********
        If RC pulses have not changed significantly, operator may have died.
        ***********/
        //if(){die(10)}
        //if(){die(11)}
        /************
        If CH3 is greater than CH3_CUTOFF, user has requested kill, abort
        ************/
        if(CH3>CH3_CUTOFF){die(4)}

        //mogrify RCs
        /*
        CH1 is go. Minimum value is GO_MIN_CNT and max is GO_MAX_CNT both of which values
        are of only passing intrest. The zero speed is right in the middle at GO_ZERO_CNT which
        can be used as a software "trim". An offset of GO_ZERO_CNT is subtracted from the CH1
        value so that generally, positive values are forward and negative ones are backward. 

        CH2 is turn. Minimum value is TURN_MIN_CNT and max is TURN_MAX_CNT both of which values
        are of only passing intrest. The zero turn point is in the middle at TURN_ZERO_CNT which
        can be used as a software "trim". An offset of TURN_ZERO_CNT is subtracted from the CH2
        value so that generally, positive values are right and negative ones are left. 
        The resulting value is added to CH1 for the left wheel. It is subtracted from CH1 for 
        the right wheel. If the resulting values are negative, they are inverted in sign and
        the reverse bit is set. The results are then sent to the output
        stepper drives via TIMER1. 
        */

    } //infty
}//main

