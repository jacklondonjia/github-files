#ifndef __GLOBALVARIABLES_H
#define __GLOBALVARIABLES_H
#include "stdint.h"
/*****************************************************/
/*********************typedef***********************/
/**************************************
*数据字节以小端格式存放在存储器中。
*一个字里的最低地址字节被认为是该字的最低有效字节，而最高地址字节是最高有效字节
******************************************/
typedef union
{
    uint16_t sum;
    struct
    {
	uint8_t l8;
	uint8_t h8;	
    };
}UnionSumWithH8L8_TypeDef;


typedef union
{
    uint32_t sum32;
    struct
    {
		uint16_t l16;	
		uint16_t h16;
    }StructH16L16;
	uint8_t bit8[4] ;
}Union4Bytes_TypeDef;

typedef enum
{ 
    Low_Level = 0 , 
    High_Level 
}LevelStatus_TypeDef;

typedef struct
{ 
    LevelStatus_TypeDef	status ;
    uint8_t 		timer_newlevel_per2ms  ; 
}SwitchLevel_TypeDef;

typedef struct
{ 
    LevelStatus_TypeDef	status ;
	LevelStatus_TypeDef	status_temp ;
    volatile uint16_t timer_temp  ;
    uint16_t 		timer_highlevel ;
    uint16_t 		timer_lowlevel ; 
}CommunicaLevel_TypeDef;


typedef enum
{ 
    KEY_Release = 0,
    KEY_Press 
}KeyAction_TypeDef;

/*************************************************/
/*******************Variable**********************/
/****************timer in systemstick*************/
extern volatile uint8_t timer_2ms_per100us  ;
extern uint8_t timer_500ms_per2ms ;
//extern volatile uint8_t timer_100us  ;
//extern volatile uint16_t timer_200msForBLE_per100us  ;
extern volatile uint16_t timer_100ms_per100us ;
extern volatile uint16_t timer_1s_per100us  ;
//extern volatile uint8_t timer_senddata2BLE_10ms  ;
//extern uint8_t timer_1s  ;
extern uint8_t timer_30s_p1s;
//extern uint16_t temp1 ;
//extern uint8_t temp2 ;
//extern uint8_t temp3 ;
//extern uint8_t temp4 ;
//extern uint8_t temp5 ;

/*************************************************/
/*******************function**********************/
/*******************main**************************/




#endif
