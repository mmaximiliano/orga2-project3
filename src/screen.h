/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "stdint.h"
#include "colors.h"
#include "defines.h"
#include "tss.h"
#include "game.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    uint8_t c;
    uint8_t a;
} ca;

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr);
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr);
void print_hex(uint32_t numero, int size, uint32_t x, uint32_t y, uint16_t attr);

void screen_drawBox(uint32_t fInit, uint32_t cInit,
                    uint32_t fSize, uint32_t cSize,
                    uint8_t character, uint8_t attr );
//TODO: meter estos en el kernel
void inicializar_pantalla();
void inicializar_fruta();
void actualizar_pantalla();

extern uint8_t screen[50][50];

int read_c(int x, int y);
uint32_t move_c(uint32_t dis, int dir);
uint32_t divide_c();
int abs(int x);

void imprimirPantallaDebug();
void imprimirNumero(int num);
void imprimirMensajeDebug();
void borrarMensajeDebug();
uint32_t matarTareaActual();

// estructura auxiliar para imprimir en modo debug
typedef struct {
	unsigned int   eax;
	unsigned int   ebx;
	unsigned int   ecx;
	unsigned int   edx;
	unsigned int   esi;
	unsigned int   edi;
	unsigned int   ebp;
	unsigned int   esp;
	unsigned int   eip;
	unsigned short cs;
	unsigned short ds;
	unsigned short es;
	unsigned short fs;
	unsigned short gs;
	unsigned short ss;
	unsigned int   eflags;
	unsigned int   cr0;
	unsigned int   cr2;
	unsigned int   cr3;
	unsigned int   cr4;
	unsigned int   stack1;
	unsigned int   stack2;
	unsigned int   stack3;
	unsigned int   stack4;
	unsigned int   stack5;
} __attribute__((__packed__)) info_debug;

#endif  /* !__SCREEN_H__ */
