/*
 *  AVR Character Frequency Measuring Library
 *
 *  Description:    This files contains functions for measuring
 *                  frequencies from input on an analog comparator.
 *
 *  Table of Contents:
 *
 *  Special Notes:  Only works for character displays in which the number of rows is a power of 2.
 *
 *  Revision History:
 *     11/30/2012   Steven Okai     Initial revision.
 *
 *
 *
 */

#include <avr/io.h>

typedef struct {

    // Timer
    uint16_t *timer;
    
    // Timer frequency
    float timerFrequency;
    
    // Analog comparator
    uint8_t *analogCmp;
    
    // Period length
    uint16_t period; // Number of timer ticks in period
    
    // Edge count
    uint8_t edges;
    
} FreqMeasurer;

// Event handler to sample frequency of input from comparator
void SampleFreq(FreqMeasurer *measurer);
float GetFrequency(FreqMeasurer *measurer);
void InitFreqMeasurer(FreqMeasurer *measurer, uint8_t *timer, uint8_t *analogCmp, float timerFreq);