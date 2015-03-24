#include <RF_Switch.h>
#include <assert.h>
#include <Arduino.h>

/* Circular buffer filled by interrupt routine, emptied by main loop. */
#define BUFFERSIZE 200
#define next(i) ((i+1) % BUFFERSIZE)
#define prev(i) ((i-1) % BUFFERSIZE)

volatile uint32_t transitionTimes[BUFFERSIZE] = {0};
volatile int toWrite;
int toRead;

void clockTransition(){

	if (toWrite != toRead){
		transitionTimes[toWrite] = micros();
		toWrite = next(toWrite);
	}else{
		/* Buffer overflow */
	}
}

RF_Switch::RF_Switch(int rxPin){
	assert(rxPin == 3 || rxPin == 2);

	attachInterrupt(rxPin-2, clockTransition, CHANGE);

	toWrite = 0;
	toRead = 0;
	transitionTimes[toWrite] = micros();
	toWrite = next(toWrite);

	nextPulseHigh = true;
}

uint16_t RF_Switch::highPulse(){
	if (!hasBuffered() || !nextPulseHigh){
		return 0;
	}

	toRead = next(toRead);

	nextPulseHigh = false;
	return transitionTimes[toRead]-transitionTimes[prev(toRead)];

}

uint16_t RF_Switch::lowPulse(){
	if (!hasBuffered() || nextPulseHigh){
		return 0;
	}

	toRead = next(toRead);

	nextPulseHigh = true;
	return transitionTimes[toRead]-transitionTimes[prev(toRead)];
}

bool RF_Switch::hasBuffered(){
	if (next(toRead) < toWrite){
		return true;
	}
	if (toWrite < toRead){
		return true;
	}
	return false;
}
