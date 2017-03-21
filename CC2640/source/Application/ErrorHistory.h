#ifndef __ERRORHISTORY_H
#define __ERRORHISTORY_H


#if(Commu_Mode_Common != Commu_Mode)
/****************************************************/
/*********************typedef************************/
#define	RunMessageSingleEntryLen_Max		17
#define	RunMessageSingleEntryLen			8	
#define	RunMessageEntryNum				    5

#define	ErrorHistoryEntryNum_Max			20
#define	ErrorHistoryEntryNum				20




typedef struct
{
	uint8_t len_entries_total ;	//总共有多少条目的信息
	uint8_t len_entries_towrite ;	//本次需要写入多少条目的故障信息
	uint8_t entry[ErrorHistoryEntryNum][RunMessageSingleEntryLen] ;	//具体信息
	uint8_t	status_error ;//上一条故障代码
	uint8_t counter_entry_statuserror_repeat ;	//故障代码一样的条目数（不算第一条的重复的条数，）
}ErrorHistoryMessage_Type;

typedef struct
{
    uint8_t counter ;
    uint8_t entry[RunMessageEntryNum][RunMessageSingleEntryLen] ;
	FlagStatus flag_geaterthan_ErrorHistoryEntryNum ;
}RunMessage_Type ;

/*************************************************/
/*******************Variable**********************/
extern ErrorHistoryMessage_Type ErrorHistoryMessage ;
extern RunMessage_Type RunMessage ;
/****************************************************/
/**********************function**********************/
FlagStatus DeleteErrorHistory(void) ;
void SaveRunMessage(void) ;
uint8_t ReturnSaveErrorHistoryEntryLen(void) ;
void SaveErrorHistoryMessage(void) ;





#endif
#endif