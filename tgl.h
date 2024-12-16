#ifndef THEMONKIFIER_GRAPHICS_LIBRARY_H
#define THEMONKIFIER_GRAPHICS_LIBRARY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <functional>

class TGLWindow {
public:
    TGLWindow(std::string window_name, std::function<void(double)> update, std::function<void(void)> render);
    void main_loop();
    std::function<void(double)> update;
    std::function<void(void)> render;

    static void mouseCallback(GLFWwindow* window, int button, int state, int mods);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void characterCallback(GLFWwindow* window, unsigned int codepoint);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
    unsigned int width = 800;
    unsigned int height = 600;
    GLFWwindow* window;
};

#endif // THEMONKIFIER_GRAPHICS_LIBRARY_H
