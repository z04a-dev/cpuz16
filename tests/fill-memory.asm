start:
    jmp whileless1000;
end;

whileless1000:
    jlt rax, 1000, incwhile;
    ;; get value from memory, and put it into rax
    mov rax, 75;
    lv rbx, rax;
    halt;
end;

;; it will execute 255 times!
incwhile: 
    call fillcell;
    jmp whileless1000;
end;

fillcell:
    sv rax, 500;
    inc rax;
    ret;
end;
