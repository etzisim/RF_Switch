#include <RF_Switch.h>
#include <Arduino.h>
#include <pins_arduino.h>

/* Ring-buffer */
#define BUFFERSIZE 100
#define next(i) ((i++) % BUFFERSIZE)
#define prev(i) ((i--) % BUFFERSIZE)


static volatile uint32_t transitionTimes[BUFFERSIZE];
static volatile int toWrite;
static int toRead;

RF_Switch::RF_Switch(){
	nextPulseHigh = true;
}

void RF_Switch::start(int pin){

	/* Pin change interrupt setup */
	*digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));
	PCIFR  |= bit (digitalPinToPCICRbit(pin));
	PCICR  |= bit (digitalPinToPCICRbit(pin));

	toWrite = -1;
	toRead = -1;

	nextPulseHigh = true;
}

/* Pin change interrupt vector, D0-D7 */
ISR (PCINT2_vect){
	int nextToWrite = next(toWrite);

	if (nextToWrite != toRead || toRead == -1){
		transitionTimes[nextToWrite] = micros();

	}else{
		/* Buffer overflow */
		exit(1);
	}
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
	if (toRead == -1){
		if (toWrite > 0){
			return true;
		}
	}else if (toRead != toWrite){
		return  true;
	}
	
	return false;
}
