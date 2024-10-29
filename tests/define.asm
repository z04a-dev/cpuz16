;; hmm..

@SIZE imm = #BEEF;
@TEST imm = 170;

;; should it be \0 terminated?
;; HELLO will get processed to memory pointer in ROM
;; oh, we can't read from rom?...
@HELLO ascii = "Hello, World!";
@HELLO_SIZE imm = 14;
;; RAW will get processed to memory pointer in ROM
@RAW data = {#BEEF, #0404, #AAAA, #FFFF};
@RAW_SIZE imm = 4;

start:
	inc rax;
	inc rax;
	;;mov rbx, @SIZE;
	;;mov x1, @TEST;
	halt;
end;
