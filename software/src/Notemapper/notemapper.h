#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t ClosestFreq(float freq);
uint8_t *FreqToNote(float freq, uint8_t keyOffset);
float CentsOff(float correctFreq, float actualFreq);
float IndexToFreq(uint8_t index);

