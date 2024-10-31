;; first program to interact with I/O

@WRITE_PORT imm = #0000; ;; fprintf to /dev/pts/7
@RAISE_CPU_STATE imm = #0001; ;; print_cpu_state to /dev/pts/7
@CHAR_PORT imm = #0002;
@HEX_PORT imm = #0003;
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

;; OR

@HELLOWORLD ascii = "Hello, World!";
@HELLOWORLD_SIZE imm = 13


@MADEBY ascii = "CPUZ16 is made by z04a!";
@MADEBY_SIZE imm = 23

@MADEBY_2 ascii = "Please enjoy!";
@MADEBY_2_SIZE imm = 13

@START_ROM imm = #4000;
@END_ROM imm = #4080;

infinite:
	nop;
	inc a3;
	jge a3, 11, infinite;
	jmp infinite_send;
	halt;
end;

@ZERO imm = 48;
@NINE imm = 58;

infinite_send:
	inc rdx;
	jlt rdx, @NINE, infinite_send_1;
	mov rdx, @ZERO;
	jmp infinite_send_1;
end;

infinite_send_1:
	;; mov rax, @START_ROM;
	;; mov rdx, @ZERO;
	sv a1, rdx;
	sv a1, @SPACE;

	mov rax, @MADEBY; ;; ascii pointer
	add rax, @START_ROM;
	mov rbx, @MADEBY_SIZE; ;; ascii size
	call print;
	call CRLF;

	mov rax, @MADEBY_2; ;; ascii pointer
	add rax, @START_ROM;
	mov rbx, @MADEBY_2_SIZE; ;; ascii size
	call print;
	call CRLF;
	call CRLF;
	;; call CRLF;
	;; call print_rom;
	mov a3, 0;
	jmp infinite;
end;

;; rax <- pointer to acsii
;; rbx <- size of ascii
print:
	push rdx;
	push a2; ;; a2 for character
	mov rdx, rax;
	add rdx, rbx; ;; rdx -> pos of last char
	call _print;
	pop a2;
	pop rdx;
	ret;
end;

_print:
	lv a2, rax; ;; get character
	sv a1, a2; ;; print from first to last-1 character
	inc rax;
	jlt rax, rdx, _print;

	;; lv a2, rax; ;; get character
	;; sv a1, a2; ;; print last character
	ret;
end;

start:
	mov a1, @CHAR_PORT;
	;;mov a1, @WRITE_PORT;
	mov rax, @START_ROM;
	;;call print_rom;
	;;call CRLF;
	mov rdx, @ZERO;
	jmp infinite;
	halt;
	;; print cpu state
	;; mov a1, @RAISE_CPU_STATE;
	;; sv a1, #FFFF;
	halt;
end;

by_char:
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
	halt;
end;

CRLF:
	push a1;
	mov a1, @CHAR_PORT;
	sv a1, @CR;
	sv a1, @LF;
	pop a1;
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
