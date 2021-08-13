#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

struct __CHIP8__;
typedef struct __CHIP8__ CHIP8;

struct __CHIP8__ {
    /* 4096 bytes of memory */
    uint8_t memory[4096];
    /* 16 8-bit registers */
    uint8_t V[16];
    /* 16-bit Index Register */
    uint16_t I;
    /* 16-bit Program Counter */
    uint16_t PC;
    /* 8-bit Stack of size 16 */
    uint16_t stack[16];
    /* 8-bit Stack Pointer */
    uint8_t SP;
    /* Display Size */
    uint32_t graphics[VIDEO_WIDTH * VIDEO_HEIGHT];
    /* 8-bit Delay and Sound Timers */
    uint8_t delayTimer, soundTimer;
    /* CHIP-8 Keypad */
    uint8_t keypad[16];
    /* CHIP-8 Font Set */
    /* 16-bit CPU Instruction */
    uint16_t opcode;
    /* Table of function pointers for opcodes */
    void (*table[0xFu+1u])(CHIP8 *c);
    /* Table of function pointers for 0 prefixed opcodes */
    void (*table0[0xEu+1u])(CHIP8 *c);
    /* Table of function pointers for 8 prefixed opcodes */
    void (*table8[0xEu+1u])(CHIP8 *c);
    /* Table of function pointers for E prefixed opcodes */
    void (*tableE[0xEu+1u])(CHIP8 *c);
    /* Table of function pointers for F prefixed opcodes */
    void (*tableF[0x65u+1u])(CHIP8 *c);

};

void initTables(CHIP8 *c);
void initCHIP8(CHIP8 *c);
void OP_00E0(CHIP8 *c);
void OP_00EE(CHIP8 *c);
void OP_1nnn(CHIP8 *c);
void OP_2nnn(CHIP8 *c);
void OP_3xkk(CHIP8 *c);
void OP_4xkk(CHIP8 *c);
void OP_5xy0(CHIP8 *c);
void OP_6xkk(CHIP8 *c);
void OP_7xkk(CHIP8 *c);
void OP_8xy0(CHIP8 *c);
void OP_8xy1(CHIP8 *c);
void OP_8xy2(CHIP8 *c);
void OP_8xy3(CHIP8 *c);
void OP_8xy4(CHIP8 *c);
void OP_8xy5(CHIP8 *c);
void OP_8xy6(CHIP8 *c);
void OP_8xy7(CHIP8 *c);
void OP_8xyE(CHIP8 *c);
void OP_9xy0(CHIP8 *c);
void OP_Annn(CHIP8 *c);
void OP_Bnnn(CHIP8 *c);
void OP_Cxkk(CHIP8 *c);
void OP_Dxyn(CHIP8 *c);
void OP_Ex9E(CHIP8 *c);
void OP_ExA1(CHIP8 *c);
void OP_Fx07(CHIP8 *c);
void OP_Fx0A(CHIP8 *c);
void OP_Fx15(CHIP8 *c);
void OP_Fx18(CHIP8 *c);
void OP_Fx1E(CHIP8 *c);
void OP_Fx29(CHIP8 *c);
void OP_Fx33(CHIP8 *c);
void OP_Fx55(CHIP8 *c);
void OP_Fx65(CHIP8 *c);
void OP_NULL();
void Table0(CHIP8 *c);
void Table8(CHIP8 *c);
void TableE(CHIP8 *c);
void TableF(CHIP8 *c);
void Cycle(CHIP8 *c);
int loadGame(CHIP8 *c, char const *game);
