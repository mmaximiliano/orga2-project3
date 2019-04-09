/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

typedef struct str_pd_entry {
    uint8_t   p:1;					// present = 1
    uint8_t   rw:1;					// read/write = 1
    uint8_t   us:1;					// user/supervisor = 0
    uint8_t   pwt:1;
    uint8_t   pcd:1;
    uint8_t   a:1;					// accessed = 0
    uint8_t   ignored:1;
    uint8_t   ps:1;					// Page Size = 0 = 4kb
    uint8_t   g:1;					// global = 0
    uint8_t   avl:3;				// available = 0
    uint32_t  table:20;
} __attribute__((__packed__)) pd_entry;

typedef struct str_pt_entry {
    uint8_t   p:1;					// present = 1
    uint8_t   rw:1;					// read/write = 1
    uint8_t   us:1;					// user/supervisor = 0
    uint8_t   pwt:1;
    uint8_t   pcd:1;
    uint8_t   a:1;					// accessed = 0
    uint8_t   db:1;					// dirty bit = 0
    uint8_t   pat:1;				// Page Table Attribute = 0
    uint8_t   g:1;					// global = 0
    uint8_t   avl:3;				// available = 0
    uint32_t  page:20;
} __attribute__((__packed__)) pt_entry;

void mmu_init();

uint32_t mmu_nextFreeKernelPage();

uint32_t mmu_nextFreeTaskPage();

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs);

void mmu_unmapPage(uint32_t virtual, uint32_t cr3);

void mmu_initKernelDir();

pd_entry* mmu_initTaskDir(uint8_t is_copy, uint32_t team);

void mmu_copy(uint32_t dir_inicio, uint32_t dir_final, uint32_t dir_a_copiar);

uint8_t getUS(uint32_t a);

uint8_t getRW(uint32_t a);

void mmu_mapTaskPages( uint32_t cr3nuevo, uint32_t PLT1, uint32_t PLT2,
                        uint32_t attrs, uint8_t is_copy, uint32_t team);

#endif	/* !__MMU_H__ */
