#include "tgl.h"
#include "shader.h"
#include "text_renderer.h"
#include "resource_manager.h"

#include <iostream>
#include <exception>

TGLWindow::TGLWindow(std::string window_name, std::function<void(double)> update, std::function<void(void)> render)
    : update{update},
      render{render}
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::terminate();
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::terminate();
    }
    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // compile and setup the shader
    // ----------------------------
    Shader shader("../src/text_vertex.vert", "../src/text_fragment.frag");
    TextRenderer tr(shader);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    shader.Use();
    shader.SetMatrix4("projection", projection);
    ResourceManager::Shaders["text"] = shader;

    // const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // SCREEN_WIDTH = 800; // mode->width;
    // SCREEN_HEIGHT = 600; // mode->height;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, characterCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void TGLWindow::main_loop() {
    // initialize game
    // ---------------
    // this is hardcoded now, but we'll have to (somehow) call
    // it from within the game loop when creating the game ...
    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // manage user input and update game state
        // -----------------
        update(deltaTime);

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

void TGLWindow::mouseCallback(GLFWwindow* window, int button, int state, int mods) {
    (void) mods;
    CardGame->MouseInput[button].first = state; // set isPressed

    glfwGetCursorPos(window,
            &(CardGame->MouseInput[button].second.first), // set xpos
            &(CardGame->MouseInput[button].second.second));// set ypos
}

void TGLWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    (void) window;
    (void) scancode;
    (void) mode;

    bool any_selected = false; // if none, use default behavior

    for (auto& [name, tb] : CardGame->text_boxes) {
        if (tb->selected) {
            tb->keyCallback(key, action);
            any_selected = true;
        }
    }

    if (!any_selected && action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                CardGame->OnEscape();
                break;
        }
    }
}

void TGLWindow::characterCallback(GLFWwindow* window, unsigned int codepoint) {
    (void) window;

    for (auto& [name, tb] : CardGame->text_boxes) {
        if (tb->selected) {
            tb->characterCallback(codepoint);
        }
    }
}

void TGLWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    (void) window;
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
}

#endif // THEMONKIFIER_GRAPHICS_LIBRARY_H
