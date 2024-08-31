#include "include/cpu_instructions.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>
#include <algorithm>

std::unordered_map<std::string, Instructions> mnemonic_to_opcode = {
    {"BRK", BRK}, {"NOP", NOP}, {"JMP", JMP}, {"ADD", ADD}, {"SUB", SUB},
    {"AND", AND}, {"OR", OR}, {"XOR", XOR}, {"NOT", NOT}, {"SHL", SHL},
    {"SHR", SHR}, {"MUL", MUL}, {"CMP", CMP}, {"CMPHL", CMPHL}, {"CMX", CMX},
    {"CMY", CMY}, {"STA_ZERO_PAGE", STA_ZERO_PAGE}, {"STA_NEXT_PAGE", STA_NEXT_PAGE},
    {"STAX_ZERO_PAGE", STAX_ZERO_PAGE}, {"STAX_NEXT_PAGE", STAX_NEXT_PAGE},
    {"TAX", TAX}, {"TAY", TAY}, {"TXA", TXA}, {"TYA", TYA}, {"TXY", TXY},
    {"TYX", TYX}, {"LDA_IMMEDIATE", LDA_IMMEDIATE}, {"LDA_ZERO_PAGE", LDA_ZERO_PAGE},
    {"LDA_NEXT_PAGE", LDA_NEXT_PAGE}, {"INX", INX}, {"DEX", DEX}, {"INY", INY},
    {"DEY", DEY}, {"JEQ", JEQ}, {"JNE", JNE}, {"BPL", JPL}, {"JMI", JMI},
    {"LDA_VRAM", LDA_VRAM}, {"STA_VRAM", STA_VRAM}, {"STAX_VRAM", STAX_VRAM},
    {"REFRESH", REFRESH}, {"FILL", FILL}, {"STAP_VRAM", STAP_VRAM}, {"TAT1", TAT1},
    {"TT1A", TT1A}, {"TXT1", TXT1}, {"TT1X", TT1X}, {"TYT1", TYT1}, {"TT1Y", TT1Y},
    {"TIT1", TIT1}, {"TT1I", TT1I}, {"TT1T2", TT1T2}, {"TT2T1", TT2T1},
    {"TPT3", TPT3}, {"TT3P", TT3P}, {"TIA", TIA}, {"TAI", TAI}, {"TIX", TIX},
    {"TIY", TIY}, {"TXI", TXI}, {"TYI", TYI}, {"IN1", IN1}, {"DE1", DE1},
    {"INP", INP}, {"DEP", DEP}, {"LDIO", LDIO}, {"LDIOP", LDIOP},
    {"STIO_VRAM", STIO_VRAM}, {"STIOX_VRAM", STIOX_VRAM}, {"STIOP_VRAM", STIOP_VRAM},
    {"STIO", STIO}, {"STIOX", STIOX}, {"STIOP", STIOP}, {"TIOA", TIOA},
    {"TAIO", TAIO}, {"GPRN", GPRN}
};

std::unordered_map<std::string, std::size_t> label_to_address;

std::string trim(const std::string &str) {
    const auto begin = str.find_first_not_of(" \t");
    const auto end = str.find_last_not_of(" \t");
    return (begin == std::string::npos) ? "" : str.substr(begin, end - begin + 1);
}

Instructions get_opcode(const std::string &mnemonic) {
    auto it = mnemonic_to_opcode.find(trim(mnemonic));
    if (it == mnemonic_to_opcode.end())
        throw std::invalid_argument("Unknown mnemonic: " + mnemonic);
    return it->second;
}

std::string remove_comments(const std::string &line) {
    auto pos = line.find("//");
    return (pos == std::string::npos) ? line : line.substr(0, pos);
}

std::vector<std::string> parse_instruction(const std::string &line) {
    std::vector<std::string> parts;
    std::istringstream ss(remove_comments(line));
    std::string part;
    while (std::getline(ss, part, ','))
        parts.push_back(trim(part));
    return parts;
}

void parse_labels(std::ifstream &file) {
    std::string line;
    std::size_t address = 0;

    while (std::getline(file, line)) {
        if (line[0] == '#') {
            label_to_address[trim(line.substr(1))] = address;
        } else {
            auto parts = parse_instruction(line);
            if (parts.empty()) continue;

            ++address; // Opcode takes one byte

            for (const auto &arg : parts) {
                if (arg.find("0x") == 0 || arg.find("#!") == 0)
                    ++address;
            }
        }
    }
    file.clear();
    file.seekg(0);
}

std::vector<uint8_t> process_assembly_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Error opening file: " + filename);

    parse_labels(file);

    file.clear();
    file.seekg(0);

    std::vector<uint8_t> bytecode;
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == '#' || line.empty()) continue;

        auto parts = parse_instruction(line);
        if (parts.empty()) continue;

        try {
            bytecode.push_back(static_cast<uint8_t>(get_opcode(parts[0])));
            for (std::size_t i = 1; i < parts.size(); ++i) {
                const std::string &arg = parts[i];
                if (arg.find("0x") == 0) {
                    bytecode.push_back(static_cast<uint8_t>(std::stoi(arg, nullptr, 16)));
                } else if (arg.find("#!") == 0) {
                    auto it = label_to_address.find(arg.substr(2));
                    if (it != label_to_address.end()) {
                        bytecode.push_back(static_cast<uint8_t>(it->second));
                    } else {
                        throw std::invalid_argument("Unknown label: " + arg);
                    }
                } else {
                    throw std::invalid_argument("Invalid argument format: " + arg);
                }
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return bytecode;
}

void write_bytecode_to_file(const std::vector<uint8_t> &bytecode, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Error opening file for writing: " + filename);

    file.write(reinterpret_cast<const char *>(bytecode.data()), bytecode.size());
    if (!file)
        throw std::runtime_error("Error writing to file: " + filename);
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    try {
        std::vector<uint8_t> bytecode = process_assembly_file(argv[1]);
        write_bytecode_to_file(bytecode, "rom/program.bin");
        std::cout << "Bytecode written to rom/program.bin" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}