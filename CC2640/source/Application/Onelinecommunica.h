#ifndef __ONELINECOMMUNICA_H
#define __ONELINECOMMUNICA_H

#include <ti/drivers/PIN.h>

__packed typedef struct
{
	uint8_t bRatedVolt;			// 48/60/64/72/84/96/120;
	uint8_t bBatDieVolt;			// 32-110;
	UnionSumWithH8L8_TypeDef hAvgCrtLmt;			// 10-200;
	UnionSumWithH8L8_TypeDef hPhaseCrtLmt;		// 10-500;
	uint8_t	bHallType;			// 120/60;
	uint8_t bTurboIncCrt;		// 0-33% of hAvgCrtLmt; show warning if >20%;
	uint8_t bTurboDuration;		// 0-30s
	uint8_t bTurboCooldown;		// 30s-300s
	uint8_t bBankScale0;			// %, 10 <= x <= 100
	uint8_t bBankScale1;			// %, 10 <= x <= 100
	uint8_t bBankScale2;			// %, 10 <= x <= 100
	UnionSumWithH8L8_TypeDef hBankCrtLmt0;		// <= hAvgCrtLmt
	UnionSumWithH8L8_TypeDef hBankCrtLmt1;		// <= hAvgCrtLmt
	UnionSumWithH8L8_TypeDef hBankCrtLmt2;		// <= hAvgCrtLmt
	uint8_t bBankTorqueScale0;	// %, 10 <= x <= 100
	uint8_t bBankTorqueScale1;	// %, 10 <= x <= 100
	uint8_t bBankTorqueScale2;	// %, 10 <= x <= 100
	uint8_t bBrakeForceScale;	// %, 0 <= x <= 100
	uint8_t bSpeedLimitScale;	// %, 10 <= x <= 100
	uint8_t bCruiseTimer;		// 3-15s;
	uint8_t bBikeRevSpeed;		// 0-20km/h, only available when closed-loop mode selected;
	uint8_t bFieldWeakenScale;	// 5-30%
	uint8_t bSpeedMeterScale;	// %, 5 <= x <= 100
	uint8_t bBankMode:2;			// 0 = Disabled; 1 = Button-type; 2 = Switch-type;
	uint8_t bCruiseMode:2;		// 0-2 = Disabled / Button-trigger / Timer-trigger;
	uint8_t bBikeRevMode:2;		// 0-2 = Disabled / Open-loop / Closed-loop
	uint8_t bFieldWeakenEn:1;	// 0 = Disabled; 1 = Enabled;
	uint8_t bTurboEn:1;			// 0 = Disabled; 1 = Enabled;
	uint8_t bBikeParkEn:1;		// 0 = Disabled; 1 = Enabled;
	UnionSumWithH8L8_TypeDef hFirmwareVer;		// (major ver).(junior ver)
	UnionSumWithH8L8_TypeDef hDataIntegrityID;	// used to check data integrity, must be last member of this structure.
}UserConfig_TypeDef;

#define		UserConfigData_Strlen				sizeof(UserConfig_TypeDef)


/********************Annotate*********************************/
/*
一线通完整的一帧信息时间为0.678s
*/


/*********************typedef************************/
typedef struct
{
	CommunicaLevel_TypeDef Level;	
	uint8_t data8bit_temp ;
	uint8_t counter_displacement ;
}Level2Data_TypeDef;

typedef struct
{
	CommunicaLevel_TypeDef Level;	
	uint8_t data8bit_temp ;
	uint8_t counter_displacement ;
}Data2Level_TypeDef;

#define		Num_HallSpeed_Filter				4	//一线通大约2.7s 通用版2s


#if(Commu_Mode_Common != Commu_Mode)
/**********************YXT parament************************/
#define		OLC_Counter_Start_Bit				400

/**********************YXT data************************/
#define		OLC_CommonCmd_ValidStrlen			9
#define		OLC_CommonCmd_Strlen				(OLC_CommonCmd_ValidStrlen + 3)
#define 	DeviceCode_CommonCmd					0x08
#define 	SeqCode_Commoncmd					0x61

/**********************Config Controller E2PROM data************************/
#define		OLC_ReadConfigCmd_ValidStrlen		(1 + UserConfigData_Strlen )
#define 	OLC_ReadConfigCmd_Strlen			(OLC_ReadConfigCmd_ValidStrlen + 3)
#define		DeviceCode_ConfigCmd				0X09
#define 	SeqCode_ConfigCmd					0x61

//#if(OLC_ReadConfigCmd_Strlen > OLC_CommonCmd_Strlen)
//	#define		OLC_Strlen		OLC_ReadConfigCmd_Strlen
//#else
//	#define		OLC_Strlen		OLC_CommonCmd_Strlen
//#endif
#define		OLC_Strlen							OLC_ReadConfigCmd_Strlen

typedef enum
{	
	OLCType_Undefined = 0 ,
	OLCType_Unfinished ,
	OLCType_Common ,
	OLCType_ReadControllerE2PROMCfg ,
	OLCType_WriteControllerE2PROMCfg ,
}OLCType_ETypeDef ;


typedef union
{
	uint8_t Data[OLC_Strlen] ;
	struct
	{
		union
		{
			uint8_t byte_0 ;
			uint8_t devicecode ;
		};
		union
		{
			uint8_t byte_1 ;
			uint8_t seqcode ;
		};
		union
		{
			uint8_t byte_2 ;
			uint8_t status1 ;
			struct
			{
				uint8_t flag_clrflagwritenconfigfinished : 1 ;
				uint8_t reserved : 7 ;
			}	;
		
		} ;
		union
		{
			uint8_t byte_3 ;
			uint8_t status2 ;
			uint8_t errorbyte ;
		} ;
		union
		{
			uint8_t byte_4 ;
			uint8_t status3 ;
			uint8_t switchbyte ;
		} ;
		union
		{
			uint8_t byte_5 ;
			uint8_t status4 ;
			uint8_t protectbyte ;
		} ;
		union
		{
			uint8_t byte_6 ;
			uint8_t status5 ;
			uint8_t current_highbyte ;
		} ;
		union
		{
			uint8_t byte_7 ;
			uint8_t status6 ;
			uint8_t hallspeed_highbyte ;
		};
		union
		{
			uint8_t byte_8 ;
			uint8_t status7 ;
			uint8_t	hallspeed_lowbyte ;
		} ;
		union
		{
			uint8_t byte_9 ;
			uint8_t status8 ;
			uint8_t q_percentagebyte ;
		} ;
		union
		{
			uint8_t byte_10 ;
			uint8_t status9 ;
			uint8_t current_lowbyte ;
		};
		union
		{
			uint8_t byte_11 ;
			uint8_t checksum ;
		};
	} ;
	struct
	{
		uint8_t devicecode_readconfig ;
		uint8_t strlen_valid ; 
		uint8_t reply_cmd ; 
		union
		{
			struct
			{
				UserConfig_TypeDef UserConfig ;
				uint8_t checksum_readcfgrply ;
			};
			struct
			{
				uint8_t result_writecfgrply ;
				uint8_t	checksum_writecfgrply ;	
			};
		};
	};
}OLCDataBuff_Typedef ;	

typedef struct
{
	Level2Data_TypeDef	Level2Data ;
	OLCDataBuff_Typedef DataBuff ;
	FlagStatus Flag_code_receive_wrong ;
	OLCType_ETypeDef type ;
}OLC_TypeDef;

typedef struct
{
	uint8_t		rawdata[4] ;
	UnionSumWithH8L8_TypeDef	counter_hall_speed ;
	uint8_t		data_current ;
	uint8_t		data_error ;
	FlagStatus	flag_motor_running ;
	FlagStatus	flag_brake ;
	
	uint32_t	data_Q_thisframe ;
}OLCM_TypeDef;

/*******************Variable**********************/
extern OLC_TypeDef	OLC ;
extern OLCM_TypeDef OLCM ;



#if(Commu_Mode_TwoWire == Commu_Mode)
/*****************************************************/
/*****************************************************/
/**********************define************************/
/**********************CustomSerialInterface parament************************/
/* 定义CSI的一帧结构为：长时间低电平 + 开始码 + 内容长度 + 内容 + 长度和内容异或校验 +结束码   */
#define		CSI_StartCode				0x3A
#define		CSI_Strlen_Status			1
#define		CSI_Strlen_Code				(4 + CSI_Strlen_Status)		//Attention:必须小于16 否则timer_target会溢出
//#define	CSI_StatusCode				0x01			//准备就绪 0x01防盗 0x00解除防盗
//#define	CSI_CRC	
#define		CSI_EndCode					0x0A

#define		Counter_OneKeyRepairSet_Cycle	3

//typedef enum
//{
//	Status_HigherLVlevel = 0 ,
//	Status_LowerLVlevel,
//}StatusLVSwitch;
//	
//typedef struct
//{
//	Data2Level_s Data2Level ;
//	//  uint8_t datacode_temp[CSI_Strlen_StartCode];
//	uint8_t datacode[CSI_Strlen_Code];
//	uint16_t timer_target[(uint8_t)(CSI_Strlen_Code*8*2+2)] ;
//	volatile uint8_t counter_tojudgelevel ;
//}CustomSerialInterface_t;
//
//typedef struct
//{
//	FlagStatus status_guard  ;
//	StatusLVSwitch status_LVLevel ;
//	FlagStatus	status_onekeyrepair ;
//	uint8_t	counter_onekeyrepairset_cycle ;
//}CustomSerialInterfaceMessage_t;
//
//
//extern CustomSerialInterface_t CustomSerialInterface ;
//extern CustomSerialInterfaceMessage_t CustomSerialInterfaceMessage ;
#endif


/***************function******************/
extern uint8_t getGPIOLevel(void);
void ReadOLCLevel(void) ;
void UpdataOLCLevelStatus(void);
void GetOLCLogicValue(void);
void Write8bitToCodeTemp(void);
OLCType_ETypeDef GetOLCType(void);
void WriteToCommuCode(void);
void DetectStartBit(void);
FlagStatus IsDeviceCodeCompliant(uint8_t);
FlagStatus IsSeqCodeCompliant(uint8_t);
FlagStatus IsSumCompliant();
void GetOneLineCommuMessage(void) ;
//void InitialLowPassFilteringPerFrame(void) ;
void LowPassFilteringQPerFrame(void) ;
void LowPassFilteringMileagePerFrame(void);
//uint32_t CalQReleaseInThisFrame(void) ;

#if(Commu_Mode_TwoWire == Commu_Mode)
void GetControllerConfigDataFromContr(void) ;
#if 0
void SendCustomSerialInterfaceCode(PIN_Handle pinHandle);
void PrepareCustomSerialInterfaceCode(void);
void initialConfigCustomSerialInterfaceCode(void);
FlagStatus IsReachRD3ChangeLeveltime(uint16_t targettimer);
void LoadCSITargetTime(void) ;
void LoadCSICode(void);
uint8_t CalCSIStatusData(void) ;
#endif
void GetReadControllerConfigCmdReplyMessage(void);
void GetWriteControllerConfigCmdReplyMessage(void);
void GetReadConfigReply(void) ;
void GetWriteConfigReply(void) ;
#endif
#endif
#endif
