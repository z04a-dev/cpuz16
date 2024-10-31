start:
    inc rax;
    add a1, 50;
    nop;
    call test;
    add a2, 70;
end;

test:
    mov rax, 777;
    mov rbx, 661;
    mov rdx, 9999;
    mov a1, 420;
    mov a2, 69;
    mov a3, 1337;
    call pushregs;
    call test2;
    call popregs;
	;; Added halt; for bytecode compatibility
	halt; 
end;

test2:
    mov a3, 1234;
    mov a2, 88;
    mov a1, 442;
    mov rdx, 9966;
    mov rbx, 679;
    mov rax, 755;
    ret;
end;

;; utility coe blocks
pushregs:
    push rax;
    push rbx;
    push rdx;
    push a1;
    push a2;
    push a3;
    ret;
end;

popregs:
    pop a3;
    pop a2;
    pop a1;
    pop rdx;
    pop rbx;
    pop rax;
    ret;
end;
