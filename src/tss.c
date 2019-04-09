/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_initial;
tss tss_idle;

tss tss_tareas_a[10];
tss tss_tareas_b[10];



void tss_init() {
		tss_init_initial();
		tss_init_idle();
		tss_init_tasks();
		//Inicializo todas las tareas como muertas
		for (int i = 1; i < 10; i++) {
			info_a_tasks[i].is_alive = 0;
			info_b_tasks[i].is_alive = 0;
		}
		inicializar_pantalla();
}

void tss_init_tasks() {
	//Linkeo la gdt con los vectores de las tareas que contienen sus TSS
	//Tareas de A:
	for(int i = 0;i<10; i++) {
		gdt[i+29].base_0_15 =    (unsigned int) &tss_tareas_a[i] & 0x0000FFFF;
		gdt[i+29].base_23_16 = ( (unsigned int) &tss_tareas_a[i] & 0x00FF0000 ) >> 16;
		gdt[i+29].base_31_24 = ( (unsigned int) &tss_tareas_a[i] & 0xFF000000 ) >> 24;
	}

	//Tareas de B:
	for(int i = 0;i<10; i++) {
		gdt[i+39].base_0_15 =    (unsigned int) &tss_tareas_b[i] & 0x0000FFFF;
		gdt[i+39].base_23_16 = ( (unsigned int) &tss_tareas_b[i] & 0x00FF0000 ) >> 16;
		gdt[i+39].base_31_24 = ( (unsigned int) &tss_tareas_b[i] & 0xFF000000 ) >> 24;
	}

	//inicializo 1 tarea para cada equipo
	tss_init_task(0, 0, 0, 0, 0, 0); //tarea 0, equipo A, tarea no es copia
	tss_init_task(0, 1, 0, 0, 0, 0); //tarea 0, equipo B, tarea no es copia

	info_a_tasks[0].is_alive = 1;
	info_a_tasks[0].x = 12;
	info_a_tasks[0].y = 12;
	info_a_tasks[0].weight = 64;
	info_a_tasks[0].max_look = 64;
	info_a_tasks[0].max_move = 1;

	info_b_tasks[0].is_alive = 1;
	info_b_tasks[0].x = 38;
	info_b_tasks[0].y = 38;
	info_b_tasks[0].weight = 64;
	info_b_tasks[0].max_look = 64;
	info_b_tasks[0].max_move = 1;
}
void tss_init_idle()
{
	// Actualizo el comienzo del TSS de la tarea idle en su descriptor de la GDT
	gdt[GDT_TSS_TAREA_IDLE_DESC].base_0_15 =    (unsigned int) &tss_idle & 0x0000FFFF;
	gdt[GDT_TSS_TAREA_IDLE_DESC].base_23_16 = ( (unsigned int) &tss_idle & 0x00FF0000 ) >> 16;
	gdt[GDT_TSS_TAREA_IDLE_DESC].base_31_24 = ( (unsigned int) &tss_idle & 0xFF000000 ) >> 24;

	// Inicializo el TSS de la tarea idle
	tss_idle.cr3    = PAGE_DIRECTORY_KERNEL;
	tss_idle.eip    = DIR_CODIGO_TAREA_IDLE;
	tss_idle.ebp    = PILA_KERNEL;
	tss_idle.esp    = PILA_KERNEL;
	tss_idle.eflags = 0x00000202;
	tss_idle.cs     = SELECTOR_CODIGO_LVL0;
	tss_idle.ds     = SELECTOR_DATOS_LVL0;
	tss_idle.es     = SELECTOR_DATOS_LVL0;
	tss_idle.gs     = SELECTOR_DATOS_LVL0;
	tss_idle.fs     = SELECTOR_DATOS_LVL0;
	tss_idle.ss     = SELECTOR_DATOS_LVL0;
	tss_idle.iomap  = 0xFFFF;
}

void tss_init_initial()
{
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_0_15 =    (unsigned int) &tss_initial & 0x0000FFFF;
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_23_16 = ( (unsigned int) &tss_initial & 0x00FF0000 ) >> 16;
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_31_24 = ( (unsigned int) &tss_initial & 0xFF000000 ) >> 24;
}

void tss_init_task(uint32_t task, uint32_t team, uint8_t is_copy, uint32_t esp,
										uint32_t ebp, uint32_t return_eip) {
	uint32_t cr3 = (uint32_t) mmu_initTaskDir(is_copy, team);

	tss *tarea = team == 0 ? &tss_tareas_a[task] : &tss_tareas_b[task];

	tarea->cr3 = cr3;
	uint32_t stack_lvl0 = mmu_nextFreeKernelPage();
	tarea->esp0 = stack_lvl0 + PAGE_SIZE;

	tarea->ss0 = SELECTOR_DATOS_LVL0;
	tarea->gs = SELECTOR_DATOS_LVL3;
	tarea->fs = SELECTOR_DATOS_LVL3;
	tarea->ds = SELECTOR_DATOS_LVL3;
	tarea->es = SELECTOR_DATOS_LVL3;

	if (is_copy == 1) {
		//se llamo desde el divide la tarea copiada devuelve 1
		tarea->eax = 1;
		tarea->eip = return_eip; //esta declarado en isr.asm

		uint32_t offset = esp & 0xFFF;
		tarea->esp = stack_lvl0 + offset;
		offset = ebp & 0XFFF;
		tarea->ebp = stack_lvl0 + offset;

		tarea->ss = SELECTOR_DATOS_LVL0;
		tarea->cs = SELECTOR_CODIGO_LVL0;

		uint32_t dir_inicial = esp & 0xFFFFF000;
		mmu_copy(dir_inicial, dir_inicial + PAGE_SIZE, stack_lvl0);
	} else {
		tarea->eip = 0x08000000;
		tarea->esp = 0x08002000; //0x08000000 + 2 paginas (1024 + 1023)
		tarea->eflags = 0x0202;

		tarea->ss = SELECTOR_DATOS_LVL3;
		tarea->cs = SELECTOR_CODIGO_LVL3;
	}
}

void resolver_conflictos() {
	for (int  i = 0; i < 10; i++) {
		uint8_t x_actual = info_a_tasks[i].x;
		uint8_t y_actual = info_a_tasks[i].y;
		//recorro las 10 tareas de A y veo si coinciden con alguna de B
		if (info_a_tasks[i].is_alive == 1) {
			uint8_t total_weight_a = info_a_tasks[i].weight;
			uint8_t total_weight_b = 0;
			for (int  j = 0; j < 10; j++) {
				if (j != i && info_a_tasks[j].is_alive == 1 && x_actual == info_a_tasks[j].x && y_actual == info_a_tasks[j].y) {
					//chequeo si hay otra de A en la misma posicion y si es asi, sumo sus pesos
					total_weight_a += info_a_tasks[j].weight;
				}
				if (info_b_tasks[j].is_alive == 1 && x_actual == info_b_tasks[j].x && y_actual == info_b_tasks[j].y) {
					//si hay alguno de b en (x,y) sumo sus pesos
					total_weight_b += info_b_tasks[j].weight;
				}
			}
			//sumo puntajes a A o a B de ser necesario
			if (total_weight_a > total_weight_b) {
					POINTS_A += total_weight_b;
					if (screen[y_actual][x_actual] >=3){
						POINTS_A += screen[y_actual][x_actual];
						screen[y_actual][x_actual] = 0;
						FRUIT_COUNT--;
					}
			} else if (total_weight_a < total_weight_b){
					POINTS_B += total_weight_a;
			}
			//chequeo si se estan comiendo y resuelvo si paso eso
			if (total_weight_b != 0 && total_weight_a >= total_weight_b) {
				for (int  j = 0; j < 10; j++) {
					if (x_actual == info_b_tasks[j].x && y_actual == info_b_tasks[j].y) {
						//mato los de B
						info_b_tasks[j].is_alive = 0;
					}
				}
			}
			if (total_weight_a <= total_weight_b) {
				for (int  j = 0; j < 10; j++) {
					if (x_actual == info_a_tasks[j].x && y_actual == info_a_tasks[j].y) {
						//mato a los A
						info_b_tasks[j].is_alive = 0;
					}
				}
			}
		}
		//chequeo si B suma puntos por fruta:
		x_actual = info_b_tasks[i].x;
		y_actual = info_b_tasks[i].y;
		if (screen[y_actual][x_actual] >= 3 && info_b_tasks[i].is_alive) {
			POINTS_B += screen[y_actual][x_actual];
			screen[y_actual][x_actual] = 0;
			FRUIT_COUNT--;
		}
	}
	//revivo las necesarias para A y B
	for (int i = 0; i < 10; ++i){
		if(info_a_tasks[i].revive == 1){
			info_a_tasks[i].is_alive = 1;
		  info_a_tasks[i].revive = 0; //revivo si necesito
		}
		if (info_b_tasks[i].revive == 1){
			info_b_tasks[i].is_alive = 1;
		  info_b_tasks[i].revive = 0; //revivo si necesito
		}
	}
}
