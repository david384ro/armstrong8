enum Instructions {
    // Miscellaneous Instructions
    BRK,             // Break (Interrupt)
    NOP,             // No Operation

    // Jump Instructions
    JMP,             // Jump to Address

    // Arithmetic Instructions
    ADD,             // Add
    SUB,             // Subtract
    MUL,             // Multiply
    AND,
    OR,
    XOR,
    NOT,
    SHL,
    SHR,
    CMP,             // Compare
    CMX,             // Compare X
    CMY,

    // Store Instructions
    STA_ZERO_PAGE,   // Store Accumulator in Zero Page
    STA_NEXT_PAGE,   // Store Accumulator in Next Page
    STAX_ZERO_PAGE,  // Store Accumulator in Zero Page by an X offset
    STAX_NEXT_PAGE,  // Store Accumulator in Next Page by an X offset

    // Transfer Instructions
    TAX,             // Transfer Accumulator to X Register
    TAY,             // Transfer Accumulator to Y Register
    TXA,             // Transfer X Register to Accumulator
    TYA,             // Transfer Y Register to Accumulator
    TXY,             // Transfer X Register to Y Register
    TYX,             // Transfer Y Register to X Register

    // Load Instructions
    LDA_IMMEDIATE,   // Load Accumulator with Immediate
    LDA_ZERO_PAGE,   // Load Accumulator with Zero Page
    LDA_NEXT_PAGE,   // Load Accumulator with Next Page

    // Increment/Decrement Instructions
    INX,             // Increment X Register
    DEX,             // Decrement X Register
    INY,             // Increment Y Register
    DEY,             // Decrement Y Register

    // Branch Instructions
    BEQ,             // Branch if Equal
    BNE,             // Branch if Not Equal
    BPL,             // Branch if Plus
    BMI,             // Branch if Minus

    // VRAM Operations
    LDA_VRAM,
    STA_VRAM,
    STAX_VRAM,
    REFRESH,
    FILL
};