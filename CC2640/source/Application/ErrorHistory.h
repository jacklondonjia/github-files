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
	uint8_t len_entries_total ;	//�ܹ��ж�����Ŀ����Ϣ
	uint8_t len_entries_towrite ;	//������Ҫд�������Ŀ�Ĺ�����Ϣ
	uint8_t entry[ErrorHistoryEntryNum][RunMessageSingleEntryLen] ;	//������Ϣ
	uint8_t	status_error ;//��һ�����ϴ���
	uint8_t counter_entry_statuserror_repeat ;	//���ϴ���һ������Ŀ���������һ�����ظ�����������
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