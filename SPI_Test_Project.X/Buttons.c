#include "../SPI_Test_Project.X/Buttons.h"
#include "macros.h"
#include "definitions.h"                // SYS function prototypes
#include "peripheral/coretimer/plib_coretimer.h"
#include "../SPI_Test_Project.X/Globals.h"
#include "../SPI_Test_Project.X/SPI_Display.h"

static uint32_t lastSW1PressTime = 0;

void SW1_User_Handler(GPIO_PIN pin, uintptr_t context)
{
    uint32_t now = CORETIMER_CounterGet();

    if ((now - lastSW1PressTime) < (200 * TICKS_PER_MS)) {
        return;  // debounce: ignore if less than 200ms since last press
    }

    lastSW1PressTime = now;

    if (SW1_Get() == SWITCH_PRESSED_STATE)
    {
        currSwitchPressedState = SWITCH_1;

        if (prevSwitchPressedState == currSwitchPressedState)
        {
            LED1_Toggle();

            
            modeIndex = (modeIndex + 1) % modeCount;
            strncpy(mode, modes[modeIndex], sizeof(mode) - 1);
            mode[sizeof(mode) - 1] = '\0';

            displayNeedsUpdate = true;  // Set the flag
            PWMNeedsUpdate = true;
        }

        prevSwitchPressedState = SWITCH_1;
    }
}

void SW2_User_Handler(GPIO_PIN pin, uintptr_t context)
{
    if(SW2_Get() == SWITCH_PRESSED_STATE)
    {
        LED1_Toggle();
    }
}


void SW3_User_Handler(GPIO_PIN pin, uintptr_t context)
{
    if(SW3_Get() == SWITCH_PRESSED_STATE)
    {
        currSwitchPressedState = SWITCH_3;

        if (prevSwitchPressedState == currSwitchPressedState)
        {
            LED1_Toggle();
        }
        prevSwitchPressedState = SWITCH_3;
    }
}

void initializeButtons(){
    GPIO_PinInterruptCallbackRegister(SW1_PIN, SW1_User_Handler, 0);
    GPIO_PinInterruptEnable(SW1_PIN);
    GPIO_PinInterruptCallbackRegister(SW2_PIN, SW2_User_Handler, 0);
    GPIO_PinInterruptEnable(SW2_PIN);
    GPIO_PinInterruptCallbackRegister(SW3_PIN, SW3_User_Handler, 0);
    GPIO_PinInterruptEnable(SW3_PIN);
}

