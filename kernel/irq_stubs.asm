[bits 32]

extern irq_handler

%macro IRQ 2
global irq%1
irq%1:
    cli
    push dword 0
    push dword %2
    jmp irq_common
%endmacro

IRQ 0, 32   ; Timer
IRQ 1, 33   ; Keyboard
IRQ 2, 34   ; Cascade
IRQ 3, 35   ; COM2
IRQ 4, 36   ; COM1
IRQ 5, 37   ; LPT2
IRQ 6, 38   ; Floppy
IRQ 7, 39   ; LPT1
IRQ 8, 40   ; RTC
IRQ 9, 41   ; Free
IRQ 10, 42  ; Free
IRQ 11, 43  ; Free
IRQ 12, 44  ; PS/2 Mouse
IRQ 13, 45  ; Coprocessor
IRQ 14, 46  ; Primary ATA
IRQ 15, 47  ; Secondary ATA

irq_common:
    pusha
 
    mov ax, ds
    push eax
 
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
 
    push esp
    call irq_handler
    add esp, 4
 
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa
    add esp, 8
    iret
