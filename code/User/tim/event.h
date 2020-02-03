#ifndef _EVENT_H
#define _EVENT_H

#include"headfile.h"


#define  TIME_INTERVAL  0.005

extern float offset0, offset1, val;
extern u8 hand_flag;


void TIM2_deal(void);
void hand_judge(void);
void hand_dispose(void);
void disp_init(void);
float GetOffset(u8 index);
float get_ave(float a[], u8 m, u8 n);
float Cap_Mid(void);
float fliter(float pre, float cur);


#endif

