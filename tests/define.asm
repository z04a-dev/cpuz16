;; hmm..

@SIZE imm = $beef;
@TEST imm = 170;

;; should it be \0 terminated?
;; HELLO will get processed to memory pointer in ROM
;; oh, we can't read from rom?...
@HELLO ascii = "Hello, World!";
@HELLO_SIZE imm = 14;
;; RAW will get processed to memory pointer in ROM
@RAW data = {$beef, 5000, $AAAA, $FFFF};
@RAW_SIZE imm = 4;

start:
	mov rbx, @SIZE;
	mov a2, @HELLO;
	;; I've lost 5 hours of debugging, because i put x3 here instead of a3.
	mov a3, @TEST;
	halt;
end;
