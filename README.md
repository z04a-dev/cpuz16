# imaginary assembly CPU (16 bit) by z04a
# Z16CPU

## everything is unsigned!

## registers:
- rax (16 bit)
- rdx (16 bit)
- rbx (16 bit)
- a1, a2, a3 (16 bit)
- ins (16 bit) <- instruction counter

## instructions
- jmp where <- jumps at memory address
- nop <- do nothing
- add what, howmuch <- add howmuch to what
- sub what, howmuch <- remove howmuch from what
- inc what <- what += 1
- dec what <- what -= 1
- halt <- panic (exit program)

## example .asm

```
;; comment
nop;

add rax, 1;
add rbx, 10;
add rax, rbx;

inc a1;
inc a1;

halt;

```

