#include "Enemies.h"
#include "GameState.h"
#include <cstdlib>
Enemy enemies[MAX_ENEMIES] = {};

void spawnEasyEnemy() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (!enemies[i].active) {
			enemies[i].active = true;
			enemies[i].type = EASY;
			enemies[i].lane = rand() % 3;
			enemies[i].x = .90f;
			enemies[i].health = 5;
			enemies[i].maxHealth = 5;
			enemies[i].attackTime = 1.8f;
			enemies[i].movementBlocked = false;
			return;
		}
	}
}
void spawnMediumEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].active = true;
            enemies[i].type = MEDIUM;
            enemies[i].lane = rand() % 3;
            enemies[i].x = .90f;
            enemies[i].health = 5;
            enemies[i].maxHealth = 5;
            enemies[i].attackTime = 1.8f;
            enemies[i].movementBlocked = false;
            return;

        }
    }
}
void spawnHardEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].active = true;
            enemies[i].type = HARD;
            enemies[i].lane = rand() % 3;
            enemies[i].x = .90f;
            enemies[i].health = 7;
            enemies[i].maxHealth = 7;
            enemies[i].attackTime = 1.0f;
            enemies[i].movementBlocked=false;
            return;
        }
    }
}
void spawnEnemyType(int type) {
     if(type==EASY)
       spawnEasyEnemy();
     else if (type == MEDIUM)
       spawnMediumEnemy();
     else if (type == HARD)
       spawnHardEnemy();
}