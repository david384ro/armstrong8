enum Instructions {
    LDA_IMMEDIATE = 0xA9,
    LDA_ZERO_PAGE = 0xA5,
    STA_ZERO_PAGE = 0x85,

    LDA_NEXT_PAGE = 0xA6,
    STA_NEXT_PAGE = 0x86,

    STA_ZERO_PAGE_X = 0x87,
    STA_NEXT_PAGE_X = 0x88,

    INX = 0xE8,
    BRK = 0x01,
    BEQ = 0xF0,
    BNE = 0xD0,
    BPL = 0x10,
    BMI = 0x30,
    JMP = 0x4C,
    ADD = 0x60,
    SUB = 0x61,
    MUL = 0x62,
    CMP = 0x63,
    CMX = 0x64,
    CMXHL = 0x65,
};