#ifndef __CUSTOMERL508001AA_H  
#define __CUSTOMERL508001AA_H

#define	TEST								COMPILE_DISABLE		
/********************Common Configure*****************************/
//48 & 60 & 72
#define LV_Grade				 48

//BLE_E & BLE_G & BLE_J
#define PCB_VERSION				BLE_J	//last version is BLE_E

//Commu_Mode_Common & Commu_Mode_OneCommu & Commu_Mode_TwoWire
#define Commu_Mode             	Commu_Mode_TwoWire 

//External_Anti_theft & FreeKeyStart & ElectricLockControl & ElectricDouSupport
#define Anti_theftDevice_Function		ElectricDouSupport
#define GPS_COMPILE						COMPILE_DISABLE

/********************Configure InFlash*****************************/
//Lead_Acid_Batteries & MnNiCo_Ternary_Battery
#define	Battery_Materials_InFlash		MnNiCo_Ternary_Battery
#define Gear_Ratio_Motor_InFlash        1				//Gear_Ratio_Motor > Gear_Ratio_Wheel
#define Gear_Ratio_Wheel_InFlash        1
#define	Number_Pole_Pairs_InFlash		23		//���������

/*	�������̥���־� ��XXӢ��
*	���ֳ�һ�㶼���⾶16Ӣ��
*		�������ֳ�����̥Ϊ3.75-12 ��ʾ�ھ�Ϊ12Ӣ�磬�����⾶Ϊ20Ӣ��
*		���������ֳ�ר����̥Ϊ18Ӣ��
*/
#define	Wheel_Diameter_Inch_Int_InFlash	    		16	
#define	Wheel_Diameter_Inch_Dec_InFlash				00
#define AH_Battery_InFlash		       	 			20
#define	BatteryStandardM_InFlash					63000
#define	Rated_Q_Release_PerFrame_A_InFlash			(9) //���ڶAH*��ٶ�/�������	���ֳ���ٶ�һ����30km/h	

#define	Customer_Type					Manufactor_Ebike

/********************Feature  Configure*****************************/











#endif