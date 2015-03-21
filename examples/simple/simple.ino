#include <RF_Switch.h>

#define rxPin 7

RF_Switch rfSwitch;
int pw;
char str[64];

void setup(){
	pinMode(rxPin, INPUT);
	rfSwitch.start(rxPin);
	Serial.begin(115200);
}

void loop(){

	if (pw = rfSwitch.highPulse()){
		sprintf(str, "High: %d micro-seconds");
		Serial.println(str);

	}

	if (pw = rfSwitch.lowPulse()){
		sprintf(str, "Low: %d micro-seconds");
		Serial.println(str);
	}
}
