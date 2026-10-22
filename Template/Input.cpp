#include "Input.h"
#include "GameState.h"



// Function to get the mouse position in normalized device coordinates
bool getMousePosition(GLFWwindow* window, double xpos, double ypos, float& mouseX, float& mouseY) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if (width <= 0 || height <= 0) {
        return false;
    }

    mouseX = 2.0f * (xpos / width) - 1.0f;
    mouseY = 1.0f - 2.0f * (ypos / height);
    return true;

}

int getLane(float mouseX, float mouseY) {

    if (mouseX < -0.70f || mouseX > 0.90f) {
        return -1; 
    }

    if (mouseY >= -0.75f && mouseY <= -0.35f) {
        return 0;
    }

        if (mouseY >= -0.30f && mouseY <= 0.10f) {
        return 1;
    }

        if (mouseY >= 0.15f && mouseY <= 0.55f) {
        return 2;
    }

    return -1;

}


//determines which card is being hovered over based on the mouse position
int getCard(float mouseX, float mouseY) {
    if (mouseY < 0.68f || mouseY > 0.92f) {
        return -1; 
    }

    for (int i = 0; i < 3; ++i) {
        if (mouseX >= cardX[i] - 0.15f && mouseX <= cardX[i] + 0.15f) {
            return i;
        }
    }



    return -1; 
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    float mouseX, mouseY;
    if (!getMousePosition(window, xpos, ypos, mouseX, mouseY)) {
        hoveredLane = -1;
        hoveredCard = -1;
        return;
    }

    hoveredLane = getLane(mouseX, mouseY);
    hoveredCard = coins >= PLAYER_COST ? getCard(mouseX, mouseY) : -1;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float mouseX, mouseY; 
    
    if (!getMousePosition(window, xpos, ypos, mouseX, mouseY)) {
        return;
    }

    int card = getCard(mouseX, mouseY);
    if (card != -1){
        placePlayer(window, card);
        return;
    }

    int lane = getLane(mouseX, mouseY);
    if (lane != -1) {
        selectedLanes[lane] = !selectedLanes[lane];
        if (selectedLanes[lane]){
            selectionCounter++;
            selectionOrder[lane] = selectionCounter;

        }

        updatePlacementLane();
    }
    
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;
    

    // temp for testing 
    if (key == GLFW_KEY_H || key == GLFW_KEY_B) {
        for (int i = 0; i < MAX_PLAYERS; i++) {

            if (!players[i].active ||
                !selectedLanes[players[i].lane])
            {
                continue;
            }

            if (key == GLFW_KEY_H) {
                damagePlayer(i, 1);
            }
            else if (players[i].type != DEFENSE) {

                setPlayerMovementBlocked(
                    i,
                    !players[i].movementBlocked
                );
            }
        }

        return;
    } 

    bool jumpKey = (key == GLFW_KEY_W );
    bool duckKey = (key == GLFW_KEY_S);

    if (!jumpKey && !duckKey)
        return;
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].active ||
            players[i].type == DEFENSE)
        {
            continue;
        }

        if (!selectedLanes[players[i].lane])
            continue;
        if (players[i].jumping || players[i].ducking)
            continue;

        players[i].actionTime = 0.0f;
        players[i].jumping = jumpKey;
        players[i].ducking = duckKey;


    }


    
}