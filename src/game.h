/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

typedef enum e_direction {
    Right = 1,
    Left = 2,
    Up = 3,
    Down = 4,
} e_direction_t;

typedef enum e_datatype {
    Null = 0, // no llega a ver que hay
    None = 10,
    Player = 20,
    Opponent = 30,
    Food = 40,
} e_datatype_t;


void game_init();

void game_nextStep();

#endif  /* !__GAME_H__ */
