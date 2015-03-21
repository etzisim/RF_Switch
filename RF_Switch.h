#ifndef RF_Switch_h
#define RF_Switch_h

#include <stdint.h>

#define BUFFERSIZE 100

class RF_Switch{

public:
	RF_Switch();
	void start(byte rxPin); /* rxPin: D0-D7 */
	uint32_t highPulse();
	uint32_t lowPulse();
private:
	byte rxPin;
	int toRead;
	volatile int toWrite;
	volatile uint32_t transitionTimes[BUFFERSIZE];
	volatile bool nextPulseHigh;
	volatile bool hasBuffered;
};

#endif
