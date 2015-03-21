#include <RF_Switch.h>
#include <Arduino.h>
#include <pins_arduino.h>

/* Ring-buffer */
#define next(i) ((i++) % BUFFERSIZE)
#define prev(i) ((i--) % BUFFERSIZE)


RF_Switch::start(byte pin){
	*digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));
	PCIFR  |= bit (digitalPinToPCICRbit(pin));
	PCICR  |= bit (digitalPinToPCICRbit(pin));

	rxPin = pin;
	bufN = -1;
	bufM = -1;
}

ISR (PCINT2_vect){
	noInterrupts();

	nextToWrite = next(toWrite);

	if (nextToWrite != toRead){
		transitionTimes[nextToWrite] = micros();

		if (toWrite != -1){
			hasBuffered = true;
		}else{
			nextPulseHigh = digitalRead(rxPin);
	}else{
		/* Buffer overflow */
		exit(1);
	}

	interrupts();
}  

uint32_t RF_Switch::highPulse(){
	if (!hasBuffered || !nextHigh){
		return 0
	}

	toRead = next(toRead);
	
	if (toRead == ToWrite){
		hasBuffered = false;
	}

	nextHigh = false;
	return bufferedTransitionTimes[toRead]-bufferedTransitionTimes[prev(toRead)];

}

uint32_t RF_Switch::lowPulse(){
	if (!hasBuffered || nextHigh){
		return 0
	}

	toRead = next(toRead);

	if (toRead == toWrite){
		hasBuffered = false;
	}

	nextHigh = true;
	return bufferedTransitionTimes[toRead]-bufferedTransitionTimes[prev(toRead)];
}
