#include "stdint.h"
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

//sensor controller
#include "scif.h"

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/timer/GPTimerCC26XX.h>
#include <xdc/runtime/Types.h>
#include <uart_logs.h>
#include "Board.h"

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

//#include "PeriodicTimer.h"
//#include "Tool_Functions.h"
//#include "FlashSNV.h"
#include "CustomerPara.h"
#include "_Annotate.h"

// Display error message if the SCIF driver has been generated with incorrect operating system setting
#ifndef SCIF_OSAL_TIRTOS_H
    #error "SCIF driver has incorrect operating system configuration for this example. Please change to 'TI-RTOS' in the Sensor Controller Studio project panel and re-generate the driver."
#endif

// Display error message if the SCIF driver has been generated with incorrect target chip package
#ifndef SCIF_TARGET_CHIP_PACKAGE_QFN48_7X7_RGZ
    #error "SCIF driver has incorrect target chip package configuration for this example. Please change to 'QFN48 7x7 RGZ' in the Sensor Controller Studio project panel and re-generate the driver."
#endif

#define BV(n)               (1 << (n))

GPTimerCC26XX_Handle hTimer;

/* Pin driver handles */
static PIN_Handle gpioPinHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State gpioPinState;

PIN_Config AppGpioTable[] = 
{
	//output pins
	Pin_T2C          | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_BLE_Connect  | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_BUZZER		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_DCJD		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_SCJC		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_CPMOS		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_MotorD1		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_MotorD2		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_MotorA2		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_MotorA1		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_9VSW		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Pin_Guard_Output | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	//input pins
	Pin_YXT			 | PIN_INPUT_EN  | PIN_NOPULL | PIN_HYSTERESIS,
	Pin_LD			 | PIN_INPUT_EN  | PIN_NOPULL,
	Pin_Guard_Input	 | PIN_INPUT_EN  | PIN_PULLUP | PIN_HYSTERESIS,                    /* UART TX pin at inactive level */
    PIN_TERMINATE                                                                               /* Terminate list                */
};

/* Queue for receiver pin level change time*/
volatile struct Buffer rx_buffer = { {0}, 0, 0 };
/* Level tracker for last known receiver pin state and passed tick time*/
static Level_Tracker levelTracker;
static uint8_t timer_500ms_per100ms = 0 ;
static uint16_t timer_1s_per100ms = 0 ;
static uint8_t timer_30s_p1s = 0 ;

/*******************************************************************************
 * Exception handler
 ******************************************************************************/

/* Task data */
Task_Struct timerTask;
char timerTaskStack[512];
Task_Struct twomsTask;
Char twomsTaskStack[512];
static volatile uint16_t timerCounter = 0;

/*******************************************************************************
 * Called by other modules to get the gpioPinHandle
 ******************************************************************************/
PIN_Handle getGpioPinHandle()
{
  if(!gpioPinHandle)
  {
	gpioPinHandle = PIN_open(&gpioPinState, AppGpioTable);
  }
  return gpioPinHandle;
}

/*******************************************************************************
 * 
 ******************************************************************************/
void scTaskAlertCallback(void) 
{

} // taskAlertCallback

/*******************************************************************************
 * 
 ******************************************************************************/
void scCtrlReadyCallback(void) 
{
} // ctrlReadyCallback

/*******************************************************************************
 * Initialize the Sensor Controller
 ******************************************************************************/
void setupSC()
{
    // Initialize the Sensor Controller
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(scCtrlReadyCallback);
    scifOsalRegisterTaskAlertCallback(scTaskAlertCallback);
    scifInit(&scifDriverSetup);
    scifStartRtcTicksNow(0x00010000 / 10);
    // Configure and start the ADC Data Logger task. The task does not signalize data exchange, but
    // has buffering capacity for 128 samples = 12.8 seconds (which are polled every 10 seconds below)
    scifStartTasksNbl(BV(SCIF_AD_SCAN_TASK_ID));
}

/*******************************************************************************
 * Timer ISR HWI
 * Minimize processing in interrupt.
 ******************************************************************************/
void timerCallback(GPTimerCC26XX_Handle handle,
		GPTimerCC26XX_IntMask interruptMask) 
{
	// interrupt callback code goes here. Minimize processing in interrupt.
	/**Receive Custom Serial Interface Code*/
	levelTracker.ticks_since_level_changed++;
	//get current level
	uint8_t gpio_lev = getGPIOLevel();
	if(gpio_lev!= levelTracker.lastLevel)
	{ 
		//input pin level changed
		//negative means low level, positive means high level, absolute value is corresponding level's duration
		//if current level is high(>0), we detected the end of a negative level
		int element = gpio_lev>0?-levelTracker.ticks_since_level_changed:levelTracker.ticks_since_level_changed;
		//Put the level time into queue, it will be read out later in a RTOS task
		bufferWrite(&rx_buffer, element);
		//update the new level
		levelTracker.lastLevel = gpio_lev;
		//reset the level tracker counter
		levelTracker.ticks_since_level_changed=0;
	}
#if(Commu_Mode_TwoWire == Commu_Mode)
	T2C.SIF.flag_100us = True  ;
	/**Send Custom Serial Interface Code*/
	//SendCustomSerialInterfaceCode(gpioPinHandle);
	T2CSIFSend_Service();
#endif
	
#if(ElectricDouSupport == Anti_theftDevice_Function)
	DST.Data2Level.Level.timer_temp ++ ;
#endif
}

/*******************************************************************************
 * 
 ******************************************************************************/
void PeriodicTimer_taskFxn(UArg a0, UArg a1)
{	
	// Open Sender pins
	gpioPinHandle = getGpioPinHandle();//PIN_open(&gpioPinState, BoardGpioInitTable);
	  if(!gpioPinHandle) 
	  {
	    //Log_error0("Error initializing board T2C pin");
	    Task_exit();
	  }
	GPTimerCC26XX_Params params;
	GPTimerCC26XX_Params_init(&params);
	params.width = GPT_CONFIG_16BIT;
	params.mode = GPT_MODE_PERIODIC_UP;
	params.debugStallMode = GPTimerCC26XX_DEBUG_STALL_OFF;
	hTimer = GPTimerCC26XX_open(CC2650_GPTIMER0A, &params);
	if (hTimer == NULL) 
	{
		//Log_error0("Failed to open GPTimer");
		Task_exit();
	}
  	LVInitial();
	VariableInitial();

	//This function returns the CPU frequency in Hz.
	Types_FreqHz freq;
	BIOS_getCpuFreq(&freq);								
	
	//this chip clock speed is 48Mhz
	// freq is struct with 32bit hi and 32bit lo, 48M < 2(32bit)
	GPTimerCC26XX_Value loadVal = freq.lo / 10000 - 1; //4799		excute per 100us 
	GPTimerCC26XX_setLoadValue(hTimer, loadVal);
	GPTimerCC26XX_registerInterrupt(hTimer, timerCallback, GPT_INT_TIMEOUT);

	GPTimerCC26XX_start(hTimer);
	while (1) 
	{
		//Task_sleep(BIOS_WAIT_FOREVER);
		Task_sleep(100*1000/Clock_tickPeriod);//100ms
//#if(Commu_Mode_TwoWire == Commu_Mode)
//		if(T2C.Data2Level.Level.timer_temp > 30000)
//		{
//			T2C.Data2Level.Level.timer_temp = 0;
//		}
//#endif
	    if(timer_500ms_per100ms < UINT8_MAX)
		{
	    	timer_500ms_per100ms ++ ;
	    }
	    if(timer_1s_per100ms < UINT16_MAX)
		{
	        timer_1s_per100ms ++ ;
	    }
	    processOneLineQueue();
		EventPer100ms();
		EventPer500ms();
		EventPer1s();
		EventPer30s();
		

	}
}

/*******************************************************************************
 * 
 ******************************************************************************/
void processOneLineQueue()
{
		int16_t temp_timer;
		//read one level time from queue
		while(bufferRead(&rx_buffer, &temp_timer) == BUFFER_OK)
		{
			if(temp_timer > 0)
			{//high level time
				OLC.Level2Data.Level.status_temp = Low_Level;//end of high level is low level
				OLC.Level2Data.Level.timer_temp = temp_timer;
			} 
			else 
			{//low level time
				OLC.Level2Data.Level.status_temp = High_Level;//end of low level is high level
				OLC.Level2Data.Level.timer_temp = -temp_timer;
			}
			//TODO: remove this after debugging
//			if(OneLineCommu.Level2Data.Level.timer_temp > 400){
//				PIN_setOutputValue(gpioPinHandle, Pin_BLE_Connect, !PIN_getInputValue(Pin_BLE_Connect));
//			}
			ReadOLCLevel();
		}
}

/*******************************************************************************
 * 
 ******************************************************************************/
void twomsTaskFxn(UArg a0, UArg a1) 
{

	while (1) 
	{
		Task_sleep(2*1000/Clock_tickPeriod);//2ms
		mainLoopFunctions();
		EventPer2ms();
	}
}

/*******************************************************************************
 * 100ms main loop
 ******************************************************************************/
void mainLoopFunctions(void)
{
//TODO: uncomment this and remove the one in 100ms task after debugging
//	processOneLineQueue();
	GuardControl();


#if(External_Anti_theft != Anti_theftDevice_Function)
	CheckFreeKeyStartActionResult();
	DetectLD((uint8_t)PIN_getInputValue(Pin_LD));
#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
	CheckSeatLockActionResult();
	CheckMechalMotorLockActionResult();
	#if(ElectricDouSupport == Anti_theftDevice_Function)
	if(False == AllDSTTransComplete() )
		SendDouSupportTransCode();		//发送码值
	else
		SendDouSupportTransIdle();		//发送空闲高电平
	CheckDouSupportActionResult();
	#endif
#endif
#endif

#if(COMPILE_ENABLE == TESTCODE_TANGZESHACHE)
	SendTangZeLockInterfaceCode() ;
#endif


#if(COMPILE_ENABLE == TESTCODE_BLE_OUTPUT_COMPILE)
	SendResultOfTestCode();
#endif

}

/*******************************************************************************
 * 
 ******************************************************************************/
void EventPer2ms(void)
{

	if(AT.Buzzer.timer_addcounter_per2ms < UINT8_MAX)
		AT.Buzzer.timer_addcounter_per2ms ++ ;
	#if(External_Anti_theft != Anti_theftDevice_Function)
			FreeKey.timer_48VSWActionOuttime_per2ms ++;

	#endif
			LVADSample(scifTaskData.adScan.output.pSamples[0]);//channel 0 -- LV
	#if(External_Anti_theft != Anti_theftDevice_Function)
			LV48VSWADSample(scifTaskData.adScan.output.pSamples[1]);//channel 1 -- LV_48VSW
	#endif

			FlashProtectWhenLVLow();

			BuzzerRingWhenEnable(gpioPinHandle);
#if(Commu_Mode_TwoWire == Commu_Mode)
		  //PrepareCustomSerialInterfaceCode();
#endif
}

/*******************************************************************************
 * 
 ******************************************************************************/
void EventPer100ms(void)
{

#if(Commu_Mode_Common == Commu_Mode)
		if(Port_YXT->IDR & Pin_YXT )
		{
			BLEMdlCnt.timer_noconnect2controller5V_100ms = 0 ;
			if(BLEMdlCnt.timer_connect2controller5V_100ms  < UINT8_MAX)
				BLEMdlCnt.timer_connect2controller5V_100ms  ++ ;
		}
		else
		{
			BLEMdlCnt.timer_connect2controller5V_100ms  = 0 ;
			if(BLEMdlCnt.timer_noconnect2controller5V_100ms < UINT8_MAX)
				BLEMdlCnt.timer_noconnect2controller5V_100ms ++ ;
		}
#else
		if(BLEMdlCnt.timer_connect2controller_100ms < UINT8_MAX)
			BLEMdlCnt.timer_connect2controller_100ms ++ ;
#endif

#if(External_Anti_theft != Anti_theftDevice_Function)
		if(LDPower.timer_LDturnoff_per100ms < UINT8_MAX)
			LDPower.timer_LDturnoff_per100ms ++ ;
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		if(SeatLock.timer_action_per100ms < UINT8_MAX)
			SeatLock.timer_action_per100ms ++ ;
		if(MechalMotorLock.timer_action_per100ms < UINT8_MAX)
			MechalMotorLock.timer_action_per100ms ++ ;
		#if(ElectricDouSupport == Anti_theftDevice_Function)
		if(DouSupport.timer_action_per100ms < UINT8_MAX)
			DouSupport.timer_action_per100ms ++ ;
		#endif

	#endif
#endif
}

/*******************************************************************************
 * 
 ******************************************************************************/
void PeriodicTimer_createTask(void)
{
  Task_Params taskParams;
  //init some values  
  levelTracker.lastLevel = 0;
  levelTracker.ticks_since_level_changed = 0;
  //T2C Send Code initialization
#if(Commu_Mode_TwoWire == Commu_Mode)
//  initialConfigCustomSerialInterfaceCode();
//  PrepareCustomSerialInterfaceCode();

	#warning	"unfinished T2C"
#endif
  
  //init sensor controller
  setupSC();		
  BLEMdlCnt.timer_connect2controller_100ms = UINT8_MAX ;
//TODO: use a task to pend sc sem and setup
  /* Configure the OS task */
  Task_Params_init(&taskParams);
  taskParams.stack = timerTaskStack;
  taskParams.stackSize = sizeof(timerTaskStack);
  taskParams.priority = 1;
  Task_construct(&timerTask, PeriodicTimer_taskFxn, &taskParams, NULL);
//2 ms task
  Task_Params_init(&taskParams);
  taskParams.stack = twomsTaskStack;
  taskParams.stackSize = sizeof(twomsTaskStack);
  taskParams.priority = 1;
  Task_construct(&twomsTask, twomsTaskFxn, &taskParams, NULL);
}

/*******************************************************************************
 * 
 ******************************************************************************/
void EventPer500ms(void)
{
	if(timer_500ms_per100ms >= 5 )
	{
		timer_500ms_per100ms = 0 ;

		if(Motor.timer_stall_500ms < UINT16_MAX)		//stall 计时
			Motor.timer_stall_500ms ++ ;
		
#if(Commu_Mode_Common == Commu_Mode)
		UpdateHallSpeedPer500ms();
#endif
		GetMotorStatus();		//run or stall or park
        if(Status_Motor_Park == Motor.status_motor)
		{
			if(True == IsChargeStatus())
				LV.flag_batterycharged = True ;	// 当充电停止时，LV.flag_batterycharged保存了曾经充电的记录

 			if(True == LV.flag_batterycharged)
				//重新计算Qremainder和Mileage
				CalQInitialAndMileageRemainder();
			else
			{
				//if(Motor.timer_stall_500ms != (TimePeriod_45min_Per1s * 2))
				if(Motor.timer_stall_500ms != ((TimePeriod_45min_Per1s + TimePeriod_15min_Per1s)  * 2))
					;
				else
				{
					Q.soc_release = Q.soc_initial - GetBatterySoc() ;
					#if(Commu_Mode_Common != Commu_Mode)
						//Q.data_max.bit32 = CalQmax();
						UpdataQInitalAndRemainder();
						Mileage.ratio_releasedivmileage_mul256 = GetRatioReleaseDivMileageMul256();
					#endif
#if 0
					Mileage.counter_hallchange_max.sum32 = CalMileageMax() ;
					Mileage.caledvalue_hallchange_1KM = CalCounterHallchange1KM();
#endif
					UpdateMileageInitialAndRemainder();
				}
			}
		}
        else if(Status_Motor_Run == Motor.status_motor)
		{
			LV.flag_batterycharged = False ;	//电机运行后，清除LV.flag_batterycharged
		}
		else	//Status_Motor_Stall
			 LV.flag_batterycharged = False ;	//电机运行后，清除LV.flag_batterycharged
	
		//LED flashing when BLE disconnect  or  LED on when BLE connect
		if(AT.status_BLEconnect)
		{
			PIN_setOutputValue(getGpioPinHandle(), Pin_BLE_Connect, 1);
		}
		else
		{
			if(PIN_getOutputValue(Pin_BLE_Connect))
        		PIN_setOutputValue(getGpioPinHandle(), Pin_BLE_Connect, 0);
			else
				PIN_setOutputValue(getGpioPinHandle(), Pin_BLE_Connect, 1);
		}	
		
		
#if	Test_Pin_Output_Enable
	#if Test_Pin_T2C
		PIN_setOutputValue(getGpioPinHandle(), Pin_T2C, 0);
	#endif
	#if Test_Pin_DCJD
		PIN_setOutputValue(getGpioPinHandle(), Pin_DCJD, 0);
	#endif
	#if Test_Pin_MotorD1
		PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD1, 0);
	#endif
	#if Test_Pin_MotorD2
		PIN_setOutputValue(getGpioPinHandle(), Pin_MotorD2, 1);
	#endif
	#if Test_Pin_MotorA2
		PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA2, 1);
	#endif
	#if Test_Pin_MotorA1
		PIN_setOutputValue(getGpioPinHandle(), Pin_MotorA1, 0);
	#endif
	#if Test_Pin_9VSW
		PIN_setOutputValue(getGpioPinHandle(), Pin_9VSW, 1);
	#endif
	#if Test_Pin_CPMOS
		PIN_setOutputValue(getGpioPinHandle(), Pin_CPMOS, 0	);
	#endif
	#if Test_Pin_BUZZER
		PIN_setOutputValue(getGpioPinHandle(), Pin_BUZZER, 0);
	#endif
	#if Test_Pin_GuardOutput
		PIN_setOutputValue(getGpioPinHandle(), Pin_Guard_Output, 1);
	#endif
	#if Test_Pin_SCJC
		PIN_setOutputValue(getGpioPinHandle(), Pin_SCJC, 0);
	#endif
#endif

#if Test_Pin_Input_Enable
	#if Test_Pin_YXT
		test1 = PIN_getInputValue(Pin_YXT);
	#endif
	#if Test_Pin_LD
		test2 = PIN_getInputValue(Pin_LD);
	#endif
	#if Test_Pin_GuardInput
		test3 = PIN_getInputValue(Pin_Guard_Input);
	#endif
#endif

		
	}
}


/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void EventPer1s(void)
{
	if(timer_1s_per100ms >= 10)
	{
		timer_1s_per100ms = 0 ;
		UpdateStatusConnectToController() ;
		if(LV.timer_openciruit_1s < UINT16_MAX)
			LV.timer_openciruit_1s ++ ;
		GuardDeadLineApproachPer1s();


#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		if(SeatLock.timer_unlockactionfrequent_per1s < UINT8_MAX)
			SeatLock.timer_unlockactionfrequent_per1s ++ ;
#endif

		timer_30s_p1s ++ ;
	}
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void EventPer30s(void)
{
	if(timer_30s_p1s >= 30)
	{
		timer_30s_p1s = 0;

		if(True == AT.mode_cyclicsend)
			CyclicSendData();
	}
}

/*******************************************************************************
 * 
 ******************************************************************************/
void LVInitial(void)
{
    for(uint8_t i= 0 ; i <= Counter_LV_Sample_Max ; i ++ )
        LVADSample(scifTaskData.adScan.output.pSamples[0]);
}

/*******************************************************************************
* 函数名  	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void VariableInitial(void)
{
	//Guard.status_autoguard = Status_Auto ;		//默认打开拔钥匙自动上锁功能
	Motor.status_motor = Status_Motor_Park ;

	/****初始防盗状态置位*********/
#if(ElectricDouSupport != Anti_theftDevice_Function)
	if(Status_Auto == Guard.status_autoguard)
	{
		Guard.cmd_lock_From_BLE = Lock_Ebike ;

		if(NO == IsDeadLineReachInFlash())
			Guard.status_deadline = Status_UnReachDeadline ;
		else
			Guard.status_deadline = Status_ReachedDeadline_Execute ;
		Guard.status = Status_Guarded ;
		SetGuard2Controller();	 	//上锁
	}
	else
	{
		Guard.cmd_lock_From_BLE = Unlock_Ebike ;
		if(NO == IsDeadLineReachInFlash())
		{
			Guard.status_deadline = Status_UnReachDeadline ;
			Guard.status = Status_UnGuarded ;
			//GPIO_SetBits(Port_Guard_Output,Pin_Guard_Output);		//防盗输出为高电平
			ResetGuard2Controller();		//解锁
		}
		else
		{
			Guard.status_deadline = Status_ReachedDeadline_Execute ;
			Guard.status = Status_Guarded ;
			//GPIO_ResetBits(Port_Guard_Output,Pin_Guard_Output);		//防盗输出为低电平
			SetGuard2Controller();	 	//上锁
		}
	}
#else
	if(Status_Fallen == DouSupport.status)
	{
		Guard.cmd_lock_From_BLE = Lock_Ebike ;
		if(NO == IsDeadLineReachInFlash())
			Guard.status_deadline = Status_UnReachDeadline ;
		else
			Guard.status_deadline = Status_ReachedDeadline_Execute ;
		Guard.status = Status_Guarded ;
		MechalMotorLock.status = Status_MotorLocked ;		//电机机械锁处在上锁状态
		SetGuard2Controller();	 	//上锁

	}
	else
	{
		Guard.cmd_lock_From_BLE = Unlock_Ebike ;
		if(NO == IsDeadLineReachInFlash())
		{
			Guard.status_deadline = Status_UnReachDeadline ;
			Guard.status = Status_UnGuarded ;
			MechalMotorLock.status = Status_MotorUnlocked ;		//电机机械锁处在解锁状态
			ResetGuard2Controller();		//解锁
		}
		else
		{
			Guard.status_deadline = Status_ReachedDeadline_Execute ;
			Guard.status = Status_Guarded ;
			MechalMotorLock.status = Status_MotorLocked ;		//电机机械锁处在上锁状态
			SetGuard2Controller();	 	//上锁
		}
	}
#endif


//  Guard.status_guardreason= None ;

#if(Commu_Mode_Common != Commu_Mode)
	RunMessage.flag_geaterthan_ErrorHistoryEntryNum = False ;
	//给RunMessage的计数清零
	RunMessage.counter = 0 ;
#endif

	//TODO: port this flash
//	if(FlashInitial.data_max)
//		NOP;




	//给传输状态初始值
	Guard.status_guardreason = CheckGuardReason();

#if(Commu_Mode_Common == Commu_Mode)
	BLEMdlCnt.timer_connect2controller5V_100ms = 0 ;
	BLEMdlCnt.timer_noconnect2controller5V_100ms = UINT8_MAX ;
	Mileage.counter_hallchange_accumu.bit32 = 0 ;

#else
	/****初始一线通断开连接*********/
	BLEMdlCnt.timer_connect2controller_100ms = UINT8_MAX ;
	//GPIO_ResetBits(Port_Reserved_D3,Pin_Reserved_D3);		//输出低电平

	for(uint8_t i= 0 ; i < ErrorHistoryEntryNum; i ++ )
		for(uint8_t j= 0 ; j < RunMessageSingleEntryLen ; j ++ )
			ErrorHistoryMessage.entry[i][j] = 0X00 ;


	#if(Commu_Mode_TwoWire == Commu_Mode)
		T2C.cmd = T2C_SendCommonDataCmd ;
		T2C.CommonData.startcode = T2C_CommonCmd_StartCode;		
		T2C.CommonData.strlencode = T2C_CommonCmd_ValidStrlen;

		T2C.ReadConfig.startcode = T2C_Config_StartCode ;
		T2C.ReadConfig.strlencode = T2C_ReadConfig_ValidStrlen ;

		T2C.WriteConfig.startcode = T2C_Config_StartCode ;
		T2C.WriteConfig.strlencode = T2C_WriteConfig_ValidStrlen ;
			
		if(None == Guard.status_guardreason)
			T2C.CommonData.guardstatusBit = 0 ;
		else
			T2C.CommonData.guardstatusBit = 1 ;
		
		#if(ThreeSpeedSet == COMPILE_ENABLE)
		T2C.CommonData.ThressSpeedBit = 2 ;
		#endif
		#if(ThreeTorqueSet== COMPILE_ENABLE)
		T2C.CommonData.ThressTorqueBit = 2 ;
		#endif
	#endif
#endif

	CalQInitialAndMileageRemainder();	//load Q and Mileage Initial Remainder And Max
	Mileage.caledvalue_hallchange_1KM = CalCounterHallchange1KM();

#if(Commu_Mode_Common != Commu_Mode)
	Mileage.ratio_releasedivmileage_mul256 = ReturnRatioReleaseDivMileageMul256InitialValue();
	//InitialLowPassFilteringPerFrame();
#endif

#if(External_Anti_theft != Anti_theftDevice_Function)
	FreeKey.status = Status_PowerOff ;
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		SeatLock.status = Status_Locked ;
		//MechalMotorLock.status = Status_MotorUnlocked ;

		#if(ElectricDouSupport == Anti_theftDevice_Function)
//		for(uint8_t i = 0 ; i< (DST_Strlen_Code*8 * 2 + 2) ;i++)
//			DST.timer_target[i] = (uint16_t)(2000/100) ;
		DST.counter_tojudgelevel = (uint8_t)(DST_Strlen_Code * 8 * 2 + 2 - 1) ;
		DST.counter_cycle = 0 ;

		//DouSupport.status = Status_Fallen ;		//attention!!!!!!!
		//DouSupport.status = Status_Retracted ;		//attention!!!!!!! 需要读写入Flash
		#endif
	#endif
#endif

#if GPS_COMPILE
	GPS.num_serial.sum	= 0 ;		
#endif


}

