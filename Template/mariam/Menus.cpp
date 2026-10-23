#include "Menus.h"
#include "UI.h"

bool insideButton(float x, float y, float centerY){
    return x >= -.32f && x<= .32f && y >= centerY - .08f && y <= centerY+ .08f;

}

void drawButton(
    GLint locTransform,
    GLint locTint,
    const std::string& label, float y) {

    bool hovered = insideButton(mouseX, mouseY, y);
    glm::mat4 identity = glm::mat4(1);

    drawRectangle(
        locTransform, locTint, identity,
        0, y, .66f,.18f,.65f,.52f,.25f);


     drawRectangle(
        locTransform, locTint, identity,
        0, y, .64f, .16f,
        hovered ? .30f : .20f,
        hovered ? .43f : .29f,
        hovered ? .63f : .44f;
     );

     drawText(
        locTransform, locTint, label,
        0, y, .010f, .95f, .93f,.85f );}
         
        void updateWindowTitle(GLFWwindow* window){
            std::string title = "Castle Gate Defenders";
            if(currentScreen == MAIN_MENU)
            title += " | Main Menu";
            else if ( currentScreen == PAUSED)
            title +=" | Paused";

            else if (currentScreen == WIN)
             title += " | Victory";

             else if (currentScreen == LOSE)
             title += " | Defeat";

             else if (currentScreen == CUTSCENE)
             title += " | The Castle Gate";

             else 
             title += " | Coins: " + std::to_string(coins)
             + " | Test cost: 10 | W/S: dodge | H/B: test | F1/F2: outcome";

             glfwSetWindowTitle(window, title.c_str());
        }
void resetGame(){
    for (int i = 0; i< MAX_PLAYERS;i++)
    players[i] = Player{};

    coins = 30;
    gameTime=0;
    cutsceneTime = 0;
    selectionCounter=0;
    placementLane= -1;
    hoveredLane = -1;
    hoveredCard = -1;

    for (int i = 0; i < 3; i++){
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

 void finishGame(GLFWwindow* window,bool won)
 {
    if (currentScreen != PLAYING) return;

    currentScreen = won ? WIN : LOSE;
    for (int i = 0; i< MAX_PLAYERS;i++)
    players[i].attacksThisFrame =0;

    refreshHover();
    updateWindowTitle(window);
 }

 void drawMenu(GLint locTransform,GLint locTint)
 {
    glm::mat4 identity = glm:mat4(1);

    drawRectangle(
        locTransform, locTint, identity,0, .03f, 1.48f, 1.08f, .60f,.48f,.23f);

    drawRectangle(
        locTransform, locTint, identity,0, .03f, 1.44f, 1.04f, .10f,.14f,.21f);

if (currentScreen == MAIN_MENU) {
    drawText(
        locTransform,locTint, "CASTLE GATE",
        0,.37,.018f,.95f,.80f,.40f
    );

     drawText(
        locTransform,locTint, "DEFENDERS",
        0,.20,.014f,.90f,.92f,.96f
    );

    drawButton(locTransform, locTint ,"START", .02f);
    drawButton(locTransform, locTint ,"EXIT", -.20f);
}

else if (currentScreen == PAUSED){
    drawText(
        locTransform,locTint, "PAUSED",0,.39f, .018f, .95f, .80f, .40f);
         

        drawButton(locTransform, locTint, "CONTINUE", .15f);
        drawButton(locTransform, locTint, "RESTART", -.07f);
        drawButton(locTransform, locTint, "MAIN MENU", -.29f);
}

else if (currentScreen == WIN)
{
    drawText(
        locTransform,locTint, "VICTORY",0,.35f, .020f, .45f, .95f, .55f);
        drawText(
        locTransform,locTint, "THE GATE IS SAFE",0,.20f, .009f, .90f, .92f, .96f);
         

        drawButton(locTransform, locTint, "RESTART", .02f);
        drawButton(locTransform, locTint, "MAIN MENU", -.20f);
}

 else if (currentScreen == LOSE){

    
    drawText(
        locTransform,locTint, "DEFEAT",0,.35f, .020f, 1.0f, .40f, .35f);
        drawText(
        locTransform,locTint, "THE GATE HAS FALLEN",0,.20f, .009f, .90f, .92f, .96f);
         

        drawButton(locTransform, locTint, "RESTART", .02f);
        drawButton(locTransform, locTint, "MAIN MENU", -.20f);


        }
    }











        
