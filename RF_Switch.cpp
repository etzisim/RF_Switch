#include <RF_Switch.h>
#include <Arduino.h>

#define BUFFERSIZE 100
#define next(i) ((i+1) % BUFFERSIZE)
#define prev(i) ((BUFFERSIZE+i-1) % BUFFERSIZE)

volatile uint32_t transitionTimes[BUFFERSIZE] = {0};
volatile int toWrite;
int toRead;
bool firstRead = true;
int pin;

void clockTransition(){
	if (firstRead){
		firstRead = false;
		if (!digitalRead(pin)){
			return;
		}
	}

	transitionTimes[toWrite] = micros();
	toWrite = next(toWrite);
}

RF_Switch::RF_Switch(int rxPin){
	//assert(rxPin == 3 || rxPin == 2);

	//attachInterrupt(rxPin-2, clockTransition, CHANGE);
	attachInterrupt(rxPin, clockTransition, CHANGE);
	pin = rxPin;

	toWrite = 0;
	toRead = 0;
}

uint16_t RF_Switch::pulse(bool *high){
	if (next(toRead) == toWrite){return 0;}

	toRead = next(toRead);
	*high = toRead%2;
	return transitionTimes[toRead]-transitionTimes[prev(toRead)];
}
