#ifndef __CUSTOMERL504001AA_H  
#define __CUSTOMERL504001AA_H  

/**********************if compile enable************/
#define	COMPILE_ENABLE			1
#define	COMPILE_DISABLE			0
#define NVIC_COMPILE			COMPILE_ENABLE
#define USART1_COMPILE			COMPILE_ENABLE
#define DMA_TX1_COMPILE			COMPILE_ENABLE
#define	DMA_RX1_COMPILE			COMPILE_ENABLE	
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

#define	TEST								COMPILE_DISABLE		
/********************Common Configure*****************************/

//Lead_Acid_Batteries & MnNiCo_Ternary_Battery
#define	Battery_Materials_InFlash		Lead_Acid_Batteries

//48 & 60 & 72
#define LV_Grade				 72

#define Lead_Acid_LV_OPENCRI_Standard_Single_Cell_A		12	
#define Lead_Acid_Battery_Section_Num						(LV_Grade / Lead_Acid_LV_OPENCRI_Standard_Single_Cell_A)
#if(48 == LV_Grade) 
	#define MnNiCo_Battery_Section_Num						13
#elif(60 == LV_Grade)
	#define MnNiCo_Battery_Section_Num						16
#elif(72 == LV_Grade)
	#define MnNiCo_Battery_Section_Num						20		//72V用20节 60V用16节  48V用13节
#else
	#error "没有合适的"
#endif

//BLE_E & BLE_G & BLE_J
#define PCB_VERSION				RGZ_A           //last version is BLE_E

//Commu_Mode_Common & Commu_Mode_OneCommu & Commu_Mode_TwoWire
#define Commu_Mode             	Commu_Mode_OneCommu            
#define Gear_Ratio_Motor_InFlash        1				//Gear_Ratio_Motor > Gear_Ratio_Wheel
#define Gear_Ratio_Wheel_InFlash        1
#define	Number_Pole_Pairs_InFlash		28		//电机极对数

/*	电机加轮胎后轮径 ：XX英寸
*	两轮车一般都是外径16英寸
*		金彭三轮车的轮胎为3.75-12 表示内径为12英寸，测量外径为20英寸
*		百事利三轮车专用轮胎为18英寸
*/
#define	Wheel_Diameter_Inch_Int_InFlash	    		16	
#define	Wheel_Diameter_Inch_Dec_InFlash				00
#define AH_Battery_InFlash		       	 		20
#define	BatteryStandardM_InFlash				40000
#define	Rated_Q_Release_PerFrame_A_InFlash		(13) //等于额定AH*额定速度/额定公里数	两轮车额定速度一般是30km/h	

#define	Customer_Type					Manufactor_Ebike

//External_Anti_theft & FreeKeyStart & ElectricLockControl & ElectricDouSupport
#define Anti_theftDevice_Function		External_Anti_theft
#define GPS_COMPILE						COMPILE_DISABLE

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





#endif