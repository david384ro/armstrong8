### ARMSTRONG 8 CPU Instruction Documentation

This documentation provides details on the functionality and behavior of each instruction for the ARMSTRONG 8 CPU emulator. The instructions manipulate the CPU’s state, including the Program Counter (PC), the Accumulator (A), the Index Register (X), and various flags.

#### Instructions

1. **`LDA_IMMEDIATE`**
   - **Opcode:** `0xA9`
   - **Operation:** Load Immediate
   - **Description:** Load an 8-bit immediate value directly into the Accumulator (A). The immediate value is specified by the next byte in the ROM.
   - **Cycles:** 2
   - **Flags Updated:** 
     - `FLAG_ZERO` is set if the value loaded into A is 0.
     - `FLAG_NEGATIVE` is set if the most significant bit (bit 7) of the value is 1 (indicating a negative number in signed 8-bit representation).

2. **`LDA_ZERO_PAGE`**
   - **Opcode:** `0xA5`
   - **Operation:** Load Zero Page
   - **Description:** Load an 8-bit value from a zero-page address (specified by the next byte in ROM) into the Accumulator (A).
   - **Cycles:** 3
   - **Flags Updated:** 
     - `FLAG_ZERO` is set if the value loaded into A is 0.
     - `FLAG_NEGATIVE` is set if the most significant bit of the value is 1.

3. **`STA_ZERO_PAGE`**
   - **Opcode:** `0x85`
   - **Operation:** Store Zero Page
   - **Description:** Store the value of the Accumulator (A) into a zero-page address. The address is specified by the next byte in ROM.
   - **Cycles:** 3

4. **`INX`**
   - **Opcode:** `0xE8`
   - **Operation:** Increment X Register
   - **Description:** Increment the Index Register (X) by 1.
   - **Cycles:** 2
   - **Flags Updated:** 
     - `FLAG_ZERO` is set if X becomes 0.
     - `FLAG_NEGATIVE` is set if the most significant bit of X is 1.

5. **`BRK`**
   - **Opcode:** `0x00`
   - **Operation:** Break
   - **Description:** Trigger a break (interrupt) and halt the CPU. This instruction simulates an interrupt request.
   - **Cycles:** 7
   - **Flags Updated:** 
     - The `halted` flag is set to `true`, stopping CPU execution.

6. **`BEQ`**
   - **Opcode:** `0xF0`
   - **Operation:** Branch if Equal
   - **Description:** Branch to a target address if the Zero flag is set. The target address is calculated by adding a signed 8-bit offset (specified by the next byte in ROM) to the current PC.
   - **Cycles:** 2 (or 3 if the branch is taken)
   - **Flags Updated:** 
     - PC is updated by adding the offset if `FLAG_ZERO` is set.

7. **`BNE`**
   - **Opcode:** `0xD0`
   - **Operation:** Branch if Not Equal
   - **Description:** Branch to a target address if the Zero flag is not set. The target address is calculated by adding a signed 8-bit offset (specified by the next byte in ROM) to the current PC.
   - **Cycles:** 2 (or 3 if the branch is taken)
   - **Flags Updated:** 
     - PC is updated by adding the offset if `FLAG_ZERO` is not set.

8. **`BPL`**
   - **Opcode:** `0x10`
   - **Operation:** Branch if Plus
   - **Description:** Branch to a target address if the Negative flag is not set. The target address is calculated by adding a signed 8-bit offset (specified by the next byte in ROM) to the current PC.
   - **Cycles:** 2 (or 3 if the branch is taken)
   - **Flags Updated:** 
     - PC is updated by adding the offset if `FLAG_NEGATIVE` is not set.

9. **`BMI`**
   - **Opcode:** `0x30`
   - **Operation:** Branch if Minus
   - **Description:** Branch to a target address if the Negative flag is set. The target address is calculated by adding a signed 8-bit offset (specified by the next byte in ROM) to the current PC.
   - **Cycles:** 2 (or 3 if the branch is taken)
   - **Flags Updated:** 
     - PC is updated by adding the offset if `FLAG_NEGATIVE` is set.

10. **`JMP`**
    - **Opcode:** `0x4C`
    - **Operation:** Jump
    - **Description:** Jump to a target address specified by the next two bytes in ROM (little-endian format). The PC is set to this address.
    - **Cycles:** 3

11. **`ADD`**
    - **Opcode:** `0x60`
    - **Operation:** Add
    - **Description:** Add an 8-bit value (specified by the next byte in ROM) to the Accumulator (A) without carry. The result is stored back in A.
    - **Cycles:** 2
    - **Flags Updated:**
      - `FLAG_ZERO` is set if the result is 0.
      - `FLAG_NEGATIVE` is set if the result’s most significant bit is 1.
      - `FLAG_CARRY` is set if the result exceeds 8 bits (i.e., there is a carry out).

12. **`SUB`**
    - **Opcode:** `0x61`
    - **Operation:** Subtract
    - **Description:** Subtract an 8-bit value (specified by the next byte in ROM) from the Accumulator (A). The result is stored back in A.
    - **Cycles:** 2
    - **Flags Updated:**
      - `FLAG_ZERO` is set if the result is 0.
      - `FLAG_NEGATIVE` is set if the result’s most significant bit is 1.
      - `FLAG_CARRY` is set if the result is not negative (i.e., no borrow was needed).

13. **`MUL`**
    - **Opcode:** `0x62`
    - **Operation:** Multiply
    - **Description:** Multiply the Accumulator (A) by an 8-bit value (specified by the next byte in ROM). The result is stored back in A.
    - **Cycles:** 2
    - **Flags Updated:**
      - `FLAG_ZERO` is set if the result is 0.
      - `FLAG_NEGATIVE` is set if the result’s most significant bit is 1.
      - `FLAG_CARRY` is set if the result exceeds 8 bits (i.e., there is an overflow).

14. **`CMP`**
    - **Opcode:** `0x63`
    - **Operation:** Compare
    - **Description:** Compare an 8-bit value (specified by the next byte in ROM) with the Accumulator (A). Set flags based on the result of the comparison.
    - **Cycles:** 2
    - **Flags Updated:**
      - `FLAG_ZERO` is set if the result of the comparison (A - value) is 0.
      - `FLAG_NEGATIVE` is set if the result’s most significant bit is 1.
      - `FLAG_CARRY` is set if the Accumulator (A) is greater than or equal to the value (i.e., no borrow was needed).

15. **Unknown Opcode**
    - **Operation:** Error Handling
    - **Description:** If the opcode does not match any known instruction, an error message is printed with the unknown opcode, and the CPU halts.
    - **Cycles:** 1

#### Flags
- **`FLAG_ZERO`**: Indicates whether the result of the last operation was zero.
- **`FLAG_NEGATIVE`**: Indicates whether the result of the last operation was negative (bit 7 set).
- **`FLAG_CARRY`**: Indicates whether there was a carry-out from the most significant bit or a borrow in subtraction.

#### Notes
- The specific cycles and behavior of branching instructions (like BEQ, BNE, etc.) may vary based on the CPU architecture’s implementation of branches.
- Flag manipulation and results of arithmetic operations are based on 8-bit arithmetic as per the ARMSTRONG 8 specification.