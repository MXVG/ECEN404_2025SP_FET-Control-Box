/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize"  function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


#include <string.h>
#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
//#include "Encoder.c"                    // enccoder signal --> frequency and duty cycle
//#include "PWM_Capture.c"                // External signal --> frequency and duty cycle
//#include "Display.c"                    // controling the display

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************


// Buffers and variables
static char uartRxBuffer[UART_BUFFER_SIZE];   // Circular buffer for incoming data
static char commandQueue[COMMAND_QUEUE_SIZE][UART_BUFFER_SIZE]; // Queue for completed commands
static size_t bufferIndex = 0;
static size_t queueHead = 0; // Queue head for adding commands
static size_t queueTail = 0; // Queue tail for processing commands
//base values defined
double frequency = 10000; //store pwm frequency value
float dutyC = 0.5;//store pwm duty cycle value
int opmode = 1;//store pwm operational mode value
int external1 = 0;//variable to store if external1 signal is being used

// Function prototypes
void ProcessCommand(const char *data);
void SplitCommand(const char *data, char *command, char *number);
bool IsQueueEmpty(void);
bool IsQueueFull(void);
void AddToQueue(const char *command);
void ProcessQueue(void);


void setduty(float duty){//command to set duty cycle
    dutyC = duty;
}
void setfreq(double freq){//command to set frequency
    frequency = freq;
}
void setmode(int mode){ //command to set the operation mode
    opmode = mode;
}

void matching (double freq, float duty){
    // Calculate timer period for each PWM signal
    double period = (CPU_CLOCK_FREQUENCY/(16*freq)) - 1;  
    
    // Configure OC1 for PWM mode
    OC1CON = 0;                // Turn off OC1 while configuring
    OC1R = period * duty;        // Set duty cycle for PWM1
    OC1RS = (period * duty);       // Set duty cycle register for PWM1
    OC1CONbits.OCM = 0b110;   // PWM mode, fault pin disabled
    OC1CONbits.ON = 1;         // Turn on OC1
    
    OC2CON = 0;
    OC2CONbits.OCTSEL = 0;
    OC2CONbits.OCM = 0b110; // PWM mode (no fault pin)
    OC2RS = period * duty;        // Set PWM duty cycle (50% as an example)
    OC2R = (period*duty);     // Set phase for OC2 to be 0 (this can be adjusted for 180-degree phase shift)
    OC2CONbits.ON = 1;      // Enable OC2

    // Configure Timer2 for PWM frequency
    T2CON = 0;                       // Stop the timer and clear the control register
    T2CONbits.TCKPS = 0b011;   // Set prescaler to 1:256
    PR2 = (CPU_CLOCK_FREQUENCY / (16*freq)) - 1;     // Base period for Timer 2
    TMR2 = 0;                           // Clear the timer register
    TMR2_Start();                 // Start Timer2
   
}


void alternating (double freq, float duty){
    // Calculate timer period for each PWM signal
    double period = (CPU_CLOCK_FREQUENCY/(16*freq)) - 1;
    
    // Configure Timer2 (Single Timer for Both PWMs)
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;  // Prescaler 1:8
    PR2 = period;  // Set Timer2 period
    TMR2 = 0;  // Reset Timer2
    
    T3CON = 0;
    T3CONbits.TCKPS = 0b011;  // Prescaler 1:8
    PR3 = period;  // Set Timer3 period
    TMR3 = period*(1-duty);  // Reset Timer3
    
    TMR2_Start();  // Start Timer2
    TMR3_Start(); // start timer3

    // Configure OC1 (First PWM Signal)
    OC1CON = 0;
    OC1R = 0;  // Start at the beginning of the cycle
    OC1RS = (period * duty);  // Set duty cycle
    OC1CONbits.OCM = 0b110;  // Edge-aligned PWM mode
    OC1CONbits.ON = 1;

    // Configure OC2 (Second PWM Signal with 180° Phase Shift)
    OC2CON = 0;
    OC2R = 0;  
    OC2RS = ((period)*(1-duty));  // Maintain same duty\ cycle but shifted
    OC2CONbits.OCM = 0b110;  // Edge-aligned PWM mode
    OC2CONbits.OCTSEL = 1;//select timer 3
    OC2CONbits.ON = 1;  
     

}

void complementary (double freq, float duty){
    // Calculate timer period for each PWM signal
    double period = (CPU_CLOCK_FREQUENCY/(16*freq)) - 1;
    
    // Configure Timer2 (Single Timer for Both PWMs)
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;  // Prescaler 1:8
    PR2 = period;  // Set Timer2 period
    TMR2 = 0;  // Reset Timer2
    
    T3CON = 0;
    T3CONbits.TCKPS = 0b011;  // Prescaler 1:8
    PR3 = period;  // Set Timer3 period
    TMR3 = (period*(1-duty))+5;  // Reset Timer3
    
    TMR2_Start();  // Start Timer2
    TMR3_Start(); // start timer3

    // Configure OC1 (First PWM Signal)
    OC1CON = 0;
    OC1R = 0;  // Start at the beginning of the cycle
    OC1RS = (period * duty);  // Set duty cycle
    OC1CONbits.OCM = 0b110;  // Edge-aligned PWM mode
    OC1CONbits.ON = 1;

    // Configure OC2 (Second PWM Signal with 180° Phase Shift)
    OC2CON = 0;
    OC2R = 0;  
    OC2RS = ((period)*(1-duty));  // Maintain same duty\ cycle but shifted
    OC2CONbits.OCM = 0b110;  // Edge-aligned PWM mode
    OC2CONbits.OCTSEL = 1;//select timer 3
    OC2CONbits.ON = 1;  
   
}


void PWMGEN(){
    //set pwm to desired frequency, duty ratio, and operation mode
    switch (opmode) {
        case 1: /// matching mode
            matching(frequency, dutyC);
            break;
        case 2: // alternating mode
            alternating(frequency, dutyC);
            break;
        case 3: /// complementary mode
            complementary(frequency, dutyC);
            break;
        default:
            //matching(frequency, dutyC);
            break;
    }    
}


void commands(char com[UART_BUFFER_SIZE], double numbers){ // executed the commands from computer
   
    if (strcmp(com, "mode")==0){//checks for if a command was called, and executes the changes
        setmode((int)numbers);// calls function to set the new mode
    }
    if ((strstr(com, "external") == 0)){//using external signal
        external1 = 1;// changes the global variable external to 1
    }
    if ((strstr(com, "freq") == 0)){//frequency command
        setfreq(numbers);//calls function to set frequency
    }
    if ((strstr(com, "duty") == 0)){//seting duty cycle
        setduty((float)numbers);//calls function to set duty cycle
    }
}


void UART_Task(void)
{
    char receivedChar;//recieved character buffer
    while (UART6_Read(&receivedChar, 1)) // Check if data is available
    {
        if (receivedChar == '*') // End of a command
        {
            uartRxBuffer[bufferIndex] = '\0'; // Null-terminate the string
            if (!IsQueueFull())              // Add to the queue if space is available
            {
                AddToQueue(uartRxBuffer);
            }
            bufferIndex = 0; // Reset buffer for next command
        }
        else
        {
            // Add character to buffer if space allows
            if (bufferIndex < UART_BUFFER_SIZE - 1)
            {
                uartRxBuffer[bufferIndex++] = receivedChar;
            }
        }
    }

    ProcessQueue(); // Process completed commands in the queue
}

// Check if the command queue is empty
bool IsQueueEmpty(void)
{
    return queueHead == queueTail;
}

// Check if the command queue is full
bool IsQueueFull(void)
{
    return ((queueHead + 1) % COMMAND_QUEUE_SIZE) == queueTail;
}

// Add a command to the queue
void AddToQueue(const char *command)
{
    strcpy(commandQueue[queueHead], command);
    queueHead = (queueHead + 1) % COMMAND_QUEUE_SIZE;
}

// Process commands in the queue
void ProcessQueue(void)
{
    while (!IsQueueEmpty())
    {
        ProcessCommand(commandQueue[queueTail]);
        queueTail = (queueTail + 1) % COMMAND_QUEUE_SIZE;
    }
}

// Process a single command
void ProcessCommand(const char *data)
{
    char command[UART_BUFFER_SIZE];//holds command
    char number[UART_BUFFER_SIZE];//holds number
    double numValue;// holds number as a double
    //UART6_Write(" 1 ", sizeof(" 1 "));
   
    memset(command, 0, sizeof(command)); //makes sure command is clear
    memset(number, 0, sizeof(number));//makes sure number is clear
   
    SplitCommand(data, command, number); // call to split the command into number and command parts
    //UART6_Write("2 ", sizeof("2 "));
   
    numValue = strtod(number, NULL); //takes the number char string and makes it a double
    sprintf(number, "%f", numValue); // takes the number and puts into into number as a string (mainly this is for writing purposes)
    numValue = strtod(number, NULL);
    
    //execute the command
    //commands(command, numValue);
    while (UART6_WriteIsBusy());
    //UART6_Write(command, sizeof(command));
    //while (UART6_WriteIsBusy());
    //UART6_Write(" at ", sizeof(" at "));
    
    if (strstr(command, "mode")){//checks for if a command was called, and executes the changes
        setmode((int)numValue);// calls function to set the new mode
        UART6_Write("33 ", sizeof("33 "));
    }
    if (strstr(command, "external")){//using external signal
        external1 = 1;// changes the global variable external to 1
        //UART6_Write("44 ", sizeof("44 "));
    }
    if (strstr(command, "freq")){//frequency command
        setfreq(numValue);//calls function to set frequency
        //UART6_Write("55 ", sizeof("55 "));
    }
    if (strstr(command, "duty")){//seting duty cycle
        setduty((float)numValue);//calls function to set duty 
        //UART6_Write("66 ", sizeof("66 "));
    }
}

// Split the input data into command and number
void SplitCommand(const char *data, char *command, char *number)
{
    const char *delimiter = ":"; // : is what i seperate the command by
   
    memset(command, 0, UART_BUFFER_SIZE); //make sure command and number are clear (nothing in them))
    memset(number, 0, UART_BUFFER_SIZE);
   
    char *token = strtok((char *)data, delimiter); // check if : is in the string

    if (token != NULL) // if there is : in the string
    {
        strcpy(command, token); // First part is the command

        token = strtok(NULL, delimiter); //take second part of command
        if (token != NULL)
        {
            strcpy(number, token); // Second part is the number
        }
    }
}


int main(void)
{
    // Initialize the Harmony framework
    SYS_Initialize(NULL);
    // variables used so i dont do pwmgeneration when not needed
    double F = 0; //temp frequency variable
    float DC = 0; //temp duty cycle variable
    int M = 0; //temp mode variable
    //check to see if connection works, and usb sends data
    //UART6_Write("hi ", sizeof("hi "));

    while (true)
    {
       //Max's code initialization?
        //initializeDisplay();
        //initializeEncoders();
        //initializePWMCapture(void)
        
        //Update frequency and duty cycle from Max's code
        
        // the encoders update frequency and duty cycle when there is any change to the encoder signal
        //encoderPoll(status, context);
        
        //external signal updates if external=1 is input,
        /*
        if (externall){
            //capturePWM(context);
        }
         */

        // Communication with the computer
        //while (UART6_WriteIsBusy());
        //UART6_Write("hi ", sizeof("hi "));
        UART_Task();
       
       
        if ((F != frequency) || (DC != dutyC) || (M != opmode)){
            PWMGEN(); // this function is called to generate the pwm  
        }

       
        // set F to frequency
        F = frequency;
        // set DC to dutyC
        DC = dutyC;
        // set M to opmode
        M = opmode;
       
       
        SYS_Tasks ();

    }

    return (EXIT_FAILURE);
}


/*******************************************************************************
 End of File
*/


 

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

