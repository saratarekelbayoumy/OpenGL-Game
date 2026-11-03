#pragma once 
#include <glm/glm.hpp>
#include<glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

const int MAX_COLLECTIBLES = 30;
const int COLLECTIBLE_VALUE = 10;

struct Collectible {
    bool active;
    float x;
    int lane;
    int value;
};
extern Collectible collectibles[MAX_COLLECTIBLES];
void spawnCollectible(int lane, float x);
void updateCollectibles();
void drawCollectible(GLint locTransform, GLint locTint);