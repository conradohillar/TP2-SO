GLOBAL cpu_vendor
GLOBAL read_keyboard
GLOBAL get_hours
GLOBAL get_minutes
GLOBAL get_seconds
GLOBAL get_registers
GLOBAL beep_asm
EXTERN nanosleep

section .text
	
cpu_vendor:
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

read_keyboard:
	push rbp
	mov rbp, rsp

	xor rax,rax

    in al,60h

	mov rsp, rbp
	pop rbp
    ret

get_hours:
    cli
	xor rax, rax
    mov al, 4
    out 70h, al
    in al, 71h
    sti
    ret

get_minutes:
    cli
	xor rax, rax
    mov al, 2
    out 70h, al
    in al, 71h
    sti

    ret

get_seconds:
    cli
	xor rax, rax
    mov al, 0
    out 70h, al
    in al, 71h
    sti

    ret

get_registers:
	push rbp
	mov rbp,rsp

	add rsp,rsi		; salto relativo al ultimo registro pusheado (r15)

	mov rcx, 0

.loop:
	cmp rcx, 16
	jge .ret
	mov rbx, qword[rsp + 8*rcx]
	mov qword[rdi], rbx
	inc rcx
	add rdi, 8
	jmp .loop

.ret:
	mov rbx, [rsp + 8*18]
	mov qword[rdi], rbx		; cargo el valor de rsp (3 direcciones antes del rip en el stack)

	mov rsp, rbp
	pop rbp
	ret


beep_asm:
	push rbp
	mov rbp,rsp

	push rdx
	mov rdx,0

    ; Calcular el divisor
    mov rax, 1193180	; PIT_FREQUENCY
	cqo
	idiv rsi			; divisor = PIT_FREQUENCY / frec

	mov rdx,rax
	xor rax,rax

    ; Configurar el PIT canal 2 (tono del beep)
    mov al, 0xB6
    out 0x43, al
    mov rax, rdx  		; recuperar el divisor
    out 0x42, al           ; Enviar byte bajo del divisor
    mov al, ah
    out 0x42, al           ; Enviar byte alto del divisor

    ; Habilitar el altavoz
    in al, 0x61
    or al, 0x03            ; Habilitar el canal 2 del PIT y el altavoz
    out 0x61, al


	call nanosleep

    ; Deshabilitar el altavoz
    in al, 0x61
    and al, 0xFC           ; Deshabilitar el canal 2 del PIT y el altavoz
    out 0x61, al

	pop rdx

	mov rsp,rbp
	pop rbp
    ret

