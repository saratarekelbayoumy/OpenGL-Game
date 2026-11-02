#pragma once 
#include <glm/glm.hpp>

const int EASY = 0;
const int MEDIUM = 1;
const int HARD = 2;
const int MAX_ENEMIES=30;

struct Enemy {
    bool active;
    int type;
    int lane;
    float x;
    int health;
    int maxHealth;
    float attackTime;
    bool movementBlocked;
};
extern Enemy enemies[MAX_ENEMIES];

void spawnEasyEnemy();
void spawnMediumEnemy();
void spawnHardEnemy();
void spawnEnemyType(int type);
void updateEnemies(float dt);
bool anyEnemiesAlive();
void damageEnemy(int index, int damage);
void drawEasyEnemy(GLint locTransform, GLint locTint, float x, float feetY, float weaponAngle, float damageScale);
void drawMediumEnemy(GLint locTransform, GLint locTint, float x, float feetY, float weaponAngle, float damageScale);
void drawHardEnemy(GLint locTransform, GLint locTint, float x, float feetY, float axeAngle, float damageScale);
void drawEnemies(GLint locTransform, GLint locTint);