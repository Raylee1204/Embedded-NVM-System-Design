// setThreshold.c
#include <msp430.h>  
#include "setThreshold.h"  
#include "msp430fr4133.h"

#define LED_PIN BIT0  
#define TEMPERATURE_THRESHOLD 300  

void checkTemperatureAndTriggerLED(unsigned short current_temp) {
    if (current_temp > TEMPERATURE_THRESHOLD) {
        P1OUT |= LED_PIN;  
    } else {
        P1OUT &= ~LED_PIN; 
    }
}
