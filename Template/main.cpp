#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
# include "GameState.h"
#include "Renderer.h"
#include "Input.h"


int main()
{
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(g_screenWidth, g_screenHeight, "Castle Gate Defenders", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, g_screenWidth, g_screenHeight);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);  
    glfwSetKeyCallback(window, key_callback);
    updateWindowTitle(window);

    Renderer renderer = {};
    if (!initRenderer(renderer)) {
        std::cerr << "Failed to initialize renderer\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    float lastFrame = (float)glfwGetTime();


    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        updatePlayers(deltaTime);
        printPlayerAttacks();
        drawScene(renderer);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyRenderer(renderer);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}