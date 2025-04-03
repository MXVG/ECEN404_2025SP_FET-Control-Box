#include "SPI_Display.h"
#include "macros.h"
#include "../SPI_Test_Project.X/Unibody_Font.h"
#include "../SPI_Test_Project.X/Static_UI.h"

unsigned char display_buffer[64 * 64] = {0};

void initializeDisplay(void){
    
    DISPLAY_RESET_OutputEnable();
    DC_OutputEnable();
    
    DISPLAY_RESET_Set();
    
    SPI1_Initialize();
    
    DISPLAY_RESET_Clear();
    __delay_us(150000);
    DISPLAY_RESET_Set();
    __delay_us(150000);
    
    Set_Command_Lock_25664(0x12);                             // Unlock commands for OLED
    Set_Display_Clock__Oscillator_Frequency_25664(0xD1);      // Set Display Clock / Oscillator Frequency
    Set_Multiplex_Ratio_25664(0x3F);                          // Set Multiplex Ratio
    Set_Display_Offset_25664(0x00);                           // Set Display Offset 
    Set_Display_Start_Line_25664(0x00);                       // Set Display Start Line
    Set_Remap_25664(0x14,0x11);                               // Set Remap. (A[1]) & (A[4]) can be adjusted to flip display orientation
    Set_GPIO_25664(0x00);                                     // Set GPIO 
    Function_Selection_25664(0x00);                           // Function Selection
    Enable_External_VSL(0xA0,0xB5);                           // Enable External VSL
    Set_Contrast_Control_25664(0x9F);                         // Set Contrast Control
    Master_Contrast_Control_25664(0x0F);                      // Master Contrast Control
    Default_Grayscale_Command_25664();                        // Default Linear Grayscale Table
    Set_Phase_Length_25664(0x74);                             // Set Phase Length
    Display_Enhancement_25664(0xA2);                          // Display Enhancement
    Set_Precharge_Voltage_25664(0x17);                        // Set Precharge Voltage
    Set_Second_Precharge_Period_25664(0x08);                  // Set Second Precharge Period
    Set_VCOMH_Voltage_25664(0x07);                            // Set VCOMH Voltage
    Display_Mode_25664(0xA6);                                 // Set Normal Display
    Exit_Partial_Display_25664();                             // Exit Partial Display Mode
    Set_Sleep_Mode_25664(0xAF);
    
    __delay_us(10000);
}

void SendSPIByte(uint8_t data){
    // Wait if SPI is busy
    while (SPI1_IsBusy());

    // Send one byte
    uint8_t txData = data;
    SPI1_Write(&txData, 1);

    // Optionally wait until transmission is complete
    while (SPI1_IsBusy());
}


void command(uint8_t _command){
    
    DC_Clear();
    SendSPIByte(_command);
    //Strobe C/D pin
    //WR->0 (automatically done)
    
}

void data(uint8_t _data){
    DC_Set();
    SendSPIByte(_data);
}


//
// COMMANDS
//
void Set_Column_Address_25664(unsigned char a, unsigned char b)
{
  command(0x15);  // Set Column Address
  data(a);        //   Default => 0x00
  data(b);        //   Default => 0x77
}
//--------------------------------------------------------------------------

void Set_Write_RAM_25664()
{
  command(0x5C);  // Enable MCU to Write DATA RAM
}
//--------------------------------------------------------------------------

void Set_Read_RAM_25664()
{
  command(0x5D);  // Enable MCU to Write DATA RAM
}
//--------------------------------------------------------------------------

void Set_Row_Address_25664(unsigned char a, unsigned char b)
{
  command(0x75);  // Set Row Start and End Address
  data(a);        // Default => 0x00
  data(b);        // Default => 0x7F
}
//--------------------------------------------------------------------------

void Set_Remap_25664(unsigned char a, unsigned char b)
{
  command(0xA0);  // Set Remap
  data(a);        // Default => 0x00
  data(b);        // Default => 0x00
}
//--------------------------------------------------------------------------

void Set_Display_Start_Line_25664(unsigned char a)
{
  command(0xA1);  // Set Display RAM Display Start Line
  data(a);        // Default => 0x00
}
//--------------------------------------------------------------------------

void Set_Display_Offset_25664(unsigned char a)
{
  command(0xA2);  // Set Verticle Shift
  data(a);        // Default => 0x00
}
//--------------------------------------------------------------------------

void Display_Mode_25664(unsigned char a)
{
  command(a);     // 0xA4 => Entire Display OFF 
                  // 0xA5 => Entire Display ON, all pixels Grayscale level 15
                  // 0xA6 => Normal Display (Default)
                  // 0xA7 => Inverse Display
}
//--------------------------------------------------------------------------

void Enable_Partial_Display_25664(unsigned char a, unsigned char b)
{
  command(0xA8);  // Turns ON partial mode 
  data(a);        // Address of start row
  data(b);        // Address of end row. 
}
//--------------------------------------------------------------------------

void Exit_Partial_Display_25664()
{
  command(0xA9);  // Exit Partial Display Mode
}
//--------------------------------------------------------------------------

void Function_Selection_25664(unsigned char a)
{
  command(0xAB);  // Function Selection
  data(a);        // Default => 0x01
}
//--------------------------------------------------------------------------

void Set_Sleep_Mode_25664(unsigned char a)
{
  command(a);     // 0xAE => Sleep Mode ON
                  // 0xAF => Sleep Mode OFF
}
//--------------------------------------------------------------------------

void Set_Phase_Length_25664(unsigned char a)
{
  command(0xB1);  // Set Phase Length
  data(a);        // Default => 0x97
}
//--------------------------------------------------------------------------

void Set_Display_Clock__Oscillator_Frequency_25664(unsigned char a)
{
  command(0xB3);  // Set Osc Frequency
  data(a);        // 
}
//--------------------------------------------------------------------------

void Enable_External_VSL(unsigned char a, unsigned char b)
{
  command(0xB4);  // Enable External VSL
  data(a);        // 
  data(b);        //  
}
//--------------------------------------------------------------------------

void Set_GPIO_25664(unsigned char a)
{
  command(0xB5);  // Set GPIO
  data(a);        // 
}
//--------------------------------------------------------------------------

void Set_Second_Precharge_Period_25664(unsigned char a)
{
  command(0xB6);  // Set Second Precharge Period
  data(a);        // 
}
//--------------------------------------------------------------------------

void Default_Grayscale_Command_25664()
{
  command(0xB9);  // Sets Default Grayscale
}
//--------------------------------------------------------------------------

void Set_Precharge_Voltage_25664(unsigned char a)
{
  command(0xBB);  // Set Precharge Voltage Level
  data(a);        // Default => 0x17
}
//--------------------------------------------------------------------------

void Set_VCOMH_Voltage_25664(unsigned char a)
{
  command(0xBE);  // Set Second Precharge Period
  data(a);        // Default => 0x04
}
//--------------------------------------------------------------------------

void Set_Contrast_Control_25664(unsigned char a)
{
  command(0xC1);  // Set Contrast Control Level
  data(a);        // Default => 0x7F
}
//--------------------------------------------------------------------------

void Master_Contrast_Control_25664(unsigned char a)
{
  command(0xC7);  // Master Contrast Control
  data(a);        // Default => 0x0F
}
//--------------------------------------------------------------------------

void Set_Multiplex_Ratio_25664(unsigned char a)
{
  command(0xCA);  // Set MUX Ratio
  data(a);        // Default => 0x7F
}
//--------------------------------------------------------------------------

void Display_Enhancement_25664(unsigned char a)
{
  command(0xD1);  // Display Enhancement
  data(a);        // 
  data(0x20);     // 
}
//--------------------------------------------------------------------------

void Set_Command_Lock_25664(unsigned char a)
{
  command(0xFD);  // Set Lock/Unlock Commands
  data(a);        // Default => 0x12
}

void draw_text_8x8(unsigned char *buffer, int x, int y, const char *text)
{
    // Ensure X is aligned
    x &= ~1;
    
    while (*text)
    {
        const char *char_ptr = strchr(font8x8_char_map, *text);
        if (!char_ptr) {
            text++;
            continue;
        }

        int char_index = char_ptr - font8x8_char_map;
        const unsigned char *glyph = &font8x8[char_index * 32];  // 32 bytes per char

        for (int row = 0; row < 8; row++) {
            int buf_y = y + row;
            if (buf_y >= 64) continue;

            for (int col = 0; col < 8; col += 2) {
                int buf_x = x + col;
                if (buf_x >= 128) continue;

                int buffer_byte_index = buf_y * 64 + (buf_x / 2);
                int glyph_byte_index = row * 4 + (col / 2);  // 4 bytes per row
                unsigned char glyph_byte = glyph[glyph_byte_index];

                buffer[buffer_byte_index] = glyph_byte;
            }
        }

        x += 8;  // move to next character
        text++;
    }
}

void draw_glyph_8x8(unsigned char *buffer, int x, int y, int glyph_index)
{
    // Make sure x is even (2 pixels per byte)
    x &= ~1;

    if (glyph_index < 0 || glyph_index >= FONT8X8_CHAR_COUNT) return;

    const unsigned char *glyph = &font8x8[glyph_index * FONT8X8_BYTES_PER_CHAR];

    for (int row = 0; row < FONT8X8_HEIGHT; row++) {
        int buf_y = y + row;
        if (buf_y >= 64) continue;

        for (int col = 0; col < FONT8X8_WIDTH; col += 2) {
            int buf_x = x + col;
            if (buf_x >= 128) continue;

            int buffer_index = buf_y * 64 + (buf_x / 2);            // 64 bytes per row
            int glyph_index = row * 4 + (col / 2);                  // 4 bytes per glyph row
            buffer[buffer_index] = glyph[glyph_index];             // 2 pixels in one byte
        }
    }
}


void update_display(double frequency, double duty, const char *mode, bool isExternal)
{

    memcpy(display_buffer, static_ui, sizeof(display_buffer));
    
    draw_text_8x8(display_buffer, 75, 0, formatDouble(frequency/1000, 0, "KHZ"));  // Frequency
    draw_text_8x8(display_buffer, 80, 16, formatDouble(duty, 1, "%"));       // Duty
    draw_text_8x8(display_buffer, 80, 32, mode);                           // Mode
    draw_text_8x8(display_buffer, 80, 48, isExternal ? "EXT" : "INT");                   // Int/Ext

    ImageDisplay_128x64_4bit(display_buffer);
}


void ImageDisplay_25664(unsigned char *image)
{
  unsigned int i, j, buff;
  Set_Remap_25664(0x14,0x11); 
//Set_Remap_25664(0x06,0x11); // To Flip Orientation 180 Degrees     
  Set_Column_Address_25664(0x1C,0x5B);
  Set_Row_Address_25664(0x00,0x3F);
  Set_Write_RAM_25664(); 
                           
  for(i=0;i<64;i++) //Columns
  {
    for(j=0;j<32;j++) //Rows
    {
      buff = *image;
      buff = ((buff >> 6) & 0x03);
      if(buff == 0x03){data(0xFF);}
      else if(buff == 0x02){data(0xF0);}
      else if(buff == 0x01){data(0x0F);}
      else data(0x00);

      buff = *image;
      buff = ((buff >> 4) & 0x03);
      if(buff == 0x03){data(0xFF);}
      else if(buff == 0x02){data(0xF0);}
      else if(buff == 0x01){data(0x0F);}
      else data(0x00);

      buff = *image;
      buff = ((buff >> 2) & 0x03);
      if(buff == 0x03){data(0xFF);}
      else if(buff == 0x02){data(0xF0);}
      else if(buff == 0x01){data(0x0F);}
      else data(0x00);

      buff = *image;
      buff = (buff & 0x03);
      if(buff == 0x03){data(0xFF);}
      else if(buff == 0x02){data(0xF0);}
      else if(buff == 0x01){data(0x0F);}
      else data(0x00);

      image++;
    }
  }
}

void ClearPixel_25664()
{
  unsigned int i, j;
  
  Set_Column_Address_25664(0x1C,0x5B);
  Set_Row_Address_25664(0x00,0x3F);
  Set_Write_RAM_25664();

  for(i=0;i<64;i++) //Columns
  {
    for(j=0;j<128;j++) //Rows
    {
      data(0x00);
      __delay_us(100);
    }
  }
}

void FillPixel_25664()
{
  unsigned int i, j;
  
  Set_Column_Address_25664(0x1C,0x5B); //1C & 5B = DISPLAY Start & End address.
  Set_Row_Address_25664(0x00,0x3F);
  Set_Write_RAM_25664();

  for(i=0;i<64;i++) //Columns
  {
      for(j = 0; j < 128; j++){
          
          if(i % 2 == 0){
              data(0x0F);
              __delay_us(100);
          }else{
            data(0xF0);
            __delay_us(100);
          }
          
      }
  }
}

void ImageDisplay_128x64_4bit(unsigned char *bitmap)
{
    unsigned int i, j;
    unsigned char byte, high_nibble, low_nibble;

    Set_Remap_25664(0x14, 0x11);
    Set_Column_Address_25664(0x1C, 0x5B); // Column range for 128 pixels
    Set_Row_Address_25664(0x00, 0x3F);    // Row range for 64 pixels
    Set_Write_RAM_25664();

    for (i = 0; i < 64; i++) // 64 rows
    {
        for (j = 0; j < 64; j++) // 64 bytes = 128 pixels (2 per byte)
        {
            byte = *bitmap++;

            // Extract two 4-bit pixel values
            high_nibble = (byte >> 4) & 0x0F;
            low_nibble  = byte & 0x0F;

            // Send them directly
            data(high_nibble << 4); // Convert 4-bit to 8-bit grayscale
            data(low_nibble << 4);
        }
    }
}

void __delay_us(unsigned int us)
{
    // Convert microseconds us into how many clock ticks it will take
    us *= SYS_FREQ / 1000000 / 2; // Core Timer updates every 2 ticks

    _CP0_SET_COUNT(0);

    while (us > _CP0_GET_COUNT()); 
}


const char *formatDouble(double value, int decimal_places, const char *units)
{
    static char buffer[24];  //NOTE: Persistent across calls

    if (decimal_places < 0) decimal_places = 0;
    if (decimal_places > 6) decimal_places = 6;

    char fmt[8];
    snprintf(fmt, sizeof(fmt), "%%.%df%%s", decimal_places);
    snprintf(buffer, sizeof(buffer), fmt, value, units ? units : "");
    return buffer;
}