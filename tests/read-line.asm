;; program that reads from tty
;; then after you press RETURN in tty
;; returns input from tty (now in reverse order)

;; half broken

;; No implementation of COUNT_PORT.
@WRITE_ADDR imm = $0000;
@READ_ADDR imm = $0001;

@CHAR_Q imm = 113;

@RAM_COUNTER imm = 100; ;; <- that's where number of received characters will be
@RAM_LINE_COUNTER imm = 101;

@CR imm = 13;
@LF imm = 10;

@BSLASH imm = 92;
@SPACE imm = 32;

@RAM_LINE_START imm = 102;

;; a1 <- last character from serial

start:
	;; init block
	call NEWLINE;
	mov a3, @READ_ADDR; ;; <- put read_address to a3;
	call poll;
	lv rax, @RAM_COUNTER;
	dec rax; ;; don't count 'q'
	halt;
end;

NEWLINE:
	call CRLF;
	mov a3, @WRITE_ADDR;
	sv a3, @BSLASH;
	sv a3, @SPACE;
	ret;
end;

CRLF:
	mov a3, @WRITE_ADDR;
	sv a3, @CR;
	sv a3, @LF;
	mov a3, @READ_ADDR;
	ret;
end;

;; if rax == 1, then character received
poll:
	call read;
	mov rax, @RAM_COUNTER;
	lv rdx, @RAM_COUNTER;
	inc rdx;
	sv rax, rdx;
	jeq a1, @CHAR_Q, return;
	jne a1, @CR, poll;
	call write;
	jmp poll;
end;

;; a2 <- temporary for character
read:
	lv a2, @READ_ADDR; ;; <- read
	jeq a2, 0, return;
	jeq a2, a1, return;
	;; character received
	mov a1, a2; ;; <- push character from temporary registry
	lv rax, @RAM_LINE_COUNTER;
	mov rbx, @RAM_LINE_START;
	add rbx, rax;
	sv rbx, a1;
	inc rax;
	mov rbx, @RAM_LINE_COUNTER;
	sv rbx, rax;
	jeq a2, @CR, read_newline;
	jmp return;
end;

read_newline:
	call CRLF;
	jmp return;
end;

write:
	push a3;
	mov a3, @WRITE_ADDR;
	lv rax, @RAM_LINE_COUNTER;
	mov rbx, @RAM_LINE_START;
	add rbx, rax;
	call loop;
	;; sv a3, 0; ;; <- reset serial
	pop a3;
	jmp return;
end;

loop:
	lv a1, rbx;
	sv a3, a1; ;; <- write to serial
	dec rbx;
	jge rbx, @RAM_LINE_START, loop;
	mov rbx, @RAM_LINE_COUNTER;
	sv rbx, 0;
	ret;
end;

return:
	ret;
end;
