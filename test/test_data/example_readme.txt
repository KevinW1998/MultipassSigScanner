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

example_ref_bstr.bin
has the same ASM but:
1. A BSTR text has been added before it
2. at 68 01 00 00 00, the "01 00 00 00" will be replaced with the direct memory address to it