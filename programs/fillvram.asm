FILL, 0x7F
REFRESH
LDA_IMMEDIATE, 0xFF
STAP_VRAM
REFRESH
INP
CMPHL, 0x00, 0x20
JNE, 0x05
REFRESH
BRK