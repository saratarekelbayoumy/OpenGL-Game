#include "Combat.h"
#include "GameState.h"
#include "Enemies.h"
#include "Projectiles.h"
#include <cmath>

void resolveEncounters() {
    for (int i = 0; i < MAX_PLAYERS;i++) {
        if (players[i].active && players[i].type != DEFENSE)
            players[i].movementBlocked = false;
    }
    for (int i = 0; i < MAX_ENEMIES;i++) {
        if (enemies[i].active)
            enemies[i].movementBlocked = false;
    }
    for (int p = 0; p < MAX_PLAYERS; p++) {
        if (!players[p].active)
            continue;
        for (int e = 0; e < MAX_ENEMIES; e++) {
            if (!enemies[e].active)
                continue;
            if (players[p].lane != enemies[e].lane)
                continue;
            float distance = std::fabs(players[p].x - enemies[e].x);
            if (distance <= .10f) {
                enemies[e].movementBlocked = true;
                if (players[p].type != DEFENSE)
                    players[p].movementBlocked = true;
            }
        }
    }
}
void updateCombat() {
    for (int p = 0; p < MAX_PLAYERS; p++) {
        if (!players[p].active)
            continue;
        if (players[p].type == DEFENSE)
            continue;
        if (players[p].attacksThisFrame<=0)
            continue;
        if (players[p].type == RANGED) {
            spawnPlayerProjectile(players[p].lane,players[p].x);
            continue;
        }
        for(int e =0 ; e<MAX_ENEMIES; e++) {
            if (!enemies[e].active)
                continue;
            if(players[p].lane != enemies[e].lane)
                continue;
			float distance = std::fabs(players[p].x - enemies[e].x);
            if (distance < .15f) {
               
                    damageEnemy(e, 1);
                    break;
            }
        }
    }
}
void updateEnemyAttacks(float dt) {
    for (int i = 0; i < MAX_ENEMIES;i++) {
        if (!enemies[i].active)
            continue;
        if (enemies[i].type != EASY && enemies[i].type != MEDIUM)
            continue;
        enemies[i].attackTime += dt;
        if (enemies[i].attackTime >= 1.8f) {
            if (enemies[i].type == EASY) {
            spawnEnemyProjectile(enemies[i].lane, enemies[i].x, HIGH_ATTACK);
        }
        else if (enemies[i].type == MEDIUM) {
            spawnEnemyProjectile(enemies[i].lane, enemies[i].x, LOW_ATTACK);
        }
            enemies[i].attackTime = 0;
        }
    }
}
void updateHardAttacks(float dt) {
    for (int e = 0; e < MAX_ENEMIES; e++) {
        if (!enemies[e].active)
            continue;
        if (enemies[e].type != HARD)
            continue;
        int target = -1;
        float targetX = -2.0f;
        for (int p = 0; p < MAX_PLAYERS;p++) {
            if (!players[p].active)
                continue;
            if (players[p].lane != enemies[e].lane)
                continue;
            float distance = std::fabs(players[p].x - enemies[e].x);
            if (distance <= .15f ) {
                if (players[p].x > targetX) {
                    target = p;
                    targetX = players[p].x;
                }
            }
        }
        if (target != -1) {
            enemies[e].attackTime += dt;
            if (enemies[e].attackTime >= 1.0f) {
                damagePlayer(target, 2);
                enemies[e].attackTime = 0;
            }
        }
        else
            enemies[e].attackTime = 0.0f;

            
        
    } 
}

void checkProjectileHits() {
    
    for (int s = 0; s < MAX_PROJECTILES; s++) {
        if (!projectiles[s].active)
            continue;
        if (projectiles[s].fromPlayer) {
            for (int e = 0; e < MAX_ENEMIES; e++) {
                if (!enemies[e].active)
                    continue;
                if (enemies[e].lane != projectiles[s].lane)
                    continue;
                float distance = std::fabs(enemies[e].x - projectiles[s].x);
                if (distance <= .15f) {
                    damageEnemy(e, projectiles[s].damage);
                    projectiles[s].active = false;
                    break;
                }
            }
            continue;
        }
        int target = -1;
		float targetX = -2.0f;
        for (int p = 0; p < MAX_PLAYERS; p++) {
            if (!players[p].active)
                continue;
            if (players[p].lane != projectiles[s].lane)
                continue;
            float distance = std::fabs(players[p].x - projectiles[s].x);
            if (distance <= .06f) {
                if (players[p].x > targetX) {
                    target = p;
                    targetX = players[p].x;
                }
            }
        }
        if (target == -1)
            continue;

        if (players[target].type == DEFENSE) {
			damagePlayer(target, projectiles[s].damage);

        
        projectiles[s].active = false;
         
        continue;
        }
    if (projectiles[s].attackKind == HIGH_ATTACK) {
        if (players[target].ducking)
            continue;
    }
    if (projectiles[s].attackKind == LOW_ATTACK) {
        if (players[target].jumping)
            continue;
    }
	damagePlayer(target, projectiles[s].damage);
    projectiles[s].active = false;
    }
    
}
