/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004

/* Misc */
/* -------------------------------------------------------------------------- */
#define SIZE_N                  50
#define SIZE_M                  50
#define TABLERO_SIZE            50
enum tablero {
  uva = 16,
  banana = 32,
  frutilla = 64,
  equipoA = 1,
  equipoB = 2
};

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 50

#define GDT_IDX_NULL_DESC           0

#define GDT_IDX_COD_0  22
#define GDT_IDX_COD_3  23
#define GDT_IDX_DAT_0  24
#define GDT_IDX_DAT_3  25
#define GDT_IDX_DATO_VID_LVL_0_DESC 26
#define GDT_TSS_TAREA_INICIAL_DESC 27
#define GDT_TSS_TAREA_IDLE_DESC 28

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC << 3)


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE             0x08000000 /* direccion virtual del codigo */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define TASK_IDLE_CODE_SRC_ADDR 0x14000	// antes estaba asi 0x00014000
#define TASK_A_CODE_ADDRESS_LV0 0x10000
#define TASK_B_CODE_ADDRESS_LV0 0x12000
#define NEW_MAPPED_PAGE			0x08000000

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR        			0x00027000
#define KERNEL_PAGE_TABLE_0    			0x00028000
#define INICIO_PAGINAS_LIBRES_KERNEL	0x100000
#define INICIO_PAGINAS_LIBRES_TASK		0x400000
#define PAGE_SIZE						4096
#define DIR_CODIGO_TAREA_IDLE   		0x14000
#define PAGE_DIRECTORY_KERNEL   		0x27000
#define PILA_KERNEL             0x27000
#define SELECTOR_CODIGO_LVL0    0xb0
#define SELECTOR_DATOS_LVL0     0xc0
#define SELECTOR_CODIGO_LVL3    0xb8|3
#define SELECTOR_DATOS_LVL3     0xc8|3

#endif  /* !__DEFINES_H__ */
