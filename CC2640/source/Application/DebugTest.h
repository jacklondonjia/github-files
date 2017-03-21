#ifndef __DEBUGTEST_H
#define __DEBUGTEST_H


/*****************************************************/
/*********************typedef************************/



/*****************************************************/
/**********************define*************************/
#define			Test_Pin_Output_Enable			0
#define			Test_Pin_Input_Enable			0
#define			Test_Pin_Function_Enable		0

#if Test_Pin_Output_Enable
	#define		Test_Pin_T2C			1	
	#define		Test_Pin_DCJD			1
	#define		Test_Pin_MotorD1		1
	#define		Test_Pin_MotorD2		1
	#define		Test_Pin_MotorA2		1
	#define		Test_Pin_MotorA1		1
	#define		Test_Pin_9VSW			1	
	#define		Test_Pin_CPMOS			1
	#define		Test_Pin_BUZZER			1
	#define		Test_Pin_GuardOutput	1
#endif

#if Test_Pin_Input_Enable
	#define		Test_Pin_YXT			1
	#define		Test_Pin_SCJC			1
	#define		Test_Pin_LD				1
	#define		Test_Pin_GuardInput		1
	#define		Test_Pin_OCJ			1
#endif

#if Test_Pin_Function_Enable
	#define		Test_Pin_UARTRX			0
	#define		Test_Pin_UARTTX			0
	#define		Test_Pin_LV				0
	#define		Test_Pin_LV48VSW		0
#endif
                                 
/****************************************************/
/**********************Variable**********************/				
extern volatile uint8_t test1 ;
extern volatile unsigned int test2 ;
extern volatile unsigned int test3 ;
                                  
                                  
                                  


/****************************************************/
/************************function********************/

#endif