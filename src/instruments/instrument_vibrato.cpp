#include <iostream>
#include <math.h>
#include "instrument_vibrato.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentVibrato::InstrumentVibrato(const std::string &param) 
  : adsr(SamplingRate, param) 
{
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);

  int N1, N2, I;
  if(!kv.to_int("N1",N1))
    N1=1;
  if(!kv.to_int("N2",N2))
    N2=5;
  if(!kv.to_int("I",I))
    I=0.75;

}


void InstrumentVibrato::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
  
    float F0=440.0*pow(2,(((float)note-69.0)/12.0))/SamplingRate; // aillant fo de la formula de note que ens donen
    float fm=N2*F0;
    float fc=N1*F0;
    phase=0;
    phase2=0;
    velocidad=vel/128.0F;
    step=2*M_PI*fc;
    step2= 2*M_PI*fm;
    index=0;
  }
  else if(cmd==0 || cmd==8){
    adsr.stop();
  }
}


const vector<float> & InstrumentVibrato::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = 0.2*velocidad*sin(phase+I*sin(phase2));
    phase = phase + step;
    phase2 = phase2 + step2;
    while(phase>2*M_PI)
      phase = phase - 2*M_PI;

    while(phase2>2*M_PI)
    phase2 = phase2 - 2*M_PI;
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}