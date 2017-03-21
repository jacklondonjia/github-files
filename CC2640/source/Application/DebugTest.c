
#include "CustomerPara.h"

volatile uint8_t  test1 ;
volatile unsigned int test2 ;
volatile unsigned int test3 ;


/****************************************************/
/************************function********************/
#if 0
/*******************************************************************************
* 函数名        : 
* 函数描述    	: 
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
void DetectCommuClutterAndOutput(GPIO_TypeDef* GPIOsource, uint16_t GPIO_Pinsource, GPIO_TypeDef* GPIOtarget, uint16_t GPIO_Pintarget)
{
	/* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOsource));
  assert_param(IS_GPIO_PIN(GPIO_Pinsource));
  assert_param(IS_GPIO_ALL_PERIPH(GPIOtarget));
  assert_param(IS_GPIO_PIN(GPIO_Pintarget));
  
  if(GPIOsource->IDR & GPIO_Pinsource)
	  GPIO_SetBits(GPIOtarget,GPIO_Pintarget);
  else
 		GPIO_ResetBits(GPIOtarget,GPIO_Pintarget);
}
#endif


