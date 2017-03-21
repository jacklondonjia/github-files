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
#include "CustomerPara.h"

#include "bcomdef.h"
//FlagStatus DMA_FirstTime = RESET ;

/*****************************************/
/*******************Variable*************/
/******************string send and receive*********************/
uint8_t counter_send=0; 
uint8_t counter_receive=0; 
uint8_t PCSend[BufferSize]={0};
uint8_t PCReceive[BufferSize]={0};
volatile FlagStatus flag_RXNE	= RESET ;

Send_DMAToUsart1_s Send_DMAToUsart1 ;

static sendNotifyMessageCBs_t *pAppCBs = NULL;


/***********************************************/
/*****************function********************/

/*******************************************************************************
* ������        :
* ��������    	: string ĩβ�յ� \r \n ʱ �ж�ΪAT�����β
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
FlagStatus GetStringEndFlag(uint8_t arr[],uint8_t strlen)
{ 
    //  if(ASCII_LF ==arr[counter]  &&  ASCII_CR == arr[counter-1] )
    if('\n' ==arr[strlen-1]  &&  '\r' == arr[strlen-2] )
        return SET ;
    else
        return RESET ;
    
}

/*******************************************************************************
* ������        :
* ��������    	: string ĩβ�յ� > �ո�ʱ �ж�ΪAT�����β
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
FlagStatus GetContentLogo(uint8_t arr[],uint8_t counter)
{ 
    if(' ' ==arr[counter]  &&  '>' == arr[counter-1] )
        return SET ;
    else
        return RESET ;
    
}

/*******************************************************************************
* ������        :
* ��������    	: ����ATCmd��response���������ݣ�ֻ�е������ţ�
* �������      : ����
* ������      : None
* ����ֵ        : None
*******************************************************************************/
/*void SendATCmdResponseCode(uint8_t code)
{
#if TESTCODE_BLE_CONNECTREPEAT_COMPILE
    HC08.counter_testconnectrepeat_send ++ ;
#endif
    DMA_Cmd(DMA1_Channel4, DISABLE);                    //�ر�DMA,��ֹ�������������
    USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
    
    //��佫Ҫ���͵�����
    Send_DMAToUsart1.Buff[0] =code;
    //������ DMA��������
    DMA_SetCurrDataCounter (DMA1_Channel4,1);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);	//������usartΪdma����
    DMA_Cmd (DMA1_Channel4,ENABLE);			//Ȼ��������dma����
}*/

/*******************************************************************************
* ������        :
* ��������    	: �͹̶����ַ������Ƚ�
* �������      : Ŀ���ַ�����ַ�������ַ�����ַ�������ַ�������
* ������      : һ�¾����True���������False
* ����ֵ        : None
*******************************************************************************/
FlagStatus CmpStrWithConstStr(uint8_t receiveArr[],const uint8_t standardArr[],uint8_t strlen_standard)
{
    for(uint8_t i =0 ;i<strlen_standard;i++)
    {
        if(receiveArr[i] != standardArr[i])
            return False;
        else
            ;
    }
    return True ;
}

/*******************************************************************************
* ������        :
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void CopyStr(uint8_t sourceArr[],uint8_t targetArr[],uint16_t strlen)
{
    for(uint8_t i =0 ;i < strlen ;i++)
    {
        targetArr[i] = sourceArr[i] ;
    }
}

/*******************************************************************************
* ������        :
* ��������    	: ��Ŀ���ַ���ȫ������
* �������      : Ŀ���ַ�����ַ��Ŀ���ַ�������
* ������      : None
* ����ֵ        : None
*******************************************************************************/
void ClearArray(uint8_t arr[],uint8_t strlen)
{ 
    for (uint8_t i = 0 ; i< strlen ; i++)
        arr[i] = 0 ;
}

/*******************************************************************************
* ������        :
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
FlagStatus IsACharacterANum(uint8_t value_char)
{
	if((value_char >= ASCII_Num_0) && (value_char <= ASCII_Num_9) )
		return True ;
	else
		return False ;

}
/*******************************************************************************
* ������        :
* ��������    	: �������IsACharacterNum����ʹ��
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
uint8_t ConvertACharacterIntoANum(uint8_t value_char)
{ 	
 	return (value_char - ASCII_Num_0);
}

/*******************************************************************************
* ������        :
* ��������    	: �������IsACharacterNum����ʹ��
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
uint8_t Convert2CharacterInto2Num(uint8_t value_char_h,uint8_t value_char_l)
{
	uint8_t char2num_h = ConvertACharacterIntoANum(value_char_h) ;
	uint8_t char2num_l = ConvertACharacterIntoANum(value_char_l) ;
	return (char2num_h * 10 + char2num_l ) ;
}


/*******************************************************************************
* ������        :
* ��������    	:
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
uint8_t ConvertHexIntoACharacter(uint8_t value_hex)
{ 
    if(value_hex < 10)
        return (value_hex + ASCII_Num_0 ) ;
    else if(value_hex >= 10 && value_hex <16)
        return (value_hex + ASCII_A) ;
    else
        return 0;
}
/*
 * SendNotifyMessage_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
bStatus_t SendNotifyMessage_RegisterAppCBs( sendNotifyMessageCBs_t *appCallbacks )
{
	  if ( appCallbacks )
	  {
	    pAppCBs = appCallbacks;

	    return ( SUCCESS );
	  }
	  else
	  {
	    return ( bleAlreadyInRequestedMode );
	  }
}

void SendDataByBLE(const ArrInfo arrInfo)
{
    if ( pAppCBs && pAppCBs->pfnNotifyCb)
      pAppCBs->pfnNotifyCb(arrInfo); // Call app function from stack task context.
}
