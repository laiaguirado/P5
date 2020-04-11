#include <iostream>
#include "instrument_dumb.h"
#include "piano.h"
#include "guitarra.h"
#include "violin.h"
#include "generico.h"
#include "seno.h"

/*
  For each new instrument:
  - Add the header in this file
  - Add the call to the constructor in get_instrument() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Instrument * get_instrument(const string &name,
			      const string &parameters) {
    Instrument * pInst = 0;
    //    cout << name << ": " << parameters << endl;
    if (name == "InstrumentDumb") {
      pInst = (Instrument *) new InstrumentDumb(parameters);
    }
    if(name == "Piano"){
      pInst = (Instrument *) new Piano(parameters);
    }
    if(name == "Guitarra"){
      pInst = (Instrument *) new Guitarra(parameters);
    }
    if(name == "Violi"){
      pInst = (Instrument *) new Violi(parameters);
    }
    if(name == "Generico"){
      pInst = (Instrument *) new Generico(parameters);
    }
    if(name == "Seno"){
      pInst = (Instrument *) new Seno(parameters);
    }
    return pInst;
  }
}
