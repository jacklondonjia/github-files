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
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
#if(TESTCODE_BLE_OUTPUT_COMPILE)
void SendResultOfTestCode(void)
{
    //make sure that last all bits have sent
    //�����ַ�ʽ���TCλ��1.��һ��usart_SR 2.дһ��uart_DR
    if(SET == USART_GetFlagStatus(USART1,USART_FLAG_TC)) 
        SendDataByDMA(Send_DMAToUsart1.Data,Send_DMAToUsart1.strlen);
}
#endif

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
FlagStatus IsBuzzerRingEnable(void)
{
    if(AT.Buzzer.counter_targetringnum_mul2)
        return True ;
    else
        return False ;
}

/*******************************************************************************
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
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
* ������  		:
* ��������    	:
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
UnionSumWithH8L8_TypeDef SwapH8L8(UnionSumWithH8L8_TypeDef In)
{
	UnionSumWithH8L8_TypeDef temp ;
	temp.h8 = In.l8 ;
	temp.l8 = In.h8 ;
	return temp ;
}

/*******************************************************************************
* ������  		:
* ��������    	: ��Ϊ���ǵ�����ֱ��True��False�жϣ�������������м�״̬����Ҫ����֮ǰ��״̬��
				��˻�����ҪGuard.status_connectwithcontroller����ת���ݴ档
				ָ��������YXT�ϵĵ�ƽ�Ƿ�Ƶ���仯�ж�  ��ע��δ���Ӽ��ź��Ŷ�������
				ͨ�ð������ж�YXT��ƽ�Ƿ�һֱΪ�ߵ�ƽ�жϡ�
* �������      : ��
* ������      : ��
* ����ֵ        : ��
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
