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
#include "CharDisplay.h"

#define TUNER_NOTE_ROW 0
#define TUNER_NOTE_POSITION 2

#define TUNER_KEY_ROW 0
#define TUNER_KEY_POSITION 14

#define TUNER_CENTS_ROW 0
#define TUNER_CENTS_POSITION 6

#define TUNER_BPM_ROW 0
#define TUNER_BPM_POSITION 9

#define TUNER_MET_STATUS_ROW 1
#define TUNER_MET_STATUS_POSITION 11

#define TUNER_NEEDLE_ROW 1
#define TUNER_NEEDLE_MIN_POSITION 1
#define TUNER_NEEDLE_MAX_POSITION 14

#define TUNER_NOTE_STR_LEN 2
#define TUNER_KEY_STR_LEN 2
#define TUNER_CENTS_STR_LEN 3
#define TUNER_BPM_STR_LEN 3
#define TUNER_INTUNE_NEEDLE_STR_LEN 2

#define TUNER_MET_ON 1
#define TUNER_MET_OFF 0

#define TUNER_CHAR_PER_ROW 16
#define TUNER_NUM_ROWS 2
#define TUNER_NUM_CHAR_IN_DISPLAY TUNER_CHAR_PER_ROW * TUNER_NUM_ROWS

#define TUNER_CENTS_PER_POSITION 5
#define TUNER_HALF_NEEDLE_RANGE 7

#define TUNER_CENTER_OF_DISPLAY_POSITION TUNER_CHAR_PER_ROW/2

#define TUNER_NORMAL_NEEDLE 0x7C

#define FALSE 0

void InitTunerDisplay(CharDisplay *display);
void DisplayNote(uint8_t *note);
void DisplayKey(uint8_t *key);
void DisplayCentsOffset(int8_t centsOff);
void DisplayBPM(uint8_t bpm);
void DisplayMetronomeStatus(uint8_t on);
void DisplayNeedle(int8_t centsOff);
void SetDisplayModeTuner();
void SetDisplayModePitchGen();
void SetDisplayModeMetronome();