#include <RF_Switch.h>
#include <stdint.h>

#define rxPin 7
#define txPin 6
#define MAX_SIGNAL_SIZE;

RF_Switch rfSwitch;
uint16_t pulseWidths[MAX_SIGNAL_SIZE] = {0};
uint16_t pw;
int i = 0;
uint32_t waitThreshold = 5000; /* milli-seconds */
uint32_t startTime;

char str[64];

void setup(){
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	rfSwitch.start(rxPin);
	Serial.begin(115200);
	startTime = millis();
}

void loop(){

	if ((millis() - startTime) < waitThreshold){
		listen();
	}else{
		echo();
		delay(100);
	}

}
void listen(){
	if (i == MAX_SIGNAL_SIZE){
		return;
	}

	if ( (pw = rfSwitch.highPulse())){
		pulseWidths[i++] = pw;
	}

	if ((pw = rfSwitch.lowPulse())){
		pulseWidths[i++] = pw;
	}
}

void echo(){
	/* Assume first pulse is high pulse */
	for (int j = 0; j < MAX_SIGNAL_SIZE; j++){
		if (pulseWidths[j]){
			digitalWrite(txPin, (j%2 ? LOW, HIGH));
			delay(pulseWidths[j]);
		}
		pulseWidths[j] = 0;
	}

}
