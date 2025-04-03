/* 
 * File:   PWM_Generation.h
 * Author: MX
 *
 * Created on March 31, 2025, 8:33 AM
 */

#ifndef PWM_GENERATION_H
#define	PWM_GENERATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PBCLK_FREQ 100000000UL
    
    
void matching(double freq, double duty);
void alternating (double freq, float duty);
void PWMGEN();


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_GENERATION_H */

