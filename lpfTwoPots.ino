/*  Example of filtering a wave,
    using Mozzi sonification library.

    Demonstrates LowPassFilter().

    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or
    check the README or http://sensorium.github.com/Mozzi/

		Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2012, CC by-nc-sa.
*/

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/chum9_int8.h> // recorded audio wavetable
#include <tables/cos2048_int8.h> // for filter modulation
#include <LowPassFilter.h>
#include <mozzi_rand.h>

Oscil<CHUM9_NUM_CELLS, AUDIO_RATE> aCrunchySound(CHUM9_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);

LowPassFilter lpf;

//*******CUT-OFF POT***********
int cutoffPot = A2;    // cut-off pot will be on A2
int cutVal = 0; //a value to store cutoff freq amount

//*******CUT-OFF POT***********
int resPot = A3;    // resonance pot will be on A3
int resVal = 0; //a value to store resonance amount value

void setup(){
  startMozzi();
  aCrunchySound.setFreq(2.f);
  kFilterMod.setFreq(1.3f);
//  lpf.setResonance(200);
}

void loop(){
  audioHook();
}

void updateControl(){

//**************CUT-OFF POT****************
    cutVal = mozziAnalogRead(cutoffPot);  // arduino checks pot value
    cutVal = map(cutVal, 0, 1023, 0, 255); // arduino changes value to max of 255 (lower value if distorting)
    byte cutoff_freq = cutVal;  // Set the cuttoff frequency to that of the pot 
    lpf.setCutoffFreq(cutoff_freq);  // change the freq
    //Change 255 value if distorting too much//

    //**************RESONANCE POT****************
    resVal = mozziAnalogRead(resPot);  // arduino checks pot value
    resVal = map(resVal, 0, 1023, 0, 255); // arduino changes value to max of 255 (lower value if distorting)
    byte res_freq = cutVal;  // Set the cuttoff frequency to equal that of the pot 
    lpf.setResonance(res_freq);  // change the freq
    //Change 255 value if distorting too much//

}

int updateAudio(){
  char asig = lpf.next(aCrunchySound.next())>>2;
  return (int) asig;
}