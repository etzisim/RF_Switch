#ifndef RF_Switch_h
#define RF_Switch_h

#include <stdint.h>

class RF_Switch{

public:
	RF_Switch();
	void start(int rxPin); /* rxPin: D0-D7 */
	uint16_t highPulse();
	uint16_t lowPulse();
private:
	bool nextPulseHigh;
	bool hasBuffered();
};

#endif
