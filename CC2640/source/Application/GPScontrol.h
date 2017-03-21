#ifndef __GPSCONTROL_H
#define __GPSCONTROL_H
/****************说明********************************

*****************************************************************************/


// Task configuration
#define UART0_TASK_PRIORITY                     1

#ifndef UART0_TASK_STACK_SIZE
#define UART0_TASK_STACK_SIZE                   644//512
#endif

//extern UART_Handle pGetUart0Handle(void) ;
//extern void WriteUart0((uint8_t *, size_t);

//extern void Uart0_createTask(void) ;
extern void SerialAppreadCallback(UART_Handle , void *, size_t ) ;
extern void Uart_init(void) ;
#if 0
/****************************************************/
/*********************typedef************************/
typedef enum
{
  Process_GPSPowerOnInitial = 0 ,		//None New or Disconnect
  //Process_ReConnect ,		//Get new connect package
  Process_HostTypeSent ,
  Process_HostTypeResp ,
  Process_DeviceNumSent ,
  Process_DeviceNumResp ,
  Process_QueryGPSStatusSent ,
  Process_QueryGPSStatusResp ,
  Process_Passthrough ,
  //Process_PassthroughSent ,
  //Process_PassthroughResp ,
  
}ProcessGPSSend_Type ;

typedef struct
{
	FlagStatus 			status ;
	uint8_t 			timer_sendconnectpackage_per100ms ;
	uint8_t 			timer_outdetect_per100ms ;		//detect		
}Connect_Typedef ;



//typedef struct
//{
//	uint8_t				num_waitforcmdresponse ;
//}CmdControl_Type ;


//typedef struct
//{
//	//FlagStatus			flag_newdata ;
//	//FlagStatus			flag_sendconfirmed ;
//	UnionSumWithH8L8	serialnum_waitforresponse ;
//}PassagethroughSend_Typedef ;

typedef struct
{
	//FlagStatus			flag_newdata ;

	uint8_t 			Data[RECEIVEBUFF_SIZE - 12] ;		//50-12 = 38
#if(RECEIVEBUFF_SIZE <= 12)
	#error "数组长度不对"
#endif
	uint8_t 			counter_data ;
	//FlagStatus			flag_receiveconfirmed ;
	UnionSumWithH8L8	serialnum_forresponse ;
}PassagethroughRec_Typedef ;


typedef struct
{
	/******************status*************************/
	Connect_Typedef				Connect ;	
	ProcessGPSSend_Type			process_send ;
	
	UnionSumWithH8L8 			num_serial ;
	
	//CmdControl_Type				CmdControl ;
	//PassagethroughSend_Typedef 	PassagethroughSend ;
	PassagethroughRec_Typedef	PassagethroughRec ;

	TargetBuff_Type				connect_target	;
	
	
	
	/******************GPS status from GPScommu*************************/
	FlagStatus			status_GPS_hardware ;
	FlagStatus			status_TCPIP ;
	FlagStatus			flag_server_gothosttype ;
	FlagStatus			flag_server_registerdevicenum ;
	FunctionalState		flag_GPRS_vibrationalarm ;

	
}GPS_Typedef;

/****************************************************/
/**********************define************************/
/**********************Package Format************************/
#define		GPS_StartByte_1						0X24
#define		GPS_StartByte_2						0X24

#define		GPS_CheckByte_2						0XAA

#define		GPS_StopByte_1						0X0D
#define		GPS_StopByte_2						0X0A

#define		GPS_Start1_ArrNum					0
#define		GPS_Start2_ArrNum					1
#define		GPS_ValidDataLen_ArrNum				2
#define		GPS_Passway_ArrNum					3
#define		GPS_Cmd_ArrNum						4

/**********************Passageway************************/
#define		CM2PT								0X01
#define		CPT2M								0X02
#define		CPT2S								0X03
#define		CS2PT								0X04
#define		MM2PT								0X05
#define		MPT2M								0X06
#define		MPT2S								0X07
#define		MS2PT								0X08

/**********************Cmd ID************************/
#define		Cmd_SetHostType						0X01
#define		Cmd_GPRSControlLock					0X05
#define		Cmd_SetDeviceNum					0X06
#define		Cmd_UpdateDeviceNum					0X07
#define		Cmd_QueryACCOffVoltage				0X0B
#define		Cmd_QueryGPSStatus					0X10
#define		Cmd_ConnectPackage					0XDF


	
/**********************ReCmd ID************************/
#define		ReCmd_SetHostType					0X01
#define		ReCmd_GPRSControlLock				0X05
#define		ReCmd_SetDeviceNum					0X06
#define		ReCmd_UpdateDeviceNum				0X07
#define		ReCmd_QueryGPSStatus				0X10
#define		ReCmd_ConnectPackage				0XDF	

/**************Strlen Data Cmd************/
#define		Strlen_ValidFixed_ExceptN			6

#define		StrlenN_Send_HostType				1
#define		StrlenN_Send_GPRSControlLock		1
#define		StrlenN_Send_SetDeviceNum			12
#define		StrlenN_Send_UpdateDeviceNum		12
#define		StrlenN_Send_QueryGPSStatus			1
#define		StrlenN_Send_QueryACCOffVoltage		1
#define		StrlenN_Send_ConnectPackage			1

/**************Strlen Data ReCmd************/
#define		StrlenN_Rec_HostType				1
#define		StrlenN_Rec_GPRSControlLock			1
#define		StrlenN_Rec_SetDeviceNum			1
#define		StrlenN_Rec_UpdateDeviceNum			1
#define		StrlenN_Rec_QueryGPSStatus			3
#define		StrlenN_Rec_ConnectPackage			1

/**************Message passthrough CmdID************/
#define		MCmd_PassthroughSend				0XAA
#define		MCmd_PassthroughResponse			0XA0


/**************Strlen Data PassThrough************/
#define		StrlenValidData_Test1				5

	
/**************Strlen Data ReMessage************/	
#define		StrlenN_Rec_PassthroughSend			1

/**************Timer ConnectPackage************/
#define		Timer_GPSconnect_SendPeriod_Per100ms	30	//3s
#define		Timer_GPSconnect_Outtime_Per100ms		90	//9s
#define		Timer_GPSNoResponse_Outtime_Per100ms	10	//1s

					
///*************ATCmd Error Define*****************/ 
//#define    	ErrorATCmd_Unrecognized             0X01
//#define     ErrorATCmd_ErrorFormat              0X02
//#define     ErrorATCmd_ConncetCmdNotYetSent     0X03
//#define     ErrorATCmd_ConncetCmdSendRepeatedly 0X04  
//							
///*************ATCmd ConnectWithController Define*****************/ 
//#define         NoConnectWithController         0x00
//#define         ConnectWithController           0x01
//							
///*************ATCmd Set Define*****************/ 
//#define         SetSuccess     					0x01
//#define         SetFailure				  		0x00	
//							
///*************ATCmd Inquiry Define*****************/ 
//#define         ReplyInquiry     				0x00							
//							
///*************Timer******************/
//#define     Timer_ConnectWithApp_Per2ms         600      //1.2s   
//							
//#define		Hours_PerDay						24
//#define		Minutes_PerHour						60
//#define		Seconds_PerMinute					60
							
/*************************************************/
/*******************Variable**********************/
extern GPS_Typedef GPS ;
extern DMA_InitTypeDef DMA_InitStructureTX2;
														
/****************************************************/
/**********************function**********************/
void ResponseTX2Test(void) ;
void GPSControl(void) ;
void (*MatchRX2Response(void))() ;
FlagStatus IsReceivedRX2FormatRight(void) ;
void RX2ErrorFormatFunction(void) ;
void RX2ProcessError(void) ;
FlagStatus IsCPT2M(void) ;
FlagStatus IsSameWithReCmd(uint8_t );
void UndefinedCommand(void) ;
void SetGPSConnectFlag(void) ;
void HandleReCmdWhenUnconnect(void) ;
//void SetConnectPackageBuffToEmpty(void) ;
void ConfirmHostTypeSet(void) ;
void ReceiveGPRSControlLockCmd(void) ;
void ConfirmDeviceNumSet(void) ;
void ConfirmDeviceNumUpdated(void) ;
void GetGPSStatus(void) ;


void GetPassagethroughMessageAndSendReceipt(void) ;
TargetBuff_Type GetPassagethroughReceiptMatchBuff(void) ;
void GetPassagethroughReceipt(void) ;
void IfNoMatchTargetBuff(void) ;

//void SendTestATCmd(void) ;
//void ATControl(void);
//void (*MatchFreeATCmd(void))();
//FlagStatus CmpStrWithFreeATCmd(const uint8_t *,uint8_t);
//void ResponseErrorATCmd(void);
//							
//void ResponseErrorATCmdErrorFormat(void) ;
//void ResponseErrorATCmdConncetCmdNotYetSent(void);
//void ResponseErrorATCmdConncetCmdSendRepeatedly(void);
////void SetBLERFPowerMax(void);
void MatchGPSSendProcess(void) ;
//FlagStatus IsMetConditionOfSendToSetHostType(void) ;
//FlagStatus IsMetConditionOfSendToSetDeviceNum(void) ;
//FlagStatus IsMetConditionOfSendToQueryGPSStatus(void) ;
TargetBuff_Type GetBuffNumWhichHaveConnectPackage(void) ;
void SendM2PTConnectPackage(void) ;
void SendToSetHostType(void) ;
void ResponseGPRSControlLock(void) ;
void SendToSetDeviceNum(void) ;
void SendToUpdateDeviceNum(void) ;
void SendToQueryGPSStatus(void) ;

#if TEST_COMPILE
	void SendToQueryACCOffVoltage(void) ;
#endif

void SendPassThroughTest1(void) ;






//void ResponseATBLEConnect(void) ;
//void ResponseATDevInfo(void);
//FlagStatus IsATDevInfoSetFormatCorrect(void) ;
//void ResponseATRentDeadline(void);
//FlagStatus IsDeadLineSetFormatCorrect(void) ;
//void ResponseDeadLineInquire(void);
//void ResponseATGuardStatus(void);
//void CheckGuardStatusAndResponse(void) ;
//void ResponseConntrollerConnectCheck(void);
//void ResponseATElectLock(void);
//void CheckElectLockAndResponse(void) ;
//void ResponseATSearchEbike(void);
//
////void SendATErrorStatusRealTimeWithNoAsk(void);
//
//							
//#if(Commu_Mode_Common != Commu_Mode)
//	void ResponseATErrorStatus(void);
//	uint8_t GetErrorStatus(void);
//	void ResponseATQ(void);
//	void ResponseATErrorHistory(void);
//    FlagStatus IsErrorHistoryDetailInquireFormatCorrect(void) ;
//	void ResponseErrorHistoryEntriesNum(void) ;
//	void ResponseErrorHistoryDeleteSuccess(void) ;
//	void ResponseErrorHistoryDetail(void);
//	#if(Commu_Mode_TwoWire == Commu_Mode)
//		#if(COMPILE_ENABLE == OneKeyRepair)
//	void ResponseOneKeyRepair(void) ;
//		#endif
//	
//		#if(COMPILE_ENABLE == Double_LV)
//			void ResponseLV1LevelUnlockSet(void);
//		#endif
//	#endif		
//#endif 	
//	
//void ResponseATMileage(void);
//void ResponseATHallSpeed(void);
//void ResponseHardwareVersion(void);
//void ResponseATCyclicSend(void);
//void CyclicSendData(void);
//void CloseCyclicSend(void) ;
//void ResponseAndSetChangeBLEModuleName(void);
//uint8_t GetBLENameStrlenFromApp(void);
//
//#if(External_Anti_theft != Anti_theftDevice_Function)
//	void ResponseFreeKeyStart(void) ;
//	void ResponseFreeKeyStartAction(void) ;
//	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
//		void ResponseSeatLock(void) ;
//		void ResponseSeatLockAction(void) ;
//		void CheckMechalMotorLockStatusAndResponse(void) ;
//		void ResponseAllParkFinish(void) ;
//		#if(ElectricDouSupport == Anti_theftDevice_Function)
//		void ResponseDouSupport(void) ;
//		void ResponseDouSupportAction(void) ;
//		#endif
//	#endif
//#endif
//
//#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
//	void ResponseTestPowerLock(void) ;
//	void CheckTestPowerLockAndResponse(void) ;
//	void ResponseTestMotorLock(void) ;
//	void CheckTestMotorLockAndResponse(void) ;
//	void ResponseTestSeatLock(void) ;
//	void CheckTestSeatLockAndResponse(void) ;
//	void ResponseTestShockAlarm(void) ;
//	void CheckTestShockAlarmAndResponse(void) ;
//#endif

#endif							
#endif