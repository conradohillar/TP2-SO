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
GLOBAL sys_create_process_asm
GLOBAL sys_kill_asm
GLOBAL sys_wait_asm
GLOBAL sys_waitpid_asm
GLOBAL sys_block_asm
GLOBAL sys_unblock_asm
GLOBAL sys_getpid_asm
GLOBAL sys_ps_asm
GLOBAL sys_free_ps_asm
GLOBAL sys_set_priority_asm
GLOBAL sys_sem_init_asm
GLOBAL sys_sem_wait_asm
GLOBAL sys_sem_post_asm
GLOBAL sys_sem_destroy_asm
GLOBAL sys_sem_open_asm
GLOBAL sys_yield
GLOBAL sys_mem_status_asm
GLOBAL sys_set_fd_asm
GLOBAL sys_create_pipe_asm
GLOBAL sys_destroy_pipe_asm


%macro pushState 0
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
	pop rdx
	pop rcx
	pop rbx
%endmacro


sys_read_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 0      ;id syscall read

    int 80h

    popState

    mov rsp, rbp
    pop rbp
    ret

sys_write_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov r9, r8      ;back_color
    mov r8, rcx     ;fore_color
    mov rcx, rdx    ;size
    mov rdx, rsi    ;buffer
    mov rsi, rdi    ;fd
    mov rdi, 1      ;id syscall write

    int 80h

	popState

    mov rsp, rbp
    pop rbp

    ret

sys_write_at_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov r9, r8                  ;scale
    mov r8, rcx                 ;pos
    mov rcx, rdx                ;color
    mov rdx, rsi                ;size
    mov rsi, rdi                ;buffer
    mov rdi, 2                  ;id syscall write

    int 80h

    add rsp, 2 * 8

	popState

    mov rsp, rbp
    pop rbp

    ret

sys_clear_screen_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdi, 3                  ;id syscall clear_screen

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_get_time_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi
    mov rdi, 4                  ;id syscall read

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_get_registers_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdi, 5                  ;id 

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_set_text_size_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                ;size
    mov rdi, 6                  ;id 

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_put_rectangle_asm:
    push rbp
    mov rbp, rsp

    pushState

    mov r9, r8
    mov r8, rcx
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 7                  ;id sysput_rectangle

    int 80h

    popState

    mov rsp, rbp
    pop rbp
    ret

sys_get_ticks_asm:
    push rbp
    mov rbp, rsp

    pushState

    mov rdi, 8                  ;id syscall get_ticks
    int 80h

    popState

    mov rsp, rbp
    pop rbp
    ret
inopcode_excep_asm:
    UD2
sys_check_keyboard_asm:
    push rbp
    mov rbp, rsp

    pushState

    mov rsi, rdi                ;buffer
    mov rdi, 9                  ;id syscall check keyboard

    int 80h

    popState

    mov rsp, rbp
    pop rbp
    ret

sys_make_sound_asm:
    push rbp
    mov rbp, rsp

    pushState

    mov rdx,rsi                ;freq 
    mov rsi,rdi                ;millis
    mov rdi,10                 ;id syscall make_sound

    int 80h

    popState

    mov rsp, rbp
    pop rbp
    ret


sys_sleep_asm:
    push rbp
    mov rbp, rsp

    pushState

    mov rdx,rsi                ;millis 
    mov rsi,rdi                ;secs
    mov rdi,11                 ;id syscall sleep

    int 80h

    popState

    mov rsp, rbp
    pop rbp
    ret

sys_create_process_asm:
    push rbp
    mov rbp, rsp

    pushState

    mov r9, r8
    mov r8, rcx
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 12                  ;id syscall create_process

    int 80h

    popState

    mov rsp, rbp
    pop rbp
    ret
    
sys_kill_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi
    mov rdi, 13                  ;id syscall kill

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_wait_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdi, 14                  ;id syscall wait

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_waitpid_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi
    mov rdi, 15                 ;id syscall wait

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_block_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi
    mov rdi, 16                  ;id syscall block

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_unblock_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi
    mov rdi, 17                  ;id syscall unblock

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_getpid_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdi, 18                  ;id syscall getpid

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_ps_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdi, 19                  ;id syscall ps

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret  

sys_free_ps_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                 ; ps
    mov rdi, 20                  ;id syscall free_ps

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_set_priority_asm:
    push rbp    
    mov rbp, rsp

    pushState
    
    mov rdx, rsi
    mov rsi, rdi                ;size
    mov rdi, 21                ;id syscall set_priority

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret   

sys_sem_init_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdx, rsi                 ; count
    mov rsi, rdi                 ; id
    mov rdi, 22                  ;id syscall sem_init

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret 

sys_sem_wait_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                 ; sem
    mov rdi, 23                  ;id syscall sem_wait

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret 

sys_sem_post_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                 ; sem
    mov rdi, 24                  ;id syscall sem_post

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret 

sys_sem_destroy_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                 ; sem
    mov rdi, 25                  ;id syscall sem_destroy

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret 

sys_sem_open_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                 ; sem
    mov rdi, 26                  ;id syscall sem_open

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret 

sys_yield:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                 ; sem
    mov rdi, 27                  ;id syscall yield

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret 

sys_mem_status_asm:
    push rbp
    mov rbp, rsp

    pushState

    mov rdi, 31

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_set_fd_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdx, rsi                 
    mov rsi, rdi                 
    mov rdi, 28                  ;id syscall set_fd

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_create_pipe_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rdi, 29                  ;id syscall create_pipe

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret

sys_destroy_pipe_asm:
    push rbp    
    mov rbp, rsp

    pushState

    mov rsi, rdi                 
    mov rdi, 30                  ;id syscall destroy_pipe

    int 80h

    popState

    mov rsp, rbp
    pop rbp

    ret