enum Instructions {
    // Miscellaneous Instructions: 0x00 - 0x0F
    BRK              = 0x01, // Break (Interrupt)
    NOP              = 0xEA, // No Operation

    // Jump Instructions: 0x40 - 0x4F
    JMP              = 0x4C, // Jump to Address

    // Arithmetic Instructions: 0x60 - 0x6F
    ADD              = 0x60, // Add
    SUB              = 0x61, // Subtract
    MUL              = 0x62, // Multiply
    CMP              = 0x63, // Compare
    CMX              = 0x64, // Compare X

    // Store Instructions: 0x80 - 0x8F
    STA_ZERO_PAGE    = 0x85, // Store Accumulator in Zero Page
    STA_NEXT_PAGE    = 0x86, // Store Accumulator in Next Page
    STAX_ZERO_PAGE   = 0x89, // Store Accumulator in Zero Page by an X offset
    STAX_NEXT_PAGE   = 0x88, // Store Accumulator in Next Page by an X offset

    // Transfer Instructions: 0x90 - 0x9F
    TAX              = 0xAA, // Transfer Accumulator to X Register
    TAY              = 0xA8, // Transfer Accumulator to Y Register
    TXA              = 0x8A, // Transfer X Register to Accumulator
    TYA              = 0x98, // Transfer Y Register to Accumulator
    TXY              = 0x99, // Transfer X Register to Y Register
    TYX              = 0xFE, // Transfer Y Register to X Register

    // Load Instructions: 0xA0 - 0xAF
    LDA_IMMEDIATE    = 0xA9, // Load Accumulator with Immediate
    LDA_ZERO_PAGE    = 0xA5, // Load Accumulator with Zero Page
    LDA_NEXT_PAGE    = 0xA6, // Load Accumulator with Next Page

    // Increment/Decrement Instructions: 0xE0 - 0xEF
    INX              = 0xE8, // Increment X Register
    DEX              = 0xCA, // Decrement X Register

    // Branch Instructions: 0xD0 - 0xDF
    BEQ              = 0xF0, // Branch if Equal
    BNE              = 0xD0, // Branch if Not Equal
    BPL              = 0x10, // Branch if Plus
    BMI              = 0x30, // Branch if Minus
};