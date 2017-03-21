#ifndef __SENDANDSTRING_H
#define __SENDANDSTRING_H


#include "bcomdef.h"
/*****************************************************/
/*********************typedef************************/
typedef struct
{	
	uint8_t Data[SENDBUFF_SIZE];
	uint8_t Buff[SENDBUFF_SIZE];
	volatile uint8_t timer_txidle_per100us ;
	volatile FlagStatus flag_txsendcomplete ;
#if(TESTCODE_BLE_OUTPUT_COMPILE )	
	uint8_t strlen ;
#endif
	
}Send_DMAToUsart1_s;

/**
 * Send notify message Call back definition
 */
typedef void (*sendNotifyMessage_t)( const ArrInfo arrInfo );

typedef struct
{
	sendNotifyMessage_t        pfnNotifyCb;  // Called when characteristic value changes
} sendNotifyMessageCBs_t;


/*****************************************************/
/**********************define************************/
/**********************uart pin define************************/
#define		Pin_Uart1_TX					GPIO_Pin_9
#define		Port_Uart1_TX					GPIOA
#define		Pin_Uart1_RX					GPIO_Pin_10
#define		Port_Uart1_RX					GPIOA
#define		Pin_Uart2_TX					GPIO_Pin_2
#define		Port_Uart2_TX					GPIOA
#define		Pin_Uart2_RX					GPIO_Pin_3
#define		Port_Uart2_RX					GPIOA

/********************* ASCII define ************************/
#define     ASCII_CR						0X0d
#define     ASCII_LF						0X0a
#define		ASCII_QuestionMark				0X3F	
#define		ASCII_Num_0						0X30
#define		ASCII_Num_1						0X31
#define		ASCII_Num_9						0X39
#define     ASCII_A							0X41
#define		ASCII_D							0X44
#define     ASCII_T							0X54
#define     ASCII_O							0X4f
#define     ASCII_K							0X4b




/******************timer*************************/
#define   TimePeriodSendCyclic_Per2ms                         250
#define	  TimeTXIdle_Per100us								230//12 //9600波特率下 空闲帧为11个 转换成100us的精度 就是11.46
//extern FlagStatus DMA_FirstTime	;
//extern volatile  uint16_t counter_SIM_PKEY ;
//extern FlagStatus  Status_SIM_PKEY ;



/*****************************************/
/*******************Variable*************/
/******************string send and receive*********************/
extern uint8_t counter_send; 
extern uint8_t counter_receive; 
extern uint8_t PCSend[BufferSize];
extern uint8_t PCReceive[BufferSize];
extern volatile FlagStatus flag_RXNE ;

extern Send_DMAToUsart1_s Send_DMAToUsart1 ;

/*****************************************/
/*****************function********************/
FlagStatus GetStringEndFlag(uint8_t *,uint8_t ) ;
FlagStatus GetContentLogo(uint8_t *,uint8_t );

void SendDataByBLE(ArrInfo arrInfo);
FlagStatus CmpStrWithConstStr(uint8_t *,const uint8_t *,uint8_t ); //参照数组的长度都在255以下
void CopyStr(uint8_t *,uint8_t *,uint16_t );
void ClearArray(uint8_t *,uint8_t ) ;
FlagStatus IsACharacterANum(uint8_t ) ;
uint8_t ConvertACharacterIntoANum(uint8_t ) ;
uint8_t Convert2CharacterInto2Num(uint8_t ,uint8_t) ;
uint8_t ConvertHexIntoACharacter(uint8_t ) ;
extern bStatus_t SendNotifyMessage_RegisterAppCBs( sendNotifyMessageCBs_t *appCallbacks );
/*******************variable*************/
//extern u8 Contrast_String_ATOK[]={"AT\r\n\r\nOK"};
//char Contrast_String_ATOK[8]={'A','T','\r','\n','\r','\n','O','K'};
//extern u8 Contrast_String_OK[BufferSize]={""};
//extern u8 Contrast_String_Error[BufferSize]={0};
//extern u8 Contrast_String_Ready[BufferSize]={0};


#endif
