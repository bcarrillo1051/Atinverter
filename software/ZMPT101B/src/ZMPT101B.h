#ifndef ZMPT101B_h
#define ZMPT101B_h

#include <Arduino.h>

#define DEFAULT_FREQUENCY 50.0f
#define DEFAULT_SENSITIVITY 500.0f
#define ADC_SCALE 4095.0f
#define VREF 5.0f

class ZMPT101B
{
public:
	ZMPT101B (uint16_t frequency = DEFAULT_FREQUENCY);
	void     setSensitivity(float value);
	float 	 getRmsVoltage(uint8_t loopCount = 1);

private:
	uint32_t period;
	float 	 sensitivity = DEFAULT_SENSITIVITY;
	int getZeroPoint();
};

#endif