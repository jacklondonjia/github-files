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
* 函数名        :
* 函数描述    	: string 末尾收到 \r \n 时 判断为AT命令结尾
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
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
* 函数名        :
* 函数描述    	: string 末尾收到 > 空格时 判断为AT命令结尾
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
FlagStatus GetContentLogo(uint8_t arr[],uint8_t counter)
{ 
    if(' ' ==arr[counter]  &&  '>' == arr[counter-1] )
        return SET ;
    else
        return RESET ;
    
}

/*******************************************************************************
* 函数名        :
* 函数描述    	: 发送ATCmd的response（不带数据，只有单个代号）
* 输入参数      : 代号
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
/*void SendATCmdResponseCode(uint8_t code)
{
#if TESTCODE_BLE_CONNECTREPEAT_COMPILE
    HC08.counter_testconnectrepeat_send ++ ;
#endif
    DMA_Cmd(DMA1_Channel4, DISABLE);                    //关闭DMA,防止处理其间有数据
    USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
    
    //填充将要发送的数据
    Send_DMAToUsart1.Buff[0] =code;
    //串口向 DMA发出请求
    DMA_SetCurrDataCounter (DMA1_Channel4,1);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);	//先配置usart为dma传输
    DMA_Cmd (DMA1_Channel4,ENABLE);			//然后再启动dma传输
}*/

/*******************************************************************************
* 函数名        :
* 函数描述    	: 和固定的字符串做比较
* 输入参数      : 目标字符串地址，参照字符串地址，参照字符串长度
* 输出结果      : 一致就输出True，否则输出False
* 返回值        : None
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
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void CopyStr(uint8_t sourceArr[],uint8_t targetArr[],uint16_t strlen)
{
    for(uint8_t i =0 ;i < strlen ;i++)
    {
        targetArr[i] = sourceArr[i] ;
    }
}

/*******************************************************************************
* 函数名        :
* 函数描述    	: 把目标字符串全部清零
* 输入参数      : 目标字符串地址，目标字符串长度
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void ClearArray(uint8_t arr[],uint8_t strlen)
{ 
    for (uint8_t i = 0 ; i< strlen ; i++)
        arr[i] = 0 ;
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
FlagStatus IsACharacterANum(uint8_t value_char)
{
	if((value_char >= ASCII_Num_0) && (value_char <= ASCII_Num_9) )
		return True ;
	else
		return False ;

}
/*******************************************************************************
* 函数名        :
* 函数描述    	: 必须配合IsACharacterNum（）使用
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
uint8_t ConvertACharacterIntoANum(uint8_t value_char)
{ 	
 	return (value_char - ASCII_Num_0);
}

/*******************************************************************************
* 函数名        :
* 函数描述    	: 必须配合IsACharacterNum（）使用
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
uint8_t Convert2CharacterInto2Num(uint8_t value_char_h,uint8_t value_char_l)
{
	uint8_t char2num_h = ConvertACharacterIntoANum(value_char_h) ;
	uint8_t char2num_l = ConvertACharacterIntoANum(value_char_l) ;
	return (char2num_h * 10 + char2num_l ) ;
}


/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
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
