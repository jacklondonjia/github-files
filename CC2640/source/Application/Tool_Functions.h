#ifndef __TOOL_FUNCTIONS_H
#define __TOOL_FUNCTIONS_H
#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * TYPEDEFS
 */
enum BufferStatus { BUFFER_OK, BUFFER_EMPTY, BUFFER_FULL };
#define BUFFER_SIZE  40
struct Buffer 
{
	int16_t data[BUFFER_SIZE];
	uint8_t newest_index;
	uint8_t oldest_index;
};

/*********************************************************************
 * FUNCTIONS
 */
enum BufferStatus bufferWrite(volatile struct Buffer *buffer, int16_t value);
enum BufferStatus bufferRead(volatile struct Buffer *buffer, int16_t *value);
enum BufferStatus bufferPeek(volatile struct Buffer *buffer, int16_t *value);
/*********************************************************************
 * @fn      adcValueAdjustValueForGainAndOffset
 *
 * @brief   Adjust the 12 bit ADC value based on factory configuration
 *
 * @param   adcValue - uncalibrated adc value with VDD REF
 *
 * @return  calibrated adc value
 */
extern uint16_t adcValueAdjustValueForGainAndOffset(int32_t adcValue);

#ifdef __cplusplus
}
#endif

#endif
