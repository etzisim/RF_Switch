#include <RF_Switch.h>
#include <assert.h>
#include <Arduino.h>

#define BUFFERSIZE 200
#define next(i) ((i+1) % BUFFERSIZE)
#define prev(i) ((BUFFERSIZE+i-1) % BUFFERSIZE)

volatile uint32_t transitionTimes[BUFFERSIZE] = {0};
volatile int buffered;
volatile int toWrite;
int toRead;
bool firstRead = true;
int pin;

void clockTransition(){
	if (buffered > 0 && toWrite == toRead){return;}

	if (firstRead){
		firstRead = false;
		if (!digitalRead(pin)){return;}
	}

	transitionTimes[toWrite] = micros();
	toWrite = next(toWrite);
	buffered++;
}

RF_Switch::RF_Switch(int rxPin){
	assert(rxPin == 3 || rxPin == 2);

	attachInterrupt(rxPin-2, clockTransition, CHANGE);
	pin = rxPin;

	toWrite = 0;
	toRead = 0;
	buffered = -1;

	nextPulseHigh = true;
}

uint16_t RF_Switch::pulse(bool *high){
	if (buffered < 1){return 0;}

	*high = nextPulseHigh;
	nextPulseHigh = !nextPulseHigh;

	toRead = next(toRead);
	buffered--;

	return transitionTimes[toRead]-transitionTimes[prev(toRead)];
}
