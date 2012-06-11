#include <string>
#include <iostream>
#include <exception>
#include "PajeFileReader.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"

using namespace std;

int main (int argc, char **argv)
{
  if (argc != 2){
    cerr << "usage : paje_validate [trace-file]" <<endl;
    return 1;
    }

  PajeFileReader *reader = new PajeFileReader (std::string(argv[1]), NULL);
  PajeEventDecoder *decoder = new PajeEventDecoder ();
  PajeSimulator *simulator = new PajeSimulator ();

  reader->setOutputComponent (decoder);
  decoder->setInputComponent (reader);
  decoder->setOutputComponent (simulator);
  simulator->setInputComponent (decoder);

  try {
    int i = 0;
    while (reader->hasMoreData()){
      reader->startChunk (i);
      reader->readNextChunk();
      reader->endOfChunkLast (!reader->hasMoreData());
      i++;
    }
  }catch (std::string exception){
    cout << exception << endl;
    return 1;
  }

  simulator->report();

  delete reader;
  delete decoder;
  delete simulator;
  return 0;
}