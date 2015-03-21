#include <RF_Switch.h>

#define rxPin 7

RF_Switch rfSwitch = RF_Switch();
int pw;
String str;

void setup(){
	pinMode(rxPin, INPUT);
	rfSwitch.start(rxPin);
	Serial.begin(115200);
}

void loop(){

	if (pw = highPulse()){
		sprintf(str, "High: %d micro-seconds");
		Serial.println(str);

	}

	if (pw = lowPulse()){
		sprintf(str, "Low: %d micro-seconds");
		Serial.println(str);
	}
}
