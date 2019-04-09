; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones
%include "print.mac"


EXTERN read_c
EXTERN move_c
EXTERN divide_c
EXTERN info_actual
EXTERN imprimirPantallaDebug
EXTERN imprimirNumero
EXTERN imprimirMensajeDebug
EXTERN borrarMensajeDebug
EXTERN matarTareaActual

BITS 32

modoDebug: db 0
modoDebugPantalla: db 0


isr_error_msg0 db     'Divide Error'
isr_error_msg_len0 equ    $ - isr_error_msg0

isr_error_msg1 db     'RESERVED'
isr_error_msg_len1 equ    $ - isr_error_msg1

isr_error_msg2 db     'NMI Interrupt'
isr_error_msg_len2 equ    $ - isr_error_msg2

isr_error_msg3 db     'Breakpoint'
isr_error_msg_len3 equ    $ - isr_error_msg3

isr_error_msg4 db     'Overflow'
isr_error_msg_len4 equ    $ - isr_error_msg4

isr_error_msg5 db     'Bound Range Exceeded'
isr_error_msg_len5 equ    $ - isr_error_msg5

isr_error_msg6 db     'Invalid Opcode (Undefined Opcode)'
isr_error_msg_len6 equ    $ - isr_error_msg6

isr_error_msg7 db     'Device Not Available (No Math Coprocessor)'
isr_error_msg_len7 equ    $ - isr_error_msg7

isr_error_msg8 db     'Double Fault'
isr_error_msg_len8 equ    $ - isr_error_msg8

isr_error_msg9 db     'Coprocessor Segment Overrun'
isr_error_msg_len9 equ    $ - isr_error_msg9

isr_error_msg10 db     'Invalid TSS'
isr_error_msg_len10 equ    $ - isr_error_msg10

isr_error_msg11 db     'Segment Not Present'
isr_error_msg_len11 equ    $ - isr_error_msg11

isr_error_msg12 db     'Stack-Segment Fault'
isr_error_msg_len12 equ    $ - isr_error_msg12

isr_error_msg13 db     'General Protection Fault'
isr_error_msg_len13 equ    $ - isr_error_msg13

isr_error_msg14 db     'Page Fault'
isr_error_msg_len14 equ    $ - isr_error_msg14

isr_error_msg16 db     'x87 FPU Floating-Point Error (Math Fault)'
isr_error_msg_len16 equ    $ - isr_error_msg16

isr_error_msg17 db     'Alignment Check'
isr_error_msg_len17 equ    $ - isr_error_msg17

isr_error_msg18 db     'Machine Check'
isr_error_msg_len18 equ    $ - isr_error_msg18

isr_error_msg19 db     'SIMD Floating-Point Exception'
isr_error_msg_len19 equ    $ - isr_error_msg19



sched_task_offset:     dd 0x00
sched_task_selector:   dw 0x00

;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov dword [info_actual + 0], eax
    mov dword [info_actual + 4], ebx
    mov dword [info_actual + 8], ecx
    mov dword [info_actual + 12], edx
    mov dword [info_actual + 16], esi
    mov dword [info_actual + 20], edi
    mov dword [info_actual + 24], ebp
    mov dword [info_actual + 28], esp
    mov eax, [esp + 4]
    mov dword [info_actual + 32], eax
    mov ax, [esp + 0]
    mov word [info_actual + 36], ax
    mov word [info_actual + 38], ds
    mov word [info_actual + 40], es
    mov word [info_actual + 42], fs
    mov word [info_actual + 44], gs
    mov word [info_actual + 46], ss
    pushfd
    pop eax
    mov dword [info_actual + 48], eax
    mov eax, cr0
    mov dword [info_actual + 52], eax
    mov eax, cr2
    mov dword [info_actual + 56], eax
    mov eax, cr3
    mov dword [info_actual + 60], eax
    mov eax, cr4
    mov dword [info_actual + 64], eax
    mov eax, [esp + 4]
    mov dword [info_actual + 68], eax
    mov eax, [esp + 8]
    mov dword [info_actual + 72], eax
    mov eax, [esp + 12]
    mov dword [info_actual + 76], eax
    mov eax, [esp + 16]
    mov dword [info_actual + 80], eax
    mov eax, [esp + 20]
    mov dword [info_actual + 84], eax

    cmp byte [modoDebug], 1
    je .debugPantalla

    mov eax, %1
    print_text_pm isr_error_msg%1, isr_error_msg_len%1, 0xF2, 0, 0
    jmp $

.debugPantalla:
    call matarTareaActual     ;mato la tarea que produjo la excepcion (Ya esta chequeado)
    mov byte [modoDebugPantalla], 1
    call imprimirPantallaDebug


    mov eax, %1     ; imprimo mensaje de error
    print_text_pm isr_error_msg%1, isr_error_msg_len%1, 0xF2, 0, 0

    mov ax, 0000000011100000b ;selector de segmento de idle indice 28
    str cx
    cmp ax, cx
    je .fin
    mov [sched_task_selector], ax
    jmp far [sched_task_offset]

    .fin:
    xchg bx, bx
    jmp $

%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19


;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
    pushad
    call pic_finish1    ;aviso que atiendo la interrupcion
    cmp byte [modoDebugPantalla], 1
    je .fin
    call nextClock      ;avanzo reloj

    ;jne .proximaTarea
    ;comentar proxima linea para no saltar siempre a la idle
    ;mov ax, 0000000011100000b ;selector de segmento de idle indice 28
    ;str cx
    ;cmp ax, cx
    ;je .fin
    ;mov [sched_task_selector], ax
    ;jmp far [sched_task_offset]
    ;jmp .fin

.proximaTarea:
    call sched_nextTask
    str cx
    ; mov ax, 0000000011100000b ;selector de segmento de idle indice 28
    cmp ax, cx
    je .fin
    mov [sched_task_selector], ax
    jmp far [sched_task_offset]
.fin:
    popad
    iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
    pushad
    call pic_finish1                    ;aviso que atiendo la interrupcion
    in al, 0x60                         ;leo el teclado (obtengo scan code)

    xor ebx, ebx                        ;ebx = 0
    inc ebx
    inc ebx                             ;ebx = 2

    cmp  ax, 0x15                         ;veo si es la Y
    jne .ciclo

    cmp byte [modoDebug], 0            ;veo si esta apagado o prendido
    je .prender
.apagar:
    cmp byte [modoDebugPantalla], 1
    je .apagarPantalla

    mov byte [modoDebug], 0
    call borrarMensajeDebug
    jmp .fin

.prender:
    mov byte [modoDebug], 1
    call imprimirMensajeDebug
    jmp .fin

.apagarPantalla:
    mov byte [modoDebugPantalla], 0
    jmp .fin


.ciclo:
    cmp bl, 0xC                         ;veo si termine
    je .fin
    cmp al, bl                          ;si esta entre 0-9 la imprimo
    je .imprimir
    inc bl
    jmp .ciclo

.imprimir:

    dec eax
    push eax
    push eax
    call imprimirNumero
    pop eax
    pop eax
.fin:
    popad
    iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
;read
global _isrRead
_isrRead:
    call pic_finish1                    ;aviso que atiendo la interrupcion
    push ebx
    push eax
    call read_c
    pop eax
    pop ebx
    iret

;move
global _isrMove
_isrMove:
  push edx
  push ecx
  call move_c
  pop ecx
  pop edx
  jmp  0000000011100000b:0 ;GDT_TSS_TAREA_IDLE_DESC
  iret

;divide
global _isrDivide
_isrDivide:

    call divide_c
    jmp 0000000011100000b:0 ;GDT_TSS_TAREA_IDLE_DESC

    iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
    pushad
    inc DWORD [isrNumber]
    mov ebx, [isrNumber]
    cmp ebx, 0x4
    jl .ok
    mov DWORD [isrNumber], 0x0
    mov ebx, 0
.ok:
    add ebx, isrClock
    print_text_pm ebx, 1, 0x0f, 49, 79
    popad
    ret
