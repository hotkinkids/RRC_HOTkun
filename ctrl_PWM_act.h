#pragma once

//extern void ctrl_PWM(void);
extern void  PWM_act(int u, int d, int p);
extern void  PWM_act2(int u, int d, int p);

extern void  PWM_act(int u, int d, int p, int max, int min);
extern void  PWM_act2(int u, int d, int p, int max, int min);