//#include "stdint.h"
//#include <string.h>
//#include "FlashSNV.h"
#include "osal_snv.h"
#include "CustomerPara.h"
//#include "TypeDefines.h"
//#include "Defines.h"
//#include "_Parameter.h"
//#include "globalvariables.h"
//#include "ATcontrol.h"
//#include "BatteryPowerCal.h"
//#include "Guard.h"
//#include "Onelinecommunica.h"
//#include "MileageCal.h"
//#include "ErrorHistory.h"
//#include "FreeKeyAndMechaLock.h"
//#include "TangZeLock.h"
//#include "PeriodicTimer.h"
//#include "_Annotate.h"
//#include "PeriodicTimer.h"
//#include "Tool_Functions.h"
//#include "FlashSNV.h"

static Flash_TypeDef Flash_CC ;
DevicePara_TypeDef DevicePara  ;

//#if(Customer_Type == Manufactor_Ebike)
const FlashInitial_TypeDef FlashInitial = //@ ADDR_FLASH_PAGE_HEAD=
{
	//4bytes
	Q_Max_InFlash,
	//4bytes
	Mileage_Max_InFlash,
	
	//4bytes
	Deadline_Day_InFlash,	//deadline day			
	Deadline_Hour_InFlash,	//deadline	hour		23小时
	Deadline_Minu_InFlash,	//deadline	minute		59分钟
	
	//4bytes
	Battery_Materials_InFlash	,		//Battery_Materials_InFlash
#if(Battery_Materials_InFlash == Lead_Acid_Batteries)
	Lead_Acid_Battery_Section_Num  ,		//
#else
	MnNiCo_Battery_Section_Num ,
#endif
	AH_Battery_InFlash ,
	(uint8_t)0X00,		//guard.auto	DouSupport:Retract GPSLockStaus:UnLock

	//4bytes
	Wheel_Diameter_Inch_Int_InFlash ,	//整数,
	Wheel_Diameter_Inch_Dec_InFlash	,	//小数
	Gear_Ratio_Motor_InFlash ,
	Gear_Ratio_Wheel_InFlash ,

	//4bytes
	Number_Pole_Pairs_InFlash ,
	BatteryStandardKM_InFlash ,
	Rated_Q_Release_PerFrame_A_InFlash ,
	0,
	
	//4bytes
	0,
	0,
	0,
	0,
	
	//4bytes
	0,
	0,
	0,
	0,
};


//#else
//const FlashInitial_TypeDef FlashInitial =
//{
//	Q_Max_InFlash,
//	Mileage_Max_InFlash,
//	0,	//deadline day
//	0,	//deadline	hour
//	0,	//deadline	minute
//	0,	//status_blank1
//	0,	//status_blank2
//	0,	//status_blank3
//	0X01,	//guard.auto 	DouSupport:Retract
//};
//#endif

//__root __no_init  uint8_t flash_unused_lastblock[1024 - sizeof(FlashStruct_TypeDef)] @(ADDR_FLASH_PAGE_HEAD + sizeof(FlashStruct_TypeDef));

static flashOperationMessageCBs_t *pAppCBs = NULL;
/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
uint8 ReadFlash()
{
	if(SUCCESS != osal_snv_read(BLE_NVID_CUST_BLE, CUST_BLE_LEN, Flash_CC.data_flash_read.bit8))
	{
		return WriteInitialValueAndReadOut();
	}
	return SUCCESS;

}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
uint8 WriteFlash()
{
	return osal_snv_write(BLE_NVID_CUST_BLE, CUST_BLE_LEN, Flash_CC.data_flash_write.bit8);
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
uint8 WriteInitialValueAndReadOut(void)
{
	osal_snv_write(BLE_NVID_CUST_BLE, CUST_BLE_LEN, (void*)&FlashInitial);
	osal_snv_read(BLE_NVID_CUST_BLE, CUST_BLE_LEN, Flash_CC.data_flash_read.bit8);

	return memcmp(Flash_CC.data_flash_read.bit8, &FlashInitial, CUST_BLE_LEN);
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
void GetStatusLVadcHighThanLVFlashProtectSchmidtOnce(void)
{
	if(Lead_Acid_Batteries ==  DevicePara.battery_materialsAndmanufact)
	{
		if(LV.avr_adc > Lead_Acid_LV_FlashProtect_Schmidt_D)
			 Flash_CC.status_higerthanFlashProtectSchmidt_once = True ;

	}
	else
	{
		uint16_t temp ;
		if(True == IsBatteryNumConventional())
			temp = MnNiCo_LV_FlashProtect_Schmidt_Con_D ;
		else
			temp = MnNiCo_LV_FlashProtect_Schmidt_UnCon_D ;
		if(LV.avr_adc > temp)
			Flash_CC.status_higerthanFlashProtectSchmidt_once = True ;			
	}
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsLVadcLowerThanLVFlashProtect(void)
{
	if(Lead_Acid_Batteries ==  DevicePara.battery_materialsAndmanufact)
	{
		if(True == Flash_CC.status_higerthanFlashProtectSchmidt_once )
		{
			if(LV.avr_adc < Lead_Acid_LV_FlashProtect_D)
			{
				Flash_CC.status_higerthanFlashProtectSchmidt_once = False ;	
				return True ;
			}
			else
				return False ;
		}
		else 
			return False ;
	}
	else
	{
		if(True == Flash_CC.status_higerthanFlashProtectSchmidt_once )
		{
			uint16_t temp ;
			if(True == IsBatteryNumConventional())
				temp = MnNiCo_LV_FlashProtect_Con_D ;
			else
				temp = MnNiCo_LV_FlashProtect_UnCon_D ;
			if(LV.avr_adc < temp)
			{
				Flash_CC.status_higerthanFlashProtectSchmidt_once = False ;	
				return True ;
			}
			else
				return False ;
		}
		else
			return False ;
	}
}


/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
void FlashProtectWhenLVLow(void)
{
	GetStatusLVadcHighThanLVFlashProtectSchmidtOnce();
	if(True == IsLVadcLowerThanLVFlashProtect())
		NotifyFlashAction(Flash_Protection);



}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
/*void FlashProtectWhenLVLow(void)
{
	if(Lead_Acid_Batteries ==  DevicePara.battery_materials)
	{
		if(Status_FlashProtect_Non == Flash_CC.status_flash_protect)
		{
			if(LV.avr_adc < Lead_Acid_LV_FlashProtect_D)
			{
				NotifyFlashAction(Flash_Protection);
				Flash_CC.status_flash_protect = Status_FlashProtected ;
			}
			else
				;
		}
		else
		{
			if(LV.avr_adc > Lead_Acid_LV_FlashProtect_D + 100) //TAG: hysteresis
				Flash_CC.status_flash_protect = Status_FlashProtect_Non ;
			else
				;
		}
	}
	else if(MnNiCo_Ternary_Battery ==  DevicePara.battery_materials)
	{
		if(Status_FlashProtect_Non == Flash_CC.status_flash_protect)
		{
			if(LV.avr_adc < MnNiCo_LV_FlashProtect_D)
			{
				NotifyFlashAction(Flash_Protection);
				Flash_CC.status_flash_protect = Status_FlashProtected ;
			}
			else
				;
		}
		else
		{
			if(LV.avr_adc > MnNiCo_LV_FlashProtect_D + 100) //TAG: hysteresis
				Flash_CC.status_flash_protect = Status_FlashProtect_Non ;
			else
				;
		}

	}
	else
		;
		
}
*/
/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsFlashDataDiffer(void)
{
#if(Commu_Mode_Common == Commu_Mode)
	if(YES == IsUpdateMileageMaxInFlash() ||YES == IsUpdateDeadLineInFlash() ||YES == IsUpdateElectLockInFlash())
#else 
	if(YES == IsUpdateQmaxInFlash() || YES == IsUpdateOdometerInFlash()\
		||YES == IsUpdateMileageMaxInFlash()\
		||YES == IsUpdateDeadLineInFlash() || YES == IsUpdateElectLockInFlash()\
		|| YES == IsUpdateBatteryParaInFlash()||YES == IsUpdateWheelDiameterInchInFlash() \
		||YES == IsUpdateGearRatioInFlash()||YES == IsUpdatePolePairsNumberInFlash()\
		||YES == IsUpdateRatedQReleasePerFrameAInFlash())
		return YES ;

	#if GPS_COMPILE	
	if(YES == IsUpdateGPSLockStatusInFlash())
		return YES ;
	#endif
	
	#if(ElectricDouSupport == Anti_theftDevice_Function)
	if(YES == IsUpdateDouSupportStatusInFlash() )
		return YES ;
	#endif			
#endif

	return NO ;
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
void WriteParaToFlash(void)
{
	ReadFlash();
	if(YES== IsFlashDataDiffer())
			WriteFlash();
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
void FlashProtectionAction(void)
{
	ReadFlash();
#if(Commu_Mode_Common == Commu_Mode)
	if(YES == IsUpdateMileageMaxInFlash() ||YES == IsUpdateDeadLineInFlash() ||YES == IsUpdateElectLockInFlash())
#elif(ElectricDouSupport == Anti_theftDevice_Function)
	if(YES == IsUpdateQmaxInFlash() || YES == IsUpdateMileageMaxInFlash()\
			||YES == IsUpdateDeadLineInFlash() || YES == IsUpdateElectLockInFlash()\
			||YES == IsUpdateDouSupportStatusInFlash() )
#else
	if(YES == IsUpdateQmaxInFlash() || YES == IsUpdateMileageMaxInFlash()\
			||YES == IsUpdateDeadLineInFlash() || YES == IsUpdateElectLockInFlash())
#endif
		WriteFlash();
	else
		;
}
#if(Commu_Mode_Common != Commu_Mode)
/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateQmaxInFlash(void)
{
	if(Flash_CC.data_flash_read.FlashStruct.q_max< Q.data_max.sum32)
	{
		//if((Q.data_max.sum32 - Flash_CC.data_flash_read.sum32[0]) > Q_current_max_Flash_Dleta_MAX)
		if((Q.data_max.sum32 - Flash_CC.data_flash_read.FlashStruct.q_max) > Q_current_max_Flash_Dleta_MAX)
		{
			Flash_CC.data_flash_write.FlashStruct.q_max = Q.data_max.sum32 ;
			return YES ;
		}
		else
			;
	}
	else
	{
		if((Flash_CC.data_flash_read.FlashStruct.q_max - Q.data_max.sum32) > Q_current_max_Flash_Dleta_MAX)
		{
			Flash_CC.data_flash_write.FlashStruct.q_max = Q.data_max.sum32 ;
			return YES ;
		}
		else
			;
	}
	return NO ;
}
#endif
/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateMileageMaxInFlash(void)
{
	if(Flash_CC.data_flash_read.FlashStruct.mileage_max < Mileage.counter_hallchange_max.sum32)
	{
		if((Mileage.counter_hallchange_max.sum32 - Flash_CC.data_flash_read.FlashStruct.mileage_max) > Mileage_max_Flash_Dleta_MAX)
		{
			Flash_CC.data_flash_write.FlashStruct.mileage_max = Mileage.counter_hallchange_max.sum32 ;
			return YES ;
		}
		else
			;
	}
	else
	{
		if((Flash_CC.data_flash_read.FlashStruct.mileage_max - Mileage.counter_hallchange_max.sum32) > Mileage_max_Flash_Dleta_MAX)
		{
			Flash_CC.data_flash_write.FlashStruct.mileage_max = Mileage.counter_hallchange_max.sum32 ;
			return YES ;
		}
		else
			;
	}
	return NO ;
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateOdometerInFlash(void)
{
	if(Flash_CC.data_flash_read.FlashStruct.odometer_0 != Mileage.odometer_perKM.bit8[0])
	{
		Flash_CC.data_flash_write.FlashStruct.odometer_0 = Mileage.odometer_perKM.bit8[0] ;
		return YES ;
	}
	if(Flash_CC.data_flash_read.FlashStruct.odometer_1 != Mileage.odometer_perKM.bit8[1])
	{
		Flash_CC.data_flash_write.FlashStruct.odometer_1 = Mileage.odometer_perKM.bit8[1] ;
		return YES ;
	}
	if(Flash_CC.data_flash_read.FlashStruct.odometer_2 != Mileage.odometer_perKM.bit8[2])
	{
		Flash_CC.data_flash_write.FlashStruct.odometer_2 = Mileage.odometer_perKM.bit8[2] ;
		return YES ;
	}
	return NO ; 
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateDeadLineInFlash(void)
{
	if(Flash_CC.data_flash_read.FlashStruct.timer_day_guard_timeleft == Guard.timer_day_guard_timeleft.sum\
		&& Flash_CC.data_flash_read.FlashStruct.timer_hour_guard_timeleft == Guard.timer_hour_guard_timeleft\
		&& Flash_CC.data_flash_read.FlashStruct.timer_minute_guard_timeleft == Guard.timer_minute_guard_timeleft)				
		return NO ;
	else
	{
		Flash_CC.data_flash_write.FlashStruct.timer_day_guard_timeleft = Guard.timer_day_guard_timeleft.sum ;
		Flash_CC.data_flash_write.FlashStruct.timer_hour_guard_timeleft = Guard.timer_hour_guard_timeleft ;
		Flash_CC.data_flash_write.FlashStruct.timer_minute_guard_timeleft = Guard.timer_minute_guard_timeleft ;
		return YES ;
	}
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateElectLockInFlash(void)
{
	if(Flash_CC.data_flash_read.FlashStruct.bit_autoguard)		//32位状态的第一个字节为电子锁
	{
		if(Status_Auto != Guard.status_autoguard)
		{
			Flash_CC.data_flash_write.FlashStruct.bit_autoguard = 0 ;
			return YES ;
		}
		return NO ;
	}
	else
	{
		if(Status_Auto == Guard.status_autoguard)
		{
			Flash_CC.data_flash_write.FlashStruct.bit_autoguard = 1 ;
			return YES	;
		}
		return NO ;
	}
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateBatteryParaInFlash(void)
{	
	if(Flash_CC.data_flash_read.FlashStruct.battery_materialsAndmanufact!= DevicePara.battery_materialsAndmanufact)		//第一个字节为轮径整数
	{
		Flash_CC.data_flash_write.FlashStruct.battery_materialsAndmanufact= DevicePara.battery_materialsAndmanufact ;
		return YES;
	}
	if(Flash_CC.data_flash_read.FlashStruct.battery_section_num != DevicePara.battery_section_num)
	{
		Flash_CC.data_flash_write.FlashStruct.battery_section_num = DevicePara.battery_section_num ;
		return YES ;
	}
	if(Flash_CC.data_flash_read.FlashStruct.battery_aH_battery != DevicePara.battery_aH_battery)
	{
		Flash_CC.data_flash_write.FlashStruct.battery_aH_battery = DevicePara.battery_aH_battery ;
		return YES ;
	}
	if(Flash_CC.data_flash_read.FlashStruct.batterystandardKM != DevicePara.batterystandardKM)
	{
		Flash_CC.data_flash_write.FlashStruct.batterystandardKM = DevicePara.batterystandardKM ;
		return YES ;
	}
	return NO;  
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateWheelDiameterInchInFlash(void)
{	
	if(Flash_CC.data_flash_read.FlashStruct.wheel_diameter_inch_int != DevicePara.wheel_diameter_inch_int )		
	{
		Flash_CC.data_flash_write.FlashStruct.wheel_diameter_inch_int = DevicePara.wheel_diameter_inch_int ;
		return YES;
	}
	if(Flash_CC.data_flash_read.FlashStruct.wheel_diameter_inch_dec != DevicePara.wheel_diameter_inch_dec)
	{
		Flash_CC.data_flash_write.FlashStruct.wheel_diameter_inch_dec = DevicePara.wheel_diameter_inch_dec ;
		return YES ;  
	}
	return NO ;
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateGearRatioInFlash(void)
{	
	if(Flash_CC.data_flash_read.FlashStruct.gear_ratio_motor != DevicePara.gear_ratio_motor)	
	{
		Flash_CC.data_flash_write.FlashStruct.gear_ratio_motor = DevicePara.gear_ratio_motor ;
		return YES ;
	}
	if(Flash_CC.data_flash_read.FlashStruct.gear_ratio_wheel != DevicePara.gear_ratio_wheel)
	{
		Flash_CC.data_flash_write.FlashStruct.gear_ratio_wheel = DevicePara.gear_ratio_wheel ;
		return YES ;
	}
	return NO ;
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdatePolePairsNumberInFlash(void)
{	
	if(Flash_CC.data_flash_read.FlashStruct.number_pole_pairs != DevicePara.number_pole_pairs)
	{
		Flash_CC.data_flash_write.FlashStruct.number_pole_pairs = DevicePara.number_pole_pairs ;
		return YES;
	}
	return NO ; 
}

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateRatedQReleasePerFrameAInFlash(void)
{	
	if(Flash_CC.data_flash_read.FlashStruct.rated_Q_release_perFrame_A != DevicePara.rated_Q_release_perFrame_A)
	{
		Flash_CC.data_flash_write.FlashStruct.rated_Q_release_perFrame_A = DevicePara.rated_Q_release_perFrame_A ;
		return YES;
	}
	return NO ;  
}


#if GPS_COMPILE
/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateGPSLockStatusInFlash(void)
{
	if(Flash_CC.data_flash_read.FlashStruct.bit_GPSguard)
	{
		if(Lock_ByGPRS != Guard.cmd_lock_From_GPRS)
		{
			Flash_CC.data_flash_write.FlashStruct.bit_GPSguard  = 0;
			return YES ; 
		}
		return NO ;		
	}
	else
	{
		if(Lock_ByGPRS == Guard.cmd_lock_From_GPRS)
		{
			Flash_CC.data_flash_write.FlashStruct.bit_GPSguard  = 1;
			return YES ;
		}
		return NO ; 
	}
}
#endif



#if(ElectricDouSupport == Anti_theftDevice_Function)
/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
FlagStatus IsUpdateDouSupportStatusInFlash(void)
{
	//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
	if(Flash_CC.data_flash_read.FlashStruct.bit_dousupport)		//32位状态的第一个字节为电子锁
	{
		if(Status_Fallen != DouSupport.status && Status_FallingAction == DouSupport.status && \
		Status_FallOuttime == DouSupport.status && Status_FallOCP == DouSupport.status)
		{
			Flash_CC.data_flash_write.FlashStruct.bit_dousupport = 0 ;
			return YES ;
		}
		return NO ;
			
	}
	else
	{
		if(Status_Fallen == DouSupport.status || Status_FallingAction == DouSupport.status || \
		Status_FallOuttime == DouSupport.status || Status_FallOCP == DouSupport.status)
		{
			Flash_CC.data_flash_write.FlashStruct.bit_dousupport = 1 ;
			return YES ;
		}
		return NO ;

	}
}
#endif

/*******************************************************************************
* 函数名        : 
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None

* 返回值        : None
*******************************************************************************/
void UpdateDeviceInformation(uint8_t* para)
{
	DevicePara.battery_materialsAndmanufact = *(para + Battery_Materials_RecArrNum) ;
	DevicePara.battery_section_num = 	*(para + Battery_SectionNum_RecArrNum) ;
	DevicePara.battery_aH_battery = 	*(para + AH_Battery_RecArrNum) ;
	DevicePara.wheel_diameter_inch_int =*(para + Wheel_Diameter_Inch_Int_RecArrNum) ;
	DevicePara.wheel_diameter_inch_dec =*(para + Wheel_Diameter_Inch_Dec_RecArrNum) ;
	DevicePara.gear_ratio_motor = 		*(para + Gear_Ratio_Motor_RecArrNum) ;
	DevicePara.gear_ratio_wheel = 		*(para + Gear_Ratio_Wheel_RecArrNum) ;
	DevicePara.number_pole_pairs = 		*(para + Number_Pole_Pairs_RecArrNum) ;
	DevicePara.batterystandardKM = 		*(para + BatteryStandardKM_RecArrNum) ;
	DevicePara.rated_Q_release_perFrame_A = *(para + Rated_Q_Release_PerFrame_A_RecArrNum) ;
	Mileage.counter_hallchange_max.bit8[3] =*(para + Mileage_Max_Byte1_RecArrNum) ;
	Mileage.counter_hallchange_max.bit8[2] = *(para + Mileage_Max_Byte2_RecArrNum) ;
	Mileage.counter_hallchange_max.bit8[1] =*(para + Mileage_Max_Byte3_RecArrNum) ;
	Mileage.counter_hallchange_max.bit8[0] = *(para + Mileage_Max_Byte4_RecArrNum) ;
	
	uint32_t cal_temp ;
	cal_temp = Current_Sigma_D_PerAH  ;
	cal_temp *= DevicePara.battery_aH_battery ;
	Q.data_max.sum32 = cal_temp ;
}

/*******************************************************************************
* 函数名  	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ReadFlashInitial(void)
{
	ReadFlash();
	
	Q.data_max.sum32= Flash_CC.data_flash_read.FlashStruct.q_max;
	Mileage.counter_hallchange_max.sum32 = Flash_CC.data_flash_read.FlashStruct.mileage_max;

	//read odometer in flash
	Mileage.odometer_perKM.bit8[3] = 0 ;
	Mileage.odometer_perKM.bit8[2] = Flash_CC.data_flash_read.FlashStruct.odometer_2 ;
	Mileage.odometer_perKM.bit8[1] = Flash_CC.data_flash_read.FlashStruct.odometer_1 ;
	Mileage.odometer_perKM.bit8[0] = Flash_CC.data_flash_read.FlashStruct.odometer_0 ;
		
	//read deadline
	Guard.timer_day_guard_timeleft.sum = Flash_CC.data_flash_read.FlashStruct.timer_day_guard_timeleft ;
	Guard.timer_hour_guard_timeleft = Flash_CC.data_flash_read.FlashStruct.timer_day_guard_timeleft ;
	Guard.timer_minute_guard_timeleft = Flash_CC.data_flash_read.FlashStruct.timer_minute_guard_timeleft ;
	Guard.timer_second_guard_timeleft = 0 ;
	if(Flash_CC.data_flash_read.FlashStruct.bit_autoguard)
		Guard.status_autoguard = Status_Auto ;
	else
		Guard.status_autoguard = Status_UnAuto ;

	if(Flash_CC.data_flash_read.FlashStruct.bit_GPSguard)
		Guard.cmd_lock_From_GPRS = Lock_ByGPRS ;
	else
		Guard.cmd_lock_From_GPRS = Unlock_ByGPRS ;
		
	if(Flash_CC.data_flash_read.FlashStruct.bit_dousupport)
		DouSupport.status = Status_Fallen ;
	else
		DouSupport.status = Status_Retracted ;
		
	DevicePara.battery_materialsAndmanufact= 		Flash_CC.data_flash_read.FlashStruct.battery_materialsAndmanufact;
	DevicePara.battery_section_num = 	Flash_CC.data_flash_read.FlashStruct.battery_section_num ;
	DevicePara.battery_aH_battery = 	Flash_CC.data_flash_read.FlashStruct.battery_aH_battery ;

	DevicePara.bit_autoguard = Flash_CC.data_flash_read.FlashStruct.bit_autoguard ;
	DevicePara.bit_GPSguard = Flash_CC.data_flash_read.FlashStruct.bit_GPSguard;
	DevicePara.bit_dousupport= Flash_CC.data_flash_read.FlashStruct.bit_dousupport ;	
		
	DevicePara.wheel_diameter_inch_int = Flash_CC.data_flash_read.FlashStruct.wheel_diameter_inch_int;
	DevicePara.wheel_diameter_inch_dec = Flash_CC.data_flash_read.FlashStruct.wheel_diameter_inch_dec;	
	DevicePara.gear_ratio_motor = 		Flash_CC.data_flash_read.FlashStruct.gear_ratio_motor ;
	DevicePara.gear_ratio_wheel = 		Flash_CC.data_flash_read.FlashStruct.gear_ratio_wheel ;
	DevicePara.number_pole_pairs = 		Flash_CC.data_flash_read.FlashStruct.number_pole_pairs ;
	DevicePara.batterystandardKM = 		Flash_CC.data_flash_read.FlashStruct.batterystandardKM ;
	DevicePara.rated_Q_release_perFrame_A = Flash_CC.data_flash_read.FlashStruct.rated_Q_release_perFrame_A ;
}


/*******************************************************************************
* 函数名  	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsDeadLineReachInFlash(void)
{
	if(0 != Flash_CC.data_flash_read.FlashStruct.timer_day_guard_timeleft)
		return NO ;
	if((0 != Flash_CC.data_flash_read.FlashStruct.timer_hour_guard_timeleft) && \
		(Flash_CC.data_flash_read.FlashStruct.timer_hour_guard_timeleft < 24))
		return NO ;
	if((0 != Flash_CC.data_flash_read.FlashStruct.timer_minute_guard_timeleft) &&\
		(Flash_CC.data_flash_read.FlashStruct.timer_minute_guard_timeleft < 60))
		return NO ;
	else
		return YES ;
}
/********************************************************************************
 * FlashOperationMessage_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 *********************************************************************************/
bStatus_t FlashOperationMessage_RegisterAppCBs( flashOperationMessageCBs_t *appCallbacks )
{
	  if ( appCallbacks )
	  {
	    pAppCBs = appCallbacks;

	    return ( SUCCESS );
	  }
	  else
	  {
	    return ( bleAlreadyInRequestedMode );
	  }
}
/*******************************************************************************
* 函数名  	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void NotifyFlashAction(const uint8_t action)
{
    if ( pAppCBs && pAppCBs->pfnNotifyCb)
      pAppCBs->pfnNotifyCb(action); // Call app function from stack task context.
}



