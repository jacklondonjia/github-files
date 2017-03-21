#ifndef __UPGRADEHISTORY_H
#define __UPGRADEHISTORY_H

/*
Firmware 1.0.1  2015.08.13 15:41	Tested
	Add DetectConnectWithConctroller 、 PCBVersion And FirmwareVersion Cmd

Firmware 1.0.2  2015.08.17 12:53	Tested
	1.Get BLEAddress，Put this message into BLEAdvertising
		Attention：发现购买的蓝牙小板有软件上的bug ，比如在蓝牙断开连接时发送查询蓝牙地址的AT命令
					应该是20,C3,8F,FC,FD,D5 /r/n 	实际上接收到的是20一帧 隔9ms 收到第一个逗号 
					再隔8ms才接收到剩余其他数据。
Firmware 1.0.3
	1.Add　Typedef Level
	2.modify FirmwareVersion
		
Firmware 1.0.4
	1.Add ATCmd: AT+Deadline=?
		查询MCU中保存的租期还有多长时间，精确到分钟	
	2.modify FirmwareVersion

Firmware 1.0.5
	1.Modify ATCmd: AT+GuardStatus=?
		1byte防盗状态+1byte上锁原因
	2.modify FirmwareVersionCheck
	3.Optimization GuardControl
	
Firmware 1.0.9
	1.Never Set	Adver Of HC08
	2.Add #warning when use test code 
	3.Add #warning to remind mode in Annotate.h
		
Firmware 1.0.0A
	1.When BLE isn't disconnect with app，unguard the bike after Power to controller
	2.Add log in iar		
		
Firmware 1.0.0B
	1.Optimization"When BLE isn't disconnect with app，unguard the bike after Power to controller"
	 	在上一个版本中，有以下bug：关闭蓝牙连接后，插上钥匙，电动车会自动解锁		

Firmware 1.0.0C
	1.Reconstruction level detection code
	2.Add Mileage Cal
		总里程参考1.1.12 按照上一次霍尔累计次数和soc消耗量 计算总里程 作为次要参考
		剩余里程按照
		剩余里程由两个加权部分组成，一部分是按照当时的电流和霍尔数计算出的mileage
			另一部分是按照本次运行总数的电流消耗和霍尔数计算处的mileage。
		attention：需要处理计算过程的0为分母的情况
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
		先配置USART_DMACmd，配置完usart 再启动DMA方式，DMA_Cmd (DMA1_Channel4,ENABLE);
	2.  Add BLE connect repeat Test		
	3. if(True == CmpStrWithFreeATCmd(Str_ATERRORSTATUS,Strlen_ATERRORSTATUS)) 
		last version : p= ResponseErrorATCmdConnectedToMCU;
		this version : p= ResponseATErrorStatus;
	4. 32位的乘除法 先做除法然后再做乘法，放置溢出 导致值很小
	5. 增加电池电压的条件编译提醒
		
Firmware 1.0.0F
	1. App add time out control ，so mcu add guardCmd response
	2. Add the pull-down resistor Of YXT 
		芯片GPIO内部上/下拉电阻 min = 20k ，typ= 30k ，max = 40k 。
	3. Modify LV 
		同名的结构体不能用在局部变量
	4. Add Inquire Mileag Remainder when charging
		非连接状态 查询剩余里程。
	5. Make sure not updata Qintial When Inquire Mileag Remainder
		确认查询剩余里程的时候不更新Qintial，防止频繁更改QInital导致总里程计算不准确
	6. solve  the problem that when MCU reset mileagemax equal 0
		因为重新写入flash时，未写入mileage max的值
	7. solve the problem that soc equal ff or fe
		因为SOC的计算中有除法，因此小数可能会被忽略，导致减法后出现负值。会溢出
		另外在非#define的代码中，不要加入浮点计算	

		
Firmware 1.0.10
	1. Modify Hall Speed Cal
		因为控制器计算有HallSpeed有余数（按照比例计算出），电机静止时hall速度是0x12 
		因此设定hallspeed最小值，让hallspeed小于0x15时，显示为0
	2. Modify Mileage Remander Cal
   		原来的电量是电压*电流，而计算剩余里程的时候只考虑了电流没有考虑电压，导致剩余里程值很大
	3. Modify message storage and process
		优化指定版接收完一帧数据后进行的信息存储和处理
		
*******************************************************************************
Firmware 1.1.2 2015.08.19 16:23		Tested
	1.Control Guard Level Without Our Controller
		Attention：为了减少租车店商家的更新设备成本 特推出该版本 
					该版本可以不使用东大蓝鸟的控制器，只是对控制器的防盗输入信号进行控制
					用输出给霍尔的5V电源来判断电门锁是否已经上电
	2. 把全局变量提取到单独的_Parameter.h文件中

Firmware 1.1.3
	1.Add Hall count
		Attention：为了将来拓展不配套使用东大蓝鸟控制器的蓝牙模块比较精准的计算速度
					以及根据电量计算剩余里程。
					特意在霍尔的6孔线束上引出一根霍尔线到蓝牙的KEY1口。
	2.Add　Typedef Level
	3.modify FirmwareVersion
		
Firmware 1.1.4
	1.Add ATCmd: AT+Deadline=?
		查询MCU中保存的租期还有多长时间，精确到分钟	
	2.modify FirmwareVersion

Firmware 1.1.5
	1.Modify ATCmd: AT+GuardStatus=?
		1byte防盗状态+1byte上锁原因
	2.modify FirmwareVersionCheck
	3.Optimization GuardControl

Firmware 1.1.6
	1.Optimization AdverMode
			When CONT=0 HC-08中的广播信息可能会被中断打断 导致广播信息被清零 
		后果是出现模块运行1-2天后 发现苹果设备无法搜索到对应的广播信息
			现在采用的方式是在非APP连接状态 没过10min 重新写入一次广播信息
				
Firmware 1.1.7
	1.rewrite AdverData of HC08 after rename HC08
	2.Optimization IsConnectedWithBLEApp()
		让连接上蓝牙2s内不再检测是否已经连接上 防止因为信号灯延时导致的蓝牙连接上又断开的问题

Firmware 1.1.8
	1.Add HallSpeed To CyclicSend Data
	2.rewrite AdverData of HC08 every 1min

Firmware 1.1.9
	1.Never Set	Adver Of HC08
	2.Add #warning when use test code 
	3.Add #warning to remind mode in Annotate.h

Firmware 1.1.0A
	1.When BLE isn't disconnect with app，unguard the bike after Power to controller
	2.Add log in iar 
				
Firmware 1.1.0B
	1.Optimization"When BLE isn't disconnect with app，unguard the bike after Power to controller"
	 	在上一个版本中，有以下bug：关闭蓝牙连接后，插上钥匙，电动车会自动解锁	
			
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
		先配置USART_DMACmd，配置完usart 再启动DMA方式，DMA_Cmd (DMA1_Channel4,ENABLE);
	2.  Add BLE connect repeat Test	
	3. change INT16_MAX To UINT16_MAX ,INT8_MAX To UINT8_MAX
	4. if(True == CmpStrWithFreeATCmd(Str_ATERRORSTATUS,Strlen_ATERRORSTATUS)) 
		last version : p= ResponseErrorATCmdConnectedToMCU;
		this version : p= ResponseATErrorStatus;
 	5. 增加计算出的总里程为0时 不写入flash
	6. 32位的乘除法 先做除法然后再做乘法，放置溢出 导致值很小
	7. 增加电池电压的条件编译提醒

Firmware 1.1.0F
	1. App add time out control ，so mcu add guardCmd response
	2. Add the pull-down resistor Of YXT 
		芯片GPIO内部上/下拉电阻 min = 20k ，typ= 30k ，max = 40k 。
	3. Modify IsConnectWithController()
		非东大指定版控制器 有时充电和正常骑行时，上电不受控制。即使芯片YXT加了下拉，也会有这样的问题。
		现在给5V判断加滤波	
	4. Modify LV 
		同名的结构体不能用在局部变量
	5. Add Inquire Mileag Remainder when charging
		非连接状态 查询剩余里程。
	6. Make sure not updata Qintial When Inquire Mileag Remainder
		确认查询剩余里程的时候不更新Qintial，防止频繁更改QInital导致总里程计算不准确
	7. solve  the problem that when MCU reset mileagemax equal 0
		因为重新写入flash时，未写入mileage max的值
	8. solve the problem that soc equal ff or fe
		因为SOC的计算中有除法，因此小数可能会被忽略，导致减法后出现负值。会溢出
		另外在非#define的代码中，不要加入浮点计算


Firmware 1.1.10
	1. Modify Analyzing conditions of Status_ReachedDeadline_Park_NoExceed To Status_ReachedDeadline_Park_Exceed
		原本的判断条件只适用于东大指定版的控制器,现在的支持两个版本
		修改后把guard.status_park放置到了IsConnectWithController()
	2. cal mileage of this trip	
		计算本次行驶里程

Firmware 1.1.11
	1. Shorten the time of the guard need after the controller power down
		原来钥匙断开以后需要4s左右，现在调整
	通用版上锁时间= 控制器掉电时间 + 0.3s  
	指定版上锁时间 = 控制器掉电时间 + 检测不到开头的两个字节。
	2.	The optional of automatically locked
		增加BLECmd：AT+
			
Firmware 1.2.12
	1. Add Two-way Communicate with controller
		增加和控制器的双向通讯

Firmware 1.2.13
	1. Modify Electronic lock Switch
		增加电子锁开关 使电动车能够去除自动上锁状态

Firmware 1.2.14
	1. Add second in deadline
		回复给app的关于租期时间的AT命令，精确到s 
			
Firmware 1.1.15
	1. Modify ErrorStatus Response
		把一键通上面获取到的故障信息处理后发给app 
        假如有两个或者以上故障，app能同时接收到

Firmware 1.1.16   2015.12.03
	1. Add DevInfo ATCmd
                版本控制只负责处理代码升级，不再反映通用版、双向控制、一线通通讯等信息
                双向控制等信息存储在DevInfo中。

Firmware 1.0.17   2015.12.04 09：22——2015.12.07
	1. Modify ATCmd
		重新构建ATCmd表，整合功能类似的ATCmd。大面积修改ATcontrol.c
    2. Modify Code
                部分进行重构
	3. Add ATCmd LVLevel
		双向控制版本的双欠压设置功能

Firmware 1.0.18   2015.12.07 10：50——2015.12.08 12:59
	1. Modify ATCmd Rename HC08
		修改HC-08命令：接收到app发来的信息以后，先回复0X0A，app断开蓝牙连接以后，给hc-08模块写入新的名称
	2. Add ErrorHistory
		增加了ErrorHistory的搜集和处理

Firmware 1.0.19   2015.12.08 12:59——2015.12.09 14:00
	1. Modify Charge Detect
		使用新的算法计算电动车是否在充电：
			当检测到电动车停止运行的时候，每隔段时间计算soc， 如果此时的soc比上次运行计算出的剩余的soc高5% 
			就认为电动车在充电。
		判断电动车是否停止运行有以下方式;
			所有版：判断霍尔是否变化 （这样好判断）
    2. Modify ErrorATCmd Response
        在比较字符串的时候，直接把接收信息标志清除。

Firmware 1.0.1A   2015.12.09 14:00——  2015.12.10 15:01
	1. Modify Q And Mileage Cal
		把连接控制器、电动车充电、和驻车时间等信息分别剥离出，移到Per500ms的处理中。
	2. Modify DMA Send
		优化发送命令，上一条完整回复给app以后 才会发送下一条response
	3. Remove SendATCmdResponseCode()
		把发送单个字节的代码全部用发送更长字节的代码代替
	4. Modify Initial And Remainder Of Q And Mileage
		 优化Q和Mileage的 Initial和Remainder

Firmware 1.0.1B   2015.12.10 15:01
	1. Modify DMA Transfer
		关闭原来的发送结束的中断。每次发送ATCmd的时候，先检查是否有上一条ATCmd的发送结束标志
		因此，第一条发送AT 回复OK	
	2. Modify ATCmdSet Format Error Judgement
		把ATCMdSet的格式错误的判断提取出来
	3. Slove the problem of array overflow
		解决程序跑飞问题 原因是ErrorHistory中的数组设置超出了声明中的设定，导致数组溢出
	4. Slove the problem of Setting charged flag
		解决未充电时充电标志位持续被置位的情况。原因是：if（）之后误加了一个";"
	5. Slove the problem of lock the motor immediately when deadline arrived
		解决到了限期，立即锁电机的问题。原因是：GetHallSpeed中的有一个判断应该是"!=" 写成了"=="

Firmware 1.0.1C   2015.12.29 15:01
	1. 修正60V的LV分压电阻值
	2. 增加220V交流电源测试版
	3. 优化了Q_MAX_20AH设定的计算
	4. 应该先算乘法再算除法 否则会不对
	5. 测试剩余里程为0的原因
		找到计算乘法出错的原因了 应该先把32bit赋值给64bit 再做乘法计算
	6. 开启tx发送结束的中断 发送结束以后隔一段时间才能开始新的dma.
	7. 修改为额定电流和额定速度 单次测试ok 但是45分钟以后更新的电量正确 但是里程不对
	8. 提高soc的精度


Firmware 1.0.1D   2016.01.11 15:01
	1. 剩余里程等于原剩余里程减去本次里程
	2.计算总电量
	3.关闭Qamx的计算
	4.增加循环发送
	5. 增加AT命令 寻车时蜂鸣器响5下
	6. 根据能量重新计算曲线
	7. 补偿移位0.678s前后电压逐渐变低导致的误差
	8.解决了上电电量不正常的现象
	9. 把hall额定速度提取出来
	

Firmware 1.0.1E   2016.01.12 15:01
	1. 把电子锁保存到flash中去
	2. 解决电子锁初始状态的问题

Firmware 1.0.1F   2016.01.19 15:01
	1.增加测试电机锁龙头锁和座桶锁的AT命令
	2.提取出电平pin脚定义 并且把发送放到主代码中

Firmware 1.0.20   2016.01.20 15:01
	1. 修改预编译的情况 把里程提取出来
	2. 厂家版本默认租期无穷大 默认解锁状态

Firmware 1.0.21   2016.03.14 15:01
	1. 测试BLE-G 控制器版的所有功能

Firmware 1.0.22   2016.04.05 15:01
	1. 测试BLE-H 控制器版的所有功能
	2 .增加电机锁
	3. 增加上电断电输出功能
	4. 增加鞍座锁
	5. 增加上电断电的初始状态
	6. 电机机械锁解锁后 电机才能转动
	7. 机械锁及双撑动作结束后给app的回复
	8. 鞍座锁不能在行驶状态打开
	9. 鞍座锁频繁动作增加限制
	10. 处理不断电座位解锁 电机卡死的问题

Firmware 1.0.23   2016.04.05 15:01
	1. 双向通讯 雅迪用 增加一键通
	2. 解决上电不能骑行的问题

Firmware 1.0.24   2016.04.21 15:01
	1. 测试BLE-J GPIO口基本配置好
	2. 增加输出大电流中断及电机反复动作的保护
	3. 免钥匙输出大电流中断测试ok
	4. 刹车解锁
	5. 双撑测试 发送码值测试ok
	6. 增加双撑命令
	7. 已经能用命令测试双撑了 但是返回的码值 毛刺太多
	8. 测试驱动逻辑 逻辑通过
	9. 增加轮动检测及上电 轮动无反应后的断电 测试ok
	10. 把双撑信息写入Flash 
	11. 解决免钥匙意外断电的情景
	12. 修改读取租期错误的bug
	13. 去除双撑状态保护
	14. 增加双撑动作延时
	15. 修改成双向通讯
	16. 解决了无需刹车就解锁的bug
	17. 测试大电流中断的报警 修改硬件解决
	18. 检测输出48v ADC采样
	19. 写输出电压检测的逻辑
	20. 增加上电或者断电失败的判断
	21. 减少双撑动作1.5s 

Firmware 1.0.25   2016.04.29 15:01
	1. 完成48V的电量计算
	2. 未启用大电流保护
	3. 把客户档案提取出来
	4. 大电流时 发送3次大电流中断信号
	5. IsDeadLineReachInFlash改成IsDeadLineReach
	6. 修改Flash中的初始时间 不能全是F 因为小时和分钟最大值不是FF

Firmware 1.0.26   2016.05.04 15:01
	1. 把include放到h文件中
  
Firmware 1.0.27   2016.05.10 15:01
	1. 把电流电压值放到循环发送中
	2. 0.678s发送一次循环 
	3. 把霍尔速度放到循环中 
	4. 把霍尔的速度不用平滑移动
	5. 生成GPRS-E的客户档案

Firmware 1.0.28   2016.05.13 08:54
	1. 修改FlashWrite的代码 解决断电后再上电 租期不正确的问题
	2. 新增新日邮政车的客户档案
	3. 新增金彭邮政车的客户档案
	4. X联盟新的客户档案
	5. 大陆鸽邮政三轮车的客户档案
	6. 增加220V的测试客户档案
	7. 增加小飞哥二轮车客户档案
	8. 把升级历史分离出.h

Firmware 1.0.29   2016.05.26 08:54
	1.	把版本号放到customerpara.h中
	2. 在循环发送中增加电压、电流的实时信息 循环时间是30s
		

Firmware 1.0.2A  2016.06.07 15：55
	1. 把test的代码用条件编译提取出来
	2. 增加usart2的DMA传输
	3. 增加GPSControl的.c 和.h文件
	4. 把循环发送的时间间隔改成0.678s
	5. 	使用两个buff发送TX2 提高发送效率
	6. 修改了爱租车防盗状态下触发阻力锁以后 无需解除双撑就可骑行的bug
	7. 增加提示 使用双撑必须使用双向通讯
	8. 满足GPS方的测试（只有一条test透传指令）
	9. 修复了透传的序列号异常的bug
	

Firmware 1.2.01   	2016.06.17 16：40
	1. 在Flash里面增加16个byte
	2. 屏蔽EventPer1s中的循环发送指令
	3. 使用App发送的指令修改Flash里面的参数
	4. 让App计算完Mileage_Max完以后发给MCU

Firmware 1.2.2   2016.07.13 16:11
	1. 增加了恢复出厂设置（默认配置参数）的功能
	2. 修改误认为8进制的条件编译 客户档案号
		Digital front plus 0b means it is binary
		Digital front plus 0 means it is octonary 
		Digital front plus 0x means it is hexadecimal
		Digital front plus none means it is decimal 
	3. 增加查询客户档案的AT 
	4. 增加了查询上电、双撑、鞍座锁状态的ATCm
	5. 增加了GIN的滤波处理
	6. 增加软件看门狗 IWDOG
	7. 增加爱租车 断电以后才能上双撑的代码
	8. 修改爱租车断电上电不成功的代码
	9. 使用三种格式共享同一个内容 uin32 
	10.使用union优化Flash的读取和写入
	11. 增加软件设置后 单片机的自动重启
	12. 增加Flash写保护的施密特触发 不会在写保护电压附近反复写Flash
	13. 修复了soc超过100溢出的情况
	14. 优化了BLE重命名过程中的死等 改成异步处理
	15. 修复了第一次代码上电后 进入防盗状态 电动车能够骑行的bug
		
Firmware 1.2.3  	2016.07.22 12:30
	1.	使用结构体指针处理PIN（开关）电平的读取和滤波
	2.	设置了400ms滤波 修复了防盗器上锁状态下 按遥控器上锁按钮 蜂鸣器响2下的bug	
	3. 	add three speed and three torque set 
	4.	fixed OnelineCommuca bug
    	Original Code unable to connect to controller
    5. 	Add _ParameterCal.h  Restructuring Customer files	
    6. 	repair the bug of GPS.CmdControl.num_waitforcmdresponse

Firmware 1.2.4 	
			
		
next version
	总里程逐渐变化  剩余里程逐渐变化
	非指定版霍尔增加滤波效果
	非指定版充满电时 电量不是100%
	剩余里程在关闭后的15min内保持不变或者渐变。
	骑行后，关闭电门锁，查询剩余里程 比打开电门锁时少了1km左右。
	充电后，有迅速上升，查询剩余里程 比打开电门锁时高了1km左右。
	考虑恒压充电时电量为90%
*/













#endif