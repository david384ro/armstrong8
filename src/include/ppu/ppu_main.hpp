#ifndef PPU_MAIN_H
#define PPU_MAIN_H
struct PPU
{
    uint16_t I; // 16 bit increment register, use it for filling specific parts up in the vram
    uint8_t vram[24768];

    void write(uint16_t address, uint8_t value)
    {
        if (address < 24768)
        {
            vram[address] = value;
        }
        else
        {
            std::cerr << "PPU memory out of bounds for write: " << address << std::endl;
        }
    }

    void fill_vram(uint8_t value)
    {
        for (int i = 0; i < 24768; i++)
        {
            vram[i] = value;
        }
    }

    uint8_t read(uint16_t address) const
    {
        if (address < 24768)
        {
            return vram[address];
        }
        else
        {
            std::cerr << "PPU memory out of bounds for read: " << address << std::endl;
            return 0;
        }
    }
};
#endif // PPU_MAIN_H