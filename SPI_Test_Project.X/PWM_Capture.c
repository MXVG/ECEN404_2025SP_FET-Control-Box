#include "PWM_Capture.h"

//PWM_Signal struct representing external PWM parameters
PWM_Signal external_PWM;

//IC9BUF storage array and indexer
/* catpuredValue Indexes:
 * Index 0: Rising Edge 1
 * Index 1: Falling Edge 1
 * Index 2: Rising Edge 2
 */
volatile uint8_t BUF9_Index;
uint16_t capturedValue[3];

uint32_t IC9BNE_mask = 0x00080000;

void initializePWMCapture(void){
    
    //Verify that ICAP pins are declared as inputs
    GPIO_PinInputEnable(GPIO_PIN_RD12);
    
    //Setup ICAP9 and TMR6 for PWM Capture
    ICAP3_Initialize();
    //TMR6_Initialize();
    
    ICAP3_Enable();
    //TMR6_Start();
    
    //Register capturePWM() to be called on 3rd capture event
    ICAP3_CallbackRegister(capturePWM, (uintptr_t)&external_PWM);
    
}


void capturePWM(uintptr_t context){
    
    PWM_Signal* capturedSignal = (PWM_Signal*)context;
    
    while((IC9BNE_mask & IC9CON) == true){
        
        capturedValue[BUF9_Index++] = ICAP3_CaptureBufferRead();
        
        if (BUF9_Index > 2){
                BUF9_Index = 0;
        }
    }
    
    //frequency = 1/(risetime1 - risetime0)
    capturedSignal->frequency = 1/(capturedValue[2] - capturedValue[0]);
    
    //duty = active_time/total_time
    capturedSignal->duty = (capturedValue[1] - capturedValue[0])/(capturedValue[2] - capturedValue[0]);
    
    
}
