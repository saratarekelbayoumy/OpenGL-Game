#pragma once
#include "GameShared.h"

bool insideButton(float x, float y, float centerY);

void drawButton(
    GLint locTransform,
    GLint locTint,
    const std::string& label, float y);

    void updateWindowTitle(GLFWwindow* window);
    void resetGame();
    void startGame(GLFWwindow* window);
    void returnToMainMenu(GLFWwindow* window);
    void finishGame(GLFWwindow* window, bool victory);
    void drawMenu(GLint locTransform, GLint locTint);
    