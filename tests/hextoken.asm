;; implemented hex values in tokens using # (as of 7 November 2024 '#' is deprecated.)
;; Use '$'
start:
	mov a1, $beef; ;; 0xbeef
	mov a2, $ffff;
	mov a3, $00ff;
	mov rax, #1021;
	mov rbx, $8000;
	mov rdx, $FF00;
	mov ins, 5;
	mul ins, 6;
	push rax;
	push rbx;
	push a1;
	push a2;
	push a3;
	pop a3;
	pop a2;
	pop a1;
end;
