global Panic
Panic:
    cli
    hlt

// try to do some sketchy stuff here
global crash_me
crash_me:
    ; div by 0
    ; mov ecx, 0x1337
     mov eax, 0
     div eax
    ;int 0x80
    ret