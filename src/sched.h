/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "screen.h"
#include "tss.h"

void sched_init();

void init_global_variables();

int16_t sched_nextTask();

extern int CURRENT_TEAM;			//tarea actual: A = 0, B = 1. empieza en A.
extern int CURRENT_TASK;
extern int POINTS_A;
extern int POINTS_B;
extern int FRUIT_COUNT;


#endif	/* !__SCHED_H__ */
