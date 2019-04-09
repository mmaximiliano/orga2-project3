/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


unsigned int nextFreeKernelPage;
unsigned int nextFreeTaskPage;

void mmu_init() {
	nextFreeKernelPage = INICIO_PAGINAS_LIBRES_KERNEL;
	nextFreeTaskPage = INICIO_PAGINAS_LIBRES_TASK;
}

uint32_t mmu_nextFreeKernelPage() {
	uint32_t freeKernelPage = nextFreeKernelPage;
	nextFreeKernelPage += PAGE_SIZE;
    return freeKernelPage;
}

uint32_t mmu_nextFreeTaskPage() {
    uint32_t freeTaskPage = nextFreeTaskPage;
		nextFreeTaskPage += PAGE_SIZE;
    return freeTaskPage;
}

void mmu_mapPage(uint32_t virtual, uint32_t cr3, uint32_t phy, uint32_t attrs) {

	unsigned int indice_pd = virtual >> 22; // Me quedo con los bits 22-31
	unsigned int indice_pt = (virtual << 10) >> 22; // Me quedo con los bits 12-21

	pd_entry* page_directory = (pd_entry*) cr3;

	// Si la tabla de páginas no existe, la creo
	if ( !( page_directory[indice_pd].p ) ){
		pt_entry* nueva_pt = (pt_entry*) mmu_nextFreeKernelPage();

	 	for ( int i = 0; i < 1024; ++i ){
	 		nueva_pt[i].p = 0;
	 	}

	 	page_directory[indice_pd].p = 1;
	 	page_directory[indice_pd].rw= 1;
	 	page_directory[indice_pd].us= 1;
	 	page_directory[indice_pd].table = (uint32_t) nueva_pt >> 12;
	}

	// Completo la entrada correspondiente de la tabla de páginas

	pt_entry* page_table = (pt_entry*) ( page_directory[indice_pd].table << 12 );

	page_table[indice_pt].p = 1;
	page_table[indice_pt].us = getUS(attrs);
	page_table[indice_pt].rw = getRW(attrs);
	page_table[indice_pt].page = phy >> 12;

	tlbflush();

}

void mmu_unmapPage(uint32_t virtual, uint32_t cr3) {

  unsigned int indice_pd = virtual >> 22;         // Me quedo con los bits 22-31
	unsigned int indice_pt = (virtual << 10) >> 22; // Me quedo con los bits 12-21

	pd_entry* page_directory = (pd_entry*) cr3;
	pt_entry* page_table = (pt_entry*) ( page_directory[indice_pd].table << 12 );
	page_table[indice_pt].p = 0;

	tlbflush();

}

pd_entry* mmu_initTaskDir(uint8_t is_copy, uint32_t team) {

	pd_entry* pd = (pd_entry*) mmu_nextFreeKernelPage();
	pt_entry* pt = (pt_entry*) mmu_nextFreeKernelPage();

	pd[0].p = 1;
	pd[0].rw = 1;
	pd[0].us = 1;
	pd[0].pwt = 0;
	pd[0].pcd = 0;
	pd[0].a = 0;
	pd[0].ignored = 0;
	pd[0].ps = 0;
	pd[0].g = 0;
	pd[0].avl = 0;
	pd[0].table = ((uint32_t)pt)>>12;

	for (int i = 1; i < 1024; i++) {
		pd[i].p = 0;
	}

	for (int i = 0; i < 1024; i++) {
		pt[i].p = 1;
		pt[i].rw = 1;
		pt[i].us = 0;
		pt[i].pwt = 0;
		pt[i].pcd = 0;
		pt[i].a = 0;
		pt[i].db = 0;
		pt[i].pat = 0;
		pt[i].g = 0;
		pt[i].avl = 0;
		pt[i].page = (uint32_t)(i);
	}
		//libero 2 paginas para datos y codigo y copio ambas (consecutivas)
	uint32_t PLT1 = mmu_nextFreeTaskPage();
	uint32_t PLT2 = mmu_nextFreeTaskPage();
	uint32_t attrs =  0x007; // p=1, us=1, rw=1

	mmu_mapTaskPages((uint32_t)pd, PLT1, PLT2, attrs, is_copy, team);
	return pd;
}

void mmu_mapTaskPages(uint32_t cr3_nuevo, uint32_t PLT1, uint32_t PLT2,
												uint32_t attrs, uint8_t is_copy, uint32_t team){
	uint32_t cr3_actual = rcr3();

	mmu_mapPage(PLT1, cr3_actual, PLT1, attrs);
	mmu_mapPage(PLT2, cr3_actual, PLT2, attrs);

	uint32_t dir_codigo;

	//isNew solo se usa para las 2 primeras tareas, despues todas las tareas
	//nuevas que se creen seran mediante divide y se copiara el codigo y datos
	//desde el 0x08000000 del cr3 de la tarea actual
	if (is_copy == 0) {
		dir_codigo = team == 0 ? TASK_A_CODE_ADDRESS_LV0 : TASK_B_CODE_ADDRESS_LV0;
	} else {
		dir_codigo = TASK_CODE;
	}

	mmu_copy(dir_codigo, dir_codigo + PAGE_SIZE, PLT1);
	mmu_copy(dir_codigo + PAGE_SIZE, dir_codigo + 2 * PAGE_SIZE, PLT2);

	mmu_mapPage(TASK_CODE, cr3_nuevo, PLT1, attrs);
	mmu_mapPage(TASK_CODE + PAGE_SIZE, cr3_nuevo, PLT2, attrs);

	mmu_unmapPage(PLT1, cr3_actual);
	mmu_unmapPage(PLT2, cr3_actual);
}

void mmu_copy(uint32_t dir_inicio, uint32_t dir_final, uint32_t dst) {
	for (uint32_t i = dir_inicio; i < dir_final; i++)
	{
		((uint8_t*)dst+i-dir_inicio)[0] = ((uint8_t*)i)[0];
	}
}

void mmu_initKernelDir() {
  pd_entry *pd = (pd_entry*) 0x27000;
  pd[0].p = 1;
	pd[0].rw = 1;
	pd[0].us = 0;
	pd[0].pwt = 0;
	pd[0].pcd = 0;
	pd[0].a = 0;
	pd[0].ignored = 0;
	pd[0].ps = 0;
	pd[0].g = 0;
	pd[0].avl = 0;
	pd[0].table = ((uint32_t)(KERNEL_PAGE_TABLE_0>>12));

// Pongo todo el resto de las posiciones en cero.
	for (int i = 1; i < 1024; ++i){
		pd[i].p = 0;
	}

// Inicializo page_table
	pt_entry* page_table_1 = (pt_entry*) KERNEL_PAGE_TABLE_0;
	for (int i = 0; i < 1024; ++i){

  page_table_1[i].p = 1;
	page_table_1[i].rw = 1;
	page_table_1[i].us = 0;
	page_table_1[i].pwt = 0;
	page_table_1[i].pcd = 0;
	page_table_1[i].a = 0;
	page_table_1[i].db = 0;
	page_table_1[i].pat = 0;
	page_table_1[i].g = 0;
	page_table_1[i].avl = 0;
	page_table_1[i].page = (uint32_t)(i);
	}
}


uint8_t getUS(uint32_t a){
	uint8_t res = (uint8_t) (a >> 2) & 1;
	return res;
}

uint8_t getRW(uint32_t a){
	uint8_t res = (uint8_t) ( a >> 1) & 1;
	return res;
}
