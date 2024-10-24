;; This is a test program for deep subroutine jump logic
;; Only works in binary mode
start:
	inc rax;
	inc rax;
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

