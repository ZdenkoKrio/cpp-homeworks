#include <cstdint>
#include <vector>
#include <map>


/* In this task, you will implement a simple register machine (i.e.
 * a simple model of a computer). The machine has an arbitrary
 * number of integer registers and byte-addressed memory. Registers
 * are indexed from 1 to ‹INT_MAX›. Each instruction takes 2
 * register numbers (indices) and an ‘immediate’ value (an integral
 * constant). Each register can hold a single value of type
 * ‹int32_t› (i.e. the size of the machine word is 4 bytes).  In
 * memory, words are stored LSB-first. The entire memory and all
 * registers start out as 0.
 *
 * The machine has the following instructions (whenever ‹reg_x› is
 * used in the «description», it means the register itself (its
 * value or storage location), not its index; the opposite holds in
 * the column ‹reg_2› which always refers to the register index).
 *
 *  │ opcode │‹reg_2›│ description                                 │
 *  ├────────┼───────┼─────────────────────────────────────────────┤
 *  │ ‹mov›  │  ≥ 1  │ copy a value from ‹reg_2› to ‹reg_1›        │
 *  │        │  = 0  │ set ‹reg_1› to ‹immediate›                  │
 *  │ ‹add›  │  ≥ 1  │ store ‹reg_1 + reg_2› in ‹reg_1›            │
 *  │        │  = 0  │ add ‹immediate› to ‹reg_1›                  │
 *  │ ‹mul›  │  ≥ 1  │ store ‹reg_1 * reg_2› in ‹reg_1›            │
 *  │ ‹jmp›  │  = 0  │ jump to the address stored in ‹reg_1›       │
 *  │        │  ≥ 1  │ jump to ‹reg_1› if ‹reg_2› is nonzero       │
 *  │ ‹load› │  ≥ 1  │ copy value from addr. ‹reg_2› into ‹reg_1›  │
 *  │ ‹stor› │  ≥ 1  │ copy ‹reg_1› to memory address ‹reg_2›      │
 *  │ ‹halt› │  = 0  │ halt the machine with result ‹reg_1›        │
 *  │        │  ≥ 1  │ same, but only if ‹reg_2› is nonzero        │
 *
 * Each instruction is stored in memory as 4 words (addresses
 * increase from left to right). Executing a non-jump instruction
 * increments the program counter by 4 words.
 *
 *  ┌────────┬───────────┬───────┬───────┐
 *  │ opcode │ immediate │ reg_1 │ reg_2 │
 *  └────────┴───────────┴───────┴───────┘
 */

enum class opcode { mov, add, mul, jmp, load, stor, hlt };

class machine {
    std::map<int32_t , uint8_t> memory;
    std::map<int32_t , int32_t> registers;
    
public:
    machine();
    int32_t get( int32_t addr ) const;
    void set( int32_t addr, int32_t v );
    
    int32_t get_reg( int32_t addr ) const;
    void set_reg( int32_t addr, int32_t v );
    int32_t run();
};