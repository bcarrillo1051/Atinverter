#include "ZMPT101B.h"

/// @brief ZMPT101B constructor
/// @param frequency AC system frequency
ZMPT101B::ZMPT101B(uint16_t frequency)
{
	period = 1000000 / frequency;
}

/// @brief Set sensitivity
/// @param value Sensitivity value
void ZMPT101B::setSensitivity(float value)
{
	sensitivity = value;
}

/// @brief Calculate zero point
/// @return zero / center value
int ZMPT101B::getZeroPoint(int ADC_val)
{
	uint32_t Vsum = 0;
	uint32_t measurements_count = 0;
	uint32_t t_start = micros();

	while (micros() - t_start < period)
	{
		Vsum += ADC_val;
		measurements_count++;
	}

	return Vsum / measurements_count;
}

/// @brief Calculate root mean square (RMS) of AC valtage
/// @param loopCount Loop count to calculate
/// @return root mean square (RMS) of AC valtage
float ZMPT101B::getRmsVoltage(uint8_t loopCount, int ADC_val)
{
	// float Vref = 5.0;
	// int ADC_max_val = 4095;
	float readingVoltage = 0.0f;

	for (uint8_t i = 0; i < loopCount; i++)
	{
		//int zeroPoint = this->getZeroPoint(ADC_val);

		int32_t Vnow = 0;
		uint32_t Vsum = 0;
		uint32_t measurements_count = 0;
		uint32_t t_start = micros();

		while (micros() - t_start < period)
		{
			Vnow = ADC_val - zeroPoint;
			Vnow = ADC_val;
			Vsum += (Vnow * Vnow);
			measurements_count++;
		}

		readingVoltage += sqrt(Vsum / measurements_count) / ADC_SCALE * VREF * sensitivity;
	}

	// readingVoltage = (Vref / ADC_max_val) * ADC_val;

	//return this->getZeroPoint(ADC_val);
	return readingVoltage / loopCount;
}
