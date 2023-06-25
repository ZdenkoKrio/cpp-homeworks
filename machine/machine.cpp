#include <iostream>
#include "machine.hpp"


machine::machine() = default;

struct instruction {
    opcode opt;
    int32_t immediate;
    int32_t reg1;
    int32_t reg2;

    instruction(opcode o, int32_t im, int32_t reg1, int32_t reg2):
            opt(o), immediate(im), reg1(reg1), reg2(reg2) {}
};

int32_t machine::get(int32_t addr) const {
    uint32_t res = 0;
    for (int i = 0; i < 4; i++) {
        auto search = memory.find(addr + i);
        if (search != memory.end()) {
            res = res + (memory.at(addr + i) << 8*i);
        } else {
            res = res + (0 << 8*i);
        }
    }

    return res;
}

void machine::set(int32_t addr, int32_t v) {
    for (int i = 0; i < 4; i++) {
        memory[addr + i] = (v >> 8*i) & 0xFF;
    }
}

int32_t machine::get_reg(int32_t addr) const {
    auto search = registers.find(addr);
    if (search != registers.end())
        return registers.at(addr);
    return 0;
}

void machine::set_reg(int32_t addr, int32_t v) {
    registers[addr] = v;
}

int32_t machine::run() {
    int32_t addr = 0;

    while (true) {
        instruction instruct(static_cast<opcode>(get(addr)), get(addr + 4),
                             get(addr + 8), get(addr + 12));
        switch (instruct.opt) {
            case opcode::mov:
                if (instruct.reg2 >= 1)
                    set_reg(instruct.reg1, get_reg(instruct.reg2));
                else
                    set_reg(instruct.reg1, instruct.immediate);
                break;

            case opcode::add:
                if (instruct.reg2 >= 1)
                    set_reg(instruct.reg1,
                            get_reg(instruct.reg1) + get_reg(instruct.reg2));
                else {
                    set_reg(instruct.reg1, instruct.immediate + get_reg(instruct.reg1));
                }
                break;

            case opcode::mul:
                if (instruct.reg2 >= 1)
                    set_reg(instruct.reg1,
                            get_reg(instruct.reg1) * get(instruct.reg2));
                break;

            case opcode::jmp:
                if (instruct.reg2 >= 1 && get_reg(instruct.reg2) >= 1) {
                        addr = get_reg(instruct.reg1);
                        continue;
                }
                if (instruct.reg2 == 0) {
                    addr = get_reg(instruct.reg1);
                    continue;
                }
                break;

            case opcode::load:
                if (instruct.reg2 >= 1) {
                    int32_t value = get(get_reg(instruct.reg2));
                    set_reg(instruct.reg1, value);
                }
                break;

            case opcode::stor:
                if (instruct.reg2 >= 1)
                    set(get_reg(instruct.reg2), get_reg(instruct.reg1));
                break;

            case opcode::hlt:
              return get_reg(instruct.reg1);
        }

        addr += 16;
    }
}

int main() {
    return 0;
}
