/*
 * Tuner.c
 *
 * Created: 11/16/2012 4:07:44 PM
 *  Author: sokai_000
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Timers.h"
//#include "CharDisplay.h"
#include "CCP.h"
#include "Timers.h"
#include "MAX6818.h"
#include "FreqMeasure.h"
#include "USART.h"
#include "GenIO.h"
#include "GenInt.h"
#include "notemapper.h"
#include "TunerDisplay.h"

#define USART_PORTF_1 0x0BB0

#define IO_PORTF 0x06A0

#define F_CPU 32000000UL  // Specifies a system clock of 32MHz

#define TUNER_KEY_CHANGE_KEY 0x08;
#define TUNER_KEY_CHANGE_NOTE 0x04;
#define TUNER_NUM_KEYS 4
#define TUNER_NUM_NOTES 13

#define TUNER_TUNER_MODE 0
#define TUNER_NOTE_GEN_MODE 1

Keypad keypad;
FreqMeasurer freqMeter;
uint8_t freqPin;

uint8_t testStr[32] = "0123456789ABCDEF0123456789ABCDEF";
uint8_t noteGenStrs[13][8] = {"C   ", "B   ", "A#/Bb", "A   ", "G#/Ab", "G   ", "F#/Gb", "F   ", "E    ", "D#/Eb", "D   ", "C#/Db" ,"C   "};
//uint16_t noteCCAs[13] = {238, 225, 212, 200, 189, 178, 168, 158, 150, 141, 133, 126, 118};
uint16_t noteCCAs[13] = {119, 113, 106, 100, 95, 89, 84, 79, 75, 71, 67, 63, 59};
uint8_t keyOffsets[4] = {0, 5, 10, 3};

ISR(ACA_AC0_vect) {

/*
      
    // Toggle the pin on an interrupt
    if (freqPin == 1) {
        
        SetPort(0x0700, 0x00);
        freqPin = 0;
        
    } else {
      
        SetPort(0x0700, 0xFF);
        freqPin = 1;  
    }
    
    */
    
    
    SampleFreq(&freqMeter);

    
    
}

ISR(PORTQ_INT0_vect) {

    // Read the keypad on an interrupt
    ReadKeypad(&keypad);
    
}

int main(void)
{
    
    CharDisplay display;
    CharDisplay *displayPtr;
    uint8_t *keysInPort;
    uint8_t *keyIntPort;
    uint8_t *keyAckPort;
    uint8_t *displayUSART;
    uint8_t *displayPort;
    uint8_t *freqGenTimer;
    uint8_t *freqGenOutputPort;
    uint8_t *freqMeterTimer;
    uint8_t *freqMeterAnalogCmp;
    
    uint8_t *debugPort;
    float freqMeterTimerFreq;
    float centsOff;
    
    float closestFreq;
    float freq;
    uint8_t *closestNote;
    uint8_t testCount;
    uint16_t test16;
    
    uint8_t *test;
    
    uint8_t currentPress;
    uint8_t currentMode;
    uint8_t currentNote;
    uint8_t currentKey;
    
    uint8_t *OSCCTRL;
    uint8_t *SCLKSEL;
    uint8_t *OSCSTATUS;
    uint8_t OSC_RDY;
    uint8_t *CPU_CCP_reg;
    uint8_t *DFLL_CTRL;
    uint8_t temp;
    char temp1;
    
    uint16_t sampleCount;
    uint16_t samplePrescale;
    
    OSCCTRL = 0x50;
    *OSCCTRL = 0x07;
    
    OSCSTATUS = 0x51;
    OSC_RDY = *OSCSTATUS & 0x07;
    while (OSC_RDY != 0x07) {
        OSC_RDY = *OSCSTATUS & 0x07;
    }
    
    // Calibrate the 32 MHz Osscilator 
    DFLL_CTRL = 0x0060;
    *DFLL_CTRL = 0x01;
    
    // Set to system clock to 32 MHz internal oscillator
    SetClk();
    
    displayUSART = USART_PORTF_1;
    displayPort = IO_PORTF;
    
    // Initialization code
    
    // Initialize character display
    InitCharDisplay(&display, displayUSART, displayPort);
    InitTunerDisplay(&display);
    
    
    // Initialize frequency generator
    freqGenTimer = 0x0A00; // Port C timer 0
    freqGenOutputPort = 0x0680;
    //FreqGenMode(freqGenTimer, freqGenOutputPort, TIMER_PRESCALE_256, 141);
    
    // Initialize the frequency measurer
    freqMeterTimer = 0x0940; // Port D timer 1
    freqMeterAnalogCmp = 0x0380; // Analog Cmp on port A
    freqMeterTimerFreq = 31250.0f;
    InitFreqMeasurer(&freqMeter, freqMeterTimer, freqMeterAnalogCmp, freqMeterTimerFreq);
    
    // Initialize the keypad
    keysInPort = 0x0720;
    keyIntPort = 0x07C0;
    keyAckPort = 0x07E0;
    InitKeypad(&keypad, keysInPort, keyIntPort, 2, keyAckPort, 0);
    
    // Initialize port J for debugging keypad
    debugPort = 0x0700;
    SetPortDirection(debugPort, IO_DIR_OUTPUT);
    
    // Initialization done, enable interrupts
    EnableIntLevel(INT_LEVEL_HIGH);
    EnableIntLevel(INT_LEVEL_MED);
    EnableIntLevel(INT_LEVEL_LOW);
    
    freqPin = 0;
    
    // Set the current mode
    currentMode = TUNER_TUNER_MODE;
    currentNote = 0;
    currentKey = 0;
    
    // Enable interrupts
    sei();
    
    // Write test character to display
    //ClearDisplay(&display);
    //SetCursorPosition(&display, 0, 0);
    //DisplayWriteChar(&display, "A");
    
    //_delay_ms(1000);
    TxUSARTByte(displayUSART, 0x12);
    
    _delay_ms(1000);
    TxUSARTByte(displayUSART, 124);
    TxUSARTByte(displayUSART, 157);
    
    //TxUSARTByte(displayUSART, 254);
    //TxUSARTByte(displayUSART, 0x01);
    //BoxCursorOn(&display);
    UnderlineCursorOn(&display);
    //CursorRight(&display);
    TxUSARTByte(displayUSART, 254);
    TxUSARTByte(displayUSART, 129);
    
    _delay_ms(1000);
    TxUSARTByte(displayUSART, 'A');
    TxUSARTByte(displayUSART, 'b');
    DisplayWriteChar(&display, 'A');
    
    _delay_ms(1000);
    
    SetDisplayModeTuner();
    DisplayNote(testStr);
    DisplayKey(testStr);
    DisplayCentsOffset(20);
    DisplayNeedle(20);
    //DisplayWriteStrToPosition(&display, testStr, 32, 0, 0);
    //DisplayWriteCharToPosition(&display, 'F', 1, 6);
    //TxUSARTByte(displayUSART, 254);
    //TxUSARTByte(displayUSART, 0x85);
    //DisplayWriteChar(&display, 'F');
    
    sampleCount = 0;
    samplePrescale = 0;
    
    
    while(1)
    {
        
        //TODO:: Please write your application code
        if (KeyAvailable(&keypad)) {
            
            // If there is a key, just output it on the LEDs
            currentPress = GetKey(&keypad);
            
            SetPort(0x0700, currentPress);
            
            switch (currentPress) {
                
                case 0xFB:
                    // Update the current key index
                    currentKey = (currentKey + 1) % TUNER_NUM_KEYS;
                    break;
                    
                case 0xF7:
                    if (currentMode == TUNER_TUNER_MODE) {
                        
                        // Change mode to note/freq generation mode
                        currentMode = TUNER_NOTE_GEN_MODE;
                        
                        // TODO: Enable frequency generation with to correct note
                        FreqGenMode(freqGenTimer, freqGenOutputPort, TIMER_PRESCALE_256, noteCCAs[currentNote]);
                        
                        currentNote++;
                        
                    } else if (currentNote == TUNER_NUM_NOTES - 1) {
                        
                        // Reset to first note
                        currentNote = 0;
                        
                        // Go back to tuner mode
                        currentMode = TUNER_TUNER_MODE;
                        
                        // TODO: Disable frequency generation
                        SetPinDirection(freqGenOutputPort, 0, IO_DIR_INPUT);
                        
                    } else {
                        
                        // TODO: Update with correct note
                        FreqGenMode(freqGenTimer, freqGenOutputPort, TIMER_PRESCALE_256, noteCCAs[currentNote]);
                        
                        // Go to next note
                        currentNote++;   
                        
                    }
                    
                    break;
                    
                default:
                    break;
                
            }
        }
        
        samplePrescale++;
        
        if (samplePrescale == 2) {
            
            sampleCount++;
            samplePrescale = 0;
        }
        
        if (sampleCount == 50000) {
        //SampleFreq(&freqMeter);
        //_delay_ms(10000000);
        testCount = (uint8_t) ((freqMeter.period) & 0xFF);
        test16 = (uint16_t)GetFrequency(&freqMeter);
        //SetPort(0x0700, (uint8_t) (test16 & 0xFF));
        //SetPort(0x0700, testCount);
        
        freq = GetFrequency(&freqMeter);
        closestFreq = ClosestFreq(freq);
        closestNote = FreqToNote(freq, keyOffsets[currentKey]);
        centsOff = CentsOff(IndexToFreq(closestFreq), freq);
        
        DisplayNote(closestNote);
        DisplayCentsOffset((uint8_t)centsOff);
        DisplayNeedle((uint8_t)centsOff);
        
        sampleCount = 0;
        }        
        
    }
    
    
    
}
