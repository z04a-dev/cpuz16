;; hmm..

@SELF imm = 50;
@TEST imm = 170;

;; should it be \0 terminated?
;; HELLO will get processed to memory pointer in ROM
;; oh, we can't read from rom?...
@HELLO ascii = "Hello, World!";
@HELLO_SIZE imm = 14;
;; RAW will get processed to memory pointer in ROM
@RAW data = {#beef, 5000, #AAAA, #FFFF};
@RAW_SIZE imm = 4;

start:
	inc rax;
	inc rax;
	mov rbx, @SELF;
	mov x3, @TEST;
	halt;
end;
