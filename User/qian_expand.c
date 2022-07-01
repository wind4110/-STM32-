#include "qian_head.h"
#include "bsp_led.h"
#include "./systick/bsp_systick.h"

void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
	
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
//	LED_GREEN;
//	Delay_ms(10);
//	LED_RGBOFF;
}

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  static u32 flag;
	
//	if(flag != (GPIOx->IDR )){
//		LED1_TOGGLE;
//	}
//	else {
//		LED1_OFF;
//		LED2_OFF;
//		LED3_OFF;
//	}
	
//	flag = GPIOx->IDR ;
	
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
	
	//LED_PURPLE;
	
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
//		LED_PURPLE;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
//		LED_GREEN;
  }
  return bitstatus;
}

void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
		//LED_PURPLE;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
		//LED_YELLOW;
	}
	
}
