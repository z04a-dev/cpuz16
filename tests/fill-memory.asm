start:
    jmp whileless1000;
end;

whileless1000:
    jlt rax, 1000, incwhile;
    halt;
end;

;; it will execute 255 times!
incwhile: 
    call fillcell;
    jmp whileless1000;
end;

fillcell:
    mov *rax, 500;
    inc rax;
    ret;
end;
