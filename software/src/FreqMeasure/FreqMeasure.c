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

#include "FreqMeasure.h"
#include "Timers.h"
#include "AnalogCmp.h"
#include "GenIO.h"

#define TIMER_CNT_OFFSET 0x20

#define FREQ_MEASURE_NUM_SAMPLES 4;
    
// Event handler to sample frequency of input from comparator
void SampleFreq(FreqMeasurer *measurer) { // Note: argument must be hard-coded
    
    uint16_t *timerCountReg;
    
    if (measurer->edges == 7) {
        // Get the address of the timer count register
        timerCountReg = (measurer->timer) + TIMER_CNT_OFFSET/2;
    
        // Get the period of the signal
        measurer->period = *timerCountReg;
    
        // Reset the timer to find the period of the next signal
        *timerCountReg = 0;
    
        // Reset edge count
        measurer->edges = 0;
    } else {
        measurer->edges = measurer->edges + 1;
            
    }
    
    return;
    
}

float GetFrequency(FreqMeasurer *measurer) {
    
    float period;
    float frequency;
    
    // Get the period of the signal
    period = (float) (measurer->period);
    
    // Calculate the frequency
    frequency = ((measurer->timerFrequency)/period)*8;
    
    return frequency;
    
}

void InitFreqMeasurer(FreqMeasurer *measurer, uint8_t *timer, uint8_t *analogCmp, float timerFreq) {
    
    // TODO: Write code to set timer frequency later?
    
    // Set up reference timer
    SetNormalMode(timer, 0xFFFF, TIMER_PRESCALE_256, INT_LEVEL_DISABLED);
    measurer->timer = timer;
    
    // Set up analog comparator for small hysteresis, signal in on pin 0, threshold voltage on pin 1
    SetSignalCmp(analogCmp, 0, AC_HYSMODE_LARGE, AC_INTMODE_RISING, INT_LEVEL_HIGH, AC_POS_IN_PIN0, AC_NEG_IN_PIN1);
    //SetSignalCmp(analogCmp, 0, AC_HYSMODE_SMALL, AC_INTMODE_RISING, INT_LEVEL_HIGH, AC_POS_IN_PIN0, AC_NEG_IN_PIN1);
    measurer->analogCmp = analogCmp;
    
    measurer->timerFrequency = timerFreq;
    
    measurer->edges = 0;
    
    return;
    
}

void FreqMeasurerMode() {
    
    
    
    return;   
}
