;; first program to interact with I/O

;; since z16c doesn't support @DEFINE ASCII (or DATA) yet
;; i've put each letter char representation here.
@WRITE_PORT imm = #0000; ;; fprintf to /dev/pts/7
@RAISE_CPU_STATE imm = #0001; ;; print_cpu_state to /dev/pts/7
@H imm = 72;
@E imm = 69;
@L imm = 76;
@O imm = 79;
@SPACE imm = 32;
@COMMA imm = 44;
@W imm = 87;
@R imm = 82;
@D imm = 68;
@BANG imm = 33;
@CR imm = 13;
@LF imm = 10;

@START_ROM imm = #4000;
@END_ROM imm = 16435;
start:
	mov a1, @WRITE_PORT;
	mov rax, @START_ROM;
	call print_rom;
	halt;
	sv a1, @H;
	sv a1, @E;
	sv a1, @L;
	sv a1, @L;
	sv a1, @O;
	sv a1, @COMMA;
	sv a1, @SPACE;
	sv a1, @W;
	sv a1, @O;
	sv a1, @R;
	sv a1, @L;
	sv a1, @D;
	sv a1, @BANG;
	;; \r\n
	call CRLF;
	;; print cpu state
	mov a1, @RAISE_CPU_STATE;
	sv a1, #FFFF;
	halt;
end;

CRLF:
	sv a1, @CR;
	sv a1, @LF;
	ret;
end;

print_rom:
	lv a2, rax;
	sv a1, a2;
	;; call CRLF;
	inc rax;
	jle rax, @END_ROM, print_rom;
	ret;
end;
