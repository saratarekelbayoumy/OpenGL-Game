#include "Projectiles.h"
#include "GameState.h"
#include "mariam/GameShared.h"
#include <glm/glm.hpp>

Projectile projectiles[MAX_PROJECTILES] = {};

void spawnEnemyProjectile(int lane, float x, int attackKind) {
    for (int i = 0; i < MAX_PROJECTILES;i++) {
        if (!projectiles[i].active) {
            projectiles[i].active = true;
            projectiles[i].fromPlayer = false;
            projectiles[i].lane = lane;
            projectiles[i].x = x - .08f;
            projectiles[i].speed = .22f;
            projectiles[i].damage = 1;
            projectiles[i].attackKind = attackKind;
            if (attackKind == HIGH_ATTACK)
                projectiles[i].y = laneY[lane] + .08f;
            else
                projectiles[i].y = laneY[lane] - .08f;

            return;
        }
    }
} 
void spawnPlayerProjectile(int lane, float x) {
    for (int i = 0; i < MAX_PROJECTILES;i++) {
        if (!projectiles[i].active) {
            projectiles[i].active = true;
            projectiles[i].fromPlayer = true;
            projectiles[i].lane = lane;
            projectiles[i].x = x + .08f;
            projectiles[i].y = laneY[lane] + .05f;
            projectiles[i].speed = .30f;
            projectiles[i].damage = 1;
            projectiles[i].attackKind = PLAYER_ATTACK;
            return;
        }
    }
}
void updateProjectiles(float dt) {
    for (int i = 0;i < MAX_PROJECTILES; i++) {
       if (!projectiles[i].active)
          continue;
       if (projectiles[i].fromPlayer)
           projectiles[i].x += projectiles[i].speed * dt;
       else
           projectiles[i].x -= projectiles[i].speed * dt;
       if (projectiles[i].x < -1.1f || projectiles[i].x > 1.1f)
           projectiles[i].active = false;

  }
}

void drawProjectiles(GLint locTransform, GLint locTint) {
    glm::mat4 identity = glm::mat4(1);
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active)
            continue;
        if (projectiles[i].fromPlayer) {
        drawRectangle(locTransform, locTint, identity, projectiles[i].x, projectiles[i].y, .055f, .010f, .75f, .55f, .25f);
    }
        else {
            drawRectangle(locTransform, locTint, identity, projectiles[i].x, projectiles[i].y, .05f, .02f, 1.0f, .8f, .2f);
        }
  }
}