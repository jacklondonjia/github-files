#ifndef __ANNOTATE_H
#define __ANNOTATE_H

#if(Commu_Mode > Commu_Mode_TwoWire)
    #error "û����������ͨѶ��ʽ��"
#endif


#if(Commu_Mode_TwoWire == Commu_Mode)
  	#if(PCB_VERSION < BLE_G)
      	#error "�̼��汾��PCB�汾����Ӧ��������ҪBLE_G"
  	#endif
	#if(COMPILE_ENABLE == OneKeyRepairSet)
		 #warning "��һ���޸�����"
  	#endif
	
	#if(COMPILE_ENABLE == Double_LVSet)
		 #warning "��˫Ƿѹ����"
  	#endif
	#if(COMPILE_ENABLE== ThreeSpeedSet)
		 #warning "�����ٹ���"
  	#endif
	#if(COMPILE_ENABLE== ThreeTorqueSet)
		 #warning "��������������"
	#endif
	#if(COMPILE_ENABLE == Double_LV)
		#error "��Ҫ�޸ĳ�Double_LVSet"
	#endif				
	#if(COMPILE_ENABLE== OneKeyRepair)
		#error "��Ҫ�޸ĳ�OneKeyRepairSet"
	#endif
	#if(COMPILE_ENABLE== ThreeSpeed)
		#error "��Ҫ�޸ĳ�ThreeSpeedSet"
	#endif
	#if(COMPILE_ENABLE== ThreeTorque)
		#error "��Ҫ�޸ĳ�ThreeTorqueSet"
	#endif
#endif

	

#if(ElectricDouSupport == Anti_theftDevice_Function)
	#if(0 == DST_CycleTimes)
		#error"DST_CycleTimes�������0"
	#endif

	#if(Commu_Mode != Commu_Mode_TwoWire)				
		#error"ʹ��˫�ű���ʹ��˫��ͨѶ�汾����ֹGoutʧЧ�����¹�"
	//���ʹ��˫��ͨѶ ���T2C�߶Ͽ� �������ͻ�������״̬
	#endif
	
#endif

#if(PCB_VERSION < RGZ_A)
	#if(COMPILE_ENABLE == EXIT_COMPILE)
		#error"�̼��汾��PCB�汾����Ӧ��BLE_J֮ǰ��ʹ���ⲿ�ж�"
	#endif
#else
	#if(COMPILE_ENABLE == EXIT_COMPILE)
		#warning"ʹ�����ⲿ�жϣ����PMOSFET�������� "
	#endif
#endif
	

#if(External_Anti_theft != Anti_theftDevice_Function)
	#if(PCB_VERSION < BLE_H)
		#error "�̼��汾��PCB�汾����Ӧ����Կ��������ҪBLE_H"
	#endif
#endif

#if(COMPILE_ENABLE == Double_LVSet)
	#if(Commu_Mode_TwoWire != Commu_Mode)
		#error "ֻ��˫����ư汾�ܹ�ʹ���޵���������"
	#endif
#endif

#if(COMPILE_ENABLE == Double_LVSet)
	#if(Lead_Acid_Batteries != Battery_Materials)
		#error "��û��д��Ǧ���ص�˫Ƿѹ�汾"
	#endif
#endif

#if(48 == LV_Grade)	
#elif(60 == LV_Grade)
	#warning "60V���;R40��R42��R43��R44 Ϊ1k;���R194Ϊ180(ohm)/1W;R7Ϊ2402 1% ;R9Ϊ750R 1%"
#elif(72 == LV_Grade)
	#warning "72V���;R40��R42��R43��R44 Ϊ1.5k;���R194Ϊ220(ohm)/1W;R7Ϊ6202 1% ;R9Ϊ1601 1%"
#else
	#error "û���������ĵ�ص�ѹ��"
#endif

#if(20 == AH_Battery_InFlash)	
#elif(12 == AH_Battery_InFlash)
	#warning "���Ϊ12AH"
#elif(30 == AH_Battery_InFlash)
	#warning "���Ϊ30AH"
#elif(40 == AH_Battery_InFlash)
	#warning "���Ϊ40AH"	
#else
	#warning "û���������ĵ��������"
#endif

#if(1 == Gear_Ratio_Motor_InFlash)
	#if(23 == Number_Pole_Pairs_InFlash)
		#warning "���������Ϊ23 Ϊ��Ħ���"
	#elif(28 == Number_Pole_Pairs_InFlash )
		#warning "���������Ϊ28 Ϊ�綯���г����"
	#elif(24 == Number_Pole_Pairs_InFlash )
		#warning "���������Ϊ24 Ϊ������������"
	#else
		#error "û�������������ֵ��������!"
	#endif	

	#if(16 == Wheel_Diameter_Inch_Int_InFlash)
	#elif(14 == Wheel_Diameter_Inch_Int_InFlash)
		#warning "�����־�Ϊ14Ӣ�� ��Ĭ�ϣ�"
	#else
		#error "û�����������������־���"
	#endif
#else	
	#if(4 == Number_Pole_Pairs_InFlash )
		#warning "���������Ϊ4 Ϊ���ֳ����"
	#elif(5 == Number_Pole_Pairs_InFlash)
		#warning "���������Ϊ5 Ϊ���ֳ���� ��Ĭ��"	
	#else
		#error "û�������������ֵ����������" 
	#endif	

	#if(20 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		;
	#elif(18 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		#warning "���ֳ��⾶18Ӣ����̥ ��Ĭ��"	
	#elif(17 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		#warning "���ֳ��⾶17Ӣ����̥ ��Ĭ��"	
	#elif(19 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		#warning "���ֳ��⾶19Ӣ����̥ ��Ĭ��"	
	#else
		#error "û�����������������־�"
	#endif

	#if(10 == Gear_Ratio_Motor_InFlash)
		;
	#elif(21 == Gear_Ratio_Motor_InFlash)
		#warning "���������������ֳ� ���ֱ�Ϊ21:1"	
	#else
		#error "��û�����������ĳ��ֱȵ����ֳ�"
	#endif	
#endif



#if(Lead_Acid_Batteries == Battery_Materials_InFlash)
//	#if((LV_Grade * 100 ) == AverageVoltage_CalByEnergy_A)
//	#else
//		#error "û���������ĵ�ؾ�ֵ��ѹ��"
//	#endif

	#if(50000 == BatteryStandardM_InFlash)
	#elif(30000 == BatteryStandardM_InFlash)
		#warning "����Ǧ���ص������Ϊ30km"
	#elif(40000 == BatteryStandardM_InFlash)
		#warning "����Ǧ���ص������Ϊ40km"
	#elif(60000 == BatteryStandardM_InFlash)
		#warning "����Ǧ���ص������Ϊ60km"	
	#else
		#warning "û���������ĵ�����������"
	#endif
#elif(MnNiCo_Ternary_Battery == Battery_Materials_InFlash)
	#if(50000 == BatteryStandardM_InFlash)
		#warning "�õ������̽�������������������﮵��"
	#elif(63000 == BatteryStandardM_InFlash)
		#warning "�õ������̽��������ڰ��⳵﮵��"	
	#else
		#warning "û���������ĵ�����������"
	#endif
	
	#if(48 == LV_Grade)	
		#if(13 != MnNiCo_Battery_Section_Num)
			#error "48V��Ԫ﮵�ʹ�õ���13�ڵ�� ���ڵ��Ϊ3.75V"
//		#else
//			#if(45 == MnNi_LV_OPENCRI_MIN_A_IntePart && 19 ==MnNi_LV_OPENCRI_MIN_A_DeciPart)
//				#warning "ÿ��﮵�ص�Ƿѹֵ�ǰ��� ���⳵ �ķ�ʽ���õ�"
//			#else
//				#error	"��ȷ�ϳ��Ҽ�﮵�ص�Ƿѹֵ"
//			#endif
		#endif
	#elif(60 == LV_Grade)
		#if( 16 != MnNiCo_Battery_Section_Num)
			#error "60V��Ԫ﮵�ʹ�õ���16�ڵ�� ���ڵ��Ϊ3.75V"
//		#else
//			#if(55 == MnNi_LV_OPENCRI_MIN_A_IntePart && 61 == MnNi_LV_OPENCRI_MIN_A_DeciPart)
//				#warning "ÿ��﮵�ص�Ƿѹֵ�ǰ��� ������ �ķ�ʽ���õ� 55.614V"
//			#else
//				#error	"��ȷ�ϳ��Ҽ�﮵�ص�Ƿѹֵ"
//			#endif
		#endif
	#elif(72 == LV_Grade)
		#if( 20 != MnNiCo_Battery_Section_Num)
			#error "72V��Ԫ﮵�ʹ�õ���20�ڵ�� ���ڵ��Ϊ3.75V"
//		#else
//			#if(55 == MnNi_LV_OPENCRI_MIN_A_IntePart && 61 == MnNi_LV_OPENCRI_MIN_A_DeciPart)
//				#warning "ÿ��﮵�ص�Ƿѹֵ�ǰ��� ������ �ķ�ʽ���õ� 55.614V"
//			#else
//				#error	"��ȷ�ϳ��Ҽ�﮵�ص�Ƿѹֵ"
//			#endif
		#endif
		
	#else
		#error "Battery_Section_Num��ûȷ�� ���ڵ��Ϊ3.75V"
	#endif
#else
#endif

#if(Manufactor_Ebike == Customer_Type)
#elif(Rent_Ebike == Customer_Type)	
	#warning "Ϊ�����̻��汾 Ĭ������Ϊ0��Ĭ������"
#else
#endif


#if(ErrorHistoryEntryNum > ErrorHistoryEntryNum_Max)
	#error "������ʷ���ϵ���ĿӦ�ò�Ӧ�ô�������趨ֵ"
#endif

#if(RunMessageEntryNum > ErrorHistoryEntryNum)
	#error "����״̬�������Ŀ����Ӧ�ô��ڱ��������ʷ����Ŀ��"
#endif


#if TESTCODE_TIMER_Q_COMPILE
	#warning "�ð汾Ϊʱ�����̵Ĳ��԰汾��"
#endif

#if TESTCODE_BLE_CONNECTREPEAT_COMPILE
	#warning "�ð汾Ϊ����BLE���Ӵ����Ĳ��԰汾��"
#endif
	
#if(TESTCODE_BLE_OUTPUT_COMPILE)
	#warning "�ð汾Ϊ���Դ���汾��"
#endif		

#if(TESTCODE_BLE_220V)
	#warning "�ð汾Ϊ220V��Դ���Դ���汾��"
#endif	



#endif
