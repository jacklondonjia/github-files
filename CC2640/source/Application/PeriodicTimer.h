#ifndef __PERIODIC_TIMER_H
#define __PERIODIC_TIMER_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "TypeDefines.h"
/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * TYPEDEFS
 */
typedef struct
{
	volatile uint8_t lastLevel;
	volatile uint16_t ticks_since_level_changed;
}Level_Tracker;


/*********************************************************************
 * FUNCTIONS
 */
void scTaskAlertCallback(void);
void scCtrlReadyCallback(void);
void setupSC(void);
void processOneLineQueue(void);
void EventPer2ms(void);
void EventPer100ms(void);
void EventPer500ms(void) ;
void EventPer1s(void) ;
void EventPer30s(void) ;
void mainLoopFunctions(void) ;
/*
 * Task creation function for the Simple BLE Peripheral.
 */
void PeriodicTimer_createTask(void);
void LVInitial(void);
void VariableInitial(void);


extern PIN_Handle getGpioPinHandle(void);
/*********************************************************************
*********************************************************************/


#ifdef __cplusplus
}
#endif

#endif
