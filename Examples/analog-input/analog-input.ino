/*
ANALOG INPUT EXAMPLE

Power up WEFT UI Board and press the encoder pushbutton to "sample" a value from Analog A9

This sketch is intentionally open-ended - have fun coming up with other mappings and behaviors!

*/




#include "waves.h"
#include <Weftlib.h>
#include <Wire.h>
#include <Encoder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <elapsedMillis.h>
#include <Audio.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


#define DISPLAY_PRESENT true

#define OLED_RESET 20 // not really used, but req by gfx lib
#define sw 8 // encoder's pushbutton switch
#define analogPin A9 // analog sensor connected here
Weft weft;
Encoder myEnc(5, 6); // encoder A and B
int wavelabel = 0;
float minFreq = 10;
float maxFreq = 400;
float ff = 100; // default frequency 100Hz
boolean triggered = false;
volatile boolean triggeredV = false;
char* wavelabels[] = {"SINE", "SQAR", "TRI", "NOIZ"};
byte analogGain = 0x07; // can be from 0x04 to 0x07

int reading = 0;
int oldReading = 0;

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     theNoise;         //xy=135,300
AudioSynthWaveform       theSine;      //xy=140,151
AudioSynthWaveform       theSquare;      //xy=142,200
AudioSynthWaveform       theTriangle;      //xy=142,248
AudioMixer4              mixer1;         //xy=391,265
AudioOutputAnalog        dac1;           //xy=675,246
AudioConnection          patchCord1(theNoise, 0, mixer1, 3);
AudioConnection          patchCord2(theSine, 0, mixer1, 0);
AudioConnection          patchCord3(theSquare, 0, mixer1, 1);
AudioConnection          patchCord4(theTriangle, 0, mixer1, 2);
AudioConnection          patchCord5(mixer1, dac1);
// GUItool: end automatically generated code

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // audio setup
  AudioMemory(48);
  mixer1.gain(0, 0.0); // sine
  mixer1.gain(1, 0.9); // square
  mixer1.gain(2, 0.0); // triangle
  mixer1.gain(3, 0.0); // noise
  theNoise.amplitude(0.8); //noise won't begin until a valid amp is specified
  theSine.begin(WAVEFORM_SINE);
  theSquare.begin(WAVEFORM_SQUARE);
  theTriangle.begin(WAVEFORM_TRIANGLE);
  theSine.begin(0.8, ff, WAVEFORM_SINE);
  theSquare.begin(0.8, ff, WAVEFORM_SQUARE);
  theTriangle.begin(0.8, ff, WAVEFORM_TRIANGLE);
  // ui setup
  pinMode(sw, INPUT_PULLUP); // encoder pushbutton
  pinMode(A14, OUTPUT); // DAC output
  attachInterrupt(sw, swTriggered, FALLING);
  analogWriteResolution(12);
  randomSeed(analogRead(0));
  // i2c peripherals setup
  weft.begin();
  weft.setToAnalogInputGain(analogGain);
  if (DISPLAY_PRESENT) {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    display.setRotation(2); // 2:rotates screen 180º, 3 is correct for WEFT UI Board
    display.setTextSize(1);
    display.setTextColor(WHITE);
  }
  pinMode(analogPin, INPUT_PULLUP);
}

void loop() {
  reading = analogRead(analogPin)*0.8 + oldReading*0.2;
  float scaled = constrain(floatmap(reading, 0, 4096, minFreq, maxFreq), minFreq, maxFreq);
  noInterrupts();
  triggered = triggeredV;
  interrupts();

  // only update display if something changed
  if (triggered) {
    triggeredV = false;
    // say "last frequency was: 80Hz" etc and randomly choose a new waveform
    ff = random(minFreq,maxFreq);
    if (DISPLAY_PRESENT) {
      updateDisplay();
    }
    updateFreq(ff);
    // switchTo(wavelabel);
  }
}

void swTriggered() {
  // interrupt handling pushbotton presses, incrementing wave type
  triggeredV = true;
}

void updateFreq(int newFreq) {
  theSine.frequency(newFreq);
  theSquare.frequency(newFreq);
  theTriangle.frequency(newFreq);
}

void switchTo(int wave) {
  // mute all other waveforms, unmute waveform we want
    for (int i = 0; i < 4; i++) {
      if (i == wave) {
        mixer1.gain(i, 0.9);
      } else {
        mixer1.gain(i, 0.0);
      }
    }
}

