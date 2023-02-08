#include "app.h"
#include <iostream>

class Application{
    Application::Application::Application(){}
    Application::~Application(){}

    int Application::run(){
        std::cout << "running" << std::endl;
    }

    void Application::stop(){

    }
};