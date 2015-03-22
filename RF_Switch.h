#ifndef RF_Switch_h
#define RF_Switch_h

#include <stdint.h>

class RF_Switch{

public:
	RF_Switch(int rxPin); /* input pin 2 or 3 */
	uint16_t highPulse();
	uint16_t lowPulse();
private:
	bool nextPulseHigh;
	bool hasBuffered();
};

#endif
