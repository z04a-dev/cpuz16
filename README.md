# imaginary assembly CPU (16 bit) by z04a 
***CPUZ16***

## everything is unsigned!

## registers:
- rax (16 bit)
- rdx (16 bit)
- rbx (16 bit)
- a1, a2, a3 (16 bit)
- ins (16 bit) <- instruction counter
- stack (512 x 16 bit)
- stack pointer (points to next available cell)
- IP <- instruction pointer (what will be executed)

## RAM
RAM size is 65535 bytes, at the end of which 512 bytes are allocated to stack

## instructions
- mov where, what <- put something from what to where
- jmp where <- jumps at label 
- nop <- do nothing
- add what, howmuch <- add howmuch to what
- sub what, howmuch <- remove howmuch from what
- inc what <- what += 1
- dec what <- what -= 1
- push what <- push register to stack
- pop what <- pops value from stack to register
- or reg, (reg or value) <- bitwise OR (result stored in reg1)
- and reg, (reg or value) <- bitwise AND (result stored in reg1)
- xor reg, (reg or value) <- bitwise XOR (result stored in reg1)
- halt <- panic (stop executing)
- end <- closes code block

## code blocks
every .asm requires start: entry point

you can define 65535 code blocks (including start:)

it's possible to jump between code blocks using jmp (not possible to jump into start:)


HOWTO:

```
start:
;; some code
    jmp example;
end;

example:
;; another code block
end;
```

## example .asm

```
;; comment
start:
    nop;

    add rax, 1;
    add rbx, 10;
    add rax, rbx;

    add a3, 500;
    push a3;
    
    sub a3, 1712;
    pop a3;

    inc a1;
    xor a1, a3;
    inc a1;
    jmp fn;

    halt;
end;

fn:
    inc rax;
    inc rax;
    sub rax, 7;
end;

```

