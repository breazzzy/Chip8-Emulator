#include <iostream>
#include <string>

#define MEMORY_SIZE 4096
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

class Chip8
{
private:
    bool m_FLAG_JMP;
    unsigned char m_memory[MEMORY_SIZE];
    unsigned char m_registers[16];
    unsigned short m_index;
    unsigned char m_sound_timer;
    unsigned char m_delay_timer;
    unsigned short m_PC; // Program Counter
    unsigned short m_stack[16];
    unsigned char m_SP; // Stack Pointer
    unsigned short m_display[SCREEN_WIDTH][SCREEN_HEIGHT];
    std::string m_last_instruction;
    bool m_keypad[16];
public:

    Chip8();
    bool isKey(unsigned int key);
    bool load(std::string path);
    bool loadFont();
    int emulate();
    unsigned char getPixel(unsigned char x, unsigned char y);
    void keyDown(unsigned int key);
    void keyUp(unsigned int key);
    short getRegisterValue(int);
    std::string lastInstruction();
    void printRegisters();
    void printInput();
    bool playSound();
    ~Chip8();
};
