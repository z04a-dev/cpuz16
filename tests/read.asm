;; program that reads characters from tty, and prints them back.

@READ_ADDR imm = #0004;
@WRITE_ADDR imm = #0003;

@CHAR_Q imm = 113;

@RAM_COUNTER imm = 100; ;; <- that's where number of received characters will be

@CR imm = 13;
@LF imm = 10;

@BSLASH imm = 92;
@SPACE imm = 32;
@BACKSPACE imm = 8;
@DEL imm = 127;

@WELCOME_MSG ascii = "Serial IO example by z04a for cpuz16";
@WELCOME_MSG_SIZE imm = 36;

@ROM_START imm = #4000;

;; a1 <- last character from serial

start:
	call CRLF;
	mov rax, @WELCOME_MSG;
	add rax, @ROM_START;
	mov rbx, @WELCOME_MSG_SIZE;
	call print;
	;; init block
	call NEWLINE;
	mov a3, @READ_ADDR; ;; <- put read_address to a3;
	call poll;
	halt;
end;

NEWLINE:
	push a3;
	call CRLF;
	mov a3, @WRITE_ADDR;
	sv a3, @BSLASH;
	sv a3, @SPACE;
	pop a3;
	ret;
end;

CRLF:
	push a3;
	mov a3, @WRITE_ADDR;
	sv a3, @CR;
	sv a3, @LF;
	pop a3;
	ret;
end;

BACKSPACE:
	push a3;
	mov a3, @WRITE_ADDR;
	sv a3, @BACKSPACE; ;; go back
	sv a3, @SPACE; ;; clear
	sv a3, @BACKSPACE; ;; go back again
	pop a3;
	jmp poll;
end;

;; if rax == 1, then character received
poll:
	call read;
	jne rax, 1, poll;
	mov rax, @RAM_COUNTER;
	lv rdx, @RAM_COUNTER;
	inc rdx;
	sv rax, rdx;
	jeq a1, @CHAR_Q, quit;
	jeq a1, @CR, poll;
	jeq a1, @BACKSPACE, BACKSPACE;
	jeq a1, @DEL, BACKSPACE;

	call write;
	mov rax, 0; ;; <- reset rax;
	jmp poll;
end;

quit:
	lv rax, @RAM_COUNTER;
	dec rax; ;; don't count 'q'
	ret;
end;

;; a2 <- temporary for character
read:
	lv a2, @READ_ADDR; ;; <- read
	jeq a2, 0, return;
	jeq a2, a1, return;
	;; character received
	mov rax, 1; ;; <- indicate that we received char
	mov a1, a2; ;; <- push character from temporary registry
	jeq a2, @CR, read_newline;
	jmp return;
end;

read_newline:
	call NEWLINE;
	jmp return;
end;

write:
	push a3;
	mov a3, @WRITE_ADDR;
	sv a3, a1; ;; <- write to serial
	sv a3, 0; ;; <- reset serial
	pop a3;
	jmp return;
end;

return:
	ret;
end;

;; yoinked from write-hello.asm
;; rax <- pointer to acsii
;; rbx <- size of ascii
print:
	push a3;
	push rdx;
	push a2; ;; a2 for character
	mov a3, @WRITE_ADDR; ;; <- put read_address to a3;
	mov rdx, rax;
	add rdx, rbx; ;; rdx -> pos of last char
	call _print;
	pop a2;
	pop rdx;
	pop a3;
	ret;
end;

_print:
	lv a2, rax; ;; get character
	sv a3, a2; ;; print from first to last-1 character
	inc rax;
	jlt rax, rdx, _print;
	ret;
end;
