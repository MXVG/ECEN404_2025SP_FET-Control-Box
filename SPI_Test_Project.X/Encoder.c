#include "Encoder.h"
#include "macros.h"

//COARSE and FINE scaling factors frequency (Hz)
const double COARSE_SCALING_FACTOR = 10000;
const double FINE_SCALING_FACTOR = 1000;

//COARSE and FINE scaling factors for duty (unit-less)
const double DUTY_CS_FACTOR = 0.1;
const double DUTY_FS_FACTOR = 0.05;

const uint32_t ICBNE_MASK = 0x00080000;

PWM_Signal encoderPWM;

void initializeEncoders(void) {
    
    //ICAP1 & ICAP2 & TMR2 & TMR3 is responsible for Duty encoder phases
    ICAP2_Initialize();
    ICAP4_Initialize();
    //TMR2_Initialize();
    TMR3_Initialize();
    
    
    //ICAP7 & ICAP8 & TMR6 & TMR7 is responsible for Frequency encoder phases
    //ICAP7_Initialize();
    //ICAP8_Initialize();
    //TMR6_Initialize();
    //TMR7_Initialize();
    
    //Start ICAP and TMRS
    ICAP2_Enable();
    ICAP4_Enable();
    //TMR2_Start();
    TMR3_Start();
    
    //ICAP7_Enable();
    //ICAP8_Enable();
    //TMR6_Start();
    //TMR7_Start();
    
    //Start polling encoders @ 1000Hz
    TMR9_Initialize();
    TMR9_Start();
    TMR9_CallbackRegister(encoderPoll, (uintptr_t)&encoderPWM);
    
}

int phaseCompare(uint16_t pulseA, uint16_t pulseB){
    
    //IF encoder is turned CW, return +1
    //ELSE encoder is turned CCW, returned -1
    if(pulseA > pulseB){
        return 1;
    }else if(pulseA < pulseB){
        return -1;
    }
    
    return 0;
}

PWM_Signal updatePWM(uint32_t *_frequency, double *_duty){ 
    // +1 = CW
    // -1 = CCW
    //double freqDirection;
    double dutyDirection;
    
    //freqDirection = 1;
    dutyDirection = 1;
    
    //New signal starts with value of current frequency and duty settings
    PWM_Signal newSignal;
    newSignal.frequency = *_frequency;
    newSignal.duty = *_duty;
    
    //Storage for IxBUF values
    uint16_t riseTime2 = 0;
    uint16_t riseTime3 = 0;
    //uint16_t riseTime7 = 0;
    //uint16_t riseTime8 = 0;
    
    /*
    //If there is data in both buffers
    if(((ICBNE_MASK & IC7CON) == true) & ((ICBNE_MASK & IC8CON) == true)){
    
        //IF at least 2 values are in ICxBUF, then increment frequency COARSELY
        //ELSE increment frequency FINELY
        if((sizeof(IC7BUF)/16 >= 1) & (sizeof(IC8BUF)/16 >= 1)){

            riseTime7 = ICAP7_CaptureBufferRead();
            riseTime8 = ICAP8_CaptureBufferRead();

            freqDirection = phaseCompare(riseTime7, riseTime8);

            newSignal.frequency += freqDirection * COARSE_SCALING_FACTOR;
        }else{

            riseTime7 = ICAP7_CaptureBufferRead();
            riseTime8 = ICAP8_CaptureBufferRead();

            freqDirection = phaseCompare(riseTime7, riseTime8);

            newSignal.frequency += freqDirection * FINE_SCALING_FACTOR;

            //matching(newSignal.frequency, newDuty);

        }
    }
     * */
        
    if(((ICBNE_MASK & IC1CON) == true) & ((ICBNE_MASK & IC2CON) == true)){
    
        //IF at least 2 values are in ICxBUF, then increment DUTY COARSELY
        //ELSE increment DUTY FINELY
        if((sizeof(IC2BUF)/16 >= 2) & (sizeof(IC3BUF)/16 >= 2)){


            
            riseTime2 = ICAP2_CaptureBufferRead();
            riseTime3 = ICAP4_CaptureBufferRead();

            dutyDirection = phaseCompare(riseTime2, riseTime3);

            newSignal.duty += dutyDirection * COARSE_SCALING_FACTOR;
        }else{

            riseTime2 = ICAP2_CaptureBufferRead();
            riseTime3 = ICAP4_CaptureBufferRead();

            dutyDirection = phaseCompare(riseTime2, riseTime3);

            newSignal.duty += dutyDirection * FINE_SCALING_FACTOR;
        }
    }
    
    return newSignal;
}


void encoderPoll(uint32_t status, uintptr_t context){
    

    
    //PWM_Signal* newPWM = (PWM_Signal*)context;
    
    //bool IC1_flag = false;
    //bool IC2_flag = false;
    bool IC2_flag = false;
    bool IC4_flag = false;
    

    //IC1_flag = ICAP1_CaptureStatusGet();
    //IC2_flag = ICAP2_CaptureStatusGet();
    IC2_flag = ICAP2_CaptureStatusGet();
    IC4_flag = ICAP4_CaptureStatusGet();


    
    //If EITHER frequency or duty encoders are turned, update frequency and duty
    if((IC2_flag | IC4_flag)){
        
        LED1_Toggle();
        PWM_Signal newPWM = updatePWM(&encoderPWM.frequency, &encoderPWM.duty);
        //updateOutputPWM(newPWM->frequency, newPWM->duty);
        //matching(newPWM.frequency, newPWM.duty);
        
        encoderPWM.frequency = newPWM.frequency;
        encoderPWM.duty = newPWM.duty;
        


    }

}


/* *****************************************************************************
 End of File
 */

