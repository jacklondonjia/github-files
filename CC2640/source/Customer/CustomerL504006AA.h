#ifndef __CUSTOMERL504006AA_H
#define __CUSTOMERL504006AA_H

#define	TEST								COMPILE_DISABLE	



/********************Common Configure*****************************/
//48 & 60 & 72
#define LV_Grade				 60

//BLE_E & BLE_G & BLE_J
#define PCB_VERSION				BLE_G           //last version is BLE_E

//Commu_Mode_Common & Commu_Mode_OneCommu & Commu_Mode_TwoWire
#define Commu_Mode             	Commu_Mode_OneCommu 

//External_Anti_theft & FreeKeyStart & ElectricLockControl & ElectricDouSupport
#define Anti_theftDevice_Function		External_Anti_theft
#define GPS_COMPILE						COMPILE_DISABLE

/********************Configure InFlash*****************************/
//Lead_Acid_Batteries & MnNiCo_Ternary_Battery
#define	Battery_Materials_InFlash		Lead_Acid_Batteries
#define Gear_Ratio_Motor_InFlash        1				//Gear_Ratio_Motor > Gear_Ratio_Wheel
#define Gear_Ratio_Wheel_InFlash        1
#define	Number_Pole_Pairs_InFlash		28		//���������

/*	�������̥���־� ��XXӢ��
*	���ֳ�һ�㶼���⾶16Ӣ��
*		�������ֳ�����̥Ϊ3.75-12 ��ʾ�ھ�Ϊ12Ӣ�磬�����⾶Ϊ20Ӣ��
*		���������ֳ�ר����̥Ϊ18Ӣ��
*/
#define	Wheel_Diameter_Inch_Int_InFlash	    		16	
#define	Wheel_Diameter_Inch_Dec_InFlash				00
#define AH_Battery_InFlash		       	 			20
#define	BatteryStandardM_InFlash					50000
#define	Rated_Q_Release_PerFrame_A_InFlash			(13) //���ڶAH*��ٶ�/�������	���ֳ���ٶ�һ����30km/h	

#define	Customer_Type					Manufactor_Ebike




#endif
