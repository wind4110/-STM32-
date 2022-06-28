#ifndef _WIND_H_
#define _WIND_H_

#define Wind_SHOW 0
#define Wind_CHANGE 1
#define Wind_ALARM 2
#define Wind_

extern int Wind_state;
extern int hours;
extern int minutes;
extern int Alarmmin;
extern int Alarmhour;

void Wind_AlarmIs(int *hours , int *minutes);

void Wind_AlarmShow(int Alarmhour, int Alarmmin);

void Wind_SetAlarm(int Alarmhour, int Alarmmin);

int Wind_RAX(int minutes);

void Wind_ChangesIs(int *hours , int *minutes);

void Wind_ChangeState(void);

void Wind_ChangeShow(int hours , int minutes);

void Wind_SetTime(int hours, int minutes);

void Wind_MinIn(int *minutes);

void Wind_HourIn(int *hours);

void Wind_TimeShow(void);

#endif
