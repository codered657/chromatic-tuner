/*
 *  Tuner Display Library
 *
 *  Description:    This files contains functions for manipulating the
 *                  character display for the tuner.
 *
 *  Table of Contents:
 *
 *  Revision History:
 *     12/03/2012   Steven Okai     Initial revision.
 *
 *
 *
 */

#include <avr/io.h>
#include <stdlib.h>
#include "TunerDisplay.h"

CharDisplay *tunerDisplay;

uint8_t tunerModeStr[TUNER_NUM_CHAR_IN_DISPLAY] =     "N:       ¢  K:  b              #";
uint8_t pitchGenModeStr[TUNER_NUM_CHAR_IN_DISPLAY] =  "N:          K:  b              #";
uint8_t metronomeModeStr[TUNER_NUM_CHAR_IN_DISPLAY] = "     BPM:       b              #";
uint8_t inTuneNeedleStr[TUNER_INTUNE_NEEDLE_STR_LEN] = ">><<";
uint8_t blankStr[16] = "                ";

void InitTunerDisplay(CharDisplay *display) {
    
    // Set tuner display
    
    // Initialize to tuner mode
    tunerDisplay = display;
    return;
}

void DisplayNote(uint8_t *note) {
    
    // Write note string to correct position in display
    DisplayWriteStrToPosition(tunerDisplay, note, TUNER_NOTE_STR_LEN, TUNER_NOTE_ROW, TUNER_NOTE_POSITION);
    
    return;
    
}

void DisplayKey(uint8_t *key) {
    
    // Write key string to correct position in display
    DisplayWriteStrToPosition(tunerDisplay, key, TUNER_KEY_STR_LEN, TUNER_KEY_ROW, TUNER_KEY_POSITION);
        
    return;
    
}

void DisplayCentsOffset(int8_t centsOff) {
    
    // TODO: pick number large enough to make sure no overwrites
    uint8_t centsStr[10];
    
    /*
    // Compose string for cents offset
    if (centsOff > 0) {
    // If cents offset is positive, write to string
        centsStr[0] = "+";
        
    } else if (centsOff < 0) {
    // If cents of negative, write sign to string
        centsStr[0] = "-";
         
    } else  {
        // Otherwise, write a blank
        centsStr[0] = " ";
        
    }
    */
    
    // Convert cents offset to string
    itoa(centsOff, centsStr, 10);
    
    // Write cents offset string to correct position in display
    DisplayWriteStrToPosition(tunerDisplay, centsStr, TUNER_CENTS_STR_LEN, TUNER_CENTS_ROW, TUNER_CENTS_POSITION);
        
    return;
    
}

void DisplayBPM(uint8_t bpm) {
    
    // TODO: pick number large enough to make sure no overwrites
    uint8_t bpmStr[10];
    
    itoa(bpm, bpmStr, 10);
    
    // Write bpm string to correct position in display
    DisplayWriteStrToPosition(tunerDisplay, bpmStr, TUNER_BPM_STR_LEN, TUNER_BPM_ROW, TUNER_BPM_POSITION);
        
    return;
}

void DisplayMetronomeStatus(uint8_t on) {
    
    uint8_t *onStr;
    
    if (on == FALSE) {
    // If not on, write off string    
        
    } else {
    // Otherwise write on string    
        
    }
    
    // Display the on/off string
    
    return;
}

void DisplayNeedle(int8_t centsOff) {
    
    int8_t needleOffset;
        
    // Clear the needle area
    DisplayWriteStrToPosition(tunerDisplay, blankStr, 14, 1, 1);
    
    // Find the needle position relative to center
    needleOffset = (centsOff/TUNER_CENTS_PER_POSITION);
        
    if (needleOffset == 0) {
    // If at center display, show in-tune needle instead of normal needle
        DisplayWriteStrToPosition(tunerDisplay, inTuneNeedleStr, TUNER_INTUNE_NEEDLE_STR_LEN, TUNER_NEEDLE_ROW, TUNER_CENTER_OF_DISPLAY_POSITION - TUNER_INTUNE_NEEDLE_STR_LEN/2);
            
    } else {
    // Calculate display position and display normal needle
        
        // Get position from left of display
        needleOffset += TUNER_CENTER_OF_DISPLAY_POSITION;
            
        if (needleOffset < TUNER_NEEDLE_MIN_POSITION) {
        // If needle position is past left bound of needle range, just leave at the far left side
            needleOffset = TUNER_NEEDLE_MIN_POSITION;
                
        } else if (needleOffset > TUNER_NEEDLE_MAX_POSITION) {
        // If need position is past right bound of needle range, just leave at far right side
            needleOffset = TUNER_NEEDLE_MAX_POSITION;
                
        }
            
        // Now display needle at correct place
        DisplayWriteCharToPosition(tunerDisplay, 'A', TUNER_NEEDLE_ROW, (uint8_t) needleOffset);
        //DisplayWriteCharToPosition(tunerDisplay, TUNER_NORMAL_NEEDLE, TUNER_NEEDLE_ROW, (uint8_t) needleOffset);
            
    }
            
    return;
    
}

void SetDisplayModeTuner() {
    
    // Send the base string for tuner mode to the display
    DisplayWriteStrToPosition(tunerDisplay, tunerModeStr, TUNER_NUM_CHAR_IN_DISPLAY, 0, 0);
    
    return;
    
}

void SetDisplayModePitchGen() {
    
    // Send the base string for pitch generator mode to the display
    DisplayWriteStrToPosition(tunerDisplay, pitchGenModeStr, TUNER_NUM_CHAR_IN_DISPLAY, 0, 0);
    
    return;
    
}

void SetDisplayModeMetronome() {
    
    // Send the base string for metronome mode mode to the display
    DisplayWriteStrToPosition(tunerDisplay, metronomeModeStr, TUNER_NUM_CHAR_IN_DISPLAY, 0, 0);
    
    return;
    
}