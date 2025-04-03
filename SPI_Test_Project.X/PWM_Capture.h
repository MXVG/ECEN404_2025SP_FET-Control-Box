/* 
 * File:   PWM_Capture.h
 * Author: MX
 *
 * Created on November 18, 2024, 1:14 PM
 */

#ifndef PWM_CAPTURE_H
#define	PWM_CAPTURE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <xc.h>
#include "definitions.h"
#include "PWM_Signal.h"


/* initializePWMCapture()
 * 
 * PRECONDITION:
 * Startup
 * 
 * POSTCONDITION:
 * Timer6 initialized
 * Input Capture 9 initialized
 * 
 * ARGS:
 * None
 * 
 * RETURNS:
 * void
 * 
 */
void initializePWMCapture(void);


/* capturePWM()
 * 
 * PRECONDITION:
 * ICAP9 interrupt triggered on 3rd capture
 * 
 * POSTCONDITION:
 * IC9BUF is empty
 * ICNE flag is cleared
 * Interrupt flag is cleared
 * Frequency and Duty variables are updated
 * 
 * ARGS:
 * uintptr_t context = PWM_Signal
 * 
 * RETURNS:
 * void
 * 
 */
void capturePWM(uintptr_t context);

#endif	/* PWM_CAPTURE_H */
