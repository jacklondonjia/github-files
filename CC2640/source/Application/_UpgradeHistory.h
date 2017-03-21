#ifndef __UPGRADEHISTORY_H
#define __UPGRADEHISTORY_H

/*
Firmware 1.0.1  2015.08.13 15:41	Tested
	Add DetectConnectWithConctroller �� PCBVersion And FirmwareVersion Cmd

Firmware 1.0.2  2015.08.17 12:53	Tested
	1.Get BLEAddress��Put this message into BLEAdvertising
		Attention�����ֹ��������С��������ϵ�bug �������������Ͽ�����ʱ���Ͳ�ѯ������ַ��AT����
					Ӧ����20,C3,8F,FC,FD,D5 /r/n 	ʵ���Ͻ��յ�����20һ֡ ��9ms �յ���һ������ 
					�ٸ�8ms�Ž��յ�ʣ���������ݡ�
Firmware 1.0.3
	1.Add��Typedef Level
	2.modify FirmwareVersion
		
Firmware 1.0.4
	1.Add ATCmd: AT+Deadline=?
		��ѯMCU�б�������ڻ��ж೤ʱ�䣬��ȷ������	
	2.modify FirmwareVersion

Firmware 1.0.5
	1.Modify ATCmd: AT+GuardStatus=?
		1byte����״̬+1byte����ԭ��
	2.modify FirmwareVersionCheck
	3.Optimization GuardControl
	
Firmware 1.0.9
	1.Never Set	Adver Of HC08
	2.Add #warning when use test code 
	3.Add #warning to remind mode in Annotate.h
		
Firmware 1.0.0A
	1.When BLE isn't disconnect with app��unguard the bike after Power to controller
	2.Add log in iar		
		
Firmware 1.0.0B
	1.Optimization"When BLE isn't disconnect with app��unguard the bike after Power to controller"
	 	����һ���汾�У�������bug���ر��������Ӻ󣬲���Կ�ף��綯�����Զ�����		

Firmware 1.0.0C
	1.Reconstruction level detection code
	2.Add Mileage Cal
		����̲ο�1.1.12 ������һ�λ����ۼƴ�����soc������ ��������� ��Ϊ��Ҫ�ο�
		ʣ����̰���
		ʣ�������������Ȩ������ɣ�һ�����ǰ��յ�ʱ�ĵ����ͻ������������mileage
			��һ�����ǰ��ձ������������ĵ������ĺͻ��������㴦��mileage��
		attention����Ҫ���������̵�0Ϊ��ĸ�����
	3.Add AT+Mileagemax=? AND AT+Mileageremainder=? Cmd
	4.Add Mileage To Flash

Firmware 1.0.0D
	1. Modify Q cal ,last version is Accumule current,this version is accumulate current multiply volatege
	2. Modify #define LV_Uncharging_Max_D and so on ,so that the value in code is sure as uint16_t
	3. Rename element of Q
	4. Modify M max cal,last version is based on the cal result of only this trip
	   this version add Weighted Algorithm ,part of the original Mmax and part of this trip cal result
	5. Modify Q remainder cal.
		In last version cpu updata Qremainder only after LV_1h equl LV_45min.
	   		Qremainder after charge is equal Qremander before charge.
	   So in this version , CPU will updata Qremainder after charge by setting a flag 
			when LV_1h is more large than LV_45min .CPU will update Qremainder when BLE module connect to controller.
	6. Modify judgement of firmwareversion
	7. Modify MmaxCal in NotSpecifiedControllerVersion
	8. Add Q_Max_12Ah	
		
Firmware 1.0.0E
	1.	Modify UART_DMA
		������USART_DMACmd��������usart ������DMA��ʽ��DMA_Cmd (DMA1_Channel4,ENABLE);
	2.  Add BLE connect repeat Test		
	3. if(True == CmpStrWithFreeATCmd(Str_ATERRORSTATUS,Strlen_ATERRORSTATUS)) 
		last version : p= ResponseErrorATCmdConnectedToMCU;
		this version : p= ResponseATErrorStatus;
	4. 32λ�ĳ˳��� ��������Ȼ�������˷���������� ����ֵ��С
	5. ���ӵ�ص�ѹ��������������
		
Firmware 1.0.0F
	1. App add time out control ��so mcu add guardCmd response
	2. Add the pull-down resistor Of YXT 
		оƬGPIO�ڲ���/�������� min = 20k ��typ= 30k ��max = 40k ��
	3. Modify LV 
		ͬ���Ľṹ�岻�����ھֲ�����
	4. Add Inquire Mileag Remainder when charging
		������״̬ ��ѯʣ����̡�
	5. Make sure not updata Qintial When Inquire Mileag Remainder
		ȷ�ϲ�ѯʣ����̵�ʱ�򲻸���Qintial����ֹƵ������QInital��������̼��㲻׼ȷ
	6. solve  the problem that when MCU reset mileagemax equal 0
		��Ϊ����д��flashʱ��δд��mileage max��ֵ
	7. solve the problem that soc equal ff or fe
		��ΪSOC�ļ������г��������С�����ܻᱻ���ԣ����¼�������ָ�ֵ�������
		�����ڷ�#define�Ĵ����У���Ҫ���븡�����	

		
Firmware 1.0.10
	1. Modify Hall Speed Cal
		��Ϊ������������HallSpeed�����������ձ�����������������ֹʱhall�ٶ���0x12 
		����趨hallspeed��Сֵ����hallspeedС��0x15ʱ����ʾΪ0
	2. Modify Mileage Remander Cal
   		ԭ���ĵ����ǵ�ѹ*������������ʣ����̵�ʱ��ֻ�����˵���û�п��ǵ�ѹ������ʣ�����ֵ�ܴ�
	3. Modify message storage and process
		�Ż�ָ���������һ֡���ݺ���е���Ϣ�洢�ʹ���
		
*******************************************************************************
Firmware 1.1.2 2015.08.19 16:23		Tested
	1.Control Guard Level Without Our Controller
		Attention��Ϊ�˼����⳵���̼ҵĸ����豸�ɱ� ���Ƴ��ð汾 
					�ð汾���Բ�ʹ�ö�������Ŀ�������ֻ�ǶԿ������ķ��������źŽ��п���
					�������������5V��Դ���жϵ������Ƿ��Ѿ��ϵ�
	2. ��ȫ�ֱ�����ȡ��������_Parameter.h�ļ���

Firmware 1.1.3
	1.Add Hall count
		Attention��Ϊ�˽�����չ������ʹ�ö������������������ģ��ȽϾ�׼�ļ����ٶ�
					�Լ����ݵ�������ʣ����̡�
					�����ڻ�����6������������һ�������ߵ�������KEY1�ڡ�
	2.Add��Typedef Level
	3.modify FirmwareVersion
		
Firmware 1.1.4
	1.Add ATCmd: AT+Deadline=?
		��ѯMCU�б�������ڻ��ж೤ʱ�䣬��ȷ������	
	2.modify FirmwareVersion

Firmware 1.1.5
	1.Modify ATCmd: AT+GuardStatus=?
		1byte����״̬+1byte����ԭ��
	2.modify FirmwareVersionCheck
	3.Optimization GuardControl

Firmware 1.1.6
	1.Optimization AdverMode
			When CONT=0 HC-08�еĹ㲥��Ϣ���ܻᱻ�жϴ�� ���¹㲥��Ϣ������ 
		����ǳ���ģ������1-2��� ����ƻ���豸�޷���������Ӧ�Ĺ㲥��Ϣ
			���ڲ��õķ�ʽ���ڷ�APP����״̬ û��10min ����д��һ�ι㲥��Ϣ
				
Firmware 1.1.7
	1.rewrite AdverData of HC08 after rename HC08
	2.Optimization IsConnectedWithBLEApp()
		������������2s�ڲ��ټ���Ƿ��Ѿ������� ��ֹ��Ϊ�źŵ���ʱ���µ������������ֶϿ�������

Firmware 1.1.8
	1.Add HallSpeed To CyclicSend Data
	2.rewrite AdverData of HC08 every 1min

Firmware 1.1.9
	1.Never Set	Adver Of HC08
	2.Add #warning when use test code 
	3.Add #warning to remind mode in Annotate.h

Firmware 1.1.0A
	1.When BLE isn't disconnect with app��unguard the bike after Power to controller
	2.Add log in iar 
				
Firmware 1.1.0B
	1.Optimization"When BLE isn't disconnect with app��unguard the bike after Power to controller"
	 	����һ���汾�У�������bug���ر��������Ӻ󣬲���Կ�ף��綯�����Զ�����	
			
Firmware 1.1.0C
	1.Reconstruction level detection code
	2.Add Mileage Cal
	3.Add AT+Mileagemax=? AND AT+Mileageremainder=? Cmd
	4.Add Mileage To Flash
		
Firmware 1.1.0D
	1. Modify #define LV_Uncharging_Max_D and so on ,so that the value in code is sure as uint16_t
	2. Rename element of Q	
	3. Modify judgement of firmwareversion
	4. Modify MmaxCal in NotSpecifiedControllerVersion
	5. Add Q_Max_12Ah
	6. Modify Mmax and Mremainder Cal
				
Firmware 1.1.0E
	1.	Modify UART_DMA
		������USART_DMACmd��������usart ������DMA��ʽ��DMA_Cmd (DMA1_Channel4,ENABLE);
	2.  Add BLE connect repeat Test	
	3. change INT16_MAX To UINT16_MAX ,INT8_MAX To UINT8_MAX
	4. if(True == CmpStrWithFreeATCmd(Str_ATERRORSTATUS,Strlen_ATERRORSTATUS)) 
		last version : p= ResponseErrorATCmdConnectedToMCU;
		this version : p= ResponseATErrorStatus;
 	5. ���Ӽ�����������Ϊ0ʱ ��д��flash
	6. 32λ�ĳ˳��� ��������Ȼ�������˷���������� ����ֵ��С
	7. ���ӵ�ص�ѹ��������������

Firmware 1.1.0F
	1. App add time out control ��so mcu add guardCmd response
	2. Add the pull-down resistor Of YXT 
		оƬGPIO�ڲ���/�������� min = 20k ��typ= 30k ��max = 40k ��
	3. Modify IsConnectWithController()
		�Ƕ���ָ��������� ��ʱ������������ʱ���ϵ粻�ܿ��ơ���ʹоƬYXT����������Ҳ�������������⡣
		���ڸ�5V�жϼ��˲�	
	4. Modify LV 
		ͬ���Ľṹ�岻�����ھֲ�����
	5. Add Inquire Mileag Remainder when charging
		������״̬ ��ѯʣ����̡�
	6. Make sure not updata Qintial When Inquire Mileag Remainder
		ȷ�ϲ�ѯʣ����̵�ʱ�򲻸���Qintial����ֹƵ������QInital��������̼��㲻׼ȷ
	7. solve  the problem that when MCU reset mileagemax equal 0
		��Ϊ����д��flashʱ��δд��mileage max��ֵ
	8. solve the problem that soc equal ff or fe
		��ΪSOC�ļ������г��������С�����ܻᱻ���ԣ����¼�������ָ�ֵ�������
		�����ڷ�#define�Ĵ����У���Ҫ���븡�����


Firmware 1.1.10
	1. Modify Analyzing conditions of Status_ReachedDeadline_Park_NoExceed To Status_ReachedDeadline_Park_Exceed
		ԭ�����ж�����ֻ�����ڶ���ָ����Ŀ�����,���ڵ�֧�������汾
		�޸ĺ��guard.status_park���õ���IsConnectWithController()
	2. cal mileage of this trip	
		���㱾����ʻ���

Firmware 1.1.11
	1. Shorten the time of the guard need after the controller power down
		ԭ��Կ�׶Ͽ��Ժ���Ҫ4s���ң����ڵ���
	ͨ�ð�����ʱ��= ����������ʱ�� + 0.3s  
	ָ��������ʱ�� = ����������ʱ�� + ��ⲻ����ͷ�������ֽڡ�
	2.	The optional of automatically locked
		����BLECmd��AT+
			
Firmware 1.2.12
	1. Add Two-way Communicate with controller
		���ӺͿ�������˫��ͨѶ

Firmware 1.2.13
	1. Modify Electronic lock Switch
		���ӵ��������� ʹ�綯���ܹ�ȥ���Զ�����״̬

Firmware 1.2.14
	1. Add second in deadline
		�ظ���app�Ĺ�������ʱ���AT�����ȷ��s 
			
Firmware 1.1.15
	1. Modify ErrorStatus Response
		��һ��ͨ�����ȡ���Ĺ�����Ϣ����󷢸�app 
        �����������������Ϲ��ϣ�app��ͬʱ���յ�

Firmware 1.1.16   2015.12.03
	1. Add DevInfo ATCmd
                �汾����ֻ������������������ٷ�ӳͨ�ð桢˫����ơ�һ��ͨͨѶ����Ϣ
                ˫����Ƶ���Ϣ�洢��DevInfo�С�

Firmware 1.0.17   2015.12.04 09��22����2015.12.07
	1. Modify ATCmd
		���¹���ATCmd�����Ϲ������Ƶ�ATCmd��������޸�ATcontrol.c
    2. Modify Code
                ���ֽ����ع�
	3. Add ATCmd LVLevel
		˫����ư汾��˫Ƿѹ���ù���

Firmware 1.0.18   2015.12.07 10��50����2015.12.08 12:59
	1. Modify ATCmd Rename HC08
		�޸�HC-08������յ�app��������Ϣ�Ժ��Ȼظ�0X0A��app�Ͽ����������Ժ󣬸�hc-08ģ��д���µ�����
	2. Add ErrorHistory
		������ErrorHistory���Ѽ��ʹ���

Firmware 1.0.19   2015.12.08 12:59����2015.12.09 14:00
	1. Modify Charge Detect
		ʹ���µ��㷨����綯���Ƿ��ڳ�磺
			����⵽�綯��ֹͣ���е�ʱ��ÿ����ʱ�����soc�� �����ʱ��soc���ϴ����м������ʣ���soc��5% 
			����Ϊ�綯���ڳ�硣
		�жϵ綯���Ƿ�ֹͣ���������·�ʽ;
			���а棺�жϻ����Ƿ�仯 ���������жϣ�
    2. Modify ErrorATCmd Response
        �ڱȽ��ַ�����ʱ��ֱ�Ӱѽ�����Ϣ��־�����

Firmware 1.0.1A   2015.12.09 14:00����  2015.12.10 15:01
	1. Modify Q And Mileage Cal
		�����ӿ��������綯����硢��פ��ʱ�����Ϣ�ֱ��������Ƶ�Per500ms�Ĵ����С�
	2. Modify DMA Send
		�Ż����������һ�������ظ���app�Ժ� �Żᷢ����һ��response
	3. Remove SendATCmdResponseCode()
		�ѷ��͵����ֽڵĴ���ȫ���÷��͸����ֽڵĴ������
	4. Modify Initial And Remainder Of Q And Mileage
		 �Ż�Q��Mileage�� Initial��Remainder

Firmware 1.0.1B   2015.12.10 15:01
	1. Modify DMA Transfer
		�ر�ԭ���ķ��ͽ������жϡ�ÿ�η���ATCmd��ʱ���ȼ���Ƿ�����һ��ATCmd�ķ��ͽ�����־
		��ˣ���һ������AT �ظ�OK	
	2. Modify ATCmdSet Format Error Judgement
		��ATCMdSet�ĸ�ʽ������ж���ȡ����
	3. Slove the problem of array overflow
		��������ܷ����� ԭ����ErrorHistory�е��������ó����������е��趨�������������
	4. Slove the problem of Setting charged flag
		���δ���ʱ����־λ��������λ�������ԭ���ǣ�if����֮�������һ��";"
	5. Slove the problem of lock the motor immediately when deadline arrived
		����������ڣ���������������⡣ԭ���ǣ�GetHallSpeed�е���һ���ж�Ӧ����"!=" д����"=="

Firmware 1.0.1C   2015.12.29 15:01
	1. ����60V��LV��ѹ����ֵ
	2. ����220V������Դ���԰�
	3. �Ż���Q_MAX_20AH�趨�ļ���
	4. Ӧ������˷�������� ����᲻��
	5. ����ʣ�����Ϊ0��ԭ��
		�ҵ�����˷������ԭ���� Ӧ���Ȱ�32bit��ֵ��64bit �����˷�����
	6. ����tx���ͽ������ж� ���ͽ����Ժ��һ��ʱ����ܿ�ʼ�µ�dma.
	7. �޸�Ϊ������Ͷ�ٶ� ���β���ok ����45�����Ժ���µĵ�����ȷ ������̲���
	8. ���soc�ľ���


Firmware 1.0.1D   2016.01.11 15:01
	1. ʣ����̵���ԭʣ����̼�ȥ�������
	2.�����ܵ���
	3.�ر�Qamx�ļ���
	4.����ѭ������
	5. ����AT���� Ѱ��ʱ��������5��
	6. �����������¼�������
	7. ������λ0.678sǰ���ѹ�𽥱�͵��µ����
	8.������ϵ����������������
	9. ��hall��ٶ���ȡ����
	

Firmware 1.0.1E   2016.01.12 15:01
	1. �ѵ��������浽flash��ȥ
	2. �����������ʼ״̬������

Firmware 1.0.1F   2016.01.19 15:01
	1.���Ӳ��Ե������ͷ������Ͱ����AT����
	2.��ȡ����ƽpin�Ŷ��� ���Ұѷ��ͷŵ���������

Firmware 1.0.20   2016.01.20 15:01
	1. �޸�Ԥ�������� �������ȡ����
	2. ���Ұ汾Ĭ����������� Ĭ�Ͻ���״̬

Firmware 1.0.21   2016.03.14 15:01
	1. ����BLE-G ������������й���

Firmware 1.0.22   2016.04.05 15:01
	1. ����BLE-H ������������й���
	2 .���ӵ����
	3. �����ϵ�ϵ��������
	4. ���Ӱ�����
	5. �����ϵ�ϵ�ĳ�ʼ״̬
	6. �����е�������� �������ת��
	7. ��е����˫�Ŷ����������app�Ļظ�
	8. ��������������ʻ״̬��
	9. ������Ƶ��������������
	10. �����ϵ���λ���� �������������

Firmware 1.0.23   2016.04.05 15:01
	1. ˫��ͨѶ �ŵ��� ����һ��ͨ
	2. ����ϵ粻�����е�����

Firmware 1.0.24   2016.04.21 15:01
	1. ����BLE-J GPIO�ڻ������ú�
	2. �������������жϼ�������������ı���
	3. ��Կ�����������жϲ���ok
	4. ɲ������
	5. ˫�Ų��� ������ֵ����ok
	6. ����˫������
	7. �Ѿ������������˫���� ���Ƿ��ص���ֵ ë��̫��
	8. ���������߼� �߼�ͨ��
	9. �����ֶ���⼰�ϵ� �ֶ��޷�Ӧ��Ķϵ� ����ok
	10. ��˫����Ϣд��Flash 
	11. �����Կ������ϵ���龰
	12. �޸Ķ�ȡ���ڴ����bug
	13. ȥ��˫��״̬����
	14. ����˫�Ŷ�����ʱ
	15. �޸ĳ�˫��ͨѶ
	16. ���������ɲ���ͽ�����bug
	17. ���Դ�����жϵı��� �޸�Ӳ�����
	18. ������48v ADC����
	19. д�����ѹ�����߼�
	20. �����ϵ���߶ϵ�ʧ�ܵ��ж�
	21. ����˫�Ŷ���1.5s 

Firmware 1.0.25   2016.04.29 15:01
	1. ���48V�ĵ�������
	2. δ���ô��������
	3. �ѿͻ�������ȡ����
	4. �����ʱ ����3�δ�����ж��ź�
	5. IsDeadLineReachInFlash�ĳ�IsDeadLineReach
	6. �޸�Flash�еĳ�ʼʱ�� ����ȫ��F ��ΪСʱ�ͷ������ֵ����FF

Firmware 1.0.26   2016.05.04 15:01
	1. ��include�ŵ�h�ļ���
  
Firmware 1.0.27   2016.05.10 15:01
	1. �ѵ�����ѹֵ�ŵ�ѭ��������
	2. 0.678s����һ��ѭ�� 
	3. �ѻ����ٶȷŵ�ѭ���� 
	4. �ѻ������ٶȲ���ƽ���ƶ�
	5. ����GPRS-E�Ŀͻ�����

Firmware 1.0.28   2016.05.13 08:54
	1. �޸�FlashWrite�Ĵ��� ����ϵ�����ϵ� ���ڲ���ȷ������
	2. ���������������Ŀͻ�����
	3. ���������������Ŀͻ�����
	4. X�����µĿͻ�����
	5. ��½���������ֳ��Ŀͻ�����
	6. ����220V�Ĳ��Կͻ�����
	7. ����С�ɸ���ֳ��ͻ�����
	8. ��������ʷ�����.h

Firmware 1.0.29   2016.05.26 08:54
	1.	�Ѱ汾�ŷŵ�customerpara.h��
	2. ��ѭ�����������ӵ�ѹ��������ʵʱ��Ϣ ѭ��ʱ����30s
		

Firmware 1.0.2A  2016.06.07 15��55
	1. ��test�Ĵ���������������ȡ����
	2. ����usart2��DMA����
	3. ����GPSControl��.c ��.h�ļ�
	4. ��ѭ�����͵�ʱ�����ĳ�0.678s
	5. 	ʹ������buff����TX2 ��߷���Ч��
	6. �޸��˰��⳵����״̬�´����������Ժ� ������˫�žͿ����е�bug
	7. ������ʾ ʹ��˫�ű���ʹ��˫��ͨѶ
	8. ����GPS���Ĳ��ԣ�ֻ��һ��test͸��ָ�
	9. �޸���͸�������к��쳣��bug
	

Firmware 1.2.01   	2016.06.17 16��40
	1. ��Flash��������16��byte
	2. ����EventPer1s�е�ѭ������ָ��
	3. ʹ��App���͵�ָ���޸�Flash����Ĳ���
	4. ��App������Mileage_Max���Ժ󷢸�MCU

Firmware 1.2.2   2016.07.13 16:11
	1. �����˻ָ��������ã�Ĭ�����ò������Ĺ���
	2. �޸�����Ϊ8���Ƶ��������� �ͻ�������
		Digital front plus 0b means it is binary
		Digital front plus 0 means it is octonary 
		Digital front plus 0x means it is hexadecimal
		Digital front plus none means it is decimal 
	3. ���Ӳ�ѯ�ͻ�������AT 
	4. �����˲�ѯ�ϵ硢˫�š�������״̬��ATCm
	5. ������GIN���˲�����
	6. ����������Ź� IWDOG
	7. ���Ӱ��⳵ �ϵ��Ժ������˫�ŵĴ���
	8. �޸İ��⳵�ϵ��ϵ粻�ɹ��Ĵ���
	9. ʹ�����ָ�ʽ����ͬһ������ uin32 
	10.ʹ��union�Ż�Flash�Ķ�ȡ��д��
	11. ����������ú� ��Ƭ�����Զ�����
	12. ����Flashд������ʩ���ش��� ������д������ѹ��������дFlash
	13. �޸���soc����100��������
	14. �Ż���BLE�����������е����� �ĳ��첽����
	15. �޸��˵�һ�δ����ϵ�� �������״̬ �綯���ܹ����е�bug
		
Firmware 1.2.3  	2016.07.22 12:30
	1.	ʹ�ýṹ��ָ�봦��PIN�����أ���ƽ�Ķ�ȡ���˲�
	2.	������400ms�˲� �޸��˷���������״̬�� ��ң����������ť ��������2�µ�bug	
	3. 	add three speed and three torque set 
	4.	fixed OnelineCommuca bug
    	Original Code unable to connect to controller
    5. 	Add _ParameterCal.h  Restructuring Customer files	
    6. 	repair the bug of GPS.CmdControl.num_waitforcmdresponse

Firmware 1.2.4 	
			
		
next version
	������𽥱仯  ʣ������𽥱仯
	��ָ������������˲�Ч��
	��ָ���������ʱ ��������100%
	ʣ������ڹرպ��15min�ڱ��ֲ�����߽��䡣
	���к󣬹رյ���������ѯʣ����� �ȴ򿪵�����ʱ����1km���ҡ�
	������Ѹ����������ѯʣ����� �ȴ򿪵�����ʱ����1km���ҡ�
	���Ǻ�ѹ���ʱ����Ϊ90%
*/













#endif