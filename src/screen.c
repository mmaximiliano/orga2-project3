/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

uint8_t screen[50][50];
info_debug info_actual;


void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    uint8_t hexa[8];
    uint8_t letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_drawBox(uint32_t fInit,
                    uint32_t cInit,
                    uint32_t fSize,
                    uint32_t cSize,
                    uint8_t character,
                    uint8_t attr ) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    uint32_t f;
    uint32_t c;
    for (f = fInit; f < fInit+fSize; f++) {
    for (c = cInit; c < cInit+cSize; c++) {
          p[f][c].c = character;
          p[f][c].a = attr;
    }}
}

void inicializar_fruta() {
  screen[49][49] = frutilla;
  screen[38][12] = frutilla;
  screen[20][15] = banana;
  screen[37][3] = banana;
  screen[12][42] = banana;
  screen[4][3] = banana;
  screen[2][28] = uva;
  screen[46][29] = uva;
  screen[38][49] = uva;
  screen[39][6] = uva;
  screen[11][34] = uva;
  screen[43][7] = uva;
  screen[13][29] = uva;
  screen[41][11] = uva;
}

void inicializar_pantalla() {
  //limpiar pantala
  for (int  i = 0; i < 50; i++) {
    for (int  j = 0; j < 50; j++) {
      screen[j][i] = 0;
      print(" ", i, j, 0x77);
    }
  }
  inicializar_fruta();
  // etiquetas
  print("A", 65, 5, 0x47);
  print("AAA", 64, 4, 0x44);
  print("AAA", 64, 6, 0x44);
  print("A", 64, 5, 0x44);
  print("A", 66, 5, 0x44);

  print("B", 65, 25, 0x17);
  print("BBB", 64, 24, 0x11);
  print("BBB", 64, 26, 0x11);
  print("B", 64, 25, 0x11);
  print("B", 66, 25, 0x11);

  //cuadro de puntos A
  print_dec(0, 12, 59, 9,  0x44);
  print_dec(0, 12, 59, 11, 0x44);
  print_dec(0, 1, 59, 10, 0x44);
  print_dec(0, 1, 70, 10, 0x44);
  //cuadro de puntos B
  print_dec(0, 12, 59, 29,  0x11);
  print_dec(0, 12, 59, 31, 0x11);
  print_dec(0, 1, 59, 30, 0x11);
  print_dec(0, 1, 70, 30, 0x11);

  actualizar_pantalla();
}

void actualizar_pantalla() {
  //pintamos las frutas
  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      switch (screen[j][i]) {
        case uva:
          print("U", i, j, 0x75);
          break;
        case banana:
          print("B", i, j, 0x72);
          break;
        case frutilla:
          print("F", i, j, 0x74);
          break;
        default:
          screen[j][i] = 0;
          print(" ", i, j, 0x77);
      }
    }
  }

  // rojo = 4, azul = 1, gris = 7, negro = 0
  for (int i = 0; i < 10; i++) { //Imprimo informacion de juego y jugadores
    if (info_a_tasks[i].is_alive == 1 || info_a_tasks[i].revive == 1) {
      screen[info_a_tasks[i].y][info_a_tasks[i].x] = equipoA; //guardo en pantalla
      print("A", info_a_tasks[i].x, info_a_tasks[i].y, 0x47); //imprimo en pantalla
      print("O", 51+(3*i),15,  0x04); //si esta vivo lo imprimo a la izquierda con su peso
      print_dec(info_a_tasks[i].weight, 2, 51+(3*i), 18 ,0x04);
    } else {
      print("X", 51+(3*i),15, 0x04); //imprimo si esta muerto a la izq con peso 0
      print_dec(00, 2, 51+(3*i), 18,0x04);
    }
    if (info_b_tasks[i].is_alive || info_b_tasks[i].revive) {
      screen[info_b_tasks[i].y][info_b_tasks[i].x]= equipoB;
      print("B", info_b_tasks[i].x, info_b_tasks[i].y, 0x17);
      print("O", 51+(3*i),35,  0x01);
      print_dec(info_b_tasks[i].weight, 2, 51+(3*i), 38, 0x01);
    } else {
      print("X", 51+(3*i),35, 0x01);
      print_dec(00, 2, 51+(3*i),38, 0x01);
    }
  }
  //pintamos los puntos

  print_dec(POINTS_A, 10, 60, 10, 0x47);
  print_dec(POINTS_B, 10,60, 30,  0x17);

}

int read_c(int x, int y) {
  info_task t;
  if (CURRENT_TEAM == 0){
    //team es A
    t = info_a_tasks[CURRENT_TASK];
  } else {
    //team es B
    t = info_b_tasks[CURRENT_TASK];
  }
  if (abs(x)+ abs(y) > t.max_look){
    return 0; //NULL
  } else {
    //transformo coordenadas relativas a absolutas

    int x_abs = (t.x + x) % 50;
    int y_abs = (t.y + y) % 50;

    switch (screen[y_abs][x_abs]) {
      case 0:
        return 10; //None
      case 1:
        return CURRENT_TEAM == 0 ? 20 : 30; //Player : Opponent
      case 2:
        return CURRENT_TEAM == 1 ? 20 : 30; //Player : Opponent
      default:
        return 40; //Food
    }
  }
}

uint32_t move_c(uint32_t dis, int dir) {
  info_task* info_tasks = CURRENT_TEAM == 0 ? info_a_tasks : info_b_tasks;
  info_task* task = &info_tasks[CURRENT_TASK];
  dis = task->max_move > dis ? dis : task->max_move;
  dis %= TABLERO_SIZE;
  switch (dir) {
    case 1: //Right
      task->x = (task->x + dis) % TABLERO_SIZE;
      break;
    case 2: //Left
      dis = TABLERO_SIZE - dis;
      task->x = (task->x + dis) % TABLERO_SIZE;
      break;
    case 3: //Up
      dis = TABLERO_SIZE - dis;
      task->y = (task->y + dis) % TABLERO_SIZE;
      break;
    case 4: //Down
      task->y = (task->y + dis) % TABLERO_SIZE;
      break;
  }
  return dis;
}

uint32_t divide_c(){
  int disponible = -1;
  //chequeo que me pueda dividir
  info_task *info_tasks = CURRENT_TEAM == 0 ? info_a_tasks : info_b_tasks;
  info_task *actual = &info_tasks[CURRENT_TASK];

  if (actual->weight <= 1)
    return -1;

  uint32_t new_weight = actual->weight / 2;
  actual->weight = new_weight;

  for (int i = 0; i < 10; ++i) {
    if (info_tasks[i].is_alive == 0 && info_tasks[i].revive == 0) {
      disponible = i;
      i = 10;
    }
  }

  if (disponible == -1)
    return -1; //devuelvo -1 si no se puede dividir

  //guardo tarea nueva y actual
  // cambio atributos de actual
  actual->max_move = actual->max_move * 2;
  actual->max_look = actual->max_look / 2;
  // cambio atributos de nueva

  info_tasks[disponible].weight = actual->weight;
  info_tasks[disponible].max_move = actual->max_move;
  info_tasks[disponible].max_look = actual->max_look;
  info_tasks[disponible].y = actual->y;
  info_tasks[disponible].x = actual->x;
  info_tasks[disponible].revive = 1;
  //parametros: int task, int team, uint8_t is_new, esp, flags, eip
  register uint32_t ebp asm("ebp");
  register uint32_t esp asm("esp");
	tss_init_task(disponible, CURRENT_TEAM, 1, esp, ebp, (uint32_t)(&&return_eip));
  return_eip:
  return CURRENT_TASK == disponible ? 1 : 0;
}

int abs(int x) {
  return x>=0 ? x : -x;
}

void imprimirPantallaDebug(){
  //pinto todo de negro
  for (int i = 0; i < 50; i++){
    for (int j = 0; j < 50; j++){
      print("a", i, j, 0x00);
    }
  }
  // imprimo nombres de registros
  print("ebx", 5, 4, 0x05);
  print("eax", 5, 2, 0x05);
  print("ecx", 5, 6, 0x05);
  print("edx", 5, 8, 0x05);
  print("esi",  5,10, 0x05);
  print("edi",  5,12, 0x05);
  print("ebp",  5,14, 0x05);
  print("esp",  5,16, 0x05);
  print("eip",  5,18, 0x05);
  print("cs",  5,20, 0x05);
  print("ds",  5,22, 0x05);
  print("es",  5,24, 0x05);
  print("fs",  5,26, 0x05);
  print("gs",  5,28, 0x05);
  print("ss",  5,30, 0x05);
  print("eflags", 5, 32, 0x05);

  //segunda columna
  print("cr0", 26, 2, 0x05);
  print("cr2", 26, 4, 0x05);
  print("cr3", 26, 6, 0x05);
  print("cr4", 26, 8, 0x05);
  print("stack", 26, 10,  0x05);

  //imprimimos datos
  print_hex(info_actual.eax, 8, 10, 2, 0x02);
  print_hex(info_actual.ebx, 8, 10, 4, 0x02);
  print_hex(info_actual.ecx, 8, 10, 6, 0x02);
  print_hex(info_actual.edx, 8, 10, 8, 0x02);
  print_hex(info_actual.esi, 8,  10,10, 0x02);
  print_hex(info_actual.edi, 8,  10,12, 0x02);
  print_hex(info_actual.ebp, 8,  10,14, 0x02);
  print_hex(info_actual.esp, 8,  10,16, 0x02);
  print_hex(info_actual.eip, 8,  10,18, 0x02);
  print_hex(info_actual.cs, 4, 10, 20, 0x02);
  print_hex(info_actual.ds, 4, 10, 22, 0x02);
  print_hex(info_actual.es, 4, 10, 24, 0x02);
  print_hex(info_actual.fs, 4, 10, 26, 0x02);
  print_hex(info_actual.gs, 4, 10, 28, 0x02);
  print_hex(info_actual.ss, 4, 10, 30, 0x02);
  print_hex(info_actual.eflags, 8, 15, 32, 0x02);

  //Segunda Columna
  print_hex(info_actual.cr0, 8, 30, 2, 0x02);
  print_hex(info_actual.cr2, 8, 30, 4, 0x02);
  print_hex(info_actual.cr3, 8, 30, 6, 0x02);
  print_hex(info_actual.cr4, 8, 30, 8, 0x02);
  print_hex(info_actual.stack1, 8, 26, 12, 0x02);
  print_hex(info_actual.stack2, 8, 26, 14, 0x02);
  print_hex(info_actual.stack3, 8, 26, 16, 0x02);
  print_hex(info_actual.stack4, 8, 26, 18, 0x02);
  print_hex(info_actual.stack5, 8, 26, 20, 0x02);

}

void imprimirNumero(int num) {
  print_dec(num, 1, 79, 0, 0x72);
}

void imprimirMensajeDebug() {
  print("Modo Debug", 50, 1, 0x72);
}

void borrarMensajeDebug() {
  print("Modo Debug", 50, 1, 0x00);
}

uint32_t matarTareaActual() {
  if(CURRENT_TEAM == 1){
    info_b_tasks[CURRENT_TASK].is_alive = 0;
  }else{
    info_a_tasks[CURRENT_TASK].is_alive = 0;
  }
  //CURRENT_TASK++;
  return CURRENT_TASK;
}
