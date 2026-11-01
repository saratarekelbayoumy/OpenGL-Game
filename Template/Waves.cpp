#include "Waves.h"
#include "Enemies.h"
#include "GameState.h"
#include "mariam/Menus.h"

const float SPAWN_INTERVAL = 2.0f;
int currentWave = 1;
int spawnedInWave = 0;
float spawnTimer = 0.0f;

void updateWaves(GLFWwindow* window, float dt) {
    spawnTimer += dt;
    int waveSize = 0;
    if (currentWave == 1)
        waveSize = 3;
    else if (currentWave == 2)
        waveSize = 5;
    else if (currentWave == 3)
        waveSize = 10;
    if (spawnedInWave < waveSize) {
        if (spawnTimer >= SPAWN_INTERVAL) {
            int type = EASY;
            if (currentWave == 1)
                type = EASY;
            else if (currentWave == 2) {
                if (spawnedInWave == 1 || spawnedInWave == 3)
                    type = MEDIUM;
                else
                    type = EASY;
            }
            else if (currentWave == 3) {
                if (spawnedInWave == 2 || spawnedInWave == 5 || spawnedInWave == 8 || spawnedInWave == 9)
                    type = HARD;
                else if (spawnedInWave == 1 || spawnedInWave == 4 || spawnedInWave == 7)
                    type = MEDIUM;
                else
                    type = EASY;
            }
            spawnEnemyType(type);
            spawnedInWave++;
            spawnTimer = 0.0f;
        }
        return;
    }
    if (anyEnemiesAlive())
        return;
    if (currentWave < 3) {
        currentWave++;
        spawnedInWave = 0;
        spawnTimer = SPAWN_INTERVAL;
    }
   
}

bool enemyReachedBase() {
    for (int i = 0; i < MAX_ENEMIES;i++) {
        if (!enemies[i].active)
            continue;
        if (enemies[i].x <= -0.78f)
            
            return true;
    }
    return false;
}
bool gameWon() {
    if (currentWave == 3 && spawnedInWave == 10 && !anyEnemiesAlive()) {
        return true;
    }
    return false;
}
void checkGameOutcome(GLFWwindow* window) {
    if (enemyReachedBase()) {
        finishGame(window, false);
        return;
    }
    if (gameWon()) {
        finishGame(window, true);
        return;
    }
}