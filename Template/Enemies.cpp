#include "Enemies.h"
#include "GameState.h"
#include <cstdlib>
#include "mariam/GameShared.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Collectibles.h"
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

void updateEnemies(float dt) {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (!enemies[i].active) 
            continue;

		if (!enemies[i].movementBlocked)
        {
            if (enemies[i].type == HARD)
                enemies[i].x -= .13f * dt;
            else 
                enemies[i].x -= .08f * dt ;
		}
        if (enemies[i].x <= -.78f) {
			enemies[i].x = -.78f;
        }
	}
}
bool anyEnemiesAlive() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            return true;
        }
    }
    return false;
}
void damageEnemy(int index, int damage) {
	if (index < 0 || index >= MAX_ENEMIES)
		return;
	if (!enemies[index].active)
		return;
	enemies[index].health -= damage;
	if (enemies[index].health <= 0) {
		enemies[index].health = 0;
		spawnCollectible(enemies[index].lane, enemies[index].x);
		enemies[index].active = false;
		
	}
}
void drawEasyEnemy(GLint locTransform, GLint locTint, float x, float feetY, float weaponAngle, float damageScale) {
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(x, feetY, 0));
	model = glm::scale(model, glm::vec3(damageScale, damageScale, 1.0f));
    drawRectangle(locTransform, locTint, model, -0.025f, 0.025f, 0.035f, 0.07f, 0.25f, 0.14f, 0.08f);
    drawRectangle(locTransform, locTint, model,0.025f, 0.025f,0.035f, 0.07f,0.25f, 0.14f, 0.08f);
    //body then head then helmat ~nour
    drawRectangle(locTransform, locTint, model,0, .10f, .11f, .11f, .38f, .22f, .12f);
    drawRectangle(locTransform, locTint, model, 0, .115f, .08f, .055f, .35f, .36f, .34f);
    drawRectangle(locTransform, locTint, model,0, .19f, .075f, .07f, .60f, .38f, .22f);
    drawRectangle(locTransform, locTint, model,0, .225f, .095f, .035f, .20f, .20f, .20f);
    drawRectangle(locTransform, locTint, model,-0.017f, 0.195f,0.009f, 0.009f,0.05f, 0.05f, 0.05f);
    drawRectangle(locTransform, locTint, model,0.017f, 0.195f,0.009f, 0.009f,0.05f, 0.05f, 0.05f);
    drawRectangle(locTransform, locTint, model,0.0f, 0.175f,0.025f, 0.006f,0.15f, 0.05f, 0.03f);
    drawRectangle(locTransform, locTint, model,-0.065f, 0.12f,0.035f, 0.09f,0.50f, 0.28f, 0.16f);
    glm::mat4 weaponModel = model;
    weaponModel = glm::translate(weaponModel,glm::vec3(-0.075f, 0.15f, 0.0f));
    weaponModel = glm::rotate(weaponModel,glm::radians(weaponAngle),glm::vec3(0.0f, 0.0f, 1.0f));
    drawRectangle(locTransform, locTint, weaponModel, -0.055f, 0.0f,0.12f, 0.016f,0.35f, 0.18f, 0.08f);
    drawRectangle(locTransform, locTint, weaponModel, -0.095f, 0.0f,0.018f, 0.075f,0.30f, 0.16f, 0.07f);
    drawRectangle(locTransform, locTint, weaponModel, -0.12f, 0.0f, 0.08f, 0.007f, 0.75f, 0.72f, 0.60f);
}
void drawMediumEnemy(GLint locTransform, GLint locTint, float x, float feetY, float weaponAngle, float damageScale) {
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(x, feetY, 0));
	model = glm::scale(model, glm::vec3(damageScale, damageScale, 1.0f));
    drawRectangle(locTransform,locTint,model,-.035f,.025f,.06f,.045f,.24f,.13f,.08f);
    drawRectangle(locTransform, locTint, model, .035f, .025f, .06f, .045f, .24f, .13f, .08f);
    drawRectangle(locTransform, locTint, model, 0, .075f, .13f, .09f, .34f, .19f, .11f);
    drawRectangle(locTransform, locTint, model,0.0f, 0.085f,0.09f, 0.045f,0.27f, 0.30f, 0.29f);
    drawRectangle(locTransform, locTint, model,0.0f, 0.145f,0.08f, 0.065f,0.58f, 0.35f, 0.20f);
    drawRectangle(locTransform, locTint, model,0.0f, 0.177f,0.10f, 0.032f,0.17f, 0.17f, 0.17f);
    drawRectangle(locTransform, locTint, model,-0.018f, 0.15f,0.009f, 0.009f,0.04f, 0.04f, 0.04f);
    drawRectangle(locTransform, locTint, model,0.018f, 0.15f,0.009f, 0.009f,0.04f, 0.04f, 0.04f);
    drawRectangle(locTransform, locTint, model,0.0f, 0.132f, 0.025f, 0.006f, 0.15f, 0.05f, 0.03f);
    drawRectangle(locTransform, locTint, model, -0.065f, 0.085f, 0.035f, 0.075f, 0.48f, 0.26f, 0.15f);
    glm::mat4 weaponModel = model;
    weaponModel = glm::translate(weaponModel,glm::vec3(-0.075f, 0.085f, 0.0f));
    weaponModel = glm::rotate(weaponModel,glm::radians(weaponAngle),glm::vec3(0.0f, 0.0f, 1.0f));
    drawRectangle(locTransform, locTint, weaponModel,-0.055f, 0.0f,0.12f, 0.016f,0.30f, 0.16f, 0.07f);
    drawRectangle(locTransform, locTint, weaponModel,-0.105f, 0.0f,0.025f, 0.055f,0.28f, 0.14f, 0.06f);
    drawRectangle(locTransform, locTint, weaponModel,-0.125f, 0.0f,0.08f, 0.007f,0.75f, 0.72f, 0.60f);

}
void drawHardEnemy(GLint locTransform, GLint locTint, float x, float feetY, float axeAngle,float damageScale) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, feetY, 0));
	model = glm::scale(model, glm::vec3(damageScale, damageScale, 1.0f));
    drawRectangle(locTransform, locTint, model, -.035f, .025f, .05f, .08f, .25f, .13f, .08f);
    drawRectangle(locTransform, locTint, model, .035f, .025f, .05f, .08f, .25f, .13f, .08f);
    drawRectangle(locTransform, locTint, model, 0, .11f, .15f, .13f, .36f, .20f, .12f);
    drawRectangle(locTransform, locTint, model, 0, .13f, .11f, .07f, .40f, .35f, .28f);
    drawRectangle(locTransform, locTint, model, 0, .07f, .015f, .025f, .15f, .08f, .04f);
    drawRectangle(locTransform, locTint, model, 0, .22f, .10f, .08f, .55f, .33f, .20f);
    drawRectangle(locTransform, locTint, model, 0, .255f, .13f, .045f, .18f, .18f, .18f);
    drawRectangle(locTransform, locTint, model, -.022f, .225f, .012f, .012f, .05f, .05f, .05f);
    drawRectangle(locTransform, locTint, model, .022f, .225f, .012f, .012f, .05f, .05f, .05f);
    drawRectangle(locTransform, locTint, model, 0, .202f, .035f, .008f, .12f, .05f, .03f);
    drawRectangle(locTransform, locTint, model, -.065f, .295f, .06f, .018f, .85f, .75f, .55f);
    drawRectangle(locTransform, locTint, model, .065f, .295f, .06f, .018f, .85f, .75f, .55f);
    drawRectangle(locTransform, locTint, model, -.085f, .14f, .045f, .11f, .45f, .24f, .14f);
    drawRectangle(locTransform, locTint, model, .085f, .14f, .045f, .11f, .45f, .24f, .14f);
    glm::mat4 axeModel = model;
    axeModel = glm::translate(axeModel,glm::vec3(-0.10f, 0.16f, 0.0f) );
    axeModel = glm::rotate(axeModel, glm::radians(axeAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    drawRectangle(locTransform,locTint,axeModel,-0.05f,0.0f,0.12f,0.018f,0.35f,0.18f,0.08f);
    drawRectangle(locTransform,locTint,axeModel,-0.11f,0.0f,0.065f,0.07f,0.70f,0.70f,0.68f);
}
void drawEnemies(GLint locTransform, GLint locTint) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active)
            continue;
        float feetY = laneY[enemies[i].lane] - .06f;
		float fraction = (float)enemies[i].health / enemies[i].maxHealth;
		float damageScale = .40f + fraction * 0.60f;
        switch (enemies[i].type) {
            case EASY:{
                float weaponAngle = 0.0f;
                if (enemies[i].attackTime > 1.4f)
                    weaponAngle = -12.0f;
                drawEasyEnemy(locTransform, locTint, enemies[i].x, feetY, weaponAngle, damageScale);
                break;
            }
            case MEDIUM: {
                float weaponAngle = 0.0f;
                if (enemies[i].attackTime > 1.4f)
                    weaponAngle = -12.0f;
                drawMediumEnemy(locTransform, locTint, enemies[i].x, feetY, weaponAngle, damageScale);
                break;
            }
            case HARD: {
                float axeAngle = 10.0f;
                if (enemies[i].movementBlocked) {
                    if (enemies[i].attackTime < 0.5f)
                        axeAngle = -35.0f;
                    else
                        axeAngle = 15.0f;
                }
                drawHardEnemy(locTransform, locTint, enemies[i].x, feetY, axeAngle, damageScale);
                break;
            }
        default:
            break;
        }
        float spacing = .019f;

        float firstX =
            enemies[i].x
            - (enemies[i].maxHealth - 1) * spacing * .5f;

        float healthY = feetY - .03f;

        for (int point = 0; point < enemies[i].maxHealth; point++)
        {
            bool filled = point < enemies[i].health;

            float red;
            float green;
            float blue;

            if (filled) {
                red = .90f;
                green = .20f;
                blue = .20f;
            }
            else {
                red = .30f;
                green = .30f;
                blue = .30f;
            }

            drawRectangle(
                locTransform,
                locTint,
                glm::mat4(1),
                firstX + point * spacing,
                healthY,
                .014f,
                .016f,
                red,
                green,
                blue
            );
        }

    }

}