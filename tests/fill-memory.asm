start:
    jmp whileless;
end;

whileless:
    jle rax, 255, incwhile;
    ;; while is over at this point
    ;; get value from memory, and put it into rax
    mov rax, 255;
    lv rbx, rax;
    halt;
end;

;; it will execute 255 times!
incwhile: 
    call fillcell;
    jmp whileless;
end;

fillcell:
    sv rax, 420;
    inc rax;
    ret;
end;
