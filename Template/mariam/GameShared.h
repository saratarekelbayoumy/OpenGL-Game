#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

#include "../GameState.h"

const int MAIN_MENU = 0;
const int PLAYING = 1;
const int PAUSED = 2;
const int WIN = 3;
const int LOSE = 4;
const int CUTSCENE = 5;

const float CUTSCENE_DURATION = 16.0f;

struct ModelPart {
    float x, y;
    float width, height;
    float angle;
    float red, green, blue;
};

extern int g_screenWidth;
extern int g_screenHeight;

extern float cutsceneTime;
extern int currentScreen;
extern float gameTime;

extern int coins;
extern int hoveredLane;
extern int hoveredCard;

extern bool selectedLanes[3];
extern int selectionOrder[3];
extern int selectionCounter;
extern int placementLane;

extern float laneY[3];
extern float cardX[3];

extern float mouseX;
extern float mouseY;

extern Player players[MAX_PLAYERS];

void drawRectangle(
    GLint locTransform, GLint locTint,
    const glm::mat4& parent,
    float x, float y, float width, float height,
    float red, float green, float blue, float angle = 0
);

void drawParts(
    GLint locTransform, GLint locTint,
    const glm::mat4& parent,
    ModelPart* parts, int count, bool grayscale = false
);

void drawCharacter(
    GLint locTransform, GLint locTint,
    int type, float x, float feetY,
    float damageScale, float heightScale,
    float attackProgress, bool grayscale
);

void refreshHover();