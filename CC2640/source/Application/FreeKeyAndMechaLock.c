//#include "stdint.h"
//#include "stdio.h"
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
//#include "Defines.h"
//#include "_Parameter.h"
//#include "globalvariables.h"
//#include "ATcontrol.h"
//#include "BatteryPowerCal.h"
//#include "Guard.h"
//#include "Onelinecommunica.h"
//#include "MileageCal.h"
//#include "sendandstring.h"
//#include "FreeKeyAndMechaLock.h"
//#include "PeriodicTimer.h"
#include "CustomerPara.h"
#include "Board.h"
//#include "Tool_Functions.h"

/*************************************************/
/*******************Variable**********************/
FreeKey_TypeDef	FreeKey ;
LDPower_TypeDef	LDPower ;
SeatLock_TypeDef	SeatLock ;	
MechalMotorLock_TypeDef	MechalMotorLock ;
DouSupportTrans_TypeDef	DST ;
//DouSupportReceive_TypeDef	DSR ;
DouSupport_TypeDef		DouSupport ;
//DouSupportTransMessage_Type DouSupportTransMessage ; 

/***********************************************/
/*****************function********************/
#if(External_Anti_theft != Anti_theftDevice_Function)
/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void FreeKeyPowerOnAction(void)
{
	#if(BLE_H ==PCB_VERSION )
	GPIO_SetBits(Port_XL,Pin_XL);		//输出高电平 电源输出的PMOSFET导通
	#elif((RGZ_A ==PCB_VERSION)||(RGZ_B ==PCB_VERSION)||(DLCC01 ==PCB_VERSION))
	PIN_setOutputValue(getGpioPinHandle(), Pin_CPMOS, 1); //输出高电平 电源输出的PMOSFET导通
	#else
		#error "缺少免钥匙的定义"
	#endif
	FreeKey.timer_48VSWActionOuttime_per2ms = 0 ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void FreeKeyPowerOffAction(void)
{
	#if(BLE_H ==PCB_VERSION )
	GPIO_ResetBits(Port_XL,Pin_XL);		//输出低电平 电源输出的PMOSFET断开
	#elif((RGZ_A ==PCB_VERSION)||(RGZ_B ==PCB_VERSION)||(DLCC01 ==PCB_VERSION))
	PIN_setOutputValue(getGpioPinHandle(), Pin_CPMOS, 0);		//输出低电平 电源输出的PMOSFET断开
	#else
		#error "缺少免钥匙的定义"
	#endif
	FreeKey.timer_48VSWActionOuttime_per2ms = 0 ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void FreeKeyPowerOn(void)
{
	FreeKey.status = Status_PowerOnAction ;
	FreeKeyPowerOnAction();	
	FreeKey.counter_retry_whenfailure = 0 ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void FreeKeyPowerOff(void)
{
	FreeKey.status = Status_PowerOffAction ;
	FreeKeyPowerOffAction();
	FreeKey.counter_retry_whenfailure = 0 ;
}


/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : value: 12-bits adc value
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void LV48VSWADSample(uint16_t value)
{ 
	FreeKey.sum_temp_adc += value ;
	FreeKey.counter_sample ++ ;
	
	if( FreeKey.counter_sample >= Counter_LV_48VSW_Sample_Max) 
	{  
		FreeKey.avr_adc =adcValueAdjustValueForGainAndOffset(FreeKey.sum_temp_adc >> Counter_LV_48VSW_Sample_Shift);
		FreeKey.sum_temp_adc = 0 ;
		FreeKey.counter_sample = 0 ;
	}  
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void CheckFreeKeyStartActionResult(void)
{
	if(Status_PowerOffAction == FreeKey.status)
	{
		if(FreeKey.avr_adc  < ADC_PowerOff_Thresholds_D)
		{
			FreeKey.status = Status_PowerOff ;
			ResponseFreeKeyStartAction();
		
		}	
		else 
		{	
			if(FreeKey.timer_48VSWActionOuttime_per2ms >= Timer_48VSWPowerOffOuttime_Per2ms)
			{
				FreeKey.counter_retry_whenfailure ++ ;
				if(FreeKey.counter_retry_whenfailure <= Counter_Retry_WhenPowerActionFailure)
					FreeKeyPowerOffAction();	
				else
				{
					if(Status_PowerOffFailure != FreeKey.status)
						ResponseFreeKeyStartAction() ;
					FreeKey.status = Status_PowerOffFailure ;	
				}
			}
			else
				;
		}
	}
	else if(Status_PowerOnAction ==FreeKey.status)	
	{
		//AD48SW();
		if(LV.avr_adc > ADC_PowerOff_Thresholds_Differ_D )
		{
			if(FreeKey.avr_adc > (LV.avr_adc - ADC_PowerOff_Thresholds_Differ_D))
			{
				FreeKey.status = Status_PowerOn ;
				ResponseFreeKeyStartAction();
			}
			else
			{
				if(FreeKey.timer_48VSWActionOuttime_per2ms >= Timer_48VSWPowerOnOuttime_Per2ms)
				{	
					FreeKey.counter_retry_whenfailure ++ ;
					if(FreeKey.counter_retry_whenfailure <= Counter_Retry_WhenPowerActionFailure)
						FreeKeyPowerOnAction();	
					else
					{
						if(Status_PowerOnFailure != FreeKey.status)
							ResponseFreeKeyStartAction() ;
						FreeKey.status = Status_PowerOnFailure  ;	
					}
				}
				else
					;
			}
		}
		else
			;
	}
	else if(Status_OCP == FreeKey.status)
	{
		if(FreeKey.counter_OCP_warning > 0)
		{
			ResponseFreeKeyStartAction() ;
			FreeKey.counter_OCP_warning -- ;
		}
		else
			;
	}
	else if(Status_PowerOff == FreeKey.status)
		;
	else
		;
}

/*******************************************************************************
* 函数描述    	:LD平时为高电平 有轮动信号时变成低电平
* 输入参数      : value of the LD pin
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void DetectLD(uint8_t value)
{
	if((Status_PowerOff == FreeKey.status) && (Status_Auto == Guard.status_autoguard))
	{
		if(value>0)
		{
			if(True  == LDPower.flag)
			{
				if(LDPower.timer_LDturnoff_per100ms >Timer_LDTurnOff_Per100ms)
				{
					LDPower.flag = False ;
					//ResetGuard2Controller();
					FreeKeyPowerOffAction();
				}
				else
					;
			}
		}
		else
		{
			if(False  == LDPower.flag)
			{
				LDPower.flag = True ;
				//SetGuard2Controller();
				FreeKeyPowerOnAction();
			}
			LDPower.timer_LDturnoff_per100ms = 0 ;
		}
	}
}



#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
/*******************************************************************************
* 函数描述    	:MA+流入 MA-流出 上锁	MA+:鞍座锁红线 MA-：鞍座锁黑线
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SeatLockLockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V输出导通
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 0);//A1线输出低电平
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 1);//A2线输出高电平
	SeatLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* 函数描述    	:MA+流出 MA-流人 解锁
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SeatLockUnlockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V输出导通
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 1);//A1线输出高电平
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 0);//A2线输出低电平
	SeatLock.timer_action_per100ms = 0 ;
	SeatLock.counter_unlockactionfrequent ++ ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SeatLockStopDrive(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 0);//9V输出断开
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 0);//A1线输出低电平
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 0);//A2线输出低电平
	SeatLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SeatUnlockAndChangeStatus(void)
{
	if(SeatLock.counter_unlockactionfrequent <= Counter_UnlockActionFrequent)
	{
		SeatLockUnlockAction();
		SeatLock.status = Status_UnlockAction ;
		ResponseSeatLockAction();
	}
	else
	{
		SeatLock.status = Status_OTP_Locked ;
		ResponseSeatLockAction();
	}
}
/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void CheckSeatLockActionResult(void)
{
	if(Status_WaitForMotorStop == SeatLock.status)
	{
		if(ConnectWithController == BLEMdlCnt.status_connect2controller)	//确认和控制器保持连接中
		{
			if(OLCM.counter_hall_speed.sum <= 2)				//确认电机基本上已经停止转动
				SeatUnlockAndChangeStatus() ;
			else
			{
				SeatLock.status = Status_UnlockFail_Locked ;
				ResponseSeatLockAction();
			}
		}
		else
		{
			if(Status_PowerOff == FreeKey.status )
				SeatUnlockAndChangeStatus() ;
			else
			{
				SeatLock.status = Status_UnlockFail_Locked ;
				ResponseSeatLockAction();
			}
		}
	}
	else if(Status_UnlockAction == SeatLock.status)
	{
		if(SeatLock.timer_action_per100ms >= Timer_SeatLockUnlockAction_Per100ms)
		{
			SeatLockStopDrive();	//stop drive and clear timer
			SeatLock.status = Status_Unlocked ;
			ResponseSeatLockAction();
		}
		else
			;
	}
	else if(Status_Unlocked == SeatLock.status)	
	{
		if(SeatLock.timer_action_per100ms >= Timer_SeatLockUnlockedDealy_Per100ms)
		{
			SeatLockLockAction();	//lock seat and clear timer
			SeatLock.status = Status_LockedAction ;
			ResponseSeatLockAction();
		}
		else
			;
	}
	else if(Status_LockedAction == SeatLock.status)	
	{
		if(SeatLock.timer_action_per100ms >= Timer_SeatLockLockAction_Per100ms)
		{
			SeatLockStopDrive();	//stop drive and clear timer
			SeatLock.status = Status_Locked ;
			ResponseSeatLockAction();
		}
		else
			;
	}
	else
		;
	if(SeatLock.timer_unlockactionfrequent_per1s >= Timer_SeatLockUnlockedActionFrequent_Per1s)
	{
		SeatLock.timer_unlockactionfrequent_per1s= 0 ;
		SeatLock.counter_unlockactionfrequent = 0 ;
	}

}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void MechalMotorLockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V输出导通
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD1, 1);//D1线输出高电平
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD2, 0);//D2线输出低电平
	MechalMotorLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* 函数描述    	: D1线：棕线 D2线：蓝线
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void MechalMotorUnlockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V输出导通
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD1, 0);//D1线输出低电平
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD2, 1);//D2线输出高电平
	MechalMotorLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void MechalMotorLockStopDrive(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 0);//A1线输出低电平
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 0);//A2线输出低电平
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 0);//9V输出断开
	MechalMotorLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void CheckMechalMotorLockActionResult(void)
{ 
	if(Status_WaitForBrake == MechalMotorLock.status)
	{
		if(True == MechalMotorLock.flag_breakconfirm)
		{
			MechalMotorUnlockAction() ;
			MechalMotorLock.status = Status_MotorUnlockAction ;
			CheckMechalMotorLockStatusAndResponse();
		}
		else
			;
	
	}
	else if(Status_MotorUnlockAction == MechalMotorLock.status)
	{
		if(MechalMotorLock.timer_action_per100ms >= Timer_MechalMotorLockUnlockAction_Per100ms)
		{
			MechalMotorLockStopDrive();	//stop drive and clear timer
			MechalMotorLock.status = Status_MotorUnlocked ;
			CheckMechalMotorLockStatusAndResponse();
			
			#if(ElectricDouSupport == Anti_theftDevice_Function)			
			DouSupportRetractAction() ;
			DouSupport.status = Status_RetractingAction ;
			#else
			ResetGuard2Controller();
			#endif
			
		}
		else
			;
	}
	else if(Status_MotorWaitForStop == MechalMotorLock.status)	
	{
		SetGuard2Controller();		//Stop motor
		if(ConnectWithController == BLEMdlCnt.status_connect2controller)	//确认和控制器保持连接中
			{
				if(OLCM.counter_hall_speed.sum <= 2)				//确认电机基本上已经停止转动
				{
					MechalMotorLockAction();	//lock seat and clear timer
					MechalMotorLock.status = Status_MotorLockedAction ;
					CheckMechalMotorLockStatusAndResponse();
				}
			}
	}
	else if(Status_MotorLockedAction == MechalMotorLock.status)	
	{
		if(MechalMotorLock.timer_action_per100ms >= Timer_MechalMotorLockLockAction_Per100ms)
		{
			MechalMotorLockStopDrive();	//stop drive and clear timer
			MechalMotorLock.status = Status_MotorLocked ;
			CheckMechalMotorLockStatusAndResponse();
			
			//关闭电源输出
			FreeKeyPowerOff();
			ResponseFreeKeyStartAction() ;
			
			#if(ElectricDouSupport == Anti_theftDevice_Function)			
			DouSupport.status = Status_WaitForPowerOff ;
			#endif
			#if 0
			#if(ElectricLockControl == Anti_theftDevice_Function )
			ResponseAllParkFinish();
			#endif
			#endif
		}
		else
			;
	}	
	else
		;
	
	

}

#if(ElectricDouSupport == Anti_theftDevice_Function)
/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void CheckDouSupportActionResult(void)
{
	if(Status_RetractingAction == DouSupport.status)
	{
		if(DouSupport.timer_action_per100ms >= Timer_DouSupportRetractAction_Per100ms)
		{
			DouSupport.status = Status_Retracted ;
			ResetGuard2Controller();
			ResponseDouSupportAction() ;
		}
	
	}
	else if(Status_WaitForPowerOff == DouSupport.status)
	{
		if(Status_PowerOff == FreeKey.status)
		{
			DouSupportFallAction() ;
			DouSupport.status = Status_FallingAction ;
		}
	}
	else if(Status_FallingAction == DouSupport.status)
	{
		if(DouSupport.timer_action_per100ms >= Timer_DouSupportRetractAction_Per100ms)
		{
			DouSupport.status = Status_Fallen ;
			ResponseDouSupportAction() ;
		}
	}
	else
		;
}
/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SendDouSupportTransCode(void)
{
  if(DST.counter_tojudgelevel & U8_BIT0)	//偶数为低电平，奇数为高电平
  {
  	  if(True == IsReachDouSupportTransChangeLeveltime(DST.timer_target[DST.counter_tojudgelevel]) ) 	
	  {
  		  PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 0);//输出低电平
			DST.counter_tojudgelevel ++ ;
	  }
	  else 
		  ;
  }
  else 
  {
	  if(True == IsReachDouSupportTransChangeLeveltime(DST.timer_target[DST.counter_tojudgelevel]) ) 	
	  {
		  	PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 1);		//输出高电平
			DST.counter_tojudgelevel ++ ;
	  }
	  else
		  ;
  } 
  
  if(DST_CycleTimes == DST.counter_cycle)
	  LoadDST();
  else if(0 == DST.counter_cycle)		//外部的判断条件 要么是cycle非0 要么还有最后一帧还没发送完
	  ;
  else
  {	
	  if(DST.counter_tojudgelevel > (uint8_t)(DST_Strlen_Code*8*2+2-1))//确保最后一个数组完整发送完
	 	 LoadDST();
	  else
		  ;
  }
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void LoadDST(void)
{
	LoadDSTCode();
	LoadDSTTargetTime();
	PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 0);//输出低电平
	DST.Data2Level.Level.timer_temp = 0 ;
	
	//if(DST.counter_cycle > 0 )
		DST.counter_cycle -- ;
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
FlagStatus AllDSTTransComplete(void)
{
	if(DST.counter_cycle > 0 )
		return False ;
	else
	{
		//if(0 == DST.counter_tojudgelevel )//当cycle为0 且发送完成 LoadDSTTargetTime会把该变量置0
		if(DST.counter_tojudgelevel >= (uint8_t)(DST_Strlen_Code * 8 * 2 + 2 -1))//确保最后一个数组完整发送完
			return True ;
		else
			return False ;
	}
}

/*******************************************************************************
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SendDouSupportTransIdle(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 1);//输出高电平
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	: 判断是否到达切换电平的时间点 如果到达清除计数并且返回判断结果
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsReachDouSupportTransChangeLeveltime(uint16_t targettimer)
{
	if( DST.Data2Level.Level.timer_temp >= targettimer)
	{
		DST.Data2Level.Level.timer_temp = 0 ;
		return True ;
	}
	else
		return False ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void LoadDSTTargetTime(void)
{
	uint8_t temp = 0 ;
	DST.timer_target[0] = (uint16_t)(100000/100) ;
	DST.timer_target[1] = (uint16_t)(2000/100) ;	
	for(uint8_t i = 0 ;i< DST_Strlen_Code ;i++)
	{
		temp = DST.datacode[i];
		for(uint8_t j = 0 ;j< 8;j++)
		{
			if(temp & U8_BIT7)
			{
				DST.timer_target[(8*i+j)*2+2] = (uint16_t)(2000/100) ;
				DST.timer_target[(8*i+j)*2+3] = (uint16_t)(4000/100) ;
			}
			else
			{
				DST.timer_target[(8*i+j)*2+2] =  (uint16_t)(4000/100) ;
				DST.timer_target[(8*i+j)*2+3] =  (uint16_t)(2000/100) ;
			}
			temp <<= 1 ;
		}		
	}
	DST.counter_tojudgelevel = 0 ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void LoadDSTCode(void)
{
	DST.datacode[0] = DST_StartCode ;
	DST.datacode[1] = DST_Strlen_Status ;	
	DST.datacode[2] = CalDSTStatusData() ;
	DST.datacode[3] = DST_StartCode ^ DST_Strlen_Status ^ DST.datacode[2] ;
	DST.datacode[4] = DST_EndCode ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
uint8_t CalDSTStatusData(void)
{
	return (uint8_t)DST.cmd ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void DouSupportFallAction(void)
{
	DST.cmd = Cmd_Fall ;
	DST.counter_cycle = DST_CycleTimes ;
	DouSupport.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void DouSupportRetractAction(void)
{
	DST.cmd = Cmd_Retract ;
	DST.counter_cycle = DST_CycleTimes ;
	DouSupport.timer_action_per100ms = 0 ;
}
#endif
#endif
#endif
