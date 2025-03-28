#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <map>

class AL1 {
private:
    uint8_t A;    // Аккумулятор
    uint8_t PC;   // Счетчик команд
    uint8_t IR;   // Регистр инструкций
    bool running; // Флаг работы
    std::vector<uint8_t> memory;

    enum Opcode {
        HLT = 0x00, // Остановить
        LDA = 0x01, // Загрузить в A
        STA = 0x02, // Сохранить из A
        ADD = 0x03, // Сложить
        SUB = 0x04, // Вычесть
        JMP = 0x05  // Перейти
    };

public:
    AL1() : A(0), PC(0), IR(0), running(false) {
        memory.resize(256, 0);
    }

    void reset() {
        A = 0;
        PC = 0;
        IR = 0;
        running = true;
    }

    void loadProgram(const std::vector<uint8_t>& program) {
        for (size_t i = 0; i < program.size() && i < memory.size(); ++i) {
            memory[i] = program[i];
        }
    }

    void setMemory(uint8_t address, uint8_t value) {
        if (address < memory.size()) {
            memory[address] = value;
        }
    }

    uint8_t getMemory(uint8_t address) const {
        return (address < memory.size()) ? memory[address] : 0;
    }

    void step() {
        if (!running) return;
        IR = memory[PC++];
        switch (IR) {
        case HLT: running = false; std::cout << "Processor halted\n"; break;
        case LDA: A = memory[memory[PC++]]; break;
        case STA: memory[memory[PC++]] = A; break;
        case ADD: A += memory[memory[PC++]]; break;
        case SUB: A -= memory[memory[PC++]]; break;
        case JMP: PC = memory[PC]; break;
        default: std::cout << "Unknown instruction: " << (int)IR << "\n"; running = false; break;
        }
    }

    void run() {
        reset();
        while (running) step();
    }

    void dump() const {
        std::cout << "A: " << (int)A << " PC: " << (int)PC << " IR: " << (int)IR << "\n";
        std::cout << "Memory dump (first 16 bytes):\n";
        for (int i = 0; i < 16; ++i) std::cout << (int)memory[i] << " ";
        std::cout << "\n";
    }
};

// Компиляция .asm в .bin
void compile(const std::string& asmFile, const std::string& binFile) {
    std::map<std::string, uint8_t> opcodes = {
        {"HLT", 0x00}, {"LDA", 0x01}, {"STA", 0x02},
        {"ADD", 0x03}, {"SUB", 0x04}, {"JMP", 0x05}
    };

    std::ifstream inFile(asmFile);
    if (!inFile) {
        std::cerr << "Error: Cannot open " << asmFile << "\n";
        return;
    }

    std::vector<uint8_t> binary;
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string opcode, arg;
        iss >> opcode;

        if (opcode.empty() || opcode[0] == ';') continue;

        auto it = opcodes.find(opcode);
        if (it == opcodes.end()) {
            std::cerr << "Error: Unknown opcode " << opcode << "\n";
            return;
        }

        binary.push_back(it->second);
        if (it->first != "HLT") {
            iss >> arg;
            try {
                uint8_t addr = std::stoi(arg);
                binary.push_back(addr);
            }
            catch (...) {
                std::cerr << "Error: Invalid address in " << line << "\n";
                return;
            }
        }
    }
    inFile.close();

    std::ofstream outFile(binFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Cannot write to " << binFile << "\n";
        return;
    }
    outFile.write(reinterpret_cast<const char*>(binary.data()), binary.size());
    outFile.close();
    std::cout << "Compiled " << asmFile << " to " << binFile << "\n";
}

// Запуск .bin файла
void execute(const std::string& binFile) {
    std::ifstream inFile(binFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: Cannot open " << binFile << "\n";
        return;
    }

    std::vector<uint8_t> program((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    AL1 cpu;
    cpu.loadProgram(program);
    // Установка начальных данных через публичный метод
    cpu.setMemory(10, 5);
    cpu.setMemory(11, 3);
    cpu.run();
    cpu.dump();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage:\n";
        std::cerr << "  al8emulator.exe make <input.asm>\n";
        std::cerr << "  al8emulator.exe run <input.bin>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile;

    if (mode == "make") {
        outputFile = inputFile.substr(0, inputFile.find(".asm")) + ".bin";
        compile(inputFile, outputFile);
    }
    else if (mode == "run") {
        execute(inputFile);
    }
    else {
        std::cerr << "Error: Unknown mode " << mode << "\n";
        return 1;
    }

    return 0;
}