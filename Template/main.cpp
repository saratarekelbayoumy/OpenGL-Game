#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int g_screenWidth = 800;
int g_screenHeight = 600;

const int MELEE = 0;
const int RANGED = 1;
const int DEFENSE = 2;

const int MAIN_MENU = 0;
const int PLAYING = 1;
const int PAUSED = 2;
const int WIN = 3;
const int LOSE = 4;

const int MAX_PLAYERS = 50;
const int PLAYER_COST = 10;

const float JUMP_DURATION = .60f;
const float JUMP_HEIGHT = .15f;
const float DUCK_DURATION = .50f;
const float ATTACK_DURATION = .80f;
const float PI = 3.14159265f;

int currentScreen = MAIN_MENU;
float gameTime = 0;

int coins = 30;
int hoveredLane = -1;
int hoveredCard = -1;

bool selectedLanes[3] = { false, false, false };
int selectionOrder[3] = { 0, 0, 0 };
int selectionCounter = 0;
int placementLane = -1;

float laneY[3] = { -.55f, -.10f, .35f };
float cardX[3] = { -.70f, -.30f, .10f };

float mouseX = -2;
float mouseY = -2;

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

Player players[MAX_PLAYERS] = {};

struct ModelPart {
    float x, y;
    float width, height;
    float angle;
    float red, green, blue;
};

ModelPart meleeParts[] = {
    // Legs and boots
    {-.023f,.036f,.026f,.050f,0,.40f,.45f,.55f},
    { .023f,.036f,.026f,.050f,0,.40f,.45f,.55f},
    {-.023f,.011f,.038f,.022f,0,.18f,.20f,.25f},
    { .026f,.011f,.040f,.022f,0,.18f,.20f,.25f},

    // Tunic and armor
    {0,.083f,.080f,.086f,0,.25f,.45f,.75f},
    {0,.097f,.070f,.058f,0,.65f,.70f,.80f},
    {0,.097f,.024f,.052f,0,.80f,.83f,.90f},

    // Arm and shoulders
    {-.048f,.085f,.020f,.056f,0,.40f,.45f,.55f},
    {-.043f,.119f,.032f,.024f,0,.72f,.76f,.84f},
    { .043f,.119f,.032f,.024f,0,.72f,.76f,.84f},

    // Belt and helmet
    {0,.062f,.082f,.014f,0,.25f,.18f,.12f},
    {0,.062f,.016f,.014f,0,.85f,.65f,.25f},
    {0,.132f,.026f,.024f,0,.85f,.70f,.52f},
    {0,.157f,.064f,.058f,0,.68f,.73f,.82f},
    {.009f,.154f,.054f,.029f,0,.42f,.47f,.56f},
    {.013f,.159f,.045f,.006f,0,.10f,.12f,.16f},
    {-.010f,.187f,.026f,.025f,0,.65f,.20f,.20f}
};

ModelPart rangedParts[] = {
    // Quiver
    {-.054f,.101f,.027f,.072f,-15,.45f,.27f,.13f},
    {-.065f,.147f,.004f,.049f,-15,.82f,.72f,.50f},
    {-.054f,.151f,.004f,.054f,-15,.82f,.72f,.50f},
    {-.065f,.169f,.014f,.012f,-15,.80f,.80f,.75f},
    {-.054f,.175f,.014f,.012f,-15,.80f,.80f,.75f},

    // Legs and boots
    {-.022f,.036f,.023f,.050f,0,.30f,.28f,.18f},
    { .022f,.036f,.023f,.050f,0,.30f,.28f,.18f},
    {-.023f,.011f,.034f,.022f,0,.25f,.16f,.10f},
    { .025f,.011f,.038f,.022f,0,.25f,.16f,.10f},

    // Tunic and belt
    {0,.086f,.074f,.080f,0,.25f,.55f,.30f},
    {0,.049f,.086f,.022f,0,.20f,.43f,.25f},
    {-.044f,.090f,.018f,.052f,0,.25f,.55f,.30f},
    {0,.069f,.077f,.012f,0,.35f,.22f,.12f},
    {.005f,.069f,.013f,.012f,0,.75f,.60f,.25f},

    // Hood and face
    {-.003f,.155f,.070f,.066f,0,.18f,.38f,.22f},
    {.009f,.151f,.044f,.043f,0,.90f,.75f,.55f},
    {.005f,.174f,.060f,.014f,0,.23f,.47f,.27f},
    {.024f,.155f,.007f,.005f,0,.12f,.12f,.10f}
};

ModelPart defenseParts[] = {
    // Legs and boots
    {-.028f,.036f,.033f,.052f,0,.42f,.45f,.53f},
    { .028f,.036f,.033f,.052f,0,.42f,.45f,.53f},
    {-.029f,.012f,.045f,.024f,0,.22f,.24f,.30f},
    { .032f,.012f,.048f,.024f,0,.22f,.24f,.30f},

    // Armor
    {0,.088f,.110f,.090f,0,.58f,.62f,.72f},
    {0,.098f,.082f,.058f,0,.72f,.75f,.83f},
    {-.061f,.085f,.024f,.059f,0,.42f,.46f,.55f},
    { .061f,.085f,.024f,.059f,0,.42f,.46f,.55f},
    {-.055f,.124f,.046f,.030f,0,.80f,.82f,.88f},
    { .055f,.124f,.046f,.030f,0,.80f,.82f,.88f},

    // Belt and helmet
    {0,.062f,.110f,.016f,0,.27f,.24f,.20f},
    {-.005f,.062f,.020f,.016f,0,.80f,.65f,.28f},
    {0,.159f,.078f,.062f,0,.62f,.67f,.77f},
    {0,.184f,.086f,.015f,0,.80f,.83f,.90f},
    {.010f,.157f,.056f,.010f,0,.12f,.14f,.20f},
    {-.006f,.157f,.012f,.055f,0,.80f,.83f,.90f}
};

ModelPart castleParts[] = {
    {0,0,.140f,1.180f,0,.47f,.50f,.57f},
    {-.068f,0,.054f,1.300f,0,.58f,.61f,.68f},
    { .068f,0,.054f,1.300f,0,.58f,.61f,.68f},
    {-.068f,-.640f,.068f,.060f,0,.38f,.41f,.48f},
    { .068f,-.640f,.068f,.060f,0,.38f,.41f,.48f},
    {-.068f,.640f,.066f,.040f,0,.73f,.76f,.82f},
    { .068f,.640f,.066f,.040f,0,.73f,.76f,.82f},
    {-.089f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    {-.047f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    { .047f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    { .089f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    {0,-.120f,.098f,.880f,0,.76f,.74f,.67f},
    {0,-.130f,.074f,.800f,0,.34f,.22f,.13f},
    {-.025f,-.130f,.004f,.780f,0,.20f,.13f,.08f},
    {0,-.130f,.004f,.780f,0,.20f,.13f,.08f},
    {.025f,-.130f,.004f,.780f,0,.20f,.13f,.08f},
    {0,.100f,.074f,.025f,0,.17f,.19f,.23f},
    {0,-.350f,.074f,.025f,0,.17f,.19f,.23f},
    {-.012f,-.130f,.009f,.045f,0,.85f,.68f,.27f},
    { .012f,-.130f,.009f,.045f,0,.85f,.68f,.27f},
    {0,.470f,.080f,.160f,0,.85f,.68f,.27f},
    {0,.470f,.060f,.130f,0,.25f,.36f,.65f},
    {0,.470f,.010f,.085f,0,.90f,.75f,.35f},
    {0,.485f,.040f,.014f,0,.90f,.75f,.35f},
    {-.068f,.350f,.017f,.100f,0,.14f,.17f,.23f},
    { .068f,.350f,.017f,.100f,0,.14f,.17f,.23f},
    {-.068f,-.300f,.017f,.100f,0,.14f,.17f,.23f},
    { .068f,-.300f,.017f,.100f,0,.14f,.17f,.23f}
};

ModelPart bannerParts[] = {
    {0,.012f,.055f,.024f,0,.38f,.40f,.45f},
    {0,.100f,.012f,.180f,0,.60f,.45f,.25f},
    {0,.196f,.022f,.020f,45,.85f,.68f,.27f},
    {.060f,.140f,.100f,.095f,0,.22f,.34f,.62f},
    {.060f,.184f,.100f,.007f,0,.85f,.68f,.27f},
    {.107f,.140f,.006f,.095f,0,.85f,.68f,.27f},
    {.060f,.096f,.100f,.007f,0,.85f,.68f,.27f},
    {.060f,.140f,.008f,.055f,0,.90f,.75f,.35f},
    {.060f,.148f,.040f,.008f,0,.90f,.75f,.35f}
};

ModelPart torchParts[] = {
    {0,.075f,.030f,.065f,0,.25f,.27f,.31f},
    {.012f,.060f,.045f,.012f,0,.40f,.42f,.47f},
    {.025f,.095f,.014f,.090f,0,.42f,.25f,.12f},
    {.025f,.070f,.024f,.012f,0,.60f,.62f,.67f},
    {.025f,.128f,.037f,.018f,0,.30f,.32f,.36f},
    {.025f,.139f,.045f,.012f,0,.60f,.62f,.67f}
};

ModelPart crateParts[] = {
    {0,.070f,.140f,.140f,0,.43f,.27f,.13f},
    {-.025f,.070f,.004f,.115f,0,.25f,.16f,.08f},
    { .025f,.070f,.004f,.115f,0,.25f,.16f,.08f},
    {0,.132f,.140f,.016f,0,.65f,.44f,.23f},
    {0,.008f,.140f,.016f,0,.65f,.44f,.23f},
    {-.062f,.070f,.016f,.140f,0,.65f,.44f,.23f},
    { .062f,.070f,.016f,.140f,0,.65f,.44f,.23f},
    {0,.070f,.165f,.012f,45,.58f,.37f,.18f},
    {0,.070f,.165f,.012f,-45,.58f,.37f,.18f},
    {-.053f,.123f,.008f,.008f,0,.20f,.21f,.23f},
    { .053f,.123f,.008f,.008f,0,.20f,.21f,.23f},
    {-.053f,.017f,.008f,.008f,0,.20f,.21f,.23f},
    { .053f,.017f,.008f,.008f,0,.20f,.21f,.23f}
};

ModelPart bushParts[] = {
    {0,.026f,.012f,.050f,0,.35f,.23f,.12f},
    {-.023f,.036f,.010f,.058f,-30,.35f,.23f,.12f},
    { .023f,.036f,.010f,.058f,30,.35f,.23f,.12f},
    {-.042f,.055f,.064f,.055f,-18,.15f,.34f,.18f},
    { .043f,.055f,.064f,.055f,18,.15f,.34f,.18f},
    {-.025f,.078f,.060f,.060f,30,.22f,.44f,.23f},
    { .025f,.078f,.060f,.060f,-30,.22f,.44f,.23f},
    {0,.095f,.060f,.060f,45,.28f,.51f,.27f},
    {0,.050f,.075f,.045f,0,.20f,.41f,.21f},
    {-.032f,.083f,.018f,.009f,-25,.40f,.62f,.32f},
    { .029f,.070f,.018f,.009f,25,.40f,.62f,.32f},
    {0,.112f,.015f,.008f,0,.45f,.66f,.35f}
};

bool digitSegments[10][7] = {
    {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0},
    {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1},
    {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1},
    {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}
};

ModelPart digitParts[] = {
    {0,.060f,.055f,.009f,0,1,1,1},
    {.030f,.030f,.009f,.050f,0,1,1,1},
    {.030f,-.030f,.009f,.050f,0,1,1,1},
    {0,-.060f,.055f,.009f,0,1,1,1},
    {-.030f,-.030f,.009f,.050f,0,1,1,1},
    {-.030f,.030f,.009f,.050f,0,1,1,1},
    {0,0,.055f,.009f,0,1,1,1}
};

ModelPart coinIconParts[] = {
    {0,0,.075f,.100f,0,.65f,.43f,.10f},
    {0,0,.057f,.082f,0,.95f,.73f,.22f},
    {0,0,.010f,.050f,0,.70f,.46f,.10f},
    {0,.014f,.030f,.009f,0,.70f,.46f,.10f},
    {0,-.014f,.030f,.009f,0,.70f,.46f,.10f}
};

// Uppercase letters drawn using small rectangles.
const char* letterPatterns[26] = {
    "01110" "10001" "10001" "11111" "10001" "10001" "10001",
    "11110" "10001" "10001" "11110" "10001" "10001" "11110",
    "01111" "10000" "10000" "10000" "10000" "10000" "01111",
    "11110" "10001" "10001" "10001" "10001" "10001" "11110",
    "11111" "10000" "10000" "11110" "10000" "10000" "11111",
    "11111" "10000" "10000" "11110" "10000" "10000" "10000",
    "01111" "10000" "10000" "10111" "10001" "10001" "01111",
    "10001" "10001" "10001" "11111" "10001" "10001" "10001",
    "11111" "00100" "00100" "00100" "00100" "00100" "11111",
    "00111" "00010" "00010" "00010" "10010" "10010" "01100",
    "10001" "10010" "10100" "11000" "10100" "10010" "10001",
    "10000" "10000" "10000" "10000" "10000" "10000" "11111",
    "10001" "11011" "10101" "10101" "10001" "10001" "10001",
    "10001" "11001" "10101" "10011" "10001" "10001" "10001",
    "01110" "10001" "10001" "10001" "10001" "10001" "01110",
    "11110" "10001" "10001" "11110" "10000" "10000" "10000",
    "01110" "10001" "10001" "10001" "10101" "10010" "01101",
    "11110" "10001" "10001" "11110" "10100" "10010" "10001",
    "01111" "10000" "10000" "01110" "00001" "00001" "11110",
    "11111" "00100" "00100" "00100" "00100" "00100" "00100",
    "10001" "10001" "10001" "10001" "10001" "10001" "01110",
    "10001" "10001" "10001" "10001" "10001" "01010" "00100",
    "10001" "10001" "10001" "10101" "10101" "10101" "01010",
    "10001" "10001" "01010" "00100" "01010" "10001" "10001",
    "10001" "10001" "01010" "00100" "00100" "00100" "00100",
    "11111" "00001" "00010" "00100" "01000" "10000" "11111"
};

const char* vertexShaderSource = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vColor;
uniform mat4 u_Transform;
void main()
{
    gl_Position = u_Transform * vec4(aPos, 1.0);
    vColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 430 core
in vec3 vColor;
out vec4 FragColor;
uniform vec4 u_Tint;
void main()
{
    FragColor = vec4(vColor, 1.0) * u_Tint;
}
)";

GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compile error:\n" << log << "\n";
    }

    return shader;
}

void drawRectangle(
    GLint locTransform, GLint locTint,
    const glm::mat4& parent,
    float x, float y, float width, float height,
    float red, float green, float blue, float angle = 0)
{
    glm::mat4 transform = glm::translate(
        parent, glm::vec3(x, y, 0)
    );
    transform = glm::rotate(
        transform, glm::radians(angle), glm::vec3(0, 0, 1)
    );
    transform = glm::scale(
        transform, glm::vec3(width, height, 1)
    );

    glUniformMatrix4fv(
        locTransform, 1, GL_FALSE, glm::value_ptr(transform)
    );
    glUniform4f(locTint, red, green, blue, 1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawParts(
    GLint locTransform, GLint locTint,
    const glm::mat4& parent,
    ModelPart* parts, int count, bool grayscale = false)
{
    for (int i = 0; i < count; i++) {
        float red = parts[i].red;
        float green = parts[i].green;
        float blue = parts[i].blue;

        if (grayscale)
            red = green = blue = (red + green + blue) / 3;

        drawRectangle(
            locTransform, locTint, parent,
            parts[i].x, parts[i].y,
            parts[i].width, parts[i].height,
            red, green, blue, parts[i].angle
        );
    }
}

void drawText(
    GLint locTransform, GLint locTint,
    const std::string& text,
    float centerX, float centerY, float cell,
    float red, float green, float blue)
{
    if (text.empty()) return;

    float textWidth = ((int)text.size() * 6 - 1) * cell;
    float left = centerX - textWidth * .50f;
    glm::mat4 identity = glm::mat4(1);

    for (int character = 0; character < (int)text.size(); character++) {
        char letter = text[character];
        if (letter < 'A' || letter > 'Z') continue;

        const char* pattern = letterPatterns[letter - 'A'];

        for (int row = 0; row < 7; row++) {
            for (int column = 0; column < 5; column++) {
                if (pattern[row * 5 + column] != '1') continue;

                drawRectangle(
                    locTransform, locTint, identity,
                    left + (character * 6 + column + .5f) * cell,
                    centerY + (3 - row) * cell,
                    cell * .88f, cell * .88f,
                    red, green, blue
                );
            }
        }
    }
}

bool insideButton(float x, float y, float centerY)
{
    return x >= -.32f && x <= .32f
        && y >= centerY - .08f && y <= centerY + .08f;
}

void drawButton(
    GLint locTransform, GLint locTint,
    const std::string& label, float y)
{
    bool hovered = insideButton(mouseX, mouseY, y);
    glm::mat4 identity = glm::mat4(1);

    drawRectangle(
        locTransform, locTint, identity,
        0, y, .66f, .18f, .65f, .52f, .25f
    );
    drawRectangle(
        locTransform, locTint, identity,
        0, y, .64f, .16f,
        hovered ? .30f : .20f,
        hovered ? .43f : .29f,
        hovered ? .63f : .44f
    );
    drawText(
        locTransform, locTint, label,
        0, y, .010f, .95f, .93f, .85f
    );
}

bool getMousePosition(
    GLFWwindow* window, double xpos, double ypos,
    float& x, float& y)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if (width <= 0 || height <= 0) return false;

    x = 2.0f * (float)xpos / width - 1;
    y = 1 - 2.0f * (float)ypos / height;
    return true;
}

int getLane(float x, float y)
{
    if (x < -.70f || x > .90f) return -1;
    if (y >= -.75f && y <= -.35f) return 0;
    if (y >= -.30f && y <= .10f) return 1;
    if (y >= .15f && y <= .55f) return 2;
    return -1;
}

int getCard(float x, float y)
{
    if (y < .68f || y > .92f) return -1;

    for (int i = 0; i < 3; i++) {
        if (x >= cardX[i] - .15f && x <= cardX[i] + .15f)
            return i;
    }

    return -1;
}

void refreshHover()
{
    hoveredLane = -1;
    hoveredCard = -1;

    if (currentScreen == PLAYING) {
        hoveredLane = getLane(mouseX, mouseY);
        if (coins >= PLAYER_COST)
            hoveredCard = getCard(mouseX, mouseY);
    }
}

void updateWindowTitle(GLFWwindow* window)
{
    std::string title = "Castle Gate Defenders";

    if (currentScreen == MAIN_MENU)
        title += " | Main Menu";
    else if (currentScreen == PAUSED)
        title += " | Paused";
    else if (currentScreen == WIN)
        title += " | Victory";
    else if (currentScreen == LOSE)
        title += " | Defeat";
    else
        title += " | Coins: " + std::to_string(coins)
        + " | Test cost: 10 | W/S: dodge | H/B: test | F1/F2: outcome";

    glfwSetWindowTitle(window, title.c_str());
}

void resetGame()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
        players[i] = Player{};

    coins = 30;
    gameTime = 0;
    selectionCounter = 0;
    placementLane = -1;
    hoveredLane = -1;
    hoveredCard = -1;

    for (int i = 0; i < 3; i++) {
        selectedLanes[i] = false;
        selectionOrder[i] = 0;
    }
}

void startGame(GLFWwindow* window)
{
    resetGame();
    currentScreen = PLAYING;
    refreshHover();
    updateWindowTitle(window);
}

void returnToMainMenu(GLFWwindow* window)
{
    resetGame();
    currentScreen = MAIN_MENU;
    refreshHover();
    updateWindowTitle(window);
}

// Nour will call this when the real outcome is determined.
void finishGame(GLFWwindow* window, bool won)
{
    if (currentScreen != PLAYING) return;

    currentScreen = won ? WIN : LOSE;

    for (int i = 0; i < MAX_PLAYERS; i++)
        players[i].attacksThisFrame = 0;

    refreshHover();
    updateWindowTitle(window);
}

void updatePlacementLane()
{
    placementLane = -1;
    int latest = -1;

    for (int i = 0; i < 3; i++) {
        if (selectedLanes[i] && selectionOrder[i] > latest) {
            latest = selectionOrder[i];
            placementLane = i;
        }
    }
}

void placePlayer(GLFWwindow* window, int type)
{
    if (currentScreen != PLAYING) return;
    if (type < MELEE || type > DEFENSE || coins < PLAYER_COST) return;

    if (placementLane == -1) {
        std::cout << "Select a lane first.\n";
        return;
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].active) {
            players[i] = Player{};
            players[i].active = true;
            players[i].type = type;
            players[i].lane = placementLane;
            players[i].x = -.60f;
            players[i].maxHealth = type == DEFENSE ? 7 : 5;
            players[i].health = players[i].maxHealth;
            players[i].firstAttackPending = type != DEFENSE;

            coins -= PLAYER_COST;
            refreshHover();
            updateWindowTitle(window);
            return;
        }
    }

    std::cout << "No free player slots.\n";
}

void damagePlayer(int index, int damage)
{
    if (index < 0 || index >= MAX_PLAYERS || damage <= 0) return;
    if (!players[index].active) return;

    players[index].health -= damage;

    if (players[index].health <= 0) {
        players[index].health = 0;
        players[index].active = false;
        players[index].jumping = false;
        players[index].ducking = false;
        players[index].actionTime = 0;
        players[index].attackTime = 0;
        players[index].movementBlocked = false;
        players[index].firstAttackPending = false;
        players[index].attacksThisFrame = 0;
    }
}

void setPlayerMovementBlocked(int index, bool blocked)
{
    if (index < 0 || index >= MAX_PLAYERS) return;
    if (!players[index].active || players[index].type == DEFENSE) return;
    players[index].movementBlocked = blocked;
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    if (!getMousePosition(window, x, y, mouseX, mouseY))
        mouseX = mouseY = -2;

    refreshHover();
}

void mouse_button_callback(
    GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (!getMousePosition(window, x, y, mouseX, mouseY)) return;

    if (currentScreen == MAIN_MENU) {
        if (insideButton(mouseX, mouseY, .02f))
            startGame(window);
        else if (insideButton(mouseX, mouseY, -.20f))
            glfwSetWindowShouldClose(window, true);

        return;
    }

    if (currentScreen == PAUSED) {
        if (insideButton(mouseX, mouseY, .15f)) {
            currentScreen = PLAYING;
            refreshHover();
            updateWindowTitle(window);
        }
        else if (insideButton(mouseX, mouseY, -.07f))
            startGame(window);
        else if (insideButton(mouseX, mouseY, -.29f))
            returnToMainMenu(window);

        return;
    }

    if (currentScreen == WIN || currentScreen == LOSE) {
        if (insideButton(mouseX, mouseY, .02f))
            startGame(window);
        else if (insideButton(mouseX, mouseY, -.20f))
            returnToMainMenu(window);

        return;
    }

    if (currentScreen != PLAYING) return;

    int card = getCard(mouseX, mouseY);

    if (card != -1) {
        placePlayer(window, card);
        return;
    }

    int lane = getLane(mouseX, mouseY);

    if (lane != -1) {
        selectedLanes[lane] = !selectedLanes[lane];

        if (selectedLanes[lane]) {
            selectionCounter++;
            selectionOrder[lane] = selectionCounter;
        }

        updatePlacementLane();
    }

    refreshHover();
}

void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) {
        if (currentScreen == PLAYING)
            currentScreen = PAUSED;
        else if (currentScreen == PAUSED)
            currentScreen = PLAYING;

        refreshHover();
        updateWindowTitle(window);
        return;
    }

    if (currentScreen != PLAYING) return;

    // Temporary outcome tests.
    if (key == GLFW_KEY_F1) {
        finishGame(window, true);
        return;
    }

    if (key == GLFW_KEY_F2) {
        finishGame(window, false);
        return;
    }

    // Temporary damage and blocking tests.
    if (key == GLFW_KEY_H || key == GLFW_KEY_B) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (!players[i].active || !selectedLanes[players[i].lane])
                continue;

            if (key == GLFW_KEY_H)
                damagePlayer(i, 1);
            else if (players[i].type != DEFENSE)
                setPlayerMovementBlocked(i, !players[i].movementBlocked);
        }

        return;
    }

    bool jumpKey = key == GLFW_KEY_W || key == GLFW_KEY_UP;
    bool duckKey = key == GLFW_KEY_S || key == GLFW_KEY_DOWN;

    if (!jumpKey && !duckKey) return;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].active || players[i].type == DEFENSE) continue;
        if (!selectedLanes[players[i].lane]) continue;
        if (players[i].jumping || players[i].ducking) continue;

        players[i].actionTime = 0;
        players[i].jumping = jumpKey;
        players[i].ducking = duckKey;
    }
}

void updatePlayers(float dt)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].attacksThisFrame = 0;
        if (!players[i].active) continue;

        if (players[i].type != DEFENSE) {
            if (!players[i].movementBlocked) {
                players[i].x += .15f * dt;
                if (players[i].x > .85f) players[i].x = .85f;
            }

            if (players[i].firstAttackPending) {
                players[i].firstAttackPending = false;
                players[i].attackTime = 0;
                players[i].attacksThisFrame = 1;
            }
            else {
                players[i].attackTime += dt;

                while (players[i].attackTime >= ATTACK_DURATION) {
                    players[i].attackTime -= ATTACK_DURATION;
                    players[i].attacksThisFrame++;
                }
            }
        }

        if (players[i].jumping) {
            players[i].actionTime += dt;
            if (players[i].actionTime >= JUMP_DURATION) {
                players[i].jumping = false;
                players[i].actionTime = 0;
            }
        }
        else if (players[i].ducking) {
            players[i].actionTime += dt;
            if (players[i].actionTime >= DUCK_DURATION) {
                players[i].ducking = false;
                players[i].actionTime = 0;
            }
        }
    }
}

void drawEnvironment(GLint locTransform, GLint locTint)
{
    glm::mat4 identity = glm::mat4(1);

    drawRectangle(
        locTransform, locTint, identity,
        .10f, -.895f, 1.60f, .17f, .28f, .30f, .35f
    );

    for (int row = 0; row < 2; row++) {
        for (int column = 0; column < 12; column++) {
            float left = -.70f + column * .14f;
            if (row == 1) left -= .07f;

            float right = left + .13f;
            if (left < -.70f) left = -.70f;
            if (right > .90f) right = .90f;
            if (right <= left) continue;

            bool light = (row + column) % 2 != 0;

            drawRectangle(
                locTransform, locTint, identity,
                (left + right) * .5f, -.938f + row * .078f,
                right - left, .067f,
                light ? .51f : .46f,
                light ? .54f : .49f,
                light ? .60f : .55f
            );
        }
    }

    for (int cap = 0; cap < 10; cap++) {
        drawRectangle(
            locTransform, locTint, identity,
            -.62f + cap * .16f, -.811f,
            .153f, .026f, .66f, .69f, .74f
        );
    }

    for (int decoration = 0; decoration < 9; decoration++) {
        ModelPart* parts;
        int count;
        float x;

        if (decoration < 2) {
            parts = bannerParts;
            count = sizeof(bannerParts) / sizeof(bannerParts[0]);
            x = decoration == 0 ? -.55f : .62f;
        }
        else if (decoration < 4) {
            parts = torchParts;
            count = sizeof(torchParts) / sizeof(torchParts[0]);
            x = decoration == 2 ? -.68f : .85f;
        }
        else if (decoration < 6) {
            parts = crateParts;
            count = sizeof(crateParts) / sizeof(crateParts[0]);
            x = decoration == 4 ? -.30f : .20f;
        }
        else {
            parts = bushParts;
            count = sizeof(bushParts) / sizeof(bushParts[0]);
            x = decoration == 6 ? -.10f
                : decoration == 7 ? .43f : .78f;
        }

        glm::mat4 parent = glm::translate(
            identity, glm::vec3(x, -.985f, 0)
        );
        drawParts(locTransform, locTint, parent, parts, count);

        if (decoration == 2 || decoration == 3) {
            float phase = gameTime * 7 + decoration;
            float height = .050f + .010f * (float)std::sin(phase);
            float width = .028f + .004f * (float)std::sin(phase + 1);
            float lean = 8 * (float)std::sin(phase * .70f);

            glm::mat4 flame = glm::translate(
                parent, glm::vec3(.025f, .145f, 0)
            );
            flame = glm::rotate(
                flame, glm::radians(lean), glm::vec3(0, 0, 1)
            );

            drawRectangle(
                locTransform, locTint, flame,
                0, height * .5f, width, height, 1, .38f, .08f
            );
            drawRectangle(
                locTransform, locTint, flame,
                0, height * .30f,
                width * .50f, height * .60f, 1, .80f, .20f
            );
        }
    }

    for (int lane = 0; lane < 3; lane++) {
        float red = lane == 1 ? .43f : .36f;
        float green = lane == 1 ? .47f : .40f;
        float blue = lane == 1 ? .43f : .36f;

        if (selectedLanes[lane]) {
            red = .55f;
            green = .65f;
            blue = .35f;
        }
        else if (hoveredLane == lane) {
            red = .55f;
            green = .59f;
            blue = .55f;
        }

        drawRectangle(
            locTransform, locTint, identity,
            .10f, laneY[lane], 1.60f, .40f, red, green, blue
        );
    }

    glm::mat4 castle = glm::translate(
        identity, glm::vec3(-.83f, -.10f, 0)
    );
    drawParts(
        locTransform, locTint, castle, castleParts,
        sizeof(castleParts) / sizeof(castleParts[0])
    );

    for (int tower = 0; tower < 2; tower++) {
        for (int row = 0; row < 9; row++) {
            drawRectangle(
                locTransform, locTint, castle,
                tower == 0 ? -.068f : .068f,
                -.57f + row * .14f,
                .054f, .005f, .40f, .43f, .50f
            );
        }
    }
}

void drawCharacter(
    GLint locTransform, GLint locTint,
    int type, float x, float feetY,
    float damageScale, float heightScale,
    float attackProgress, bool grayscale)
{
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(x, feetY, 0));
    model = glm::scale(
        model, glm::vec3(damageScale, damageScale * heightScale, 1)
    );

    ModelPart* parts;
    int count;

    if (type == MELEE) {
        parts = meleeParts;
        count = sizeof(meleeParts) / sizeof(meleeParts[0]);
    }
    else if (type == RANGED) {
        parts = rangedParts;
        count = sizeof(rangedParts) / sizeof(rangedParts[0]);
    }
    else {
        parts = defenseParts;
        count = sizeof(defenseParts) / sizeof(defenseParts[0]);
    }

    drawParts(locTransform, locTint, model, parts, count, grayscale);

    if (type == MELEE) {
        drawRectangle(
            locTransform, locTint, model,
            .04f, .085f, .045f, .025f, .60f, .60f, .60f
        );

        glm::mat4 weapon = glm::translate(
            model, glm::vec3(.065f, .085f, 0)
        );
        float angle = -70 * (float)std::sin(attackProgress * PI);
        weapon = glm::rotate(
            weapon, glm::radians(angle), glm::vec3(0, 0, 1)
        );

        drawRectangle(
            locTransform, locTint, weapon,
            0, .055f, .015f, .09f, .85f, .85f, .85f
        );
        drawRectangle(
            locTransform, locTint, weapon,
            0, .01f, .045f, .012f,
            grayscale ? .50f : .75f,
            grayscale ? .50f : .55f,
            grayscale ? .50f : .20f
        );
        drawRectangle(
            locTransform, locTint, weapon,
            0, -.01f, .012f, .03f,
            grayscale ? .35f : .40f,
            grayscale ? .35f : .25f,
            grayscale ? .35f : .15f
        );
    }
    else if (type == RANGED) {
        float recoil = .015f * (float)std::sin(attackProgress * PI);

        drawRectangle(
            locTransform, locTint, model,
            .04f - recoil, .10f, .045f, .025f,
            grayscale ? .55f : .25f,
            .55f, grayscale ? .55f : .30f
        );

        for (int part = 0; part < 2; part++) {
            drawRectangle(
                locTransform, locTint, model,
                .07f - recoil, part == 0 ? .125f : .075f,
                .012f, .065f,
                grayscale ? .50f : .65f,
                grayscale ? .50f : .40f,
                grayscale ? .50f : .20f,
                part == 0 ? 25.0f : -25.0f
            );
        }

        drawRectangle(
            locTransform, locTint, model,
            .057f - recoil, .10f, .004f, .11f, .85f, .85f, .85f
        );
    }
    else {
        drawRectangle(
            locTransform, locTint, model,
            .045f, .085f, .08f, .13f, .85f, .85f, .85f
        );
        drawRectangle(
            locTransform, locTint, model,
            .045f, .085f, .06f, .11f,
            grayscale ? .45f : .35f,
            grayscale ? .45f : .40f,
            grayscale ? .45f : .65f
        );

        for (int stripe = 0; stripe < 2; stripe++) {
            drawRectangle(
                locTransform, locTint, model,
                .045f, stripe == 0 ? .085f : .092f,
                stripe == 0 ? .010f : .038f,
                stripe == 0 ? .065f : .010f,
                grayscale ? .75f : .85f,
                grayscale ? .75f : .70f,
                grayscale ? .75f : .30f
            );
        }
    }
}

void drawGameplay(GLint locTransform, GLint locTint)
{
    glm::mat4 identity = glm::mat4(1);

    drawRectangle(
        locTransform, locTint, identity,
        0, .80f, 2, .30f, .20f, .23f, .30f
    );

    for (int type = 0; type < 3; type++) {
        bool available = coins >= PLAYER_COST;
        bool hovered = available && hoveredCard == type;

        drawRectangle(
            locTransform, locTint, identity,
            cardX[type], .80f, .30f, .24f,
            !available ? .35f : hovered ? .40f : .25f,
            !available ? .35f : hovered ? .60f : .40f,
            !available ? .35f : hovered ? .85f : .65f
        );

        drawCharacter(
            locTransform, locTint,
            type, cardX[type], .71f, 1, 1, 0, !available
        );
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].active) continue;

        float feetY = laneY[players[i].lane] - .06f;

        if (players[i].jumping) {
            float u = players[i].actionTime / JUMP_DURATION;
            feetY += 4 * JUMP_HEIGHT * u * (1 - u);
        }

        float fraction =
            (float)players[i].health / players[i].maxHealth;

        drawCharacter(
            locTransform, locTint,
            players[i].type, players[i].x, feetY,
            .40f + .60f * fraction,
            players[i].ducking ? .50f : 1.0f,
            players[i].attackTime / ATTACK_DURATION, false
        );
    }

    // Health indicators
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].active) continue;

        float feetY = laneY[players[i].lane] - .06f;

        if (players[i].jumping) {
            float u = players[i].actionTime / JUMP_DURATION;
            feetY += 4 * JUMP_HEIGHT * u * (1 - u);
        }

        float spacing = .019f;
        float rowWidth = (players[i].maxHealth - 1) * spacing + .014f;
        float firstX = players[i].x
            - (players[i].maxHealth - 1) * spacing * .5f;
        float y = feetY - .026f;

        drawRectangle(
            locTransform, locTint, identity,
            players[i].x, y, rowWidth + .012f, .028f,
            .10f, .12f, .14f
        );

        for (int point = 0; point < players[i].maxHealth; point++) {
            bool filled = point < players[i].health;

            drawRectangle(
                locTransform, locTint, identity,
                firstX + point * spacing, y, .014f, .016f,
                filled ? .30f : .32f,
                filled ? .90f : .34f,
                filled ? .35f : .36f
            );
        }
    }

    // Coin counter
    drawRectangle(
        locTransform, locTint, identity,
        .63f, .80f, .55f, .22f, .12f, .15f, .20f
    );

    glm::mat4 icon = glm::translate(
        identity, glm::vec3(.43f, .80f, 0)
    );
    drawParts(
        locTransform, locTint, icon, coinIconParts,
        sizeof(coinIconParts) / sizeof(coinIconParts[0])
    );

    std::string coinText = std::to_string(coins);
    float scale = coinText.size() > 4
        ? 4.0f / (float)coinText.size() : 1.0f;

    for (int position = 0; position < (int)coinText.size(); position++) {
        int digit = coinText[position] - '0';
        if (digit < 0 || digit > 9) continue;

        glm::mat4 parent = glm::translate(
            identity,
            glm::vec3(.55f + position * .085f * scale, .80f, 0)
        );
        parent = glm::scale(parent, glm::vec3(scale, scale, 1));

        for (int segment = 0; segment < 7; segment++) {
            if (!digitSegments[digit][segment]) continue;

            drawRectangle(
                locTransform, locTint, parent,
                digitParts[segment].x, digitParts[segment].y,
                digitParts[segment].width, digitParts[segment].height,
                .95f, .85f, .45f
            );
        }
    }
}

void drawMenu(GLint locTransform, GLint locTint)
{
    glm::mat4 identity = glm::mat4(1);

    drawRectangle(
        locTransform, locTint, identity,
        0, .03f, 1.48f, 1.08f, .60f, .48f, .23f
    );
    drawRectangle(
        locTransform, locTint, identity,
        0, .03f, 1.44f, 1.04f, .10f, .14f, .21f
    );

    if (currentScreen == MAIN_MENU) {
        drawText(
            locTransform, locTint, "CASTLE GATE",
            0, .37f, .018f, .95f, .80f, .40f
        );
        drawText(
            locTransform, locTint, "DEFENDERS",
            0, .20f, .014f, .90f, .92f, .96f
        );
        drawButton(locTransform, locTint, "START", .02f);
        drawButton(locTransform, locTint, "EXIT", -.20f);
    }
    else if (currentScreen == PAUSED) {
        drawText(
            locTransform, locTint, "PAUSED",
            0, .39f, .018f, .95f, .80f, .40f
        );
        drawButton(locTransform, locTint, "CONTINUE", .15f);
        drawButton(locTransform, locTint, "RESTART", -.07f);
        drawButton(locTransform, locTint, "MAIN MENU", -.29f);
    }
    else if (currentScreen == WIN) {
        drawText(
            locTransform, locTint, "VICTORY",
            0, .35f, .020f, .45f, .95f, .55f
        );
        drawText(
            locTransform, locTint, "THE GATE IS SAFE",
            0, .20f, .009f, .90f, .92f, .96f
        );
        drawButton(locTransform, locTint, "RESTART", .02f);
        drawButton(locTransform, locTint, "MAIN MENU", -.20f);
    }
    else if (currentScreen == LOSE) {
        drawText(
            locTransform, locTint, "DEFEAT",
            0, .35f, .020f, 1.0f, .40f, .35f
        );
        drawText(
            locTransform, locTint, "THE GATE HAS FALLEN",
            0, .20f, .009f, .90f, .92f, .96f
        );
        drawButton(locTransform, locTint, "RESTART", .02f);
        drawButton(locTransform, locTint, "MAIN MENU", -.20f);
    }
}

int main()
{
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        g_screenWidth, g_screenHeight,
        "Castle Gate Defenders", nullptr, nullptr
    );

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    updateWindowTitle(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, g_screenWidth, g_screenHeight);

    GLuint vertexShader = compileShader(
        GL_VERTEX_SHADER, vertexShaderSource
    );
    GLuint fragmentShader = compileShader(
        GL_FRAGMENT_SHADER, fragmentShaderSource
    );

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char log[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, log);
        std::cerr << "Shader link error:\n" << log << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -.5f,-.5f,0, 1,1,1,
         .5f,-.5f,0, 1,1,1,
         .5f, .5f,0, 1,1,1,
        -.5f,-.5f,0, 1,1,1,
         .5f, .5f,0, 1,1,1,
        -.5f, .5f,0, 1,1,1
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint locTransform = glGetUniformLocation(
        shaderProgram, "u_Transform"
    );
    GLint locTint = glGetUniformLocation(
        shaderProgram, "u_Tint"
    );

    float lastFrame = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        for (int i = 0; i < MAX_PLAYERS; i++)
            players[i].attacksThisFrame = 0;

        if (currentScreen == PLAYING) {
            gameTime += dt;
            updatePlayers(dt);

            // Temporary attack output
            for (int i = 0; i < MAX_PLAYERS; i++) {
                if (players[i].active &&
                    players[i].attacksThisFrame > 0)
                {
                    std::cout << "Player " << i << " began "
                        << players[i].attacksThisFrame
                        << " attack(s)\n";
                }
            }
        }

        glClearColor(.1f, .1f, .15f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        drawEnvironment(locTransform, locTint);

        if (currentScreen != MAIN_MENU)
            drawGameplay(locTransform, locTint);

        if (currentScreen != PLAYING)
            drawMenu(locTransform, locTint);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}