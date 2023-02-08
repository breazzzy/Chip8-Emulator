#include <iostream>
#include <string>

#define MEMORY_SIZE 4096
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

class Chip8
{
private:
    bool FLAG_JMP;
    unsigned char memory[MEMORY_SIZE];
    unsigned char V[16];
    unsigned short I;
    unsigned char sound_timer;
    unsigned char delay_timer;
    unsigned short PC; // Program Counter
    unsigned short stack[16];
    unsigned char SP; // Stack Pointer
    unsigned short display[SCREEN_WIDTH][SCREEN_HEIGHT];
    bool keypad[16];
public:

    Chip8();
    bool load(std::string path);
    bool loadFont();
    int emulate();
    unsigned char getPixel(unsigned char x, unsigned char y);
    void keyDown(unsigned int key);
    void keyUp(unsigned int key);
    void printRegisters();
    void printInput();
    ~Chip8();
};
