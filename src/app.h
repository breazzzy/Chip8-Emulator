#pragma once

#include <SDL2/SDL.h>

namespace App
{
    class Application
    {
    public:
        Application();
        ~Application();
        int run();
        void stop();

    private:
        int m_exit_status{0};
        bool m_running { true };
    };
}