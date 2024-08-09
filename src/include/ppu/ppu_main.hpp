// planning on making it

#ifndef PPU_MAIN_H
#define PPU_MAIN_H
struct PPU {
    uint8_t vram[16384];

    void write(uint16_t address, uint8_t value) {
        if (address < 16384) {
            vram[address] = value;
        } else {
            std::cerr << "PPU memory out of bounds for write: " << address << std::endl;
        }
    }

    void fill_vram(uint8_t value) {
        for (int i = 0; i < 16384; i++) {
            vram[i] = value;
        }
    }

    uint8_t read(uint16_t address) const {
        if (address < 16384) {
            return vram[address];
        } else {
            std::cerr << "PPU memory out of bounds for read: " << address << std::endl;
            return 0;
        }
    }
};
#endif //PPU_MAIN_H