//#include "stdint.h"
//#include "stdio.h"
//#include "TypeDefines.h"
//#include "Defines.h"
//#include "_Parameter.h"
//#include "globalvariables.h"
//#include "ATcontrol.h"
//#include "BatteryPowerCal.h"
//#include "Guard.h"
//#include "Onelinecommunica.h"
//#include "sendandstring.h"
//#include "MileageCal.h"
//#include "ErrorHistory.h"
//#include "FreeKeyAndMechaLock.h"
//#include "BLESystem.h"
#include "CustomerPara.h"
#include "Board.h"




#if(Commu_Mode_Common != Commu_Mode)
/*************************************************/
/*******************Variable**********************/
OLC_TypeDef OLC ;
OLCM_TypeDef OLCM ;

/****************************************************/
/**********************function**********************/
/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void ReadOLCLevel(void)
{	
	if(OLC.Level2Data.Level.status != OLC.Level2Data.Level.status_temp)
	{
		UpdataOLCLevelStatus(); 
		if(OLC.Level2Data.Level.status == Low_Level)
		{
			if(OLC.Level2Data.Level.timer_lowlevel >= OLC_Counter_Start_Bit)
				DetectStartBit();
			else			
				GetOLCLogicValue();
		}	  
	} 
	else
		;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void UpdataOLCLevelStatus(void)
{
  	if(OLC.Level2Data.Level.status_temp == 0)
  	{
		OLC.Level2Data.Level.timer_highlevel = OLC.Level2Data.Level.timer_temp ;	
  	}	  
  	else
  	{	
		OLC.Level2Data.Level.timer_lowlevel = OLC.Level2Data.Level.timer_temp ;
  	} 
  	OLC.Level2Data.Level.timer_temp = 0 ;
  	OLC.Level2Data.Level.status = OLC.Level2Data.Level.status_temp ;

  	BLEMdlCnt.timer_connect2controller_100ms = 0 ;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void GetOLCLogicValue(void)
{
  OLC.Level2Data.data8bit_temp <<= 1 ;
  if(OLC.Level2Data.Level.timer_highlevel > OLC.Level2Data.Level.timer_lowlevel )
  {  
	if((OLC.Level2Data.Level.timer_highlevel- OLC.Level2Data.Level.timer_lowlevel) > 5)
	  //	  logic_temp = 1 ;		  
	  OLC.Level2Data.data8bit_temp ++ ;
	else
	  OLC.Flag_code_receive_wrong = YES ;	  
  }
  else
  {	
	if((OLC.Level2Data.Level.timer_lowlevel- OLC.Level2Data.Level.timer_highlevel) > 5)
	  //	  logic_temp = 0 ;		  
	  ;
	else
	  OLC.Flag_code_receive_wrong = YES ;	
  } 
  OLC.Level2Data.counter_displacement ++ ;
  if(0 == OLC.Level2Data.counter_displacement % 8 )
	Write8bitToCodeTemp(); 
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void Write8bitToCodeTemp(void)
{
	//stack the new byte
	OLC.DataBuff.Data[(OLC.Level2Data.counter_displacement / 8 ) - 1] = OLC.Level2Data.data8bit_temp ;
	OLCType_ETypeDef type ;
	type = GetOLCType() ;
	
	if(OLCType_Common == GetOLCType())
		WriteToCommuCode();	
	
#if(Commu_Mode_TwoWire == Commu_Mode)
	else if(OLCType_ReadControllerE2PROMCfg == GetOLCType())
		GetReadControllerConfigCmdReplyMessage();
	else if (OLCType_WriteControllerE2PROMCfg == type)
		GetWriteControllerConfigCmdReplyMessage();
#endif
	else if(OLCType_Unfinished == type)
		;
	else
		;	
	OLC.Level2Data.data8bit_temp = 0 ;
}	

/*******************************************************************************
* ������  		: 
* ��������    	: 
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
OLCType_ETypeDef GetOLCType(void)
{
	if((OLC_CommonCmd_Strlen * 8) == OLC.Level2Data.counter_displacement )
	{
		if((True == IsDeviceCodeCompliant(DeviceCode_CommonCmd))&&\
			(True == IsSeqCodeCompliant(SeqCode_Commoncmd)))
			return OLCType_Common ;
		
		else
			return OLCType_Undefined ;
			
	}
	else if(((T2C_WriteConfig_ValidStrlen + 3) * 8) == OLC.Level2Data.counter_displacement)
	{
		if(True == IsDeviceCodeCompliant(DeviceCode_ConfigCmd))
			return OLCType_ReadControllerE2PROMCfg ;
			
		else
			return OLCType_Undefined ;
		
	}
	else if((5 * 8) == OLC.Level2Data.counter_displacement)
	{
		if(True == IsDeviceCodeCompliant(DeviceCode_ConfigCmd))
			return OLCType_WriteControllerE2PROMCfg;
			
		else
			return OLCType_Undefined ;
	}
	else
		return OLCType_Unfinished ;
	
}


/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void WriteToCommuCode(void)
{
	if(True == IsSumCompliant(OLC.DataBuff.checksum))
	{			
		GetOneLineCommuMessage();
		CalQAndMileageReleaseAndCurrentAvr();
		CalCurrentReleaseAndHallChangeInFixedTime() ;
		//Guard.status_park = AnalysisStateOfParkWhenConnectedWithController();
		SaveRunMessage();
				
		ErrorHistoryMessage.len_entries_towrite = ReturnSaveErrorHistoryEntryLen() ;
		if(0 != ErrorHistoryMessage.len_entries_towrite)	
			SaveErrorHistoryMessage() ; 
				
#if(CyclicSendMessagePer0_678s == CyclicSendPeriod)	
		if(True == AT.mode_cyclicsend)
			CyclicSendData();
		else
			;
#endif  
				
	}
	else
		;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void DetectStartBit(void)
{
  OLC.Level2Data.data8bit_temp = 0;
  OLC.Level2Data.counter_displacement = 0;
  OLC.Flag_code_receive_wrong= NO ;
}
/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
FlagStatus IsDeviceCodeCompliant(uint8_t devicecode)
{
  if( devicecode == OLC.DataBuff.devicecode )
  	return True ;
  else
	return False ; 
}

 /*******************************************************************************
 * ������		 :
 * �������� 	 :
 * ������� 	 : ��
 * ������ 	 : ��
 * ����ֵ		 : ��
 *******************************************************************************/
FlagStatus IsSeqCodeCompliant(uint8_t seqcode)
{
  if(seqcode == OLC.DataBuff.seqcode)
	return True ;
  else
	return False ; 
}
/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
FlagStatus IsSumCompliant(void)
{
  uint8_t checksum_temp ;
  checksum_temp = OLC.DataBuff.Data[0] ;
  
  for(uint8_t i = 1 ; i<= (OLC.Level2Data.counter_displacement / 8 - 2 );i ++)
	checksum_temp= checksum_temp ^  OLC.DataBuff.Data[i] ;
  if(checksum_temp == OLC.DataBuff.Data[OLC.Level2Data.counter_displacement/8 - 1])
	return True ;
  else
	return False ; 
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void GetOneLineCommuMessage(void)
{
	//get raw data
	OLCM.rawdata[0] = OLC.DataBuff.errorbyte ;    //error cruise and assist and so on 
	OLCM.rawdata[1] = OLC.DataBuff.switchbyte ;    //threespeed,charge,break,runflag and so on 
	OLCM.rawdata[2] = OLC.DataBuff.protectbyte ;    //overcurrent,protect,back,speedlimit and so on
	OLCM.rawdata[3] = OLC.DataBuff.current_lowbyte ;    //current     the value div 4 is real current
	
	//Get counter_hall_speed
	OLCM.counter_hall_speed.h8 = OLC.DataBuff.hallspeed_highbyte ;
	OLCM.counter_hall_speed.l8 = OLC.DataBuff.hallspeed_lowbyte ;
	if(OLCM.counter_hall_speed.sum < 0x15)
		OLCM.counter_hall_speed.sum = 0 ;
	GetHallSpeedFilteredValue();
	
	//Get current
	if(OLC.DataBuff.current_lowbyte <= 0x01 || OLC.DataBuff.current_lowbyte >= 0XFE  )
		OLCM.data_current = 0 ;
	else
		OLCM.data_current = OLC.DataBuff.current_lowbyte ;	//div 4 is the real current
		
//	if(OneLineCommu.datacode[10] > 0x01)
//		OneLineCommuMessage.data_current = OneLineCommu.datacode[10] ;	//����4������ʵֵ
//	else
//		OneLineCommuMessage.data_current = 0 ;
	
	//Get error
	//��Ϊȥ��D7��D2��D1��Ӱ�죬�����0111 1001��0x79��
	OLCM.data_error = OLC.DataBuff.errorbyte & 0x79 ;
	
	if(OLC.DataBuff.switchbyte & U8_BIT5)
	{
		OLCM.flag_brake = True ;
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		if((Status_PowerOn == FreeKey.status) && (MechalMotorLock.status == Status_WaitForBrake))
		{
			MechalMotorLock.counter_brake_filter ++ ;
			if(MechalMotorLock.counter_brake_filter >= Counter_Brake_Filter)
				MechalMotorLock.flag_breakconfirm = True ;
		}
		else
		{
			MechalMotorLock.counter_brake_filter = 0 ;
			MechalMotorLock.flag_breakconfirm = False ;
		}
	#endif
	}
	else
	{
		OLCM.flag_brake = False ;
	#if(ElectricLockControl == Anti_theftDevice_Function ||  ElectricDouSupport == Anti_theftDevice_Function)
		MechalMotorLock.counter_brake_filter = 0 ;
		MechalMotorLock.flag_breakconfirm = False ;
	#endif
	
	}
	
	
	//Get motor run status
	if(OLC.DataBuff.switchbyte & U8_BIT6)
		OLCM.flag_motor_running = True ;
	else
		OLCM.flag_motor_running = False ;

	
	//static uint16_t LV_last = 0 ;
	static uint8_t counter = 0 ;
	uint32_t cal_temp ;
	//Get Q release in this frame
	if(OLCM.data_current > 0X01)
	{
		/*//��������0.678sǰ���ѹ�Ĳ�ֵ������2����ֵ���ƶ���ֵ����
		cal_temp = (LV.avr_adc + LV_last ) / 2 ;
		
		//counter���������������������ĵ�����Ϣ��ΪĨȥβ�����µ����
		//���������15A���� 	���Ϊ1/4 / 15 =1.6%
		if(counter % 2)
			cal_temp = (cal_temp>>LV_Accuracy_Reduced_ForQ) * (OLCM.data_current + 1);
		else
			cal_temp = (cal_temp>>LV_Accuracy_Reduced_ForQ) * OLCM.data_current ;*/
		
		//counter���������������������ĵ�����Ϣ��ΪĨȥβ�����µ����
		//���������15A���� 	���Ϊ1/4 / 15 =1.6%
		if(counter % 2)
			cal_temp = OLCM.data_current + 1;
		else
			cal_temp = OLCM.data_current ;
				
		OLCM.data_Q_thisframe = cal_temp ;
		counter ++ ;
		//OLCM.data_Q_thisframe = ((LV.avr_adc >>LV_Accuracy_Reduced_ForQ) * OneLineCommuMessage.data_current);
		// OLCM.data_Q_thisframe =  OneLineCommuMessage.data_current ;
		//LowPassFilteringQPerFrame();
		//LowPassFilteringMileagePerFrame();
	}
	else
		OLCM.data_Q_thisframe = 0 ;
	//LV_last = LV.avr_adc ;

}
/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ���ص��Ǹ߾��ȵ���������ֵ
* ����ֵ        : ��
*******************************************************************************/
//void InitialLowPassFilteringPerFrame(void)
//{
//	uint64_t temp_cal ;
//	temp_cal = 15 * 4 ;	//����15A����
//	temp_cal = temp_cal * AverageVoltage_CalByEnergy_D ;
//	Q.data_lowpassfilter_perframe = temp_cal ;
//	temp_cal = 470  ;
//	Mileage.counter_lowpassfilter_perframe = temp_cal ;
//}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ���ص��Ǹ߾��ȵ���������ֵ
* ����ֵ        : ��
*******************************************************************************/
void LowPassFilteringQPerFrame(void)
{
	uint32_t temp_cal ;
	temp_cal = Q.data_lowpassfilter_perframe ;
	temp_cal = temp_cal * (PerFrame_LowPass_Filter_Cardinal - PerFrame_LowPass_Filter_Factor)\
			+(LV.avr_adc * OLCM.data_current) * PerFrame_LowPass_Filter_Factor ;
	temp_cal = temp_cal/ PerFrame_LowPass_Filter_Cardinal ;
	Q.data_lowpassfilter_perframe = temp_cal ;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      :
* ����ֵ        : ��
*******************************************************************************/
void LowPassFilteringMileagePerFrame(void)
{
	uint32_t temp_cal ;
	temp_cal = Mileage.counter_lowpassfilter_perframe ;
	//temp_cal = temp_cal * 256 ;
	temp_cal = temp_cal * (PerFrame_LowPass_Filter_Cardinal - PerFrame_LowPass_Filter_Factor)\
		+ OLCM.counter_hall_speed.sum * PerFrame_LowPass_Filter_Factor ;
	temp_cal = temp_cal/ PerFrame_LowPass_Filter_Cardinal ;
	Mileage.counter_lowpassfilter_perframe = temp_cal ;
}


/*******************************************************************************
* ������  		:
* ��������    	:�ͷŵĵ����� ��ѹֵ��12bit)����4�����10bit����һ��ͨ���յ��ĵ���ֵ(8bit)
		16bit*8bit ʹ��Ӳ���˷��� ֻ��Ҫ����cpuʱ�伴����ɡ�
		ѹ����ѹֵλ����������32bit�������ṩ������10bit*20bit�������� = 30bit��������
		��Ȼ��4������չ�ռ䡣
		������ֵ��80V ����10bit ���Ծ�ȷ��0.08V �㹻����
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
/*uint32_t CalQReleaseInThisFrame(void)                 
{
	return ((LV.avr_adc >>LV_Accuracy_Reduced_ForQ) * OLCM.data_current );
}*/

uint8_t getGPIOLevel()
{
	return (uint8_t)PIN_getInputValue(Pin_YXT);
}


#if(Commu_Mode_TwoWire == Commu_Mode)
/*******************************************************************************
* ������  		:
* ��������    	: 
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void GetReadControllerConfigCmdReplyMessage(void)
{
	if(True == IsSumCompliant())
	{	
		if((T2C_ReadConfigCmd == OLC.DataBuff.reply_cmd)&&\
			((UserConfigData_Strlen + 1)== OLC.DataBuff.strlen_valid))
			GetReadConfigReply();
		else
			;
	}
	else
		;
}

/*******************************************************************************
* ������		:
* ��������		: 
* �������		: ��
* ������		: ��
* ����ֵ		: ��
*******************************************************************************/
void GetWriteControllerConfigCmdReplyMessage(void)
{
	if(True == IsSumCompliant())
	{
		if((T2C_WriteConfigCmd == OLC.DataBuff.reply_cmd)&&\
			((1 + 1)== OLC.DataBuff.strlen_valid))
			GetWriteConfigReply();
		else
			;
	}
	else
		;
}

/*******************************************************************************
* ������		:
* ��������		: 
* �������		: ��
* ������		: ��
* ����ֵ		: ��
*******************************************************************************/
void GetReadConfigReply(void)
{
	UserConfig.UserConfigStruct = OLC.DataBuff.UserConfig ;
	UserConfigAT.UserConfigStruct = AdjustOrderOfUserConfig(UserConfig.UserConfigStruct) ;
	ResponseReadControllerE2PROMConfigBlock();		
	T2C.cmd = T2C_ReadConfigFinished ;				
}

/*******************************************************************************
* ������		:
* ��������		: 
* �������		: ��
* ������		: ��
* ����ֵ		: ��
*******************************************************************************/
void GetWriteConfigReply(void)
{
	if(T2C_WriteConfigWait == T2C.cmd)
	{
		if(0 == OLC.DataBuff.result_writecfgrply)
			T2C.cmd = T2C_SendCommonDataCmd ;
		else 
			T2C.cmd = T2C_WriteConfigCmd ;
	}
	else
		;
}

#if 0
/*******************************************************************************
* ������  		:
* ��������    	: 0.1s�͵�ƽ��2ms�ߵ�ƽ
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void SendCustomSerialInterfaceCode(PIN_Handle pinHandle)
{
	if(CustomSerialInterface.counter_tojudgelevel <= (uint8_t)(CSI_Strlen_Code*8*2+2-1)){
	  if(CustomSerialInterface.counter_tojudgelevel & U8_BIT0)	//ż��Ϊ�͵�ƽ������Ϊ�ߵ�ƽ
	  {
		  if(True == IsReachRD3ChangeLeveltime(CustomSerialInterface.timer_target[CustomSerialInterface.counter_tojudgelevel]) )
		  {
			    PIN_setOutputValue(pinHandle, Pin_T2C, 0);
				CustomSerialInterface.counter_tojudgelevel ++ ;
		  }
		  else
			  ;
	  }
	  else
	  {
		  if(True == IsReachRD3ChangeLeveltime(CustomSerialInterface.timer_target[CustomSerialInterface.counter_tojudgelevel]) )
		  {
			    PIN_setOutputValue(pinHandle, Pin_T2C, 1);
				CustomSerialInterface.counter_tojudgelevel ++ ;
		  }
		  else
			  ;
	  }
	}

//  if(CustomSerialInterface.counter_tojudgelevel > (uint8_t)(CSI_Strlen_Code*8*2+2-1))//ȷ�����һ����������������
//  {
//	  LoadCSICode();
//	  LoadCSITargetTime();
//  }
}
/**
 * Call this in RTOS task to load to CustomSerialInterface.timer_target array
 */
void PrepareCustomSerialInterfaceCode(void){

  if(CustomSerialInterface.counter_tojudgelevel > (uint8_t)(CSI_Strlen_Code*8*2+2-1))//ȷ�����һ����������������
  {
	  LoadCSICode();
	  LoadCSITargetTime();
  }
}
/**
 * Call this beforing calling PrepareCustomSerialInterfaceCode for the first time
 */
void initialConfigCustomSerialInterfaceCode(void){
	//set counter_tojudgelevel to max out the value so that the PrepareCustomSerialInterfaceCode will load
	//the array for initially
	CustomSerialInterface.counter_tojudgelevel = (uint8_t)(CSI_Strlen_Code*8*2+2-1) + 1;
	CustomSerialInterface.Data2Level.Level.timer_temp = 0;
}
/*******************************************************************************
* ������  		:
* ��������    	: �ж��Ƿ񵽴��л���ƽ��ʱ��� �����������������ҷ����жϽ��
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
FlagStatus IsReachRD3ChangeLeveltime(uint16_t targettimer)
{
	if( CustomSerialInterface.Data2Level.Level.timer_temp >= targettimer)
	{
		CustomSerialInterface.Data2Level.Level.timer_temp = 0 ;
		return True ;
	}
	else
		return False ;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void LoadCSITargetTime(void)
{
	uint8_t temp = 0 ;
	CustomSerialInterface.timer_target[0] = (uint16_t)(100000/100) ;
	CustomSerialInterface.timer_target[1] = (uint16_t)(2000/100) ;	
	for(uint8_t i = 0 ;i< CSI_Strlen_Code ;i++)
	{
		temp = CustomSerialInterface.datacode[i];
		for(uint8_t j = 0 ;j< 8;j++)
		{
			if(temp & U8_BIT7)
			{
				CustomSerialInterface.timer_target[(8*i+j)*2+2] = (uint16_t)(2000/100) ;
				CustomSerialInterface.timer_target[(8*i+j)*2+3] = (uint16_t)(4000/100) ;
			}
			else
			{
				CustomSerialInterface.timer_target[(8*i+j)*2+2] =  (uint16_t)(4000/100) ;
				CustomSerialInterface.timer_target[(8*i+j)*2+3] =  (uint16_t)(2000/100) ;
			}
			temp <<= 1 ;
		}		
	}
	CustomSerialInterface.counter_tojudgelevel = 0 ;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void LoadCSICode(void)
{
	CustomSerialInterface.datacode[0] = CSI_StartCode ;
	CustomSerialInterface.datacode[1] = CSI_Strlen_Status ;	
	CustomSerialInterface.datacode[2] = CalCSIStatusData() ;
	CustomSerialInterface.datacode[3] = CSI_Strlen_Status ^ CustomSerialInterface.datacode[2] ;
	CustomSerialInterface.datacode[4] = CSI_EndCode ;
}

/*******************************************************************************
* ������  		:
* ��������    	: bit 0:����״̬ bit1��˫Ƿѹ״̬��0��45VǷѹ;1:42VǷѹ  ����ѹ����48Vʱ���Զ��ָ���45VǷѹ��
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
uint8_t CalCSIStatusData(void)
{
	uint8_t statusdata  ;
	statusdata = 0x00 ;
	
	//Set GuardStatus
	if(True == CustomSerialInterfaceMessage.status_guard )
		statusdata |=  U8_BIT0 ;
	else
		statusdata &=  ~U8_BIT0 ;
	//statusdata |= TurnFlagStatusToU8(CustomSerialInterfaceMessage.status_guard);
	
	#if(COMPILE_ENABLE == Double_LVSet)
		if(True == IsLVHigherThanLV1RecoverLevel())
			CustomSerialInterfaceMessage.status_LVLevel = Status_HigherLVlevel ;
	#else
		CustomSerialInterfaceMessage.status_LVLevel = Status_LowerLVlevel ;
    #endif
	
	//Set LVLevel
	if(Status_HigherLVlevel == CustomSerialInterfaceMessage.status_LVLevel )
		statusdata &= ~U8_BIT1 ;
	else
		statusdata |= U8_BIT1 ;
	
	if(True == CustomSerialInterfaceMessage.status_onekeyrepair)
	{
		statusdata |= U8_BIT2 ;
		CustomSerialInterfaceMessage.counter_onekeyrepairset_cycle -- ;
		if(0 == CustomSerialInterfaceMessage.counter_onekeyrepairset_cycle )
			CustomSerialInterfaceMessage.status_onekeyrepair = False ;
	}
	else
		statusdata &= ~U8_BIT2 ;

	return statusdata ;
}
#endif
#endif
#endif



