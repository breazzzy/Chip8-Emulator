// #include <iostream>
// #include "chip8.h"
// #include <GLFW/glfw3.h>
// #include <stdlib.h>
// #include <stdio.h>

// #define SCALE = 4

// static void error_callback(int error, const char* description)
// {
//     fputs(description, stderr);
// }
// static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GL_TRUE);
// }

// int main(){
//     Chip8 chip8 = Chip8();
//     // std::cout << chip8.m_PC << std::endl;

//     GLFWwindow* window;
//     glfwSetErrorCallback(error_callback);
//     if (!glfwInit())
//         exit(EXIT_FAILURE);
//     window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
//     if (!window)
//     {
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetKeyCallback(window, key_callback);
//     if (chip8.load("roms/PICTURE")){
//         // emulate
//         std::cout << "----------------EMU START-----------------" << std::endl;
//         while(!glfwWindowShouldClose(window)){
//             chip8.emulate();
//             glClear(GL_COLOR_BUFFER_BIT);
//             glMatrixMode(GL_PROJECTION);
//             glLoadIdentity();
//             glOrtho(0.0,64.0,0.0,32.0,0.0,1.0);
//             for(int x = 0; x < 64; x++){
//                 for (int y = 0; y < 32; y++){
//                     if(chip8.getPixel(x,y) == 1){
//                         // float r = rand() % 10 / 10.0;
//                         // std::cout << r << std::endl;
//                         glColor4f(1.0,1.0,1.0,1.0);
//                         // glRectf(-1 + (x / 64.0)*2,-1 + (y / 32.0) *2,.01,.01);
//                         glRectf(x,y,x+1,y+1);
//                     }else{
//                         glColor4f(0.0,0.0,0.0,1.0);
//                         // glRectf(-1 + (x / 64.0)*2,-1 + (y / 32.0) *2,.01,.01);
//                         glRectf(x,y,x+1,y+1);
//                     }
//                 }
//             }
//             glfwSwapBuffers(window);
//             glfwPollEvents();
//         }
//     }else{
//         std::cout << "----------------ROM ERROR----------------" << std::endl;
//     }
    
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     exit(EXIT_SUCCESS);

    


//     // std::cout << chip8.memory[0x200] << std::endl;
//     return 0;
// }