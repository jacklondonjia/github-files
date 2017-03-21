#include "stdint.h"
#include "Tool_Functions.h"

#include <driverlib/aux_adc.h>
/********************************************************************************
 * Write to a circular buffer
 *******************************************************************************/
enum BufferStatus bufferWrite(volatile struct Buffer *buffer, int16_t value) 
{
	uint8_t next_index = (((buffer->newest_index) + 1) % BUFFER_SIZE);

	if (next_index == buffer->oldest_index) {
		return BUFFER_FULL;
	}

	buffer->data[buffer->newest_index] = value;
	buffer->newest_index = next_index;
	return BUFFER_OK;
}
/********************************************************************************
 * Read for a circular buffer
 *******************************************************************************/
enum BufferStatus bufferRead(volatile struct Buffer *buffer, int16_t *value) 
{

	if (buffer->newest_index == buffer->oldest_index) 
	{
		return BUFFER_EMPTY;
	}

	*value = buffer->data[buffer->oldest_index];
	buffer->oldest_index = ((buffer->oldest_index + 1) % BUFFER_SIZE);
	return BUFFER_OK;
}

/********************************************************************************
 * peek into a circular buffer
 *******************************************************************************/
enum BufferStatus bufferPeek(volatile struct Buffer *buffer, int16_t *value) 
{

	uint8_t last_index = ((BUFFER_SIZE + (buffer->newest_index) - 1) % BUFFER_SIZE);
	if (buffer->newest_index == buffer->oldest_index) 
	{
		return BUFFER_EMPTY;
	}
	*value = buffer->data[last_index];
	return BUFFER_OK;
}

/********************************************************************************
 * Adjust the 12 bit ADC value based on factory configuration, VDD REF
 *******************************************************************************/
uint16_t adcValueAdjustValueForGainAndOffset(int32_t adcValue) 
{
	adcValue = AUXADCAdjustValueForGainAndOffset(adcValue,
													 AUXADCGetAdjustmentGain(AUXADC_REF_VDDA_REL),
													 AUXADCGetAdjustmentOffset(AUXADC_REF_VDDA_REL));
	return adcValue;
}
