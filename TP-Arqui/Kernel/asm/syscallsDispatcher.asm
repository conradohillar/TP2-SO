EXTERN sys_read
EXTERN sys_write
EXTERN sys_write_at
EXTERN sys_clear_screen
EXTERN sys_get_time
EXTERN sys_get_registers
EXTERN sys_set_text_size
EXTERN sys_put_rectangle
EXTERN sys_get_ticks
EXTERN sys_check_keyboard
EXTERN sys_make_sound
EXTERN sys_sleep
EXTERN sys_create_process
EXTERN sys_kill
EXTERN sys_block
EXTERN sys_unblock

GLOBAL syscalls_dispatcher

section .text

syscalls_dispatcher:
    sti
    push rbp
    mov rbp, rsp

    cmp rdi, 0
    je read
    cmp rdi, 1
    je write
    cmp rdi, 2
    je write_at
    cmp rdi, 3
    je clear
    cmp rdi, 4
    je get_time
    cmp rdi, 5
    je registers
    cmp rdi, 6
    je set_text_size
    cmp rdi, 7
    je put_rectangle
    cmp rdi, 8
    je get_ticks
    cmp rdi, 9
    je check_keyboard
    cmp rdi,10
    je make_sound
    cmp rdi,11
    je sleep
    cmp rdi,12
    je create_process
    cmp rdi,13
    je kill
    cmp rdi,14
    je block
    cmp rdi,15
    je unblock

end:
    cli
    mov rsp, rbp
    pop rbp
    iretq


read:
    mov rdi, rsi                ;fd
    mov rsi, rdx                ;buffer
    mov rdx, rcx                ;size
    
    call sys_read
    jmp end 


write:
    mov rdi, rsi                ;fd
    mov rsi, rdx                ;buffer
    mov rdx, rcx                ;size
    mov rcx, r8                 ;fore_color
    mov r8, r9                  ;back_color
    
    call sys_write

    jmp end   


write_at:
    mov rdi, rsi                ;buffer
    mov rsi, rdx                ;size
    mov rdx, rcx                ;color
    mov rcx, r8                 ;pos
    mov r8, r9                  ;scale
    
    call sys_write_at
    add rsp, 8

    jmp end  
 

clear:
    call sys_clear_screen
    jmp end 


get_time:
    mov rdi, rsi                ;buffer

    call sys_get_time
    jmp end 


registers:
    call sys_get_registers
    jmp end 


set_text_size:
    mov rdi, rsi                ;scale

    call sys_set_text_size
    jmp end 


put_rectangle:
    mov rdi, rsi                ;top_left_x
    mov rsi, rdx                ;top_left_y
    mov rdx, rcx                ;bottom_right_x
    mov rcx, r8                 ;bottom_right_y
    mov r8, r9                  ;color
    
    call sys_put_rectangle

    jmp end

get_ticks:
    call sys_get_ticks

    jmp end

check_keyboard:
    mov rdi, rsi                ;buffer

    call sys_check_keyboard

    jmp end

make_sound: 
    mov rdi,rsi     ; duration
    mov rsi,rdx     ; frequency

    call sys_make_sound

    jmp end

sleep: 
    mov rdi,rsi     ; duration
    mov rsi,rdx     ; frequency

    call sys_sleep

    jmp end

create_process:
    mov rdi, rsi                ;code
    mov rsi, rdx                ;argc
    mov rdx, rcx                ;argv
    mov rcx, r8                 ;name
    mov r8, r9                  ;in_fg
    
    call sys_create_process

    jmp end

kill:
    mov rdi, rsi                ;pid
    call sys_kill
    jmp end 

block:
    mov rdi, rsi                ;pid
    call sys_block
    jmp end 

unblock:
    mov rdi, rsi                ;pid
    call sys_unblock
    jmp end 