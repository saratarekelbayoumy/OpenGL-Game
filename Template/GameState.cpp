#include <iostream>
#include <string>
#include "GameState.h"

int g_screenWidth = 800;
int g_screenHeight = 600;
float laneY[3] = { -0.55f, -0.10f, 0.35f };
float cardX[3] = { -0.7f, -0.3f, 0.1f };
int hoveredLane = -1;
int hoveredCard = -1;
int placementLane = -1;
int coins = 30;
bool selectedLanes[3] = { false, false, false };
int selectionOrder[3] = { 0, 0, 0};
int selectionCounter = 0;
Player players[MAX_PLAYERS] = {};

void updatePlacementLane(){
    placementLane = -1;
    int latestSelection = -1;
    for (int i = 0; i < 3; i++) {
        if (selectedLanes[i] && selectionOrder[i] > latestSelection) {
            latestSelection = selectionOrder[i];
            placementLane = i;
        }
    }
}

void updateWindowTitle(GLFWwindow* window)
{
    std::string title =
        "Castle Gate Defenders | Coins: " +
        std::to_string(coins) +
        " | Test cost: 10 each" +
        " | W/S: dodge | H: damage | B: block";

    glfwSetWindowTitle(window, title.c_str());
}

void placePlayer(GLFWwindow* window, int type){
    if (type < MELEE || type > DEFENSE || coins < PLAYER_COST) {
        return;
    }

    if (placementLane == -1) {
        std::cout << "Select a lane first.\n";
        return;
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].active) {
            players[i].active = true;
            players[i].type = type;
            players[i].lane = placementLane;
            players[i].x = -0.60f;
            players[i].maxHealth = type == DEFENSE ? 7 : 5;
            players[i].health = players[i].maxHealth;
            players[i].jumping = false;
            players[i].ducking = false;
            players[i].actionTime = 0.0f;
            players[i].attackTime = 0.0f;
            players[i].movementBlocked = false;
            players[i].firstAttackPending = type != DEFENSE;
            players[i].attacksThisFrame = 0;
            coins -= PLAYER_COST;

            if (coins < PLAYER_COST)
                hoveredCard = -1;

            updateWindowTitle(window);
            return;
        }
    }

    std::cout << "no free player slots.\n";

}

void damagePlayer(int index, int damage){
    if (index < 0 || index >= MAX_PLAYERS || damage <= 0)
        return;

    if (!players[index].active)
        return;

    players[index].health -= damage;

    if (players[index].health <= 0) {
        players[index].health = 0;
        players[index].active = false;
        players[index].jumping = false;
        players[index].ducking = false;
        players[index].actionTime = 0.0f;
        players[index].attackTime = 0.0f;
        players[index].movementBlocked = false;
        players[index].firstAttackPending = false;
        players[index].attacksThisFrame = 0;
    }

    std::cout << "Player " << index
              << " health: " << players[index].health
              << "/" << players[index].maxHealth << "\n";
}

void setPlayerMovementBlocked(int index, bool blocked){
    if (index < 0 || index >= MAX_PLAYERS)
        return;

    if (!players[index].active || players[index].type == DEFENSE)
        return; 



    players[index].movementBlocked = blocked;
}

void updatePlayers(float deltaTime){
    float playerSpeed = 0.15f;
    
    for(int i = 0; i < MAX_PLAYERS; i++){
     players[i].attacksThisFrame = 0;

     if (!players[i].active)
         continue;
    if (players[i].type != DEFENSE) {
        if (!players[i].movementBlocked){

        

        players[i].x += playerSpeed * deltaTime;

        if (players[i].x > 0.85f) {
            players[i].x = 0.85f;
        }
    }
    


    

    if (players[i].firstAttackPending) {
        players[i].firstAttackPending = false;
        players[i].attackTime = 0.0f;
        players[i].attacksThisFrame = 1;
        
    }
    else {
        players[i].attackTime += deltaTime;
        while (players[i].attackTime >= ATTACK_DURATION) {

            players[i].attackTime -= ATTACK_DURATION;
            players[i].attacksThisFrame++;
        }
    }
    }


    if (players[i].jumping) {
        players[i].actionTime += deltaTime;
        if (players[i].actionTime >= JUMP_DURATION) {
            players[i].jumping = false;
            players[i].actionTime = 0.0f;
        }
    }

    else if (players[i].ducking) {
        players[i].actionTime += deltaTime;
        if (players[i].actionTime >= DUCK_DURATION) {
            players[i].ducking = false;
            players[i].actionTime = 0.0f;
        }
    }

}
}


// Temporary attack output for testing
void printPlayerAttacks() {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (players[i].active && players[i].attacksThisFrame > 0) {
            std::cout << "Player " << i << " attacks " << players[i].attacksThisFrame << " times this frame.\n";
        }
    }
}

