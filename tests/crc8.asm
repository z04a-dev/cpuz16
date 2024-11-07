;; simple crc8 (GSM-A) algorithm.
;; works only in bytecode
@VALUE imm = $beef;
@INIT imm = $0000;
@POLY imm = $1D00;
@XOROUT imm = $0000;
start:
	;; get CRC-8 of 0xBEEF (48879)
	mov rax, @VALUE; ;; put num to rax
	mov a1, @INIT; ;; init value of crc is 0 (compliant with CRC-8\GSM-A)
	call crc8;
	xor a1, @XOROUT;
end;

;; crc8 accepts num in rax
;; return crc in a1
crc8:
	;; poly is 0x1D00 (7424) which is compliant with CRC-8\GSM-A
	mov a3, @POLY; ;; put poly value in a3
	xor a1, rax; ;; xor crc with num
	call bitwise;
	ror a1, 8; ;; move from big endian to little endian
	ret;
end;

bitwise:
	inc rbx; ;; rbx is counter (1-16)
	call cmp;
	jlt rbx, 16, bitwise; ;; repeat for every bit
	ret;
end;

cmp:
	mov rdx, a1;
	;; check if MSB (most significant bit) is set.
	and rdx, $8000; ;; rdx & 0x8000
	jne rdx, 0, rdxjne; ;; if (crc & 0x8000) != 0
	rol a1, 1; ;; else crc <<= 1
	ret;
end;

rdxjne:
	rol a1, 1; ;; crc <<= 1
	xor a1, a3; ;; crc ^= poly
	ret;
end;
