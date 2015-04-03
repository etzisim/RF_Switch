#include <RF_Switch.h>
#include <assert.h>
#include <Arduino.h>

#define BUFFERSIZE 200
#define next(i) ((i+1) % BUFFERSIZE)
#define prev(i) ((i-1) % BUFFERSIZE)

volatile uint32_t transitionTimes[BUFFERSIZE] = {0};
volatile int buffered;
volatile int toWrite;
int toRead;

void clockTransition(){
	if (buffered > 0 && toWrite == toRead){
		return;
	}

	transitionTimes[toWrite] = micros();
	toWrite = next(toWrite);
	buffered++;
}

RF_Switch::RF_Switch(int rxPin){
	assert(rxPin == 3 || rxPin == 2);

	attachInterrupt(rxPin-2, clockTransition, CHANGE);

	toWrite = 0;
	toRead = 0;
	buffered = -1;

	nextPulseHigh = true;
}

uint16_t RF_Switch::highPulse(){
	if (buffered > 0 || !nextPulseHigh){
		return 0;
	}

	toRead = next(toRead);
	buffered--;

	nextPulseHigh = false;
	return transitionTimes[toRead]-transitionTimes[prev(toRead)];

}

uint16_t RF_Switch::lowPulse(){
	if (buffered > 0 || nextPulseHigh){
		return 0;
	}

	toRead = next(toRead);
	buffered--;

	nextPulseHigh = true;
	return transitionTimes[toRead]-transitionTimes[prev(toRead)];
}

