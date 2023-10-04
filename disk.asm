disk_load:
	pusha
	push dx
	
	mov ah, 0x02
	mov al, dh ; num of sectors
	mov cl, 0x02 ; start sector
	mov ch, 0x00 ; cylinder
	mov dh, 0x00 ; head
	
	; dl = drive number is set 
	; es:bx - buffer pointer
	
	int 0x13 ; BIOS
	jc disk_loop
	
	pop dx
	cmp al, dh
	jne disk_loop
	popa
	ret

disk_loop:
	jmp $ 
	