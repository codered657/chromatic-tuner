#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "notemapper.h"

#define NUM_OF_NOTES 88

// Starting from C8 down to A0
float noteFrequencies[88] = {4186.01, 3951.07, 3729.31, 3520.00, 3322.44, 3135.96, 2959.96, 2793.83, 2637.02, 2489.02, 2349.32, 2217.46, 2093.00, 1975.53, 1864.66, 1760.00, 1661.22, 1567.98, 1479.98, 1396.91, 1318.51, 1244.51, 1174.66, 1108.73, 1046.50, 987.767, 932.328, 880.000, 830.609, 783.991, 739.989, 698.456, 659.255, 622.254, 587.330, 554.365, 523.251, 493.883, 466.164, 440.000, 415.305, 391.995, 369.994, 349.228, 329.628, 311.127, 293.665, 277.183, 261.626, 246.942, 233.082, 220.000,207.652, 195.998, 184.997, 174.614, 164.814,155.563, 146.832, 138.591, 130.813, 123.471, 116.541, 110.000, 103.826, 97.9989, 92.4986, 87.3071, 82.4069, 77.7817, 73.4162, 69.2957, 65.4064, 61.7354, 58.2705, 55.0000, 51.9131, 48.9994, 46.2493, 43.6535, 41.2034, 38.8909, 36.7081, 34.6478, 32.7032, 30.8677, 29.1352, 27.5000};
    
// Array of note strings
uint8_t noteStrs[12][8] = {"C   ", "B   ", "A#/Bb", "A   ", "G#/Ab", "G   ", "F#/Gb", "F   ", "E    ", "D#/Eb", "D   ", "C#/Db"};

/* This function returns the index of the closest frequency */
uint8_t ClosestFreq(float freq) {

    uint8_t i = 0;
    float closestFreq = 0.0;
    float smallestDiff = 20000.0;
    float newDiff;
    
    newDiff = fabsf(freq - noteFrequencies[0]);
    closestFreq = noteFrequencies[0];
    
    // While the new frequency difference is less than the smallest, keep searching
    while(newDiff < smallestDiff && i < NUM_OF_NOTES) {
    
        i++;
        
        // Record new smallest difference
        smallestDiff = newDiff;
        
        // Record new closest frequency
        closestFreq = noteFrequencies[i];
        
        // Get next difference
        newDiff = fabsf(freq - noteFrequencies[i]);

    }

    //printf("Cents off: %f\n", CentsOff(noteFrequencies[i - 1], freq));
    return ((i - 1) % 12);

}

/* This function takes a frequency, and returns the nearest musical note as a
 * pointer to a string for the note.
 */
uint8_t *FreqToNote(float freq, uint8_t keyOffset) {
    
    uint8_t noteIndex;
    uint8_t *noteBaseAddr;
    
    noteBaseAddr = &noteStrs;
    // Get note index of closest frequency
    noteIndex = (ClosestFreq(freq) + keyOffset) % 12;
    
    // Print note
    //printf("The note is: %s\n", noteStrs[noteIndex]);
    
    return (noteBaseAddr + (noteIndex << 3));
    
}

float CentsOff(float correctFreq, float actualFreq) {
    double ratio = (double) (actualFreq/correctFreq);
    double base = 2.0;
    return (float) (1200.0 * (log(ratio)/log(base)));
}

float IndexToFreq(uint8_t index) {
    
    return noteFrequencies[index];
}
/*
int main(int argc, char **argv) {
    int i, res;
    float freq;

    if (argc != 2) {
        printf("usage:  %s N1\n\n", argv[0]);
        printf("\tPrints the closest note to the frequency N1\n\n");
        return 1;
    }

    // Get frequency
    res = sscanf(argv[1], "%f", &freq);
    
    printf("Input frequency is: %f\n", freq);
    freqToNote(freq);
    
    return 0;

}
*/