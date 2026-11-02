#include "Collectibles.h"
#include "GameState.h"
#include "mariam/GameShared.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Collectible collectibles[MAX_COLLECTIBLES] = {};

void spawnCollectible(int lane, float x) {
	for (int i = 0; i < MAX_COLLECTIBLES;i++) {
		if (!collectibles[i].active) {
			collectibles[i].active = true;
			collectibles[i].lane = lane;
			collectibles[i].x = x;
			collectibles[i].value = COLLECTIBLE_VALUE;
			return;
		}
	}
}

void updateCollectibles() {
    for (int c = 0; c < MAX_COLLECTIBLES; c++) {
        if (!collectibles[c].active)
            continue;
        for (int p = 0; p < MAX_PLAYERS; p++) {
            if (!players[p].active)
                continue;
            if (players[p].lane != collectibles[c].lane)
                continue;
            float distance = std::fabs(players[p].x - collectibles[c].x);
            if (distance <= 0.07f) {
                coins += collectibles[c].value;
                collectibles[c].active = false;
                break;
            }
        }
    }
}

void drawCollectibles(GLint locTransform, GLint locTint) {
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
    if(!collectibles[i].active) 
        continue;
    float y = laneY[collectibles[i].lane] - 0.02f;
    drawRectangle(locTransform, locTint, glm::mat4(1), collectibles[i].x, y, 0.035f, 0.035f, 1.0f, 0.80f, 0.10f);
    drawRectangle(locTransform, locTint, glm::mat4(1), collectibles[i].x, y, 0.015f, 0.025f, 1.0f, 0.95f, 0.40f);
    }
}