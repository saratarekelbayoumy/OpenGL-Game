#include "Cutscene.h"
#include "Environment.h"
#include "UI.h"
#include "Menus.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>


ModelPart cutsceneRaiderParts[] = {
    {-.030f,.013f,.043f,.026f,0,.20f,.16f,.12f},
    { .030f,.013f,.043f,.026f,0,.20f,.16f,.12f},
    {-.027f,.044f,.029f,.045f,0,.37f,.24f,.17f},
    { .027f,.044f,.029f,.045f,0,.37f,.24f,.17f},

    {0,.094f,.095f,.083f,0,.46f,.25f,.17f},
    {0,.106f,.075f,.045f,0,.34f,.33f,.32f},
    {-.025f,.110f,.008f,.008f,0,.75f,.72f,.61f},
    { .025f,.110f,.008f,.008f,0,.75f,.72f,.61f},

    {-.051f,.126f,.030f,.029f,-15,.38f,.36f,.33f},
    { .051f,.126f,.030f,.029f,15,.38f,.36f,.33f},
    {0,.066f,.100f,.014f,0,.18f,.14f,.12f},
    {0,.066f,.019f,.014f,0,.71f,.56f,.25f},
    {0,.159f,.066f,.054f,0,.66f,.49f,.34f},

    {0,.183f,.078f,.022f,0,.32f,.31f,.30f},
    {-.039f,.195f,.016f,.036f,-30,.83f,.77f,.61f},
    { .039f,.195f,.016f,.036f,30,.83f,.77f,.61f},
    {.017f,.165f,.017f,.006f,0,.10f,.09f,.08f},
    {.018f,.145f,.025f,.006f,0,.20f,.12f,.09f} };

    void beginCutscene(GLFWwindow* window) {

        resetGame();
        currentScreen = CUTSCENE;
        refreshHover();
        updateWindowTitle(window); }



        void updateCutscene(GLFWwindow* window, float dt) {
        cutsceneTime += dt;

        if (cutsceneTime >= CUTSCENE_DURATION) {
            cutsceneTime = CUTSCENE_DURATION;
            currentScreen = PLAYING;
            gameTime = 0;
            refreshHover();
            updateWindowTitle(window); }

        }

        void drawCutsceneRaider(GLint locTransform, GLint locTint, float x, float feetY, float axeAngle) {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(x, feetY, 0));
            model = glm::scale(model, glm::vec3(-1.8f,1.8f,1));

          drawParts(locTransform, locTint, model, cutsceneRaiderParts, sizeof(cutsceneRaiderParts)/sizeof(cutsceneRaiderParts[0]));
        drawRectangle(locTransform, locTint, model, .047f,.098f,.047f,.026f,.46f,.25f,.17f);
        glm::mat4 axe = glm::translate(model, glm::vec3(.07f,.10f,0));
        axe = glm::rotate(axe, glm::radians(axeAngle), glm::vec3(0,0,1));
          drawRectangle(locTransform, locTint,axe,0,.028f,.012f,.110f,.47f,.29f,.14f);
          drawRectangle(locTransform, locTint,axe,.020f,.077f,.062f,.037f,.69f,.69f,.66f,-15);
          drawRectangle(locTransform, locTint,axe,.042f,.083f,.016f,.044f,.88f,.86f,.79f,-15);}

          void drawCutscene(GLint locTransform, GLint locTint) {
              glm::mat4 identity = glm::mat4(1);

              float t = cutsceneTime;
              drawRectangle(locTransform, locTint, identity, 0,0,2,2,.16f,.23f,.33f);
              drawRectangle(locTransform, locTint, identity, 0,-.66f,2,.68f,.32f,.36f,.29f);
              drawRectangle(locTransform, locTint, identity, 0,-.43f,2,.025f,.55f,.56f,.49f);


               // bottom path  
                      drawRectangle(locTransform, locTint, identity,.10f, -.72f, 1.60f,.56f,.46f,.49f,.55f);

        for (int row =0; row <7; row++) {
            for (int column =0; column < 12; column++) {
                float left = -.70f + column * .14f;

                if (row == 1)
                left -= .07f;

                float right = left + .13f;

                if (left < -.70f) left = -.70f;
                if (right > .90f) right = .90f;
                if (right <=left) continue;

                bool light = (row + column) % 2 != 0;

                drawRectangle(locTransform, locTint, identity, (left + right) * .5f, -.95f + row * .083f, 
                right - left, .075f,
                light? .51f : .46f,
                light ? .54f : .49f,
                light? .60f : .55f);
        }
    }

    for (int cap = 0; cap < 10; cap++) {
        drawRectangle(locTransform,locTint,identity,-.62f + cap * .16f, -.455f, .153f, .026f, .66f, .69f ,.74f);

    }

        //decoration
          for (int decoration = 0; decoration <9; decoration++) {
        ModelPart* parts;
        int count;
        float x;

        if (decoration <2) {
            parts = bannerParts;
            count = sizeof(bannerParts) / sizeof(bannerParts[0]);   
        x = decoration == 0 ? -.55f : .62f;
        }

        else if (decoration < 4) {
            parts = torchParts;
            count = sizeof(torchParts) / sizeof(torchParts[0]);
            x = decoration == 2 ? -.68f : .85f; }
        else if (decoration < 6) {
            parts = crateParts;
            count = sizeof(crateParts) / sizeof(crateParts[0]);
            x = decoration == 4 ? -.30f : .20f;
        }
        else {
            parts = bushParts;
            count = sizeof(bushParts) / sizeof(bushParts[0]);
            x = decoration == 6 ? -.10f : decoration == 7 ? .43f : .78f;
        }


             float decorationY=(decoration<4) ? -.69f : -.88f;

       glm::mat4 parent= glm::translate(identity,glm::vec3(x,decorationY,0));

        drawParts(locTransform, locTint, parent, parts, count);

        if (decoration == 2 || decoration == 3) {

            float phase = cutsceneTime * 7 + decoration;
            float height = .050f + .010f * (float)std::sin(phase);
            float width = .028f + .004f * (float)std::sin(phase +1);
            float lean = 8 * (float)std::sin(phase * .70f);

            glm::mat4 flame = glm::translate(parent, glm::vec3(.025f, .145f,0));

            flame = glm ::rotate (flame, glm:: radians(lean) ,glm::vec3(0,0,1));

            drawRectangle(locTransform, locTint, flame, 0,height * .5f, width, height, 1,.38f,.08f);

            drawRectangle(locTransform, locTint, flame, 0,height * .30f, width * .50f, height * .60f, 1,.80f,.20f); 
        }
    }

                  






              glm::mat4 castle = glm::translate(identity, glm::vec3(-.83f, -.05f,0));
              castle = glm::scale(castle, glm::vec3(1.2f,.57f,1));

              drawParts(locTransform, locTint, castle, castleParts, sizeof(castleParts)/sizeof(castleParts[0]));

              for (int tower = 0; tower < 2; tower++){
                for (int row = 0; row < 9; row++) {
                    drawRectangle(locTransform, locTint, castle,
                        tower == 0 ? -.068f : .068f,
                        -0.57f + row * .14f,.054f, .005f, .40f, .43f, .50f );
                    }
                }

                

                float knightX = -.50f;
                float knightY = -.43f;
                float knightAttack =0;
                float raiderX = .98f;
                float raiderY = -.43f;
                float axeAngle = -20;

                if (t < 4) {
                    knightX = -.50f + .10f * (float)std::sin(t * PI / 4);
                    knightY += .006f * (float)std::sin(t * 8);

                }

                else if (t<8) {

                    float u = (t-4) / 4;

                    raiderX = .98f - .68f * u;
                    raiderY += .008f *(float)std::sin(u *24);
                    axeAngle = -20 - 45 * u;


                }
            else if (t<12) {
                float u = (t - 8) / 4;
                knightX = -.50f + .30f * u;
                knightAttack = .50f *u;
                raiderX = .30f;
                axeAngle = -65 + 8 * (float)std::sin(u * PI * 2);
            }

            else {
                float u = (t - 12) / 4;
                knightX = -.20f;
                knightAttack = .50f;
                raiderX = .30f + .72f*u;
                raiderY += .008f * (float)std::sin(u * 24);
                axeAngle = -65 + 45 *u;
                float archerX = -.66f + .27f * u;
                float guardX = -.72f + .16f * u;
                float bob = .004f * (float)std::sin(u * 24);
                
                drawCharacter(locTransform, locTint, RANGED,archerX, -.43f + bob , 1.8f,1,0,false);
                drawCharacter(locTransform, locTint, DEFENSE, guardX, -.43f , 1.8f ,1,0,false);
            }

            drawCharacter(locTransform, locTint, MELEE, knightX, knightY, 1.8f, 1, knightAttack, false);

            if (t >= 4)
                drawCutsceneRaider(locTransform, locTint, raiderX, raiderY, axeAngle);

                drawRectangle(locTransform, locTint, identity, 0, .69f, 1.90f, .44f, .09f,.13f,.19f);
                if(t<4){
                    drawText(locTransform, locTint, "OUR HOME BEHIND THESE GATES", 0, .77f,.010f,.95f,.84f,.55f);
                     drawText(locTransform, locTint, "WE KEEP THE CASTLE SAFE",0,.60f,.010f,.92f,.94f,.97f); }

                     else if (t<8) {
                        drawText(locTransform, locTint, "RAIDER",0,.77F,.014f,1,.55f,.40f);
                        drawText(locTransform, locTint, "OPEN THE GATE OR FACE OUR AXES",0,.60f,.009f,.92f,.94f,.97f);}

                          else if (t<12) {
                            drawText(locTransform, locTint, "KNIGHT",0,.77F,.014F,.55F,.78F,1);
                            drawText(locTransform, locTint, "YOU WILL NOT TAKE OUR HOME",0,.60f,.010f,.92f,.94f,.97f);}

                            else { 
                                drawText(locTransform, locTint, "DEFENDERS TO THE GATE",0,.77f,.012f,.95f,.84f,.55f);
                                drawText(locTransform, locTint, "HOLD THE LANES AND PROTECT THE CASTLE",0,.60f,.008f,.92f,.94f,.97f);
                            }
                            }