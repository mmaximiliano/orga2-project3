; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

;GDT
EXTERN GDT_DESC
EXTERN gdt_fill_table
;interrupciones
EXTERN IDT_DESC
EXTERN idt_init
EXTERN pic_reset
EXTERN pic_enable
EXTERN pic_disable
;memoria
EXTERN mmu_initKernelDir
EXTERN mmu_init
;TSS
EXTERN tss_init
;Scheduler
EXTERN sched_init
;Screen
EXTERN screen_drawBox
EXTERN inicializar_fruta


;; Saltear seccion de datos
jmp start

%define PILA_KERNEL             0x27000
%define BASE_PAGE_DIRECTORY     0x27000
%define SELECTOR_CODIGO_LVL0    0xB0
%define SELECTOR_DATOS_LVL0     0xC0
%define SELECTOR_TSS_INICIAL    0xD8
%define SELECTOR_VIDEO          0xD0
%define SELECTOR_TSS_IDLE       0000000011100000b
;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

paginacion_habilitada_msg db     'Paginacion Habilitada'
paginacion_habilitada_len equ    $ - paginacion_habilitada_msg

libreta_grupo_msg db '123/17, 368/17'
libreta_grupo_len equ $ - libreta_grupo_msg

basura1_msg db     'BASURA1'
basura1_len equ    $ - basura1_msg

;offset: dd 0
;selector: dw 0

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font


    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0

    ; Habilitar A20
    call A20_enable

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp SELECTOR_CODIGO_LVL0:modoprotegido

BITS 32
modoprotegido:
    ; Deshabilitar interrupciones
    cli
    ; Establecer selectores de segmentos
    mov ax, SELECTOR_DATOS_LVL0         ;muevo selector para segmento
    mov ds, ax                          ;copio al segmento de datos el valor que puse en ax
    mov es, ax                          ;pongo el resto de los registros de segmento en el mismo segmento
    mov gs, ax
    mov ss, ax
    mov ax, SELECTOR_VIDEO
    mov fs, ax

    ; Establecer la base de la pila
    mov eax, PILA_KERNEL
    mov ebp, eax
    mov esp, eax

    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 2, 0

    ; Inicializar pantalla
    xor ebx, ebx        ;limpio ebx
.pintarGris:
    mov word [fs:ebx], 1111000000000000b            ;back gris claro, fore gris oscuro
    add ebx, 2
    cmp ebx, 8000
    jl .pintarGris

    mov ebx, 100
    mov eax, 100
.pintarNegro:
    mov word [fs:eax], 0000000000000000b            ;back gris claro, fore gris oscuro
    add ebx, 2
    add eax, 2
    cmp ebx, 160
    jl .pintarNegro
    mov ebx, 100
    add eax, 100
    cmp eax, 8000
    jl .pintarNegro

    ;Completamos la gdt
    call gdt_fill_table
    ; Inicializar el manejador de memoria
    call mmu_init
    ; Inicializar el directorio de paginas
    call mmu_initKernelDir

    ; ; Cargar directorio de paginas
    mov eax, BASE_PAGE_DIRECTORY
    mov cr3, eax


    ; ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Imprimo un mensaje de paginación habilitada.
    print_text_pm paginacion_habilitada_msg, paginacion_habilitada_len, 0x07, 2, 0

    ; Escribimos una libreta del  grupo
    print_text_pm libreta_grupo_msg, libreta_grupo_len, 0x07, 0 , 63

    ; Inicializar tss y Inicializar tss de la tarea Idle
    call tss_init

    ; Inicializar el scheduler
    call sched_init

    ; Inicializar la IDT
    call idt_init

    ; Cargar IDT
    lidt [IDT_DESC]



    ; Configurar controlador de interrupciones
    call pic_disable
    call pic_reset
    call pic_enable
    ; ; Cargar tarea inicial

    mov ax, SELECTOR_TSS_INICIAL
    ltr ax

    ; Habilitar interrupciones
    sti

    ;inicializar el juego

    ; Saltar a la primera tarea: Idle
    jmp SELECTOR_TSS_IDLE:0


    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
