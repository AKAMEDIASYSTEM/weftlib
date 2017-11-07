/*
WEFTlib Electrovibration library
2017, nfeehan / AKA
Addition of some methods to make electrovibration actuation more convenient. Probably also useful for piezo effects?

Based gratefully off of:
  Yurikleb_DRV2667.h - Library for controling the TI - DRV2667 Hapic Piezo Driver
  Created by Yuri Klenaov, January, 2016. (yurikleb.com)
  Released into the public domain.
  For more information see the DRV2667 Datasheet http://www.ti.com/lit/ds/symlink/drv2667.pdf
*/


  /*
  there are three modes for this IC that result in output.
waveform synthesis playback:
waveform "Direct Playback": in this mode, the IC basically reads a soundfile you've stored in RAM (via the savePattern() call)
analog in: calling setToAnalogInput() or setToAnalogInputGain(byte gain)

  */

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h> //Wire Library to use I2C

#include "Weftlib.h"


Weft::Weft(){
}

void Weft::begin() {
  /*
  Initialization Procedure
1. Apply power to the DRV2667 device.
2. Wait for 1 ms for the DRV2667 device to power-up before attempting an I2C write.
3. Exit low-power standby mode by clearing the STANDBY bit in register 0x02, bit6.
4. Choose the interface modeas analog or digital in register 0x01, bit2.
5. Select the gain setting for your application in register 0x01, bits[1:0].
6. Choose the desired timeout period if using the digital interface mode (FIFO), in register 0x02, bits[3:2].
7. If using the digital interface mode, the device is now ready to receive data. 
   If using the analog input mode, set the EN_OVERRIDE bit in register 0x02, bit 1 to enable the boost 
   and high-voltage amplifier and begin sourcing the waveform to the analog input.
*/
  Wire.begin();
  delay(300);
}


//Send a Wave to the DRV2667
void Weft::playWave(byte WaveForm[][4], byte WavesNumber) {

  //control
  writeRegisterBytes(REG_CTRL, STANDBY_OFF); //Take device out of standby mode
  writeRegisterBytes(REG_GAINS, 0x03); //Set Gain 0-3 (0x00-0x03 25v-100v)
  writeRegisterBytes(0x03, 0x01); //Set sequencer to play WaveForm ID #1
  writeRegisterBytes(0x04, 0x00); //End of sequence
  //header
  /*
  The header block describes the location of the waveform data content.
  The structure of the header consists of 5- byte blocks containing the following information (see Figure 26):
• Start address, upper byte
• Start address, lower byte
• Stop address, upper byte
• Stop address, lower byte
• Repeat count ( A 0 in this byte is interpreted as an infinite loop and the waveform is played indefinitely until the GO bit is cleared by the user.)
*/
  writeRegisterBytes(0xFF, 0x01); //Set memory to page 1
  writeRegisterBytes(0x00, 0x05); //Header size –1
  writeRegisterBytes(0x01, 0x80); //Start address upper byte (page), also indicates Mode 3
  writeRegisterBytes(REG_CTRL, 0x06); //Start address lower byte (in page address)
  writeRegisterBytes(0x03, 0x00); //Stop address upper byte
  writeRegisterBytes(0x04, 0x06+WavesNumber-1); //Stop address Lower byte
  writeRegisterBytes(0x05, 0x01); //Repeat count, play WaveForm once
  
  //WaveForm Data From the array
  for(byte i = 0; i < WavesNumber; i++){
    writeRegisterBytes(0x06+i*4+0, WaveForm[i][0]); 
    writeRegisterBytes(0x06+i*4+1, WaveForm[i][1]); 
    writeRegisterBytes(0x06+i*4+2, WaveForm[i][2]); 
    writeRegisterBytes(0x06+i*4+3, WaveForm[i][3]);
  }
  
  //Control
  writeRegisterBytes(0xFF, 0x00); //Set page register to control space
  writeRegisterBytes(REG_CTRL, 0x01); //Set GO bit (execute WaveForm sequence)
  
  //delay( 1000 * (cycles / (7.8125 * frequency)) );

}

//Send a Wave to the DRV2667
void Weft::playWaveGain(byte WaveForm[][4], byte WavesNumber, byte theGain) {
  if(theGain>0x03){
    theGain=0x03;
  }
  //control
  writeRegisterBytes(REG_CTRL, STANDBY_OFF); //Take device out of standby mode
  writeRegisterBytes(REG_GAINS, theGain); //Set Gain 0-3 (0x00-0x03 25v-100v)
  writeRegisterBytes(0x03, 0x01); //Set sequencer to play WaveForm ID #1
  writeRegisterBytes(0x04, 0x00); //End of sequence
  //header
  writeRegisterBytes(0xFF, 0x01); //Set memory to page 1
  writeRegisterBytes(0x00, 0x05); //Header size –1
  writeRegisterBytes(0x01, 0x80); //Start address upper byte (page), also indicates Mode 3
  writeRegisterBytes(REG_CTRL, 0x06); //Start address lower byte (in page address)
  writeRegisterBytes(0x03, 0x00); //Stop address upper byte
  writeRegisterBytes(0x04, 0x06+WavesNumber-1); //Stop address Lower byte
  writeRegisterBytes(0x05, 0x01); //Repeat count, play WaveForm once
  
  //WaveForm Data From the array
  for(byte i = 0; i < WavesNumber; i++){
    writeRegisterBytes(0x06+i*4+0, WaveForm[i][0]); 
    writeRegisterBytes(0x06+i*4+1, WaveForm[i][1]); 
    writeRegisterBytes(0x06+i*4+2, WaveForm[i][2]); 
    writeRegisterBytes(0x06+i*4+3, WaveForm[i][3]);
  }
  
  //Control
  writeRegisterBytes(0xFF, 0x00); //Set page register to control space
  writeRegisterBytes(REG_CTRL, 0x01); //Set GO bit (execute WaveForm sequence)ss
  
  //delay( 1000 * (cycles / (7.8125 * frequency)) );
}

float Weft::getWaveDuration(byte wf[][4], byte wn){
  // TODO: test this to make sure it's correct!
  // return wave duration in ms
  float theCount = 0;
    //WaveForm Data From the array
  // ... the divide-by-4 might be wrong here
  for(int j = 0; j < wn/4; j++){
    // freq is in the 2nd byte
    // num cycles is in 3rd
    // duration in ms = 1000 * num
    Serial.println(wf[j][1]);
    if(wf[j][1]==0){
      return 0;
    } else {
      theCount += (1000.0 * float(wf[j][2])) / (7.8125 * float(wf[j][1]));
    }
  }
  return theCount;
}

void Weft::setToAnalogInput(){
  //control
  writeRegisterBytes(REG_CTRL, STANDBY_OFF); //Take device out of standby mode
  writeRegisterBytes(REG_GAINS, 0x07); //Set to analog input + Gain 0-3 (0x04-0x07 25v-100v)
  writeRegisterBytes(REG_CTRL, BOOST_AMP_ENABLED); //Set EN_OVERRIDE bit = boost and amplifier active
}

void Weft::setToAnalogInputGain(byte theGain){
  //control
  if(theGain>0x07){
    theGain=0x07;
  }
  if(theGain<0x04){
    theGain=0x04;
  }
  writeRegisterBytes(REG_CTRL, STANDBY_OFF); //Take device out of standby mode
  writeRegisterBytes(REG_GAINS, theGain); //Set to analog input + Gain 0-3 (0x04-0x07 25v-100v)
  writeRegisterBytes(REG_CTRL, BOOST_AMP_ENABLED); //Set EN_OVERRIDE bit = boost and amplifier active
}

void Weft::setTimeoutPeriod(byte thePeriod){
  // TODO: test this on scope!
  if(thePeriod==0x00 || thePeriod==0x04 || thePeriod==0x08 || thePeriod==0x0C){
  writeRegisterBytes(REG_CTRL, thePeriod);  
  }
  
  // 0x00: 5ms
  // 0x04: 10ms
  // 0x08: 15ms
  // 0x0C: 20ms

}

void Weft::resetDevice(){
  // TODO: test this on scope!
  writeRegisterBytes(REG_CTRL, RESET_DEVICE);
  // note this ALSO places IC into Standby Mode
}

void Weft::setToStandby(){
  // TODO: test this on scope!
  writeRegisterBytes(REG_CTRL, STANDBY_ON);
}

void Weft::wakeFromStandby(){
  // TODO: test this on scope!
  writeRegisterBytes(REG_CTRL, STANDBY_OFF);
}

//Write Bytes via I2c (Using Wire Library)
void Weft::writeRegisterBytes(byte reg, byte val) {
  Wire.beginTransmission(DRV2667_ADDR);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

void Weft::i2c_Scan(){
  byte count = 0;

  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      count++;
      delay (1);
    }
  }  
}