#include <GLFW/glfw3.h>

extern int g_screenWidth;
extern int g_screenHeight;
const int MELEE = 0;
const int RANGED = 1;
const int DEFENSE = 2;
const int MAX_PLAYERS = 50;
const int PLAYER_COST = 10;
const float JUMP_DURATION = 0.6f;
const float JUMP_HEIGHT = 0.15f;
const float DUCK_DURATION = 0.5f;
const float ATTACK_DURATION = 0.8f;
const float PI = 3.14159265f;
extern float laneY[3];
extern float cardX[3];
extern int hoveredLane;
extern int hoveredCard;
extern int placementLane;
extern int coins;
extern bool selectedLanes[3];
extern int selectionOrder[3];
extern int selectionCounter;

struct Player {
    bool active;
    int type;
    int lane;
    float x;
    int health;
    int maxHealth;
    bool jumping;
    bool ducking;
    float actionTime;
    float attackTime;
    bool movementBlocked;
    bool firstAttackPending;
    int attacksThisFrame;
};

extern Player players[MAX_PLAYERS];

void placePlayer(GLFWwindow* window, int type);
void updateWindowTitle(GLFWwindow* window);
void updatePlacementLane();
void damagePlayer(int playerIndex, int damage);
void setPlayerMovementBlocked(int playerIndex, bool blocked);
void updatePlayers(float deltaTime);
void printPlayerAttacks();


