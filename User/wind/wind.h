#ifndef _WIND_H_
#define _WIND_H_

#define Wind_SHOW 0
#define Wind_CHANGE 1
#define Wind_ALARM 2
#define Wind_HOUR 3
#define Wind_MIN 4
#define Wind_OFF 5

#define ADDR_FLASH_PAGE_30    ((uint32_t)0x08008800) /* Base @ of Page 30, 1 Kbytes */

#include <stdint.h>

extern int Wind_state;
extern int Wind_loc;
extern int hours;
extern int minutes;
extern int Alarmmin;
extern int Alarmhour;
extern int AlarmCount;

void Wind_AlarmIs(int *hours , int *minutes);

void Wind_AlarmShow(int Alarmhour, int Alarmmin);

void Wind_SetAlarm(int Alarmhour, int Alarmmin);

void Wind_ChangesIs(int *hours , int *minutes);

void Wind_ChangeState(void);

void Wind_ChangeLoc(void);

void Wind_ChangeShow(int hours , int minutes);

void Wind_SetTime(int hours, int minutes);

void Wind_MinIn(int *minutes, int i);

void Wind_TimeShow(void);

void test(int x,int y);

uint8_t Wind_WriteAlarm(int *x);

uint8_t Wind_ReadAlarm(void);

#endif
