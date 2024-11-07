;; Primitive bubble sorting algorithm
;; interpreter-compatible
;; a3 -> end of array
;; rdx -> first value (1000)
;; rax -> ptr to first value (i think)
start:
    mov a3, $0428; 
    mov rax, $0040;
    mov rdx, 1000;
    add rdx, rdx;
    jmp whileless;
end;

whileless:
    jle rax, a3, incwhile;
    call nilreg;
    jmp sort;
end;
;; start of RAM is 0x0040
nilreg:
    mov rax, 0;
    mov rbx, 0;
    mov rdx, 0;
    mov a1, $0040;
    mov a2, $0040;
    ret;
end;

incwhile: 
    call fillcell;
    jmp whileless;
end;

fillcell:
    dec rdx;
    sv rax, rdx;
    inc rax;
    ret;
end;

sort:
    mov a2, a1;
    inc a2;
    jlt a1, a3, fcell;
    halt;
end;
fcell:
    jle a2, a3, scell;
    inc a1;
    jmp sort;
end;
scell:
    lv rax, a1;
    lv rbx, a2;
    jge rax, rbx, change;
    inc a2;
    jmp fcell;
end;
change:
    push rbx;
    push rdx;
    lv rbx, a1;
    lv rdx, a2;
    sv a1, rdx;
    sv a2, rbx;
    pop rdx;
    pop rbx;
    inc a2;
    jmp fcell;
end;
