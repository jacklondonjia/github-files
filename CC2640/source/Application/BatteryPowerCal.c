//#include "stdio.h"
//#include "Defines.h"
//#include "_Parameter.h"
//#include "globalvariables.h"
//#include "ATcontrol.h"
//#include "BatteryPowerCal.h"
//#include "Guard.h"
//#include "Onelinecommunica.h"
//#include "sendandstring.h"
//#include "MileageCal.h"
//#include "Tool_Functions.h"
#include "CustomerPara.h"

/*************************************************/
/*******************Variable**********************/
LV_TypeDef 		LV ;
Q_TypeDef		Q ;
Current_TypeDef	Current ;
Motor_TypeDef	Motor ;

/****************************************************/

/**********************function**********************/
/*******************************************************************************
* 函数名  	 	: 需要考虑其他状态进入连接状态的处理
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
/*void GetQmaxAndMmaxCalElement(void)                //使用状态机
{
	if(Status_UnConnected_AfterOpCal == LV.status_opencircuit)
	{
		if(True == BLEModule.status_connect2controller)          // && RESET == IfPWMOutPut())
		{
			CalQInitialAndMileageRemainder();	
			LV.status_opencircuit = Status_Connected ;
		}
		else
			;	
	}
	else if(Status_Connected == LV.status_opencircuit )
	{ 
		if(False == BLEModule.status_connect2controller)	
		{
			StartOpenCircuitVoltageTimer();
			LV.status_opencircuit  = Status_UnConnected_BeforeFirstAD ;
		}
		else
			;	
	} 
	else if(Status_UnConnected_BeforeFirstAD == LV.status_opencircuit)
	{
		if(True == BLEModule.status_connect2controller)          // && RESET == IfPWMOutPut())
		{		
			LV.status_opencircuit = Status_Connected ;
		}
		else
		{
			if(LV.timer_openciruit_1s >= TimePeriod_45min_Per1s)
			{
				LV.data_45min_afteropen = LV.avr_adc;
				LV.status_opencircuit  = Status_UnConnected_BeforeSecondAD ;		
			} 
			else
				;
		}
	}
	else
	{ 
		if(True == BLEModule.status_connect2controller)          // && RESET == IfPWMOutPut())
		{
			//QInitalAndQuitCalQmax();
#if(Commu_Mode_Common == Commu_Mode)	
			Q.soc_initial = GetBatterySoc(); 
#else
			UpdataQRemainder();
#endif
			UpdataMileageRemainder();
			
			LV.status_opencircuit = Status_Connected ;
		}
		else
		{
			if(LV.timer_openciruit_1s >= TimePeriod_1h_Per1s)
			{
				LV.data_1h_afteropen = LV.avr_adc;
				if(NO == IsOpenVoltageInChargingStatus())
				{
					Q.soc_release = Q.soc_initial - Q.soc_1h_afteropen ;
#if(Commu_Mode_Common != Commu_Mode)
					Q.data_max.bit32 = CalQmax();
					UpdataQRemainder();
#endif
					Mileage.counter_hallchange_max.bit32 = CalMileageMax() ;		  
					UpdataMileageRemainder();		  		  
				}
				else
					LV.flag_batterycharged = SetFlagBatteryCharged();			//          AbandonQmax();	  
				LV.status_opencircuit = Status_UnConnected_AfterOpCal ;
			}
		}
	}   
}

*/

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : value: 12-bits adc value
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void LVADSample(uint16_t value)
{ 
	LV.sum_temp_adc += value ;
	LV.counter_sample ++ ;
	
	if( LV.counter_sample >= Counter_LV_Sample_Max) 
	{  
		LV.avr_adc =  adcValueAdjustValueForGainAndOffset(LV.sum_temp_adc >> Counter_LV_Sample_Shift);
		LV.sum_temp_adc = 0 ;
		LV.counter_sample = 0 ;
	}  
}
/*******************************************************************************
* 函数名  		: 根据电压值计算电量的百分比soc值
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
uint16_t GetBatterySoc(void)
{
	return CalSocMul16(LV.avr_adc); 
}
/*******************************************************************************
* 函数名  		: 根据电压值计算电量的百分比soc值
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
uint16_t CalSocMul16(uint16_t  LV_adc)
{
	uint32_t  data_soc ;
	/*	data_soc = (data_LV-46)*100/8.8 ;
	soc = (34.3/1.3 * 3.3/4096 *Value - 46 )*(100/8.8)
	soc= 0.241558Value-522.7272 */
	if(Lead_Acid_Batteries == DevicePara.battery_materialsAndmanufact)	
	{	
		if(LV_adc < Lead_Acid_LV_OPENCRI_MIN_D)	
		{
			
			data_soc = SOC_MIN ;
			data_soc = data_soc * 16 ;
		}
		else if (LV_adc > Lead_Acid_LV_OPENCRI_MAX_D)
		{
			data_soc = SOC_MAX ;
			data_soc = data_soc * 16 ;
		}	
		else
		{		//data_soc = LV_adc * SOC_FACTOR1 / SOC_FACTOR2 - SOC_FACTOR3 ;
			data_soc = LV_adc ;
			data_soc = data_soc * SOC_FACTOR1 * 16 / SOC_FACTOR2 - SOC_FACTOR3 * 16 ;
		}
	}
	else if(MnNiCo_Ternary_Battery_CHILWEE == DevicePara.battery_materialsAndmanufact)
	{
		uint16_t data_adc_max,data_adc_min ;
		if(True == IsBatteryNumConventional())
		{
			data_adc_max = MnNiCo_LV_OPENCRI_MAX_D_Con_CHILWEE ;
			data_adc_min = MnNiCo_LV_OPENCRI_MIN_D_Con_CHILWEE ;
		}
		else
		{
			data_adc_max = MnNiCo_LV_OPENCRI_MAX_D_UnCon_CHILWEE ;
			data_adc_min = MnNiCo_LV_OPENCRI_MIN_D_UnCon_CHILWEE ;
		}

		if(LV_adc < data_adc_min)	
		{
			data_soc = SOC_MIN ;
			data_soc = data_soc * 16 ;
		}
		else if (LV_adc > data_adc_max)
		{
			data_soc = SOC_MAX ;
			data_soc = data_soc * 16 ;
		}	
		else
		{
			//最高压4.187V 最低压3.47589V
			if(48 == LV_Grade)	
			{
				//低于45.916V 的电压和soc的关系为 soc =
				//高于45.916V，低于48.178VV的电压和soc的关系为 soc = 
				//高于48.178V，低于50.856VV的电压和soc的关系为 soc = 
				//高于50.856V，低于52.481V 的电压和soc的关系为 soc =
				//高于52.481V,低于的电压与soc的关系为 soc = 
				if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_Con_CHILWEE)			//45.916V
				{
					//soc = 13.774 * LV_A -622.45	
					//data_soc = 13.774* 0.021257 * LV_D -622.45	
					//data_soc = 0.292794 * LV_D -622.45	
					//16 * data_soc = 4.684703* LV_D -9959
					data_soc = LV_adc ;
					if((data_soc * 14 / 3) > 9959)
						data_soc = data_soc * 14 / 3 - 9959 ;
					else
						data_soc = 0 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_Con_CHILWEE)	//45.916V
				{
					//data_soc = 14.107 * LV_A - 638.97
					//data_soc = 14.107  * 0.021257 * LV_D - 638.97		
					//data_soc = 0.299872 * LV_D - 638.97	
					//16 * data_soc = 4.79796* LV_D - 10223.5
					data_soc = LV_adc ;				
					data_soc = data_soc * 24/5 - 10223 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_Con_CHILWEE)	//48.178V
				{
					//soc = 11.254 * LV_A-502.3
					//soc = 11.254 * 0.021257 * LV_D -502.3
					//soc = 0.239226 * LV_D -502.3
					//16 * soc = 3.82762 * LV_D - 8036.8
					data_soc = LV_adc ;		
					data_soc = data_soc * 23/6	- 8036 ; 
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_Con_CHILWEE)	//50.856V
				{
					//soc = 9.1627 * LV_A -396.28
					//soc = 9.1627 * 0.021257 * LV_D -396.28
					//soc = 0.194772 * LV_D -396.28
					//16 * soc = 3.116344* LV_D - 6340.48
					data_soc = LV_adc ;
					data_soc = data_soc * 25/8	- 6340 ; 
				}
				else										//52.481V
				{
					//soc = 7.7825 * LV_A - 323.85
					//soc = 7.7825  * 0.021257 * LV_D - 323.85
					//soc = 0.165433* LV_D - 323.85
					//16 * soc = 2.646922 * LV_D - 5181.6
					data_soc = LV_adc ;
					data_soc = data_soc * 8/ 3 - 5181 ;
					if(data_soc >= 1600)
						data_soc = 1600 ;
				}	
			}
			else if(60 == LV_Grade)
			{
				if(16 == DevicePara.battery_section_num)
				{
					//低于56.512V 的电压和soc的关系为 soc = 4.915 * U - 265.41
					//高于59.296V，低于58.375V的电压和soc的关系为 soc = 15.443 * U - 851.22
					//高于62.592V，低于67.04V 的电压和soc的关系为 soc =7.3241 * U - 377.25
					//高于64.592V的电压与soc的关系为 soc =1.2344 * U + 17.249
					if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_Con_CHILWEE)
					{
						//soc = 11.161 * LV_A -620.71	
						//data_soc = 11.161* 0.026586914 * LV_D -620.71	
						//data_soc = 0.296737 * LV_D -620.71	\
						//16 * data_soc = 4.74778* LV_D -9931.36
								
						if((data_soc * 19 / 4) > 9959)
							data_soc = data_soc * 19 / 4 - 9959 ;
						else
							data_soc = 0 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_Con_CHILWEE)
					{
						//data_soc = 11.462 * LV_A - 638.97
						//data_soc = 11.462  * 0.026586914 * LV_D - 638.97		
						//data_soc = 0.304739 * LV_D - 638.97	
						//16 * data_soc = 4.87582* LV_D - 10223.5
						data_soc = LV_adc ;
						data_soc = data_soc * 39 / 8 - 10223 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_Con_CHILWEE)
					{
						//soc = 9.1441 * LV_A-502.3
						//soc = 9.1441 * 0.026586914 * LV_D -502.3
						//soc = 0.243113 * LV_D -502.3
						//16 * soc = 3.889814 * LV_D - 8036.8
						data_soc = LV_adc ;
						data_soc = data_soc * 35/9	- 8037 ; 
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_Con_CHILWEE)
					{
						//soc = 7.4447 * LV_A -396.28
						//soc = 7.4447 * 0.026586914 * LV_D -396.28
						//soc = 0.197932 * LV_D -396.28
						//16 * soc = 3.1669055 * LV_D - 6340.48
						data_soc = LV_adc ;
						data_soc = data_soc * 19/6	- 6340 ; 
					}
					else
					{
						//soc = 6.3233 * LV_A - 323.85
						//soc = 6.3233 * 0.026586914 * LV_D - 323.85
						//soc = 0.168117* LV_D - 323.85
						//16 * soc = 2.689872 * LV_D - 5181.6
						data_soc = LV_adc ;
						data_soc = data_soc * 43/ 16 - 5182 ; 
					}
				}
				else		/*battery num is 17*/
				{
					if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_UnCon_CHILWEE)
					{
						//soc = 10.50447 * LV_A -620.71	
						//data_soc = 10.50447* 0.026586914 * LV_D -620.71	
						//data_soc = 0.27928 * LV_D -620.71	
						//16 * data_soc = 4.46850* LV_D -9931.36			
						if((data_soc * 67 / 15) > 9959)
							data_soc = data_soc * 67 / 15 - 9959 ;
						else
							data_soc = 0 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_UnCon_CHILWEE)
					{
						//data_soc = 10.78776 * LV_A - 638.97
						//data_soc = 10.78776  * 0.026586914 * LV_D - 638.97		
						//data_soc = 0.28681 * LV_D - 638.97	
						//16 * data_soc = 4.58901* LV_D - 10223.5
						data_soc = LV_adc ;
						data_soc = data_soc * 55 / 12 - 10223 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_UnCon_CHILWEE)
					{
						//soc = 8.60621 * LV_A-502.3
						//soc = 8.60621 * 0.026586914 * LV_D -502.3
						//soc = 0.22881 * LV_D -502.3
						//16 * soc = 3.66100 * LV_D - 8036.8
						data_soc = LV_adc ;
						data_soc = data_soc * 11/3	- 8037 ; 
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_UnCon_CHILWEE)
					{
						//soc = 7.00677 * LV_A -396.28
						//soc = 7.00677 * 0.026586914 * LV_D -396.28
						//soc = 0.18628 * LV_D -396.28
						//16 * soc = 2.98061 * LV_D - 6340.48
						data_soc = LV_adc ;
						data_soc = data_soc * 3/1	- 6340 ; 
					}
					else
					{
						//soc = 5.95134 * LV_A - 323.85
						//soc = 5.95134 * 0.026586914 * LV_D - 323.85
						//soc = 0.15822* LV_D - 323.85
						//16 * soc = 2.53164 * LV_D - 5181.6
						data_soc = LV_adc ;
						data_soc = data_soc * 38/ 15 - 5182 ; 
					}	

				}
			}
			else if(72 == LV_Grade)
			{
				//低于70.64V 的电压和soc的关系为 
				//高于70.64V，低于74.12V的电压和soc的关系为 
				//高于74.12V，低于78.24V 的电压和soc的关系为
				//高于78.24V,低于80.74V 的电压与soc的关系为 
				//高于80.74V 的电压与soc的关系为 
				if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_Con_CHILWEE)
				{
					//soc = 8.9525 * LV_A -622.45	
					//data_soc = 8.9525* 0.032025146 * LV_D - 622.45
					//data_soc = 0.286705124 * LV_D - 622.45	
					//16 * data_soc = 4.587281982* LV_D -9959.2
					data_soc = LV_adc ;
					if((data_soc * 78 / 17) > 9959)
						data_soc = data_soc * 78 / 17 - 9959 ;
					else
						data_soc = 0 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_Con_CHILWEE)
				{
					//data_soc = 9.1694 * LV_A - 638.97
					//data_soc = 9.1694  * 0.032025146 * LV_D - 638.97		
					//data_soc = 0.293651378* LV_D - 638.97	
					//16 * data_soc = 4.69842205* LV_D - 10223.5
					data_soc = LV_adc ;
					data_soc = data_soc * 47 / 10 - 10223 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_Con_CHILWEE)
				{
					//soc = 7.3153 * LV_A-502.3
					//soc = 7.3153 * 0.032025146 * LV_D -502.3
					//soc = 0.234273554 * LV_D -502.3
					//16 * soc = 3.748376865* LV_D - 8036.8
					data_soc = LV_adc ;
					data_soc = data_soc * 15 / 4	- 8037 ; 
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_Con_CHILWEE)					
				{
					//soc = 5.9558 * LV_A -396.28
					//soc = 5.9558 * 0.032025146 * LV_D -396.28
					//soc = 0.190735367 * LV_D -396.28
					//16 * soc = 3.051765879 * LV_D - 6340.48
					data_soc = LV_adc ;
					data_soc = data_soc * 58 /19	- 6340 ; 
				}
				else
				{
					//soc = 5.0586 * LV_A - 323.85
					//soc = 5.0586 * 0.032025146* LV_D - 323.85
					//soc = 0.162002406* LV_D - 323.85
					//16 * soc = 2.592038496 * LV_D - 5181.6
					data_soc = LV_adc ;
					data_soc = data_soc * 13/ 5 - 5182 ;
				}	
			}
			else
				;
		}
	}
	else if(MnNiCo_Ternary_Battery_PHYLION == DevicePara.battery_materialsAndmanufact)
	{
		uint16_t data_adc_max,data_adc_min ;
		if(True == IsBatteryNumConventional())
		{
			data_adc_max = MnNiCo_LV_OPENCRI_MAX_D_Con_PHYLION ;
			data_adc_min = MnNiCo_LV_OPENCRI_MIN_D_Con_PHYLION ;
		}
		else
		{
			data_adc_max = MnNiCo_LV_OPENCRI_MAX_D_UnCon_PHYLION ;
			data_adc_min = MnNiCo_LV_OPENCRI_MIN_D_UnCon_PHYLION ;
		}

		if(LV_adc < data_adc_min)	
		{
			data_soc = SOC_MIN ;
			data_soc = data_soc * 16 ;
		}
		else if (LV_adc > data_adc_max)
		{
			data_soc = SOC_MAX ;
			data_soc = data_soc * 16 ;
		}	
		else
		{              
			if(48 == LV_Grade)	
			{
				if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_Con_PHYLION)			//3.47V * 13
				{
					//soc = 0.5741 * LV_A - 20.896	
					//data_soc = 0.5741* 0.021257 * LV_D - 20.896		
					//data_soc = 0.012203722 * LV_D - 20.896
					//16 * data_soc = 0.195259552 * LV_D - 334.336
					data_soc = LV_adc ;
					if((data_soc * 1 / 5) > 334)
						data_soc = data_soc * 1 / 5 - 334 ;
					else
						data_soc = 0 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_Con_PHYLION)		//3.63V * 13
				{
					//data_soc = 2.4038 * LV_A - 103.44
					//data_soc = 2.4038  * 0.021257 * LV_D - 103.44		
					//data_soc = 0.051097905 * LV_D - 103.44	
					//16 * data_soc = 0.817566472 * LV_D - 1655.04
					
					data_soc = LV_adc ;				
					data_soc = data_soc * 9/11 - 1655 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_Con_PHYLION)		//3.85V * 13
				{
					//soc = 8.2128 * LV_A - 377.16
					//soc = 8.2128 * 0.021257 * LV_D - 377.16
					//soc = 0.17458061 * LV_D - 377.16
					//16 * soc =  2.79328976 * LV_D - 6034.56
					data_soc = LV_adc ;		
					data_soc = data_soc * 14/5	- 6034 ; 
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_Con_PHYLION)		//4.11V * 13
				{
					//soc = 18.081 * LV_A - 871.43
					//soc = 18.081 * 0.021257 * LV_D - 871.43
					//soc = 0.384350284 * LV_D - 871.43
					//16 * soc = 6.149604537 * LV_D - 13942.88
					data_soc = LV_adc ;
					data_soc = data_soc * 80/13	- 13942 ; 
				}
				else														//lower than 4.2V * 13
				{
					//soc = 4.2735 * LV_A - 133.33
					//soc = 4.2735   * 0.021257 * LV_D - 133.33
					//soc = 0.090842372 * LV_D - 133.33
					//16 * soc = 1.45347796 * LV_D - 2133.28
					data_soc = LV_adc ;
					data_soc = data_soc * 16/ 11 - 2133 ;
					if(data_soc >= 1600)
						data_soc = 1600 ;
				}	
			}
			else if(60 == LV_Grade)
			{
				if(16 == DevicePara.battery_section_num)
				{
					if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_Con_PHYLION)
					{
						//soc = 0.46645625 * LV_A -20.896	
						//data_soc = 0.46645625* 0.026586914 * LV_D -20.896	
						//data_soc = 0.012401632 * LV_D -20.896	
						//16 * data_soc = 0.198426116* LV_D -334.336	
						data_soc = LV_adc ;
						if((data_soc * 1 / 5) > 334)
							data_soc = data_soc * 1 / 5 - 334 ;
						else
							data_soc = 0 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_Con_PHYLION)
					{
						//data_soc = 1.9530875 * LV_A -103.44
						//data_soc = 1.9530875  * 0.026586914 * LV_D -103.44		
						//data_soc = 0.05192657 * LV_D -103.44	
						//16 * data_soc = 0.830825112 * LV_D -1655.04
						data_soc = LV_adc ;
						data_soc = data_soc * 5 / 6 - 1655 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_Con_PHYLION)
					{
						//soc = 6.6729 * LV_A-377.16
						//soc = 6.6729 * 0.026586914 * LV_D -377.16
						//soc = 0.177411819 * LV_D -377.16
						//16 * soc = 2.838589102 * LV_D -6034.56
						data_soc = LV_adc ;
						data_soc = data_soc * 17 / 6 - 6034 ; 
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_Con_PHYLION)
					{
						//soc = 14.6908125 * LV_A -871.43
						//soc = 14.6908125 * 0.026586914 * LV_D -871.43
						//soc = 0.390583369 * LV_D -871.43
						//16 * soc = 6.249333911 * LV_D-13942.88
						data_soc = LV_adc ;
						data_soc = data_soc * 25 / 4 - 13942 ; 
					}
					else
					{
						//soc = 3.47221875 * LV_A -133.33
						//soc = 3.47221875 * 0.026586914 * LV_D -133.33
						//soc = 0.092315582* LV_D -133.33
						//16 * soc = 1.477049304 * LV_D -2133.28
						data_soc = LV_adc ;
						data_soc = data_soc * 28/ 19 - 2133 ; 
					}	
				}
				else /*unconventional battery num*/
				{
					if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_UnCon_PHYLION)
					{
						//soc = 0.43901 * LV_A -20.896	
						//data_soc = 0.43901* 0.026586914 * LV_D -20.896	
						//data_soc = 0.01167 * LV_D -20.896	
						//16 * data_soc = 0.18675* LV_D -334.336	
						data_soc = LV_adc ;
						if((data_soc * 2 / 11) > 334)
							data_soc = data_soc * 2 / 11 - 334 ;
						else
							data_soc = 0 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_UnCon_PHYLION)
					{
						//data_soc = 1.8382 * LV_A -103.44
						//data_soc = 1.8382  * 0.026586914 * LV_D -103.44		
						//data_soc = 0.04887 * LV_D -103.44	
						//16 * data_soc = 0.78195 * LV_D -1655.04
						data_soc = LV_adc ;
						data_soc = data_soc * 7 / 9 - 1655 ;
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_UnCon_PHYLION)
					{
						//soc = 6.280376 * LV_A-377.16
						//soc = 6.280376 * 0.026586914 * LV_D -377.16
						//soc = 0.16697 * LV_D -377.16
						//16 * soc = 2.67161 * LV_D -6034.56
						data_soc = LV_adc ;
						data_soc = data_soc * 8 / 3 - 6034 ; 
					}
					else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_UnCon_PHYLION)
					{
						//soc = 13.82664 * LV_A -871.43
						//soc = 13.82664 * 0.026586914 * LV_D -871.43
						//soc = 0.36760 * LV_D -871.43
						//16 * soc = 5.88172 * LV_D-13942.88
						data_soc = LV_adc ;
						data_soc = data_soc * 47 / 8 - 13942 ; 
					}
					else
					{
						//soc = 3.26797 * LV_A -133.33
						//soc = 3.26797 * 0.026586914 * LV_D -133.33
						//soc = 0.086885* LV_D -133.33
						//16 * soc = 1.390164 * LV_D -2133.28
						data_soc = LV_adc ;
						data_soc = data_soc * 18/ 13 - 2133 ; 
					}	
					
				
				
				}	
			}				
			else if(72 == LV_Grade)
			{ 
				if(LV_adc <= MnNiCo_LV_K_Turning_Point1_D_Con_PHYLION)
				{
					//soc = 0.373165 * LV_A -20.896	
					//data_soc = 0.373165* 0.03202514 * LV_D -20.896
					//data_soc = 0.011950664 * LV_D -20.896	
					//16 * data_soc = 0.191210621* LV_D -334.336
					data_soc = LV_adc ;
					if((data_soc * 1 / 5) > 334)
						data_soc = data_soc * 1 / 5 - 334 ;
					else
						data_soc = 0 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point2_D_Con_PHYLION)
				{
					//data_soc = 1.56247 * LV_A -103.44
					//data_soc = 1.56247  * 0.032025146 * LV_D -103.44		
					//data_soc = 0.050038331* LV_D -103.44	
					//16 * data_soc = 0.80061329* LV_D -1655.04
					data_soc = LV_adc ;
					data_soc = data_soc * 4 / 5 - 1655 ;
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point3_D_Con_PHYLION)
				{
					//soc = 5.33832	 * LV_A-377.16
					//soc = 5.33832	 * 0.032025146 * LV_D -377.16
					//soc = 0.17096048 * LV_D -377.16
					//16 * soc = 2.73536768* LV_D -6034.56
					data_soc = LV_adc ;
					data_soc = data_soc * 30 / 11	- 6034 ; 
				}
				else if(LV_adc <= MnNiCo_LV_K_Turning_Point4_D_Con_PHYLION)
				{
					//soc = 11.75265 * LV_A -871.43
					//soc = 11.75265 * 0.032025146 * LV_D -871.43
					//soc = 0.376380338 * LV_D -871.43
					//16 * soc = 6.022085405 * LV_D -13942.88
					data_soc = LV_adc ;
					data_soc = data_soc * 157 /26	- 13942 ; 
				}
				else
				{
					//soc = 2.777775 * LV_A -133.33
					//soc = 2.777775 * 0.032025146* LV_D -133.33
					//soc = 0.088958651* LV_D -133.33	
					//16 * soc = 1.42333842 * LV_D -2133.28
					data_soc = LV_adc ;
					data_soc = data_soc * 10/ 7 - 2133 ; 
				}	
			}
			else
				;
		}	
	}
	else
		;
	
	if(data_soc > (SOC_MAX * 16))
		data_soc = SOC_MAX * 16 ;
	return (uint16_t)data_soc ;
}      
			

#if(Commu_Mode_Common != Commu_Mode)
/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
/*void InitialQ(void)
{
	
	Q.data_release_temp= 0 ;  
	Q.data_initial= Q.data_max.bit32 / 100 * Q.soc_initial;
	Q.data_remainder.bit32 = Q.data_initial ; 
}*/

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
uint16_t ReturnRatioReleaseDivMileageMul256InitialValue(void)
{
	uint64_t temp_cal ;
	temp_cal = Q.data_max.sum32 ;
	temp_cal = temp_cal * 255 / Mileage.counter_hallchange_max.sum32 ;
	return (uint16_t)temp_cal ;
}


/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
uint32_t CalQmax(void)
{
	/*Q_max = Current_release_temp * TimePeriod_OneLineCommu * 100 /(soc_initial - soc_1h_afteropen  );
	data_max = Q_max/TimePeriod_OneLineCommu*/
	if(0 != Q.soc_release)
	{
		uint64_t temp_cal;
		temp_cal = Q.data_release_temp   ;
		temp_cal = temp_cal * 16 * 100 / Q.soc_release   ;
		return (uint32_t)temp_cal ;
	}
		//return (Q.data_release_temp   * 100 / Q.soc_release); 
	else
		return Q.data_max.sum32 ;
	
	//  UpdateQmaxInFlash();
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ReduceQRemainderBasedCurrent(void)
{ 
	//  if(Q.data_remainder.bit32 > 0)
	//	Q.data_remainder.sum32 = Q.data_initial- Q.data_release_temp;
	//  else
	//	Q.data_remainder.sum32 = 0 ;
	if(Q.data_initial > Q.data_release_temp)
		Q.data_remainder.sum32 = Q.data_initial- Q.data_release_temp;
	else
		Q.data_remainder.sum32 = 0 ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
/*void QInitalAndQuitCalQMax(void)
{
	Q.soc_initial = GetBatterySoc();     
	//InitialQ(); 
    Q.data_release_temp= 0 ;  
	Q.data_initial= Q.data_max.bit32 / 100 * Q.soc_initial;
	Q.data_remainder.bit32 = Q.data_initial ; 
}*/

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void UpdataQInitalAndRemainder(void)
{
	/*QInitalAndQuitCalQMax();
	Q.data_remainder.bit32 =  Q.data_initial ; */
    
    Q.soc_initial = GetBatterySoc();     
	//InitialQ(); 
    Q.data_release_temp = 0 ;  
	 
	uint64_t temp_cal;
	temp_cal = Q.data_max.sum32   ;
	temp_cal = temp_cal * Q.soc_initial / 16 / 100 ;
	Q.data_initial = (uint32_t)temp_cal ;
	//Q.data_initial = Q.data_max.bit32 / 100 * Q.soc_initial;
	Q.data_remainder.sum32 = Q.data_initial ; 
}

#if(COMPILE_ENABLE == Double_LVSet)
/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsLVLowerThanLV1Level(void)
{
	if(LV.avr_adc < LV_Level1_D)
		return True ;
	else
		return False ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsLVHigherThanLV1RecoverLevel(void)
{
	if(LV.avr_adc >= LV_Level1Recover_D)
		return True ;
	else
		return False ;
}

#endif
#endif



/*******************************************************************************
* 函数名  		:
* 函数描述    	: run状态，timer_stall_500ms不断清零；非run状态，timer_stall_500ms不断累加。
				当	timer_stall_500ms累计到Time_Stall_Max 进入park状态
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void GetMotorStatus(void)  
{
	if(Status_Motor_Run == Motor.status_motor)
	{
		if(GetHallSpeed() < MinHall_WhenMotorRunning)
		{
			Motor.status_motor = Status_Motor_Stall ;
		}
		else
			Motor.timer_stall_500ms = 0 ;		//stall清零
	}
	else if(Status_Motor_Stall == Motor.status_motor)
	{
		if(GetHallSpeed() >= MinHall_WhenMotorRunning)
		{
			Motor.timer_stall_500ms = 0 ;		//stall清零
			Motor.status_motor = Status_Motor_Run ;
		}
		else
		{
			if(Motor.timer_stall_500ms >= Time_Stall_Max )	//stall计时达到
			{
				Motor.status_motor = Status_Motor_Park ;
			}
			else
				;		
		}
	}
	else	//Park
	{	
		if(GetHallSpeed() >= MinHall_WhenMotorRunning)
		{
			Motor.timer_stall_500ms = 0 ;		//stall清零
			Motor.status_motor = Status_Motor_Run ;
		}
	}
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 副作用		：	当电池实际电量远大于给定的电量时，可能会一直回复在充电中
						比如，实际20AH，代码中给定为45AH，就会在park时，不断报警在充电中
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************soc_release + ************************************************************/
FlagStatus IsChargeStatus(void) 
{
    uint16_t soc_charge ;
	Union4Bytes_TypeDef temp_cal ;
#if(Commu_Mode_Common == Commu_Mode)
    UnionSumWithH8L8_TypeDef mileage_remainder;
    UnionSumWithH8L8_TypeDef mileage_max ;
	
	mileage_remainder.StructH8L8.h8 = Mileage.counter_hallchange_remainder.bit8[3] ;
	mileage_remainder.StructH8L8.l8 = Mileage.counter_hallchange_remainder.bit8[2] ;
	
	mileage_max.StructH8L8.h8 = Mileage.counter_hallchange_max.bit8[3] ;
	mileage_max.StructH8L8.l8 = Mileage.counter_hallchange_max.bit8[2] ;
	
	temp_cal.sum32 = mileage_remainder.sum ;
	temp_cal.sum32 = temp_cal.sum * ONE_HUNDRED / mileage_max.sum + Soc_Charge_Criteria;
	soc_charge = temp_cal.StructH16L16.l16 ;
    //soc_charge.bit32 = ONE_HUNDRED * mileage_remainder / mileage_max + Soc_Charge_Criteria;    
#else
	UnionSumWithH8L8_TypeDef q_remainder  ;
	UnionSumWithH8L8_TypeDef q_max   ;
	q_remainder.h8 = Q.data_remainder.bit8[3] ;
	q_remainder.l8 = Q.data_remainder.bit8[2] ;
	
	q_max.h8 = Q.data_max.bit8[3] ;
	q_max.l8 = Q.data_max.bit8[2] ;
	
	temp_cal.sum32 = q_remainder.sum ;
	temp_cal.sum32 = temp_cal.sum32 * ONE_HUNDRED /  q_max.sum + Soc_Charge_Criteria;
	soc_charge = temp_cal.StructH16L16.l16 ;
	 //soc_charge.bit32 = ONE_HUNDRED * q_remainder.sum / q_max.sum + Soc_Charge_Criteria;	
#endif
    
	soc_charge = soc_charge * 16 ;
   // if(GetBatterySoc() >= soc_charge.bit8[0])
	if(GetBatterySoc() > soc_charge)
        return True ;
    else
        return False ;
}


/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
/*FlagStatus IsOpenVoltageInChargingStatus(void)                        
{ 
	if(LV.data_45min_afteropen < LV_Uncharging_Max_D &&  LV.data_1h_afteropen < LV_Uncharging_Max_D)
	{
		if (LV.data_1h_afteropen>= LV.data_45min_afteropen)
		{
			if((LV.data_1h_afteropen - LV.data_45min_afteropen) >= Delta_Voltage_Open_Stable_Max_D )
				return YES ;
			else
			{
				Q.soc_1h_afteropen =  CalSoc(LV.data_1h_afteropen) ;
				//		if(Q.soc_1h_afteropen < Q.soc_initial && Q.data_release_temp > 0)
				if(Q.soc_1h_afteropen < Q.soc_initial && Mileage.counter_hallchange_accumu.bit32 > 0  )
					return NO ;
				else
					return YES ;
			}	  
		}
		else
			return YES ;  	
	}
	else
		return YES ;
}	
*/
/*******************************************************************************
* 函数名  		:
* 函数描述    	:  CPU will sett a flag after battery charged.(when LV_1h is more large than LV_45min .)
CPU will update Qremainder when BLE module connect to controller.
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
/*FlagStatus SetFlagBatteryCharged(void)                        
{ 
	return SET ;
}*/

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
/*void StartOpenCircuitVoltageTimer(void)                         //RTC
{
	LV.timer_openciruit_1s = 0 ;
}*/

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void CalQInitialAndMileageRemainder(void)     
{
#if(Commu_Mode_Common != Commu_Mode)
	UpdataQInitalAndRemainder();
#endif
	UpdateMileageInitialAndRemainder();
}	

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
/*uint32_t CalMileageRemainderWithoutUpdataQInitial(void)   
{
	 return (Mileage.counter_hallchange_max.bit32 / 100 * GetBatterySoc()) ;
}*/

/*******************************************************************************
* 函数名  		: 
* 函数描述    	: 
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsBatteryNumConventional(void) 
{
#if(48 == LV_Grade)
	if(13 == DevicePara.battery_section_num)
		return True ;
	else
		return False ;
#elif(60 == LV_Grade)
	if(16 == DevicePara.battery_section_num)
		return True ;
	else
		return False ;
#elif(72 == LV_Grade)
		return True ;
#endif
}