#ifndef __ANNOTATE_H
#define __ANNOTATE_H

#if(Commu_Mode > Commu_Mode_TwoWire)
    #error "没见过这样的通讯方式！"
#endif


#if(Commu_Mode_TwoWire == Commu_Mode)
  	#if(PCB_VERSION < BLE_G)
      	#error "固件版本和PCB版本不对应！至少需要BLE_G"
  	#endif
	#if(COMPILE_ENABLE == OneKeyRepairSet)
		 #warning "有一键修复功能"
  	#endif
	
	#if(COMPILE_ENABLE == Double_LVSet)
		 #warning "有双欠压功能"
  	#endif
	#if(COMPILE_ENABLE== ThreeSpeedSet)
		 #warning "有三速功能"
  	#endif
	#if(COMPILE_ENABLE== ThreeTorqueSet)
		 #warning "有力矩三挡功能"
	#endif
	#if(COMPILE_ENABLE == Double_LV)
		#error "需要修改成Double_LVSet"
	#endif				
	#if(COMPILE_ENABLE== OneKeyRepair)
		#error "需要修改成OneKeyRepairSet"
	#endif
	#if(COMPILE_ENABLE== ThreeSpeed)
		#error "需要修改成ThreeSpeedSet"
	#endif
	#if(COMPILE_ENABLE== ThreeTorque)
		#error "需要修改成ThreeTorqueSet"
	#endif
#endif

	

#if(ElectricDouSupport == Anti_theftDevice_Function)
	#if(0 == DST_CycleTimes)
		#error"DST_CycleTimes必须大于0"
	#endif

	#if(Commu_Mode != Commu_Mode_TwoWire)				
		#error"使用双撑必须使用双向通讯版本，防止Gout失效导致事故"
	//如果使用双向通讯 如果T2C线断开 控制器就会进入防盗状态
	#endif
	
#endif

#if(PCB_VERSION < RGZ_A)
	#if(COMPILE_ENABLE == EXIT_COMPILE)
		#error"固件版本和PCB版本不对应！BLE_J之前不使用外部中断"
	#endif
#else
	#if(COMPILE_ENABLE == EXIT_COMPILE)
		#warning"使用了外部中断，检测PMOSFET输出大电流 "
	#endif
#endif
	

#if(External_Anti_theft != Anti_theftDevice_Function)
	#if(PCB_VERSION < BLE_H)
		#error "固件版本和PCB版本不对应！免钥匙至少需要BLE_H"
	#endif
#endif

#if(COMPILE_ENABLE == Double_LVSet)
	#if(Commu_Mode_TwoWire != Commu_Mode)
		#error "只有双向控制版本能够使用无电续航功能"
	#endif
#endif

#if(COMPILE_ENABLE == Double_LVSet)
	#if(Lead_Acid_Batteries != Battery_Materials)
		#error "还没有写非铅酸电池的双欠压版本"
	#endif
#endif

#if(48 == LV_Grade)	
#elif(60 == LV_Grade)
	#warning "60V电池;R40、R42、R43、R44 为1k;插件R194为180(ohm)/1W;R7为2402 1% ;R9为750R 1%"
#elif(72 == LV_Grade)
	#warning "72V电池;R40、R42、R43、R44 为1.5k;插件R194为220(ohm)/1W;R7为6202 1% ;R9为1601 1%"
#else
	#error "没见过这样的电池电压！"
#endif

#if(20 == AH_Battery_InFlash)	
#elif(12 == AH_Battery_InFlash)
	#warning "电池为12AH"
#elif(30 == AH_Battery_InFlash)
	#warning "电池为30AH"
#elif(40 == AH_Battery_InFlash)
	#warning "电池为40AH"	
#else
	#warning "没见过这样的电池容量！"
#endif

#if(1 == Gear_Ratio_Motor_InFlash)
	#if(23 == Number_Pole_Pairs_InFlash)
		#warning "电机极对数为23 为电摩电机"
	#elif(28 == Number_Pole_Pairs_InFlash )
		#warning "电机极对数为28 为电动自行车电机"
	#elif(24 == Number_Pole_Pairs_InFlash )
		#warning "电机极对数为24 为威科力特殊电机"
	#else
		#error "没见过这样的两轮电机极对数!"
	#endif	

	#if(16 == Wheel_Diameter_Inch_Int_InFlash)
	#elif(14 == Wheel_Diameter_Inch_Int_InFlash)
		#warning "两轮轮径为14英寸 非默认！"
	#else
		#error "没见过这样的两轮外轮径！"
	#endif
#else	
	#if(4 == Number_Pole_Pairs_InFlash )
		#warning "电机极对数为4 为三轮车电机"
	#elif(5 == Number_Pole_Pairs_InFlash)
		#warning "电机极对数为5 为三轮车电机 非默认"	
	#else
		#error "没见过这样的三轮电机极对数！" 
	#endif	

	#if(20 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		;
	#elif(18 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		#warning "三轮车外径18英寸轮胎 非默认"	
	#elif(17 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		#warning "三轮车外径17英寸轮胎 非默认"	
	#elif(19 == Wheel_Diameter_Inch_Int_InFlash && 1 != Gear_Ratio_Motor_InFlash)	
		#warning "三轮车外径19英寸轮胎 非默认"	
	#else
		#error "没见过这样的三轮外轮径"
	#endif

	#if(10 == Gear_Ratio_Motor_InFlash)
		;
	#elif(21 == Gear_Ratio_Motor_InFlash)
		#warning "金彭邮政特种三轮车 齿轮比为21:1"	
	#else
		#error "还没有遇到这样的齿轮比的三轮车"
	#endif	
#endif



#if(Lead_Acid_Batteries == Battery_Materials_InFlash)
//	#if((LV_Grade * 100 ) == AverageVoltage_CalByEnergy_A)
//	#else
//		#error "没见过这样的电池均值电压！"
//	#endif

	#if(50000 == BatteryStandardM_InFlash)
	#elif(30000 == BatteryStandardM_InFlash)
		#warning "设置铅酸电池的总里程为30km"
	#elif(40000 == BatteryStandardM_InFlash)
		#warning "设置铅酸电池的总里程为40km"
	#elif(60000 == BatteryStandardM_InFlash)
		#warning "设置铅酸电池的总里程为60km"	
	#else
		#warning "没见过这样的电池总里程数！"
	#endif
#elif(MnNiCo_Ternary_Battery == Battery_Materials_InFlash)
	#if(50000 == BatteryStandardM_InFlash)
		#warning "该电池总里程仅仅适用于威科力测试锂电池"
	#elif(63000 == BatteryStandardM_InFlash)
		#warning "该电池总里程仅仅适用于爱租车锂电池"	
	#else
		#warning "没见过这样的电池总里程数！"
	#endif
	
	#if(48 == LV_Grade)	
		#if(13 != MnNiCo_Battery_Section_Num)
			#error "48V三元锂电使用的是13节电池 单节电池为3.75V"
//		#else
//			#if(45 == MnNi_LV_OPENCRI_MIN_A_IntePart && 19 ==MnNi_LV_OPENCRI_MIN_A_DeciPart)
//				#warning "每节锂电池的欠压值是按照 爱租车 的方式配置的"
//			#else
//				#error	"请确认厂家及锂电池的欠压值"
//			#endif
		#endif
	#elif(60 == LV_Grade)
		#if( 16 != MnNiCo_Battery_Section_Num)
			#error "60V三元锂电使用的是16节电池 单节电池为3.75V"
//		#else
//			#if(55 == MnNi_LV_OPENCRI_MIN_A_IntePart && 61 == MnNi_LV_OPENCRI_MIN_A_DeciPart)
//				#warning "每节锂电池的欠压值是按照 威科力 的方式配置的 55.614V"
//			#else
//				#error	"请确认厂家及锂电池的欠压值"
//			#endif
		#endif
	#elif(72 == LV_Grade)
		#if( 20 != MnNiCo_Battery_Section_Num)
			#error "72V三元锂电使用的是20节电池 单节电池为3.75V"
//		#else
//			#if(55 == MnNi_LV_OPENCRI_MIN_A_IntePart && 61 == MnNi_LV_OPENCRI_MIN_A_DeciPart)
//				#warning "每节锂电池的欠压值是按照 威科力 的方式配置的 55.614V"
//			#else
//				#error	"请确认厂家及锂电池的欠压值"
//			#endif
		#endif
		
	#else
		#error "Battery_Section_Num还没确认 单节电池为3.75V"
	#endif
#else
#endif

#if(Manufactor_Ebike == Customer_Type)
#elif(Rent_Ebike == Customer_Type)	
	#warning "为租赁商户版本 默认租期为0，默认上锁"
#else
#endif


#if(ErrorHistoryEntryNum > ErrorHistoryEntryNum_Max)
	#error "保存历史故障的数目应该不应该大于最大设定值"
#endif

#if(RunMessageEntryNum > ErrorHistoryEntryNum)
	#error "运行状态保存的条目数不应该大于保存故障历史的条目数"
#endif


#if TESTCODE_TIMER_Q_COMPILE
	#warning "该版本为时间缩短的测试版本！"
#endif

#if TESTCODE_BLE_CONNECTREPEAT_COMPILE
	#warning "该版本为测试BLE连接次数的测试版本！"
#endif
	
#if(TESTCODE_BLE_OUTPUT_COMPILE)
	#warning "该版本为测试代码版本！"
#endif		

#if(TESTCODE_BLE_220V)
	#warning "该版本为220V电源测试代码版本！"
#endif	



#endif
