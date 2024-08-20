LDIOP
STIOP_VRAM
REFRESH // this is not optimized probably dont use it
INP
CMPHL, 0xBF, 0x60
BNE, 0x00
REFRESH
BRK