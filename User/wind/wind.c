#include "./usart/bsp_debug_usart.h"
#include "./rtc/bsp_rtc.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./wind/wind.h"

int Wind_state = Wind_SHOW;
int minutes;
int hours;
int Alarmmin;
int Alarmhour;

int Wind_RAX(int minutes)
{
    return minutes / 10 * 16 + minutes % 10;
}

void Wind_ChangeState()
{
	Wind_state = Wind_state==Wind_SHOW ? Wind_CHANGE : Wind_SHOW;
}

void Wind_ChangesIs(int *hours , int *minutes)
{
		RTC_TimeTypeDef RTC_TimeStructure;

		// 获取日历
		HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
		*hours = RTC_TimeStructure.Hours;
		*minutes = RTC_TimeStructure.Minutes;
}

void Wind_ChangeShow(int hours , int minutes)
{
		char LCDTemp[100];
			//液晶显示时间
			sprintf(LCDTemp,"%0.2d:%0.2d", 
			hours, 
			minutes);
	ILI9341_DisplayStringEx(0*48+5 ,0*48+5,125,125,( uint8_t *)LCDTemp,0);		
  ILI9341_DisplayStringEx( 5 ,4*48+20,20,20,(uint8_t *)"Time Set",0);	
	
}

void Wind_MinIn(int *minutes)
{
	if(*minutes  < 59)
		(*minutes)++;
	else 
      *minutes = 0;
}

void Wind_HourIn(int *hours)
{
	if(*hours < 23)
		(*hours)++;
	else
		*hours = 0;
}


void Wind_SetTime(int hours, int minutes)
{
	char LCDTemp[100];
	RTC_TimeTypeDef RTC_TimeStructure;
	 
	RTC_TimeStructure.Minutes = minutes;
  RTC_TimeStructure.Hours = hours;
	
	#ifdef _WIND_DEBUG_SETTIME_
	sprintf(LCDTemp,"%0.2d:%0.2d", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes);
	ILI9341_DisplayStringEx(0*48+5 ,2*48+5,125,125,(uint8_t *)LCDTemp,0);
	#endif
	RTC_TimeStructure.Minutes = Wind_RAX(minutes);
	RTC_TimeStructure.Hours = Wind_RAX(hours);
	
	HAL_RTC_SetTime(&Rtc_Handle,&RTC_TimeStructure, RTC_FORMAT_BCD);

	HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX,RTC_BKP_DATA);
	
}

void Wind_TimeShow()
{
	uint8_t Rtctmp=0;
	char LCDTemp[100];
	RTC_TimeTypeDef RTC_TimeStructure;

		// 获取日历
		HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
			
		// 每秒打印一次
		if(Rtctmp != RTC_TimeStructure.Seconds)
		{
			// 打印时间
			printf("The Time :  %0.2d:%0.2d \r\n\r\n", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes);
			
			//液晶显示时间
			sprintf(LCDTemp,"%0.2d:%0.2d", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes);
#ifdef USE_LCD_DISPLAY
//			ILI9341_DispStringLine_EN(LINE(5),LCDTemp);
		ILI9341_DisplayStringEx(0*48+5 ,0*48+5,125,125,(uint8_t *)LCDTemp,0);
#endif	
		}
		Rtctmp = RTC_TimeStructure.Seconds;
	
}


