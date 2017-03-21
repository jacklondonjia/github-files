#ifndef __GUARD_H
#define __GUARD_H

/*********************Annotate********************************/
/*²âÊÔ128min Îó²îÔÚ2min */
/*********************typedef************************/
typedef enum
{
  Status_UnGuarded  = 0 ,
  Status_Guarded ,
}StatusGuard_ETypeDef;
  
typedef enum
{
  Status_UnAuto  = 0 ,
  Status_Auto ,
}AutoGuard_ETypeDef;
  

typedef enum
{
	None = 0 ,
   	Controller_Disconnect ,
  	DeadlineBecomeDue ,
	AppLockCmd,
	RemoteLock,
	GPRSLock,
}StatusGuardReason_ETypeDef;

typedef enum
{
  Status_UnReachDeadline  =0 ,
  Status_ReachedDeadline_NoExecute,
  Status_ReachedDeadline_Execute,
}StatusReachDeadline_ETypeDef;

typedef enum
{
  Unlock_Ebike =0 ,
  Lock_Ebike ,
}LockCmdFromBle_ETypeDef;

typedef enum
{
  Unlock_ByGPRS = 0 ,
  Lock_ByGPRS ,
}LockCmdFromGPRS_ETypeDef ;


typedef struct
{
	StatusGuard_ETypeDef		status ;
	StatusGuardReason_ETypeDef	status_guardreason ;
	AutoGuard_ETypeDef		status_autoguard ;
	FlagStatus	status_ATCmd_response ;
	//FlagStatus	status_connectwithcontroller ;
	FlagStatus	status_unguardrecoverable_whenstayAppconnected ;
	SwitchLevel_TypeDef		GInPin ; 
	
	LockCmdFromBle_ETypeDef		cmd_lock_From_BLE ;
/****************deadline***********************/
	StatusReachDeadline_ETypeDef	status_deadline ;
	UnionSumWithH8L8_TypeDef	timer_day_guard_timeleft ;
	uint8_t timer_hour_guard_timeleft ;
	uint8_t timer_minute_guard_timeleft ;
	uint8_t timer_second_guard_timeleft ;
//#if GPS_COMPILE
	LockCmdFromGPRS_ETypeDef	cmd_lock_From_GPRS ;	
//#endif	

}Guard_TypeDef;


/*****************************************************/
/**********************define************************/


/**********************Guard data************************/
#define		Counter_Receive_Data_Max	6
#define 	TimePeriod_Noconnected2Controller_100ms          3	//300ms

/*	when anti-theft device is guarded ,press the lock button ,
*	GIn will give a 300ms high level and then become low level*/
#define		Timer_GIn_Valid_Per2ms				200		//400ms		//30ms
/*************************************************/
/*******************Variable**********************/
extern Guard_TypeDef	Guard ;

/*************************************************/
/*******************function**********************/
void GuardControl(void);
StatusGuardReason_ETypeDef	CheckGuardReason(void);
void SetGuardStatus(void);
void ResetGuardStatus(void);
void SetGuard2Controller(void) ;
void ResetGuard2Controller(void) ;
void ATCmdGuardResponseWhenGuardStatusUnchange(void) ;

//void ReadGuardDeadLine(void);
void GuardDeadLineApproachPer1s(void);
void IfDeadLineUpdate(void);
























#endif