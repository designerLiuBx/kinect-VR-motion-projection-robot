#ifndef __MOTCOMP_H
#define __MOTCOMP_H
#include "Gen_include.h"

void main_controller(void);
void mode_check(void);
void get_acc(void);
void motion_comp(void);
double getCubeRoot(double Y);
double lowfilter(double x);
char lowfilter2(double x);
double lowfilter3(double x);
#endif
