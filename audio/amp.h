#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../lib/RtAudio.h"
#include "math.h"
 
// Pass-through function.
int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if (status) std::cout << "Stream over/underflow detected." << std::endl;
 
  unsigned int *bytes = (unsigned int *) data;
  int* frames = (int*) inputBuffer;
  unsigned int gain = 180000000;
  for (int i = 0; i < *bytes / 4; i++){
    frames[i] = 4 * gain * 0.91f * tanh(((float)(frames[i]) / 100000000.00f) * gain / 10000000.00f);
  }
  memcpy( outputBuffer, inputBuffer, *bytes );
  return 0;
}
 
void StartAmp()
{
  RtAudio adac;
  std::vector<unsigned int> deviceIds = adac.getDeviceIds();
  if ( deviceIds.size() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  for(int i = 0; i < deviceIds.size(); i++){
    std::cout << "[" << i << "] " << adac.getDeviceNames()[i] << " (ID " 
    << deviceIds[i] << ")" << std::endl;
  }
  int iDevNum = 0, oDevNum = 0;
  std::cout << "Choose input device: ";
  std::cin >> iDevNum;
  std::cout << "Choose output device: ";
  std::cin >> oDevNum;
  // Set the same number of channels for both input and output.
  unsigned int bufferBytes, bufferFrames = 1;
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = deviceIds[iDevNum]; // first available device
  iParams.nChannels = 1;
  oParams.deviceId = deviceIds[oDevNum]; // first available device
  oParams.nChannels = 1;
 
  if ( adac.openStream( &oParams, &iParams, RTAUDIO_SINT32, 48000,
                        &bufferFrames, &inout, (void *)&bufferBytes ) ) {
    std::cout << '\n' << adac.getErrorText() << '\n' << std::endl;
    exit( 0 ); // problem with device settings
  }
 
  bufferBytes = bufferFrames * 4;
 
  if ( adac.startStream() ) {
    std::cout << adac.getErrorText() << std::endl;
    goto cleanup;
  }
 
  char input;
  std::cout << "\nRunning ... press <enter> to quit.\n";
  std::cin >> input;
 
  // Block released ... stop the stream
  if (adac.isStreamRunning() )
    adac.stopStream();  // or could call adac.abortStream();
 
 cleanup:
  if ( adac.isStreamOpen() ) adac.closeStream();
}