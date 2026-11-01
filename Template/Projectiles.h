#pragma once
#include <glad/glad.h>


const int MAX_PROJECTILES = 100;
const int HIGH_ATTACK = 0;
const int LOW_ATTACK = 1;
const int PLAYER_ATTACK = 2;

struct Projectile {
    bool active;
    bool fromPlayer;
    int damage;
    int lane;
    float x, y;
    float speed;
    int attackKind;
};
extern Projectile projectiles[MAX_PROJECTILES];

void spawnEnemyProjectile(int lane, float x, int attackKind) ;   
void spawnPlayerProjectile(int lane, float x) ;
void updateProjectiles(float dt);
void drawProjectiles(GLint locTransform, GLint locTint);

