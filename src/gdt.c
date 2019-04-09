/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [GDT_IDX_COD_0] = (gdt_entry) {
        (uint16_t)    0x9A00,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x0A,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x02,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [GDT_IDX_COD_3] = (gdt_entry) {
        (uint16_t)    0x9A00,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x0A,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x03,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x02,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [GDT_IDX_DAT_0] = (gdt_entry) {
        (uint16_t)    0x9A00,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x02,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    [GDT_IDX_DAT_3] = (gdt_entry) {
        (uint16_t)    0x9A00,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x03,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x02,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x01,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
// Revisar de aca para abajo
        /* Dato Nivel 0 */
    [GDT_IDX_DATO_VID_LVL_0_DESC] = (gdt_entry) {
        (uint16_t)    0x1F3F,         /* limit[0:15]  */
        (uint16_t)    0x8000,         /* base[0:15]   */
        (uint8_t)     0x0B,           /* base[23:16]  */
        (uint8_t)     0x02,           /* type         */
        (uint8_t)     0x01,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    /* TSS de la tarea inicial */
    [GDT_TSS_TAREA_INICIAL_DESC] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type:1001    */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    /* TSS de la tarea idle */
    [GDT_TSS_TAREA_IDLE_DESC] = (gdt_entry) {
        (uint16_t)    0x0067,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x09,           /* type:1001    */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x01,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x01,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    }
};

void gdt_fill_table() {
  for (uint8_t i = 29; i < 49; i++) {
        gdt[i].limit_0_15 = 0x0067;             /* limit[0:15]  */
        gdt[i].base_0_15 = 0x0000;              /* base[0:15]   */
        gdt[i].base_23_16 = 0x00;               /* base[23:16]  */
        gdt[i].type = 0x09;                     /* type:1001    */
        gdt[i].s = 0x00;                        /* s            */
        gdt[i].dpl = 0x03;                      /* dpl          */
        gdt[i].p = 0x01;                        /* p            */
        gdt[i].limit_16_19 = 0x00;              /* limit[16:19] */
        gdt[i].avl = 0x00;                      /* avl          */
        gdt[i].l = 0x00;                        /* l            */
        gdt[i].db = 0x01;                       /* db           */
        gdt[i].g = 0x00;                        /* g            */
        gdt[i].base_31_24 = 0x00;               /* base[31:24]  */
  }
}

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
