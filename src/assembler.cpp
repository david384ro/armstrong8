#include "include/cpu_instructions.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

std::unordered_map<std::string, Instructions> mnemonic_to_opcode = {
    {"BRK", BRK},
    {"NOP", NOP},
    {"JMP", JMP},
    {"ADD", ADD},
    {"SUB", SUB},
    {"AND", AND},
    {"OR", OR},
    {"XOR", XOR},
    {"NOT", NOT},
    {"SHL", SHL},
    {"SHR", SHR},
    {"MUL", MUL},
    {"CMP", CMP},
    {"CMPHL", CMPHL},
    {"CMX", CMX},
    {"CMY", CMY},
    {"STA_ZERO_PAGE", STA_ZERO_PAGE},
    {"STA_NEXT_PAGE", STA_NEXT_PAGE},
    {"STAX_ZERO_PAGE", STAX_ZERO_PAGE},
    {"STAX_NEXT_PAGE", STAX_NEXT_PAGE},
    {"TAX", TAX},
    {"TAY", TAY},
    {"TXA", TXA},
    {"TYA", TYA},
    {"TXY", TXY},
    {"TYX", TYX},
    {"LDA_IMMEDIATE", LDA_IMMEDIATE},
    {"LDA_ZERO_PAGE", LDA_ZERO_PAGE},
    {"LDA_NEXT_PAGE", LDA_NEXT_PAGE},
    {"INX", INX},
    {"DEX", DEX},
    {"INY", INY},
    {"DEY", DEY},
    {"BEQ", BEQ},
    {"BNE", BNE},
    {"BPL", BPL},
    {"BMI", BMI},
    {"LDA_VRAM", LDA_VRAM},
    {"STA_VRAM", STA_VRAM},
    {"STAX_VRAM", STAX_VRAM},
    {"REFRESH", REFRESH},
    {"FILL", FILL},
    {"STAP_VRAM", STAP_VRAM},
    {"TAT1", TAT1},
    {"TT1A", TT1A},
    {"TXT1", TXT1},
    {"TT1X", TT1X},
    {"TYT1", TYT1},
    {"TT1Y", TT1Y},
    {"TIT1", TIT1},
    {"TT1I", TT1I},
    {"TT1T2", TT1T2},
    {"TT2T1", TT2T1},
    {"TPT3", TPT3},
    {"TT3P", TT3P},
    {"TIA", TIA},
    {"TAI", TAI},
    {"TIX", TIX},
    {"TIY", TIY},
    {"TXI", TXI},
    {"TYI", TYI},
    {"IN1", IN1},
    {"DE1", DE1},
    {"INP", INP},
    {"DEP", DEP},
    {"LDIO", LDIO},
    {"LDIOP", LDIOP},
    {"STIO_VRAM", STIO_VRAM},
    {"STIOX_VRAM", STIOX_VRAM},
    {"STIOP_VRAM", STIOP_VRAM},
    {"STIO", STIO},
    {"STIOX", STIOX},
    {"STIOP", STIOP},
    {"TIOA", TIOA},
    {"TAIO", TAIO}};
std::string ltrim(const std::string &str)
{
    std::string s = str;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
    return s;
}

std::string rtrim(const std::string &str)
{
    std::string s = str;
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
    return s;
}

std::string trim(const std::string &str)
{
    return ltrim(rtrim(str));
}
Instructions get_opcode(const std::string &mnemonic)
{
    std::string trimmedmnemonic = trim(mnemonic);
    auto it = mnemonic_to_opcode.find(trimmedmnemonic);
    if (it != mnemonic_to_opcode.end())
    {
        return it->second;
    }
    else
    {
        throw std::invalid_argument("Unknown mnemonic: " + mnemonic);
    }
}

std::string remove_comments(const std::string &line)
{
    std::size_t pos = line.find("//");
    if (pos != std::string::npos)
    {
        return line.substr(0, pos);
    }
    return line;
}

std::vector<std::string> parse_instruction(const std::string &line)
{
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string part;
    while (std::getline(ss, part, ','))
    {
        parts.push_back(part);
    }
    return parts;
}

std::vector<uint8_t> process_assembly_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::vector<uint8_t> bytecode;

    std::string line;
    while (std::getline(file, line))
    {
        line = remove_comments(line);
        if (line.empty())
            continue;

        auto parts = parse_instruction(line);
        if (parts.empty())
            continue;

        try
        {
            std::string mnemonic = parts[0];
            Instructions opcode = get_opcode(mnemonic);
            bytecode.push_back(static_cast<uint8_t>(opcode));

            for (std::size_t i = 1; i < parts.size(); ++i)
            {
                std::string arg = parts[i];
                arg.erase(0, arg.find_first_not_of(" \t"));
                arg.erase(arg.find_last_not_of(" \t") + 1);

                if (arg.find("0x") == 0)
                {
                    uint8_t value = std::stoi(arg, nullptr, 16);
                    bytecode.push_back(value);
                }
                else
                {
                }
            }
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    return bytecode;
}

void write_bytecode_to_file(const std::vector<uint8_t> &bytecode, const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file for writing: " + filename);
    }

    file.write(reinterpret_cast<const char *>(bytecode.data()), bytecode.size());
    if (!file)
    {
        throw std::runtime_error("Error writing to file: " + filename);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <assembly_file>" << std::endl;
        return 1;
    }

    try
    {
        std::string filename = argv[1];
        std::vector<uint8_t> bytecode = process_assembly_file(filename);
        write_bytecode_to_file(bytecode, "rom/program.bin");
        std::cout << "Bytecode written to rom/program.bin" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}