#pragma once
#include <string>
#include <deque>

#include <SDL2/SDL.h>
#include "chip8.h"

class Application
{
public:
    Application();
    ~Application();
    int run();
    void stop();

private:
    int m_exit_status{0};
    bool m_running{true};
    bool m_debug{true};
    void m_handleInput(SDL_Event event);
    // std::deque<std::string> m_last20Instructions;
    Chip8 m_chip8;
    const SDL_KeyCode m_keys[16] = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
    };
};