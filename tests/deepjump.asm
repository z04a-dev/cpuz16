;; This is a test program for deep subroutine jump logic
;; Currently not working, because call doesn't push RP to stack
;; Also i don't think it would be possible to implement it in interpreter.
start:
;; fix -> i've changed first two inc from rax to smth different for testing
	inc a3;
	inc rbx;
	mov rbx, 50;
	call first;	
	halt;
end;

first:
	inc rbx;
	inc rbx;
	call second;
	ret;
end;

second:
	inc rdx;
	inc rdx;
	call third;
	ret;
end;

third:
	inc a1;
	inc a1;
	push rax;
	call four
	pop rax;
	ret;
end;

four:
	inc a2;
	inc a2;
	ret;
end;

