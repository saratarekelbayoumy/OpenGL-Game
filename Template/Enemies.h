#pragma once 

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
