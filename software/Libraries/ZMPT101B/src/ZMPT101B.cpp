#include "ZMPT101B.h"
#include "Atinverter.h"

extern Atinverter atinverter; // Access the global instance of atinverter

/// @brief ZMPT101B constructor
/// @param frequency AC system frequency
ZMPT101B::ZMPT101B(uint16_t frequency)
{
	period = 1000 / frequency; // 60Hz = 16666 µs (or ~16.7 milliseconds)
}

/// @brief Set sensitivity
/// @param value Sensitivity value
void ZMPT101B::setSensitivity(float value)
{
	sensitivity = value;
}

/// @brief Calculate zero point
/// @return zero / center value
int ZMPT101B::getZeroPoint()
{
	uint32_t Vsum = 0;
	uint32_t measurements_count = 0;
	uint32_t t_start = atinverter.millis2();

	while (atinverter.millis2() - t_start < period)
	{
		Vsum += atinverter.readADC(0x00);
		measurements_count++;
	}

	return Vsum / measurements_count;
}

/// @brief Calculate root mean square (RMS) of AC valtage
/// @param loopCount Loop count to calculate
/// @return root mean square (RMS) of AC valtage
float ZMPT101B::getRmsVoltage(uint8_t loopCount)
{
	// float Vref = 5.0;
	// int ADC_max_val = 4095;
	double readingVoltage = 0.0f;

	for (uint8_t i = 0; i < loopCount; i++)
	{
		int zeroPoint = this->getZeroPoint();

		int32_t Vnow = 0;
		uint32_t Vsum = 0;
		uint32_t measurements_count = 0;
		uint32_t t_start = atinverter.millis2();

		while (atinverter.millis2() - t_start < period)
		{
			Vnow = atinverter.readADC(0x00) - zeroPoint;
			Vsum += (Vnow * Vnow);
			measurements_count++;
		}

		readingVoltage += sqrt(Vsum / measurements_count) / ADC_122S021_MAX_VALUE * VREF * sensitivity;
	}

	return readingVoltage / loopCount;
}
