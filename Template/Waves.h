#pragma once 
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

extern int currentWave;
extern int spawnedInWave;
extern float spawnTimer;

void updateWaves(GLFWwindow* window, float dt);
bool enemyReachedBase();
bool gameWon();
void checkGameOutcome(GLFWwindow* window);
