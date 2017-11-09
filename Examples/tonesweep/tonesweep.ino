/*
TONESWEEP example

Simple frequency sweep. Try adjusting the duration and gain

*/

#include <waves.h>
#include <Weftlib.h>
Weft weft;

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthToneSweep      tonesweep1;     //xy=350,239
AudioOutputAnalog        dac1;           //xy=661,239
AudioConnection          patchCord1(tonesweep1, dac1);
// GUItool: end automatically generated code

float amp = 0.8; // play at 80% volume
int lo = 10;  // min freq of 10Hz
int hi = 600; //max freq of 1kHz
int duration = 10; // note this is in seconds, not ms
byte analogGain = 0x07; // can be from 0x04 to 0x07

void setup() {
  AudioMemory(64);
  weft.begin();
  weft.setToAnalogInputGain(analogGain);
}

void loop() {
    if(tonesweep1.isPlaying()){
     delay(1); 
    } else {
      tonesweep1.play(amp, lo, hi, duration);
    }
}