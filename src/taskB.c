/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "syscall.h"

void task() {
  int a = syscall_divide();

  if (a == 1) {
    syscall_move(2, Right);
    for (uint32_t i = 0; i < 35; i++) {
      syscall_move(2, Left);
    }
  //Divide error:
    int z = 0;
    z = 1/z;
  } else {
    syscall_move(2, Left);
  }
  for (uint32_t i = 0; i < 105; i++) {
    syscall_move(2, Left);
  }
//Divide error:
  int z = 0;
  z = 1/z;

  while(1){
     syscall_move(1, Up);
}
    // while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
