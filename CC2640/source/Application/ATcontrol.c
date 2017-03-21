#include <xdc/runtime/Log.h>
//#include "stdint.h"
//#include "stdio.h"
//#include <stdlib.h>
//#include <string.h>
#include "CustomerPara.h"
//#include "sendandstring.h"


/*************************************************/
/*********************Define **********************/
/**
 * constant strings
 */
/*****************HC-08 ATCmd*********************/
//const uint8_t Str_OK[] = 				{"OK"} ;
const uint8_t Str_ATNAME[] = 			{"AT+NAME="} ;

/*****************FreeATCmd**********************/
const uint8_t Str_ATBLECONNECT[] =		{"AT+BLEConnect"};
const uint8_t Str_ATDEVINFO[] = 		{"AT+DevInfo="}; 
const uint8_t Str_ATDEVINFOSET[] =		{0X04} ;
const uint8_t Str_ATDEADLINE[] = 		{"AT+DeadLine="};
const uint8_t Str_ATGUARDSTATUS[] = 	{"AT+GuardStatus="};
const uint8_t Str_ATERRORSTATUS[] = 	{"AT+ErrorStatus=?"};
const uint8_t Str_ATERRORHISTORY[] = 	{"AT+ErrorHistory="};
const uint8_t Str_ATELECTLOCK[] = 		{"AT+ElectLock="};
const uint8_t Str_ATQ[] = 				{"AT+Q=?"};
const uint8_t Str_ATMILEAGE[] = 		{"AT+Mileage=?"};
const uint8_t Str_ATHALLSPEED[] = 		{"AT+HallSpeed=?"};
const uint8_t Str_ATCONTRCONNECT[] = 	{"AT+ContrConnect=?"};
const uint8_t Str_ATHARDWAREVERSION[] ={"AT+HardwareVersion=?"};
const uint8_t Str_ATUNLOCKLEVEL1LV[] =	{"AT+UnlockLevel1LV=1"};
const uint8_t Str_ATONEKEYREPAIR[]= 	{"AT+OneKeyRepair=1"} ;
const uint8_t Str_ATThreeSpeed[]= 		{0X17} ;
const uint8_t Str_ATThreeTorque[]= 		{0X18} ;
const uint8_t Str_ATCntrlE2PROMConfig[]=		{0X19} ;
const uint8_t Str_ATSEARCHEBIKE[] =		{"AT+SearchEbike="};
const uint8_t Str_ATCYCLICSEND[] =		{"AT+CyclicSend="};
const uint8_t Str_ATCFILENUM[] =		{"AT+CFileNum="};

#if(External_Anti_theft != Anti_theftDevice_Function)
	const uint8_t Str_ATFREEKEYSTART[] =	{"AT+FreeKeyStart="};
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		const uint8_t Str_ATSEATLOCK[] =	{"AT+SeatLock="};
		const uint8_t Str_ATMECHALLOCK[]=	{"AT+MechalLock="} ;
		#if( ElectricDouSupport == Anti_theftDevice_Function)
		const uint8_t Str_ATDOUSUPPORT[]=	{"AT+DouSupport="} ;
		#endif
	#endif
#endif

#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
	const uint8_t Str_ATTESTMOTORLOCK[] =	{"AT+TESTMotoLock="};
	const uint8_t Str_ATTESTSEATLOCK[] =	{"AT+TESTSeatLock="};
	const uint8_t Str_ATTESTSHOCKALARM[] =	{"AT+TESTShockAlarm="};
#endif
	/**
	 * Length of string constants
	 */
	/*********************HC-08 ATCmd*******************/
	//#define Strlen_OK		              	(sizeof(Str_OK)/sizeof(Str_OK[0]) -1)
	#define Strlen_ATNAME               	(sizeof(Str_ATNAME)/sizeof(Str_ATNAME[0]) -1)

	/*****************FreeATCmd**********************/
	#define Strlen_ATBLECONNECT				(sizeof(Str_ATBLECONNECT)/sizeof(Str_ATBLECONNECT[0])-1)
	#define Strlen_ATDEVINFO				(sizeof(Str_ATDEVINFO)/sizeof(Str_ATDEVINFO[0])-1)
	#define Strlen_ATDEVINFOSET				1//(sizeof(Str_ATDEVINFOSET)/sizeof(Str_ATDEVINFOSET[0])-1)
	#define Strlen_ATDEADLINE				(sizeof(Str_ATDEADLINE)/sizeof(Str_ATDEADLINE[0])-1)
	#define Strlen_ATGUARDSTATUS			(sizeof(Str_ATGUARDSTATUS)/sizeof(Str_ATGUARDSTATUS[0])-1)
	#define Strlen_ATERRORSTATUS			(sizeof(Str_ATERRORSTATUS)/sizeof(Str_ATERRORSTATUS[0])-1)
	#define Strlen_ATERRORHISTORY			(sizeof(Str_ATERRORHISTORY)/sizeof(Str_ATERRORHISTORY[0])-1)
	#define Strlen_ATELECTLOCK				(sizeof(Str_ATELECTLOCK)/sizeof(Str_ATELECTLOCK[0])-1)
	#define Strlen_ATQ						(sizeof(Str_ATQ)/sizeof(Str_ATQ[0])-1)
	#define Strlen_ATMILEAGE				(sizeof(Str_ATMILEAGE)/sizeof(Str_ATMILEAGE[0])-1)
	#define Strlen_ATHALLSPEED				(sizeof(Str_ATHALLSPEED)/sizeof(Str_ATHALLSPEED[0])-1)
	#define Strlen_ATCONTRCONNECT			(sizeof(Str_ATCONTRCONNECT)/sizeof(Str_ATCONTRCONNECT[0])-1)
	#define Strlen_ATHARDWAREVERSION		(sizeof(Str_ATHARDWAREVERSION)/sizeof(Str_ATHARDWAREVERSION[0])-1)
	#define Strlen_ATUNLOCKLEVEL1LV			(sizeof(Str_ATUNLOCKLEVEL1LV)/sizeof(Str_ATUNLOCKLEVEL1LV[0])-1)
	#define Strlen_ATONEKEYREPAIR			(sizeof(Str_ATONEKEYREPAIR)/sizeof(Str_ATONEKEYREPAIR[0])-1)
	#define	Strlen_ATThreeSpeed				1//(sizeof(Str_ATThreeSpeed)/sizeof(Str_ATThreeSpeed[0])-1)
	#define	Strlen_ATThreeTorque			1//(sizeof(Str_ATThreeTorque)/sizeof(Str_ATThreeTorque[0])-1)
	#define	Strlen_ATCntrlE2PROMConfig		1//(sizeof(Str_ATCntrlE2PROMConfig)/sizeof(Str_ATCntrlE2PROMConfig[0])-1)
	#define	Strlen_ATSEARCHEBIKE			(sizeof(Str_ATSEARCHEBIKE)/sizeof(Str_ATSEARCHEBIKE[0])-1)
	#define Strlen_ATCYCLICSEND				(sizeof(Str_ATCYCLICSEND)/sizeof(Str_ATCYCLICSEND[0])-1)
	#define	Strlen_ATCFILENUM				(sizeof(Str_ATCFILENUM)/sizeof(Str_ATCFILENUM[0])-1)
	
	#if(External_Anti_theft != Anti_theftDevice_Function)
		#define Strlen_ATFREEKEYSTART				(sizeof(Str_ATFREEKEYSTART)/sizeof(Str_ATFREEKEYSTART[0])-1)

		#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
			#define	Strlen_ATSEATLOCK				(sizeof(Str_ATSEATLOCK)/sizeof(Str_ATSEATLOCK[0])-1)
			#define	Strlen_ATMECHALLOCK				(sizeof(Str_ATMECHALLOCK)/sizeof(Str_ATMECHALLOCK[0])-1)

			#if( ElectricDouSupport == Anti_theftDevice_Function)
			#define	Strlen_ATDOUSUPPORT				(sizeof(Str_ATDOUSUPPORT)/sizeof(Str_ATDOUSUPPORT[0])-1)
			#endif

		#endif

	#endif

	#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
		#define Strlen_ATTESTMOTORLOCK				(sizeof(Str_ATTESTMOTORLOCK)/sizeof(Str_ATTESTMOTORLOCK[0])-1)
		#define Strlen_ATTESTSEATLOCK				(sizeof(Str_ATTESTSEATLOCK)/sizeof(Str_ATTESTSEATLOCK[0])-1)
		#define Strlen_ATTESTSHOCKALARM				(sizeof(Str_ATTESTSHOCKALARM)/sizeof(Str_ATTESTSHOCKALARM[0])-1)
	#endif

/**
 * Compare string & response string function pairs
 */
const ResponseStrategy resp_strategy_table[] = {
		Str_ATBLECONNECT, 	Strlen_ATBLECONNECT, 	ResponseErrorATCmdConncetCmdSendRepeatedly,//,ResponseATBLEConnect
		Str_ATDEVINFO, 		Strlen_ATDEVINFO,		ResponseATDevInfo,
		Str_ATDEVINFOSET, 	Strlen_ATDEVINFOSET,	ResponseATDevInfoSet,
		Str_ATDEADLINE,		Strlen_ATDEADLINE,		ResponseATRentDeadline,
		Str_ATELECTLOCK, 	Strlen_ATELECTLOCK, 	ResponseATElectLock,
		Str_ATGUARDSTATUS, 	Strlen_ATGUARDSTATUS, 	ResponseATGuardStatus,
		Str_ATSEARCHEBIKE, 	Strlen_ATSEARCHEBIKE, 	ResponseATSearchEbike,
#if(External_Anti_theft != Anti_theftDevice_Function)
		Str_ATFREEKEYSTART, Strlen_ATFREEKEYSTART, 	ResponseFreeKeyStart,
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		Str_ATSEATLOCK, 	Strlen_ATSEATLOCK, 		ResponseSeatLock,
		#if(ElectricDouSupport == Anti_theftDevice_Function)
		Str_ATDOUSUPPORT, 	Strlen_ATDOUSUPPORT, 	ResponseDouSupport,
		#endif
	#endif
#endif
#if(Commu_Mode_Common != Commu_Mode)
		Str_ATERRORSTATUS, 	Strlen_ATERRORSTATUS, 	ResponseATErrorStatus,
		Str_ATQ, 			Strlen_ATQ, 			ResponseATQ,
		Str_ATERRORHISTORY, Strlen_ATERRORHISTORY, 	ResponseATErrorHistory,
	#if(Commu_Mode_TwoWire == Commu_Mode)
		#if(COMPILE_ENABLE == OneKeyRepairSet)
		Str_ATONEKEYREPAIR, Strlen_ATONEKEYREPAIR, 	ResponseOneKeyRepair,
		#endif
		
		#if(COMPILE_ENABLE == Double_LVSet)
		Str_ATUNLOCKLEVEL1LV, Strlen_ATUNLOCKLEVEL1LV, ResponseLV1LevelUnlockSet,
		#endif
		
		#if(COMPILE_ENABLE == ThreeSpeedSet)
		Str_ATThreeSpeed, 	Strlen_ATThreeSpeed, 	ResponseThreeSpeed,
		#endif
		
		#if(COMPILE_ENABLE == ThreeTorqueSet)
		Str_ATThreeTorque, 	Strlen_ATThreeTorque, 	ResponseThreeTorque,
		#endif
		
		Str_ATCntrlE2PROMConfig, Strlen_ATCntrlE2PROMConfig, ResponseControllerE2PROMConfig,
	#endif
#endif
		Str_ATMILEAGE, 		Strlen_ATMILEAGE, 		ResponseATMileage,
		Str_ATHALLSPEED, 	Strlen_ATHALLSPEED, 	ResponseATHallSpeed,
		Str_ATCONTRCONNECT, Strlen_ATCONTRCONNECT, 	ResponseConntrollerConnectCheck,
		Str_ATHARDWAREVERSION, Strlen_ATHARDWAREVERSION, ResponseHardwareVersion,
		Str_ATCFILENUM, 	Strlen_ATCFILENUM, 		ResponseCustomerFileNum,
#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
		Str_ATTESTPOWERLOCK, Strlen_ATTESTPOWERLOCK, ResponseTestPowerLock,
		Str_ATTESTMOTORLOCK, Strlen_ATTESTMOTORLOCK, ResponseTestMotorLock,
		Str_ATTESTSEATLOCK,	Strlen_ATTESTSEATLOCK, 	ResponseTestSeatLock,
		Str_ATTESTSHOCKALARM, Strlen_ATTESTSHOCKALARM, ResponseTestShockAlarm,
#endif
		Str_ATCYCLICSEND, 	Strlen_ATCYCLICSEND, 		ResponseATCyclicSend,
		Str_ATNAME, 		Strlen_ATNAME, 			ResponseAndSetChangeBLEModuleName
};

/*************************************************/
/*******************Variable**********************/
AT_TypeDef AT ;

/****************************************************/
/**********************function**********************/
/***********************************************
 * Main application is responsible for calling this function when
 * Disconnected from the BLE App
 **********************************************/
void processDisconnectWithBLEApp()
{
	CloseCyclicSend();
	AT.status_BLEconnect = False;
	/*当App连接断开时，下次上电门锁不可以直接解锁*/
	Guard.status_unguardrecoverable_whenstayAppconnected = False ;
}

/***********************************************
 * Current received AT command total Length
 **********************************************/
static uint8_t currentCommandLength = 0;

/***********************************************
 * Calculate and return the length of the parameter end with 0x00
 **********************************************/
uint8_t getCurrentCommandLength()
{
	return currentCommandLength;
}

/***********************************************
 *	return a pointer to the response string
 **********************************************/
ArrInfo ATControl(uint8_t* in,const uint8_t len) 
{
	int i;
	ArrInfo arrInfo;
	currentCommandLength = len;
	//check if the connect command is received
	if(AT.status_BLEconnect == False)
	{
		//check if the incoming string is the connect command  
		//if (memcmp(Str_ATBLECONNECT, in, Strlen_ATBLECONNECT) == 0)
		if (strncmp((const char *)Str_ATBLECONNECT, (char *)in, Strlen_ATBLECONNECT) == 0)
		{
			arrInfo = ResponseATBLEConnect(in + Strlen_ATBLECONNECT);
			return arrInfo;
		} 
		else 
		{
			arrInfo = ResponseErrorATCmdConncetCmdNotYetSent(in + 0);
			return arrInfo;
		}
	}
	for (i = 0; i < sizeof(resp_strategy_table) / sizeof(ResponseStrategy); i++) 
	{
		if (strncmp((char *)resp_strategy_table[i].compare_str, (char *)in, resp_strategy_table[i].compare_str_len) == 0)//compare matches
		{
			arrInfo = resp_strategy_table[i].ResponseStr(in+resp_strategy_table[i].compare_str_len);
			return arrInfo;
		}
	}
	//no match, return error code
	arrInfo = ResponseErrorATCmd();
	return arrInfo;
	//apend the NULL char to the array
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
ArrInfo NOPFunction(uint8_t* para)
{
  ArrInfo nulArr = { NULL,NULL };
	return nulArr;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseErrorATCmd()
{    
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ErrorATCmd* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ErrorATCmd, arr};
    arr[0] = RCODEWithData_ErrorATCmd ;
    arr[1] = ErrorATCmd_Unrecognized ;
    
#if((1 + 1) !=StrlenWithData_ErrorATCmd)
	#error "StrlenWithData_ErrorATCmd长度不对！"
#endif
        
	CloseCyclicSend();
    return arrInfo;
    //AT.flag_alreadysent = True ;  
}


/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ResponseErrorATCmdErrorFormat()
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ErrorATCmd* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ErrorATCmd, arr};
    arr[0] = RCODEWithData_ErrorATCmd ;
    arr[1] = ErrorATCmd_ErrorFormat ;
    
#if((1 + 1) !=StrlenWithData_ErrorATCmd)
	#error "StrlenWithData_ErrorATCmd长度不对！"
#endif

    SendDataByBLE(arrInfo);

}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseErrorATCmdConncetCmdNotYetSent(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ErrorATCmd* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ErrorATCmd, arr};
    arr[0] = RCODEWithData_ErrorATCmd ;
    arr[1] = ErrorATCmd_ConncetCmdNotYetSent ;

#if((1 + 1) !=StrlenWithData_ErrorATCmd)
	#error "StrlenWithData_ErrorATCmd长度不对！"
#endif

	CloseCyclicSend();
    return arrInfo;

}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseErrorATCmdConncetCmdSendRepeatedly(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ErrorATCmd* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ErrorATCmd, arr};
    arr[0] = RCODEWithData_ErrorATCmd ;
    arr[1] = ErrorATCmd_ConncetCmdSendRepeatedly ;
Log_info0("Calling ResponseErrorATCmdConncetCmdSendRepeatedly.");
#if((1 + 1) !=StrlenWithData_ErrorATCmd)
	#error "StrlenWithData_ErrorATCmd长度不对！"
#endif
	CloseCyclicSend();
    return arrInfo;
}
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATBLEConnect(uint8_t* para)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_BLEConnect* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_BLEConnect, arr};
	arr[0] = RCODEWithData_BLEConnect ;

#if((0 + 1) !=StrlenWithData_BLEConnect)
	#error "StrlenWithData_BLEConnect长度不对！"
#endif
	//Flag it as connected before reacting to other commands
	AT.status_BLEconnect=True;
	CloseCyclicSend();
    return arrInfo;
}
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void FillATDevInfoDataMessageAndSend(uint8_t* para)
{
//	ArrInfo arrInfo={NULL, NULL};
//	CloseCyclicSend();
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_DevInfo* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_DevInfo, arr};

		arr[0] = RCODEWithData_DevInfo ;
		arr[1] = 0X00 ;
		arr[2] = Commu_Mode ;
		arr[3] = DevicePara.gear_ratio_motor ;
		arr[4] = DevicePara.gear_ratio_wheel ;
		arr[5] = DevicePara.number_pole_pairs ;
		arr[6] = DevicePara.wheel_diameter_inch_int ;
		arr[7] = DevicePara.battery_aH_battery ;

		arr[8] = 0X00 ;
		if(COMPILE_ENABLE == Double_LVSet)
			arr[8] |= U8_BIT0 ;
		if(COMPILE_ENABLE == OneKeyRepairSet)
			arr[8] |= U8_BIT1 ;
		if(COMPILE_ENABLE == ThreeSpeedSet)
			arr[8] |= U8_BIT2 ;
		if(COMPILE_ENABLE == ThreeTorqueSet)
			arr[8] |= U8_BIT3 ;
	
		arr[9] = DevicePara.wheel_diameter_inch_dec ;
		arr[10] = DevicePara.battery_materials ;
		arr[11] = DevicePara.battery_section_num ;
		arr[12] = DevicePara.batterystandardKM ;
		arr[13] = DevicePara.rated_Q_release_perFrame_A ;
		arr[14] = Anti_theftDevice_Function ;
		
        
#if((14 + 1) !=StrlenWithData_DevInfo)
	#error "StrlenWithData_DevInfo长度不对！"
#endif
		//return arrInfo;
		SendDataByBLE(arrInfo);	
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
ArrInfo ResponseATDevInfo(uint8_t* para)
{
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
	if(*para == ASCII_QuestionMark)
	{
		FillATDevInfoDataMessageAndSend(para);
		return arrInfo ;
	}
	if(*para == ASCII_Num_0)
	{
		DevicePara.gear_ratio_motor = Gear_Ratio_Motor_InFlash;
		DevicePara.gear_ratio_wheel = Gear_Ratio_Wheel_InFlash;
		DevicePara.number_pole_pairs = Number_Pole_Pairs_InFlash;
		DevicePara.wheel_diameter_inch_int = Wheel_Diameter_Inch_Int_InFlash;
		DevicePara.battery_aH_battery = AH_Battery_InFlash;
		DevicePara.wheel_diameter_inch_dec = Wheel_Diameter_Inch_Dec_InFlash ;
		DevicePara.battery_materialsAndmanufact = Battery_Materials_InFlash ;
		if(Battery_Materials_InFlash == Lead_Acid_Batteries)
			DevicePara.battery_section_num = Lead_Acid_Battery_Section_Num ;
		else
			DevicePara.battery_section_num = MnNiCo_Battery_Section_Num;
		DevicePara.batterystandardKM = BatteryStandardKM_InFlash ;
		DevicePara.rated_Q_release_perFrame_A = Rated_Q_Release_PerFrame_A_InFlash ;
		Mileage.caledvalue_hallchange_1KM = CalCounterHallchange1KM();
		FillATDevInfoDataMessageAndSend(para);
		return arrInfo ;
	}
	else
	{
		ResponseErrorATCmdErrorFormat();
    	return arrInfo;
	}
}
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsATDevInfoSetFormatCorrect(uint8_t* para)
{
	//长度是否够
    if(getCurrentCommandLength() != (Strlen_ATDEVINFO + Strlen_Data_DevInfo) ){
        return False ;
    }
	
	uint8_t gear_motor ;
	uint8_t gear_wheel ;
	gear_motor = *(para+1) ;
	gear_wheel = *(para+2) ;
	
	//如果是三轮车，极对数必须是4
	if(gear_motor >gear_wheel)
	{
		if(Pole_Pairs_Tricycle_Standard != *(para+Number_Pole_Pairs_RecArrNum))
			return False ;
	}
	//如果是二轮车，极对数必须大于等于23
	else if(gear_motor == gear_wheel )
	{
		if(*(para + Number_Pole_Pairs_RecArrNum) < Pole_Pairs_TwoWheel_Min)
			return False ;
		if(*(para + Number_Pole_Pairs_RecArrNum) > Pole_Pairs_TwoWheel_Max)
			return False ;
	}
    else//电机齿轮变速比必须大于等于后轮变速比
		return False ;

	if(*(para + Wheel_Diameter_Inch_Int_RecArrNum) < Wheel_Diameter_Inch_Int_Min)
		return False ;
	if(*(para + Wheel_Diameter_Inch_Int_RecArrNum) > Wheel_Diameter_Inch_Int_Max)
		return False ;

	//轮径的小数部分不能大于等于100
	if(*(para + Wheel_Diameter_Inch_Dec_RecArrNum) >= ONE_HUNDRED)
		return False ;
	
	//安时数必须不小于12AH
	if(*(para + AH_Battery_RecArrNum) < AH_Battery_Min )
        return False ;
	
	//只有带双向控制的硬件才能设置是否有双欠压功能
	if(Commu_Mode_TwoWire != Commu_Mode )
	{
		if(*(para + T2CFuncition_RecArrNum) > 0 )
			return False ;
	}
	
		//电池材料应该在铅酸和三元锂电之间选择
	if(*(para + Battery_Materials_RecArrNum) > MnNiCo_Ternary_Battery_CHILWEE)
		return False ;
	
	if(0X00 == *(para + Battery_Materials_RecArrNum))	//Lead_Acid_Batteries
	{
		if((LV_Grade / 12) !=*(para +Battery_SectionNum_RecArrNum))
			return False ;
	}
	else if(0X01 == *(para + Battery_Materials_RecArrNum))	//MnNiCo_Ternary_Battery
	{
		uint8_t sectionnum[2] ;
#if(48 == LV_Grade)
		sectionnum[0] = 13 ;		/*conventional*/
		sectionnum[1] = 14 ;		/*unconventional*/
#elif(60 == LV_Grade)
		sectionnum[0] = 16 ;		/*conventional*/
		sectionnum[1] = 17 ;		/*unconventional*/
#elif(72 == LV_Grade)
		sectionnum[0] = 20 ;		/*conventional*/
		sectionnum[1] = 20 ;		/*unconventional*/
#endif
		for(uint8_t i = 0 ;i < 2 ;i++)
		{
			if(*(para + Battery_SectionNum_RecArrNum)!= sectionnum[i])
				return False ;
		}
	}
	else
		;
	
	//防盗方式应该在已有的方式中选择
	if(*(para + Anti_theftDevice_Function_RecArrNum) > ElectricDouSupport)
		return False ;
	
	return True ; 
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATDevInfoSet(uint8_t* para)
{
	ArrInfo arrInfo ={NULL, NULL};
	CloseCyclicSend();
   	
    uint8_t * arr = (uint8_t*)malloc(StrlenWithData_DevInfoSet* sizeof(uint8_t));
    arrInfo.arr = arr;
    arrInfo.n = StrlenWithData_DevInfoSet;

	arr[0] = RCODEWithData_DevInfo ;
	arr[1] = 0X01 ;
	if(True == IsATDevInfoSetFormatCorrect(para))
	{
		UpdateDeviceInformation(para);
		CalQInitialAndMileageRemainder();
		UpdateMileageInitialAndRemainder();	
		Mileage.caledvalue_hallchange_1KM = CalCounterHallchange1KM();
		arr[2] = 0X01 ;		//success
	}
	else
		arr[2] = 0X00 ;		//fail
	
        
#if((2 + 1) != StrlenWithData_DevInfoSet)
	#error "StrlenWithData_DevInfoSet长度不对!"
#endif
	



	WriteParaToFlash();
	SoftwareReset();	
	//SoftReset.timer_wait_per2ms = 0 ;			//prepare for software reset
	//SoftReset.status = True ;
	return arrInfo ;
}



/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATRentDeadline(uint8_t* para)
{
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
	if(*para == ASCII_QuestionMark)
	{
		
		 arrInfo = ResponseDeadLineInquire();
		 return arrInfo;
    }
    else if(True == IsDeadLineSetFormatCorrect(para))
    {
        Guard.timer_day_guard_timeleft.h8 = *(para);
        Guard.timer_day_guard_timeleft.l8 = *(para + 1);
        Guard.timer_hour_guard_timeleft =*(para + 2) ;
        Guard.timer_minute_guard_timeleft =*(para + 3) ;
        Guard.timer_second_guard_timeleft =*(para + 4) ;
		
		uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_DeadLine* sizeof(uint8_t));
		arrInfo.arr = arr;
		arrInfo.n = StrlenWithData_DeadLine;
		arr[0] = RCODEWithData_DeadLine ;
		arr[1] = SetSuccess ;
		//用0补全剩余的所有byte
		for(uint8_t i= 2 ;i < StrlenWithData_DeadLine ;i++ )
			arr[i] =  0 ;		    
		return arrInfo;
		
        //AT.flag_alreadysent = True ;
    }
    else
    {
    	ResponseErrorATCmdErrorFormat();
    	return arrInfo;
        //AT.flag_alreadysent = True ;
    }
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsDeadLineSetFormatCorrect(uint8_t* para)
{
    if(getCurrentCommandLength() < (Strlen_ATDEADLINE + Strlen_Data_Deadline))
        return False ;
    if(*(para + 2)>= Hours_PerDay)
        return False ;
    if(*(para + 3) >= Minutes_PerHour)
        return False ;
    if(*(para + 4) >= Seconds_PerMinute)
        return False ;
    else
        return True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseDeadLineInquire()
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_DeadLine* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_DeadLine, arr};
    arr[0] = RCODEWithData_DeadLine ;
	arr[1] = ReplyInquiry ;
    arr[2]= Guard.timer_day_guard_timeleft.h8 ;
    arr[3]= Guard.timer_day_guard_timeleft.l8 ;
    arr[4]= Guard.timer_hour_guard_timeleft ;
    arr[5]= Guard.timer_minute_guard_timeleft ;
    arr[6]= Guard.timer_second_guard_timeleft ;
    
#if((6 + 1) !=StrlenWithData_DeadLine)
#error "StrlenWithData_DeadLine长度不对"
#endif
    return arrInfo;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATGuardStatus(uint8_t* para)
{
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
    if(*para == ASCII_QuestionMark)
	{
    	CheckGuardStatusAndResponse();
    	return arrInfo;
    }
    else if(ASCII_Num_0 == *para )
    {
        Guard.cmd_lock_From_BLE = Unlock_Ebike ;

        /*当AT+GuardStatus命令变化时，下次上电门锁不可以直接解锁*/
        Guard.status_unguardrecoverable_whenstayAppconnected = False ;
        Guard.status_ATCmd_response = True ;	// Flag AT guardCmd response Enable
        //		SendATCmdResponseCode(RCODE_UnGuarded);
        //AT.flag_alreadysent = True ;

#if(External_Anti_theft != Anti_theftDevice_Function)
		Guard.status_guardreason = CheckGuardReason();
		if(Guard.status_guardreason != DeadlineBecomeDue)
			//打开电源输出
			FreeKeyPowerOn();
#endif

    }
    else if( ASCII_Num_1 == *para)
    {
        Guard.cmd_lock_From_BLE = Lock_Ebike ;

        /*当AT+GuardCmd变化时，下次上电门锁不可以直接解锁*/
        Guard.status_unguardrecoverable_whenstayAppconnected = False ;
        Guard.status_ATCmd_response = True ;	// Flag AT guardCmd response Enable
    }
    else
    {
        ResponseErrorATCmdErrorFormat();
        return arrInfo;
    }
    return arrInfo;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void CheckGuardStatusAndResponse()
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_GuardStatus* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_GuardStatus, arr};
    arr[0] = RCODEWithData_GuardStatus ;
    if(Status_UnGuarded == Guard.status)
        arr[1]= 0X00 ;
    else
        arr[1]= Guard.status_guardreason;
    
#if((1 + 1) !=StrlenWithData_GuardStatus)
    #error "StrlenWithData_GuardStatus长度不对"
#endif
    SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseConntrollerConnectCheck(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ControllerConnect* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ControllerConnect, arr};
    arr[0] = RCODEWithData_ControllerConnect ;
	if(True == BLEMdlCnt.status_connect2controller)
	{
		arr[1]= ConnectWithController ;
	}
    else{
        arr[1]= NoConnectWithController ;
    }

#if((1 + 1) !=StrlenWithData_ControllerConnect)
    #error "StrlenWithData_ControllerConnect长度不对"
#endif    
	CloseCyclicSend();
    return arrInfo;
   
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATElectLock(uint8_t* para)
{
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
    if(ASCII_QuestionMark ==*para){
        CheckElectLockAndResponse();
    }
    else if(ASCII_Num_0 == *para )
    {
        if(Status_Auto == Guard.status_autoguard )
        {
            Guard.cmd_lock_From_BLE = Unlock_Ebike ;
            Guard.status_autoguard = Status_UnAuto ;
        }
        CheckElectLockAndResponse();
    }
    else if( ASCII_Num_1 ==*para)
    {
        if(Status_UnAuto == Guard.status_autoguard )
        {
            if(Status_Guarded == Guard.status )
                Guard.cmd_lock_From_BLE = Lock_Ebike ;
            Guard.status_autoguard = Status_Auto ;
        }
        CheckElectLockAndResponse();
    }
    else
    {
        //SendATCmdResponseCode(RERRORCODE_ElectLockSetFormat);
        ResponseErrorATCmdErrorFormat();
    }
    return arrInfo;

}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void CheckElectLockAndResponse()
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ElectLock* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ElectLock, arr};
    arr[0] = RCODEWithData_ElectLock ;
    arr[1]= (uint8_t)(Guard.status_autoguard);
    
 #if((1 + 1) != StrlenWithData_ElectLock)
    #error "StrlenWithData_ElectLock长度不对"
#endif    
    SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATSearchEbike(uint8_t* para)
{
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
	if( ASCII_Num_1 == *para)
	{
		uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_SearchEbike* sizeof(uint8_t));
		ArrInfo arrInfo = {StrlenWithData_SearchEbike, arr};
		arr[0] = RCODEWithData_SearchEbike ;
		arr[1]= 0X01;
		
	#if((1 + 1) != StrlenWithData_ElectLock)
		#error "StrlenWithData_SearchEbike长度不对"
	#endif    
		
		/********BuzzerSet*************/
		AT.Buzzer.counter_targetringnum_mul2 = Ring5_Mul2 ;
		AT.Buzzer.counter_nowringnum_mul2 = 0;
		AT.Buzzer.timer_addcounter_per2ms = 0 ;
		return arrInfo;
	}
	else{
		ResponseErrorATCmdErrorFormat();
	}
	return arrInfo;
}



#if(Commu_Mode_Common != Commu_Mode)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATErrorStatus(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ErrorStatus* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ErrorStatus, arr};
    //AT.status_error_realtime = GetErrorStatus() ;
    arr[0] = RCODEWithData_ErrorStatus ;
    //arr[1]= AT.status_error_realtime;
    arr[1] = GetErrorStatus() ;
    
#if((1 + 1) != StrlenWithData_ErrorStatus)
    #error "StrlenWithData_ErrorStatus长度不对"
#endif      
    //AT.flag_alreadysent = True ;
	CloseCyclicSend();
    return arrInfo;

}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATErrorHistory(uint8_t* para)
{
  ArrInfo arrInfo = {NULL, NULL};
	//TODO: Port this -- Not needed now
//    if(ASCII_QuestionMark == Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY])
//        ResponseErrorHistoryEntriesNum();
//    else if(ASCII_D == Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY] )
//    {
//        while(True != DeleteErrorHistory() )
//			;
//		ResponseErrorHistoryDeleteSuccess() ;
//    }
//    /*else if((ASCII_QuestionMark == Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY + 2 ]) \
//		&& (True == IsACharacterANum(Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY ]))
//		&& (True == IsACharacterANum(Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY + 1 ])))	*/
//    else if(True == IsErrorHistoryDetailInquireFormatCorrect())
//    {
//        ResponseErrorHistoryDetail() ;
//    }
//    else
//    {
//        //SendATCmdResponseCode(RERRORCODE_ElectLockSetFormat);
//        ResponseErrorATCmdErrorFormat();
//    }
//	CloseCyclicSend();
  return arrInfo;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsErrorHistoryDetailInquireFormatCorrect(void)
{
	//TODO: Port this -- Not needed now
//    if((ASCII_QuestionMark != Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY + 2 ]))
//        return False ;
//    if(True != IsACharacterANum(Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY ]))
//        return False ;
//    if(True != IsACharacterANum(Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATERRORHISTORY + 1 ]))
//        return False ;
//    else
//        return True ;

	return False;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseErrorHistoryEntriesNum(uint8_t* para)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ErrorHistory* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ErrorHistory, arr};
    arr[0] = RCODEWithData_ErrorHistory	;
	if(ErrorHistoryMessage.len_entries_total > 0 )
	{
		arr[1] = 0X00;
		arr[2] = ErrorHistoryMessage.len_entries_total ;
	}
	else
	{
		arr[1] = 0X00;
		arr[2] = 0X00;	
	}
    
 #if((2 + 1) != StrlenWithData_ErrorHistory)
    #error "StrlenWithData_ErrorHistory长度不对"
#endif    
	return arrInfo;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseErrorHistoryDeleteSuccess(uint8_t* para)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ErrorHistory* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ErrorHistory, arr};
    arr[0] = RCODEWithData_ErrorHistory	;
	arr[1] = 0X02 ;
	arr[2] = 0X00 ;
	
 #if((2 + 1) != StrlenWithData_ErrorHistory)
    #error "StrlenWithData_ErrorHistory长度不对"
#endif    
	return arrInfo;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseErrorHistoryDetail(uint8_t* para)
{
	uint8_t *  arr = (uint8_t*)malloc(RunMessageSingleEntryLen* sizeof(uint8_t));
	ArrInfo arrInfo = {RunMessageSingleEntryLen, arr};
	uint8_t line ;
	uint8_t num_h ;
	uint8_t num_l ;
    arr[0] = RCODEWithData_ErrorHistory	;
	arr[1] = 0X01;
	
	num_h = *para ;
	num_l = *(para+1) ;
	//TODO: Port this -- Not needed now
	//line = Convert2CharacterInto2Num(num_h , num_l) ;
	for(uint8_t i = 0 ; i <  RunMessageSingleEntryLen ;i ++ ){
		arr[i + 2] =  ErrorHistoryMessage.entry[line][i] ;
	}
	return arrInfo;

}


/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
uint8_t GetErrorStatus(void)
{
    //return (OneLineCommu.datacode[3] & 0x79) ;
	return OLCM.data_error ;
}
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATQ(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_Q* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_Q, arr};
    
    arr[0] = RCODEWithData_Q ;
//#define TESTCODE_BLE_220V 1
#if(TESTCODE_BLE_220V)
	arr[1] = 0X05 ;//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[2] = 0X06 ;
    arr[3] = 0X07 ;
    arr[4] = 0X08 ;
    arr[5] = 0X01 ;//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[6] = 0X02 ;
    arr[7] = 0X03 ;
    arr[8] = 0X04 ;
	arr[9] = 0X04 ;
	arr[10] = 0X04 ;
	arr[11] = 0X04 ;
	arr[12] = 0X04 ;
	
#else
    arr[1] = Q.data_max.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[2] = Q.data_max.bit8[2];
    arr[3] = Q.data_max.bit8[1];
    arr[4] = Q.data_max.bit8[0];
    arr[5] = Q.data_remainder.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[6] = Q.data_remainder.bit8[2];
    arr[7] = Q.data_remainder.bit8[1];
    arr[8] = Q.data_remainder.bit8[0];
	Union4Bytes_TypeDef temp_send ;
	temp_send.sum32 = Q.data_release_temp ;
	arr[9] = temp_send.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[10] = temp_send.bit8[2];
    arr[11] = temp_send.bit8[1];
    arr[12] = temp_send.bit8[0];
#endif
    
#if((12 + 1) != StrlenWithData_Q)
    #error "StrlenWithData_Q长度不对"
#endif
	CloseCyclicSend();
	return arrInfo;

}
#if(Commu_Mode_TwoWire == Commu_Mode)
	#if(COMPILE_ENABLE == OneKeyRepairSet)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseOneKeyRepair(uint8_t* para)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_OneKeyRepair* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_OneKeyRepair, arr};
    arr[0] = RCODEWithData_OneKeyRepair ;
	T2C.counter_onekeyrepairset_cycle = Counter_OneKeyRepairSet_Cycle ;
	arr[1]= SetSuccess;
	
#if((1 + 1) != StrlenWithData_OneKeyRepair)
    #error "StrlenWithData_OneKeyRepair长度不对"
#endif   
    
    return arrInfo;
    //AT.flag_alreadysent = True ;	
	CloseCyclicSend();
}
	#endif


	#if(COMPILE_ENABLE == Double_LVSet)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseLV1LevelUnlockSet(uint8_t* para)
{	
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_UnLockLevel1LV* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_UnLockLevel1LV, arr};
    arr[0] = RCODEWithData_UnLockLevel1LV ;
	if(True == IsLVLowerThanLV1Level())	
	{
		CustomSerialInterfaceMessage.status_LVLevel = Status_LowerLVlevel ;
		arr[1]= SetSuccess;
	}
	else
		arr[1]= SetFailure;				//因为电压足够高，设置无电续航失败
    
#if((1 + 1) != StrlenWithData_UnLockLevel1LV)
    #error "StrlenWithData_UnLockLevel1LV长度不对"
#endif   
    
    return arrInfo;
    //AT.flag_alreadysent = True ;	
	CloseCyclicSend();
}
	#endif

	#if(COMPILE_ENABLE == ThreeSpeedSet)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseThreeSpeed(uint8_t* para)
{	
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
    if(ATActionCode_Query == *para )
	{
    	ResponseThreeSpeedQuery();
    	return arrInfo;
    }
    if(ATActionCode_Config_Defalt == *para )
    {
        if((*(para + 1)<= Status_HighSpeed) && (*(para + 1)))
        {
			T2C.CommonData.ThressSpeedBit = *(para + 1) ;
			ResponseThreeSpeedConfig();
			return arrInfo;
        }
    }
	ResponseErrorATCmd();
	return arrInfo;
	
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
void ResponseThreeSpeedQuery(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ThreeSpeed* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ThreeSpeed, arr};
	arr[0] = RCODEWithData_ThreeSpeed;
	arr[1]= ATActionCode_Query ;
	//arr[2]= CustomSerialInterfaceMessage.status_threespeed;
	arr[2]= T2C.CommonData.ThressSpeedBit ;
	
#if((1 + 2) != StrlenWithData_ThreeSpeed)
    #error "StrlenWithData_ThreeSpeed is wrong"
#endif   
	SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/	
void ResponseThreeSpeedConfig(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ThreeSpeed* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ThreeSpeed, arr};
	arr[0] = RCODEWithData_ThreeSpeed;
	arr[1] = ATActionCode_Config_Defalt ;
	arr[2] = SetSuccess;
		
#if((1 + 2) != StrlenWithData_ThreeSpeed)
    #error "StrlenWithData_ThreeSpeed is wrong"
#endif   

	SendDataByBLE(arrInfo);
}	
	#endif

	#if(COMPILE_ENABLE == ThreeTorqueSet)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseThreeTorque(uint8_t* para)
{	
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
    if(ATActionCode_Query == *para )
	{
    	ResponseThreeTorqueQuery();
    	return arrInfo;
    }
    if(ATActionCode_Config_Defalt == *para )
    {
        if((*(para + 1)<= Status_HighTorque) && (*(para + 1)))
        {
			T2C.CommonData.ThressTorqueBit = *(para + 1) ; ;
			ResponseThreeTorqueConfig();
			return arrInfo;
        }
    }
	ResponseErrorATCmd();
	return arrInfo;
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
void ResponseThreeTorqueQuery(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ThreeTorque* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ThreeTorque, arr};
	arr[0] = RCODEWithData_ThreeTorque;
	arr[1]= ATActionCode_Query ;
	arr[2]= T2C.CommonData.ThressTorqueBit ;
	
#if((1 + 2) != StrlenWithData_ThreeTorque)
    #error "StrlenWithData_ThreeTorque is wrong"
#endif   
	SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/	
void ResponseThreeTorqueConfig(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ThreeTorque* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ThreeTorque, arr};
	arr[0] = RCODEWithData_ThreeTorque;
	arr[1] = ATActionCode_Config_Defalt ;
	arr[2] = SetSuccess;
		
#if((1 + 2) != StrlenWithData_ThreeTorque)
    #error "StrlenWithData_ThreeTorque is wrong"
#endif   

	SendDataByBLE(arrInfo);
}	
	#endif	

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseControllerE2PROMConfig(uint8_t* para)
{	
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
	if( *(para + 1) <= AT_DeviceInfoBlockNum_Max )
	{
		T2C.AT_blocknum = *(para + 1) ;	
    	if(ATActionCode_Query == *para )
		{
			if(True == BLEMdlCnt.status_connect2controller)
			{
				if(T2C_SendCommonDataCmd == T2C.cmd)
				{
					T2C.cmd = T2C_ReadConfigCmd ;
					ResponseReadControllerE2PROMConfigStart();
					return arrInfo;
				}
				else if(T2C_ReadConfigFinished == T2C.cmd)
				{
					ResponseReadControllerE2PROMConfigBlock();
					return arrInfo;
				}
				else
					;
			}
			else
			{
				T2C.cmd = T2C_SendCommonDataCmd ;
				ResponseReadControllerE2PROMConfigFail();
				return arrInfo;
			}
		}
		else if(ATActionCode_Config_Defalt == *para )
		{	
			if(True == BLEMdlCnt.status_connect2controller)
			{
				UpdateControllerE2PROMConfigBlockFromATCmd(para);
				ResponseWriteControllerE2PROMConfigBlock();
				return arrInfo;
			}
			else
			{
				T2C.cmd = T2C_SendCommonDataCmd ;
				ResponseWriteControllerE2PROMConfigFail();
				return arrInfo;
			}
		}
		else
			;
	}
	ResponseErrorATCmd();
	return arrInfo;
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
void ResponseReadControllerE2PROMConfigStart(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ReadContrE2Cfg* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ReadContrE2Cfg, arr};
	arr[0] = RCODEWithData_ContrE2Cfg ;
	arr[1] = ATActionCode_Query ;
	arr[2] = T2C.AT_blocknum  ;
	arr[3] = ExecutionWait ;		
	for(uint8_t i = 0 ; i < T2C_ATConfigBlockStrlen ; i++)
		arr[4 + i] = 0 ;
	
#if((4 + T2C_ATConfigBlockStrlen ) != StrlenWithData_ReadContrE2Cfg)
    #error "StrlenWithData_ReadContrE2Cfg is wrong"
#endif   

	SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/	
void ResponseReadControllerE2PROMConfigFail(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ReadContrE2Cfg* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ReadContrE2Cfg, arr};
	arr[0] = RCODEWithData_ContrE2Cfg ;
	arr[1] = ATActionCode_Query ;
	arr[2] = T2C.AT_blocknum  ;
	arr[3] = SetFailure ;		
	for(uint8_t i = 0 ; i < T2C_ATConfigBlockStrlen ; i++)
		arr[4 + i] = 0 ;
				
#if((4 + T2C_ATConfigBlockStrlen ) != StrlenWithData_ReadContrE2Cfg)
	#error "StrlenWithData_ReadContrE2Cfg is wrong"
#endif   

	SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/	
void ResponseReadControllerE2PROMConfigBlock(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_ReadContrE2Cfg* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_ReadContrE2Cfg, arr};
	arr[0] = RCODEWithData_ContrE2Cfg ;
	arr[1] = ATActionCode_Query ;
	arr[2] = T2C.AT_blocknum  ;
	arr[3] = SetSuccess ;		
	for(uint8_t i = 0 ; i < T2C_ATConfigBlockStrlen ; i++)
		arr[4 + i] = UserConfig.UserConfigArr[i + T2C_ATConfigBlockStrlen * T2C.AT_blocknum] ;
				
#if((4 + T2C_ATConfigBlockStrlen ) != StrlenWithData_ReadContrE2Cfg)
	#error "StrlenWithData_ReadContrE2Cfg is wrong"
#endif   

	SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
void UpdateControllerE2PROMConfigBlockFromATCmd(uint8_t* para)
{
	//update blockwrittenflag and configdata[]
	if(0 == T2C.AT_blocknum)	
	{
		for(uint8_t i = 0 ;i < T2C_ATConfigBlockStrlen ;i++)
			//T2C.WriteConfig.configdata [T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = Reveive_Usart1ToDMA.Data[3 + i] ;
			//UserConfig.UserConfigArr[T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = Reveive_Usart1ToDMA.Data[3 + i] ;
			UserConfig.UserConfigArr[T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = *(para + 2  + i);
		T2C.BlockWrittenFlag.block0 = 1 ;
	}
	else if(1 == T2C.AT_blocknum )
	{
		for(uint8_t i = 0 ;i < T2C_ATConfigBlockStrlen ;i++)
			//T2C.WriteConfig.configdata [T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = Reveive_Usart1ToDMA.Data[3 + i] ;
//			UserConfig.UserConfigArr[T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = Reveive_Usart1ToDMA.Data[3 + i] ;
			UserConfig.UserConfigArr[T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = *(para + 2  + i);
		T2C.BlockWrittenFlag.block1 = 1 ;
	}
	else if(2 == T2C.AT_blocknum)
	{
		for(uint8_t i = 0 ;i < (UserConfigData_Strlen - T2C_ATConfigBlockStrlen * T2C.AT_blocknum) ;i++)
			//T2C.WriteConfig.configdata [T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = Reveive_Usart1ToDMA.Data[3 + i] ;
//			UserConfig.UserConfigArr[T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = Reveive_Usart1ToDMA.Data[3 + i] ;
			UserConfig.UserConfigArr[T2C_ATConfigBlockStrlen * T2C.AT_blocknum + i] = *(para + 2  + i);
		T2C.BlockWrittenFlag.block2 = 1 ;
	}
	else
		;	
	if(0X07 == T2C.BlockWrittenFlag.flag_all)
	{	
		UserConfig.UserConfigStruct = AdjustOrderOfUserConfig(UserConfigAT.UserConfigStruct) ;			//at write  ,update UserConfigAT 
		T2C.BlockWrittenFlag.flag_all = 0 ;
		T2C.cmd = T2C_WriteConfigCmd ;
	}
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
void ResponseWriteControllerE2PROMConfigBlock(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_WriteContrE2Cfg* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_WriteContrE2Cfg, arr};
	arr[0] = RCODEWithData_ContrE2Cfg ;
	arr[1] = ATActionCode_Config_Defalt ;
	arr[2] = T2C.AT_blocknum ;
	arr[3] = SetSuccess	;		
				
#if((1 + 3 ) != StrlenWithData_WriteContrE2Cfg)
    #error "StrlenWithData_WriteContrE2Cfg is wrong"
#endif   
				
	SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
void ResponseWriteControllerE2PROMConfigFail(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_WriteContrE2Cfg* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_WriteContrE2Cfg, arr};
	arr[0] = RCODEWithData_ContrE2Cfg ;
	arr[1] = ATActionCode_Config_Defalt ;
	arr[2] = T2C.AT_blocknum ;
	arr[3] = SetFailure;		
				
#if((1 + 3 ) != StrlenWithData_WriteContrE2Cfg)
    #error "StrlenWithData_WriteContrE2Cfg is wrong"
#endif   

	SendDataByBLE(arrInfo);			
}

/*******************************************************************************
* 函数名		:
* 函数描述		:
* 输入参数		: 无
* 输出结果		: 无
* 返回值		: 无
*******************************************************************************/
void ResponseWriteControllerE2PROMConfigFinished(void)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_WriteContrE2Cfg* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_WriteContrE2Cfg, arr};
	arr[0] = RCODEWithData_ContrE2Cfg ;
	arr[1] = ATActionCode_Config_Defalt ;
	arr[2] = 0XFF ;
	arr[3] = SetSuccess ;		
				
#if((1 + 3 ) != StrlenWithData_WriteContrE2Cfg)
    #error "StrlenWithData_WriteContrE2Cfg is wrong"
#endif   
				
	SendDataByBLE(arrInfo);	
}	



	
	
#endif
#endif

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:本次行驶里程在park45分钟时及检测到充电标志才清零
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATMileage(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_Mileage* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_Mileage, arr};
    
    /*if(True == BLEModule.status_connect2controller)
        ;
    else
        Mileage.counter_hallchange_remainder.bit32 = CalMileageRemainderWithoutUpdataQInitial();*/
    
    arr[0] = RCODEWithData_Mileage ;
#if(TESTCODE_BLE_220V)
	arr[1] = 0X06 ; //因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[2] = 0X08 ;
    arr[3] = 0X0A ;
    arr[4] = 0X0C ;
    arr[5] = 0X05 ;//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[6] = 0X06 ;
    arr[7] = 0X07 ;
    arr[8] = 0X08 ;
    arr[9] = 0X01 ;//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[10] = 0X02 ;
    arr[11] = 0X03 ;
    arr[12] = 0X04 ;
	arr[13] = 0X01 ;
    arr[14] = 0X02 ;
    arr[15] = 0X03 ;
#else
    arr[1] = Mileage.counter_hallchange_max.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[2] = Mileage.counter_hallchange_max.bit8[2];
    arr[3] = Mileage.counter_hallchange_max.bit8[1];
    arr[4] = Mileage.counter_hallchange_max.bit8[0];
    arr[5] = Mileage.counter_hallchange_remainder.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[6] = Mileage.counter_hallchange_remainder.bit8[2];
    arr[7] = Mileage.counter_hallchange_remainder.bit8[1];
    arr[8] = Mileage.counter_hallchange_remainder.bit8[0];
    arr[9] = Mileage.counter_hallchange_accumu.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
    arr[10] = Mileage.counter_hallchange_accumu.bit8[2];
    arr[11] = Mileage.counter_hallchange_accumu.bit8[1];
    arr[12] = Mileage.counter_hallchange_accumu.bit8[0];
	arr[13] = Mileage.odometer_perKM.bit8[2] ;
    arr[14] = Mileage.odometer_perKM.bit8[1] ;
    arr[15] = Mileage.odometer_perKM.bit8[0] ;
#endif
    
#if((15 + 1) != StrlenWithData_Mileage)
    #error "StrlenWithData_Mileage长度不对"
#endif     
    
	CloseCyclicSend();
    return arrInfo;
    //AT.flag_alreadysent = True ;
}
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATHallSpeed(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_HallSpeed* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_HallSpeed, arr};
	UnionSumWithH8L8_TypeDef hallspeed ;
	hallspeed.sum = GetHallSpeed();
    arr[0] = RCODEWithData_HallSpeed ;
#if(TESTCODE_BLE_220V)
	arr[1]= 0x08;
    arr[2]= 0XA0;
#else
    arr[1]= hallspeed.h8;
    arr[2]= hallspeed.l8;
#endif
    
#if((2 + 1) != StrlenWithData_HallSpeed)
    #error "StrlenWithData_HallSpeed长度不对"
#endif   
    
	CloseCyclicSend();
    return arrInfo;
    //AT.flag_alreadysent = True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseHardwareVersion(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_HardwareVersion* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_HardwareVersion, arr};
    arr[0] = RCODEWithData_HardwareVersion ;
    arr[1]= (uint8_t)(PCB_VERSION >> 8);
    arr[2]= (uint8_t)(PCB_VERSION);
    arr[3]= (uint8_t)(FirmwareVersion >> 16);
    arr[4]= (uint8_t)(FirmwareVersion >> 8);
    arr[5]= (uint8_t)(FirmwareVersion);
    
#if((5 + 1) != StrlenWithData_HardwareVersion)
    #error "StrlenWithData_HardwareVersion长度不对"
#endif   

	CloseCyclicSend();
	return arrInfo;
    //AT.flag_alreadysent = True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseCustomerFileNum(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_CFileNum* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_CFileNum, arr};

    arr[0] = RCODEWithData_CFileNum ;
    arr[1] = 0x00;
    arr[2] = (uint8_t)(CumstomerNum >> 32) ;	
    arr[3] = (uint8_t)(CumstomerNum >> 24) ;
    arr[4] = (uint8_t)(CumstomerNum >> 16) ; 	
    arr[5] = (uint8_t)(CumstomerNum >> 8) ;		
	arr[6] = (uint8_t)(CumstomerNum) ;		
    
#if((6 + 1) != StrlenWithData_CFileNum)
    #error "StrlenWithData_CFileNum is wrong"
#endif   

	CloseCyclicSend();
	return arrInfo;

}


/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseATCyclicSend(uint8_t* para)
{
	 ArrInfo arrInfo = {NULL, NULL};
	if(ASCII_Num_0 == *para )
	{
		AT.mode_cyclicsend = False ;
	}
	else if(ASCII_Num_1 == *para )
	{
		AT.mode_cyclicsend = True ;
		CyclicSendData();
	}
	else
	{
		ResponseErrorATCmdErrorFormat();
		CloseCyclicSend();
	}
	return arrInfo;
}


/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void CyclicSendData()
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_CyclicSend* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_CyclicSend, arr};
    arr[0] = RCODEWithData_CyclicSend  ;

#if(COMPILE_ENABLE == TESTCODE_BLE_220V)
	static uint8_t temp = 0 ;
	//获取故障代码
	arr[1] = temp;	
	
	//获取剩余电量
	if(temp%3)
		arr[2] = 0X01 ;
	else
		arr[2] = 0X00 ;
	arr[3] = 0X02;
	arr[4] = 0X03;
	arr[5] = 0X04; ;
	
	//获取里程
	arr[6] = 0X05; 
	arr[7] = 0X06; 
	arr[8] = 0X07; 
	arr[9] = 0X08;
	arr[10] = 0X0A ;
	arr[11] = 0X0B ;
	arr[12] = 0X0C ;
	arr[13] = 0X0D ;
	arr[14] = 0X0E ;
	temp ++ ;
	if(temp >= 10)
		temp = 0 ;
	

#else
	#if(Commu_Mode_Common == Commu_Mode)
		//获取故障代码
		arr[1] = 0;		
		//获取剩余电量
		arr[2] = 0 ;	
		arr[3] = 0 ;
		arr[4] = 0 ;
		arr[5] = 0 ;
		
		arr[10] = 0 ;		//?????????
		arr[11]= 0 ;		
		arr[14] = 0 ;		//??????	??????????
		arr[15] = 0 ;
	#else
		//获取故障代码
		arr[1] = GetErrorStatus();		
		//获取剩余电量
		arr[2] = Q.data_remainder.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
		arr[3] = Q.data_remainder.bit8[2];
		arr[4] = Q.data_remainder.bit8[1];
		arr[5] = Q.data_remainder.bit8[0];

		arr[10] = 0 ;   //?????????
		arr[11] = OLCM.data_current	;		//??4?????
		arr[14] = OLCM.counter_hall_speed.h8 ;	//0.5s?????? ??
		arr[15]	= OLCM.counter_hall_speed.l8 ;
	#endif
		
		//获取剩余里程
		arr[6] = Mileage.counter_hallchange_remainder.bit8[3];//因为存储器使用的是小端存储方式，因此高位数据在bit[3]中
		arr[7] = Mileage.counter_hallchange_remainder.bit8[2];
		arr[8] = Mileage.counter_hallchange_remainder.bit8[1];
		arr[9] = Mileage.counter_hallchange_remainder.bit8[0];

		Union4Bytes_TypeDef lv_temp ;
		lv_temp.sum32 = LV.avr_adc ;
		
		#if(48 == LV_Grade)		//LVDigital2Analog ?0.021257136   *16 =	0.340114176		*17/50
		lv_temp.sum32 = lv_temp.sum32 * 17 / 50 ;
		#elif(60 == LV_Grade)	//LVDigital2Analog ?0.026586914   *16= 0.425390624		*17/40
		lv_temp.sum32 = lv_temp.sum32 * 17 / 40 ;	;
		#elif(72 == LV_Grade)	//LVDigital2Analog ?0.032025146 *16 =0.512402336			*41/80
		lv_temp.sum32 = lv_temp.sum32 * 41 / 80 ;	;
		#else
		#endif
		
        UnionSumWithH8L8_TypeDef temp ;
        temp.sum = lv_temp.StructH16L16.l16 ;
		arr[12] = temp.h8;
		arr[13] = temp.l8;
#endif
		
#if((15 + 1) != StrlenWithData_CyclicSend)
    #error "StrlenWithData_CyclicSend长度不对"
#endif   
		SendDataByBLE(arrInfo);

}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	: 当处在循环发送时，一旦接收到新的ATCmd，就关闭循环发送。如果循环发送的
的值还没有完全发送结束，就等待DMA发送结束。
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void CloseCyclicSend(void)
{
    if(True == AT.mode_cyclicsend )
        AT.mode_cyclicsend = False ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:app发过来的指令，和mcu需要发给hc-08的指令一致
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseAndSetChangeBLEModuleName(uint8_t* para)
{
    uint8_t arr_first[Strlen_ATNAME + Strlen_BLENAME_MAX];
    uint8_t strlen_send = Strlen_ATNAME + GetBLENameStrlenFromApp();
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_BLERename* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_BLERename, arr};
    
    for(uint8_t i = 0; i < strlen_send;i ++)
	{
        arr_first[i]= *(para+i);
    }
	
	arr[0] = RCODE_ReceiveNameSuccess ;
	CloseCyclicSend();
    //TODO:  -- Not needed now change device name according to para, 12 bytes, then save it to Flash
	//(then disconnect and reboot after the app have received the response)
	return arrInfo;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
uint8_t GetBLENameStrlenFromApp(void)
{
    if(getCurrentCommandLength() >= (Strlen_ATNAME + Strlen_BLENAME_MAX))
        return Strlen_BLENAME_MAX ;
    else
        return (getCurrentCommandLength() - Strlen_ATNAME );
}




#if(External_Anti_theft != Anti_theftDevice_Function)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseFreeKeyStart(uint8_t* para)
{
	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
	if(ASCII_Num_0 == *para)
	{
		FreeKeyPowerOff();
		ResponseFreeKeyStartAction() ;
		return arrInfo;
    }
    else if(ASCII_Num_1 == *para )
	{
    	FreeKeyPowerOn();
		ResponseFreeKeyStartAction() ;
		return arrInfo;
    }
    else
    {
    	ResponseErrorATCmdErrorFormat();
    	return arrInfo;
    }
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ResponseFreeKeyStartAction()
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_FreeKeyStart* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_FreeKeyStart, arr};
    arr[0] = RCODEWithData_FreeKeyStart ;
	arr[1]= (uint8_t)FreeKey.status ;
    
#if((1 + 1) !=StrlenWithData_FreeKeyStart)
    #error "StrlenWithData_FreeKeyStart长度不对"
#endif
    
	SendDataByBLE(arrInfo);
	//test!!!!
}

#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseSeatLock(uint8_t* para)
{

	ArrInfo arrInfo = {NULL, NULL};
	CloseCyclicSend();
	if(ASCII_Num_0 == *para)
	{
		if(Status_UnlockAction == SeatLock.status)		//如果正在开锁 则不改变状态
			ResponseSeatLockAction();
		else if(Status_LockedAction == SeatLock.status)		//如果正在锁车 就先停止输出 然后再解锁
		{
			SeatLockStopDrive();	//stop drive and clear timer
			SeatLock.status = Status_WaitForMotorStop ;
		}
		else
			SeatLock.status = Status_WaitForMotorStop ;
	}
	else if(ASCII_QuestionMark == *para)
		ResponseSeatLockAction();
	else
        ResponseErrorATCmdErrorFormat();
	
	return arrInfo;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ResponseSeatLockAction()
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_SeatLock* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_SeatLock, arr};
    arr[0] = RCODEWithData_SeatLock ;
	arr[1]= (uint8_t)SeatLock.status ;
    
#if((1 + 1) !=StrlenWithData_SeatLock)
    #error "StrlenWithData_SeatLock长度不对"
#endif

	SendDataByBLE(arrInfo);
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void CheckMechalMotorLockStatusAndResponse()
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_MechalLock* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_MechalLock, arr};
    arr[0] = RCODEWithData_MechalLock ;
	arr[1]= (uint8_t)MechalMotorLock.status;
    
#if((1 + 1) !=StrlenWithData_MechalLock)
    #error "StrlenWithData_MechalLock长度不对"
#endif
    
	SendDataByBLE(arrInfo);
}



/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseAllParkFinish(uint8_t* para)
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_AllParkFinish* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_AllParkFinish, arr};
    arr[0] = RCODEWithData_AllParkFinish ;
	arr[1]= 0X01 ;
    
#if((1 + 1) !=StrlenWithData_AllParkFinish)
    #error "StrlenWithData_AllParkFinish长度不对"
#endif
    
    return arrInfo;
}


#if(ElectricDouSupport == Anti_theftDevice_Function)

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseDouSupport(uint8_t* para)
{
	ArrInfo arrInfo={NULL, NULL};
	if(ASCII_Num_0 == *para)		//双撑收起
	{
//		if(Status_Retracted == DouSupport.status || Status_RetractingAction == DouSupport.status)		//如果收起或已经收起 则不改变状态
//			ResponseDouSupportAction();
//		else
//		{
//			if(Status_LockedAction == DouSupport.status)		//如果正在锁车 就先停止输出 然后再解锁
//				SeatLockStopDrive();	//stop drive and clear timer
			DouSupportRetractAction();
			DouSupport.status = Status_RetractingAction ;
			ResponseDouSupportAction();
//		}
	}
	else if(ASCII_Num_1 == *para)		//双撑落下
	{
//		if(Status_Fallen == DouSupport.status || Status_FallingAction == DouSupport.status)		//如果正在落下或者已经落下 则不改变状态
//			ResponseDouSupportAction();
//		else
//		{
			DouSupportFallAction();
			DouSupport.status = Status_FallingAction ;
			ResponseDouSupportAction();
//		}
	}
	else if(ASCII_QuestionMark == *para)
		ResponseDouSupportAction();
	else
    {
        //SendATCmdResponseCode(RERRORCODE_ElectLockSetFormat);
        ResponseErrorATCmdErrorFormat();
    }
	CloseCyclicSend();
	return arrInfo;
}
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ResponseDouSupportAction()
{
	uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_DouSupport* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_DouSupport, arr};
    arr[0] = RCODEWithData_DouSupport ;
	arr[1]= (uint8_t)DouSupport.status ;
    
#if((1 + 1) != StrlenWithData_DouSupport)
    #error "StrlenWithData_DouSupport长度不对"
#endif
    
	SendDataByBLE(arrInfo);


}

#endif

#endif





#endif



#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseTestPowerLock(uint8_t* para)
{
    if(ASCII_Num_0 == Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATTESTPOWERLOCK] )
    {
		TangZeLockInterface.keyvalue = KV_PowerOff ;
        CheckTestPowerLockAndResponse();	
    }
    else if( ASCII_Num_1 ==Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATTESTPOWERLOCK])
    {
        TangZeLockInterface.keyvalue = KV_PowerOn ;
        CheckTestPowerLockAndResponse();
    }
    else
    {
        //SendATCmdResponseCode(RERRORCODE_ElectLockSetFormat);
        ResponseErrorATCmdErrorFormat();
    }
	CloseCyclicSend();
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo CheckTestPowerLockAndResponse(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_TESTPowerLock* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_TESTPowerLock, arr};
    arr[0] = RCODEWithData_TESTPowerLock ;
    if(KV_PowerOff == TangZeLockInterface.keyvalue)
        arr[1]= 0X00 ;			//断电
    else
        arr[1]= 0X01 ;			//上电

#if((1 + 1) !=StrlenWithData_TESTPowerLock)
    #error "StrlenWithData_TESTPowerLock长度不对"
#endif    
    
    return arrInfo;
    //AT.flag_alreadysent = True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseTestMotorLock(uint8_t* para)
{
    if(ASCII_Num_0 == Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATTESTMOTORLOCK] )
    {
		TangZeLockInterface.keyvalue = KV_UnlockMotor ;
        CheckTestMotorLockAndResponse();	
    }
    else if( ASCII_Num_1 ==Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATTESTMOTORLOCK])
    {
        TangZeLockInterface.keyvalue = KV_LockMotor ;
        CheckTestMotorLockAndResponse();
    }
    else
    {
        //SendATCmdResponseCode(RERRORCODE_ElectLockSetFormat);
        ResponseErrorATCmdErrorFormat();
    }
	CloseCyclicSend();
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo CheckTestMotorLockAndResponse(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_TESTMotorLock* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_TESTMotorLock, arr};
    arr[0] = RCODEWithData_TESTMotorLock ;
    if(KV_UnlockMotor == TangZeLockInterface.keyvalue)
        arr[1]= 0X00 ;			//解锁
    else
        arr[1]= 0X01 ;			//上锁

#if((1 + 1) !=StrlenWithData_TESTMotorLock)
    #error "StrlenWithData_TESTMotorLock长度不对"
#endif    
    
    return arrInfo;
    //AT.flag_alreadysent = True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseTestSeatLock(uint8_t* para)
{
    if(ASCII_Num_0 == Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATTESTSEATLOCK] )
    {
		TangZeLockInterface.keyvalue = KV_UnlockSeat ;
        CheckTestSeatLockAndResponse();	
    }
    else
    {
        //SendATCmdResponseCode(RERRORCODE_ElectLockSetFormat);
        ResponseErrorATCmdErrorFormat();
    }
	CloseCyclicSend();
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo CheckTestSeatLockAndResponse(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_TESTSeatLock* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_TESTSeatLock, arr};
    arr[0] = RCODEWithData_TESTSeatLock ;
    arr[1]= 0X00 ;			//解锁

#if((1 + 1) !=StrlenWithData_TESTSeatLock)
    #error "StrlenWithData_TESTSeatLock长度不对"
#endif    
    
    return arrInfo;
    //AT.flag_alreadysent = True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo ResponseTestShockAlarm(uint8_t* para)
{
    if(ASCII_Num_0 == Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATTESTSHOCKALARM] )
    {
		TangZeLockInterface.keyvalue = KV_UnsetShockAlarm ;
        CheckTestShockAlarmAndResponse();	
    }
    else if( ASCII_Num_1 ==Reveive_Usart1ToDMA.Buff_CPU[Strlen_ATTESTSHOCKALARM])
    {
        TangZeLockInterface.keyvalue = KV_SetShockAlarm ;
        CheckTestShockAlarmAndResponse();
    }
    else
    {
        //SendATCmdResponseCode(RERRORCODE_ElectLockSetFormat);
        ResponseErrorATCmdErrorFormat();
    }
	CloseCyclicSend();
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
ArrInfo CheckTestShockAlarmAndResponse(uint8_t* para)
{
    uint8_t *  arr = (uint8_t*)malloc(StrlenWithData_TESTShockAlarm* sizeof(uint8_t));
	ArrInfo arrInfo = {StrlenWithData_TESTShockAlarm, arr};
    arr[0] = RCODEWithData_TESTShockAlarm ;
    if(KV_UnsetShockAlarm == TangZeLockInterface.keyvalue)
        arr[1]= 0X00 ;			//解锁
    else
        arr[1]= 0X01 ;			//上锁

#if((1 + 1) !=StrlenWithData_TESTShockAlarm)
    #error "StrlenWithData_TESTShockAlarm长度不对"
#endif    
    
    return arrInfo;
    //AT.flag_alreadysent = True ;
}


#endif
