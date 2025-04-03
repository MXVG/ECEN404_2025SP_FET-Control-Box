#include "encoder.h"
#include "definitions.h"
#include "Globals.h"

#define FREQUENCY_STEP 1000


void encoderHandler(GPIO_PIN pin, uintptr_t context)
{
    static uint8_t prevAB = 0;

    uint8_t currA = GPIO_PinRead(PHASE_A_PIN);
    uint8_t currB = GPIO_PinRead(PHASE_B_PIN);
    for (volatile int i = 0; i < 1000; i++);  // 1-5us debounce delay
    uint8_t currAB = (currA << 1) | currB;

    uint8_t transition = (prevAB << 2) | currAB;

    // Lookup table based on valid quadrature transitions
    const int8_t transition_table[16] = {
        0,  +1,  -1,  0,
       -1,   0,   0, +1,
       +1,   0,   0, -1,
        0,  -1,  +1,  0
    };

    int8_t direction = transition_table[transition];
    prevAB = currAB;

    if (direction != 0)
    {
        _frequency += direction * FREQUENCY_STEP;
        if (_frequency < 100000) _frequency = 100000;
        displayNeedsUpdate = true;
        PWMNeedsUpdate = true;
    }
}


void encoderInitialize(void)
{
    

    GPIO_PinInterruptCallbackRegister(PHASE_A_PIN, encoderHandler, 0);
    GPIO_PinInterruptEnable(PHASE_A_PIN);
    
    GPIO_PinInterruptCallbackRegister(PHASE_B_PIN, encoderHandler, 0);
    GPIO_PinInterruptEnable(PHASE_B_PIN);
}