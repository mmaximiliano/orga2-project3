/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "gdt.h"
#include "mmu.h"
#include "screen.h" //matriz con la pantalla

extern uint32_t return_eip;

typedef struct str_tss {
    uint16_t  ptl;
    uint16_t  unused0;
    uint32_t    esp0;
    uint16_t  ss0;
    uint16_t  unused1;
    uint32_t    esp1;
    uint16_t  ss1;
    uint16_t  unused2;
    uint32_t    esp2;
    uint16_t  ss2;
    uint16_t  unused3;
    uint32_t    cr3;
    uint32_t    eip;
    uint32_t    eflags;
    uint32_t    eax;
    uint32_t    ecx;
    uint32_t    edx;
    uint32_t    ebx;
    uint32_t    esp;
    uint32_t    ebp;
    uint32_t    esi;
    uint32_t    edi;
    uint16_t  es;
    uint16_t  unused4;
    uint16_t  cs;
    uint16_t  unused5;
    uint16_t  ss;
    uint16_t  unused6;
    uint16_t  ds;
    uint16_t  unused7;
    uint16_t  fs;
    uint16_t  unused8;
    uint16_t  gs;
    uint16_t  unused9;
    uint16_t  ldt;
    uint16_t  unused10;
    uint16_t  dtrap;
    uint16_t  iomap;
} __attribute__((__packed__, aligned (8))) tss;

extern tss tss_initial;
extern tss tss_idle;
//?
extern tss tss_tareas_a[10];
extern tss tss_tareas_b[10];

void tss_init();
void tss_init_idle();
void tss_init_initial();
void tss_init_tasks();
void tss_init_task(uint32_t task, uint32_t team, uint8_t is_copy, uint32_t esp, uint32_t ebp, uint32_t return_eip);


typedef struct info_task_d {
    uint8_t weight;
    uint8_t max_move;
    uint8_t max_look;
    uint8_t x;
    uint8_t y;
    uint8_t is_alive;  //0 si no existe 1 si si
    uint8_t revive; //1 si hay que revivirla
    uint8_t sarasadebug;
} info_task;


info_task info_a_tasks[10];
info_task info_b_tasks[10];

info_task* current_task();
void resolver_conflictos();


#endif  /* !__TSS_H__ */
