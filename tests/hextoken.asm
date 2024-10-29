;; implemented hex values in tokens using #
start:
	mov a1, #beef; ;; 0xbeef
	mov a2, #ffff;
	mov a3, #00ff;
	mov rax, #1021;
	mov rbx, #8000;
	mov rdx, #FF00;
	mov ins, 5;
	mul ins, 6;
end;
