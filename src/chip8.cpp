#include "chip8.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;

#define DEBUG true
#define _debug(f, ...) if(DEBUG) (fprintf(stderr,f, __VA_ARGS__))

// bool DEBUG = true;

const unsigned char fontset[5 * 16] = {
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
    file.open(path, std::ios::in);
    std::string line;
    if (!file.is_open())
    {
        // break
        return false;
    }
    char c;
    int size_check = 0x200;
    for (int i = 0x200; file.get(c); i++)
    {
        if (size_check >= 4096)
        {
            // break;
            return false;
        }
        memory[i] = c;
        size_check++;
    }

    // load fonts
    if (!loadFont())
    {
        return false;
    }
    this->PC = 0x200;
    cout << "----------------ROM LOADED----------------" << endl;
    return true;
}

bool Chip8::loadFont()
{
    for (int i = 0; i < 0x1FF; i++)
    {
        memory[i] = fontset[i];
    }
    return true;
}

void Chip8::registerKey(unsigned char key)
{
    this->key = key;
}

unsigned char Chip8::getPixel(unsigned char x, unsigned char y){
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
    unsigned short nnn = (instruction & 0x0FFF);
    unsigned char x = (instruction & 0x0F00) >> 4 * 2;
    unsigned char y = (instruction & 0x00F0) >> 4;
    unsigned char n = (instruction & 0x000F);

    unsigned char byte = (instruction & 0x00FF); // byte also refered to as kk
    // if(opcode == 0xf000){
    //     cout << "HERAHERASGRB" << endl;
    // }
    switch (opcode)
    {
    case 0x0000:
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
            this->PC = this->stack[this->SP];
            if (this->SP > 0)
            {
                this->SP--;
            }
            else
            {
                // this shouldnt happen?
                cout << "Stack Pointer neg";
            }
        }else{
            _debug("[X] ERR: 0x%X\n", instruction);
        }
        break;
    case 0x1000: // JMP
        _debug("[_] JMP: 0x%X\n", instruction);
        this->PC = nnn;
        FLAG_JMP = true;
        break;
    case 0x2000: // CALL
        _debug("[_] CALL: 0x%X\n", instruction);
        this->stack[this->SP] = this->PC;
        this->SP++;
        break;
    case 0x3000: // SE SKIP if Vx == byte
        _debug("[_] SE: 0x%X\n", instruction);
        if (V[x] == byte)
        {
            this->PC += 2;
        }
        break;
    case 0x4000: // SNE SKIP if Vx != byte
        _debug("[_] SNE: 0x%X\n", instruction);
        if (V[x] != byte)
        {
            this->PC += 2;
        }
        break;
    case 0x5000: // SE  Skip next instruction if Vx = Vy.
        _debug("[_] SE: 0x%X\n", instruction);
        if (V[x] == V[y])
        {
            this->PC += 2;
        }
        break;
    case 0x6000: // LD  Put the value byte into register Vx.
        _debug("[_] LD: 0x%X\n", instruction);
        V[x] = byte;
        break;
    case 0x7000: // ADD Adds the value byte to the value of register Vx, then stores the result in Vx.
        _debug("[_] ADD: 0x%X\n", instruction);
        V[x] = V[x] + byte;
        break;
    case 0x8000: // LD Vx, Vy Stores the value of register Vy in register Vx.
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
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = result & 0x0F;
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
            if (V[x] % 2 == 0)
            {
                V[0xF] = 0;
            }
            else
            {
                V[0xF] = 1;
            }
            V[x] = V[x] >> 1;
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
            if (V[x] < 128)
            {
                V[0xF] = 0;
            }
            else
            {
                V[0xF] = 1;
            }
            V[x] = V[x] << 1;
            break;
        default:
            _debug("[X] ERR: 0x%X\n", instruction);
            break;
            // cout << "Instruction not recognized" << endl;
        }
    case 0x9000: // SNE Vx, Vy // Skip next instruction if Vx != Vy.
        _debug("[_] SNE: 0x%X\n", instruction);
        if (V[x] != V[y])
        {
            this->PC += 2;
        }
        break;
    case 0xA000:
        _debug("[_] LD: 0x%X\n", instruction);
        this->I = nnn;
        break;
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
        // cout << "Draw" << endl;
        unsigned char x_coord = V[x];
        unsigned char y_coord = V[y];
        for(int h = 0; h < n; h++){
            // cout << "Here";
            unsigned short pixel = this->memory [I + h];
            for(int w = 0; w < 8; w++){
                unsigned char x_pos = x_coord + w;
                unsigned char y_pos = y_coord + h;

                if(x_pos > 63){
                    x_pos -= 63;
                }else if(x_pos < 0){
                    x_pos += 63;
                }
                if(y_pos > 31){
                    y_pos -= 31;
                }else if(y_pos < 0){
                    y_pos += 31;
                }

                if((pixel & (0x80 >> w)) > 0){
                    if(this->display[x_pos][y_pos] > 0){
                        V[0xF] = 1;
                    }else{
                        V[0xF] = 0;
                    }
                    
                    display[x_pos][y_pos] ^= 1;
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
            if (this->key == V[x])
            {
                this->PC += 2;
            }
            break;
        case 0x0A1:
            _debug("[_] SKNP: 0x%X\n", instruction);
            if (this->key != V[x])
            {
                this->PC += 2;
            }
            break;
        default:
            // cout << "Instruction not recognized " << hex << instruction << endl;
            _debug("[X] ERR: 0x%X\n", instruction);
            break;
        }
        break;
    }
    case 0xf000:
        // cout << "0xf000 " << hex << nnn << endl;
        switch (nnn & 0x0FF)
        {
        case 0x007: // LD Vx, DT
            _debug("[_] LD: 0x%X\n", instruction);
            V[x] = this->delay_timer;
            break;
        case 0x00A: // LD Vx K
            _debug("[_] LD: 0x%X\n", instruction);
            if (this->key == 0)
            {
                break;
            }
            else
            {
                V[x] = this->key;
            }
            break;
        case 0x015:
            _debug("[_] LD: 0x%X\n", instruction);
            this->delay_timer = V[x];
            break;
        case 0x018:
            _debug("[_] LD: 0x%X\n", instruction);
            this->sound_timer = V[x];
            break;
        case 0x01E:
            _debug("[_] ADD: 0x%X\n", instruction);
            this->I = this->I + V[x];
            break;
        case 0x029:
            _debug("[_] LD: 0x%X\n", instruction);
            this->I = V[x] * 5;
            break;
        case 0x033:
            _debug("[_] LD: 0x%X\n", instruction);
            memory[this->I] = V[x] / 100;
            memory[this->I + 1] = (V[x] / 10) % 10;
            memory[this->I + 2] = (V[x] % 10);
            // 100/10 = 10 % 10 = 0
            // 115/10 = 11 % 10 = 10
            // 115 % 10 = 5
            break;
        case 0x055:
            _debug("[_] LD: 0x%X\n", instruction);
            for (int i = 0; i < x; i++)
            {
                memory[I + i] = V[i];
            }
            this->I = this->I + x + 1;
            break;
        case 0x065:
            _debug("[_] LD: 0x%X\n", instruction);
            for (int i = 0; i < x; i++)
            {
                V[i] = memory[this->I + i];
            }
            this->I = this->I + x + 1;
            break;
        default:
            // cout << "0xF000 invalid " << hex << instruction << " with opcode " << opcode << endl;
            _debug("[X] ERR: 0x%X\n", instruction);
            break;
        }
        break;
    default:
        cout << "OPCODE INVALID: " << hex << opcode << endl;
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
