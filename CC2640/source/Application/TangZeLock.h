#ifndef __TANGZELOCK_H
#define __TANGZELOCK_H

#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
/*************************************************/
/*******************PinDef**********************/
#define 	Pin_TZLI				Pin_Uart2_TX
#define 	Port_TZLI				Port_Uart2_TX
	
/*************************************************/
/*******************TypeDef**********************/
#define		Strlen_TZLI_Code		3 
typedef enum
{
	KV_default =0 ,
	KV_PowerOn,
	KV_PowerOff,
	KV_UnlockMotor,
	KV_LockMotor,
	KV_UnlockSeat,
	KV_UnsetShockAlarm,
	KV_SetShockAlarm,
}KeyValue_E;

typedef struct
{
	KeyValue_E keyvalue ;
	
	Data2Level_s Data2Level ;
	//  uint8_t datacode_temp[CSI_Strlen_StartCode];
	uint8_t datacode[Strlen_TZLI_Code];
	uint16_t timer_target[(uint8_t)(Strlen_TZLI_Code * 8 * 2 + 2)] ;
	uint8_t counter_tojudgelevel ; 
	
}TangZeLockInterface_type;

/*******************Variable**********************/	
extern TangZeLockInterface_type	TangZeLockInterface ;


/*******************Function**********************/	
void SendTangZeLockInterfaceCode(void) ;
FlagStatus IsReachTangZeLockChangeLeveltime(uint16_t targettimer) ;
void LoadTZLITargetTime(void) ;
void LoadTZLICode(void);

#endif
#endif