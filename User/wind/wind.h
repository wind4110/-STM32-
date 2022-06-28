#ifndef _WIND_H_
#define _WIND_H_

#define Wind_SHOW 0
#define Wind_CHANGE 1

extern int Wind_state;
extern int hours;
extern int minutes;
extern int Alarmmin;
extern int Alarmhour;


int Wind_RAX(int minutes);

void Wind_ChangesIs(int *hours , int *minutes);

void Wind_ChangeState(void);

void Wind_ChangeShow(int hours , int minutes);

void Wind_SetTime(int hours, int minutes);

void Wind_MinIn(int *minutes);

void Wind_HourIn(int *hours);

void Wind_TimeShow(void);

#endif
