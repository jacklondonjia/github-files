#ifndef __CUSTOMERL502102AA_H  
#define __CUSTOMERL502102AA_H

/**********************if compile enable************/
#define	COMPILE_ENABLE			1
#define	COMPILE_DISABLE			0
#define NVIC_COMPILE			COMPILE_ENABLE
#define USART1_COMPILE			COMPILE_ENABLE
#define USART2_COMPILE			COMPILE_DISABLE	
#define DMA_TX_COMPILE			COMPILE_ENABLE
#define	DMA_RX_COMPILE			COMPILE_ENABLE	
#define ADC1_COMPILE			COMPILE_ENABLE
#define YXT_COMPILE				COMPILE_ENABLE
#define GUARD_COMPILE			COMPILE_ENABLE
#define BLESET_COMPILE			COMPILE_ENABLE
#define	BUZZER_COMPILE			COMPILE_ENABLE
#define	YK_COMPILE				COMPILE_DISABLE
#define	Reserved_Pin_COMPILE	COMPILE_ENABLE

#define	USART1_TX_INT_COMPILE	        	COMPILE_DISABLE
#define	USART1_RX_INT_COMPILE	        	COMPILE_DISABLE
#define	USART1_IDLE_INT_COMPILE	        	COMPILE_ENABLE


#define TESTCODE_TIMER_Q_COMPILE			COMPILE_DISABLE
#define TESTCODE_BLE_OUTPUT_COMPILE			COMPILE_DISABLE
#define TESTCODE_BLE_CONNECTREPEAT_COMPILE	COMPILE_DISABLE
#define	TESTCODE_BLE_220V					COMPILE_DISABLE
#define	TESTCODE_TANGZESHACHE				COMPILE_DISABLE


/********************Common Configure*****************************/

//Lead_Acid_Batteries & MnNiCo_Ternary_Battery
#define	Battery_Materials		Lead_Acid_Batteries

#if(Lead_Acid_Batteries == Battery_Materials)
	#define LV_OPENCRI_Standard_Single_Cell_A		12	
	#define Battery_Section_Num						(LV_Grade / LV_OPENCRI_Standard_Single_Cell_A)
#elif(MnNiCo_Ternary_Battery == Battery_Materials)
	#define Battery_Section_Num						13		//60V 用16节  48V用13节
#else
#endif

//48 & 60 & 72
#define LV_Grade				60

//BLE_E & BLE_G & BLE_J
#define PCB_VERSION				BLE_G           //last version is BLE_E

//Commu_Mode_Common & Commu_Mode_OneCommu & Commu_Mode_TwoWire
#define Commu_Mode             	Commu_Mode_OneCommu            
#define Gear_Ratio_Motor        21				//Gear_Ratio_Motor > Gear_Ratio_Wheel
#define Gear_Ratio_Wheel        1
#define	Number_Pole_Pairs		4		//电机极对数

/*	电机加轮胎后轮径 ：XX英寸
*	两轮车一般都是外径16英寸
*		金彭三轮车的轮胎为3.75-12 表示内径为12英寸，测量外径为20英寸
*		百事利三轮车专用轮胎为18英寸
*/
#define	Wheel_Diameter_Inch	    		18		
#define AH_Battery		       	 		32
#define	BatteryStandardM				60000
#define	Rated_Q_Release_PerFrame_A		(8) //等于额定AH*额定速度/额定公里数	两轮车额定速度一般是30km/h	

#define	Customer_Type					Manufactor_Ebike

//External_Anti_theft & FreeKeyStart & ElectricLockControl & ElectricDouSupport
#define Anti_theftDevice_Function		External_Anti_theft
#define FirmwareVersion					0x010028	

#if(External_Anti_theft != Anti_theftDevice_Function)
	/***********是否需要打开电流中断 */
	#define		EXIT_COMPILE			COMPILE_ENABLE		//打开CPMOSFET的大电流中断
#else
	#define		EXIT_COMPILE			COMPILE_DISABLE
#endif
/********************Feature  Configure*****************************/
#if(Commu_Mode_Common == Commu_Mode)
	#define		Double_LVSet						COMPILE_DISABLE
	#define		OneKeyRepairSet						COMPILE_DISABLE
	#define		ThreeSpeedSet						COMPILE_DISABLE
	#define		ThreeTorqueSet						COMPILE_DISABLE
	#define		CyclicSendPeriod					CyclicSendMessagePer30s
#else
	#define		CyclicSendPeriod					CyclicSendMessagePer0_678s	

	#if(Commu_Mode_TwoWire == Commu_Mode)
		#define		Double_LVSet						COMPILE_DISABLE
		#define		OneKeyRepairSet						COMPILE_DISABLE
		#define		ThreeSpeedSet						COMPILE_DISABLE
		#define		ThreeTorqueSet						COMPILE_DISABLE
	#else 
		#define		Double_LVSet						COMPILE_DISABLE
		#define		OneKeyRepairSet						COMPILE_DISABLE
		#define		ThreeSpeedSet						COMPILE_DISABLE
		#define		ThreeTorqueSet						COMPILE_DISABLE
	#endif
#endif



#if(BLE_J == PCB_VERSION)
	#define ADC2_COMPILE				COMPILE_ENABLE
	#define	Pin_LV_48VSW				GPIO_Pin_5
	#define Port_LV_48VSW				GPIOA
	#define ADC1_Channel_LV_48VSW		ADC_Channel_5
#endif


/*********************GPIO pin define******************/
#if(BLE_E == PCB_VERSION)
	#define	Port_BLE_RST			GPIOA
	#define Pin_BLE_RST				GPIO_Pin_8
	#define	Port_BLE_Connect		GPIOB
	#define Pin_BLE_Connect			GPIO_Pin_12
	#define	Port_BUZZER				GPIOB
	#define Pin_BUZZER				GPIO_Pin_5	
	#define Pin_Guard_Input			GPIO_Pin_13
	#define Port_Guard_Input		GPIOB
	#define Pin_Guard_Output		GPIO_Pin_15
	#define Port_Guard_Output		GPIOB
	#define	Port_YXT				GPIOA
	#define	Pin_YXT					GPIO_Pin_11
	#define Port_LED0				GPIOB
	#define Pin_LED0				GPIO_Pin_14
	#define	Port_KEY1				GPIOA
	#define Pin_KEY1				GPIO_Pin_12

	#define Pin_Reserved_D1			GPIO_Pin_4
	#define Port_Reserved_D1		GPIOB
	#define Pin_Reserved_D2			GPIO_Pin_3
	#define Port_Reserved_D2		GPIOB
	#define Pin_Reserved_D3			GPIO_Pin_15
	#define Port_Reserved_D3		GPIOA
	#define Pin_Reserved_AD1		GPIO_Pin_0
	#define Port_Reserved_AD1		GPIOA
	#define Pin_Reserved_AD2		GPIO_Pin_1
	#define Port_Reserved_AD2		GPIOA
		



#elif(BLE_G == PCB_VERSION)
	#define	Port_BLE_RST		    GPIOA
	#define Pin_BLE_RST				GPIO_Pin_8
	#define	Port_BLE_Connect		GPIOB
	#define Pin_BLE_Connect			GPIO_Pin_12
	#define	Port_BUZZER				GPIOB
	#define Pin_BUZZER				GPIO_Pin_5	
	#define Pin_Guard_Input			GPIO_Pin_13
	#define Port_Guard_Input		GPIOB
	#define Pin_Guard_Output		GPIO_Pin_15
	#define Port_Guard_Output		GPIOB
	#define	Port_YXT				GPIOA
	#define	Pin_YXT					GPIO_Pin_12
	#define Port_LED0				GPIOB
	#define Pin_LED0				GPIO_Pin_14
	#define	Port_HALLU				GPIOA
	#define Pin_HALLU				GPIO_Pin_11

	#define Pin_Reserved_D1			GPIO_Pin_4
	#define Port_Reserved_D1		GPIOB
	#define Pin_Reserved_D2			GPIO_Pin_3
	#define Port_Reserved_D2		GPIOB
	#define Pin_Reserved_D3			GPIO_Pin_15
	#define Port_Reserved_D3		GPIOA
	#define Pin_Reserved_AD1		GPIO_Pin_0
	#define Port_Reserved_AD1		GPIOA
	#define Pin_Reserve_AD2			GPIO_Pin_1
	#define Port_Reserved_AD2		GPIOA

	#define	Port_CSI				Port_Reserved_D3
	#define	Pin_CSI					Pin_Reserved_D3

#elif(BLE_H == PCB_VERSION)
	#define	Port_BLE_RST		    GPIOA
	#define Pin_BLE_RST				GPIO_Pin_8
	#define	Port_BLE_Connect		GPIOB
	#define Pin_BLE_Connect			GPIO_Pin_15
	#define	Port_BUZZER				GPIOB
	#define Pin_BUZZER				GPIO_Pin_13
	#define Port_Guard_Input		GPIOB
	#define Pin_Guard_Input			GPIO_Pin_12
	#define Port_Guard_Output		GPIOB
	#define Pin_Guard_Output		GPIO_Pin_14
	#define	Port_YXT				GPIOA
	#define	Pin_YXT					GPIO_Pin_11
	#define Port_LED0				GPIOA
	#define Pin_LED0				GPIO_Pin_12
	#define	Port_HALLU				GPIOB
	#define Pin_HALLU				GPIO_Pin_10
	#define	Port_LD					GPIOB
	#define Pin_LD					GPIO_Pin_11
	#define	Port_XL					GPIOB
	#define Pin_XL					GPIO_Pin_2

	#define Port_9VSW				GPIOB
	#define Pin_9VSW				GPIO_Pin_7
	#define Port_MotorD1			GPIOB
	#define Pin_MotorD1				GPIO_Pin_5
	#define Port_MotorD2			GPIOB
	#define Pin_MotorD2				GPIO_Pin_6
	#define Port_MotorA1			GPIOB
	#define Pin_MotorA1				GPIO_Pin_9
	#define Port_MotorA2			GPIOB
	#define Pin_MotorA2				GPIO_Pin_8

	#define Port_Reserved_D1		GPIOB
	#define Pin_Reserved_D1			GPIO_Pin_4
	#define Port_Reserved_D2		GPIOB
	#define Pin_Reserved_D2			GPIO_Pin_3
	#define Port_Reserved_D3		GPIOA
	#define Pin_Reserved_D3			GPIO_Pin_15
	#define Port_Reserved_AD1		GPIOA
	#define Pin_Reserved_AD1		GPIO_Pin_0
	#define Port_Reserved_AD2		GPIOA
	#define Pin_Reserved_AD2		GPIO_Pin_1

#elif(BLE_J == PCB_VERSION)
	#define	Port_BLE_RST		    GPIOA
	#define Pin_BLE_RST				GPIO_Pin_8
	#define	Port_BLE_Connect		GPIOB
	#define Pin_BLE_Connect			GPIO_Pin_15
	#define	Port_BUZZER				GPIOB
	#define Pin_BUZZER				GPIO_Pin_13
	#define Port_SCJC				GPIOB			//电动双撑控制电路
	#define Pin_SCJC				GPIO_Pin_12
	#define Port_Guard_Input		GPIOB			//防盗输入 or 电动双撑接收电路
	#define Pin_Guard_Input			GPIO_Pin_11
	#define Port_Guard_Output		GPIOB
	#define Pin_Guard_Output		GPIO_Pin_14
	#define	Port_YXT				GPIOA
	#define	Pin_YXT					GPIO_Pin_11
	#define Port_T2C				GPIOA			//和控制器的双向通讯线
	#define Pin_T2C					GPIO_Pin_12
	#define	Port_LD					GPIOB			//轮动检测
	#define Pin_LD					GPIO_Pin_10
	#define	Port_CPMOS				GPIOB			//免钥匙驱动
	#define Pin_CPMOS				GPIO_Pin_2
	#define	Port_OCJ				GPIOB			//免钥匙驱动的过电流检测
	#define Pin_OCJ					GPIO_Pin_1

	#define Port_9VSW				GPIOB
	#define Pin_9VSW				GPIO_Pin_9
	#define Port_MotorD1			GPIOB
	#define Pin_MotorD1				GPIO_Pin_6
	#define Port_MotorD2			GPIOB
	#define Pin_MotorD2				GPIO_Pin_5
	#define Port_MotorA1			GPIOB
	#define Pin_MotorA1				GPIO_Pin_8
	#define Port_MotorA2			GPIOB
	#define Pin_MotorA2				GPIO_Pin_7
	#define Port_DCJD				GPIOB
	#define Pin_DCJD				GPIO_Pin_4

	#define Port_Reserved_D2		GPIOB
	#define Pin_Reserved_D2			GPIO_Pin_3
	#define Port_Reserved_D3		GPIOA
	#define Pin_Reserved_D3			GPIO_Pin_15
	#define Port_Reserved_AD1		GPIOA
	#define Pin_Reserved_AD1		GPIO_Pin_0
	#define Port_Reserved_AD2		GPIOA
	#define Pin_Reserved_AD2		GPIO_Pin_1

	#define	Port_CSI				Port_T2C
	#define	Pin_CSI					Pin_T2C

#else
	#error "没见过这种PCB版本！"
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