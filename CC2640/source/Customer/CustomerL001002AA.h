#ifndef __CUSTOMERL001002AA_H
#define __CUSTOMERL001002AA_H

#define	TEST								COMPILE_DISABLE	



/********************Common Configure*****************************/
//48 & 60 & 72
#define LV_Grade				 48

//BLE_E & BLE_G & BLE_J
#define PCB_VERSION				DLCC01           //last version is BLE_E

//Commu_Mode_Common & Commu_Mode_OneCommu & Commu_Mode_TwoWire
#define Commu_Mode             	Commu_Mode_TwoWire 

//External_Anti_theft & FreeKeyStart & ElectricLockControl & ElectricDouSupport
#define Anti_theftDevice_Function		ElectricDouSupport
#define GPS_COMPILE						COMPILE_DISABLE

/********************Configure InFlash*****************************/
//Lead_Acid_Batteries & MnNiCo_Ternary_Battery_CHILWEE & MnNiCo_Ternary_Battery_PHYLION
#define	Battery_Materials_InFlash		Lead_Acid_Batteries

#if(MnNiCo_Ternary_Battery_CHILWEE == Battery_Materials_InFlash)
	#define			IsNormalMnNiCoBatteryNum		1			//if MnNiCo_Battery_Num is 13/16/20 ,set 1,else set 0(14/17)
#endif

#define	MnNiCo_Battery_Section_Num		13
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
#define AH_Battery_InFlash		       	 			20
#define	BatteryStandardM_InFlash					50000
#define	Rated_Q_Release_PerFrame_A_InFlash			(13) //等于额定AH*额定速度/额定公里数	两轮车额定速度一般是30km/h	

#define	Customer_Type					Manufactor_Ebike




#endif
