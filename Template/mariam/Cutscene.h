#pragma once
#include "GameShared.h"

void beginCutscene(GLFWwindow* window);

void updateCutscene(GLFWwindow* window, float dt);

void drawCutsceneRaider(GLint locTransform, GLint locTint, float x,float feetY, float axeAngle);

void drawCutscene(GLint locTransform, GLint locTint);