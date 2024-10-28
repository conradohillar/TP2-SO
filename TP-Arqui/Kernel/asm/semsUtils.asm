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
    mov byte[rdi], 1    ; *lock in rdi
    ret 