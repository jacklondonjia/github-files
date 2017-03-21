/*
1.����δָ��������Ϊ�������������������ģ����˵��
	����ȡ��һ�������ߣ��ۼƵĻ�����Ϊ�û�����0.5s���ۼƵ���Ŀ



2.����ָ��������Ϊ�������������������ģ����˵��
	������Ϊһ��ͨ������0.5s�ڻ�����Ŀ
	Attention������Ļ���������һ������6�λ����仯���жϵġ�

һ��ͨһ֡����ʱ��Ϊ0.678s

���ƹ��ĵ綯��Ϊ���� ��ȫ�ٵ���Ϊ8A ʱ��Ϊ32km
ָ��������Ϊ���󣬵綯���ܹ����е�ʱ��Ϊtime=Qremainder*0.678s/8A
		���=time*25*1000/3600






*/

#ifndef __MILEAGE_H
#define __MILEAGE_H

/****************************************************/
/*********************typedef************************/
typedef	struct
{
#if(Commu_Mode_Common == Commu_Mode)
	Level_TypeDef Level	;
	uint16_t counter_temp_hallUchange_per500ms ;
	uint16_t counter_hallchange_per500ms ;
#else
	uint8_t counter_frame ;
	uint32_t counter_hallchange_perfixedtime ;
	uint32_t counter_hallchange_remainder_basedpresent ;
	uint32_t counter_hallchange_remainder_basedprevious ;	
	uint32_t counter_lowpassfilter_perframe ;
	
	
	uint16_t ratio_releasedivmileage_mul256 ;		//Ϊ�˾��� �ñ����ǳ���256�� �����ʱ��Ҫȥ��256��Ӱ��
#endif
	uint16_t counter_hall_speed_filtered ;
	Union4Bytes_TypeDef	counter_hallchange_accumu ;
	Union4Bytes_TypeDef	counter_hallchange_max ;
	uint32_t counter_hallchange_initial ;
	Union4Bytes_TypeDef	counter_hallchange_remainder ;//��Ϊ��Ҫ�Ѹò������������ͣ������Ҫת����ʽ
	Union4Bytes_TypeDef	odometer_perKM ;
	uint32_t caledvalue_hallchange_1KM ;
	uint32_t counter_hallchange_accumu_under1KM ;	
	
}Mileage_TypeDef;

	
	
/******************define**********************/
#if(Commu_Mode_Common == Commu_Mode)
	#define	HallFrameTime_Compensation_Ratio						1
#else
	#define	Counter_Frame_Onelinecommunica_For_MileageCal			15//177
	#define	Timer_PerFrame_Onelinecommunica_For_MileageCal			0.678	//s
	#define	Timer_HallSpeedPerFrame_Onelinecommunica_For_MileageCal	0.5	//s
	#define	HallFrameTime_Compensation_Ratio	(Timer_HallSpeedPerFrame_Onelinecommunica_For_MileageCal\
													/ Timer_PerFrame_Onelinecommunica_For_MileageCal)
	#define	Mileage_Remainder_Weighted_CardinalNumber				10
	#define	Mileage_Remainder_Weighted_Persent						1
	#define	Mileage_Remainder_Weighted_Previous				(Mileage_Remainder_Weighted_CardinalNumber-Mileage_Remainder_Weighted_Persent)
	#define	Mileage_Max_Weighted_CardinalNumber					10
	#define	Mileage_Max_Weighted_CumulativeOperationResults		8
	#define	Mileage_Max_Weighted_ThisRunResult				\
		(Mileage_Max_Weighted_CardinalNumber-Mileage_Max_Weighted_CumulativeOperationResults)	
	#define	QDivMileage_Weighted_CardinalNumber					10
	#define	QDivMileage_Weighted_CumulativeOperationResults		9
	#define	QDivMileage_Weighted_ThisRunResult				\
		(QDivMileage_Weighted_CardinalNumber- QDivMileage_Weighted_Weighted_CumulativeOperationResults)			
			
#endif

			
		

#define	Gear_Ratio						(Gear_Ratio_Motor_InFlash / Gear_Ratio_Wheel_InFlash)

#define Perimeter_Wheel_Meter			(PI * Wheel_Diameter_Inch_Int_InFlash * 2.54 /100)//1.3
#define HallNum_Per_Round               (6 * Number_Pole_Pairs_InFlash * Gear_Ratio)
//#define Mileage_PerKm					(uint32_t)(1000 * HallFrameTime_Compensation_Ratio * Gear_Ratio *\
//										Number_Pole_Pairs * 6 / Perimeter_Wheel_Meter)
//#define Mileage_Max						(uint32_t)(BatteryStandardM  *\
//										HallFrameTime_Compensation_Ratio * Gear_Ratio *\
//										Number_Pole_Pairs * 6 / Perimeter_Wheel_Meter)
#define Mileage_Max_InFlash						(uint32_t)(BatteryStandardM_InFlash * HallNum_Per_Round\
                                            * HallFrameTime_Compensation_Ratio/ Perimeter_Wheel_Meter)

/*#define Mileage_Max_12Ah				(uint32_t)(Mileage_Max_20Ah * 12 / 20)
#if(20 == AH_Battery)
	#define Mileage_Max						Mileage_Max_20Ah
#elif(12 == AH_Battery)
	#define Mileage_Max						Mileage_Max_12Ah
#else
#endif*/
#define Mileage_max_Flash_Dleta_MAX		((uint32_t)(Mileage_Max_InFlash/100))
#define	BatteryStandardKM_InFlash		(uint8_t)(BatteryStandardM_InFlash/1000)


/*******************Rated************************/
#define	Rated_Q_Release_PerFrame		(uint8_t)(Rated_Q_Release_PerFrame_A_InFlash * 4)    
//#define	Rated_Q_Release_PerFrame		(60*(AverageVoltage_CalByEnergy_D >> LV_Accuracy_Reduced_ForQ))
//#define	Rated_HallChange_PerFrame		587

/*******************************************************************************************************************
*              ����̣�50000m��                                              ����תһȦ�Ļ�������HallNum_Per_Round��
*    ��ʽ = �������������������������������������������������������� * һ��ͨ��hall�ٶȵ�ʱ�䣨0.5s��*����������������������������������������������������������������������������������������
*             ��ʱ����20AH��                                                 ����һȦ���ܳ���Perimeter_Wheel_Meter��
*            ���������������������������� * 3600
*              ��ֵ������A��

***********************************************************************************************************************/
#if 0
#define	Rated_HallChange_PerFrame		(uint16_t)(BatteryStandardM_InFlash * Rated_Q_Release_PerFrame_A_InFlash\
                                        * Timer_HallSpeedPerFrame_Onelinecommunica_For_MileageCal\
                                        * HallNum_Per_Round / (Perimeter_Wheel_Meter\
                                        * AH_Battery_InFlash * TimePeriod_1h_Per1s))
#endif
/*******************Variable**********************/
/******************Mileage**********************/
extern Mileage_TypeDef	Mileage ;

/*******************Function*********************/
#if(Commu_Mode_Common == Commu_Mode)	
	void UpdateHallLevel(void);
	//void ReadHallLevel1AndLevel2(void);
	void UpdateHallSpeedPer500ms(void);
	void CalMileageRemainder(void) ;
#else
	void CalQAndMileageReleaseAndCurrentAvr(void)  ;
	void CalCurrentReleaseAndHallChangeInFixedTime(void);
	uint32_t CalCounterHallchange1KM(void) ;
#if 0
	uint32_t CalMileageBasedRatedRatio(uint32_t) ;
#endif
	uint32_t CalMileageRemainderBasedSubtract(void);
	//uint32_t CalMileageRemainderBasedRatedRatio(void) ;
	uint32_t CalMileageRemainderBasedLowPassFilterRatioPerFrame(void);
	uint32_t CalMileageRemainderBasedPersentOperatingStatus(void);
	uint32_t CalMileageRemainderBasedPreviousOperatingStatus(void);
	uint32_t CalMileageRemainderBasedWeightedCal(void);
	uint16_t GetRatioReleaseDivMileageMul256(void) ;
	uint32_t GetMileageRemainderBasedRatioReleaseDivMileage(void) ;
#endif

void GetHallSpeedFilteredValue(void) ;	
uint16_t GetHallSpeed(void);
#if 0
uint32_t CalMileageMax(void) ;
#endif
void UpdateMileageInitialAndRemainder(void);
//void MileageInitalAndQuitCalMileageMax(void);
//void UpdataMileageRemainder(void);
//FlagStatus AnalysisStateOfParkWhenConnectedWithController(void) ;






#endif
