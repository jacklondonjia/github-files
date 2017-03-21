#include "CustomerPara.h"



/*************************************************/
/*********************Define**********************/
#define UART0_READ_EVT        		0x0001

#define MAX_NUM_RX_BYTES      		20   // Maximum RX bytes to receive in one go
#define MAX_NUM_TX_BYTES      		20   // Maximum TX bytes to send in one go
/*******************Variable**********************/
// Entity ID globally used to check for source and/or destination of messages
// 线程注册到ICALL时分配的ID
//static ICall_EntityID selfEntity;

// Semaphore globally used to post events to the application thread
// 信号量，app线程没有事情处理时是阻塞状态，发信号量给app线程唤醒线程
//static ICall_Semaphore sem;

// events flag for internal application events.
// app事件标志变量
//static uint16_t events;

/* Task data */
//Task_Struct uart0Task;
//char uart0TaskStack[UART0_TASK_STACK_SIZE];

static UART_Handle  uart0_handle;
static UART_Params  uart0_params;

#define 		WantedRxBytes 			20            // Number of bytes received so far
static uint8_t   rxBuf[MAX_NUM_RX_BYTES];  // Receive buffer
static volatile uint8_t   txBuf[MAX_NUM_TX_BYTES];  // Transmit buffer
static uint8_t   size_g;
	
///*********************************************************************
//* @brief   
//* @attention :	readMode and writeMode default is UART_MODE_BLOCKING
//				readTimeout and writeTimeout default is UART_WAIT_FOREVER
//* @param   
//*
//* @return  
//**********************************************************************/
//UART_Handle pGetUart0Handle(void)
//{
//	if(!uart0_handle)
//  	{
//		UART_Params_init(&uart0_params);
//		uart0_params.baudRate = 9600;
//		uart0_params.writeDataMode = UART_DATA_BINARY;
//		uart0_params.readDataMode = UART_DATA_BINARY;
//		uart0_params.readReturnMode = UART_RETURN_FULL;
//		uart0_params.readEcho = UART_ECHO_OFF;
//
//		uart0_handle = UART_open(Board_UART, &uart0_params);
//		// Enable RETURN_PARTIAL
//		//	UART_control(uart0_handle, UARTCC26XX_CMD_RETURN_PARTIAL_ENABLE, NULL);
//	}
//  	return uart0_handle;
//}

/*********************************************************************
* @brief   
*
* @param   
*
* @return  
**********************************************************************/
/*void WriteUart0(UART_Handle handle,uint8_t *buffer, size_t size)
{
	UART_write(handle,buffer, size);
}
*/


/*********************************************************************
* @brief   
*
* @param   
*
* @return  
**********************************************************************/
/*void ReadUart0(UART_Handle handle,uint8_t *buffer, size_t size)
{
	// Begin read
	int rxBytes = 0 ;
	rxBytes = UART_read(handle, rxBuf, 10);
	test1 = rxBytes ;
}
*/	
/*********************************************************************
* @brief   
*
* @param    equal interruput
*
* @return  
**********************************************************************/
void SerialAppreadCallback(UART_Handle handle, void *rxBuf1, size_t size)
{
/*	// set event
	events |= UART0_READ_EVT;
	
	// Copy bytes from RX buffer to TX buffer
	for(size_t i = 0; i < size; i++)
		txBuf[i] = ((uint8_t*)rxBuf)[i];
	
	size_g = size;
	
	// Wake up the application.
	Semaphore_post(sem);
	
	// Start another read, with size the same as it was during first call to
	// UART_read()
	UART_read(handle, rxBuf, size);*/
	//UART_write(uart0_handle, rxBuf1, size);
	   
	// Copy bytes from RX buffer to TX buffer
    for(size_t i = 0; i < size; i++)
        txBuf[i] = ((uint8_t*)rxBuf1)[i];
	test1 = size ;

    //txBuf[1] = 0;
  	UART_read(uart0_handle, rxBuf, WantedRxBytes);
}

/*********************************************************************
* @brief   
*
* @param   
*
* @return  
**********************************************************************/
void Uart_init(void)
{
	// ******************************************************************
	// N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
	// ******************************************************************
	// Register the current thread as an ICall dispatcher application
	// so that the application can send and receive messages.
	
	// 将线程注册到ICALL，注册了才能收发消息
	//ICall_registerApp(&selfEntity, &sem);
	
	// Create an RTOS queue for message from profile to be sent to app.
	// 创建消息队列
	// appMsgQueue = Util_constructQueue(&appMsg);
	
	//UART_Params      params;
	
	// Init UART and specify non-default parameters
	UART_Params_init(&uart0_params);
	uart0_params.baudRate      = 	9600//115200;
	uart0_params.writeDataMode = 	UART_DATA_BINARY;
	uart0_params.readDataMode  = 	UART_DATA_BINARY;
	uart0_params.readReturnMode = 	UART_RETURN_FULL;//+
	uart0_params.readMode      = 	UART_MODE_CALLBACK;
	uart0_params.readEcho = 		UART_ECHO_OFF;
	uart0_params.readCallback  = 	SerialAppreadCallback;
	
	// Open the UART and do the read
	uart0_handle = UART_open(Board_UART, &uart0_params);
	// Enable RETURN_PARTIAL
	UART_control(uart0_handle, UARTCC26XX_RETURN_PARTIAL_ENABLE, NULL);
	// UART_read()
	UART_read(uart0_handle, rxBuf, WantedRxBytes);
}



/*********************************************************************
* @brief   
*
* @param   
*
* @return  
**********************************************************************/
/*void Uart0_taskFxn(UArg a0, UArg a1)
{
	// Initialize application
	Uart_init();
	events &= ~UART0_READ_EVT;   //clear event 
	
	// Application main loop
	for (;;)
	{
		// Waits for a signal to the semaphore associated with the calling thread.
		// Note that the semaphore associated with a thread is signaled when a
		// message is queued to the message receive queue of the thread or when
		// ICall_signal() function is called onto the semaphore.
		// 等待信号量，若没收到信号量则线程阻塞，不会执行后续的代码
		ICall_Errno errno = ICall_wait(ICALL_TIMEOUT_FOREVER);
		
		if (errno == ICALL_ERRNO_SUCCESS)   // 收到信号量
		{
			;
		}
		
		if (events & UART0_READ_EVT)
		{
			events &= ~UART0_READ_EVT;   //clear event
			
			// Echo the bytes received back to transmitter
			//WriteUart0(uart0_handle, txBuf, size_g);
			UART_write(uart0_handle,txBuf, size_g);
		}
	}	
}
*/
/*********************************************************************
* @brief   
*
* @param   
*
* @return  
**********************************************************************/
/*void Uart0_createTask(void)
{
	Task_Params taskParams;
	
	Task_Params_init(&taskParams);
  	taskParams.stack = uart0TaskStack;
 	taskParams.stackSize = UART0_TASK_STACK_SIZE;
  	taskParams.priority = UART0_TASK_PRIORITY;
  	Task_construct(&uart0Task, Uart0_taskFxn, &taskParams, NULL);
}
*/



#if 0
/*************************************************/
/*******************Variable**********************/
GPS_Typedef GPS ;
DMA_InitTypeDef DMA_InitStructureTX2;



#if GPS_COMPILE
/****************************************************/
/**********************function**********************/
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ResponseTX2Test(void)
{
	uint8_t arr[2];
    arr[0] = 0X01 ;
	arr[1] = 0X02 ;  
    //WriteToTX2Buff(arr,2);
	FillTX2BuffWithData(GetFinalTX2Buff(),arr,2) ;
}

/*******************************************************************************
* 函数名        :
* 函数描述    	:
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void GPSControl(void)
{
	
//    if(False == AT.status_BLEconnect)
//    {
//        if(True == Reveive_Usart2ToDMA.flag_newdata)
//        {
//         #if TESTCODE_BLE_CONNECTREPEAT_COMPILE
//            HC08.counter_testconnectrepeat_receive ++ ;
//         #endif
//            if(True == CmpStrWithFreeATCmd(Str_ATBLECONNECT,Strlen_ATBLECONNECT))
//            {
//                //SendATCmdResponseCode(RCODE_BLEConnect);
//				ResponseATBLEConnect();
//                HC08.timer_afterconnectwithApp_per2ms = TimePeriod_2s_Per2ms ; 
//                AT.status_BLEconnect = True; 
//            }
//            else
//                ResponseErrorATCmdConncetCmdNotYetSent();
//        }
//        else
//            ;
//    }
//    else
//    {
//        ( * MatchFreeATCmd())();
//        //SendATErrorStatusRealTimeWithNoAsk();
//    }
//    
//    if(False == IsConnectedWithBLEApp())
//    {
//		AT.status_BLEconnect = False ;
//		CloseCyclicSend();
//        /*当App连接断开时，下次上电门锁不可以直接解锁*/
//        Guard.status_unguardrecoverable_whenstayAppconnected = False ;	
//    }
	if(GPS.Connect.timer_sendconnectpackage_per100ms >= Timer_GPSconnect_SendPeriod_Per100ms )
	{
		GPS.Connect.timer_sendconnectpackage_per100ms = 0 ;
		
		if(TargetBuff1 == GetBuffNumWhichHaveConnectPackage())
		{
			SendTX2Buff1ToDMA();
			Send_DMAToUsart2.Buff1.status = BuffSending ;		
		}
		else if (TargetBuff2 == GetBuffNumWhichHaveConnectPackage())
		{
			SendTX2Buff2ToDMA();
			Send_DMAToUsart2.Buff2.status = BuffSending ;		
		}	
		else	
		{
			GPS.connect_target = GetFinalTX2Buff() ;
			SendM2PTConnectPackage() ;
			if(TargetBuff1 == GPS.connect_target)
			{
				SendTX2Buff1ToDMA();
				Send_DMAToUsart2.Buff1.status = BuffSending ;
			}
			else 
			{
				SendTX2Buff2ToDMA();
				Send_DMAToUsart2.Buff2.status = BuffSending ;
			}	
		}
	}
	
	if(GPS.Connect.timer_outdetect_per100ms >= Timer_GPSconnect_Outtime_Per100ms)
	{
		GPS.Connect.status = False ;
	} 
	
	( * MatchRX2Response())();
	
		
	if(True == GPS.Connect.status )
	{
		MatchGPSSendProcess();	
		CheckTX2BuffStatusAndSendBuff();
	}
	else
	 ;
	
	
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void MatchGPSSendProcess(void)
{
	if(Process_GPSPowerOnInitial == GPS.process_send)
	{	
		GPS.process_send = Process_HostTypeSent ;
		SendToSetHostType() ;
	}
	else if(Process_HostTypeResp == GPS.process_send)
	{
		GPS.process_send = Process_DeviceNumSent ;
		SendToSetDeviceNum() ;
	}
	else if(Process_DeviceNumResp == GPS.process_send)
	{
		GPS.process_send =  Process_QueryGPSStatusSent;
		SendToQueryGPSStatus() ;
	}
	else if(Process_QueryGPSStatusResp == GPS.process_send)
	{
		GPS.process_send = Process_Passthrough ;
		
#if TEST_COMPILE
		SendToQueryACCOffVoltage();
#else
		SendPassThroughTest1();				//test!!!!!!
#endif
		
	}
	else
		;
	
}
	
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:用if..return...if..return 函数判断 用函数指针做执行 优化掉这么长的if...else函数
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void (*MatchRX2Response(void))()
{
    void (*p)()  ;
	p = NOPFunction ;
	
    if(True == Reveive_Usart2ToDMA.flag_newdata)
    {
		Reveive_Usart2ToDMA.flag_newdata = False ;
		if(True == IsReceivedRX2FormatRight() )
		{
			if((True == IsCPT2M() ) && (True == IsSameWithReCmd(ReCmd_ConnectPackage)) )
			{
				p = SetGPSConnectFlag;
				return p ;
			}
			else
			{
				if(True == GPS.Connect.status)
				{
					if(True == IsCPT2M())		//CPT2M
					{
						if(True == IsSameWithReCmd(ReCmd_SetHostType))
						{
							if(Process_HostTypeSent == GPS.process_send )
								GPS.process_send = Process_HostTypeResp ;
							//if(Cmd_SetHostType == GPS.CmdControl.num_waitforcmdresponse )
								p = ConfirmHostTypeSet ;
//							else
//								p = NOPFunction ;
							return p ;
						}
						if(True == IsSameWithReCmd(ReCmd_GPRSControlLock))
						{
							p = ReceiveGPRSControlLockCmd ;
							return p ;
						}
						if(True == IsSameWithReCmd(ReCmd_SetDeviceNum))
						{
							if(Process_DeviceNumSent == GPS.process_send )
								GPS.process_send = Process_DeviceNumResp ;
							//if(Cmd_SetDeviceNum == GPS.CmdControl.num_waitforcmdresponse )
								p = ConfirmDeviceNumSet ;
//							else
//								p = NOPFunction ;
							return p ;
						}
						if(True == IsSameWithReCmd(ReCmd_UpdateDeviceNum))
						{
							p = ConfirmDeviceNumUpdated ;
							return p ;
						}
						if(True == IsSameWithReCmd(ReCmd_QueryGPSStatus))
						{
							if(Process_QueryGPSStatusSent == GPS.process_send )
								GPS.process_send = Process_QueryGPSStatusResp ;
							//if(Cmd_QueryGPSStatus == GPS.CmdControl.num_waitforcmdresponse )
								p = GetGPSStatus ;
//							else
//								p = NOPFunction ;
							return p ;
						}						
						else
						{
							p = UndefinedCommand;
							return p ;
						}
					}
					else
					{
						if(Process_Passthrough == GPS.process_send )
						{
							if(MCmd_PassthroughSend ==  Reveive_Usart2ToDMA.Data[GPS_Cmd_ArrNum])
								p = GetPassagethroughMessageAndSendReceipt ; 
							else if(MCmd_PassthroughResponse ==  Reveive_Usart2ToDMA.Data[GPS_Cmd_ArrNum])
								p = GetPassagethroughReceipt ;
							else
								p = UndefinedCommand ;
						}
						else
							p = RX2ProcessError;
						
						return p ;
					}
				}
				else
				{
					p = HandleReCmdWhenUnconnect ;
					return p ;
				}
			}
		}
		else
		{
			p = RX2ErrorFormatFunction ;
			return p ;
		}    
    }
    else
    {
        p = NOPFunction ;
        return p ;
    }
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsReceivedRX2FormatRight(void)
{
	if(Reveive_Usart2ToDMA.counter < 12)		//strlen too short
		return  False ;
	if(GPS_StartByte_1 != Reveive_Usart2ToDMA.Data[0]  )
		return False ;
	if(GPS_StartByte_2 != Reveive_Usart2ToDMA.Data[1]  )
		return False ;
	if(GPS_CheckByte_2 != Reveive_Usart2ToDMA.Data[Reveive_Usart2ToDMA.counter - 3] )
		return False ;
	if(GPS_StopByte_1 != Reveive_Usart2ToDMA.Data[Reveive_Usart2ToDMA.counter - 2] )
		return False ;
	if(GPS_StopByte_2 != Reveive_Usart2ToDMA.Data[Reveive_Usart2ToDMA.counter - 1] )
		return False ;
	uint8_t local_temp ;
	local_temp = Reveive_Usart2ToDMA.counter ;
	if((local_temp - 5 ) != Reveive_Usart2ToDMA.Data[2])
		return False ;
	if((Reveive_Usart2ToDMA.Data[3] != CPT2M ) && (Reveive_Usart2ToDMA.Data[3] != MPT2M) )
		return False ;
	if((local_temp - 5 ) != Reveive_Usart2ToDMA.Data[2])
		return False ;
	uint8_t check1 ;
	check1 = Reveive_Usart2ToDMA.Data[2] ;
	for(uint8_t i = 3 ;i < (Reveive_Usart2ToDMA.counter - 4 ) ; i ++ )
		check1 ^= Reveive_Usart2ToDMA.Data[i] ;
	if(check1 != Reveive_Usart2ToDMA.Data[Reveive_Usart2ToDMA.counter - 4])
		return False ;
	else
		return True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void RX2ErrorFormatFunction(void)
{
	;			//test!!!!!!!

}


/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void RX2ProcessError(void)
{
	;
}


/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsCPT2M(void)
{
    if(CPT2M == Reveive_Usart2ToDMA.Data[3])
        return True ;
    else
        return False ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:当收到相应的命令时把相应的buff状态置空
				如果相应的命令占了两个buff，就同时把两个buff都置空
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsSameWithReCmd(uint8_t recmd_ID)
{
    if(recmd_ID == Reveive_Usart2ToDMA.Data[GPS_Cmd_ArrNum])
	{
		if(BuffWaitForReply == Send_DMAToUsart2.Buff1.status )
		{
			if(recmd_ID == Send_DMAToUsart2.Buff1.data[GPS_Cmd_ArrNum])
			{
				Send_DMAToUsart2.Buff1.status = BuffEmpty ;			
			}
			else
				;				
		}
		else 
			;
		if(BuffWaitForReply == Send_DMAToUsart2.Buff2.status )
		{
			if(recmd_ID == Send_DMAToUsart2.Buff2.data[GPS_Cmd_ArrNum])
			{
				Send_DMAToUsart2.Buff2.status = BuffEmpty ;			
			}
			else
				;				
		}
		else 
			 ;	
				
		 return True ;		
	}
    else
        return False ;
}	

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:当收到相应的命令时把相应的buff状态置空
				如果相应的命令占了两个buff，就同时把两个buff都置空
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsSameWithRePassthrough(uint8_t recmd_ID)
{
    if(recmd_ID == Reveive_Usart2ToDMA.Data[GPS_Cmd_ArrNum])
	{
		if(BuffWaitForReply == Send_DMAToUsart2.Buff1.status )
		{
			if(recmd_ID == Send_DMAToUsart2.Buff1.data[GPS_Cmd_ArrNum])
			{
				Send_DMAToUsart2.Buff1.status = BuffEmpty ;			
			}
			else
				;				
		}
		else 
			;
		if(BuffWaitForReply == Send_DMAToUsart2.Buff2.status )
		{
			if(recmd_ID == Send_DMAToUsart2.Buff2.data[GPS_Cmd_ArrNum])
			{
				Send_DMAToUsart2.Buff2.status = BuffEmpty ;			
			}
			else
				;				
		}
		else 
			 ;	
				
		 return True ;		
	}
    else
        return False ;
}	



/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void UndefinedCommand(void)
{
	;

}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SetGPSConnectFlag(void)
{
	if(False == GPS.Connect.status)
	{
		GPS.Connect.status = True ;
		//GPS.process_send = Process_ReConnect ;
	}
	GPS.Connect.timer_outdetect_per100ms = 0 ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void HandleReCmdWhenUnconnect(void)
{
	;
}

///*******************************************************************************
//* 函数名  	 	:
//* 函数描述    	:当收到心跳包时把相应的buff状态置空
//				如果心跳包占了两个buff，就同时把两个buff都置空
//* 输入参数      : 无
//* 输出结果      : 无
//* 返回值        : 无
//*******************************************************************************/
//void SetConnectPackageBuffToEmpty(void)
//{						
//	if(BuffWaitForReply == Send_DMAToUsart2.Buff1.status )
//	{
//		if((CM2PT == Send_DMAToUsart2.Buff1.data[3]) && \
//			(Cmd_ConnectPackage == Send_DMAToUsart2.Buff1.data[4])
//		{
//			Send_DMAToUsart2.Buff1.status = BuffEmpty ;			
//		}
//		else
//			;				
//	}
//	else 
//		;
//	if(BuffWaitForReply == Send_DMAToUsart2.Buff2.status )
//	{
//		if((CM2PT == Send_DMAToUsart2.Buff2.data[3]) && \
//			(Cmd_ConnectPackage == Send_DMAToUsart2.Buff2.data[4])
//		{
//			Send_DMAToUsart2.Buff1.status = BuffEmpty ;			
//		}
//		else
//			;				
//	}
//	else 
//		;		
//}
						
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ConfirmHostTypeSet(void)
{
	if(SetSuccess == Reveive_Usart2ToDMA.Data[5])
		;
	else
		;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ReceiveGPRSControlLockCmd(void)
{
	if(0X00 == Reveive_Usart2ToDMA.Data[5])
		Guard.cmd_lock_From_GPRS = Unlock_ByGPRS ;
	else
		Guard.cmd_lock_From_GPRS = Lock_ByGPRS ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ConfirmDeviceNumSet(void)
{
	if(SetSuccess == Reveive_Usart2ToDMA.Data[5])
		;
	else
		;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ConfirmDeviceNumUpdated(void)
{
	if(SetSuccess == Reveive_Usart2ToDMA.Data[5])
		;
	else
		;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void GetGPSStatus(void)
{
	if(Reveive_Usart2ToDMA.Data[5] & U8_BIT0)
		GPS.status_GPS_hardware =  True ;
	else
		GPS.status_GPS_hardware =  False ;
	if(Reveive_Usart2ToDMA.Data[5] & U8_BIT1)
		GPS.status_TCPIP =  True ;
	else
		GPS.status_TCPIP =  False ;
	if(Reveive_Usart2ToDMA.Data[5] & U8_BIT2)
		GPS.flag_server_gothosttype =  True ;
	else
		GPS.flag_server_gothosttype =  False ;
	if(Reveive_Usart2ToDMA.Data[5] & U8_BIT3)
		GPS.flag_server_registerdevicenum =  True ;
	else
		GPS.flag_server_registerdevicenum =  False ;
	if(Reveive_Usart2ToDMA.Data[5] & U8_BIT4)
		GPS.flag_GPRS_vibrationalarm =  ENABLE ;
	else
		GPS.flag_GPRS_vibrationalarm =  DISABLE ;
	if(Reveive_Usart2ToDMA.Data[5] & U8_BIT5)
		Guard.cmd_lock_From_GPRS =  Lock_ByGPRS;
	else
		Guard.cmd_lock_From_GPRS =  Unlock_ByGPRS;
}





/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
FlagStatus IsPassagethroughMessageFormatRight(void)
{	
	if(Reveive_Usart2ToDMA.Data[5] != GPS.PassagethroughRec.counter_data )	// if passthrough message length is not equal actual length
	   return False ;
	else
		return True ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void GetPassagethroughMessageAndSendReceipt(void)
{
	uint8_t arr[StrlenN_Rec_PassthroughSend + 2];
	arr[0] = MM2PT ;										//Passageway
	arr[1] = MCmd_PassthroughResponse ;
	
	GPS.PassagethroughRec.counter_data = Reveive_Usart2ToDMA.counter - 12 ;
	if(True == IsPassagethroughMessageFormatRight())
	{
		for(uint8_t i = 0 ;i < GPS.PassagethroughRec.counter_data; i ++ )
			GPS.PassagethroughRec.Data[i] = Reveive_Usart2ToDMA.Data[5 + 1 + i] ;
		//GPS.PassagethroughRec.flag_receiveconfirmed = True ;
		arr[2] = 0X01 ;			//success
	}
	else
		//GPS.PassagethroughRec.flag_receiveconfirmed = False ;
		arr[2] = 0X00 ;			//failture
	GPS.PassagethroughRec.serialnum_forresponse.h8 =  \
			Reveive_Usart2ToDMA.Data[5 + 1 + GPS.PassagethroughRec.counter_data ] ;
	GPS.PassagethroughRec.serialnum_forresponse.l8 =  \
			Reveive_Usart2ToDMA.Data[5 + 1 + GPS.PassagethroughRec.counter_data + 1 ] ;
	
#if((2 + 1) !=(StrlenN_Rec_PassthroughSend + 2))
	#error "StrlenN_Rec_PassthroughSend长度不对"
#endif
	
	 //WriteToTX2Buff(arr,StrlenN_Rec_PassthroughSend) ;
	FillTX2BuffWithData(GetFinalTX2Buff(),arr,StrlenN_Rec_PassthroughSend) ;
	//SendPassagethroughReceipt();
}


/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
TargetBuff_Type GetPassagethroughReceiptMatchBuff(void)
{
	if(Reveive_Usart2ToDMA.Data[5] != SUCCESS)
		return NoTarget ;
	if((Reveive_Usart2ToDMA.Data[6] == Send_DMAToUsart2.Buff1.num_serial.h8)\
		&&(Reveive_Usart2ToDMA.Data[7] == Send_DMAToUsart2.Buff1.num_serial.l8))
		return TargetBuff1 ;
	else if ((Reveive_Usart2ToDMA.Data[6] == Send_DMAToUsart2.Buff2.num_serial.h8)\
		&&(Reveive_Usart2ToDMA.Data[7] == Send_DMAToUsart2.Buff2.num_serial.l8))
		return TargetBuff2 ;
	else
		return NoTarget ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void GetPassagethroughReceipt(void)
{
	if(TargetBuff1 == GetPassagethroughReceiptMatchBuff())
	{
		//Send_DMAToUsart2.Buff1.flag_sendconfirmed = True  ;
		Send_DMAToUsart2.Buff1.status = BuffEmpty ;
	}
	else if(TargetBuff2 == GetPassagethroughReceiptMatchBuff())
	{
		//Send_DMAToUsart2.Buff2.flag_sendconfirmed = True  ;
		Send_DMAToUsart2.Buff2.status = BuffEmpty ;
	}
	else	//No Target
		IfNoMatchTargetBuff() ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void IfNoMatchTargetBuff(void)
{
	;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
TargetBuff_Type GetBuffNumWhichHaveConnectPackage(void)
{
	if((CM2PT == Send_DMAToUsart2.Buff1.data[GPS_Passway_ArrNum]) &&\
		(Cmd_ConnectPackage == Send_DMAToUsart2.Buff1.data[GPS_Cmd_ArrNum]) && \
		(BuffWaitForReply == Send_DMAToUsart2.Buff1.status)	)
		return TargetBuff1 ;
	else if((CM2PT == Send_DMAToUsart2.Buff2.data[GPS_Passway_ArrNum]) &&\
		(Cmd_ConnectPackage == Send_DMAToUsart2.Buff2.data[GPS_Cmd_ArrNum]) && \
		(BuffWaitForReply == Send_DMAToUsart2.Buff2.status)	)
		return TargetBuff2 ;
	else
		return NoTarget ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SendM2PTConnectPackage(void)
{
	 uint8_t arr[StrlenN_Send_ConnectPackage + 2];
     arr[0] = CM2PT ;										//Passageway
	 arr[1] = Cmd_ConnectPackage ;
	 arr[2] = 0X00 ;		//fixed value	

#if((2 + 1) !=(StrlenN_Send_ConnectPackage + 2))
	#error "StrlenN_Send_ConnectPackage长度不对"
#endif
	 
	 //SendDataByTX2DMA(arr,StrlenN_Send_ConnectPackage);
	 FillTX2BuffWithData(GPS.connect_target,arr,StrlenN_Send_ConnectPackage) ;
	 
	 
//	 if( TargetBuff1 == WriteToTX2Buff(arr,StrlenN_Send_ConnectPackage) )
//	 {
//	 	SendTX2Buff1ToDMA() ;
//		Send_DMAToUsart2.Buff1.status = BuffSending ;
//	 }	 
//	else if(TargetBuff2 == WriteToTX2Buff(arr,StrlenN_Send_ConnectPackage))
//	{
//	
//		SendTX2Buff2ToDMA() ;
//		Send_DMAToUsart2.Buff2.status = BuffSending ;
//	}
//	else
//		;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SendToSetHostType(void)
{
	 uint8_t arr[StrlenN_Send_HostType + 2];
     arr[0] = CM2PT ;										//Passageway
	 arr[1] = Cmd_SetHostType ;

#if(L000001AG == CumstomerNum)
	arr[2] = HostTypeB ;
#else
	 arr[2] = HostTypeA ;	
#endif

#if((2 + 1) !=(StrlenN_Send_HostType + 2))
	#error "StrlenN_Send_HostType长度不对"
#endif
	 
	 //SendDataByTX2DMA(arr,StrlenN_Send_HostType);
	 //WriteToTX2Buff(arr,StrlenN_Send_HostType) ;
	 FillTX2BuffWithData(GetFinalTX2Buff(),arr,StrlenN_Send_HostType) ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void ResponseGPRSControlLock(void)
{
	 uint8_t arr[StrlenN_Send_GPRSControlLock + 2];
     arr[0] = CM2PT ;										//Passageway
	 arr[1] = Cmd_GPRSControlLock ;
	 arr[2] = SetSuccess ;	

#if((2 + 1) !=(StrlenN_Send_GPRSControlLock + 2))
	#error "StrlenN_Send_GPRSControlLock长度不对"
#endif
	 
	 //SendDataByTX2DMA(arr,StrlenN_Send_GPRSControlLock);
	  //WriteToTX2Buff(arr,StrlenN_Send_GPRSControlLock) ;
	  FillTX2BuffWithData(GetFinalTX2Buff(),arr,StrlenN_Send_GPRSControlLock) ;
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SendToSetDeviceNum(void)
{
	 uint8_t arr[StrlenN_Send_SetDeviceNum + 2];
     arr[0] = CM2PT ;										//Passageway
	 arr[1] = Cmd_SetDeviceNum ;
	 
#if(L000001AG == CumstomerNum)
	  arr[2] = 0X00 ;			//test!!!!!!!
	 arr[3] = 0X00 ;
	 arr[4] = 0X00 ;
	 arr[5] = 0X00 ;
	 arr[6] = 0X00 ;
	 arr[7] = 0X00 ;
	 arr[8] = 0X00 ;
	 arr[9] = 0X00 ;
	 arr[10] = 0X00 ;
	 arr[11] = 0X00 ;
	 arr[12] = 0X00 ;
	 arr[13] = 0X00 ;
#else	 
	 arr[2] = 0X01 ;			//test!!!!!!!
	 arr[3] = 0X02 ;
	 arr[4] = 0X03 ;
	 arr[5] = 0X04 ;
	 arr[6] = 0X05 ;
	 arr[7] = 0X06 ;
	 arr[8] = 0X07 ;
	 arr[9] = 0X08 ;
	 arr[10] = 0X09 ;
	 arr[11] = 0X0A ;
	 arr[12] = 0X0B ;
	 arr[13] = 0X0C ;
#endif
	 
#if((13 + 1) !=(StrlenN_Send_SetDeviceNum + 2))
	#error "StrlenN_Send_SetDeviceNum长度不对"
#endif
	 
	 //SendDataByTX2DMA(arr,StrlenN_Send_SetDeviceNum);
	 FillTX2BuffWithData(GetFinalTX2Buff(),arr,StrlenN_Send_SetDeviceNum) ;
	 
}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SendToUpdateDeviceNum(void)
{
	 uint8_t arr[StrlenN_Send_UpdateDeviceNum + 2];
     arr[0] = CM2PT ;										//Passageway
	 arr[1] = Cmd_UpdateDeviceNum ;
	 arr[2] = 0X11 ;			//test!!!!!!!
	 arr[3] = 0X12 ;
	 arr[4] = 0X13 ;
	 arr[5] = 0X14 ;
	 arr[6] = 0X15 ;
	 arr[7] = 0X16 ;
	 arr[8] = 0X17 ;
	 arr[9] = 0X18 ;
	 arr[10] = 0X19 ;
	 arr[11] = 0X1A ;
	 arr[12] = 0X1B ;
	 arr[13] = 0X1C ;
	 
#if((13 + 1) !=(StrlenN_Send_UpdateDeviceNum + 2))
	#error "StrlenN_Send_UpdateDeviceNum长度不对"
#endif
	 
	 //SendDataByTX2DMA(arr,StrlenN_Send_UpdateDeviceNum); 	 
	 //WriteToTX2Buff(arr,StrlenN_Send_UpdateDeviceNum) ;
	 FillTX2BuffWithData(GetFinalTX2Buff(),arr,StrlenN_Send_UpdateDeviceNum) ;

}

/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SendToQueryGPSStatus(void)
{
	uint8_t arr[StrlenN_Send_QueryGPSStatus + 2];
	arr[0] = CM2PT ;										//Passageway
	arr[1] = Cmd_QueryGPSStatus ;
	arr[2] = 0X00 ;		//fixed value	
	
#if((2 + 1) !=(StrlenN_Send_QueryGPSStatus + 2))
	#error "StrlenN_Send_QueryGPSStatus长度不对"
#endif
	
	//SendDataByTX2DMA(arr,StrlenN_Send_QueryGPSStatus);
	 //WriteToTX2Buff(arr,StrlenN_Send_QueryGPSStatus) ;
	FillTX2BuffWithData(GetFinalTX2Buff(),arr,StrlenN_Send_QueryGPSStatus) ;

}

#if TEST_COMPILE
/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SendToQueryACCOffVoltage(void)
{
	uint8_t arr[StrlenN_Send_QueryACCOffVoltage + 2];
	arr[0] = CM2PT ;										//Passageway
	arr[1] = Cmd_QueryACCOffVoltage ;
	arr[2] = 0X00 ;		//fixed value

	
#if((2 + 1) !=(StrlenN_Send_QueryACCOffVoltage + 2))
	#error "StrlenN_Send_QueryACCOffVoltage长度不对"
#endif

	FillTX2BuffWithData(GetFinalTX2Buff(),arr,StrlenN_Send_QueryACCOffVoltage) ;	
}

#endif



/*******************************************************************************
* 函数名  	 	:
* 函数描述    	:
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void SendPassThroughTest1(void)
{
	uint8_t arr[StrlenValidData_Test1 + 3];
	arr[0] = MM2PT ;										//Passageway
	arr[1] = MCmd_PassthroughSend ;
	arr[2] = StrlenValidData_Test1 ;		
	arr[3] = 0X54 ;		//"T"
	arr[4] = 0X45 ;		//"E"
	arr[5] = 0X53 ;		//"S"
	arr[6] = 0X54 ;		//"T"
	arr[7] = 0X31 ;		//"1"

	
#if((7 + 1) !=(StrlenValidData_Test1 + 3))
	#error "StrlenValidData_Test1长度不对"
#endif
	
	//SendDataByTX2DMA(arr,(StrlenValidData_Test2 + 1));	
	//WriteToTX2Buff(arr,(StrlenValidData_Test2 + 1)) ;
	FillTX2BuffWithData(GetFinalTX2Buff(),arr,(StrlenValidData_Test1 + 1)) ;	
}


#endif
#endif