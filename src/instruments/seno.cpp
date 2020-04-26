#include <iostream>
#include <math.h>
#include "seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

Seno::Seno(const std::string &param) : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
  int N;
  if (!kv.to_int("N",N))
    N = 40; //default value

  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
}
void Seno::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    phase = 0;
    float F0=440.0*pow(2,(((float)note-69.0)/12.0))/SamplingRate; 
    A=vel/128.0;
    step=2*M_PI*F0;
  }
  else if(cmd==0 || cmd==8){
    adsr.stop();
  }
}
const vector<float> & Seno::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }   
  else if (not bActive)
    return x;
  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = 0.3*A*sin(phase);
    phase = phase + step;
    while(phase>2*M_PI)
      phase = phase - 2*M_PI;
  }
  adsr(x); //apply envelope to x and update internal status of ADSR
  return x;
}