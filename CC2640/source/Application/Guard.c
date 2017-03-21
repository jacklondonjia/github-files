//#include <stdint.h>
//#include "stdio.h"
//#include "TypeDefines.h"
//#include "Defines.h"
//#include "_Parameter.h"
//#include "globalvariables.h"
//#include "BLESystem.h"
//#include "ATcontrol.h"
//#include "BatteryPowerCal.h"
//#include "Guard.h"
//#include "Onelinecommunica.h"
//#include "MileageCal.h"
//#include "FreeKeyAndMechaLock.h"
//#include "PeriodicTimer.h"
#include "CustomerPara.h"
#include "Board.h"

/*************************************************/
/*******************Variable**********************/
Guard_TypeDef	Guard ;

/***********************************************/
/*****************function********************/
/*******************************************************************************
* 函数名        : 1:1信号输入低电平有效
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void GuardControl(void)
{
		
	Guard.status_guardreason = CheckGuardReason();
	
	if(Status_Guarded == Guard.status)
	{
		if(None == Guard.status_guardreason)
			ResetGuardStatus();
		else if(AppLockCmd == Guard.status_guardreason)
		{
			if(True == Guard.status_unguardrecoverable_whenstayAppconnected)
			{
				Guard.status_unguardrecoverable_whenstayAppconnected = False ;
				Guard.cmd_lock_From_BLE = Unlock_Ebike ;
				Guard.status_guardreason = None ;
				ResetGuardStatus();
			}
		}
		else
			;
	}
	else
	{
		if(None == Guard.status_guardreason)
			;
		else if(Controller_Disconnect == Guard.status_guardreason)
		{
			Guard.cmd_lock_From_BLE = Lock_Ebike ;		//用来保证每次电门锁断电以后需要用手机解锁
			Guard.status_unguardrecoverable_whenstayAppconnected = True ;	//当App连接没有断时，下次上电门锁可以直接解锁
			SetGuardStatus();
		}
		else
			SetGuardStatus();
	}
	
	ATCmdGuardResponseWhenGuardStatusUnchange();
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
StatusGuardReason_ETypeDef	CheckGuardReason(void)
{
	if(Status_ReachedDeadline_Execute == Guard.status_deadline)
		return DeadlineBecomeDue ;
	
#if(ElectricDouSupport != Anti_theftDevice_Function)
	/*如果电平为高电平 则表示防盗器不在防盗状态*/
	//if(Bit_RESET == GPIO_ReadInputDataBit(Port_Guard_Input,Pin_Guard_Input))
	if(0 == (uint8_t)PIN_getInputValue(Pin_Guard_Input))
		return RemoteLock ;
	
	#if(External_Anti_theft!= Anti_theftDevice_Function)
		#error "需要更改这部分代码"
	#endif
	
#endif

#if GPS_COMPILE
		if(Lock_ByGPRS == Guard.cmd_lock_From_GPRS)
			return GPRSLock ;
#endif

	
	if(Status_Auto == Guard.status_autoguard)
	{
		if(False == BLEMdlCnt.status_connect2controller)
			return Controller_Disconnect ;
		
		if(Lock_Ebike == Guard.cmd_lock_From_BLE)
			return AppLockCmd ;
		else
			return None ;
	}
	else
		return None ;
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SetGuardStatus(void)
{	
#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
	if(Status_MotorUnlockAction == MechalMotorLock.status)
		MechalMotorLockStopDrive();	
	MechalMotorLock.status = Status_MotorWaitForStop ;		//等待电机停止转动
	CheckMechalMotorLockStatusAndResponse();
	/*在下面 有response*/
#else
	SetGuard2Controller();
#endif
	
	//	/********SendGuardStatusChange*************/
	//	CloseCyclicSendWhenReceiveNewATCmd();
	//	SendATCmdResponseCode(RCODE_Guarded);
	//	//AT.flag_alreadysent = True ;
	
	/********BuzzerSet*************/
	AT.Buzzer.counter_targetringnum_mul2 = Ring2_Mul2 ;
	AT.Buzzer.counter_nowringnum_mul2 = 0;
	AT.Buzzer.timer_addcounter_per2ms = 0 ;
	
	Guard.status = Status_Guarded;
	
	CheckGuardStatusAndResponse();
	Guard.status_ATCmd_response = False ;	// Flag AT guardCmd response clear
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void ResetGuardStatus(void)
{
#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
	if(Status_MotorLockedAction == MechalMotorLock.status)
		MechalMotorLockStopDrive();	
	MechalMotorLock.status = Status_WaitForBrake ;		//等待刹车信号
	CheckMechalMotorLockStatusAndResponse();
#else
	ResetGuard2Controller();	
#endif
	

	
	//	/********SendGuardStatusChange*************/
	//	CloseCyclicSendWhenReceiveNewATCmd();
	//	SendATCmdResponseCode(RCODE_UnGuarded);
	//	//AT.flag_alreadysent = True ;
	
	/********BuzzerSet*************/
	AT.Buzzer.counter_targetringnum_mul2 = Ring1_Mul2 ;
	AT.Buzzer.counter_nowringnum_mul2 = 0;
	AT.Buzzer.timer_addcounter_per2ms = 0 ;
	
	Guard.status = Status_UnGuarded;	
	CheckGuardStatusAndResponse();
	Guard.status_ATCmd_response = False ;	// Flag AT guardCmd response clea
}


/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SetGuard2Controller(void)
{
#if(Commu_Mode_TwoWire == Commu_Mode)
	//CustomSerialInterfaceMessage.status_guard = True ;
	T2C.CommonData.guardstatusBit = 1 ;
#else
	PIN_setOutputValue(getGpioPinHandle(), Pin_Guard_Output, 0);//输出低电平 （低电平有效）
#endif
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void ResetGuard2Controller(void)
{
#if(Commu_Mode_TwoWire == Commu_Mode)
	//CustomSerialInterfaceMessage.status_guard = False	;
	T2C.CommonData.guardstatusBit = 0 ;
#else
	PIN_setOutputValue(getGpioPinHandle(), Pin_Guard_Output, 1);		//输出高电平 （低电平有效）
#endif
}


/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void ATCmdGuardResponseWhenGuardStatusUnchange(void)
{
	if(True == Guard.status_ATCmd_response)
	{
		CheckGuardStatusAndResponse();
		Guard.status_ATCmd_response = False ;
	}
	else
		;
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void GuardDeadLineApproachPer1s(void)
{
	if(Status_UnReachDeadline == Guard.status_deadline  )
	{
		if(0 == Guard.timer_second_guard_timeleft )
		{
			if( 0 == Guard.timer_minute_guard_timeleft )
			{
				if(0 == Guard.timer_hour_guard_timeleft )
				{
					if(0 == Guard.timer_day_guard_timeleft.sum )
						Guard.status_deadline = Status_ReachedDeadline_NoExecute ;
					else  
					{
						Guard.timer_day_guard_timeleft.sum -- ;
						Guard.timer_hour_guard_timeleft = 23 ;
						Guard.timer_minute_guard_timeleft = 59 ;
						Guard.timer_second_guard_timeleft = 59 ;
					}
				}
				else
				{
					Guard.timer_hour_guard_timeleft -- ;
					Guard.timer_minute_guard_timeleft = 59 ;
					Guard.timer_second_guard_timeleft = 59 ;
				}
			}
			else
			{
				Guard.timer_minute_guard_timeleft -- ;
				Guard.timer_second_guard_timeleft = 59 ;
			}
		}
		else
		{
			Guard.timer_second_guard_timeleft -- ;
		}
	}
	else if(Status_ReachedDeadline_NoExecute == Guard.status_deadline)
	{	  
		if(Status_Motor_Park == Motor.status_motor)
		{
			 Guard.status_deadline = Status_ReachedDeadline_Execute ;
		}
		else
			;	
	}
	else
		;
	IfDeadLineUpdate();
}
/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void IfDeadLineUpdate(void)
{
	if(Guard.timer_day_guard_timeleft.sum > 0 || Guard.timer_hour_guard_timeleft > 0  \
		||Guard.timer_minute_guard_timeleft > 0 || Guard.timer_second_guard_timeleft > 0)
		Guard.status_deadline = Status_UnReachDeadline ;
}



