//#include "stdio.h"
//#include "_Parameter.h"
//#include "globalvariables.h"
//#include "ATcontrol.h"
//#include "BatteryPowerCal.h"
//#include "Guard.h"
//#include "Onelinecommunica.h"
//#include "MileageCal.h"
//#include "ErrorHistory.h"
#if 0
#include "TangZeLock.h"
#include "CustomerPara.h"

#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)

/*******************Variable**********************/	
TangZeLockInterface_type	TangZeLockInterface	;


/****************************************************/
/**********************function**********************/
/*******************************************************************************
* ?????��???  		:
* ?????��???��?��    	:?��?��???����?13???????����?13  1:	1200us��???����?13,400us????����?13
				?????��???��????��?????��??1??o?��?������?????��???
* ��?��?����?????��      : ?��?
* ��?��??o??��???      : ?��?
* ��?��????�?        : ?��?
*******************************************************************************/
void SendTangZeLockInterfaceCode(void)
{
  if(TangZeLockInterface.counter_tojudgelevel & U8_BIT0)	//?????��??o????����?13????��???��??o��???����?13
  {
  	  if(True == IsReachTangZeLockChangeLeveltime(TangZeLockInterface.timer_target[TangZeLockInterface.counter_tojudgelevel]) ) 	
	  {
			GPIO_SetBits(Port_TZLI,Pin_TZLI);		//��?��??o��???����?13
			TangZeLockInterface.counter_tojudgelevel ++ ;
	  }
	  else
		  ;
  }
  else 
  {
	  if(True == IsReachTangZeLockChangeLeveltime(TangZeLockInterface.timer_target[TangZeLockInterface.counter_tojudgelevel]) ) 	
	  {
			GPIO_ResetBits(Port_TZLI,Pin_TZLI);		//��?��??o????����?13
			TangZeLockInterface.counter_tojudgelevel ++ ;
	  }
	  else
		  ;
  } 
  if(TangZeLockInterface.counter_tojudgelevel > (uint8_t)(Strlen_TZLI_Code * 8 * 2 + 2 - 1 ))//????????�?????�??a??��????????��??�����????
  {
	  LoadTZLICode();
	  LoadTZLITargetTime();
  }
}
/*******************************************************************************
* ?????��???  		:
* ?????��???��?��    	: ??��?�C-??��??|??�㨨???????��?����?13????��?������??1 ?|??????�㨨????����?�訨????��?1???����?��?????��?�C-??��???
* ��?��?����?????��      : ?��?
* ��?��??o??��???      : ?��?
* ��?��????�?        : ?��?
*******************************************************************************/
FlagStatus IsReachTangZeLockChangeLeveltime(uint16_t targettimer)
{
	if( TangZeLockInterface.Data2Level.Level.timer_temp >= targettimer)
	{
		TangZeLockInterface.Data2Level.Level.timer_temp = 0 ;
		return True ;
	}
	else
		return False ;
}

/*******************************************************************************
* ?????��???  		:
* ?????��???��?��    	:
* ��?��?����?????��      : ?��?
* ��?��??o??��???      : ?��?
* ��?��????�?        : ?��?
*******************************************************************************/
void LoadTZLITargetTime(void)
{
	uint8_t temp = 0 ;
	TangZeLockInterface.timer_target[0] = (uint8_t)(400/100) ;
	TangZeLockInterface.timer_target[1] = (uint8_t)(12400/100) ;	
	for(uint8_t i = 0 ;i< Strlen_TZLI_Code ;i++)
	{
		temp = TangZeLockInterface.datacode[i];
		for(uint8_t j = 0 ;j< 8;j++)
		{
			if(temp & U8_BIT7)
			{
				TangZeLockInterface.timer_target[(8*i+j)*2+2] = (uint8_t)(1200/100) ;
				TangZeLockInterface.timer_target[(8*i+j)*2+3] = (uint8_t)(400/100) ;
			}
			else
			{
				TangZeLockInterface.timer_target[(8*i+j)*2+2] =  (uint8_t)(400/100) ;
				TangZeLockInterface.timer_target[(8*i+j)*2+3] =  (uint8_t)(1200/100) ;
			}
			temp <<= 1 ;
		}		
	}
	TangZeLockInterface.counter_tojudgelevel = 0 ;
}

/*******************************************************************************
* ?????��???  		:
* ?????��???��?��    	: ??��???20bit??o0 ???4??abit??o???��??����??�????16??a???��????��?????????���̡�??��?���??��3??abyte?????��???
* ��?��?����?????��      : ?��?
* ��?��??o??��???      : ?��?
* ��?��????�?        : ?��?
*******************************************************************************/
void LoadTZLICode(void)
{
	TangZeLockInterface.datacode[0] = 0X00 ;
	TangZeLockInterface.datacode[1] = 0X00 ;	
	TangZeLockInterface.datacode[2] = TangZeLockInterface.keyvalue ;
}

	
	
#endif
#endif
