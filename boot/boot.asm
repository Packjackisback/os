[org 0x7C00]
[bits 16]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov [boot_drive], dl

    mov si, msg_loading
    call print_string_rm

    mov bx, 0x1000
    mov es, bx
    xor bx, bx
    mov ah, 0x02
	mov al, 10
    mov ch, 0
	mov cl, 2
	mov dh, 0
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    mov si, msg_loaded
    call print_string_rm

    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

disk_error:
    mov si, msg_error
    call print_string_rm
    jmp hang_rm

print_string_rm:
    pusha
.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    popa
    ret

hang_rm:
    hlt
    jmp hang_rm

msg_loading db "Loading kernel...", 13, 10, 0
msg_loaded  db "Kernel loaded!", 13, 10, 0
msg_error   db "Disk read error!", 13, 10, 0
boot_drive  db 0

[bits 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp 0x10000

; GDT
gdt_start:
    dq 0x0000000000000000
gdt_code:
    dq 0x00cf9a000000ffff
gdt_data:
    dq 0x00cf92000000ffff
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510 - ($ - $$) db 0
dw 0xAA55
