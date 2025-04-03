#include <string.h>
#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "../SPI_Test_Project.X/PWM_Generation.h"
#include "../SPI_Test_Project.X/macros.h"

double _frequency = 100000;
double _duty = 0.5;
char mode[16] = "MATCH";
int modeIndex = 0;
const char *modes[] = {
    "MATCH", "COMP", "ALT"
};
const int modeCount = sizeof(modes) / sizeof(modes[0]);

bool isExternal = false;

void matching (double freq, double duty){
    // Calculate timer period for each PWM signal
    double period = (SYS_FREQ/(2*freq)) - 1;
     
    // Configure OC1 for PWM mode
    OC1CON = 0;                // Turn off OC1 while configuring
    OC1R = period * duty;        // Set duty cycle for PWM1
    OC1RS = period * duty;       // Set duty cycle register for PWM1
    OC1CONbits.OCM = 0b110;   // PWM mode, fault pin disabled
    OC1CONbits.ON = 1;         // Turn on OC1

    // Configure OC2 for PWM mode
    OC2CON = 0;                // Turn off OC2 while configuring
    OC2R = period * duty;        // Set duty cycle for PWM2
    OC2RS = period * duty;       // Set duty cycle register for PWM2
    OC2CONbits.OCM = 0b110;   // PWM mode, fault pin disabled
    //OC2CONbits.ON = 1;         // Turn on OC2

    // Configure Timer2 for PWM frequency
    T2CON = 0;                          // Stop the timer and clear the control register
    //T2CONbits.TCKPS = 0b011;   // Set prescaler to 1:256
    PR2 = (SYS_FREQ / (2*freq)) - 1;   // Base period for Timer 2
    TMR2 = 0;                            // Clear the timer register
    TMR2_Start();                  // Start Timer2
   
}

void alternating (double freq, float duty){
    // Calculate timer period for each PWM signal
    double period = (CPU_CLOCK_FREQUENCY/(16*freq)) - 1;  
   
    // Configure OC1 for PWM mode
    OC1CON = 0;                // Turn off OC1 while configuring
    OC1R = period * duty;        // Set duty cycle for PWM1
    OC1RS = (period * duty);       // Set duty cycle register for PWM1
    OC1CONbits.OCM = 0b110;   // PWM mode, fault pin disabled
    OC1CONbits.ON = 1;         // Turn on OC1
   
    OC2CON = 0;
    OC2CONbits.OCM = 0b110; // PWM mode (no fault pin)
    OC2RS = period * duty;        // Set PWM duty cycle (50% as an example)
    OC2R = (period*duty);     // Set phase for OC2 to be 0 (this can be adjusted for 180-degree phase shift)
    OC2CONbits.ON = 1;      // Enable OC2

    // Configure Timer2 for PWM frequency
    T2CON = 0;                       // Stop the timer and clear the control register
    T2CONbits.TCKPS = 0b011;   // Set prescaler to 1:256
    PR2 = (CPU_CLOCK_FREQUENCY / (16*freq)) - 1;     // Base period for Timer 2
    TMR2 = 0;                           // Clear the timer register
    TMR2_Start();                 // Start Timer2
   

   
}

void complementary (double freq, float duty){
    // Calculate timer period for each PWM signal
    double period = (CPU_CLOCK_FREQUENCY/(16*freq)) - 1;
     
    OC2CON = 0;
    OC2CONbits.OCM = 0b110; // PWM mode (no fault pin)
    //adjust for delay
    unsigned int delay = 20;          // 150 ns delay in timer ticks
    OC2R = (period * duty) + delay;       // Delay OC1 by 150 ns
    OC2RS = (period * duty) + delay;      // Keep OC1RS synchronized              // Set phase for OC2 to be 0 (this can be adjusted for 180-degree phase shift)
    OC2CONbits.ON = 1;      // Enable OC2
   
    // Configure OC1 for PWM mode
    OC1CON = 0;                // Turn off OC1 while configuring
    OC1R = period * duty;        // Set duty cycle for PWM1
    OC1RS = (period * duty);       // Set duty cycle register for PWM1
    OC1CONbits.OCM = 0b110;   // PWM mode, fault pin disabled
    OC1CONbits.ON = 1;         // Turn on OC1

    // Configure Timer2 for PWM frequency
    T2CON = 0;                          // Stop the timer and clear the control register
    T2CONbits.TCKPS = 0b011;   // Set prescaler to 1:256
    PR2 = (CPU_CLOCK_FREQUENCY / (16*freq)) - 1;      // Base period for Timer 2
    TMR2 = 0;                             // Clear the timer register
    TMR2_Start();                         // Start Timer2
   
   
}


void PWMGEN(void)
{
    if (strcmp(mode, "MATCH") == 0)
    {
        matching(_frequency, _duty);
    }
    else if (strcmp(mode, "ALT") == 0)
    {
        alternating(_frequency, _duty);
    }
    else if (strcmp(mode, "COMP") == 0)
    {
        complementary(_frequency, _duty);
    }
    else
    {
        // Default to MATCH if the mode string is unknown
        matching(_frequency, _duty);
    }
}