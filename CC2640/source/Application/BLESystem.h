#ifndef __BLESYSTEM_H
#define __BLESYSTEM_H

#include "TypeDefines.h"
#include <ti/drivers/PIN.h>
#include <ti/drivers/PWM.h>
/****************************************************/
/*********************typedef************************/

typedef	struct
{
	uint8_t timer_addcounter_per2ms ;
	uint8_t counter_targetringnum_mul2 ;
	uint8_t counter_nowringnum_mul2 ;
}BUZZER_TypeDef;

typedef struct
{
	FlagStatus	status_connect2controller ;
	#if(Commu_Mode_Common == Commu_Mode)
		uint8_t timer_connect2controller5V_100ms ;
		uint8_t timer_noconnect2controller5V_100ms ;
	#else
		uint8_t timer_connect2controller_100ms  ;
	#endif
}BLEModuleConnect_TypeDef;;


/**********************Buzzer************************/		
#define 	Ring1_Mul2							(1*2)
#define		Ring2_Mul2							(2*2) 
#define		Ring5_Mul2							(5*2)
#define		Timer_AddCounter_Per2ms				75			//75*2ms =150ms
					



/*************************************************/
/*******************Variable**********************/
extern BLEModuleConnect_TypeDef	BLEMdlCnt;

/*************************************************/
/*******************function**********************/
void UpdateBLEAdverCyclical(void);
void SendResultOfTestCode(void);
FlagStatus IsBuzzerRingEnable(void);
void BuzzerRingWhenEnable(PIN_Handle pinHandle);
UnionSumWithH8L8_TypeDef SwapH8L8(UnionSumWithH8L8_TypeDef ) ;

void UpdateStatusConnectToController(void) ;	
extern void SoftwareReset(void) ;

#endif
