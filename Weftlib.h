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

#ifndef Weftlib_h
#define Weftlib_h


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h> //Wire Library to use I2C 

#define DRV2667_ADDR 0x59 //The DRV2667 Chip default I2C address.
#define REG_GAINS 0x01
#define REG_CTRL 0x02
#define REG_WAVEFORM0 0x03
#define REG_WAVEFORM1 0x04
#define REG_WAVEFORM2 0x05
#define REG_WAVEFORM3 0x06
#define REG_WAVEFORM4 0x07
#define REG_WAVEFORM5 0x08
#define REG_WAVEFORM6 0x09
#define REG_WAVEFORM7 0x0A
#define BOOST_AMP_ENABLED 0x02
#define RESET_DEVICE 0x80
#define STANDBY_ON 0x40
#define STANDBY_OFF 0x00

class Weft
{
  public:
    Weft(void);
    void begin(void);

    void playWave(byte WaveForm[][4], byte WavesNumber);

    void playWaveGain(byte WaveForm[][4], byte WavesNumber, byte theGain);
    float getWaveDuration(byte wf[][4], byte wn);
    void setToAnalogInput();
    void setToAnalogInputGain(byte theGain);

    /*
    The DRV2667 device has a timeout period after the FIFO has emptied. 
    This timeout period allows the user time to send a subsequent waveform before the device logic
    puts the device into idle mode, that then allows the host processor time to cue up an adjoining
    waveform from memory. After the timeout expires, the DRV2667 device must re-enter the 2 ms
    startup sequence before the next waveform plays. The timeout period is register- selectable
    to be 5, 10, 15 or 20 ms.
    */
    void setTimeoutPeriod(byte thePeriod); // can only be 0x00, 0x04, 0x08, or 0x0c. Defaults to shortest, 5ms -> 0x00
    // directPlayback()  
    /*
    from datasheet:
    The waveforms are stored as 8-bit twos-complement, Nyquist-rate data points, and are played from RAM at an 8- kHz data rate.
    Up to 250 ms of total waveform playback time may be stored in the Direct Playback From RAM mode format in the 2-kB memory.
    The waveform sizes are completely customizable, so many small waveforms may be stored or fewer long ones.
    The sum of the waveform lengths must not be greater than the 2-kB RAM size.
    */

    // patternWrite(data)  // write a waveform to the internal RAM, can't be more than 2kb total.  Successful write returns index / address?
    // patternRead(index)
    // This may need to return an index # if mult files are supported by this lib
    void resetDevice();
    void setToStandby();
    void wakeFromStandby();
    void writeRegisterBytes(byte reg, byte val);
    void i2c_Scan(void);
  
 

  // private:

};

#endif

