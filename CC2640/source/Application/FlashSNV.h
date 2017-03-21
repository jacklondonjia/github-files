#ifndef __FLASHSNV_H
#define __FLASHSNV_H
#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

#include "globalvariables.h"
#include "_Parameter.h"
#include "TypeDefines.h"

#include "bcomdef.h"

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
//Blank Data
#define DATA_FLASH_BLANK				0xffffffff
#define ADDR_FLASH_PAGE_HEAD			(0x0001d030)//(0x08007c00)	 

	 
#define		Commu_Mode_RecArrNum					1
#define		Gear_Ratio_Motor_RecArrNum				2
#define		Gear_Ratio_Wheel_RecArrNum				3
#define		Number_Pole_Pairs_RecArrNum				4
#define		Wheel_Diameter_Inch_Int_RecArrNum		5
#define		AH_Battery_RecArrNum					6
#define		T2CFuncition_RecArrNum					7
#define		Wheel_Diameter_Inch_Dec_RecArrNum		8
#define		Battery_Materials_RecArrNum				9
#define		Battery_SectionNum_RecArrNum			10
#define		BatteryStandardKM_RecArrNum				11
#define		Rated_Q_Release_PerFrame_A_RecArrNum	12
#define		Anti_theftDevice_Function_RecArrNum		13
#define		Mileage_Max_Byte1_RecArrNum				14
#define		Mileage_Max_Byte2_RecArrNum				15
#define		Mileage_Max_Byte3_RecArrNum				16
#define		Mileage_Max_Byte4_RecArrNum				17	 

#define 	BLE_NVID_CUST_BLE 						BLE_NVID_CUST_START
#define 	CUST_BLE_LEN 							32//16//4 x 32-bit values
/*********************************************************************
 * TYPEDEFS
 */
typedef enum
{
  Status_FlashProtect_Non  =0 ,
  Status_FlashProtected
}StatusFlashProtect;


typedef struct
{
	uint32_t q_max;
	uint32_t mileage_max ;
	uint16_t timer_day_guard_timeleft;
	uint8_t timer_hour_guard_timeleft ;
	uint8_t timer_minute_guard_timeleft ;
	union
	{
		uint8_t battery_materialsAndmanufact ;
		struct
		{	 
			uint8_t battery_materials :	3 ;
			uint8_t manufacturer_MnNiCo :	5 ;
		};
	};
	uint8_t battery_section_num ;
	uint8_t battery_aH_battery ;
	union
	{
		uint8_t status ;
		struct
		{
			uint8_t bit_autoguard : 	1 ;
			uint8_t bit_GPSguard : 		1 ;
			uint8_t	bit_reserved1 :		2 ;
			uint8_t bit_dousupport :	1 ;
			uint8_t	bit_reserved2 :		3 ;
		};
	};
	uint8_t wheel_diameter_inch_int ;	//整数
	uint8_t wheel_diameter_inch_dec	;	//小数
	uint8_t gear_ratio_motor ;
	uint8_t gear_ratio_wheel ;
	uint8_t number_pole_pairs ;
	uint8_t batterystandardKM ;
	uint8_t rated_Q_release_perFrame_A ;
	uint8_t odometer_2 ;		// odometer  3byte
	uint8_t odometer_1;
	uint8_t odometer_0 ;
	uint8_t reserve_1 ;		
	uint8_t reserve_2;
	uint8_t reserve_3 ;
	uint8_t reserve_4 ;
	uint8_t reserve_5 ;
	uint8_t reserve_6 ;

}FlashStruct_TypeDef ;


typedef union
{
	uint8_t bit8[32];
	FlashStruct_TypeDef	FlashStruct	;
}UnionFlashStruct_TypeDef;

typedef struct
{
	uint32_t addr_flash_read;
  	UnionFlashStruct_TypeDef data_flash_read;
	uint32_t addr_flash_write;
	UnionFlashStruct_TypeDef data_flash_write;
	FlagStatus status_higerthanFlashProtectSchmidt_once ;
	//StatusFlashProtect status_flash_protect ;
}Flash_TypeDef;

typedef struct
{
	uint32_t q_max;
	uint32_t mileage_max ;
	uint16_t timer_day_guard_timeleft;
	uint8_t timer_hour_guard_timeleft ;
	uint8_t timer_minute_guard_timeleft ;
	union
	{
		uint8_t battery_materialsAndmanufact ;
		struct
		{	 
			uint8_t battery_materials :	3 ;
			uint8_t manufacturer_MnNiCo :	5 ;
		};
	};
	uint8_t battery_section_num ;
	uint8_t battery_aH_battery ;
	union
	{
		uint8_t status ;
		struct
		{
			uint8_t bit_autoguard : 	1 ;
			uint8_t bit_GPSguard : 		1 ;
			uint8_t	bit_reserved1 :		2 ;
			uint8_t bit_dousupport :	1 ;
			uint8_t	bit_reserved2 :		3 ;
		};
	};
	uint8_t wheel_diameter_inch_int ;	//整数
	uint8_t wheel_diameter_inch_dec	;	//小数
	uint8_t gear_ratio_motor ;
	uint8_t gear_ratio_wheel ;
	uint8_t number_pole_pairs ;
	uint8_t batterystandardKM ;
	uint8_t rated_Q_release_perFrame_A ;
	uint8_t odometer_2 ;		// odometer  3byte
	uint8_t odometer_1;
	uint8_t odometer_0 ;
	uint8_t reserve_1 ;
	uint8_t reserve_2 ;
	uint8_t reserve_3 ;
	uint8_t reserve_4 ;
	uint8_t reserve_5 ;
	uint8_t reserve_6 ;
}FlashInitial_TypeDef ;

typedef struct
{
	union
	{
		uint8_t battery_materialsAndmanufact ;
		struct
		{	 
			uint8_t battery_materials :	3 ;
			uint8_t manufacturer_MnNiCo :	5 ;
		};
	};
	uint8_t battery_section_num ;
	uint8_t battery_aH_battery ;
	union
	{
		uint8_t status ;
		struct
		{
			uint8_t bit_autoguard : 	1 ;
			uint8_t bit_GPSguard : 		1 ;
			uint8_t	bit_reserved1 :		2 ;
			uint8_t bit_dousupport :	1 ;
			uint8_t	bit_reserved2 :		3 ;
		};
	};
	uint8_t wheel_diameter_inch_int ;	
	uint8_t wheel_diameter_inch_dec	;	
	uint8_t gear_ratio_motor ;
	uint8_t gear_ratio_wheel ;
	uint8_t number_pole_pairs ;
	uint8_t batterystandardKM ;
	uint8_t rated_Q_release_perFrame_A ;
}DevicePara_TypeDef ;


typedef union
{	
	uint8_t data[21];
	struct
	{
		uint8_t month_arr1 ;
		uint8_t month_arr2 ;
		uint8_t month_arr3 ;
		uint8_t reserved_1 ;
		uint8_t	date_arr1 ;
		uint8_t	date_arr2 ;
		uint8_t reserved_2 ;
		uint8_t year_arr1 ;
		uint8_t year_arr2 ;
		uint8_t year_arr3 ;
		uint8_t year_arr4 ;
		uint8_t reserved_3 ;
		uint8_t hour_arr1 ;
		uint8_t hour_arr2 ;
		uint8_t colon_1 ;
		uint8_t minute_arr1 ;
		uint8_t minute_arr2 ;
		uint8_t colon_2 ;
		uint8_t second_arr1 ;
		uint8_t second_arr2 ;
		uint8_t	end ;
	};
}HexTime_UnionTypeDef ;

/**
 * Send notify message Call back definition
 */
typedef void (*flashOperationMessage_t)(uint8_t action);

typedef struct
{
	flashOperationMessage_t        pfnNotifyCb;  // Called when characteristic value changes
} flashOperationMessageCBs_t;


typedef enum
{
	Flash_Protection  = 0 ,
	Flash_READ,
	Flash_WRITE,
}FlashActionState;

/*********************************************************************
 * Variables
 */
extern Flash_TypeDef Flash_CC ;
extern DevicePara_TypeDef DevicePara ;
extern const FlashInitial_TypeDef FlashInitial;
/*********************************************************************
 * FUNCTIONS
 */
/**
 * Read SNV Flash memory data and assign value to variables
 * The function need to be called in a ICall_registerApp Task
 */
extern uint8 ReadFlash();
/**
 * Write variables data to SNV Flash memory
 * The function need to be called in a ICall_registerApp Task
 */
extern uint8 WriteFlash();
/**
 * Self Flash test by write then read data
 * The function need to be called in a ICall_registerApp Task
 * @return  0 if successful.
 *          Otherwise, failure.
 */
extern uint8 WriteInitialValueAndReadOut(void);
extern void FlashProtectWhenLVLow(void);
extern void FlashProtectionAction(void);
#if(Commu_Mode_Common != Commu_Mode)
FlagStatus IsUpdateQmaxInFlash(void);
#endif

FlagStatus IsUpdateMileageMaxInFlash(void) ;
FlagStatus IsUpdateOdometerInFlash(void) ;
FlagStatus IsUpdateDeadLineInFlash(void);
FlagStatus IsUpdateElectLockInFlash(void) ;
FlagStatus IsUpdateBatteryParaInFlash(void) ;
FlagStatus IsUpdateWheelDiameterInchInFlash(void) ;
FlagStatus IsUpdateGearRatioInFlash(void) ;
FlagStatus IsUpdatePolePairsNumberInFlash(void) ;
FlagStatus IsUpdateRatedQReleasePerFrameAInFlash(void) ;

#if GPS_COMPILE
FlagStatus IsUpdateGPSLockStatusInFlash(void) ;
#endif

#if(ElectricDouSupport == Anti_theftDevice_Function)
FlagStatus IsUpdateDouSupportStatusInFlash(void) ;
#endif

void UpdataDataFlashWrite(void);

extern void ReadFlashInitial(void);
extern FlagStatus IsDeadLineReachInFlash(void);

extern bStatus_t FlashOperationMessage_RegisterAppCBs( flashOperationMessageCBs_t *appCallbacks );
void NotifyFlashAction(const uint8_t action);

extern void UpdateDeviceInformation(uint8_t* para) ;
extern void WriteParaToFlash(void) ;
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
