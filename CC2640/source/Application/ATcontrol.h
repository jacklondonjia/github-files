#ifndef __ATCONTROL_H
#define __ATCONTROL_H

//#include "TypeDefines.h"
//
//#include "BLESystem.h"


/****************************************************/
/*********************typedef************************/

typedef struct
{
	/******************status*************************/
	FlagStatus status_BLEconnect ;
	BUZZER_TypeDef	Buzzer ;
	FlagStatus flag_alreadysent ;
	FlagStatus	mode_cyclicsend ;
	//uint8_t	status_error_realtime ;
	//	uint8_t	timer_cyclicsend_per2ms ;
	/***********************LV***********************/
}AT_TypeDef;

typedef struct 
{
	unsigned int n;//array length
	void *arr;
} ArrInfo;
typedef ArrInfo (*responseStr_t)( uint8_t * );

typedef struct
{
	const uint8_t * compare_str;//the string to compare with incoming string
	const uint8_t compare_str_len;
	responseStr_t ResponseStr;//the function return the string
}ResponseStrategy;
/****************************************************/
/**********************define************************/
/*****************BLE*******************************/
#define BLE_RST(a) 	if(a)	\
						GPIO_SetBits(Port_BLE_RST,Pin_BLE_RST);	\
					else	\
						GPIO_ResetBits(Port_BLE_RST,Pin_BLE_RST)
							
/*********************HC-08 ATCmd*******************/
#define 	Strlen_BLENAME_MAX			12
#define		Strlen_Data_Deadline		( 2 + 1 + 1 + 1 )
#define     Strlen_Data_DevInfo         ( 1 + 2 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 )
#define		Strlen_Mileage_Max			4

							
/*****************BLE_ATCmdResponseCode**************/
#define		RCODEWithData_ErrorATCmd			0XE0
#define		RCODEWithData_BLEConnect			0X01
#define		RCODEWithData_DevInfo			    0X04  
#define		RCODEWithData_DeadLine				0X02
#define		RCODEWithData_GuardStatus			0X03					
#define		RCODEWithData_ErrorStatus			0X05
#define		RCODEWithData_Q 					0X06
#define		RCODEWithData_Mileage			    0X07  
#define		RCODEWithData_ErrorHistory			0X08
#define		RCODEWithData_CyclicSend			0X09
#define		RCODE_ReceiveNameSuccess			0X0A
#define		RCODEWithData_HallSpeed				0X0B
#define		RCODEWithData_ControllerConnect		0X0C
#define		RCODEWithData_HardwareVersion		0X0E
#define		RCODEWithData_ElectLock				0X0F
#define		RCODEWithData_UnLockLevel1LV		0X0D
#define		RCODEWithData_SearchEbike			0X10
							
#define		RCODEWithData_FreeKeyStart			0X11
#define		RCODEWithData_SeatLock				0X12
#define		RCODEWithData_DouSupport			0X13
#define		RCODEWithData_MechalLock			0X14							
#define		RCODEWithData_AllParkFinish			0X15
#define		RCODEWithData_CFileNum				0X16
#define		RCODEWithData_ThreeSpeed			0X17
#define		RCODEWithData_ThreeTorque			0X18
#define		RCODEWithData_ContrE2Cfg			0X19

							
#define		RCODEWithData_OneKeyRepair			0X1A
							
#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
	#define		RCODEWithData_TESTMotorLock			0X12
	#define		RCODEWithData_TESTSeatLock			0X13
	#define		RCODEWithData_TESTShockAlarm		0X14							
#endif
							
/*************BLE_ATCmdResponse Strlen******************/
#define     StrlenWithData_ErrorATCmd           (1 + 1)
#define     StrlenWithData_BLEConnect           1
#define		StrlenWithData_GuardStatus			(1 + 1)
#define     StrlenWithData_DevInfo              (1 + 1 + Strlen_Data_DevInfo )
#define     StrlenWithData_DevInfoSet			(1 + 1 + 1) 
#define		StrlenWithData_ElectLock			(1 + 1)
#define		StrlenWithData_ErrorStatus			(1 + 1)
#define		StrlenWithData_ErrorHistory			(1 + 1 + 1)
#define		StrlenWithData_ErrorHistoryDetail	(1 + 1 + RunMessageSingleEntryLen)	
#define		StrlenWithData_DeadLine				(1 + 1 + Strlen_Data_Deadline)					
#define		StrlenWithData_Q			        (1 + 4 + 4 + 4)
#define		StrlenWithData_Mileage				(1 + 4 + 4 + 4 + 3 )
#define 	StrlenWithData_HallSpeed			(1 + 2)
#define     StrlenWithData_ControllerConnect	(1 + 1)
#define     StrlenWithData_BLERename           	1							
#define 	StrlenWithData_HardwareVersion		(1 + 2 + 3)
#define 	StrlenWithData_UnLockLevel1LV		(1 + 1)
#define		StrlenWithData_OneKeyRepair			(1 + 1)
#define		StrlenWithData_SearchEbike			(1 + 1)
#define		StrlenWithData_CyclicSend			(1 + 1 + 4 + 4 + 2 + 2 + 2)

							
#define		StrlenWithData_FreeKeyStart			(1 + 1)
#define		StrlenWithData_SeatLock				(1 + 1)
#define		StrlenWithData_DouSupport			(1 + 1)
#define		StrlenWithData_MechalLock			(1 + 1)							
#define		StrlenWithData_AllParkFinish		(1 + 1)
#define		StrlenWithData_CFileNum				(1 + 1 + 5 )		
#define		StrlenWithData_ThreeSpeed			(1 + 1 + 1)
#define		StrlenWithData_ThreeTorque			(1 + 1 + 1)
#define		StrlenWithData_ReadContrE2Cfg		(1 + 1 + 1 + 1 + 16)
#define		StrlenWithData_WriteContrE2Cfg		(1 + 1 + 1 + 1)

							
#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)							
	#define		StrlenWithData_TESTMotorLock		(1 + 1)	
	#define		StrlenWithData_TESTSeatLock			(1 + 1)
	#define		StrlenWithData_TESTShockAlarm		(1 + 1)	
#endif
							
/*************ATCmd Error Define*****************/ 
#define    	ErrorATCmd_Unrecognized             0X01
#define     ErrorATCmd_ErrorFormat              0X02
#define     ErrorATCmd_ConncetCmdNotYetSent     0X03
#define     ErrorATCmd_ConncetCmdSendRepeatedly 0X04  

/*************ATCmdByte*****************/ 
#define    	ATCmdByte_Cmd            			0X00
#define    	ATCmdByte_Action           			0X01

#define		ATCmdByte_Status					0X02
#define		ATCmdByte_BlockNum					0X02

/*************ATActionCode*****************/ 
#define    	ATActionCode_Query            		0X00
#define    	ATActionCode_Config_Defalt          0X01

#define		ATActionCode_Config_Second			0X02

							
/*************ATCmd ConnectWithController Define*****************/ 
#define         NoConnectWithController         0x00
#define         ConnectWithController           0x01
							
/*************ATCmd Set Define*****************/ 
#define         SetSuccess     					0x01
#define         SetFailure				  		0x00	

#define			ExecutionWait					0X02

							
/*************ATCmd Inquiry Define*****************/ 
#define         ReplyInquiry     				0x00							
							
/*************Timer******************/
#define     Timer_ConnectWithApp_Per2ms         600      //1.2s   
#define		Timer_BLEResetDelay_Per2ms			100			//200ms

#define		Hours_PerDay						24
#define		Minutes_PerHour						60
#define		Seconds_PerMinute					60


/*************DevInfoSet******************/
#define		Pole_Pairs_Tricycle_Standard		4	//????????
#define		Pole_Pairs_TwoWheel_Min				23
#define		Pole_Pairs_TwoWheel_Max				50
#define		Wheel_Diameter_Inch_Int_Min			16
#define		Wheel_Diameter_Inch_Int_Max			40
#define		AH_Battery_Min						12

/****************************************************/
/*********************typedef************************/
typedef enum
{
	Rename_Complete = 0 ,
	Rename_BLEConnect  ,
	Rename_BLEDisconnect ,
}Rename_BLEConStatus_Type;

typedef struct
{
	uint8_t arr[8 + 12] ;		//uint8_t arr[Strlen_ATNAME + Strlen_BLENAME_MAX];
	uint8_t strlen_BLENameset ;
	Rename_BLEConStatus_Type BLEConStatus ;
	uint8_t timer_BLEResetDelay_per2ms ;
}BLEReName_Type ;



//typedef struct
//{
//	/******************status*************************/
//	FlagStatus status_BLEconnect ;
//	BUZZER_s Buzzer ;
//	FlagStatus flag_alreadysent ;
//	FlagStatus	mode_cyclicsend ;
//	BLEReName_Type BLEReName ;
//	//uint8_t	status_error_realtime ;
//	//	uint8_t	timer_cyclicsend_per2ms ;
//	/***********************LV***********************/
//}AT_TypeDef;	

							
/*************************************************/
/*******************Variable**********************/
extern AT_TypeDef AT ;
														
/****************************************************/
/**********************function**********************/
void 		processDisconnectWithBLEApp();
uint8_t 	getCurrentCommandLength();
ArrInfo 	ATControl(uint8_t* in, const uint8_t len);
ArrInfo 	ResponseErrorATCmd(void);
							
void 		ResponseErrorATCmdErrorFormat(void) ;
ArrInfo 	ResponseErrorATCmdConncetCmdNotYetSent(uint8_t* para);
ArrInfo 	ResponseErrorATCmdConncetCmdSendRepeatedly(uint8_t* para);
ArrInfo 	ResponseATBLEConnect(uint8_t* para);
//void SetBLERFPowerMax(void);
void 		FillATDevInfoDataMessageAndSend(uint8_t* para) ;
ArrInfo 	ResponseATDevInfo(uint8_t* para);
FlagStatus 	IsATDevInfoSetFormatCorrect(uint8_t* para) ;
ArrInfo 	ResponseATDevInfoSet(uint8_t* para);

ArrInfo 	ResponseATRentDeadline(uint8_t* para);
FlagStatus 	IsDeadLineSetFormatCorrect(uint8_t* para) ;
ArrInfo 	ResponseDeadLineInquire(void);
ArrInfo 	ResponseATGuardStatus(uint8_t* para);
void 		CheckGuardStatusAndResponse() ;
ArrInfo 	ResponseConntrollerConnectCheck(uint8_t* para);
ArrInfo 	ResponseATElectLock(uint8_t* para);
void 		CheckElectLockAndResponse() ;
ArrInfo 	ResponseATSearchEbike(uint8_t* para);

ArrInfo 	NOPFunction(uint8_t* para) ;
//void SendATErrorStatusRealTimeWithNoAsk(void);

							
#if(Commu_Mode_Common != Commu_Mode)
ArrInfo 	ResponseATErrorStatus(uint8_t* para);
	uint8_t GetErrorStatus(void);
	ArrInfo ResponseATQ(uint8_t* para);
	ArrInfo ResponseATErrorHistory(uint8_t* para);
    FlagStatus IsErrorHistoryDetailInquireFormatCorrect(void) ;
    ArrInfo ResponseErrorHistoryEntriesNum(uint8_t* para) ;
    ArrInfo ResponseErrorHistoryDeleteSuccess(uint8_t* para) ;
    ArrInfo ResponseErrorHistoryDetail(uint8_t* para);
	#if(Commu_Mode_TwoWire == Commu_Mode)
		#if(COMPILE_ENABLE == OneKeyRepairSet)
    ArrInfo ResponseOneKeyRepair(uint8_t* para) ;
		#endif
	
		#if(COMPILE_ENABLE == Double_LVSet)
    ArrInfo ResponseLV1LevelUnlockSet(uint8_t* para);
		#endif
		
		#if(COMPILE_ENABLE == ThreeSpeedSet)
	ArrInfo ResponseThreeSpeed(uint8_t* para);	
	void ResponseThreeSpeedQuery(void);	
	void ResponseThreeSpeedConfig(void);
		#endif
		
		#if(COMPILE_ENABLE == ThreeTorqueSet)
	ArrInfo ResponseThreeTorque(uint8_t* para);
	void ResponseThreeTorqueQuery(void);
	void ResponseThreeTorqueConfig(void);	
		#endif
		
	ArrInfo ResponseControllerE2PROMConfig(uint8_t* para) ;
	void ResponseReadControllerE2PROMConfigStart(void) ;
	void ResponseReadControllerE2PROMConfigFail(void) ;
	//void ResponseReadControllerE2PROMConfigFinished(uint8_t* para) ;
	void ResponseReadControllerE2PROMConfigBlock(void) ;
	void UpdateControllerE2PROMConfigBlockFromATCmd(uint8_t* para) ;	
	void ResponseWriteControllerE2PROMConfigBlock(void) ;
	void ResponseWriteControllerE2PROMConfigFail(void) ;
	void ResponseWriteControllerE2PROMConfigFinished(void) ;
		
	#endif		
#endif 	
	
ArrInfo ResponseATMileage(uint8_t* para);
ArrInfo ResponseATHallSpeed(uint8_t* para);
ArrInfo ResponseHardwareVersion(uint8_t* para);
ArrInfo ResponseCustomerFileNum(uint8_t* para);

ArrInfo ResponseATCyclicSend(uint8_t* para);
void CyclicSendData();
void CloseCyclicSend(void) ;
ArrInfo ResponseAndSetChangeBLEModuleName(uint8_t* para);
uint8_t GetBLENameStrlenFromApp(void);

#if(External_Anti_theft != Anti_theftDevice_Function)
ArrInfo ResponseFreeKeyStart(uint8_t* para) ;
void ResponseFreeKeyStartAction(void) ;
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
			ArrInfo ResponseSeatLock(uint8_t* para) ;
			void ResponseSeatLockAction(void) ;
			void CheckMechalMotorLockStatusAndResponse(void) ;
			ArrInfo ResponseAllParkFinish(uint8_t* para) ;
		#if(ElectricDouSupport == Anti_theftDevice_Function)
			ArrInfo ResponseDouSupport(uint8_t* para) ;
			void ResponseDouSupportAction(void) ;
		#endif
#endif
#endif
							
#endif
