#include "./usart/bsp_debug_usart.h"
#include "./rtc/bsp_rtc.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./wind/wind.h"
#include "./beep/bsp_beep.h"   
#include "./flash/bsp_spi_flash.h"

int Wind_state = Wind_SHOW;
int Wind_loc = Wind_HOUR;
int minutes;
int hours;
int Alarmmin;
int Alarmhour;
int AlarmCount;


/*------------------------------------------------------------------------------------------------
********************************* some functions **********************************
------------------------------------------------------------------------------------------------*/

void Wind_MinIn(int *minutes, int i)
{
    if(i == 1){
        if(*minutes  < 59)
            (*minutes)++;
        else 
            *minutes = 0;
    }
    else
    {
        if(*minutes > 0)
            (*minutes)--;
        else
            *minutes = 59;
    }
}

void Wind_ChangeState()
{
	Wind_state = Wind_state == Wind_ALARM ?  Wind_SHOW : Wind_state + 1;
}

void Wind_ChangeLoc(void)
{
        Wind_loc = Wind_loc == Wind_MIN ? Wind_HOUR : Wind_loc + 1;
}

/*--------------------------------------------------------------------------------------------------
**************************       the time set driver      *******************************     
---------------------------------------------------------------------------------------------------*/
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
    sprintf( LCDTemp,"%0.2d", hours );
    if(Wind_loc == Wind_HOUR)        
        ILI9341_DisplayStringEx(5 ,5,125,125,( uint8_t *)LCDTemp,1);
    else
        ILI9341_DisplayStringEx(5 ,5,125,125,( uint8_t *)LCDTemp,0);

    ILI9341_DisplayStringEx(5+125 ,5,125,125,( uint8_t *)":",0);
    
    sprintf( LCDTemp,"%0.2d", minutes );
    if(Wind_loc == Wind_HOUR)        
        ILI9341_DisplayStringEx(5 + 187,5,125,125,( uint8_t *)LCDTemp,0);
    else
        ILI9341_DisplayStringEx(5 + 187,5,125,125,( uint8_t *)LCDTemp,1);
    
    ILI9341_DisplayStringEx( 5 ,4*48+20,20,20,(uint8_t *)"Time Set   ",0);
    
    
	
}

void Wind_SetTime(int hours, int minutes)
{
		#ifdef _WIND_DEBUG_SETTIME_
	char LCDTemp[100];
		#endif
	
	RTC_TimeTypeDef RTC_TimeStructure;
	 
	RTC_TimeStructure.Minutes = minutes;
  RTC_TimeStructure.Hours = hours;
	
	#ifdef _WIND_DEBUG_SETTIME_
	sprintf(LCDTemp,"%0.2d:%0.2d", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes);
	ILI9341_DisplayStringEx(0*48+5 ,2*48+5,125,125,(uint8_t *)LCDTemp,0);
	#endif
	
//	RTC_TimeStructure.Minutes = Wind_RAX(minutes);
//	RTC_TimeStructure.Hours = Wind_RAX(hours);
	
	HAL_RTC_SetTime(&Rtc_Handle,&RTC_TimeStructure, RTC_FORMAT_BIN);

//	HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX,RTC_BKP_DATA);
	
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
			//液晶显示时间
			sprintf(LCDTemp,"%0.2d:%0.2d", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes);
#ifdef USE_LCD_DISPLAY
		ILI9341_DisplayStringEx(0*48+5 ,0*48+5,125,125,(uint8_t *)LCDTemp,0);
#endif	
		}
		Rtctmp = RTC_TimeStructure.Seconds;
		
		sprintf(LCDTemp,"%0.2d", 
			RTC_TimeStructure.Seconds);
		ILI9341_DisplayStringEx(0*48+5 ,3*48+5,20,20,(uint8_t *)LCDTemp,0);
		
		ILI9341_DisplayStringEx( 5 ,4*48+20,20,20,(uint8_t *)"Time view",0);
		
		if(Alarmflag == 1)
		{
			BEEP_ON;
			Alarmflag = 0;
		}
	
}

/*------------------------------------------------------------------------------------------------------
************************************** Alarm Set ****************************************
-------------------------------------------------------------------------------------------------------*/
void Wind_AlarmIs(int *Alarmhour , int *Alarmmin)
{
	 RTC_AlarmTypeDef sAlarm;
	
//	 HAL_RTCEx_BKUPRead(&Rtc_Handle, RTC_BKP_DRX);
	 HAL_RTC_GetAlarm(&Rtc_Handle, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
	 *Alarmhour = sAlarm.AlarmTime.Hours;
	 *Alarmmin = sAlarm.AlarmTime.Minutes;
}

void Wind_AlarmShow(int Alarmhour, int Alarmmin)
{
    char LCDTemp[100];
    sprintf( LCDTemp,"%0.2d", Alarmhour );
    if(Wind_loc == Wind_HOUR)        
        ILI9341_DisplayStringEx(5 ,5,125,125,( uint8_t *)LCDTemp,1);
    else
        ILI9341_DisplayStringEx(5 ,5,125,125,( uint8_t *)LCDTemp,0);

    ILI9341_DisplayStringEx(5+125 ,5,125,125,( uint8_t *)":",0);
    
    sprintf( LCDTemp,"%0.2d", Alarmmin );
    if(Wind_loc == Wind_HOUR)        
        ILI9341_DisplayStringEx(5 + 187,5,125,125,( uint8_t *)LCDTemp,0);
    else
        ILI9341_DisplayStringEx(5 + 187,5,125,125,( uint8_t *)LCDTemp,1);
    	
    
  ILI9341_DisplayStringEx( 5 ,4*48+20,20,20,(uint8_t *)"Alarm Set  ",0);	
}

void Wind_SetAlarm(int Alarmhour, int Alarmmin)
{
	RTC_AlarmTypeDef sAlarm;
	
	sAlarm.Alarm = RTC_ALARM_A;
	
	sAlarm.AlarmTime.Hours = Alarmhour;
	sAlarm.AlarmTime.Minutes = Alarmmin;
	sAlarm.AlarmTime.Seconds = 0x0U;
	
	HAL_RTC_SetAlarm_IT(&Rtc_Handle, &sAlarm, RTC_FORMAT_BIN);

//	HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX,RTC_BKP_DATA);
	
}

void test(int x, int y)
{
    char LCDTemp[100];
			//液晶显示时间
			sprintf(LCDTemp,"%0.2d:%0.2d", 
			x, 
			y);
	ILI9341_DisplayStringEx(3*48+5 ,3*48+5,24,24,( uint8_t *)LCDTemp,0);
}

uint8_t Wind_WriteAlarm(int *x)
{
    uint8_t output;
    SPI_FLASH_SectorErase(ADDR_FLASH_PAGE_30);
    SPI_FLASH_BufferWrite((uint8_t*)x, ADDR_FLASH_PAGE_30, (uint16_t)sizeof(int));
    SPI_FLASH_BufferRead(&output,ADDR_FLASH_PAGE_30, (uint16_t)sizeof(int));
    return output;
}

uint8_t Wind_ReadAlarm(void)
{
    uint8_t output;
    SPI_FLASH_BufferRead(&output,ADDR_FLASH_PAGE_30, (uint16_t)sizeof(int));
    return output;
}










