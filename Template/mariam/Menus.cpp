#include "Menus.h"
#include "UI.h"
#include "Template/Enemies.h"
#include "Template/Projectiles.h"
#include "Template/Collectibles.h"
#include "Template/Waves.h"
#include "../Audio.h"

int currentScreen = MAIN_MENU;
float gameTime =0.0f;
float cutsceneTime =0.0f;
float mouseX =-2.0f;
float mouseY =-2.0f;
float soundVolume = 1.0f;
bool soundMuted = false;

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
        hovered ? .63f : .44f
     );

     drawText(
        locTransform, locTint, label,
        0, y, .010f, .95f, .93f,.85f );}
         
        
void resetGame(){
    for (int i = 0; i< MAX_PLAYERS;i++)
    players[i] = Player{};

    coins =100;
     meleePurchased = 0;
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
    for(int i =0; i<MAX_ENEMIES; i++){ 
      enemies[i]=Enemy{};
    }
    for(int i=0; i<MAX_PROJECTILES ; i++){
      projectiles[i]= Projectile{};
    }
    for(int i=0; i<MAX_COLLECTIBLES; i++){
        collectibles[i]= Collectible{};
    }
    currentWave=1;
    spawnedInWave=0;
    spawnTimer=0.0f;
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
    playSound(won ? "audio/victory.wav" : "audio/defeat.wav");
    for (int i = 0; i< MAX_PLAYERS;i++)
    players[i].attacksThisFrame =0;

    refreshHover();
    updateWindowTitle(window);
 }

 void drawMenu(GLint locTransform,GLint locTint)
 {
    glm::mat4 identity = glm::mat4(1);

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

   drawButton(locTransform, locTint, "START",.02f);
drawButton(locTransform, locTint, "SOUND SETTINGS",-.20f);
drawButton(locTransform, locTint, "EXIT",-.40f);
}

else if (currentScreen == SOUND_SETTINGS) {

    drawText(
        locTransform, locTint, "SOUND SETTINGS", 0,.42f,.014f,    .95f, .80f, .40f);

    drawButton(locTransform, locTint, "VOLUME +",.20f);
   float displayedVolume = soundMuted ? 0.0f : soundVolume;

std::string volumeText = "VOLUME: " +
    std::to_string((int)(displayedVolume * 100.0f + 0.5f)) + "%";

drawText(
    locTransform, locTint,
    volumeText,
    0, .32f, .006f,
    .95f, .93f, .85f
);
    drawButton(locTransform, locTint, "VOLUME -",.01f);
drawButton( locTransform, locTint,  soundMuted ? "UNMUTE" : "MUTE", -.18f);


    drawButton(locTransform, locTint, "BACK",-.37f);}



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











        
