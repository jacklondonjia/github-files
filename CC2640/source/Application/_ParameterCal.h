#ifndef __PARAMETERCAL_H
#define __PARAMETERCAL_H


/********************ConfigControllerFlash Config*****************************/
#if(Commu_Mode_TwoWire == Commu_Mode)
	#if(L000001AF == CumstomerNum)
		#define	ConfigControllerFlash_COMPILE	COMPILE_ENABLE
	#else
		#define	ConfigControllerFlash_COMPILE	COMPILE_DISABLE
	#endif
#else
	#define	ConfigControllerFlash_COMPILE	COMPILE_DISABLE
#endif

/********************CyclicSendPeriod Config*****************************/
#if(Commu_Mode_Common == Commu_Mode)
	#define		CyclicSendPeriod					CyclicSendMessagePer30s
#else
	#define		CyclicSendPeriod					CyclicSendMessagePer0_678s
#endif


/********************Anti_theftDevice_Function Config*****************************/
#if(External_Anti_theft != Anti_theftDevice_Function)
	/***********是否需要打开电流中断 */
	#define		EXIT_COMPILE			COMPILE_DISABLE		//打开CPMOSFET的大电流中断
#else
	#define		EXIT_COMPILE			COMPILE_DISABLE
#endif

/********************Commu_Mode_TwoWireSet Auto Config*****************************/
#if(Commu_Mode_Common == Commu_Mode)
	#define		Double_LVSet						COMPILE_DISABLE
	#define		OneKeyRepairSet						COMPILE_DISABLE
	#define		ThreeSpeedSet						COMPILE_DISABLE
	#define		ThreeTorqueSet						COMPILE_DISABLE
#else
	#if(Commu_Mode_TwoWire == Commu_Mode)
		#define		Double_LVSet						COMPILE_DISABLE
		#define		OneKeyRepairSet						COMPILE_DISABLE
		#if((L000001AE == CumstomerNum)||(L000002AB == CumstomerNum)||(L601001AA == CumstomerNum))
			#define		ThreeSpeedSet						COMPILE_ENABLE
			#define		ThreeTorqueSet						COMPILE_ENABLE
		#else
			#define		ThreeSpeedSet						COMPILE_DISABLE
			#define		ThreeTorqueSet						COMPILE_DISABLE
		#endif
	#else 
		#define		Double_LVSet						COMPILE_DISABLE
		#define		OneKeyRepairSet						COMPILE_DISABLE
		#define		ThreeSpeedSet						COMPILE_DISABLE
		#define		ThreeTorqueSet						COMPILE_DISABLE
	#endif
#endif


#define		Deadline_Day_InFlash				(uint16_t)0XFFFF
#define		Deadline_Hour_InFlash				(uint8_t)(23)
#define		Deadline_Minu_InFlash				(uint8_t)(59)

/********************Battery Num Auto Cal*****************************/
#define Lead_Acid_LV_OPENCRI_Standard_Single_Cell_A			12	
#define Lead_Acid_Battery_Section_Num						(uint8_t)(LV_Grade / Lead_Acid_LV_OPENCRI_Standard_Single_Cell_A)
#if(48 == LV_Grade) 
	#define MnNiCo_Battery_Con_Section_Num						13
	#define MnNiCo_Battery_UnCon_Section_Num					14
#elif(60 == LV_Grade)
	#define MnNiCo_Battery_Con_Section_Num						16
	#define MnNiCo_Battery_UnCon_Section_Num					17
#elif(72 == LV_Grade)
	#define MnNiCo_Battery_Con_Section_Num						20
	#define MnNiCo_Battery_UnCon_Section_Num					20		//72V用20节 60V用16节  48V用13节
#else
	#error "没有合适的"
#endif

/********************GPS Auto Config*****************************/
#if GPS_COMPILE
	#define	HostType							HostTypeA
	#define USART2_COMPILE						COMPILE_ENABLE	
	#define DMA_TX2_COMPILE						COMPILE_ENABLE
	#define	DMA_RX2_COMPILE						COMPILE_ENABLE
	#define	USART2_TX_INT_COMPILE	        	COMPILE_DISABLE
	#define	USART2_RX_INT_COMPILE	        	COMPILE_DISABLE
	#define	USART2_IDLE_INT_COMPILE	        	COMPILE_ENABLE
	
#else
	#define	HostType							HostTypeA
	#define USART2_COMPILE						COMPILE_DISABLE	
	#define DMA_TX2_COMPILE						COMPILE_DISABLE
	#define	DMA_RX2_COMPILE						COMPILE_DISABLE
	#define	USART2_TX_INT_COMPILE	        	COMPILE_DISABLE
	#define	USART2_RX_INT_COMPILE	        	COMPILE_DISABLE
	#define	USART2_IDLE_INT_COMPILE	        	COMPILE_DISABLE
#endif

#if(BLE_J == PCB_VERSION)
	#define ADC2_COMPILE				COMPILE_ENABLE
	#define	Pin_LV_48VSW				GPIO_Pin_5
	#define Port_LV_48VSW				GPIOA
	#define ADC1_Channel_LV_48VSW		ADC_Channel_5
#endif

/**************timeperiod define**************/
#define TimePeriod_10ms_Per100us		100
#define TimePeriod_2ms_Per100us			20
#define TimePeriod_200ms_Per100us		2000
#define TimePeriod_50ms_Per100us		500
#define TimePeriod_500ms_Per2ms			250
#define TimePeriod_2s_Per2ms			1000
#define TimePeriod_1h_Per1s   			(60 * 60)

#if TESTCODE_TIMER_Q_COMPILE
    #define TimePeriod_1min_Per1s			60
    #define TimePeriod_10min_Per1s 		    (TimePeriod_1min_Per1s*	1)
	#define TimePeriod_15min_Per1s 		    (TimePeriod_1min_Per1s*	2)
    #define TimePeriod_45min_Per1s 		    (TimePeriod_1min_Per1s*	10)
	#define	Time_Stall_Max					(1 * TimePeriod_1min_Per1s * 2 )	
#else
    #define TimePeriod_1min_Per1s			60
    #define TimePeriod_10min_Per1s 		    (TimePeriod_1min_Per1s * 10)
	#define TimePeriod_15min_Per1s 		    (TimePeriod_1min_Per1s * 15)
    #define TimePeriod_45min_Per1s 		    (TimePeriod_1min_Per1s * 45)   
	#define	Time_Stall_Max					(TEN * TimePeriod_1min_Per1s * 2 )	//10 minutes
#endif























#endif
