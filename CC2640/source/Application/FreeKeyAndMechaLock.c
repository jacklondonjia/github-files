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
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void FreeKeyPowerOnAction(void)
{
	#if(BLE_H ==PCB_VERSION )
	GPIO_SetBits(Port_XL,Pin_XL);		//����ߵ�ƽ ��Դ�����PMOSFET��ͨ
	#elif((RGZ_A ==PCB_VERSION)||(RGZ_B ==PCB_VERSION)||(DLCC01 ==PCB_VERSION))
	PIN_setOutputValue(getGpioPinHandle(), Pin_CPMOS, 1); //����ߵ�ƽ ��Դ�����PMOSFET��ͨ
	#else
		#error "ȱ����Կ�׵Ķ���"
	#endif
	FreeKey.timer_48VSWActionOuttime_per2ms = 0 ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void FreeKeyPowerOffAction(void)
{
	#if(BLE_H ==PCB_VERSION )
	GPIO_ResetBits(Port_XL,Pin_XL);		//����͵�ƽ ��Դ�����PMOSFET�Ͽ�
	#elif((RGZ_A ==PCB_VERSION)||(RGZ_B ==PCB_VERSION)||(DLCC01 ==PCB_VERSION))
	PIN_setOutputValue(getGpioPinHandle(), Pin_CPMOS, 0);		//����͵�ƽ ��Դ�����PMOSFET�Ͽ�
	#else
		#error "ȱ����Կ�׵Ķ���"
	#endif
	FreeKey.timer_48VSWActionOuttime_per2ms = 0 ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void FreeKeyPowerOn(void)
{
	FreeKey.status = Status_PowerOnAction ;
	FreeKeyPowerOnAction();	
	FreeKey.counter_retry_whenfailure = 0 ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void FreeKeyPowerOff(void)
{
	FreeKey.status = Status_PowerOffAction ;
	FreeKeyPowerOffAction();
	FreeKey.counter_retry_whenfailure = 0 ;
}


/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : value: 12-bits adc value
* ������      : ��
* ����ֵ        : ��
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
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
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
* ��������    	:LDƽʱΪ�ߵ�ƽ ���ֶ��ź�ʱ��ɵ͵�ƽ
* �������      : value of the LD pin
* ������      : None
* ����ֵ        : None
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
* ��������    	:MA+���� MA-���� ����	MA+:���������� MA-������������
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void SeatLockLockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V�����ͨ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 0);//A1������͵�ƽ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 1);//A2������ߵ�ƽ
	SeatLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* ��������    	:MA+���� MA-���� ����
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void SeatLockUnlockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V�����ͨ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 1);//A1������ߵ�ƽ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 0);//A2������͵�ƽ
	SeatLock.timer_action_per100ms = 0 ;
	SeatLock.counter_unlockactionfrequent ++ ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void SeatLockStopDrive(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 0);//9V����Ͽ�
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 0);//A1������͵�ƽ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 0);//A2������͵�ƽ
	SeatLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
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
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void CheckSeatLockActionResult(void)
{
	if(Status_WaitForMotorStop == SeatLock.status)
	{
		if(ConnectWithController == BLEMdlCnt.status_connect2controller)	//ȷ�ϺͿ���������������
		{
			if(OLCM.counter_hall_speed.sum <= 2)				//ȷ�ϵ���������Ѿ�ֹͣת��
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
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void MechalMotorLockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V�����ͨ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD1, 1);//D1������ߵ�ƽ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD2, 0);//D2������͵�ƽ
	MechalMotorLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* ��������    	: D1�ߣ����� D2�ߣ�����
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void MechalMotorUnlockAction(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);//9V�����ͨ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD1, 0);//D1������͵�ƽ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD2, 1);//D2������ߵ�ƽ
	MechalMotorLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void MechalMotorLockStopDrive(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 0);//A1������͵�ƽ
	PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 0);//A2������͵�ƽ
	PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 0);//9V����Ͽ�
	MechalMotorLock.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
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
		if(ConnectWithController == BLEMdlCnt.status_connect2controller)	//ȷ�ϺͿ���������������
			{
				if(OLCM.counter_hall_speed.sum <= 2)				//ȷ�ϵ���������Ѿ�ֹͣת��
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
			
			//�رյ�Դ���
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
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
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
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void SendDouSupportTransCode(void)
{
  if(DST.counter_tojudgelevel & U8_BIT0)	//ż��Ϊ�͵�ƽ������Ϊ�ߵ�ƽ
  {
  	  if(True == IsReachDouSupportTransChangeLeveltime(DST.timer_target[DST.counter_tojudgelevel]) ) 	
	  {
  		  PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 0);//����͵�ƽ
			DST.counter_tojudgelevel ++ ;
	  }
	  else 
		  ;
  }
  else 
  {
	  if(True == IsReachDouSupportTransChangeLeveltime(DST.timer_target[DST.counter_tojudgelevel]) ) 	
	  {
		  	PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 1);		//����ߵ�ƽ
			DST.counter_tojudgelevel ++ ;
	  }
	  else
		  ;
  } 
  
  if(DST_CycleTimes == DST.counter_cycle)
	  LoadDST();
  else if(0 == DST.counter_cycle)		//�ⲿ���ж����� Ҫô��cycle��0 Ҫô�������һ֡��û������
	  ;
  else
  {	
	  if(DST.counter_tojudgelevel > (uint8_t)(DST_Strlen_Code*8*2+2-1))//ȷ�����һ����������������
	 	 LoadDST();
	  else
		  ;
  }
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void LoadDST(void)
{
	LoadDSTCode();
	LoadDSTTargetTime();
	PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 0);//����͵�ƽ
	DST.Data2Level.Level.timer_temp = 0 ;
	
	//if(DST.counter_cycle > 0 )
		DST.counter_cycle -- ;
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
FlagStatus AllDSTTransComplete(void)
{
	if(DST.counter_cycle > 0 )
		return False ;
	else
	{
		//if(0 == DST.counter_tojudgelevel )//��cycleΪ0 �ҷ������ LoadDSTTargetTime��Ѹñ�����0
		if(DST.counter_tojudgelevel >= (uint8_t)(DST_Strlen_Code * 8 * 2 + 2 -1))//ȷ�����һ����������������
			return True ;
		else
			return False ;
	}
}

/*******************************************************************************
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void SendDouSupportTransIdle(void)
{
	PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 1);//����ߵ�ƽ
}

/*******************************************************************************
* ������  		:
* ��������    	: �ж��Ƿ񵽴��л���ƽ��ʱ��� �����������������ҷ����жϽ��
* �������      : ��
* ������      : ��
* ����ֵ        : ��
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
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
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
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
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
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
uint8_t CalDSTStatusData(void)
{
	return (uint8_t)DST.cmd ;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void DouSupportFallAction(void)
{
	DST.cmd = Cmd_Fall ;
	DST.counter_cycle = DST_CycleTimes ;
	DouSupport.timer_action_per100ms = 0 ;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
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
