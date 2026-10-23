#include "UI.h"
#include <glm/gtc/matrix_transform.hpp>


bool digitSegments[10][7] = {
    {1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0},
    {1,1,0,1,1,0,1},
    {1,1,1,1,0,0,1},
    {0,1,1,0,0,1,1},
    {1,0,1,1,0,1,1},
    {1,0,1,1,1,1,1},
    {1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1} };


    ModelPart digitParts[] = {
        {0,.060f,.055f,.009f,0,1,1,1},
    {.030f,.030f,.009f,.050f,0,1,1,1},
    {.030f,-.030f,.009f,.050f,0,1,1,1},
    {0,-.060f,.055f,.009f,0,1,1,1},
    {-.030f,-.030f,.009f,.050f,0,1,1,1},
    {-.030f,.030f,.009f,.050f,0,1,1,1},
    {0,0,.055f,.009f,0,1,1,1} };

    ModelPart coinIconParts[] = {
       {0,0,.075f,.100f,0,.65f,.43f,.10f},
    {0,0,.057f,.082f,0,.95f,.73f,.22f},
    {0,0,.010f,.050f,0,.70f,.46f,.10f},
    {0,.014f,.030f,.009f,0,.70f,.46f,.10f},
    {0,-.014f,.030f,.009f,0,.70f,.46f,.10f} };
    
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

  void drawText(GLint locTransform, GLint locTint, const std ::string& text, float centerX, float centerY, float cell, float red, float green, float blue) {
    if (text.empty()) return;
    float textWidth = ((int)text.size() * 6 -1) * cell;
    float left = centerX - textWidth * .50f;
    glm::mat4 identity = glm::mat4(1);

    for (int character = 0; character < (int)text.size(); character++) {
        char letter = text[character];
        if (letter < 'A' || letter > 'Z' ) continue;

        const char* pattern = letterPatterns[letter - 'A'];

        for (int row= 0; row< 7 ; row++){
            for (int column = 0; column < 5; column++) {
                if (pattern[row * 5 + column] != '1') continue;

                     drawRectangle( 
                        locTransform, locTint, identity,
                        left + (character * 6 + column + .5f) * cell, centerY + (3 - row) * cell, cell * .88f, cell * .88f, red , green, blue );

            }
        }

    }
}

void drawGameplay(GLint locTransform, GLint locTint){
    glm::mat4 identity = glm::mat4(1);
    
    drawRectangle(locTransform, locTint, identity, 0,.80f,2, .30f, .20f, .23f, .30f);

    for (int type = 0; type < 3; type++) { 
        bool available = coins >= PLAYER_COST;
        bool  hovered = available && hoveredCard == type;

        drawRectangle(
            locTransform, locTint, identity,
            cardX[type], .80f, .30f, .24f,
            !available ? .35f : hovered ? .40f : .25f,
            !available ? .35f : hovered ? .60f : .40,
            !available ? .35f : hovered ? .85f : .65f 
        );

        drawCharacter(
            locTransform, locTint, type, cardX[type], .745f, .65f, .85f, 0, !available
        );

    

       
       
        const char* cardName[] = { "MELEE", "RANGED", "DEFENSE" };

        drawText(
            locTransform, locTint, cardName[type],
            cardX[type], .89f, .006f, 1.0f,1.0f ,1.0f
        ); 

        drawText(
            locTransform, locTint, "COINS",cardX[type]+ .035f, .705f, .004f,.95f, .85f, .45f );

            std::string priceText = std::to_string(PLAYER_COST);
            float priceScale =.22f;

            for(int position = 0; position < (int)priceText.size(); position++) {
                int digit = priceText[position] - '0';
            

                glm::mat4 priceParent = glm::translate(identity, glm::vec3(cardX[type] - .065f + position * .085f *priceScale, .705f, 0));

                priceParent = glm::scale(priceParent, glm::vec3(priceScale, priceScale, 1));

                for (int segment = 0; segment < 7; segment++) {
                    if (!digitSegments[digit][segment]) continue;

                    drawRectangle(
                        locTransform, locTint, priceParent,
                        digitParts[segment].x, digitParts[segment].y,
                        digitParts[segment].width, digitParts[segment].height,.95f, .85f, .45f );
                    }
                }

            }

            if (coins < PLAYER_COST) {
                drawText(
                    locTransform, locTint, "NOT ENOUGH COINS",
                    -.30f, .625f, .006f, 1.0f, .65f, .35f
                ); 

            }

            for (int i= 0; i< MAX_PLAYERS; i++) { 
                if (!players[i].active) continue;

                float feetY= laneY[players[i].lane] - .06f;

                if(players[i].jumping) {
                    
                float u =players[i].actionTime / JUMP_DURATION;
                    feetY += 4 * JUMP_HEIGHT * u * (1 -u);


                }

                float fraction = (float)players[i].health / players[i].maxHealth;

                drawCharacter(
                    locTransform, locTint,players[i].type, players[i].x, feetY, .40f + .60f* fraction, players[i].ducking ? .50f : 1.0f, players[i].attackTime / ATTACK_DURATION, false);

                }

                for (int i = 0; i< MAX_PLAYERS; i++) {
                    if (!players[i].active) continue;
                     
                    float feetY = laneY[players[i].lane] - .06f;
                    if(players[i].jumping){
                        float u = players[i].actionTime / JUMP_DURATION;
                        feetY += 4 * JUMP_HEIGHT * u * (1 -u);
                    }
                
                    float spacing = .019f;
                    float rowWidth = (players[i].maxHealth - 1) * spacing + .014f;
                    float firstX= players[i].x - (players[i].maxHealth - 1) * spacing * .5f;

                    float y = feetY - .026f;

                    drawRectangle(locTransform, locTint, identity, players[i].x, y, rowWidth + .012f, .028f, .10f, .12f, .14f);

                   for (int point = 0; point< players[i].maxHealth; point++){
                    bool filled = point < players[i].health;
                    drawRectangle(locTransform, locTint, identity, firstX + point * spacing, y, .014f, .016f, filled ? .30f : .32f, filled ? .90f : .34f, filled ? .35f : .36f);

                   }
                }

                drawRectangle(locTransform, locTint, identity, .63f, .80f, .55f,.22f,.12f,.15f,.20f);

                glm::mat4 icon =glm::translate(identity, glm::vec3(.43f,.80f,0)); 

                drawParts(locTransform, locTint, icon, coinIconParts, sizeof(coinIconParts)/sizeof(coinIconParts[0]));

                std::string coinText = std::to_string(coins);

                float scale = coinText.size() > 4 ? 4.0f / (float)coinText.size() : 1.0f;

                for (int position = 0; position < (int)coinText.size(); position++) {
                    int digit = coinText[position] - '0';

                    if(digit < 0 || digit > 9)continue;

                    glm::mat4 parent = glm::translate(identity, glm::vec3(.55f + position * .085f * scale, .80f, 0));
                     parent = glm::scale(parent, glm::vec3(scale, scale, 1));

                    for (int segment = 0; segment < 7; segment++) {
                        if (!digitSegments[digit][segment]) continue;

                        drawRectangle(
                            locTransform, locTint, parent,
                            digitParts[segment].x, digitParts[segment].y,
                            digitParts[segment].width, digitParts[segment].height, .95f, .85f, .45f);
                        }
                    }
                }
            
            
                    

                
                
                











        





            
