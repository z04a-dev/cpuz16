start:
	mov rbx, 15000;
	call hz;
	halt;		
end;

hz:
	inc rax;
	jlt rax, rbx, hz;
	ret;
end;
