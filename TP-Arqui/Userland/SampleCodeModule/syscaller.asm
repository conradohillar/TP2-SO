section .text

GLOBAL sys_read_asm
GLOBAL sys_write_asm
GLOBAL sys_write_at_asm
GLOBAL sys_clear_screen_asm
GLOBAL sys_get_time_asm
GLOBAL sys_get_registers_asm
GLOBAL sys_set_text_size_asm
GLOBAL sys_put_rectangle_asm
GLOBAL sys_get_ticks_asm
GLOBAL sys_check_keyboard_asm
GLOBAL sys_make_sound_asm
GLOBAL sys_sleep_asm



sys_read_asm:
    push rbp    
    mov rbp, rsp

    call pushState

    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 0      ;id syscall read

    int 80h

    call popState

    mov rsp, rbp
    pop rbp
    ret

sys_write_asm:
    push rbp    
    mov rbp, rsp

    call pushState

    mov r9, r8      ;back_color
    mov r8, rcx     ;fore_color
    mov rcx, rdx    ;size
    mov rdx, rsi    ;buffer
    mov rsi, rdi    ;fd
    mov rdi, 1      ;id syscall write

    int 80h

	call popState

    mov rsp, rbp
    pop rbp

    ret

sys_write_at_asm:
    push rbp    
    mov rbp, rsp

    call pushState

    mov r9, r8                  ;scale
    mov r8, rcx                 ;pos
    mov rcx, rdx                ;color
    mov rdx, rsi                ;size
    mov rsi, rdi                ;buffer
    mov rdi, 2                  ;id syscall write

    int 80h

    add rsp, 2 * 8

	call popState

    mov rsp, rbp
    pop rbp

    ret

sys_clear_screen_asm:
    push rbp    
    mov rbp, rsp

    call pushState

    mov rdi, 3                  ;id syscall clear_screen

    int 80h

    call popState

    mov rsp, rbp
    pop rbp

    ret

sys_get_time_asm:
    push rbp    
    mov rbp, rsp

    call pushState

    mov rsi, rdi
    mov rdi, 4                  ;id syscall read

    int 80h

    call popState

    mov rsp, rbp
    pop rbp

    ret

sys_get_registers_asm:
    push rbp    
    mov rbp, rsp

    call pushState

    mov rdi, 5                  ;id 

    int 80h

    call popState

    mov rsp, rbp
    pop rbp

    ret

sys_set_text_size_asm:
    push rbp    
    mov rbp, rsp

    call pushState

    mov rsi, rdi                ;size
    mov rdi, 6                  ;id 

    int 80h

    call popState

    mov rsp, rbp
    pop rbp

    ret

sys_put_rectangle_asm:
    push rbp
    mov rbp, rsp

    call pushState

    mov r9, r8
    mov r8, rcx
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 7                  ;id syscall put_rectangle

    int 80h

    call popState

    mov rsp, rbp
    pop rbp
    ret

sys_get_ticks_asm:
    push rbp
    mov rbp, rsp

    call pushState

    mov rdi, 8                  ;id syscall get_ticks
    int 80h

    call popState

    mov rsp, rbp
    pop rbp
    ret
inopcode_excep_asm:
    UD2
sys_check_keyboard_asm:
    push rbp
    mov rbp, rsp

    call pushState

    mov rsi, rdi                ;buffer
    mov rdi, 9                  ;id syscall check keyboard

    int 80h

    call popState

    mov rsp, rbp
    pop rbp
    ret

sys_make_sound_asm:
    push rbp
    mov rbp, rsp

    call pushState

    mov rdx,rsi                ;freq 
    mov rsi,rdi                ;millis
    mov rdi,10                 ;id syscall make_sound

    int 80h

    call popState

    mov rsp, rbp
    pop rbp
    ret


sys_sleep_asm:
    push rbp
    mov rbp, rsp

    call pushState

    mov rdx,rsi                ;millis 
    mov rsi,rdi                ;secs
    mov rdi,11                 ;id syscall make_sound

    int 80h

    call popState

    mov rsp, rbp
    pop rbp
    ret
    

pushState:
    push rbp
    mov rbp,rsp

	push rbx
	push rcx
	push rdx
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

    mov rsp,rbp
    pop rbp
    ret

popState:
    push rbp
    mov rbp,rsp

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
	pop rdx
	pop rcx
	pop rbx

    mov rsp,rbp
    pop rbp
    ret
