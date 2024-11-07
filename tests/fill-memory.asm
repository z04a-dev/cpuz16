start:
    ;; start at $0000
    mov rax, $0040
    mov a1, $001f
    jmp whileless;
end;

whileless:
    jle rax, 255, incwhile;
    ;; while is over at this point
    ;; get value from memory, and put it into rax
    ;; mov rax, 255;
    ;; lv rbx, rax;
    halt;
end;

;; it will execute 255 times!
incwhile: 
    call fillcell;
    jmp whileless;
end;

fillcell:
    ;; put into &(rax) some value.
    sv rax, a1;
    inc a1;
    inc rax;
    ret;
end;
