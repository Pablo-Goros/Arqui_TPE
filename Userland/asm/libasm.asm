GLOBAL sys_call
GLOBAL _hlt
GLOBAL _cli
GLOBAL _sti

section .text

; syscall handler
sys_call:
    mov rax, rdi        ; syscall number
    mov rdi, rsi        ; arg1
    mov rsi, rdx        ; arg2
    mov rdx, rcx        ; arg3
    mov r10, r8         ; arg4
    int 0x80
    ret

; wait for interrupt
_hlt:
	sti
	hlt
	ret

; disable interrupts
_cli:
	cli
	ret

; enable interrupts
_sti:
	sti
	ret