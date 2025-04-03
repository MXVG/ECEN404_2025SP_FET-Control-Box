/** Descriptive File Name

  @Company
 *  ECEN 403 FETBOX 

  @File Name
    encoder.h

  @Summary
 *  Simple encoder library for the PEC11R-4220F-S0024 rotary encoder

  @Description
 *  Simple encoder library for the PEC11R-4220F-S0024 rotary encoder
 */
/* ************************************************************************** */

#ifndef ENCODER_H
#define ENCODER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <xc.h>
#include "definitions.h"
#include "PWM_Signal.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

void encoder_initialize(void);


extern volatile double frequency;
extern volatile bool displayNeedsUpdate;

#endif /* ENCODER_H */

/* *****************************************************************************
 End of File
 */
