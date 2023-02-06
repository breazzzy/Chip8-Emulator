#include <iostream>
#include "chip8.h"
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"

#define SCALE 16
#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64

int main()
{
    Chip8 chip8 = Chip8();
    // std::cout << chip8.PC << std::endl;

    SDL_Window *window = NULL;
    SDL_Surface *screen = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        fprintf(stderr, "could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("EMU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    bool running = true;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED);
    if (chip8.load("roms/maze.ch8"))
    {
        // emulate
        std::cout << "----------------EMU START-----------------" << std::endl;
        while (running)
        {
            chip8.emulate();
            SDL_Event event;
            if (SDL_PollEvent(&event))
            {
                std::cout << "event";
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    break;
                }
            }
            // draw 
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (int x = 0; x < 64; x++)
            {
                for (int y = 0; y < 32; y++)
                {
                    if (chip8.getPixel(x, y) == 1)
                    {
                        // float r = rand() % 10 / 10.0;
                        // std::cout << r << std::endl;
                        // glColor4f(1.0,1.0,1.0,1.0);
                        // // glRectf(-1 + (x / 64.0)*2,-1 + (y / 32.0) *2,.01,.01);
                        // glRectf(x,y,x+1,y+1);
                        SDL_Rect rect;
                        rect.x = x * SCALE;
                        rect.y = y * SCALE;
                        rect.h = 1 * SCALE;
                        rect.w = 1 * SCALE;
                        SDL_RenderFillRect(renderer, &rect);
                    }
                    else
                    {
                        // glColor4f(0.0,0.0,0.0,1.0);
                        // // glRectf(-1 + (x / 64.0)*2,-1 + (y / 32.0) *2,.01,.01);
                        // glRectf(x,y,x+1,y+1);
                    }
                }
            }
            SDL_RenderPresent(renderer);

        }
    }
    else
    {
        std::cout << "----------------ROM ERROR----------------" << std::endl;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    // std::cout << chip8.memory[0x200] << std::endl;
    return 0;
}