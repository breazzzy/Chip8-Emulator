#include "chip8.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sstream>
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
        m_memory[i] = 0;
    }
    // m_registers[16];
    for (int i = 0; i < 16; i++)
    {
        m_registers[i] = 0;
    }
    m_index = 0;
    m_sound_timer = 0;
    m_delay_timer = 0;
    m_SP = 0; // Stack Pointer
    m_PC = 0; // Program Counter
    // m_stack[16];
    for (int i = 0; i < 16; i++)
    {
        m_stack[i] = 0;
    }
    // display
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            m_display[x][y] = 0;
        }
    }

    // std::cout << m_SP;
}

bool Chip8::isKey(unsigned int key){
    return this->m_keypad[key];
}

std::string Chip8::lastInstruction(){
    return this->m_last_instruction;
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
        m_memory[size_check + i] = c;
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
    //     cout << "m_registers" << hex << i << ": " << m_registers[i] << endl;
    // }
    // for (int i = 0; i < 16; i++)
    // {
    //     m_keypad[i] = false;
    //     cout << "Keypad" << hex << i << ": " << m_keypad[i] << endl;
    // }
    // m_index = NULL;
    // cout << "Index: " << m_index << endl;
    this->m_PC = 0x200;
    // memory[0x1FF] = 5;
    file.close();
    cout << "----------------ROM LOADED----------------" << endl;
    return true;
}

bool Chip8::loadFont()
{
    for (int i = 0; i < 80; ++i)
    {
        m_memory[0x50 + i] = fontset[i];
    }
    return true;
}

void Chip8::keyDown(unsigned int key)
{
    this->m_keypad[key] = true;
}

void Chip8::keyUp(unsigned int key)
{
    this->m_keypad[key] = false;
}

short Chip8::getRegisterValue(int index){
    if(index == 16)
        return this->m_PC;
    if(index == 17)
        return this->m_index;
    if(index == 18)
        return this->m_SP;
    if(index == 19)
        return this->m_delay_timer;
    if(index == 20)
        return this->m_sound_timer;
    return this->m_registers[index];
}

void Chip8::printRegisters()
{
    for (int i = 0; i < 16; i++)
    {
        cout << "m_registers[" << hex << i << "]: " << hex << (short)this->m_registers[i] << " ";
        if (i % 4 == 0)
        {
            cout << endl;
        }
    }
    cout << "m_index: " << hex << (short)m_index << endl;
}
    
void Chip8::printInput()
{
    for (int i = 0; i < 16; i++)
    {
        cout << "key[" << hex << i << "]: " << (int)this->m_keypad[i] << " ";
        if (i % 4 == 0)
        {
            cout << endl;
        }
    }
}

unsigned char Chip8::getPixel(unsigned char x, unsigned char y)
{
    return this->m_display[x][y];
}

int Chip8::emulate()
{
    if (this->m_delay_timer > 0)
    {
        this->m_delay_timer--;
    }
    if (this->m_sound_timer > 0)
    {
        this->m_sound_timer--;
    }

    m_FLAG_JMP = false;

    unsigned short instruction = (m_memory[m_PC] << 8 | m_memory[m_PC + 1]);
    unsigned short opcode = instruction & 0xF000;
    unsigned short nnn = (instruction & 0x0FFFu);
    unsigned char x = (instruction & 0x0F00) >> 4 * 2;
    unsigned char y = (instruction & 0x00F0) >> 4;
    unsigned char n = (instruction & 0x000F);

    unsigned char byte = (instruction & 0x00FF); // byte also refered to as kk
    std::stringstream s;
    s << std::hex << instruction;
    m_last_instruction = s.str();
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
                    m_display[x][y] = 0;
                }
            }
        }
        else if (instruction == 0x00EE) // RET
        {
            _debug("[_] RET: 0x%X\n", instruction);
            --m_SP;
            this->m_PC = this->m_stack[this->m_SP];
        }
        else
        {

            // SYS

            _debug("[_] ERR: 0x%X\n", instruction);
            return 1;
            // this->m_PC = nnn;
        }
        break;
    }
    case 0x1000:
    { // JMP
        _debug("[_] JMP: 0x%X\n", instruction);
        this->m_PC = nnn;
        m_FLAG_JMP = true;
        break;
    }
    case 0x2000:
    { // CALL
        _debug("[_] CALL: 0x%X\n", instruction);
        this->m_stack[this->m_SP] = this->m_PC;
        // addres = (instruction & 0x0FFF);
        ++m_SP;
        this->m_PC = nnn;
        return 0;
        break;
    }
    case 0x3000:
    { // SE SKIP if Vx == byte
        _debug("[_] SE: 0x%X\n", instruction);
        if (m_registers[x] == byte)
        {
            // cout << "Skipping\n";
            this->m_PC += 2;
        }
        break;
    }
    case 0x4000:
    { // SNE SKIP if Vx != byte
        _debug("[_] SNE: 0x%X\n", instruction);
        if (m_registers[x] != byte)
        {
            this->m_PC += 2;
        }
        break;
    }
    case 0x5000:
    { // SE  Skip next instruction if Vx = Vy.
        _debug("[_] SE: 0x%X\n", instruction);
        if (m_registers[x] == m_registers[y])
        {
            this->m_PC += 2;
        }
        break;
    }
    case 0x6000:
    { // LD  Put the value byte into register Vx.
        _debug("[_] LD: 0x%X\n", instruction);
        m_registers[x] = byte;
        break;
    }
    case 0x7000:
    { // ADD Adds the value byte to the value of register Vx, then stores the result in Vx.
        _debug("[_] ADD: 0x%X\n", instruction);
        m_registers[x] += byte;
        break;
    }
    case 0x8000:
    { // LD Vx, Vy Stores the value of register Vy in register Vx.
        switch (instruction & 0x000F)
        {
        case 0x0000: // LD Vx, Vy //Stores the value of register Vy in register Vx.
            _debug("[_] LD: 0x%X\n", instruction);
            m_registers[x] = m_registers[y];
            break;
        case 0x0001: // OR Vx, Vy //Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx
        {
            _debug("[_] OR: 0x%X\n", instruction);
            m_registers[x] = m_registers[x] | m_registers[y];
            break;
        }
        case 0x0002: // AND Vx, Vy // Set Vx = Vx AND Vy.
            _debug("[_] AND: 0x%X\n", instruction);
            m_registers[x] = m_registers[x] & m_registers[y];
            break;
        case 0x0003: // XOR Vx, Vy // Set Vx = Vx XOR Vy.
            _debug("[_] XOR: 0x%X\n", instruction);
            m_registers[x] = m_registers[x] ^ m_registers[y];
            break;
        case 0x0004: // ADD Vx, Vy // Set Vx = Vx + Vy, set VF = carry.
        {
            _debug("[_] ADD: 0x%X\n", instruction);
            short result = m_registers[x] + m_registers[y];
            if (result > 0xFF)
            {
                // Carry goes to m_registers[F];
                // cout << "Carry flag\n";
                m_registers[0xF] = 1;
            }
            else
            {
                m_registers[0xF] = 0;
            }
            m_registers[x] = result & 0xFF;
            break;
        }
        case 0x0005: // SUB Vx, Vy // Set Vx = Vx - Vy, set VF = NOT borrow.
            _debug("[_] SUB: 0x%X\n", instruction);
            if (m_registers[x] > m_registers[y])
            {
                m_registers[0xF] = 1;
            }
            else
            {
                m_registers[0xF] = 0;
            }
            m_registers[x] = m_registers[x] - m_registers[y];
            break;
        case 0x0006: // SHR Vx {, Vy} // Set Vx = Vx SHR 1.
            _debug("[_] SHR: 0x%X\n", instruction);
            m_registers[0xF] = (m_registers[x] & 0x1);
            m_registers[x] >>= 1;
            break;
        case 0x0007: // SUBN Vx, Vy // Set Vx = Vy - Vx, set VF = NOT borrow.
            _debug("[_] SUBN: 0x%X\n", instruction);
            if (m_registers[y] > m_registers[x])
            {
                m_registers[0xF] = 1;
            }
            else
            {
                m_registers[0xF] = 0;
            }
            m_registers[x] = m_registers[y] - m_registers[x];
            break;
        case 0x000E: // SHL Vx {, Vy} // Set Vx = Vx SHL 1.
            _debug("[_] SHL: 0x%X\n", instruction);
            m_registers[0xF] = (m_registers[x] & 0x80) >> 7;
            m_registers[x] <<= 1;
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
        if (m_registers[x] != m_registers[y])
        {
            this->m_PC += 2;
        }
        break;
    }
    case 0xA000:
    {
        _debug("[_] LD: 0x%X\n", instruction);
        uint16_t address = instruction & 0x0fffu;
        m_index = address;
        // return 1;
        // return 1;
        break;
    }
    case 0xB000:
    {
        _debug("[_] JP: 0x%X\n", instruction);
        this->m_PC = nnn + m_registers[0];
        break;
    }
    case 0xC000:
    {
        _debug("[_] RND: 0x%X\n", instruction);
        unsigned char r = rand() % 256;
        m_registers[x] = r & byte;
        break;
    }
    case 0xD000:
    {
        _debug("[_] DRW: 0x%X\n", instruction);
        unsigned char x_coord = m_registers[x] % 64;
        unsigned char y_coord = m_registers[y] % 32;
        m_registers[0xF] = 0;
        for (int h = 0; h < n; ++h)
        {
            unsigned short pixel = this->m_memory[m_index + h];
            for (int w = 0; w < 8; ++w)
            {
                unsigned char x_pos = x_coord + w;
                unsigned char y_pos = y_coord + h;

                unsigned short *screen_pixel = &m_display[x_pos][y_pos];

                if (pixel & (0x80 >> w))
                {
                    if (this->m_display[x_pos][y_pos] > 0)
                    {
                        m_registers[0xF] = 1;
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
            if (this->m_keypad[m_registers[x]])
            {
                this->m_PC += 2;
            }
            break;
        case 0x0A1:
            _debug("[_] SKNP: 0x%X\n", instruction);
            if (this->m_keypad[m_registers[x]])
            {
                this->m_PC += 2;
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
            m_registers[x] = this->m_delay_timer;
            break;
        case 0x000A: // LD Vx K
        {
            _debug("[_] LD: 0x%X\n", instruction);
            bool key_flag = false; // Flag for whether a key press was found

            for (int i = 0; i < 16; ++i)
            {
                if (this->m_keypad[i] == true)
                {
                    m_registers[x] = i;
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
            this->m_delay_timer = m_registers[x];
            break;
        case 0x018:
            _debug("[_] LD: 0x%X\n", instruction);
            this->m_sound_timer = m_registers[x];
            break;
        case 0x01E:
        {
            _debug("[_] ADD: 0x%X\n", instruction);
            this->m_index += m_registers[x];
            break;
        }
        case 0x029:
        {
            _debug("[_] LD: 0x%X\n", instruction);
            m_index = 0x50 + (m_registers[x] * 5);
            break;
        }
        case 0x033:
        {
            _debug("[_] LD: 0x%X\n", instruction);
            auto value = m_registers[x];

            m_memory[m_index + 2] = value % 10;
            value /= 10;

            // Tens-place
            m_memory[m_index + 1] = value % 10;
            value /= 10;

            // Hundreds-place
            m_memory[m_index] = value % 10;

            // 100/10 = 10 % 10 = 0
            // 115/10 = 11 % 10 = 10
            // 115 % 10 = 5
            break;
        }
        case 0x055:
            _debug("[_] LD: 0x%X\n", instruction);
            for (int i = 0; i <= x; ++i)
            {
                m_memory[m_index + i] = m_registers[i];
            }
            // this->m_index = this->m_index + x + 1;
            break;
        case 0x065:
            _debug("[_] LD: 0x%X\n", instruction);
            for (int i = 0; i <= x; ++i)
            {
                m_registers[i] = m_memory[this->m_index + i];
            }
            // return 1;
            // this->m_index = this->m_index + x + 1;
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

    if (!m_FLAG_JMP)
    {
        this->m_PC += 2;
    }

    // cout << "OPCODE = " << int(opcode) << " X = " << int(x) << endl;

    return 0;
    // unsigned char x =
}

bool Chip8::playSound(){
    return m_sound_timer > 0;
}

Chip8::~Chip8()
{
}
