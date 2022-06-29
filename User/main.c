/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103 STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include "./usart/bsp_debug_usart.h"
#include <stdlib.h>
#include "./led/bsp_led.h" 
#include "./lcd/bsp_ili9341_lcd.h"
#include "./flash/bsp_spi_flash.h"
#include "./RTC/bsp_rtc.h"
#include ".\key\bsp_key.h" 
#include "./wind/wind.h"
#include "./beep/bsp_beep.h"   
#include "./myfonts/myfonts.h"


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
	
int main(void)
{
  /* ϵͳʱ�ӳ�ʼ����72MHz */
  SystemClock_Config();
  /* LED �˿ڳ�ʼ�� */
  LED_GPIO_Config();
 /* ���ڳ�ʼ�� */
  DEBUG_USART_Config();
	
  BEEP_GPIO_Config();

  printf("\n\r����һ��RTC����ʵ�� \r\n");

#ifdef USE_LCD_DISPLAY	
	/*=========================Һ����ʼ����ʼ===============================*/
	ILI9341_Init();         //LCD ��ʼ��

	//����0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
	//���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
	//���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����  
  ILI9341_GramScan ( 3 );
  /*=========================Һ����ʼ������===============================*/
#endif	
 /*##-1- Configure the RTC peripheral #######################################*/
  /*����RTCԤ��Ƶ����RTC���ݼĴ��� */
  /*Asynch Prediv  = ��HAL�Զ����� */
  Rtc_Handle.Instance = RTC; 
  Rtc_Handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;

	 if (HAL_RTC_Init(&Rtc_Handle) != HAL_OK)
  {
    /* Initialization Error */
    printf("\r\n RTC��ʼ��ʧ��\r\n");
  }
  else
	{
		/*init the date and time.*/
    /*RTC_CalendarConfig();*/
		
		/* ����Ƿ��Դ��λ */
		if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
		{
		  printf("\r\n ������Դ��λ....\r\n");
		}
		/* ����Ƿ��ⲿ��λ */
		else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
		{
		  printf("\r\n �����ⲿ��λ....\r\n");
		}
		/* Clear source Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();
	} 
	
	/*key init*/
	
	 Key_GPIO_Config();
	
	/*init*/
	
	LCD_SetFont(&Font24x32);
	LCD_SetTextColor(WHITE);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
    
    /*alarm init*/
    Wind_AlarmIs(&Alarmhour , &Alarmmin);
    RTC_AlarmSet();
    
    
    
/*----------------------------------------------------------------------    
************************ reaction program*******************************
----------------------------------------------------------------------*/
    
    
	while(1){	
		
		if(Wind_state == Wind_SHOW)
		{
		   LED_RED;
           Wind_TimeShow();
		}
		else if (Wind_state == Wind_CHANGE)
		{
		    LED_GREEN;
			  if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON){
				    Wind_MinIn(&minutes);
						//Wind_HourIn(&hours);
				    Wind_SetTime(hours,minutes);
		     }
				Wind_ChangesIs(&hours , &minutes);
				Wind_ChangeShow(hours,minutes);
		}
		else if ( Wind_state == Wind_ALARM)
		{
			LED_BLUE;
			 if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON){
// 				        Wind_MinIn(&Alarmmin);
						Wind_HourIn(&Alarmhour);
				    Wind_SetAlarm(Alarmhour,Alarmmin);
		    }
			Wind_AlarmIs(&Alarmhour , &Alarmmin);
			Wind_AlarmShow(Alarmhour,Alarmmin);
		}
		
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON ){
        Wind_ChangeState();
		}
	}
}

	



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 2
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

