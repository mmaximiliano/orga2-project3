/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (uint32_t) &idt
};


    // La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    // interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    // atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    // la siguiene manera:

    // void idt_inicializar() {
    //     // IDT_ENTRY(0);
    //     // ...
    //     // IDT_ENTRY(19);
    //     // ...
    // }



#define IDT_ENTRY(numero)                                                                          \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) 0xB0;                                                          \
    idt[numero].attr = (uint16_t) 0x8e00;                                                            \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);



//attr = 1000 1110 00000
void idt_init() {
    // Excepciones
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);

    //External
    IDT_ENTRY(32);      // Teclado
    IDT_ENTRY(33);      // Reloj
    // syscall
    // READ = 0x47
                                                                         
    idt[0x47].offset_0_15 = (uint16_t) ((uint32_t)(&_isrRead) & (uint32_t) 0xFFFF);        
    idt[0x47].segsel = (uint16_t) 0xB0;                                                          
    idt[0x47].attr = (uint16_t) 0xee00;                                                             
    idt[0x47].offset_16_31 = (uint16_t) ((uint32_t)(&_isrRead) >> 16 & (uint32_t) 0xFFFF);

      // MOVE = 0x49
    idt[0x49].offset_0_15 = (uint16_t) ((uint32_t)(&_isrMove) & (uint32_t) 0xFFFF);        
    idt[0x49].segsel = (uint16_t) 0xB0;                                                          
    idt[0x49].attr = (uint16_t) 0xee00;                                                             
    idt[0x49].offset_16_31 = (uint16_t) ((uint32_t)(&_isrMove) >> 16 & (uint32_t) 0xFFFF);
 // DIVIDE = 0x4C
    idt[0x4C].offset_0_15 = (uint16_t) ((uint32_t)(&_isrDivide) & (uint32_t) 0xFFFF);        
    idt[0x4C].segsel = (uint16_t) 0xB0;                                                          
    idt[0x4C].attr = (uint16_t) 0xee00;                                                             
    idt[0x4C].offset_16_31 = (uint16_t) ((uint32_t)(&_isrDivide) >> 16 & (uint32_t) 0xFFFF);

}
