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
* 函数名        :
* 函数描述    	:接收到App发来的删除命令，则删除原有的故障历史信息
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
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
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void SaveRunMessage(void) 
{
	for(uint8_t i = 0; i< 4 ; i++)
		RunMessage.entry[RunMessage.counter][i] = OLCM.rawdata[i] ;
	RunMessage.entry[RunMessage.counter][4] = OLCM.counter_hall_speed.h8 ;
	RunMessage.entry[RunMessage.counter][5] = OLCM.counter_hall_speed.l8 ; //霍尔速度
	RunMessage.entry[RunMessage.counter][6] = (uint8_t)(LV.avr_adc >> 8 ) ;	//
	RunMessage.entry[RunMessage.counter][7] = (uint8_t)(LV.avr_adc & 0X00FF) ;	//电压值	


	
#if(7 >= RunMessageSingleEntryLen)
    #error "RunMessage.entry[][]第二个括号里面的长度超过设定值"
#endif    
	
    RunMessage.counter ++ ;
	if(RunMessage.counter >= RunMessageEntryNum)
	{
		RunMessage.counter = 0 ;
		RunMessage.flag_geaterthan_ErrorHistoryEntryNum = True ;
	}
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
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
					ErrorHistoryMessage.counter_entry_statuserror_repeat ++ ;	//累计重复故障信息
				return 0 ;
			}
		}
		else
		{
			ErrorHistoryMessage.counter_entry_statuserror_repeat = 0 ;	//原本没有故障，第1次故障时该变量清零 表示还没重复
			ErrorHistoryMessage.status_error = status ;	
			if(True == RunMessage.flag_geaterthan_ErrorHistoryEntryNum)
				return RunMessageEntryNum ;
			else
				return RunMessage.counter ;
		}
	}
	else
	{
		ErrorHistoryMessage.counter_entry_statuserror_repeat = 0 ;		//没有故障时清零
		ErrorHistoryMessage.status_error = 0 ;
		return 0 ;
	}
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:当出现故障的信息，把故障前的运行状态排序
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
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
		
			if(0 != len )	//还有数据没有转移完
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
	else	// 故障历史条目数超出最大值时 不再写入
		;
}


#endif
