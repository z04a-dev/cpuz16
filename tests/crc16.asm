;; simple crc16 (IBM-3740) algorithm.
;; works only in bytecode
start:
	;; get CRC-16 of 0xBEEF (48879)
	mov rax, #BEEF; ;; put num to rax
	mov a1, #FFFF; ;; init value of crc is 0xFFFF (compliant with CRC-16\IBM-3740)
	call crc16;
end;

;; crc16 accepts num in rax
;; return crc in a1
crc16:
	;; poly is 0x1021 (4129) which is compliant with CRC-16\IBM-3740
	mov a3, #1021; ;; put poly value in a3
	xor a1, rax; ;; xor crc with num
	call bitwise;
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
	and rdx, #8000; ;; rdx & 0x8000
	jne rdx, 0, rdxjne; ;; if (crc & 0x8000) != 0
	rol a1, 1; ;; else crc <<= 1
	ret;
end;

rdxjne:
	rol a1, 1; ;; crc <<= 1
	xor a1, a3; ;; crc ^= poly
	ret;
end;
