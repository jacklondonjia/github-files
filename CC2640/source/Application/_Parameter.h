#ifndef __PARAMETER_H
#define __PARAMETER_H
//set this to 1 to disable the paired/bonded check before processing AT commands
#define DEBUG_VERSION 1
/********************PCB Version*****************************/
#define BLE_D					0x0101	//�Ѿ���̭
#define BLE_E					0x0102	
#define	BLE_G					0x0103	
#define	BLE_H					0x0104
#define	BLE_J					0x0105
#define RGZ_A					0x0200
#define	RGZ_B					0x0201
#define DLCC01					0x0202

/******************** Commu_Mode *****************************/
#define Commu_Mode_Common               0x00            //0x00��ͨ�ÿ������汾  0x01��һ��ͨͨѶ�汾    0x02��˫����ư汾
#define Commu_Mode_OneCommu             0x01
#define Commu_Mode_TwoWire              0x02

/******************** Battery_Materials *****************************/
//low 3bit means Materials, high 5bit means factory
#define	Lead_Acid_Batteries				0X00		//Ǧ����
#define MnNiCo_Ternary_Battery_CHILWEE	0X01		//��Ԫ﮵�� ����
#define	MnNiCo_Ternary_Battery_PHYLION	0X09		//��Ԫ﮵�� �Ǻ�

/******************** Customer type *****************************/
#define Manufactor_Ebike		0X00	//�綯������
#define	Rent_Ebike				0X01	//�⳵�̻���

/******************** Anti-theft Device Function *****************************/
#define External_Anti_theft		0X00	//���÷����� ������е�������������ơ������綯˫�Žš�������Կ������
#define	FreeKeyStart			0X01	//����Կ������
#define	ElectricLockControl		0X02	//����һ���Ļ��������ӻ�е��������������
#define	ElectricDouSupport		0X03	//����һ���Ļ��������ӵ綯˫�Ž�

/******************** Cyclic Send TimePeriod *****************************/
#define CyclicSendMessagePer30s			0X00	//30s ??????????
#define CyclicSendMessagePer0_678s		0X01	//????????? ?0.678s???? ????1s??1??	

/*********************LV pin define******************/
//#define	Pin_LV				GPIO_Pin_4
//#define Port_LV				GPIOA
//#define ADC1_Channel_LV		ADC_Channel_4

/********************Host Type*****************************/
#define	HostTypeA				0X01
#define	HostTypeB				0X02

/********************data_size*************/
#define BufferSize			        50
#define SENDBUFF_SIZE				50
#define RECEIVEBUFF_SIZE			50




/**********************if compile enable************/
#define	COMPILE_ENABLE			1
#define	COMPILE_DISABLE			0
#define NVIC_COMPILE			COMPILE_ENABLE
#define USART1_COMPILE			COMPILE_ENABLE
#define DMA_TX1_COMPILE			COMPILE_ENABLE
#define	DMA_RX1_COMPILE			COMPILE_ENABLE	

#define ADC1_COMPILE			COMPILE_ENABLE
#define YXT_COMPILE				COMPILE_ENABLE
#define GUARD_COMPILE			COMPILE_ENABLE
#define BLESET_COMPILE			COMPILE_ENABLE
#define	BUZZER_COMPILE			COMPILE_ENABLE
#define	YK_COMPILE				COMPILE_DISABLE
#define	Reserved_Pin_COMPILE	COMPILE_ENABLE

#define	USART1_TX_INT_COMPILE	        	COMPILE_DISABLE
#define	USART1_RX_INT_COMPILE	        	COMPILE_DISABLE
#define	USART1_IDLE_INT_COMPILE	        	COMPILE_ENABLE

#define TESTCODE_TIMER_Q_COMPILE			COMPILE_DISABLE
#define TESTCODE_BLE_OUTPUT_COMPILE			COMPILE_DISABLE
#define TESTCODE_BLE_CONNECTREPEAT_COMPILE	COMPILE_DISABLE
#define	TESTCODE_BLE_220V					COMPILE_DISABLE
#define	TESTCODE_TANGZESHACHE				COMPILE_DISABLE


#endif
