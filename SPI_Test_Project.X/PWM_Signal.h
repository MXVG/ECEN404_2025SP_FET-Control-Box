/* 
 * File:   PWM_Signal.h
 * Author: MX
 *
 * Created on November 25, 2024, 4:21 PM
 */

#ifndef PWM_SIGNAL_H
#define	PWM_SIGNAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

typedef struct{
    
    double frequency;
    double duty;

    
}PWM_Signal;


PWM_Signal createPWM_Signal(double frequency, double duty)
{
    PWM_Signal sig;

    sig.frequency = frequency;
    sig.duty = duty;

    return sig;
}

#endif	/* PWM_SIGNAL_H */

