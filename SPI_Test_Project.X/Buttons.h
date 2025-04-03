#ifndef BUTTONS_H
#define	BUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif


#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes    


  
#define SWITCH_PRESSED_STATE                    0   // Active LOW switch

#define SWITCH_1                                1   // Switch 1 is pressed
#define SWITCH_3                                3   // Switch 1 is pressed
    
#define TICKS_PER_MS 100000  // 100MHz core timer = 100k ticks/ms
    
static volatile uint8_t prevSwitchPressedState = SWITCH_1;
static volatile uint8_t currSwitchPressedState = SWITCH_1;


void SW1_User_Handler(GPIO_PIN pin, uintptr_t context);
void SW2_User_Handler(GPIO_PIN pin, uintptr_t context);
void SW3_User_Handler(GPIO_PIN pin, uintptr_t context);

void initializeButtons(void);


#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */

