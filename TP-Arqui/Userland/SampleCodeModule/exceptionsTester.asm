
GLOBAL inopcode_excep_asm
GLOBAL divzero_excep_asm

inopcode_excep_asm:
    UD2

divzero_excep_asm:
    mov rcx, 0
    div rcx