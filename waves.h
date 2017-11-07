
//****** SAMPLE WAVE FORMS *******

// WaveForm Array: [Amplitude, Freq, Cycles, Envelope]
// Amplitude    --  min:0=50v max: 255=100v
// Frequerncy   --  0-255 or 0x00-0xFF
// Duration     --  Cycles 0-255
// Envelope     --  (Ramp up + down)
// Max 60 waves per array !!

/*
   The envelope byte is divided into two nibbles. The upper nibble, bits [7:4], sets the ramp-up rate
  at the beginning of the synthesized sinusoid, and the lower nibble, bits [3:0], sets the ramp-down
  rate at the end of the synthesized sinusoid. The user must note that the ramp-up time is included
  in the duration parameter of the waveform, and the ramp-down time is appended to the duration
  parameter of the waveform. As such, if a ramp-up time is used, the ramp-up time must be less
  than the duration time as programmed in byte 3. Also note that the Total Ramp Time is for a
  ramp to full-scale amplitude (amplitude = 0xFF). Ramps to a fraction of full-scale have the same
  fraction of the Total Ramp Time.
*/

byte WaveForm_1[4][4] = {
  {128, 0x15, 50, 0x00},
  {64, 0xFF, 50, 0x00},
  {128, 0x19, 50, 0x00},
  {64, 0xF0, 50, 0x00}
};

byte WaveForm_2[6][4] = {
  {128, 0x15, 50, 0x09},
  {128, 0x17, 50, 0x09},
  {128, 0x19, 50, 0x09},
  {128, 0x1B, 50, 0x09},
  {128, 0x1D, 50, 0x09},
  {128, 0x1F, 50, 0x09}
};

byte WaveForm_3[2][4] = {
  {128, 0x15, 50, 0x09},
  {128, 0x17, 50, 0x09}
};

byte WaveForm_4[5][4] = {
  {128, 0x15, 50, 0x09},
  {200, 0x13, 50, 0x16},
  {150, 0x13, 50, 0x16},
  {100, 0x13, 50, 0x12},
  {50, 0x13, 50, 0x08}
};

byte WaveForm_5[59][4] = {
{255, 0x01, 100, 0x00},
{254, 0x05, 100, 0x00},
{252, 0x0a, 100, 0x00},
{249, 0x0e, 100, 0x00},
{244, 0x12, 100, 0x00},
{237, 0x17, 100, 0x00},
{230, 0x1b, 100, 0x00},
{221, 0x1f, 100, 0x00},
{211, 0x23, 100, 0x00},
{201, 0x28, 100, 0x00},
{189, 0x2c, 100, 0x00},
{177, 0x30, 100, 0x00},
{164, 0x35, 100, 0x00},
{151, 0x39, 100, 0x00},
{138, 0x3d, 100, 0x00},
{124, 0x42, 100, 0x00},
{111, 0x46, 100, 0x00},
{97, 0x4a, 100, 0x00},
{84, 0x4e, 100, 0x00},
{72, 0x53, 100, 0x00},
{60, 0x57, 100, 0x00},
{49, 0x5b, 100, 0x00},
{39, 0x60, 100, 0x00},
{29, 0x64, 100, 0x00},
{21, 0x68, 100, 0x00},
{14, 0x6d, 100, 0x00},
{9, 0x71, 100, 0x00},
{4, 0x75, 100, 0x00},
{2, 0x7a, 100, 0x00},
{0, 0x7e, 100, 0x00},
{0, 0x82, 100, 0x00},
{2, 0x86, 100, 0x00},
{4, 0x8b, 100, 0x00},
{9, 0x8f, 100, 0x00},
{14, 0x93, 100, 0x00},
{21, 0x98, 100, 0x00},
{29, 0x9c, 100, 0x00},
{39, 0xa0, 100, 0x00},
{49, 0xa5, 100, 0x00},
{60, 0xa9, 100, 0x00},
{72, 0xad, 100, 0x00},
{84, 0xb2, 100, 0x00},
{97, 0xb6, 100, 0x00},
{111, 0xba, 100, 0x00},
{124, 0xbe, 100, 0x00},
{138, 0xc3, 100, 0x00},
{151, 0xc7, 100, 0x00},
{164, 0xcb, 100, 0x00},
{177, 0xd0, 100, 0x00},
{189, 0xd4, 100, 0x00},
{201, 0xd8, 100, 0x00},
{211, 0xdd, 100, 0x00},
{221, 0xe1, 100, 0x00},
{230, 0xe5, 100, 0x00},
{237, 0xe9, 100, 0x00},
{244, 0xee, 100, 0x00},
{249, 0xf2, 100, 0x00},
{252, 0xf6, 100, 0x00},
{254, 0xfb, 100, 0x00}};

byte WaveForm_6[4][4] = {
  {255, 0x10, 255, 0x09},
  {255, 0x12, 255, 0x09},
  {255, 0x14, 255, 0x09},
  {255, 0xF0, 255, 0x09}
};


//****** END OF WAVEFORMS ******
