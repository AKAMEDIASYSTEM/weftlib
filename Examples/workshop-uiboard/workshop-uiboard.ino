#include "waves.h"
#include <Weftlib.h>
#include <Wire.h> //Include arduino Wire Library to enable to I2C
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
Weft weft;
Encoder myEnc(5, 6); // encoder A and B

float minFreq = 10;
float maxFreq = 400;
float ff = 10;
volatile int wavelabelV = 0;
int wavelabel = 0;
int wavelabelOld = 0;
int encUpLimit = 300;
int encLoLimit = 0;
long newPosition = 0;
long oldPosition  = -999;
char* wavelabels[] = {"SINE", "SQAR", "TRI", "NOIZ", "FILE"};
char* filenames[] = {"SINE", "SQAR", "TRI", "NOIZ", "FILE"};
elapsedMillis timeSincePlay;
unsigned long waveformLength = 620; //ms of waveform file to play, should eventually be dynamic via weft.getWaveDuration
boolean playingDigital = false;
boolean analogSet = false;
byte analogGain = 0x07; // can be from 0x04 to 0x07
byte digitalGain = 0x03; // can be from 0x00 to 0x03

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
  theSine.begin(0.8, minFreq * 2, WAVEFORM_SINE);
  theSquare.begin(0.8, minFreq * 2, WAVEFORM_SQUARE);
  theTriangle.begin(0.8, minFreq * 2, WAVEFORM_TRIANGLE);
  // ui setup
  pinMode(sw, INPUT_PULLUP); // encoder pushbutton
  pinMode(A14, OUTPUT); // DAC output
  attachInterrupt(sw, swTriggered, FALLING);
  analogWriteResolution(12);
  // i2c peripherals setup
  weft.begin();
  if (DISPLAY_PRESENT) {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    display.setRotation(2); // 2:rotates screen 180º, 3 is correct for WEFT UI Board
    display.setTextSize(1);
    display.setTextColor(WHITE);
  }
  Serial.begin(9600);
  Serial.println("hey now");
}

void loop() {

  readEnc();
  noInterrupts();
  wavelabel = wavelabelV;
  interrupts();

  // only update display if something changed
  if ((newPosition != oldPosition) || (wavelabel != wavelabelOld) ) {
    oldPosition = newPosition;
    wavelabelOld = wavelabel;
    if (DISPLAY_PRESENT) {
      updateDisplay(newPosition);
    }
    updateFreq(newPosition);
    switchTo(wavelabel);
  }

  if (playingDigital && (timeSincePlay > waveformLength)) {
    // retrigger the digital file if we are playing it
    weft.playWaveGain(WaveForm_6, sizeof(WaveForm_6), digitalGain); //Play one of the Waveforms defined in waves.h;
    timeSincePlay = 0;
    playingDigital = true;
    analogSet = false;
  }

}

void swTriggered() {
  // interrupt handling pushbotton presses, incrementing wave type
  wavelabelV = (wavelabelV + 1) % 5;
}

void readEnc() {
  newPosition = myEnc.read();
  if (newPosition > encUpLimit) {
    myEnc.write(encUpLimit);
  }
  if (newPosition < encLoLimit) {
    myEnc.write(encLoLimit);
  }
}

void updateFreq(int newPos) {
  ff = constrain(floatmap(newPos, encLoLimit, encUpLimit, minFreq, maxFreq), minFreq, maxFreq);
  //  Serial.println("changing freq to " + newPos);
  theSine.frequency(ff);
  theSquare.frequency(ff);
  theTriangle.frequency(ff);
}

void switchTo(int wave) {
  // mute all other waveforms, unmute waveform we want
  if (wave == 4) { // TODO, un-hardcode this and match on "FILE" (enum? string compare?)
    Serial.println("file time!");
    // mute all analog channels
    // TODO: not sure this is necessary? Testing needed.
    mixer1.gain(0, 0.0);
    mixer1.gain(1, 0.0);
    mixer1.gain(2, 0.0);
    mixer1.gain(3, 0.0);
    analogSet = false;
    if (!playingDigital) {
      // play the digital file
      waveformLength = round(weft.getWaveDuration(WaveForm_6, sizeof(WaveForm_6)));
      weft.playWaveGain(WaveForm_6, sizeof(WaveForm_6), digitalGain); //Play one of the Waveforms defined in waves.h;
      timeSincePlay = 0;
      playingDigital = true;
      analogSet = false;
      Serial.println(weft.getWaveDuration(WaveForm_6, sizeof(WaveForm_6)));
      Serial.println(waveformLength);
    } else {
      if (timeSincePlay > waveformLength) {
        playingDigital = false;
      }
    }
  } else {
    // if not in file-playback mode, set analog as needed
    playingDigital = false;
    if (!analogSet) {
      weft.setToAnalogInputGain(analogGain);
      analogSet = true;
    }
    for (int i = 0; i < 4; i++) {
      if (i == wave) {
        mixer1.gain(i, 0.9);
      } else {
        mixer1.gain(i, 0.0);
      }
    }

  }
}

