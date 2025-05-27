GLOBAL cpuVendor
GLOBAL get_key_asm
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

get_key_asm:
	push rbp
    mov rbp, rsp

.wait:
    in al, 0x64 ; read from keyboard controller
    test al, 0x01    ; ¿OBF = 1?    (al & 0x01)
    jz .wait ; if OB = 0, keep waiting
    ; else, get char

    xor rax, rax ; clear rax

    in al, 0x60 ; read from keyboard controller

    mov rsp, rbp
    pop rbp
    ret


