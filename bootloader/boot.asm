[org 0x7c00]    ;tells the program that it originates from mem address 0x7c00 (the boot sector)
mov ah, 0x0e    ;switches the bios to teletype mode

mov bx, Booting
printBooting:
    mov al, [bx]
    cmp al, 0x00
    je startCycle
    int 0x10    ;prints character in al
    inc bx  ;increments bx by 1, moves the next character into bx
    jmp printBooting

Booting:
    db "Booting", 0x00

startCycle:
    mov bx, cycleChars
    jmp cycle

cycle:
    mov al, [bx]
    int 0x10
    inc bx
    cmp al, 0x00
    je postCycle
    ;   next part related to pausing for a quater second was written by google ai because i'm too stupid to figure it out myself, i almost understand it though
    mov ah, 86h ;switches the bios to wait mode
    mov al, 00h ;clears the al register to stop the bios going mad
    mov cx, 0003h   ;high bit (first half of the number) of 250000 microseconds
    mov dx, 0D090h  ;low bit (second half of the number) of 250000 microseconds
    int 15h ;tells the bios to wait for 250000 microseconds
    mov ah, 0x0e    ;switches the bios back to teletype mode
    ;
    mov al, 0x08
    int 0x10
    jmp cycle

postCycle:
    mov bx, cycleChars
    mov al, 0x08
    int 0x10
    jmp cycle
    
cycleChars:
    db "|/-\", 0x00

times 510-($-$$) db 0x00   ;fills 510 bytes - the length of the previous bytes with 0s, this leaves 2 bytes for the magic number 55aa
db 0x55, 0xaa   ;the magic number
