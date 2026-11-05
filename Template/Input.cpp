 #include "mariam/GameShared.h"
#include "Input.h"
#include "GameState.h"
#include "mariam/Menus.h"
#include "mariam/Cutscene.h"
#include "Audio.h"



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
        int cost = getPlayerCost(i);

        if(coins<0 ||coins < cost || (i != MELEE && meleePurchased <3))
        continue;
        if (mouseX >= cardX[i] - 0.15f && mouseX <= cardX[i] + 0.15f) {
            return i;
        }
    }



    return -1; 
}

void refreshHover() {
    hoveredLane =-1;
    hoveredCard =-1;

    if (currentScreen == PLAYING) {
        hoveredLane =getLane(mouseX, mouseY);
        int card = getCard(mouseX, mouseY);
        if (coins >= getPlayerCost(card))
            hoveredCard =getCard(mouseX, mouseY);
    }} 


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if (!getMousePosition(window, xpos, ypos, mouseX, mouseY)) {
        hoveredLane = -1;
        hoveredCard = -1;
        return;
    }
refreshHover();
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
    if (currentScreen == MAIN_MENU) {
   
    if (insideButton(mouseX, mouseY, 0.02f)) {
        beginCutscene(window);
    }

   
    else if (insideButton(mouseX, mouseY, -0.20f)) {
        currentScreen = SOUND_SETTINGS;
        refreshHover();
        updateWindowTitle(window);
    }

    
    else if (insideButton(mouseX, mouseY, -0.40f)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    return;
}

    if (currentScreen == SOUND_SETTINGS) {

    
    if (insideButton(mouseX, mouseY, 0.20f)) {
        soundVolume += 0.1f;

        if (soundVolume > 1.0f)
            soundVolume = 1.0f;
    }

   
    else if (insideButton(mouseX, mouseY, 0.01f)) {
        soundVolume -= 0.1f;

        if (soundVolume < 0.0f)
            soundVolume = 0.0f;
    }

   
    else if (insideButton(mouseX, mouseY, -0.18f)) {
        soundMuted = !soundMuted;
    }

  
    else if (insideButton(mouseX, mouseY, -0.37f)) {
        currentScreen = MAIN_MENU;
        refreshHover();
        updateWindowTitle(window);
    }

    setAudioVolume(soundVolume, soundMuted);

    return;}



if (currentScreen==PAUSED){
   //cont
    if(insideButton(mouseX,mouseY,0.15f)) {
        currentScreen =PLAYING;
        refreshHover();
        updateWindowTitle(window);}

    //res
    else if(insideButton(mouseX, mouseY,-0.07f)) {
        startGame(window);}
//main menu
    else if(insideButton(mouseX, mouseY,-0.29f)) {
        returnToMainMenu(window);}
     return;}

     if (currentScreen==WIN || currentScreen==LOSE){
     //res
    if (insideButton(mouseX,mouseY,0.02f)) {
        startGame(window);}
    //main menu
    else if (insideButton(mouseX,mouseY,-0.20f)){
        returnToMainMenu(window);}

    return;}


     
if (currentScreen !=PLAYING) {
    return;}
    
if (placingDefense) {
    int clickedLane = getLane(mouseX, mouseY);
    if (clickedLane != -1) {
        int previousLane = placementLane;
        placementLane = clickedLane;
        placePlayer(window, DEFENSE, mouseX);
        placementLane = previousLane;
        placingDefense = false;
        refreshHover();
        updateWindowTitle(window);
    }
    return;
}



    int card = getCard(mouseX, mouseY);
    if (card != -1){
        if (card == DEFENSE){
            if (coins >= getPlayerCost(DEFENSE)) {
                placingDefense = true;
                refreshHover();
                updateWindowTitle(window);
            }
        }
        else {
            placePlayer(window, card, -0.6f);
        }
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
        //ESC toggles between playing and pause
if (key ==GLFW_KEY_ESCAPE) {
    if (currentScreen ==PLAYING) {
        currentScreen =PAUSED;
    }
    else if (currentScreen ==PAUSED) {
        currentScreen =PLAYING;
    }

    refreshHover();
    updateWindowTitle(window);
    return;}

    if (currentScreen != PLAYING){
        return;
    }



    bool jumpKey = (key == GLFW_KEY_W  || key==GLFW_KEY_UP);
    bool duckKey = (key == GLFW_KEY_S || key ==GLFW_KEY_DOWN);

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