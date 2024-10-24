# imaginary assembly CPU (16 bit) by z04a 
***CPUZ16***

All registers are unsigned. Supports two modes: interpreter and binary.
Has in-house compiler.

## registers:
- rax (16 bit)
- rdx (16 bit)
- rbx (16 bit)
- a1, a2, a3 (16 bit)
- ins (16 bit) <- program counter (for bytecode execution)
- ic (64 bit) <- instruction counter (debugging purposes only)
- stack (512 x 16 bit)
- stack pointer (points to next available cell)
- IP <- instruction pointer (what will be executed) (only used in interpreter)
- RP <- return pointer (points to jmp+1 in caller function) (only used in interpreter)

## RAM
RAM size is 65535 bytes (32767 16 bit cells), at the end of which 512 bytes (256 16 bit cells) are allocated to stack

## instructions
- #0000 nop <- do nothing
- #0001 mov where, what <- put something from what to where
- #0002 add what, howmuch <- add howmuch to what
- #0003 sub what, howmuch <- remove howmuch from what
- #0004 inc what <- what += 1
- #0005 dec what <- what -= 1
- #0006 jmp where <- jumps at label (don't save return pointer)
- #0007 call where <- calls function (saves return pointer)
- #0008 ret <- returns from function to rp
- #000a and reg, (reg or value) <- bitwise AND (result stored in reg1)
- #000b xor reg, (reg or value) <- bitwise XOR (result stored in reg1)
- #000c push what <- push register to stack
- #000d pop what <- pops value from stack to register
- #0009 or reg, (reg or value) <- bitwise OR (result stored in reg1)
- #000e halt <- panic (stop executing)
- #000f end <- closes code block

***conditional jump ISA extension***

all of instructions receive three args:

<REG || ADDR || NUM> <REG || ADDR || NUM> LABEL 
- #0010 jeq <- if first arg == second arg, then jump
- #0011 jne <- if first arg != second arg, then jump
- #0012 jgt <- if first arg >  second arg, then jump
- #0013 jlt <- if first arg <  second arg, then jump
- #0014 jge <- if first arg >= second arg, then jump
- #0015 jle <- if first arg <= second arg, then jump

***memory access ISA extension***
- #0016 lv <- load value from arg2(mem addr) to arg1(reg)
- #0017 sv <- put value of arg2 to arg1(pointer to mem)

***mathematical ISA extension***
- #0018 mul <- multiply val1 by val2
- #0019 div <- divide val1 by val2

## VM

This repository provides you with working in-house implementation of CPUZ16 virtual machine
and CPUZ16 compiler.

To compile them, use `make` command in root directory.

This will create `z16` (VM) and `z16c` (compiler) in build/ directory.

`z16` accepts two types of files: correct .asm and correct CPUZ16 binaries (typically .bin).

There is a number of working (and non working) tests in tests/ directory, so you can try to run them.

## compiler and bytecode

CPUZ16 has in-house compiler called z16c, that is being compiled with all targets.

If you only want to compile compiler, then use this command:

`make compiler`             <- for optimized version
`make compiler_unoptimized` <- for unoptimized version

To use compiler, simply provide it with correct .asm file, like this:

`z16c ./source.asm`

Compiler will generate .bin file, that will contain correct bytecode, which can be executed using
CPUZ16 VM.

`z16 ./output.bin`

Note:
It's only possible to do nested subroutine calls using bytecode execution.
Raw interpreter would not work.

## code blocks
every .asm requires start: entry point

you can define 65535 code blocks (including start:)

it's possible to jump between code blocks using jmp (not possible to jump into start:)

HOWTO:

```
start:
;; some code
    jmp example;
;; if you don't put HALT in .start, VM will put it automatically at the end.
end;

example:
;; another code block
;; program will halt at the end of this fn, if you don't put RET here.
;; but only if your VM is in interpreter mode, otherwise it will pass through.
end;
```

## example .asm (old, needs fixing)

```
;; comment
start:
    nop;

    add rax, 1;
    add rbx, 10;
    add rax, rbx;
    mov rax, #41FA;

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
    ret;
end;

```

