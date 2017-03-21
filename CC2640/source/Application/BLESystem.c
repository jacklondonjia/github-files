//#include "stdio.h"
//#include "_Parameter.h"
//#include "Defines.h"
//#include "globalvariables.h"
//#include "BLESystem.h"
//#include "ATcontrol.h"
//#include "BatteryPowerCal.h"
//#include "Onelinecommunica.h"
//#include "Guard.h"
#include "hal_board.h"
#include "CustomerPara.h"
#include "Board.h"

/*******************Variable**********************/
BLEModuleConnect_TypeDef	BLEMdlCnt ;
/****************************************************/
/**********************function**********************/

#if 0
/********************************************************************************
 *  @brief
 *  
 ********************************************************************************/
void OpenPWMDriver(void)
{
	PWM_Handle      handle;
	PWM_Params      params;
	PWM_Params_init(&params);
	params.period = 20000;             // Period in microseconds
	params.dutyMode = PWM_DUTY_TIME;   // Duty specified in microseconds
	handle = PWM_open(Board_PWM, &params);
	if (handle == NULL) 
	{
		System_printf("PWM did not open");
	}
	// Update PWM to have a duty of 3000 microseconds
	PWM_setDuty(handle, 3000);
}
#endif

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
#if(TESTCODE_BLE_OUTPUT_COMPILE)
void SendResultOfTestCode(void)
{
    //make sure that last all bits have sent
    //有两种方式清除TC位：1.读一次usart_SR 2.写一次uart_DR
    if(SET == USART_GetFlagStatus(USART1,USART_FLAG_TC)) 
        SendDataByDMA(Send_DMAToUsart1.Data,Send_DMAToUsart1.strlen);
}
#endif

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsBuzzerRingEnable(void)
{
    if(AT.Buzzer.counter_targetringnum_mul2)
        return True ;
    else
        return False ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void BuzzerRingWhenEnable(PIN_Handle pinHandle)
{
    if(True == IsBuzzerRingEnable())
    {
        if(AT.Buzzer.timer_addcounter_per2ms >= Timer_AddCounter_Per2ms)	
        {
            AT.Buzzer.timer_addcounter_per2ms = 0 ;
            AT.Buzzer.counter_nowringnum_mul2 ++ ;
            if(AT.Buzzer.counter_nowringnum_mul2 < AT.Buzzer.counter_targetringnum_mul2)
            {
                if(AT.Buzzer.counter_nowringnum_mul2 % 2)
                	PIN_setOutputValue(pinHandle, Pin_BUZZER, 1);
                else
                	PIN_setOutputValue(pinHandle, Pin_BUZZER, 0);
            }
            else
            {
                PIN_setOutputValue(pinHandle, Pin_BUZZER, 0);
                AT.Buzzer.counter_nowringnum_mul2 = 0 ;
                AT.Buzzer.counter_targetringnum_mul2 = 0 ;
            }
        }
        else 
            ;
    }
    else
        ;	
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
UnionSumWithH8L8_TypeDef SwapH8L8(UnionSumWithH8L8_TypeDef In)
{
	UnionSumWithH8L8_TypeDef temp ;
	temp.h8 = In.l8 ;
	temp.l8 = In.h8 ;
	return temp ;
}

/*******************************************************************************
* 函数名  		:
* 函数描述    	: 因为考虑到除了直接True和False判断，还有种情况是中间状态，需要保持之前的状态。
				因此还是需要Guard.status_connectwithcontroller来中转和暂存。
				指定版依靠YXT上的电平是否频繁变化判断  需注意未连接及信号扰动的问题
				通用版依靠判断YXT电平是否一直为高电平判断。
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void UpdateStatusConnectToController(void)
{
#if(Commu_Mode_Common == Commu_Mode)
	if(False == BLEMdlCnt.status_connect2controller)
	{
		if(BLEMdlCnt.timer_connect2controller5V_100ms > TimePeriod_Noconnected2Controller_100ms)   // > 2s
			BLEMdlCnt.status_connect2controller = True ;
	}
	else		
	{
		if(BLEMdlCnt.timer_noconnect2controller5V_100ms > TimePeriod_Noconnected2Controller_100ms)   // > 2s
			BLEMdlCnt.status_connect2controller = False ;
	}
#else
	if(False == BLEMdlCnt.status_connect2controller)
	{
		if(BLEMdlCnt.timer_connect2controller_100ms < TimePeriod_Noconnected2Controller_100ms)   // <4s
			BLEMdlCnt.status_connect2controller = True ;	
	}
	else
	{
		if(BLEMdlCnt.timer_connect2controller_100ms >= TimePeriod_Noconnected2Controller_100ms)   // <4s
			BLEMdlCnt.status_connect2controller = False ;
	
	}
#endif
}

/*********************************************************************
 * @brief  
 *
 * @param   
 *
 * @return  
 *********************************************************************/
void SoftwareReset(void)
{
	HAL_SYSTEM_RESET();
}
