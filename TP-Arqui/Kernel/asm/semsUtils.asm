GLOBAL acquire
GLOBAL release 

acquire: 
    mov al, 0
.retry:
    xchg [rdi], al    ; *lock in rdi
    test al, al 
    jz .retry
    ret

release:
    mov qword[rdi], 1    ; *lock in rdi
    ret 