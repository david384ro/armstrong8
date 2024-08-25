enum Instructions
{
    // Miscellaneous Instructions
    BRK, // Break (Interrupt)
    NOP, // No Operation

    // Jump Instructions
    JMP, // Jump to Address

    // Arithmetic Instructions
    ADD,   // Add
    SUB,   // Subtract
    MUL,   // Multiply
    AND,   // Logical AND
    OR,    // Logical OR
    XOR,   // Logical XOR
    NOT,   // Logical NOT
    SHL,   // Shift Left
    SHR,   // Shift Right
    CMP,   // Compare
    CMX,   // Compare X
    CMY,   // Compare Y
    CMPHL, // Compare PPU's I register with a high and low byte

    // Store Instructions
    STA_ZERO_PAGE,  // Store Accumulator in Zero Page
    STA_NEXT_PAGE,  // Store Accumulator in Next Page
    STAX_ZERO_PAGE, // Store Accumulator in Zero Page by an X offset
    STAX_NEXT_PAGE, // Store Accumulator in Next Page by an X offset

    // Transfer Instructions
    TAX, // Transfer Accumulator to X Register
    TAY, // Transfer Accumulator to Y Register
    TXA, // Transfer X Register to Accumulator
    TYA, // Transfer Y Register to Accumulator
    TXY, // Transfer X Register to Y Register
    TYX, // Transfer Y Register to X Register
    TIA,
    TAI,
    TIX,
    TIY,
    TXI,
    TYI,

    TAIO,
    TIOA,

    // Transfer to TMP registers
    TAT1,
    TT1A,
    TXT1,
    TT1X,
    TYT1,
    TT1Y,
    TIT1,
    TT1I,
    TT1T2,
    TT2T1,

    // TPT3, TT3P
    TPT3,
    TT3P,

    // Load Instructions
    LDA_IMMEDIATE, // Load Accumulator with Immediate
    LDA_ZERO_PAGE, // Load Accumulator with Zero Page
    LDA_NEXT_PAGE, // Load Accumulator with Next Page

    // Increment/Decrement Instructions
    INX, // Increment X Register
    DEX, // Decrement X Register
    INY, // Increment Y Register
    DEY, // Decrement Y Register
    IN1, // Increment I1 Register
    DE1, // Decrement I1 Register
    INP, // Increment PPU's I Register
    DEP, // Decrement PPU's I Regsiter

    // Jump Instructions
    JEQ, // Jump if Equal
    JNE, // Jump if Not Equal
    JPL, // Jump if Plus
    JMI, // Jump if Minus

    // VRAM Operations
    LDA_VRAM,
    STA_VRAM,
    STAX_VRAM,
    STAP_VRAM,
    REFRESH,
    FILL,

    // IO and VRAM
    LDIO,
    LDIOP,
    STIO,
    STIOX,
    STIOP,
    STIO_VRAM,
    STIOX_VRAM,
    STIOP_VRAM,

    GPRN
};