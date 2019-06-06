#include <stdbool.h>

#undef min
#undef max

#include <string>

// Returns TRUE if successful, FALSE if not
bool initVirtualInput(int argc, char* argv[]);

std::string getLineOfInput(bool anythingHeld);
bool isInteractive(void);
void printHelp(void);

unsigned currentCycle(void);  // current cycle number, first cycle is 0
void nextCycle(void);  // should only be used by cores/virtual/main.cpp, to increment currentCycle()

void logUSBEvent(std::string descrip, void* data, int length);
void logUSBEvent_keyboard(std::string descrip);  // assumes 'descrip' uniquely describes the raw data too
void logLEDStates(std::string descrip);  // assumes 'descrip' uniquely describes the LED states
