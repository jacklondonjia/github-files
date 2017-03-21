#ifndef __DEFINES_H
#define __DEFINES_H



/**********************define************************/
/*****************************************************/
/******************** BIT define *****************************/
#define U8_BIT0						0x01
#define U8_BIT1						0x02
#define U8_BIT2						0x04
#define U8_BIT3						0x08
#define U8_BIT4						0x10
#define U8_BIT5						0x20
#define U8_BIT6						0x40
#define U8_BIT7						0x80

/******************** Logic define *****************************/
#define	True						SET
#define False						RESET
#define YES	        				SET
#define NO	        				RESET
#define ON							SET
#define	OFF							RESET

typedef enum 
{
	RESET = 0, 
	SET = !RESET
} FlagStatus;

/***************** Nop define **********************/
#define NOP		asm( " nop " )


//#define	uint8_t 					uint8
//#define	uint16_t					uint16
//#define	uint32_t					uint32
/********************const*************/
#define	PI		3.141592654	

/******************** const num*****************************/
#define TEN				10
#define ONE_HUNDRED		100
#define ONE_THOUSAND	1000
#define TEN_THOUSAND	10000

//BitBand  1byte(8bit) expand to 32bit 
//please read <Cortex©\M3 È¨ÍþÖ¸ÄÏ> P91
//---------------------------------------------------------------------------------------------------------------------
#define RAMBitBandVal(addr, bitnum, Val)	(*(volatile uint32_t*)((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF)<<5) + (bitnum<<2)) = Val)
#define SetRAMBitBand(addr, bitnum)			(*(volatile uint32_t*)((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF)<<5) + (bitnum<<2)) = 1)
#define ClrRAMBitBand(addr, bitnum)			(*(volatile uint32_t*)((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF)<<5) + (bitnum<<2)) = 0)
#define XorRAMBitBand(addr, bitnum)			(*(volatile uint32_t*)((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF)<<5) + (bitnum<<2)) ^= 1)
#define GetRAMBitBand(addr, bitnum)			(*(volatile uint32_t*)((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF)<<5) + (bitnum<<2)))


#define	GPIOx_IDR_Offset					0X08
#define	GPIOx_ODR_Offset					0X0C
//---------------------------------------------------------------------------------------------------------------------
//#define PortBitVal(addr, bitnum, Val)		RAMBitBandVal(addr, bitnum, Val)
//#define GetPortBit(GPIOx, bitnum)			GetSFRBitBand(GPIOx ## _IDR_ADR, bitnum)	
//#define SetPortBit(GPIOx, bitnum)			SetSFRBitBand(GPIOx ## _ODR_ADR, bitnum)
//#define ClrPortBit(GPIOx, bitnum)			ClrSFRBitBand(GPIOx ## _ODR_ADR, bitnum)
//#define XorPortBit(GPIOx, bitnum)			XorSFRBitBand(GPIOx ## _ODR_ADR, bitnum)
//
//
////output port
//#define PinSet(PinNamex,Val)				PortBitVal(Port_ ## PinNamex + GPIOx_ODR_Offset, Pin_ ## PinNamex, Val)














#endif