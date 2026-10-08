[org 0x7c00]    ;tells the program that it originates from mem address 0x7c00 (the boot sector)

mov [diskNum], dl

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
    inc [loopCount]   ;increments the value stored in loopCount
    cmp [loopCount], 5    ;to change how many times to cycle, number of desired cycles * 2, final result must be an integer
    je setupFBooting    ;to cycle endlessly delete all related code to printing "Finished Booting"
    mov bx, cycleChars
    mov al, 0x08
    int 0x10
    jmp cycle
    
cycleChars:
    db "|/-\", 0x00

setupFBooting:
    mov bx, FBooting
    jmp printFBooting

printFBooting:
    mov al, [bx]
    cmp al, 0x00
    je setupBConfirm
    int 0x10
    inc bx
    jmp printFBooting

FBooting:
    db 0x0a, 0x0d, "Finished Booting", 0x00   ;newline, carriage return, "Finished Booting", null

setupBConfirm:
    mov bx, BConfirmChar
    jmp printBConfirm

printBConfirm:
    mov al, [bx]
    cmp al, 0x00
    je BConfirm
    int 0x10
    inc bx
    jmp printBConfirm

BConfirm:   ;this is only here to follow the tutorial
    mov ah, 0   ;sets the bios to input mode
    int 0x16    ;waits for the next input from the user, puts the scancode into ah and the ascii character in al
    mov ah, 0x0e
    int 0x10    ;print the character you input as this in not done automatically
    cmp al, "Y"
    je stackSetup
    cmp al, "y"
    je stackSetup
    jmp setupBConfirm

BConfirmChar:
    db 0x0a, 0x0d, "Press Y To Start Kernel ", 0x00

stackSetup: ;used to setup and test the stack
    mov bp, 0x8000  ;sets botton and top of stack
    mov sp, 0x8000
    mov bh, 5
    push bx ;pushes the 5 to the stack
    mov bh, 6   ;change bh to 6
    pop bx  ;removes the top of the stack and puts it  in bx
    cmp bh, 5
    je diskSetup
    jmp stackSetup

diskSetup:
    mov ax, 0
    mov es, ax  ;part 1 of where to store it in memory
    mov ah, 2   ;tells computer to read from disk
    mov al, 1   ;number of sectors to read
    mov ch, 0   ;cyliner to read from
    mov cl, 2   ;sector number
    mov dh, 0   ;head number
    mov dl, [diskNum]   ;disk number for hdd/ssd
    mov bx, 0x7e00  ;part 2 of where to store it in memory
    int 0x13    ;reads from the disk
    mov al, [0x7e00]
    cmp al, "a"
    je Fin
    jmp diskSetup

Fin:
    mov ah, 0x0e
    mov al, "a"
    int 0x10
    jmp $

diskNum:
    db 0x00

loopCount:
    db 0x00

times 510-($-$$) db 0x00   ;fills 510 bytes - the length of the previous bytes with 0s, this leaves 2 bytes for the magic number 55aa
db 0x55, 0xaa   ;the magic number
times 512 db "a"