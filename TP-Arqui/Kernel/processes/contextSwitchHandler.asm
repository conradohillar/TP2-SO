EXTERN schedule

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

; IDEA del context switching handler

; interrumpe el timer tick

pushState

mov rdi, rsp
call schedule      ; Se llama al scheduler para que indique qué proceso sigue, el scheduler devuelve el rsp del proceso a ejecutar en rax
mov rsp, rax

; Send EOI
mov al, 20h
out 20h, al

popState 

iretq 