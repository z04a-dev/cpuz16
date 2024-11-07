start:
    mov rax, 1000;
    div rax, 2;
    mov rbx, 255;
    mul rbx, 5;

    mov a1, 7; ;; address of 0x0007
    mov a2, 504;
    mul a2, 50;
    div a2, 2;
    sv a1, a2;
    halt;

    ;; Address evaluation is deprecated.
    ;;  mov $0007, 504;
    ;;  mul $0007, 50;
    ;;  div $0007, 2;
    ;;  halt;
end;
