/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
int CURRENT_TEAM = 0;			//tarea actual: A = 0, B = 1. empieza en A.
int CURRENT_TASK = -1;
int POINTS_A = 0;
int POINTS_B = 0;
int FRUIT_COUNT = 14;

void sched_init() {
  //hace falta esto?
}

int16_t sched_nextTask() {
  //avanzo hasta que llegue a la proxima task viva o a 10

  CURRENT_TASK++;   //Inicializada en -1
  if (CURRENT_TEAM == 0) {
    while (info_a_tasks[CURRENT_TASK].is_alive != 1 && CURRENT_TASK < 10) {
      CURRENT_TASK++;
    }
  }else {
    while (info_b_tasks[CURRENT_TASK].is_alive != 1 && CURRENT_TASK < 10) {
      CURRENT_TASK++;
    }
  }
//si la task es 10 resuelvo si necesito y cambio de equipo y reseteo el contador
  if (CURRENT_TASK == 10) {
    if (CURRENT_TEAM == 1) {
      //termina turno
      resolver_conflictos();
      actualizar_pantalla();
    }
    CURRENT_TASK = 0;
    CURRENT_TEAM = CURRENT_TEAM == 0 ? 1 : 0;
  }
  if (CURRENT_TEAM == 0) {
    while (info_a_tasks[CURRENT_TASK].is_alive != 1 && CURRENT_TASK < 10) {
      CURRENT_TASK++;
    }
  }else {
    while (info_b_tasks[CURRENT_TASK].is_alive != 1 && CURRENT_TASK < 10) {
      CURRENT_TASK++;
    }
  }

  if(CURRENT_TASK == 10){
    //pinto todo de negro
    for (int i = 0; i < 50; i++){
      for (int j = 0; j < 50; j++){
        print("a", i, j, 0x00);
      }
    }
    if(CURRENT_TEAM == 1){
      print("EL EQUIPO A ES EL GANADOR",15, 15, 0x75);
    }else{
      print("EL EQUIPO B ES EL GANADOR",15, 15, 0x75);
    }

    while (1) {
      /* nada */
    }


  }

  if(FRUIT_COUNT == 0){
    if(POINTS_A >= POINTS_B){
      print("EL EQUIPO A ES EL GANADOR",15, 15, 0x75);
    }else{
      print("EL EQUIPO B ES EL GANADOR",15, 15, 0x75);
    }
    while (1) {
      /* nada */
    }
  }

  //calculo selector de tarea a saltar
  int indice = CURRENT_TEAM == 0 ? CURRENT_TASK + 29 : CURRENT_TASK + 39;
  uint16_t selector = indice <<3;

  return selector;
}
