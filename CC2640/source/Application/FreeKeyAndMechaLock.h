#ifndef __FreeKeyAndMechaLock_H
#define __FreeKeyAndMechaLock_H

/*********************Annotate********************************/
/*该部分用来控制免钥匙启动、断开及相关的执行部件失败的处理以及机械锁的控制部分 */
/*********************typedef************************/
typedef enum
{
  Status_PowerOff = 0 ,
  Status_PowerOn, 
  Status_PowerOffAction ,
  Status_PowerOnAction ,
  Status_PowerOffFailure ,
  Status_PowerOnFailure ,
  Status_OCP ,
}StatusFreeKey_ETypeDef;

typedef struct
{
	StatusFreeKey_ETypeDef status ;
	uint8_t counter_retry_whenfailure ;
	uint8_t counter_OCP_warning	;
	uint16_t sum_temp_adc ;
	uint16_t avr_adc ;
	uint8_t counter_sample ;
	uint8_t timer_48VSWActionOuttime_per2ms ;
	
}FreeKey_TypeDef;

typedef struct
{
	FlagStatus flag ;
	uint8_t timer_LDturnoff_per100ms ;
}LDPower_TypeDef;


typedef enum
{
  Status_Unlocked = 0 ,
  Status_Locked, 
  Status_UnlockAction ,
  Status_LockedAction ,
  Status_WaitForMotorStop ,
  Status_UnlockFail_Locked ,
  Status_OTP_Locked ,			//发热保护 此时为上锁状态
}StatusSeatLock_ETypeDef;
  


typedef struct
{
	StatusSeatLock_ETypeDef status ;
	uint8_t timer_action_per100ms ;
	uint8_t counter_unlockactionfrequent ;
	uint8_t timer_unlockactionfrequent_per1s ;
}SeatLock_TypeDef;

typedef enum
{
  Status_MotorUnlocked = 0 ,
  Status_MotorLocked, 
  Status_MotorUnlockAction ,
  Status_MotorLockedAction ,
  Status_WaitForBrake ,			//等待刹车信号 一旦有刹车信号就执行机械锁解锁动作
  Status_MotorWaitForStop ,
}StatusMechalMotorLock_ETypeDef;

typedef struct
{
	StatusMechalMotorLock_ETypeDef status ;
	uint8_t timer_action_per100ms ;
	uint8_t counter_brake_filter ;
	FlagStatus	flag_breakconfirm ;

}MechalMotorLock_TypeDef;




/*****************************************************/
/**********************define************************/
#define	Counter_Retry_WhenPowerActionFailure			3
#define	Counter_OCP_Warning								3


#define	Timer_48VSWPowerOnOuttime_Per2ms			50	//100ms		实测21~30左右
#define	Timer_48VSWPowerOffOuttime_Per2ms			200	//400ms		实测110~130左右
#define	ADC_PowerOff_Thresholds_A					8		//8V
#define	ADC_PowerOff_Thresholds_D					(uint16_t)(ADC_PowerOff_Thresholds_A * LVAnalog2Digital)
#define	ADC_PowerOff_Thresholds_Differ_A			1	//1V
#define	ADC_PowerOff_Thresholds_Differ_D			(uint16_t)(ADC_PowerOff_Thresholds_Differ_A * LVAnalog2Digital)
#define	Timer_LDTurnOff_Per100ms					35	//3.5s		//控制器醒3s 这里放点余量为3.5s

#define Counter_LV_48VSW_Sample_Max					16
#define Counter_LV_48VSW_Sample_Shift				4	//移位
	
#define	Timer_SeatLockLockAction_Per100ms			9	//900ms
#define	Timer_SeatLockUnlockAction_Per100ms			9	//900ms
#define	Timer_SeatLockUnlockedDealy_Per100ms		20	//2000ms =2s
#define	Timer_SeatLockUnlockedActionFrequent_Per1s	40	//40s
#define	Counter_UnlockActionFrequent				2

#define	Timer_MechalMotorLockLockAction_Per100ms			9	//900ms
#define	Timer_MechalMotorLockUnlockAction_Per100ms			9	//900ms

#define	Counter_Brake_Filter						2
	

/**********************DST parament************************/
/* 定义DST的一帧结构为：长时间低电平 + 开始码 + 内容长度 + 内容 + 长度和内容异或校验 +结束码   */
#define		DST_StartCode				0x0A
#define		DST_Strlen_Status			4
#define		DST_Strlen_Code				(1 + DST_Strlen_Status)		//Attention:必须小于16 否则timer_target会溢出
#define		DST_EndCode					0x0A	
#define		DST_CycleTimes				3

/**********************DSR parament************************/
#define		DSR_Timer_Counter_Start_Bit_Per100us				400
#define		DSR_Bytesize_Code					5
#define 	DSR_Device_Code						0x07

#define		Timer_DouSupportRetractAction_Per100ms				55//双撑收起运行7s 可以放点余量
#define		Timer_DouSupportFallAction_Per100ms					55//双撑收起运行7s 可以放点余量
typedef enum
{
	Cmd_Nop = 0 ,
	Cmd_Fall, 
	Cmd_Retract ,
	Cmd_PowerOn ,
	Cmd_PowerOff ,
	Cmd_SetGuard ,
	Cmd_ResetGuard ,
  	Cmd_SearchEBike ,
}CmdDouSupportTrans_TypeDef; 


typedef enum
{
  Status_Retracted = 0 ,
  Status_Fallen, 
  Status_RetractingAction ,
  Status_FallingAction ,
  Status_WaitForPowerOff,
  Status_RetractOuttime ,			
  Status_FallOuttime ,
  Status_RetractOCP ,
  Status_FallOCP ,
}StatusDouSupport_ETypeDef;


typedef struct
{
	Data2Level_TypeDef Data2Level ;
	uint8_t datacode[DST_Strlen_Code];
	uint16_t timer_target[(uint8_t)(DST_Strlen_Code*8*2+2)] ;
	uint8_t counter_tojudgelevel ; 
	CmdDouSupportTrans_TypeDef	cmd ;
	uint8_t counter_cycle ;
	StatusDouSupport_ETypeDef	status ;
}DouSupportTrans_TypeDef;	

//typedef struct
//{
//	Level2Data_TypeDef Level2Data ;
//	uint8_t datacode_temp[OLC_CommonCmd_Strlen];
//	uint8_t datacode[OLC_CommonCmd_Strlen];
//	FlagStatus Flag_code_receive_wrong ;
//}DouSupportReceive_TypeDef;		


typedef struct
{
	StatusDouSupport_ETypeDef status ;
	uint8_t timer_action_per100ms ;	
}DouSupport_TypeDef;
	
//typedef struct
//{
//	FlagStatus status_guard  ;
//	StatusLVSwitch status_LVLevel ;
//}DouSupportTransMessage_Type;
	

/**********************Guard data************************/

/*************************************************/
/*******************Variable**********************/
extern FreeKey_TypeDef	FreeKey ;
extern LDPower_TypeDef	LDPower ;
extern SeatLock_TypeDef SeatLock ;
extern MechalMotorLock_TypeDef	MechalMotorLock ; 

extern DouSupportTrans_TypeDef	DST ;
//extern DouSupportReceive_TypeDef	DSR ;
extern DouSupport_TypeDef		DouSupport ;
//extern DouSupportTransMessage_Type DouSupportTransMessage ; 


/*************************************************/
/*******************function**********************/
#if(External_Anti_theft != Anti_theftDevice_Function)

void FreeKeyPowerOnAction(void) ;
void FreeKeyPowerOffAction(void) ;
void FreeKeyPowerOn(void) ;
void FreeKeyPowerOff(void) ;
void LV48VSWADSample(uint16_t value) ;
void CheckFreeKeyStartActionResult(void) ;
void DetectLD(uint8_t value) ;
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		void SeatLockLockAction(void) ;
		void SeatLockUnlockAction(void) ; 
		void SeatLockStopDrive(void) ;
		void CheckSeatLockActionResult(void) ;
		
		void MechalMotorLockAction(void) ;
		void MechalMotorUnlockAction(void) ;
		void MechalMotorLockStopDrive(void) ;
		void CheckMechalMotorLockActionResult(void) ;
		#if(ElectricDouSupport == Anti_theftDevice_Function)
		void CheckDouSupportActionResult(void) ;
		void SendDouSupportTransCode(void) ; 
		FlagStatus IsReachDouSupportTransChangeLeveltime(uint16_t targettimer) ;
		void LoadDSTTargetTime(void) ;
		void LoadDSTCode(void) ;
		void LoadDST(void);
		FlagStatus AllDSTTransComplete(void) ;
		void SendDouSupportTransIdle(void) ;
		uint8_t CalDSTStatusData(void) ;
		void DouSupportFallAction(void) ;
		void DouSupportRetractAction(void) ;
		
		#endif
	#endif
#endif

#endif
