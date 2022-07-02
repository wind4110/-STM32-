
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
#include "./irda/bsp_irda.h"
#include "qian_head.h"
#include "./systick/bsp_systick.h"
#include "./beep/bsp_beep.h"

extern uint8_t  frame_flag;
extern uint8_t  isr_cnt;
extern uint8_t  frame_cnt;

#define CLI() __set_PRIMASK(1)		        /* 关闭总中断 */  
#define SEI() __set_PRIMASK(0)				/* 开放总中断 */ 

extern uint8_t  frame_flag;
extern uint8_t  isr_cnt;
extern uint8_t  frame_cnt;

extern int detime,rtime;

int detime,rtime;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
	uint8_t key_val;
    //int runled = 0;
	
  /* LED 初始化 */
	LED_GPIO_Config();
	LED1_ON;
	
    /* 蜂鸣器初始化 */
    BEEP_GPIO_Config();    
//       
    /* flash初始化 */
    SPI_FLASH_Init();
  
	/* 配置SysTick 为10us中断一次 */
	SysTick_Init();
	
  /* 重新配置SysTick的中断优先级为最高，要不然SysTick延时中断抢占不了IO EXTI中断
   * 因为SysTick初始化时默认配置的优先级是最低的
   * 或者当你用其他定时器做延时的时候，要配置定时器的优先级高于IO EXTI中断的优先级
   */
  NVIC_SetPriority (SysTick_IRQn, 0);
  
  
  /* USART1 config 115200 8-N-1 */
//	USART_Config();
	printf("\r\n 这是一个红外遥控发射与接收实验 \r\n");
  
  /* 初始化红外接收头CP1838用到的IO */
	IrDa_Init();  
	
	
  /*=========================液晶初始化开始===============================*/
	ILI9341_Init();         //LCD 初始化

	
	//其中0、3、5、6 模式适合从左至右显示文字，
	//不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
	//其中 6 模式为大部分液晶例程的默认显示方向  
  ILI9341_GramScan ( 3 );
  /*=========================液晶初始化结束===============================*/
  
  /*##-1- Configure the RTC peripheral #######################################*/
  /*配置RTC预分频器和RTC数据寄存器 */
  /*Asynch Prediv  = 由HAL自动计算 */
  Rtc_Handle.Instance = RTC; 
  Rtc_Handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;

  if (HAL_RTC_Init(&Rtc_Handle) != HAL_OK)
  {
    /* Initialization Error */
    printf("\r\n RTC初始化失败\r\n");;
  }
  else
	{	
		/* 检查是否电源复位 */
		if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
		{
		  printf("\r\n 发生电源复位....\r\n");
		}
		/* 检查是否外部复位 */
		else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
		{
		  printf("\r\n 发生外部复位....\r\n");
		}
		/* Clear source Reset Flag */
        __HAL_RCC_CLEAR_RESET_FLAGS();
	} 
  
  
	LCD_SetFont(&Font8x16);
	LCD_SetTextColor(WHITE);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
    
        /*alarm init*/
    Wind_ReadAlarm(&Alarmhour,&Alarmmin);
//    Alarmhour = 1;
//    Alarmmin = 13;
    
    RTC_AlarmSet();
	
//    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    
    		/*init the date and time.*/
//    RTC_CalendarConfig();

	for(;;)
	{
        
        if( frame_flag == 1 ) /* 一帧红外数据接收完成 */
        {
            key_val = IrDa_Process();
            printf("\r\n key_val=%d \r\n",key_val);
            printf("\r\n 按键次数frame_cnt=%d \r\n",frame_cnt);
            printf("\r\n 中断次数isr_cnt=%d \r\1  n",isr_cnt);
            
            
			Alarmflag = 0;
            
            if( key_val == 162 )
                Wind_state = Wind_state == Wind_OFF ? Wind_SHOW : Wind_OFF;
            
            if( Wind_state != Wind_OFF){ 
            
            /* 不同的遥控器面板对应不同的键值，需要实际测量 */
            switch( key_val )
            {
                case 0:  
                    //LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n Error \r\n");
                    break;
                    
                case 162:
                    //LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n POWER \r\n");
                    break;
                    
                case 226:
                    //LED1_TOGGLE;
                    //runled = !runled;
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n MENU \r\n");
                    break;
                
                case 34:
                    //LED1_TOGGLE;
                    Wind_alarmstate = Wind_alarmstate == 0 ? 1 : 0;
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n TEST \r\n");
                    break;
                
                
            /********* “+”键  **********/
                case 2:
                    //LED3_TOGGLE;
                    if (Wind_state == Wind_CHANGE)
                    {                      
                        if(Wind_loc == Wind_HOUR)
                            Wind_HourIn(&hours,1);
                        else
                            Wind_MinIn(&minutes,1);
                        Wind_SetTime(hours,minutes);
                        Wind_ChangesIs(&hours , &minutes);
                    }
                    else if ( Wind_state == Wind_ALARM )
                    {       
                        if(Wind_loc == Wind_HOUR)
                            Wind_HourIn(&Alarmhour,1);
                        else
                            Wind_MinIn(&Alarmmin,1);
                        /*设置闹钟*/
                        Wind_SetAlarm(Alarmhour,Alarmmin);                            
                    }
                    
    
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n + \r\n");
                    break;
                
                case 194:
                    //LED1_TOGGLE;
                    Wind_state = Wind_state == Wind_SHOW ?  Wind_ALARM : Wind_state - 1;
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n RETURN \r\n");
                    break;
                
                case 224:
                    //LED1_TOGGLE;
                    
                    Wind_ChangeLoc();
                    
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n |<< \r\n");
                    break;
             
                
                
                /*播放键*/
                case 168:
                    //LED2_TOGGLE;
                    Wind_ChangeState();
                    printf("\r\n key_val=%d \r\n",key_val);
                    printf("\r\n > \r\n");
                    break;
                
                case 144:
                  //LED1_TOGGLE;
                  
                  Wind_ChangeLoc();
                
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n >>| \r\n");
                break;
                
                case 104:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 0 \r\n");
                break;
                
                /*“-”键*/
                case 152:
                  //LED1_TOGGLE;
                if (Wind_state == Wind_CHANGE)
                    {                      
                        if(Wind_loc == Wind_HOUR)
                            Wind_HourIn(&hours,0);
                        else
                            Wind_MinIn(&minutes,0);
                        Wind_SetTime(hours,minutes);
                        Wind_ChangesIs(&hours , &minutes);
                    }
                    else if ( Wind_state == Wind_ALARM )
                    {       
                        if(Wind_loc == Wind_HOUR)
                            Wind_HourIn(&Alarmhour,0);
                        else
                            Wind_MinIn(&Alarmmin,0);
                                       
                        /*设置闹钟*/
                        Wind_SetAlarm(Alarmhour,Alarmmin);                            
                    }
                
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n - \r\n");
                break;
                
                case 176:
                  //LED1_TOGGLE;
                if (Wind_state == Wind_CHANGE)
                    {                      
                        if(Wind_loc == Wind_HOUR)
                            hours = 0;
                        else
                            minutes = 0;
                        Wind_SetTime(hours,minutes);
                        Wind_ChangesIs(&hours , &minutes);
                    }
                    else if ( Wind_state == Wind_ALARM )
                    {       
                        if(Wind_loc == Wind_HOUR)
                            Alarmhour = 0;
                        else
                            Alarmmin = 0;
                                       
                        /*设置闹钟*/
                        Wind_SetAlarm(Alarmhour,Alarmmin);                            
                    }
                
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n C \r\n");
                break;
                
                case 48:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 1 \r\n");
                break;
                
                case 24:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 2 \r\n");
                break;
                
                case 122:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 3 \r\n");
                break;
                
                case 16:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 4 \r\n");
                break;
                
                case 56:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 5 \r\n");
                break;
                
                case 90:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 6 \r\n");
                break;
                
                  case 66:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 7 \r\n");
                break;
                
                case 74:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 8 \r\n");
                break;
                
                case 82:
                  //LED1_TOGGLE;
                  printf("\r\n key_val=%d \r\n",key_val);
                  printf("\r\n 9 \r\n");
                break;
                
                default:       
                break;
            }
            
            if((num = Wind_IsNum(key_val)) != -1)
            {
                Wind_Num();
            }
            }
         
        }
        
        /*时间显示模式*/
		if( Wind_state == Wind_SHOW)
		{
           Wind_TimeShow();
		}
        
        /*时间设定模式*/
		else if (Wind_state == Wind_CHANGE)
		{
			Wind_ChangesIs(&hours , &minutes);
			Wind_ChangeShow(hours,minutes);
		}
        
        /*闹钟设定模式*/
		else if ( Wind_state == Wind_ALARM )
		{
			Wind_AlarmShow(Alarmhour,Alarmmin);
                 /*把闹钟写入flash*/
            Wind_WriteAlarm(&Alarmhour, &Alarmmin);  
//            test(1,AlarmCount);
		}
        else if(Wind_state == Wind_OFF)
        {
            ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
        }            
         

        if(Alarmflag == 1)
        {
            BEEP_ON;
            SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
            LED_RED;
            detime = 20000;
            while(detime){};
            LED_BLUE;
            detime = 10000;
            while(detime);
        }
        else 
        {
            SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
            LED_RGBOFF;
            BEEP_OFF;
        }

        
        if(Wind_state != Wind_OFF){
        if(Wind_alarmstate == 0)
        {
            HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
            ILI9341_DisplayStringEx( 4*48+5 ,4*48+20,20,20,(uint8_t *)"       ",0);
        }
        else
        {
            HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
            ILI9341_DisplayStringEx( 4*48+5 ,4*48+20,20,20,(uint8_t *)"Alarmed",0);
        }
    }
        
    }
}
/*********************************************END OF FILE**********************/

