;; We can now create complex programs! Poggers
start:
    call whileless255;
    mov a3, 1337;
    halt;
end;

whileless255:
    jlt rax, 255, incwhile;
    ret;
end;

;; it will execute 255 times!
incwhile: 
    inc rax;
    jmp whileless255;
end;
