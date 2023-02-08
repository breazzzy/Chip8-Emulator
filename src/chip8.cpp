#include "chip8.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;

#define DEBUG false
#define _debug(f, ...) \
    if (DEBUG)         \
        (fprintf(stderr, f, __VA_ARGS__));

// bool DEBUG = true;

uint8_t fontset[80] =
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

Chip8::Chip8()
{
    // memory[4096];
    for (int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }
    // V[16];
    for (int i = 0; i < 16; i++)
    {
        V[i] = 0;
    }
    I = 0;
    sound_timer = 0;
    delay_timer = 0;
    SP = 0; // Stack Pointer
    PC = 0; // Program Counter
    // stack[16];
    for (int i = 0; i < 16; i++)
    {
        stack[i] = 0;
    }
    // display
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            display[x][y] = 0;
        }
    }

    // std::cout << SP;
}

bool Chip8::load(std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    std::string line;
    if (!file.is_open())
    {
        // break
        return false;
    }
    char c;
    int i = 0;
    int size_check = 0x200;
    // for (int i = 0x200; file.get(c); i++)
    // {
    //     cout << hex << +(char)c << endl;
    //     if (size_check >= 4096)
    //     {
    //         // break;
    //         return false;
    //     }
    //     memory[i] = c;
    //     size_check++;
    // }

    while (file)
    {
        c = file.get();
        memory[size_check + i] = c;
        // cout << hex << +c << endl;

        if (size_check + i > 4096)
        {
            return false;
        }
        i++;
    }

    // load fonts
    if (!loadFont())
    {
        return false;
    }
    // for (int i = 0; i < 16; i++)
    // {
    //     cout << "V" << hex << i << ": " << V[i] << endl;
    // }
    // for (int i = 0; i < 16; i++)
    // {
    //     keypad[i] = false;
    //     cout << "Keypad" << hex << i << ": " << keypad[i] << endl;
    // }
    // I = NULL;
    // cout << "Index: " << I << endl;
    this->PC = 0x200;
    // memory[0x1FF] = 5;
    file.close();
    cout << "----------------ROM LOADED----------------" << endl;
    return true;
}

bool Chip8::loadFont()
{
    for (int i = 0; i < 80; ++i)
    {
        memory[0x50 + i] = fontset[i];
    }
    return true;
}

void Chip8::keyDown(unsigned int key)
{
    this->keypad[key] = true;
}

void Chip8::keyUp(unsigned int key)
{
    this->keypad[key] = false;
}

void Chip8::printRegisters()
{
    for (int i = 0; i < 16; i++)
    {
        cout << "V[" << hex << i << "]: " << hex << (short)this->V[i] << " ";
        if (i % 4 == 0)
        {
            cout << endl;
        }
    }
    cout << "I: " << hex << (short)I << endl;
}
    
void Chip8::printInput()
{
    for (int i = 0; i < 16; i++)
    {
        cout << "key[" << hex << i << "]: " << (int)this->keypad[i] << " ";
        if (i % 4 == 0)
        {
            cout << endl;
        }
    }
}

unsigned char Chip8::getPixel(unsigned char x, unsigned char y)
{
    return this->display[x][y];
}

int Chip8::emulate()
{
    if (this->delay_timer > 0)
    {
        this->delay_timer--;
    }
    if (this->sound_timer > 0)
    {
        this->sound_timer--;
    }

    FLAG_JMP = false;

    unsigned short instruction = (memory[PC] << 8 | memory[PC + 1]);
    unsigned short opcode = instruction & 0xF000;
    unsigned short nnn = (instruction & 0x0FFFu);
    unsigned char x = (instruction & 0x0F00) >> 4 * 2;
    unsigned char y = (instruction & 0x00F0) >> 4;
    unsigned char n = (instruction & 0x000F);

    unsigned char byte = (instruction & 0x00FF); // byte also refered to as kk
    switch (opcode)
    {
    case 0x0000:
    {
        /* code */
        if (instruction == 0x00E0) // CLS
        {
            _debug("[_] CLS: 0x%X\n", instruction);
            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                for (int y = 0; y < SCREEN_HEIGHT; y++)
                {
                    display[x][y] = 0;
                }
            }
        }
        else if (instruction == 0x00EE) // RET
        {
            _debug("[_] RET: 0x%X\n", instruction);
            --SP;
            this->PC = this->stack[this->SP];
        }
        else
        {

            // SYS

            _debug("[_] ERR: 0x%X\n", instruction);
            return 1;
            // this->PC = nnn;
        }
        break;
    }
    case 0x1000:
    { // JMP
        _debug("[_] JMP: 0x%X\n", instruction);
        this->PC = nnn;
        FLAG_JMP = true;
        break;
    }
    case 0x2000:
    { // CALL
        _debug("[_] CALL: 0x%X\n", instruction);
        this->stack[this->SP] = this->PC;
        // addres = (instruction & 0x0FFF);
        ++SP;
        this->PC = nnn;
        return 0;
        break;
    }
    case 0x3000:
    { // SE SKIP if Vx == byte
        _debug("[_] SE: 0x%X\n", instruction);
        if (V[x] == byte)
        {
            // cout << "Skipping\n";
            this->PC += 2;
        }
        break;
    }
    case 0x4000:
    { // SNE SKIP if Vx != byte
        _debug("[_] SNE: 0x%X\n", instruction);
        if (V[x] != byte)
        {
            this->PC += 2;
        }
        break;
    }
    case 0x5000:
    { // SE  Skip next instruction if Vx = Vy.
        _debug("[_] SE: 0x%X\n", instruction);
        if (V[x] == V[y])
        {
            this->PC += 2;
        }
        break;
    }
    case 0x6000:
    { // LD  Put the value byte into register Vx.
        _debug("[_] LD: 0x%X\n", instruction);
        V[x] = byte;
        break;
    }
    case 0x7000:
    { // ADD Adds the value byte to the value of register Vx, then stores the result in Vx.
        _debug("[_] ADD: 0x%X\n", instruction);
        V[x] += byte;
        break;
    }
    case 0x8000:
    { // LD Vx, Vy Stores the value of register Vy in register Vx.
        switch (instruction & 0x000F)
        {
        case 0x0000: // LD Vx, Vy //Stores the value of register Vy in register Vx.
            _debug("[_] LD: 0x%X\n", instruction);
            V[x] = V[y];
            break;
        case 0x0001: // OR Vx, Vy //Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx
        {
            _debug("[_] OR: 0x%X\n", instruction);
            V[x] = V[x] | V[y];
            break;
        }
        case 0x0002: // AND Vx, Vy // Set Vx = Vx AND Vy.
            _debug("[_] AND: 0x%X\n", instruction);
            V[x] = V[x] & V[y];
            break;
        case 0x0003: // XOR Vx, Vy // Set Vx = Vx XOR Vy.
            _debug("[_] XOR: 0x%X\n", instruction);
            V[x] = V[x] ^ V[y];
            break;
        case 0x0004: // ADD Vx, Vy // Set Vx = Vx + Vy, set VF = carry.
        {
            _debug("[_] ADD: 0x%X\n", instruction);
            short result = V[x] + V[y];
            if (result > 0xFF)
            {
                // Carry goes to V[F];
                // cout << "Carry flag\n";
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = result & 0xFF;
            break;
        }
        case 0x0005: // SUB Vx, Vy // Set Vx = Vx - Vy, set VF = NOT borrow.
            _debug("[_] SUB: 0x%X\n", instruction);
            if (V[x] > V[y])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = V[x] - V[y];
            break;
        case 0x0006: // SHR Vx {, Vy} // Set Vx = Vx SHR 1.
            _debug("[_] SHR: 0x%X\n", instruction);
            V[0xF] = (V[x] & 0x1);
            V[x] >>= 1;
            break;
        case 0x0007: // SUBN Vx, Vy // Set Vx = Vy - Vx, set VF = NOT borrow.
            _debug("[_] SUBN: 0x%X\n", instruction);
            if (V[y] > V[x])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = V[y] - V[x];
            break;
        case 0x000E: // SHL Vx {, Vy} // Set Vx = Vx SHL 1.
            _debug("[_] SHL: 0x%X\n", instruction);
            V[0xF] = (V[x] & 0x80) >> 7;
            V[x] <<= 1;
            break;
        default:
            _debug("[X] ERR: 0x%X\n", instruction);
            return 1;
            break;
        }
        break;
    }
    case 0x9000:
    { // SNE Vx, Vy // Skip next instruction if Vx != Vy.
        _debug("[_] SNE: 0x%X\n", instruction);
        if (V[x] != V[y])
        {
            this->PC += 2;
        }
        break;
    }
    case 0xA000:
    {
        _debug("[_] LD: 0x%X\n", instruction);
        uint16_t address = instruction & 0x0fffu;
        I = address;
        // return 1;
        // return 1;
        break;
    }
    case 0xB000:
    {
        _debug("[_] JP: 0x%X\n", instruction);
        this->PC = nnn + V[0];
        break;
    }
    case 0xC000:
    {
        _debug("[_] RND: 0x%X\n", instruction);
        unsigned char r = rand() % 256;
        V[x] = r & byte;
        break;
    }
    case 0xD000:
    {
        _debug("[_] DRW: 0x%X\n", instruction);
        unsigned char x_coord = V[x] % 64;
        unsigned char y_coord = V[y] % 32;
        V[0xF] = 0;
        for (int h = 0; h < n; ++h)
        {
            unsigned short pixel = this->memory[I + h];
            for (int w = 0; w < 8; ++w)
            {
                unsigned char x_pos = x_coord + w;
                unsigned char y_pos = y_coord + h;

                unsigned short *screen_pixel = &display[x_pos][y_pos];

                if (pixel & (0x80 >> w))
                {
                    if (this->display[x_pos][y_pos] > 0)
                    {
                        V[0xF] = 1;
                    }

                    *screen_pixel ^= 0xFFFFFFFF;
                }
            }
        }

        break;
    }
    case 0xe000:
    {
        switch (nnn & 0x0FF)
        {
        case 0x09E: // SKP Vx
            _debug("[_] SKP: 0x%X\n", instruction);
            if (this->keypad[V[x]])
            {
                this->PC += 2;
            }
            break;
        case 0x0A1:
            _debug("[_] SKNP: 0x%X\n", instruction);
            if (this->keypad[V[x]])
            {
                this->PC += 2;
            }
            break;
        default:
            _debug("[X] ERR: 0x%X\n", instruction);
            return 1;
            break;
        }
        break;
    }
    case 0xf000:
    {
        switch (nnn & 0x0FF)
        {
        case 0x0007: // LD Vx, DT
            _debug("[_] LD: 0x%X\n", instruction);
            V[x] = this->delay_timer;
            break;
        case 0x000A: // LD Vx K
        {
            _debug("[_] LD: 0x%X\n", instruction);
            bool key_flag = false; // Flag for whether a key press was found

            for (int i = 0; i < 16; ++i)
            {
                if (this->keypad[i] == true)
                {
                    V[x] = i;
                    key_flag = true;
                }
            }

            if (!key_flag)
            {
                // if key press not found return and go to next emulation cycle;
                return 0;
            }
            break;
        }
        case 0x015:
            _debug("[_] LD: 0x%X\n", instruction);
            this->delay_timer = V[x];
            break;
        case 0x018:
            _debug("[_] LD: 0x%X\n", instruction);
            this->sound_timer = V[x];
            break;
        case 0x01E:
        {
            _debug("[_] ADD: 0x%X\n", instruction);
            this->I += V[x];
            break;
        }
        case 0x029:
        {
            _debug("[_] LD: 0x%X\n", instruction);
            I = 0x50 + (V[x] * 5);
            break;
        }
        case 0x033:
        {
            _debug("[_] LD: 0x%X\n", instruction);
            auto value = V[x];

            memory[I + 2] = value % 10;
            value /= 10;

            // Tens-place
            memory[I + 1] = value % 10;
            value /= 10;

            // Hundreds-place
            memory[I] = value % 10;

            // 100/10 = 10 % 10 = 0
            // 115/10 = 11 % 10 = 10
            // 115 % 10 = 5
            break;
        }
        case 0x055:
            _debug("[_] LD: 0x%X\n", instruction);
            for (int i = 0; i <= x; ++i)
            {
                memory[I + i] = V[i];
            }
            // this->I = this->I + x + 1;
            break;
        case 0x065:
            _debug("[_] LD: 0x%X\n", instruction);
            for (int i = 0; i <= x; ++i)
            {
                V[i] = memory[this->I + i];
            }
            // return 1;
            // this->I = this->I + x + 1;
            break;
        default:
            // cout << "0xF000 invalid " << hex << instruction << " with opcode " << opcode << endl;
            _debug("[X] F ERR: 0x%X\n", instruction);
            return 1;
            break;
        }
        break;
    }
    default:
        cout << "OPCODE INVALID: " << hex << opcode << endl;
        return 1;
    }

    if (!FLAG_JMP)
    {
        this->PC += 2;
    }

    // cout << "OPCODE = " << int(opcode) << " X = " << int(x) << endl;

    return 0;
    // unsigned char x =
}

Chip8::~Chip8()
{
}
