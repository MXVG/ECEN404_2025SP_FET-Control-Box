/* 
 * File:   SPI_Display.h
 * Author: MX
 *
 * Created on March 29, 2025, 5:17 PM
 */

#ifndef SPI_DISPLAY_H
#define	SPI_DISPLAY_H

#include <xc.h>
#include "definitions.h"
#include <string.h>
#include <stdio.h>





#ifdef	__cplusplus
extern "C" {
#endif

    
    
extern unsigned char display_buffer[64 * 64];

void initializeDisplay(void);


void Set_Column_Address_25664(unsigned char a, unsigned char b);
void Set_Write_RAM_25664(void);
void Set_Read_RAM_25664(void);
void Set_Row_Address_25664(unsigned char a, unsigned char b);
void Set_Remap_25664(unsigned char a, unsigned char b);
void Set_Display_Start_Line_25664(unsigned char a);
void Set_Display_Offset_25664(unsigned char a);
void Display_Mode_25664(unsigned char a);
void Enable_Partial_Display_25664(unsigned char a, unsigned char b);
void Exit_Partial_Display_25664(void);
void Function_Selection_25664(unsigned char a);
void Set_Sleep_Mode_25664(unsigned char a);
void Set_Phase_Length_25664(unsigned char a);
void Set_Display_Clock__Oscillator_Frequency_25664(unsigned char a);
void Enable_External_VSL(unsigned char a, unsigned char b);
void Set_GPIO_25664(unsigned char a);
void Set_Second_Precharge_Period_25664(unsigned char a);
void Default_Grayscale_Command_25664(void);
void Set_Precharge_Voltage_25664(unsigned char a);
void Set_VCOMH_Voltage_25664(unsigned char a);
void Set_Contrast_Control_25664(unsigned char a);
void Master_Contrast_Control_25664(unsigned char a);
void Set_Multiplex_Ratio_25664(unsigned char a);
void Display_Enhancement_25664(unsigned char a);
void Set_Command_Lock_25664(unsigned char a);


void ClearPixel_25664(void);
void FillPixel_25664(void);
void ImageDisplay_25664(unsigned char *image);
void ImageDisplay_128x64_4bit(unsigned char *bitmap);
void draw_text_8x8(unsigned char *buffer, int x, int y, const char *text);

void draw_glyph_8x8(unsigned char *buffer, int x, int y, int glyph_index);

void update_display(double frequency, double duty, const char *mode, bool isExternal);

void command(uint8_t _command);
void data(uint8_t _data);    
void SendSPIByte(uint8_t data);

const char *formatDouble(double value, int decimal_places, const char *units);


void __delay_us(unsigned int us);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_DISPLAY_H */

