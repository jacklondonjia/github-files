#ifndef __CUSTOMERPARA_H
#define __CUSTOMERPARA_H

//Digital front plus 0b means it is binary
//Digital front plus 0 means it is octonary 
//Digital front plus 0x means it is hexadecimal

//Digital front plus none means it is decimal 


/************Test Customer Files****************/
//单向 G板	48V  二轮车
#define	L000001AA		     10101				//0000010101	

//单向 E板	48V  二轮车		GPRS测试代码
#define	L000001AB		  	10102	

//单向 E板	48V  二轮车		220测试代码
#define	L000001AC		     10103	

//单向 E板	72V  二轮车		电压及其他参数可通过AT配置版本
#define	L000001AD		     10104	

//双向 G板	60V  二轮车		萨博控制器
#define	L000001AE		     10105

//双向 G板	60V  二轮车		config controller paras which are in E2PROM
#define	L000001AF		     10106

//单向 G板	48V  三轮车
#define	L000002AA		     20101	

//单向 RGZ_B	48V  二轮车
#define L001001AA		   10010101

//双向 DLCC01	48V  二轮车		双撑
#define L001002AA		   10020101



/************威科力****************/
#define	L064002AA		  640020101
#define	L064002AB		  640020102
#define	L064002AC		  640020103
#define L064003AA		  640030101

/************雅迪****************/
#define	L242107AA		2421070101


/************爱租车****************/
#define	L501001AA		5010010101


/************金彭邮政三轮车****************/
#define L502102AA		5021020101
#define	L063008AA		 630080101		//金彭 菜鸟网络 临时程序

/************X联盟****************/
#define	L504001AA		5040010101
#define	L504002AA		5040020101
#define	L504003AA		5040030101
#define	L504005AA		5040050101
#define	L504006AA		5040060101
#define	L504007AA		5040070101
#define	L504107AA		5041070101

/************新日邮政三轮车****************/
#define L505101AA		5051010101

/************小飞哥两轮车****************/
#define L506101AA		5061010101
#define L506102AA		5061020101

/************大陆鸽邮政三轮车****************/
#define L507101AA		5071010101

/************猪八戒双撑****************/
#define L508001AA		5080010101


#define	CumstomerNum				L001002AA

#if (L001001AA == CumstomerNum)
	#define FirmwareVersion			0X020201
#else
	#define FirmwareVersion			0X010204
#endif


#include "stdint.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "Defines.h"
#include "Board.h"

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <ICall.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#include "util.h"
#include	"_Parameter.h"
#if(L000001AA == CumstomerNum)			
	#include 	"CustomerL000001AA.h"
#elif(L000001AB == CumstomerNum)			
	#include 	"CustomerL000001AB.h"
#elif(L000001AC == CumstomerNum)			
	#include 	"CustomerL000001AC.h"
#elif(L000001AD == CumstomerNum)			
	#include 	"CustomerL000001AD.h"
#elif(L000001AE == CumstomerNum)			
	#include 	"CustomerL000001AE.h"
#elif(L000001AF == CumstomerNum)			
	#include 	"CustomerL000001AF.h"	

#elif(L001001AA == CumstomerNum)			
	#include 	"CustomerL001001AA.h"			//External_Anti_theft
#elif(L001002AA == CumstomerNum)			
	#include 	"CustomerL001002AA.h"			//ElectricDouSupport

#elif(L063008AA == CumstomerNum)
	#include 	"CustomerL063008AA.h"
#elif(L064002AA == CumstomerNum)			
	#include 	"CustomerL064002AA.h"
#elif(L064002AB == CumstomerNum)			
	#include 	"CustomerL064002AB.h"
#elif(L064002AC == CumstomerNum)			
	#include 	"CustomerL064002AC.h"
#elif(L064003AA == CumstomerNum)			
	#include 	"CustomerL064003AA.h"
#elif(L242107AA == CumstomerNum)
	#include	"CustomerL242107AA.h"	
#elif(L501001AA == CumstomerNum)
	#include	"CustomerL501001AA.h"
#elif(L502102AA == CumstomerNum)
	#include	"CustomerL502102AA.h"
#elif(L504001AA == CumstomerNum)
	#include	"CustomerL504001AA.h"
#elif(L504002AA == CumstomerNum)
	#include	"CustomerL504002AA.h"
#elif(L504003AA == CumstomerNum)
	#include	"CustomerL504003AA.h"
#elif(L504005AA == CumstomerNum)
	#include	"CustomerL504005AA.h"
#elif(L504006AA == CumstomerNum)
	#include	"CustomerL504006AA.h"
#elif(L504007AA == CumstomerNum)
	#include	"CustomerL504007AA.h"
#elif(L504107AA == CumstomerNum)
	#include	"CustomerL504107AA.h"	
#elif(L505101AA == CumstomerNum)
	#include	"CustomerL505101AA.h"
#elif(L506101AA == CumstomerNum)
	#include	"CustomerL506101AA.h"
#elif(L506102AA == CumstomerNum)
	#include	"CustomerL506102AA.h"	
#elif(L507101AA == CumstomerNum)
	#include	"CustomerL507101AA.h"
#elif(L508001AA == CumstomerNum)
	#include	"CustomerL508001AA.h"	
#else
	#error	"没有载入合适的客户档案.h文件"
#endif	

#include "_ParameterCal.h"


#include "globalvariables.h"
#include "FlashSNV.h"
#include "TypeDefines.h"
#include "DebugTest.h"
#include "BLESystem.h"
#include "ATcontrol.h"
#include "BatteryPowerCal.h"
#include "Guard.h"
#include "Onelinecommunica.h"
#include "sendandstring.h"
#include "MileageCal.h"
#include "ErrorHistory.h"
#include "FreeKeyAndMechaLock.h"
#include "T2C.h"
#include "GPScontrol.h"

//#include "FlashSNV.h"

#include "PeriodicTimer.h"
#include "Tool_Functions.h"









#endif
