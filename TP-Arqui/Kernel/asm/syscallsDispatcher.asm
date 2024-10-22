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
EXTERN sys_wait
EXTERN sys_waitpid
EXTERN sys_block
EXTERN sys_unblock
EXTERN sys_getpid
EXTERN sys_ps
EXTERN sys_free_ps
EXTERN sys_set_priority
EXTERN sys_sem_init
EXTERN sys_sem_wait
EXTERN sys_sem_post
EXTERN sys_sem_destroy

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
    je my_wait
    cmp rdi,15
    je waitpid
    cmp rdi,16
    je block
    cmp rdi,17
    je unblock
    cmp rdi,18
    je getpid
    cmp rdi,19
    je ps
    cmp rdi,20
    je free_ps
    cmp rdi,21
    je set_priority
    cmp rdi,22
    je sem_init
    cmp rdi,23
    je sem_wait
    cmp rdi,24
    je sem_post
    cmp rdi,25
    je sem_destroy

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

my_wait:
    call sys_wait
    jmp end 

waitpid:
    mov rdi, rsi                ;pid
    call sys_waitpid
    jmp end 

block:
    mov rdi, rsi                ;pid
    call sys_block
    jmp end 

unblock:
    mov rdi, rsi                ;pid
    call sys_unblock
    jmp end 

getpid:
    call sys_getpid
    jmp end

ps:
    call sys_ps
    jmp end 

free_ps:
    mov rdi, rsi                ;ps
    call sys_free_ps
    jmp end 

set_priority:
    mov rdi, rsi                ;pid
    mov rsi, rdx                ;priority
    call sys_set_priority
    jmp end

sem_init:
    mov rdi, rsi                ;count
    mov rsi, rdx                ;id
    call sys_sem_init
    jmp end

sem_wait:
    mov rdi, rsi                ;id
    call sys_sem_wait
    jmp end

sem_post:
    mov rdi, rsi                ;id
    call sys_sem_post
    jmp end

sem_destroy:
    mov rdi, rsi                ;id
    call sys_sem_destroy
    jmp end
