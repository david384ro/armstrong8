#include "include/cpu_instructions.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

std::unordered_map<std::string, Instructions> mnemonic_to_opcode = {
    {"BRK", BRK},
    {"NOP", NOP},
    {"JMP", JMP},
    {"ADD", ADD},
    {"SUB", SUB},
    {"MUL", MUL},
    {"CMP", CMP},
    {"CMX", CMX},
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
    {"BEQ", BEQ},
    {"BNE", BNE},
    {"BPL", BPL},
    {"BMI", BMI}
};

Instructions get_opcode(const std::string& mnemonic) {
    auto it = mnemonic_to_opcode.find(mnemonic);
    if (it != mnemonic_to_opcode.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Unknown mnemonic: " + mnemonic);
    }
}

std::string remove_comments(const std::string& line) {
    std::size_t pos = line.find("//");
    if (pos != std::string::npos) {
        return line.substr(0, pos);
    }
    return line;
}

std::vector<std::string> parse_instruction(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string part;
    while (std::getline(ss, part, ',')) {
        parts.push_back(part);
    }
    return parts;
}

std::vector<uint8_t> process_assembly_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::vector<uint8_t> bytecode;

    std::string line;
    while (std::getline(file, line)) {
        line = remove_comments(line);
        if (line.empty()) continue; // Skip empty lines

        auto parts = parse_instruction(line);
        if (parts.empty()) continue; // Skip if no parts

        try {
            // Assume the first part is the mnemonic
            std::string mnemonic = parts[0];
            Instructions opcode = get_opcode(mnemonic);
            bytecode.push_back(static_cast<uint8_t>(opcode));

            // Handle arguments
            for (std::size_t i = 1; i < parts.size(); ++i) {
                std::string arg = parts[i];
                // Remove any leading/trailing whitespace
                arg.erase(0, arg.find_first_not_of(" \t"));
                arg.erase(arg.find_last_not_of(" \t") + 1);
                
                // Check if the argument is in hexadecimal format
                if (arg.find("0x") == 0) {
                    uint8_t value = std::stoi(arg, nullptr, 16);
                    bytecode.push_back(value);
                } else {
                    // Handle other formats if needed
                }
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return bytecode;
}

void print_bytecode_array(const std::vector<uint8_t>& bytecode) {
    std::cout << "uint8_t program[] = {";
    for (size_t i = 0; i < bytecode.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytecode[i]);
    }
    std::cout << "};" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <assembly_file>" << std::endl;
        return 1;
    }

    try {
        std::string filename = argv[1];
        std::vector<uint8_t> bytecode = process_assembly_file(filename);
        print_bytecode_array(bytecode);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}