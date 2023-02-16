#include "app.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>
#include <stdio.h>
#include <SDL2/SDL.h>
// #include "../lib/glad/include/glad/glad.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
// #include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdlrenderer.h"

#define SCALE 16
#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define CYCLE_DELAY 0
#define AMPLITUDE 28000
#define SAMPLE_RATE 44100
#define M_PI 3.14159265358979323846

Application::Application()
{
    m_chip8 = Chip8();
}
Application::~Application() {}

void audioCallback(void *userdata, Uint8 *stream, int len)
{
    Sint16 *buffer = (Sint16 *)stream;
    int length = len / 2; // 2 bytes per sample for AUDIO_S16SYS
    float &sample_nr(*(float *)userdata);

    for (int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)SAMPLE_RATE;
        buffer[i] = (Sint16)(AMPLITUDE * sin(2.0f * M_PI * 441.0f * time)); // render 441 HZ sine wave
    }
}

bool Application::load(std::string path){
    if (!m_chip8.load(path))
    {
        std::cout << "---ROM ERROR---" << std::endl;
        m_last20Instructions.push_front("-ROM ERR-");
        m_loaded = false;
        return false;
    }
    else
    {
        m_last20Instructions.push_front("-ROM LOAD-");
        m_loaded = true;
        std::cout << "---ROM LOADED---" << std::endl;
        return true;
    }
}

int Application::run()
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Chip8EMU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCALE * WINDOW_WIDTH + 100, SCALE * WINDOW_HEIGHT + 120, window_flags);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Fonts
#ifdef _WIN32
    io.Fonts->AddFontFromFileTTF("../fonts/Minecraft.ttf", 16);
#else
    io.Fonts->AddFontFromFileTTF("./fonts/Minecraft.ttf", 16);
#endif

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Audio setup
    SDL_AudioDeviceID audio_device;

    int samplenr = 0;

    SDL_AudioSpec audio_spec;
    SDL_zero(audio_spec);
    audio_spec.freq = SAMPLE_RATE / 2;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 2048;
    audio_spec.callback = audioCallback;
    audio_spec.userdata = &samplenr;

    audio_device = SDL_OpenAudioDevice(
        NULL, 0, &audio_spec, NULL, 0);

    // Define style
    // ImGui::StyleColorsDark
    ImGuiStyle *style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_FrameBg] = ImVec4(.3, .2, .2, 1.0);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(.3, .2, .2, 1.0);
    // style->Colors[ImGuiCol_WindowBg] = ImVec4((45/255),(40/255),(40/255),1.0);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    // style->Colors[ImGuiCol_Bg] = ImVec4((45/255),(40/255),(40/255),1.0);

    // Main loop
    bool done = false;
    bool paused = true;
    auto lastCycleTime = std::chrono::high_resolution_clock::now();

    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.key.keysym.sym == SDLK_COMMA)
                paused = paused ? false : true;
            m_handleInput(event);
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt >= CYCLE_DELAY && !paused)
        {
            lastCycleTime = currentTime;
            if (m_loaded)
            {
                if (m_chip8.emulate() != 0)
                {
                    return 1;
                }
                else
                {
                    m_last20Instructions.push_front(m_chip8.lastInstruction());
                    if (m_last20Instructions.size() > 20)
                    {
                        m_last20Instructions.pop_back();
                    }
                }
            }
        }
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        // {
        //     static float f = 0.0f;
        //     static int counter = 0;

        //     ImGui::Begin("Hello, world!", NULL); // Create a window called "Hello, world!" and append into it.

        //     ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        //     ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        //     ImGui::Checkbox("Another Window", &show_another_window);
        //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        //     ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

        //     if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
        //         counter++;
        //     ImGui::SameLine();
        //     ImGui::Text("counter = %d", counter);

        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //     ImGui::End();
        // }

        {
            // ImGui::BeginMainMenuBar();
            // bool sele = false;
            // bool enab = true;
            // // ImGui::MenuItem("item1", 'h', &sele, enab);
            // ImGui::EndMainMenuBar();
        } {
            // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.6f, 0.6f,1.0f));
            // ImGui::PushID(1);
            if (this->m_debug)
            {
                // Insturction window
                ImGui::SetNextWindowPos(ImVec2(SCALE * WINDOW_WIDTH, 0));
                ImGui::SetNextWindowSize(ImVec2(100, SCALE * WINDOW_HEIGHT));
                ImGui::Begin("Instrutions", NULL, ImGuiWindowFlags_NoMove);
                ImGui::Checkbox("Paused", &paused);
                for (int i = 0; i < m_last20Instructions.size(); i++)
                {
                    std::string s = m_last20Instructions[i];
                    ImGui::Text(s.c_str());
                }
                ImGui::End();

                // Moving cursor for register bar
                ImGui::SetNextWindowPos(ImVec2(0, SCALE * WINDOW_HEIGHT + 1));
                ImGui::SetNextWindowSize(ImVec2(SCALE * WINDOW_WIDTH + 100, 120));
                ImGui::Begin("Registers", NULL, ImGuiWindowFlags_NoMove);

                // ImGui::SetCursorPos();
                if (ImGui::BeginTable("Registers", 9))
                {
                    // ImGui::TableSetupColumn("S",  ImGuiTableColumnFlags_NoResize);
                    for (int row = 0; row < 4; row++)
                    {
                        ImGui::TableNextRow();
                        for (int column = 0; column < 9; column++)
                        {
                            if (column < 4)
                            {
                                ImGui::TableSetColumnIndex(column);
                                ImGui::Text("Register[%X]: %X", (row * 4) + column, (int)m_chip8.getRegisterValue((row * 4) + column));
                            }
                            else
                            {
                                if (column == 5)
                                {
                                    ImGui::TableSetColumnIndex(4);
                                    if (row == 0)
                                    {
                                        ImGui::Text("PC: %X", (int)m_chip8.getRegisterValue(16));
                                    }
                                    if (row == 1)
                                    {
                                        ImGui::Text("Index: %X", (int)m_chip8.getRegisterValue(17));
                                    }
                                    if (row == 2)
                                    {
                                        ImGui::Text("SP: %X", (int)m_chip8.getRegisterValue(18));
                                    }
                                    if (row == 3)
                                    {
                                        ImGui::Text("Sound: %X", (int)m_chip8.getRegisterValue(20));
                                    }
                                }
                                else if (column > 5)
                                {
                                    int key = 0;
                                    for (int j = 5; j < 9; j++)
                                    {
                                        ImGui::TableSetColumnIndex(j);
                                        ImGui::Text("Key: %X", (int)m_chip8.isKey(row * 4 + key));
                                        key++;
                                    }
                                }
                            }
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::End();
                // ImGui::PopStyleColor();
                // ImGui::PopID();
                // Instruction
                ImGui::SetNextWindowPos(ImVec2(0, SCALE * WINDOW_HEIGHT + 1));
                ImGui::SetNextWindowSize(ImVec2(SCALE * WINDOW_WIDTH + 100, 120));
            }
        }
        // Audio

        if (m_chip8.playSound())
        {
            // std::cout << "PLAYING SOUND \a" << std::endl;
            SDL_PauseAudioDevice(audio_device, 0);
        }
        else
        {
            SDL_PauseAudioDevice(audio_device, 1);
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));

        // SDL rendering

        SDL_SetRenderDrawColor(renderer, 45, 40, 40, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        {
            for (int x = 0; x < 64; x++)
            {
                for (int y = 0; y < 32; y++)
                {
                    if (m_chip8.getPixel(x, y))
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
                    }
                }
            }
        }

        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_CloseAudioDevice(audio_device);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void Application::m_handleInput(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN)
    {
        for (int i = 0; i < 16; i++)
        {
            if (event.key.keysym.sym == this->m_keys[i])
            {
                m_chip8.keyDown(i);
            }
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        for (int i = 0; i < 16; i++)
        {
            if (event.key.keysym.sym == this->m_keys[i])
            {
                m_chip8.keyUp(i);
            }
        }
    }
}

void Application::stop()
{
    m_running = false;
}
