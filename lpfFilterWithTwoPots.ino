/*  
TWO POT VERSION
Example of filtering a wave,
    using Mozzi sonification library.

    Demonstrates StateVariable().

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
#include <tables/whitenoise8192_int8.h>
#include <tables/cos2048_int8.h> // for filter modulation
#include <StateVariable.h>
#include <mozzi_rand.h> // for rand()
#include <AutoMap.h> // To track pots **********************************************************



/*cutoff  range 0-255 represents 0-8191 Hz (AUDIO_RATE/2). Be careful of distortion at the lower end, especially with high resonance.
resonance range 0-255, 255 is most resonant.*/
//// Set up Freq Pot*************************************************************************
const int freqPot = A5;    // select the input pin for the potentiometer
AutoMap freqPotMap(0, 1023, 0, 1023);  // remap the atk pot to 4 seconds
//
//// Set up Res Pot*************************************************************************
const int resPot = A4;    // select the input pin for the potentiometer
AutoMap resPotMap(0, 1023, 2, 212);  // 0-255 val distorted, 40-215 within range



Oscil <WHITENOISE8192_NUM_CELLS, AUDIO_RATE> aNoise(WHITENOISE8192_DATA); // audio noise
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);

StateVariable <NOTCH> svf; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH


void setup(){
  startMozzi();
  // cast to float because the resulting freq will be small and fractional
  aNoise.setFreq((float)AUDIO_RATE/WHITENOISE8192_SAMPLERATE);
  kFilterMod.setFreq(1.3f);
//  svf.setResonance(100);
//  svf.setCentreFreq(1200);
}



void updateControl(){

  //// Set up Resonance/Freq Pot*************************************************************************
    int freqVal = mozziAnalogRead(freqPot);    // read the value from the attack pot
    freqVal = freqPotMap(freqVal);

    int resVal = mozziAnalogRead(resPot);    // read the value from the attack pot
    resVal = resPotMap(resVal);
    
  if (rand(CONTROL_RATE/2) == 0){ // about once every half second
    kFilterMod.setFreq(freqVal/64);  // choose a new modulation frequency****Replace setFreq((float)rand(255)/64)****
  }
  int cutoff_freq = 2200 + kFilterMod.next()*12; // 
  svf.setCentreFreq(cutoff_freq);
  svf.setResonance(resVal); 
}


int updateAudio(){
  // watch output levels, they can distort if too high
  // also, at very resonant settings, the input signal may need attenuating
  return svf.next(aNoise.next())>>3;
}


void loop(){
  audioHook();
}