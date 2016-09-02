example.bin has the following ASM:

00
00
55                push    ebp
8B EC             mov     ebp, esp
90                nop
90                nop
90                nop
68 01 00 00 00    push    offset 0x00000001
90                nop
90                nop
C9                leave
C2 14 00          retn    0x14