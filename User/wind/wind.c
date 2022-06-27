#include "./usart/bsp_debug_usart.h"
#include "./rtc/bsp_rtc.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./wind/wind.h"

void Wind_DateIn()
{
	RTC_DateTypeDef sdatestructure;
	RTC_TimeTypeDef stimestructure;
	
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	// 获取日历
	HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStructure, RTC_FORMAT_BIN);
	
	 sdatestructure = RTC_DateStructure;
	 stimestructure = RTC_TimeStructure;

	sdatestructure.Date += 0x02U;
	
//	HAL_RTC_SetTime(&Rtc_Handle,&stimestructure, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&Rtc_Handle,&sdatestructure,RTC_FORMAT_BCD);
	
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


