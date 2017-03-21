//#include "stdint.h"
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
//#include "ErrorHistory.h"
#include "CustomerPara.h"

#if(Commu_Mode_Common != Commu_Mode)
/*************************************************/
/*******************Variable**********************/
ErrorHistoryMessage_Type ErrorHistoryMessage ;
RunMessage_Type RunMessage ;

/****************************************************/
/**********************function**********************/
/*******************************************************************************
* ������        :
* ��������    	:���յ�App������ɾ�������ɾ��ԭ�еĹ�����ʷ��Ϣ
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
FlagStatus DeleteErrorHistory(void) 
{
	
	for(uint8_t i= 0 ; i < ErrorHistoryEntryNum; i ++ )
		for(uint8_t j= 0 ; j < RunMessageSingleEntryLen ; j ++ )
			ErrorHistoryMessage.entry[i][j] = 0X00 ;
	ErrorHistoryMessage.len_entries_total = 0;
	
	uint8_t sum = 0 ;
	sum = ErrorHistoryMessage.len_entries_total ;
	for(uint8_t i= 0 ; i < ErrorHistoryEntryNum; i ++ )
		for(uint8_t j= 0 ; j < RunMessageSingleEntryLen; j ++ )
			sum += ErrorHistoryMessage.entry[i][j] ;
	
	if(0 != sum)
		return False ;
	else
		return True ;
}
/*******************************************************************************
* ������        :
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void SaveRunMessage(void) 
{
	for(uint8_t i = 0; i< 4 ; i++)
		RunMessage.entry[RunMessage.counter][i] = OLCM.rawdata[i] ;
	RunMessage.entry[RunMessage.counter][4] = OLCM.counter_hall_speed.h8 ;
	RunMessage.entry[RunMessage.counter][5] = OLCM.counter_hall_speed.l8 ; //�����ٶ�
	RunMessage.entry[RunMessage.counter][6] = (uint8_t)(LV.avr_adc >> 8 ) ;	//
	RunMessage.entry[RunMessage.counter][7] = (uint8_t)(LV.avr_adc & 0X00FF) ;	//��ѹֵ	


	
#if(7 >= RunMessageSingleEntryLen)
    #error "RunMessage.entry[][]�ڶ�����������ĳ��ȳ����趨ֵ"
#endif    
	
    RunMessage.counter ++ ;
	if(RunMessage.counter >= RunMessageEntryNum)
	{
		RunMessage.counter = 0 ;
		RunMessage.flag_geaterthan_ErrorHistoryEntryNum = True ;
	}
}

/*******************************************************************************
* ������        :
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
uint8_t ReturnSaveErrorHistoryEntryLen(void) 
{
	uint8_t len ;
	uint8_t status ;
	
	status = GetErrorStatus() ;	
	if(0 != status)
	{
		if(0 != ErrorHistoryMessage.status_error )
		{
			if(ErrorHistoryMessage.status_error != status)
			{
				if(ErrorHistoryMessage.counter_entry_statuserror_repeat >= (ErrorHistoryEntryNum - 1))
					len = ErrorHistoryEntryNum ;
				else
					len = ErrorHistoryMessage.counter_entry_statuserror_repeat + 1;
				
				ErrorHistoryMessage.counter_entry_statuserror_repeat = 0 ;
				ErrorHistoryMessage.status_error = status ;
				return len ;		
			}
			else
			{
				if(ErrorHistoryMessage.counter_entry_statuserror_repeat < UINT8_MAX)
					ErrorHistoryMessage.counter_entry_statuserror_repeat ++ ;	//�ۼ��ظ�������Ϣ
				return 0 ;
			}
		}
		else
		{
			ErrorHistoryMessage.counter_entry_statuserror_repeat = 0 ;	//ԭ��û�й��ϣ���1�ι���ʱ�ñ������� ��ʾ��û�ظ�
			ErrorHistoryMessage.status_error = status ;	
			if(True == RunMessage.flag_geaterthan_ErrorHistoryEntryNum)
				return RunMessageEntryNum ;
			else
				return RunMessage.counter ;
		}
	}
	else
	{
		ErrorHistoryMessage.counter_entry_statuserror_repeat = 0 ;		//û�й���ʱ����
		ErrorHistoryMessage.status_error = 0 ;
		return 0 ;
	}
}

/*******************************************************************************
* ������        :
* ��������    	:�����ֹ��ϵ���Ϣ���ѹ���ǰ������״̬����
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void SaveErrorHistoryMessage(void) 
{	
	if((ErrorHistoryMessage.len_entries_total + ErrorHistoryMessage.len_entries_towrite) <= ErrorHistoryEntryNum)
	{
		uint8_t len = ErrorHistoryMessage.len_entries_towrite ;
		
		if(True == RunMessage.flag_geaterthan_ErrorHistoryEntryNum)
		{	
			//for(uint8_t i = 0; (i < ErrorHistoryEntryNum - (RunMessage.counter + 1)) && (len > 0) ; i ++)
			for(uint8_t i = 0; (i < RunMessageEntryNum - RunMessage.counter) && (len > 0) ; i ++)
		
			{
				for(uint8_t j = 0 ; j <  RunMessageSingleEntryLen; j ++)
					ErrorHistoryMessage.entry[ErrorHistoryMessage.len_entries_total + i][j] \
							= RunMessage.entry[RunMessage.counter  + i][j] ;
				len -- ;
			}
		
			if(0 != len )	//��������û��ת����
			{
				for(uint8_t i = 0 ; (i < RunMessage.counter) &&(len > 0) ; i ++)
				//for(uint8_t i = 0 ; (i < (RunMessage.counter + 1)) &&(len > 0) ; i ++)
				{
					for(uint8_t j = 0 ; j <  RunMessageSingleEntryLen; j ++)
						ErrorHistoryMessage.entry[ErrorHistoryMessage.len_entries_total + RunMessageEntryNum\
							- RunMessage.counter + i][j] = RunMessage.entry[i][j] ;
					len -- ;
				}
			
			}
		}
		else
		{		
			for(uint8_t i = 0 ; (i < RunMessage.counter) ; i ++)
				for(uint8_t j = 0 ; j <  RunMessageSingleEntryLen; j ++)
					ErrorHistoryMessage.entry[i][j] = RunMessage.entry[i][j] ;
		
		}
		ErrorHistoryMessage.len_entries_total += ErrorHistoryMessage.len_entries_towrite ;
		ErrorHistoryMessage.len_entries_towrite = 0 ;			
	}
	else	// ������ʷ��Ŀ���������ֵʱ ����д��
		;
}


#endif
