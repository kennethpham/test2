#include "CHIP-8.h"
#include <sys/time.h>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONT_START_ADDRESS = 0x50;
#define FONTSET_SIZE 80

uint8_t fontset[FONTSET_SIZE] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};


void initTables(CHIP8 *c) {
    uint8_t i;
    for (i = 0x0; i <= 0xFu+1u; i++) {
        *(c->table + i) = &OP_NULL;
    }
    for (i = 0x0; i <= 0xEu+1u; i++) {
        *(c->table0 + i) = &OP_NULL;
    }
    for (i = 0x0; i <= 0xEu+1u; i++) {
        *(c->table8 + i) = &OP_NULL;
    }
    for (i = 0x0; i <= 0xEu+1u; i++) {
        *(c->tableE + i) = &OP_NULL;
    }
    for (i = 0x0; i <= 0x65u+1u; i++) {
        *(c->tableF + i) = &OP_NULL;
    }
}

void initCHIP8(CHIP8 *c) {
    uint16_t i;
    /* Initializing memory */
    for (i = 0x0; i < 0x1000; i += 1u) {
        *(c->memory + i) = 0x0u;
    }
    /* Initializing registers */
    for (i = 0x0; i < 0x10; i += 1u) {
        *(c->V + i) = 0x0u;
    }
    /* Initializing Index Register */
    c->I = 0x0;
    /* Initializing Program Counter */
    c->PC = START_ADDRESS;
    /* Initializing Stack */
    for (i = 0x0; i < 0x10; i += 1u) {
        *(c->stack + i) = 0x0u;
    }
    /* Initializing Stack Pointer */
    c->SP = 0x0;
    /* Initializing Delay Timer */
    c->delayTimer = 0x0;
    /* Initializing Sound Timer */
    c->soundTimer = 0x0;
/*    memcpy(c->fontSet, (uint8_t[])
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    },
    sizeof(c->fontSet));*/
    for (i = 0; i < FONTSET_SIZE; i++) {
        *(c->memory + FONT_START_ADDRESS + i) = *(fontset + i);
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec * tv.tv_sec);

    initTables(c);

    *(c->table + 0x0) = &Table0;
    *(c->table + 0x1) = &OP_1nnn;
    *(c->table + 0x2) = &OP_2nnn;
    *(c->table + 0x3) = &OP_3xkk;
    *(c->table + 0x4) = &OP_4xkk;
    *(c->table + 0x5) = &OP_5xy0;
    *(c->table + 0x6) = &OP_6xkk;
    *(c->table + 0x7) = &OP_7xkk;
    *(c->table + 0x8) = &Table8;
    *(c->table + 0x9) = &OP_9xy0;
    *(c->table + 0xA) = &OP_Annn;
    *(c->table + 0xB) = &OP_Bnnn;
    *(c->table + 0xC) = &OP_Cxkk;
    *(c->table + 0xD) = &OP_Dxyn;
    *(c->table + 0xE) = &TableE;
    *(c->table + 0xF) = &TableF;

    *(c->table0 + 0x0) = &OP_00E0;
    *(c->table0 + 0xE) = &OP_00EE;

    *(c->table8 + 0x0) = &OP_8xy0;
    *(c->table8 + 0x1) = &OP_8xy1;
    *(c->table8 + 0x2) = &OP_8xy2;
    *(c->table8 + 0x3) = &OP_8xy3;
    *(c->table8 + 0x4) = &OP_8xy4;
    *(c->table8 + 0x5) = &OP_8xy5;
    *(c->table8 + 0x6) = &OP_8xy6;
    *(c->table8 + 0x7) = &OP_8xy7;
    *(c->table8 + 0xE) = &OP_8xyE;

    *(c->tableE + 0x1) = &OP_ExA1;
    *(c->tableE + 0xE) = &OP_Ex9E;

    *(c->tableF + 0x07) = &OP_Fx07;
    *(c->tableF + 0x0A) = &OP_Fx0A;
    *(c->tableF + 0x15) = &OP_Fx15;
    *(c->tableF + 0x18) = &OP_Fx18;
    *(c->tableF + 0x1E) = &OP_Fx1E;
    *(c->tableF + 0x29) = &OP_Fx29;
    *(c->tableF + 0x33) = &OP_Fx33;
    *(c->tableF + 0x55) = &OP_Fx55;
    *(c->tableF + 0x65) = &OP_Fx65;
}

void Table0(CHIP8 *c) {
    (*(c->table0 + (c->opcode & 0x000Fu)))(c);
}

void Table8(CHIP8 *c) {
    (*(c->table8 + (c->opcode & 0x000Fu)))(c);
}

void TableE(CHIP8 *c) {
    (*(c->tableE + (c->opcode & 0x000Fu)))(c);
}

void TableF(CHIP8 *c) {
    printf("TABLEF index = %#X\n", c->opcode & 0x00FFu);
    (*(c->tableF + (c->opcode & 0x00FFu)))(c);
}

void OP_NULL() {
    printf("RAN OP_NULL\n");
}

int loadGame(CHIP8 *c, char const *game) {
    FILE *fp;
    fp = fopen(game, "rb");
    if (fp == NULL) {
        perror("ERROR OPENING FILE");
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, SEEK_END);
    long int size = ftell(fp);
    char *fBuffer = (char*)malloc(sizeof(char) * size);
    fseek(fp, 0, SEEK_SET);
    fread(fBuffer, 1, size, fp);
    fclose(fp);
    long int i;
    for (i = 0; i< size; i++) {
        *(c->memory + START_ADDRESS + i) = *(fBuffer + i);
    }
    free(fBuffer);
    return 0;
}

/* CLS: Clear the display */
void OP_00E0(CHIP8 *c) {
    printf("IN OP_00E0\n");
    memset(c->graphics, 0, sizeof(c->graphics));
}

/* RET: Return from a subroutine */
void OP_00EE(CHIP8 *c) {
    printf("OP_00EE\n");
    --(c->SP);
    c->PC = *(c->stack + c->SP);
}

/* JP addr: Jump to location nnn */
void OP_1nnn(CHIP8 *c) {
    printf("OP_1nnn\n");
    uint16_t address = c->opcode & 0x0FFFu;
    c->PC = address;
    printf("PC = %#X\n", c->opcode & 0x0FFFu);
    printf("c->PC = %#X\n", c->PC);
}

/* CALL addr: Call subroutine at nnn */
void OP_2nnn(CHIP8 *c) {
    uint16_t address = c->opcode & 0x0FFFu;
    *(c->stack + c->SP) = c->PC;
    ++c->SP;
    c->PC = address;
}

/* SE Vx, byte: Skip next instruction if Vx = kk */
void OP_3xkk(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t byte = c->opcode & 0x00FFu;
    if (*(c->V + Vx) == byte) {
        c->PC += 2;
    }
}

/* SNE Vx, byte: Skip next instruction if Vx != kk */
void OP_4xkk(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t byte = c->opcode & 0x00FFu;
    if (*(c->V + Vx) != byte) {
        c->PC += 2;
    }
}

/* SE Vx, Vy: Skip next instruction if Vx != Vy */
void OP_5xy0(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    if (*(c->V + Vx) == *(c->V + Vy)) {
        c->PC += 2;
    }
}

/* LD Vx, byte: Set Vx = kk */
void OP_6xkk(CHIP8 *c) {
    printf("IN OP_6xkk\n");
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t byte = c->opcode & 0x00FFu;
    *(c->V + Vx) = byte;
}

/* ADD Vx, byte: Set Vx = Vx + kk */
void OP_7xkk(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t byte = c->opcode & 0x00FFu;
    *(c->V + Vx) += byte;
}

/* LD Vx, Vy: Set Vx = Vy */
void OP_8xy0(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    *(c->V + Vx) = *(c->V + Vy);
}

/* OR Vx, Vy: Set Vx = Vx OR Vy */
void OP_8xy1(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    *(c->V + Vx) |= *(c->V + Vy);
}

/* AND Vx, Vy: Set Vx = Vx AND Vy */
void OP_8xy2(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    *(c->V + Vx) &= *(c->V + Vy);
}

/* XOR Vx, Vy: Set Vx = Vx XOR Vy */
void OP_8xy3(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    *(c->V + Vx) ^= *(c->V + Vy);
}

/* ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry  */
void OP_8xy4(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    uint16_t sum = *(c->V + Vx) + *(c->V + Vy);
    if (sum > 255U) {
        *(c->V + 0xF) = 1;
    }
    else {
        *(c->V + 0xF) = 0;
    }
    *(c->V + Vx) = sum & 0xFFu;
}

/* SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow */
void OP_8xy5(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    if (*(c->V + Vx) > *(c->V + Vy)) {
        *(c->V + 0xF) = 1;
    }
    else {
        *(c->V + 0xF) = 0;
    }
    *(c->V + Vx) -= *(c->V + Vy);
}

/* SHR Vx: Set Vx = Vx SHR 1 */
void OP_8xy6(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    *(c->V + 0xF) = (*(c->V + Vx) & 0x1u);
    *(c->V + Vx) >>= 1u;
}

/* SUBN Vx, Vy: Set Vx = Vy - Vx, set VF = NOT borrow */
void OP_8xy7(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    if (*(c->V + Vy) > *(c->V + Vx)) {
        *(c->V + 0xF) = 1;
    }
    else {
        *(c->V + 0xF) = 0;
    }
    *(c->V + Vx) = *(c->V + Vy) - *(c->V + Vx);
}

/* SHL Vx {, Vy}: Set Vx = Vx SHL 1 */
void OP_8xyE(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    *(c->V + 0xF) = (*(c->V + Vx) & 0x80u) >> 7u;
    *(c->V + Vx) <<= 1u;
}

/* SNE Vx, Vy: Skip next instruction if Vx != Vy */
void OP_9xy0(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0u) >> 4u;
    if (*(c->V + Vx) != *(c->V + Vy)) {
        c->PC += 2;
    }
}

/* LD I, addr: Set I = nnn */
void OP_Annn(CHIP8 *c) {
    uint16_t address = c->opcode & 0x0FFFu;
    c->I = address;
}

/* JP V0, addr: Jump to location nnn + V0 */
void OP_Bnnn(CHIP8 *c) {
    uint16_t address = c->opcode & 0x0FFFu;
    c->PC = *(c->V + 0) + address;
}

/* RND Vx, byte: Set Vx = random byte AND kk */
void OP_Cxkk(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t byte = c->opcode & 0x00FFu;
    *(c->V + Vx) = ((rand() % 256u) & byte);
}

/* DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I
 * at (Vx, Vy), set VF = collision */
void OP_Dxyn(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00) >> 8u;
    uint8_t Vy = (c->opcode & 0x00F0) >> 4u;
    uint8_t height = c->opcode & 0x000Fu;

    uint8_t xPos = *(c->V + Vx) % VIDEO_WIDTH;
    uint8_t yPos = *(c->V + Vy) % VIDEO_HEIGHT;

    *(c->V + 0xF) = 0;

    uint8_t row, col;
    for (row = 0; row < height; row++) {
        uint8_t spriteByte = *(c->memory + c->I + row);
        for (col = 0; col < 8; col++) {
            uint8_t spritePixel = spriteByte & (0x80u >> col);
            uint32_t *screenPixel = &(*(c->graphics + (yPos + row)
                            * VIDEO_WIDTH + (xPos + col)));
            if (spritePixel) {
                if (*screenPixel == 0xFFFFFFFF) {
                    *(c->V + 0xF) = 1;
                }
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
    printf("OP_Dxyn\n");
}

/* SKP Vx: Skip next instruction if key with the value of Vx is pressed */
void OP_Ex9E(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t key = *(c->V + Vx);
    if (*(c->keypad + key)) {
        c->PC += 2;
    }
}

/* SKNP Vx: Skip next instruction if key with the value of Vx is not pressed */
void OP_ExA1(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t key = *(c->V + Vx);
    if (!*(c->keypad + key)) {
        c->PC += 2;
    }
}

/* LD Vx, DT: Set Vx = delay timer value */
void OP_Fx07(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    *(c->V + Vx) = c->delayTimer;
}

/* LD Vx, K: Wait for a key press, store the value of the key in Vx */
void OP_Fx0A(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    if (*(c->keypad)) {
        *(c->V + Vx) = 0;
    }
    else if (*(c->keypad + 1)) {
        *(c->V + Vx) = 1;
    }
    else if (*(c->keypad + 2)) {
        *(c->V + Vx) = 2;
    }
    else if (*(c->keypad + 3)) {
        *(c->V + Vx) = 3;
    }
    else if (*(c->keypad + 4)) {
        *(c->V + Vx) = 4;
    }
    else if (*(c->keypad + 5)) {
        *(c->V + Vx) = 5;
    }
    else if (*(c->keypad + 6)) {
        *(c->V + Vx) = 6;
    }
    else if (*(c->keypad + 7)) {
        *(c->V + Vx) = 7;
    }
    else if (*(c->keypad + 8)) {
        *(c->V + Vx) = 8;
    }
    else if (*(c->keypad + 9)) {
        *(c->V + Vx) = 9;
    }
    else if (*(c->keypad + 10)) {
        *(c->V + Vx) = 10;
    }
    else if (*(c->keypad + 11)) {
        *(c->V + Vx) = 11;
    }
    else if (*(c->keypad + 12)) {
        *(c->V + Vx) = 12;
    }
    else if (*(c->keypad + 13)) {
        *(c->V + Vx) = 13;
    }
    else if (*(c->keypad + 14)) {
        *(c->V + Vx) = 14;
    }
    else if (*(c->keypad + 15)) {
        *(c->V + Vx) = 15;
    }
    else {
        c->PC -= 2;
    }
}

/* LD DT, Vx: Set delay timer = Vx */
void OP_Fx15(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    c->delayTimer = *(c->V + Vx);
}

/* LD ST, Vx: Set sound timer = Vx */
void OP_Fx18(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    c->soundTimer = *(c->V + Vx);
}

/* ADD I, Vx: Set I = I + Vx */
void OP_Fx1E(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    c->I += *(c->V + Vx);
}

/* LD F, Vx: Set I = location of sprite for digit Vx */
void OP_Fx29(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t digit = *(c->V + Vx);
    c->I = FONT_START_ADDRESS + (5 * digit);
}

/* LD B, Vx: Store BCD representation of Vx in memory locations I,
 * I+1, and I+2 */
void OP_Fx33(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t value = *(c->V + Vx);

    /* Ones-place */
    *(c->memory + c->I + 2) = value % 10;
    value /= 10;
    /* Tens-place */
    *(c->memory + c->I + 1) = value % 10;
    value /= 10;
    /* Hundreds-place */
    *(c->memory + c->I) = value % 10;
}

/* LD [I], Vx: Store the registers V0 through Vx in memory starting
 * at location I */
void OP_Fx55(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t i;
    for (i = 0; i <= Vx; i++) {
        *(c->memory + c->I + i) = *(c->V + i);
    }
}

/* LD Vx, [I]: Read registers V0 through Vx from memory starting at
 * location I */
void OP_Fx65(CHIP8 *c) {
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8u;
    uint8_t i;
    for (i = 0; i <= Vx; i++) {
        *(c->V + i) = *(c->memory + c->I + i);
    }
}

void Cycle(CHIP8 *c) {
    /* Fetch */
    c->opcode = ((*(c->memory + c->PC) << 8u) | (*(c->memory + c->PC + 1)));
    printf("OPCODE = %#X | table index = %#X\n", (c->opcode), (c->opcode& 0xF000u) >> 12u);
    /* Increment the PC before executing anything */
    c->PC += 2;
    /* Decode and Execute */
    (*(c->table + ((c->opcode & 0xF000u) >> 12u)))(c);
    /* Decrement the delay timer if set */
    if (c->delayTimer > 0) {
        --c->delayTimer;
    }
    /* Decrement the sound timer if set */
    if (c->soundTimer > 0) {
        --c->soundTimer;
    }
}
