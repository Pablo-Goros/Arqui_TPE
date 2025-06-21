GLOBAL _hlt
GLOBAL _cli
GLOBAL _sti

GLOBAL picMasterMask
GLOBAL picSlaveMask

GLOBAL haltcpu

GLOBAL load_idtr

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _irqWriteHandler
GLOBAL _syscallHandler

GLOBAL _int80Handler
GLOBAL get_registers
GLOBAL _exception0Handler ; Zero Division Exception
GLOBAL _exception6Handler ; Invalid Opcode Exception

EXTERN int_write
EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN sysCallDispatcher
EXTERN getStackBase  

EXTERN last_context_regs

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
    pushState

    mov rdi, %1 ; pasaje de parametro
    call exceptionDispatcher

    popState
    
    ; Now set up a proper iretq frame to return to userland/shell
    push qword 0x0      ; SS
    call getStackBase
    push rax            ; RSP - stack base address
    pushf               ; RFLAGS
    push qword 0x8      ; CS - assuming code segment is 0x8
    mov rax, userland
    push rax            ; RIP - return to userland/shell entry point
    
    iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

	
; --- Carga la IDT desde [RDI] (pointer a estructura {limit, base})
load_idtr:
    ; Memoria en [RDI] = {uint16_t limit; uint64_t base;}
    lidt    [rdi]
    ret


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


_int80Handler:
	pushState
	mov rdi, last_context_regs ; guarda en rdi la direccion del arreglo para copiar el contexto
	mov rsi, rsp ; rsi apunta a la pila actual (contexto de la interrupcion)
	call get_registers_from_context

	mov r9, r8
	mov r8, r10
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi 
	mov rdi, rax
	
	call sysCallDispatcher
	mov [aux], rax
	popState
	mov rax, [aux]
	iretq

	;Zero Division Exception
_exception0Handler:
	mov rdi, 0
	exceptionHandler 0
	iretq

	; Invalid Opcode Exception
_exception6Handler:
    mov   rdi, 6
    exceptionHandler 6
    iretq


haltcpu:
	cli
	hlt
	ret

; get_registers:
;     mov [rdi],rax
;     mov [rdi + 8], rbx
;     mov [rdi + 16], rcx
;     mov [rdi + 24], rdx
;     mov [rdi + 32], rsi
;     mov [rdi + 40], rdi
;     mov [rdi + 48], rbp
;     mov [rdi + 56], rsp
;     mov [rdi + 64], r8
;     mov [rdi + 72], r9
;     mov [rdi + 80], r10
;     mov [rdi + 88], r11
;     mov [rdi + 96], r12
;     mov [rdi + 104], r13
;     mov [rdi + 112], r14
;     mov [rdi + 120], r15

;     call .get_rip

; .get_rip:
;     pop rax ; get return address (RIP)
;     mov [rdi + 128], rax ; store it in the registers structure
;     pushfq
;     pop rax
;     mov [rdi + 136], rax ; store RFLAGS in the registers structure
;     ret

get_registers_from_context:
    mov rdx, rsi         ; rdx = context (puntero a la pila)

    ; Copiar los registros en el mismo orden que los pusheaste
    mov rax, [rdx + 0]      ; RAX
    mov [rdi + 0], rax		; guarda lo que hay en rax en 
    mov rax, [rdx + 8]      ; RBX
    mov [rdi + 8], rax
    mov rax, [rdx + 16]     ; RCX
    mov [rdi + 16], rax
    mov rax, [rdx + 24]     ; RDX
    mov [rdi + 24], rax
    mov rax, [rdx + 32]     ; RBP
    mov [rdi + 32], rax
    mov rax, [rdx + 40]     ; RDI
    mov [rdi + 40], rax
    mov rax, [rdx + 48]     ; RSI
    mov [rdi + 48], rax
    mov rax, [rdx + 56]     ; R8
    mov [rdi + 56], rax
    mov rax, [rdx + 64]     ; R9
    mov [rdi + 64], rax
    mov rax, [rdx + 72]     ; R10
    mov [rdi + 72], rax
    mov rax, [rdx + 80]     ; R11
    mov [rdi + 80], rax
    mov rax, [rdx + 88]     ; R12
    mov [rdi + 88], rax
    mov rax, [rdx + 96]     ; R13
    mov [rdi + 96], rax
    mov rax, [rdx + 104]    ; R14
    mov [rdi + 104], rax
    mov rax, [rdx + 112]    ; R15
    mov [rdi + 112], rax

    ; Si tu contexto guardado en la pila incluye RIP y RFLAGS, también podés copiarlos:
    ; Por ejemplo, si después de pushState se guardan RIP y RFLAGS:
    mov rax, [rdx + 120]    ; RIP
    mov [rdi + 120], rax
    mov rax, [rdx + 128]    ; RFLAGS
    mov [rdi + 128], rax

    ret

section .bss
	aux resq 1

section .rodata
    userland equ 0x400000