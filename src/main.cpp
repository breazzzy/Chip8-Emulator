#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <stdio.h>
// #include "SDL2/SDL.h"
// #include <glad/glad.h>

#include "app.cpp"

// #include "../imgui/imgui.h"
// #include "../imgui/imgui_impl_sdl2.h"
// #include "../imgui/imgui_impl_opengl3.h"

#define SCALE 16
#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define CYCLE_DELAY 4

const SDL_KeyCode keys[16] = {
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

int main(int argv, char **args)
{
    Application app = Application();
    app.run();
    // Chip8 chip8 = Chip8();
    // // std::cout << chip8.PC << std::endl;

    // SDL_Window *window = NULL;
    // SDL_Surface *screen = NULL;

    // if (SDL_Init(SDL_INIT_VIDEO) > 0)
    // {
    //     fprintf(stderr, "could not initialize SDL: %s\n", SDL_GetError());
    //     return 1;
    // }

    // window = SDL_CreateWindow("EMU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //                           WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE,
    //                           SDL_WINDOW_SHOWN);
    // if (window == NULL)
    // {
    //     fprintf(stderr, "could not initialize SDL: %s\n", SDL_GetError());
    //     return 1;
    // }

    // bool running = true;
    // IMGUI_CHECKVERSION();
    // ImGui::CreateContext();
    // ImGuiIO &io = ImGui::GetIO();
    // (void)io;

    // // setup Dear ImGui style
    // ImGui::StyleColorsDark();
    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    // SDL_GL_MakeCurrent(window, gl_context);
    // // setup platform/renderer bindings
    // std::string glsl_version = "#version 130";
    // if (chip8.load("roms/p.ch8"))
    // { // Rom loaded and read to emulate
    //     // emulate
    //     auto lastCycleTime = std::chrono::high_resolution_clock::now();
    //     std::cout << "----------------EMU START-----------------" << std::endl;
    //     bool paused = true;
    //     while (running)
    //     {
    //         SDL_Event event;
    //         if (SDL_PollEvent(&event))
    //         {
    //             if (event.type == SDL_QUIT)
    //             {
    //                 running = false;
    //                 break;
    //             }
    //             if (event.type == SDL_KEYDOWN)
    //             {
    //                 if (event.key.keysym.sym == SDLK_SPACE)
    //                 {
    //                     paused = paused ? false : true;
    //                 }
    //                 if (event.key.keysym.sym == SDLK_COMMA)
    //                 {
    //                     chip8.emulate();
    //                 }
    //                 if (event.key.keysym.sym == SDLK_n)
    //                 {
    //                     chip8.printInput();
    //                 }
    //                 if (event.key.keysym.sym == SDLK_m)
    //                 {
    //                     chip8.printRegisters();
    //                 }
    //                 for (int i = 0; i < 16; i++)
    //                 {
    //                     if (keys[i] == event.key.keysym.sym)
    //                     {
    //                         chip8.keyDown(i);
    //                     }
    //                 }
    //             }
    //             if (event.type == SDL_KEYUP)
    //             {
    //                 for (int i = 0; i < 16; i++)
    //                 {
    //                     if (keys[i] == event.key.keysym.sym)
    //                     {
    //                         chip8.keyUp(i);
    //                     }
    //                 }
    //             }
    //         }

    //         auto currentTime = std::chrono::high_resolution_clock::now();
    //         float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
    //         if (dt > CYCLE_DELAY && !paused)
    //         {
    //             lastCycleTime = currentTime;
    //             if (chip8.emulate() != 0)
    //             {
    //                 running = false;
    //                 break;
    //             }
    //         }

    //         // Draw SDL
    //         SDL_SetRenderDrawColor(renderer, 45, 40, 40, 255);
    //         SDL_RenderClear(renderer);
    //         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //         for (int x = 0; x < 64; x++)
    //         {
    //             for (int y = 0; y < 32; y++)
    //             {
    //                 if (chip8.getPixel(x, y))
    //                 {
    //                     // float r = rand() % 10 / 10.0;
    //                     // std::cout << r << std::endl;
    //                     // glColor4f(1.0,1.0,1.0,1.0);
    //                     // // glRectf(-1 + (x / 64.0)*2,-1 + (y / 32.0) *2,.01,.01);
    //                     // glRectf(x,y,x+1,y+1);
    //                     SDL_Rect rect;
    //                     rect.x = x * SCALE;
    //                     rect.y = y * SCALE;
    //                     rect.h = 1 * SCALE;
    //                     rect.w = 1 * SCALE;
    //                     SDL_RenderFillRect(renderer, &rect);
    //                 }
    //                 else
    //                 {
    //                 }
    //             }
    //         }
    //         SDL_RenderPresent(renderer);
    //     }
    // }
    // else
    // {
    //     std::cout << "----------------ROM ERROR----------------" << std::endl;
    // }

    // SDL_DestroyWindow(window);
    // SDL_Quit();

    // // std::cout << chip8.memory[0x200] << std::endl;
    return 0;
}