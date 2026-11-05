#include "UI.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <iostream>


   

    ModelPart coinIconParts[] = {
       {0,0,.075f,.100f,0,.65f,.43f,.10f},
    {0,0,.057f,.082f,0,.95f,.73f,.22f},
    {0,0,.010f,.050f,0,.70f,.46f,.10f},
    {0,.014f,.030f,.009f,0,.70f,.46f,.10f},
    {0,-.014f,.030f,.009f,0,.70f,.46f,.10f} };
    
    
    
struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;};

std::map<char, Character> Characters;

static unsigned int VAO =0;
static unsigned int VBO =0;
static unsigned int textProgram =0;
GLuint compileShader(GLenum type, const char* source);



bool initTextRenderer() {
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) {
        std::cout << "Could not initialize FreeType\n";
        return false;
    }

    FT_Face face;

    if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
        std::cout << "Could not load font\n";
        FT_Done_FreeType(ft);
        return false;
    }

    if (FT_Set_Pixel_Sizes(face, 0, 48)) {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return false;
    }

    GLint oldAlignment = 4;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldAlignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        unsigned int texture = 0;

        if (face->glyph->bitmap.width > 0 &&
            face->glyph->bitmap.rows > 0) {

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE
            );
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE
            );
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR
            );
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER,
                GL_LINEAR
            );
        }

        Character character = {
            texture,
            glm::ivec2(
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows
            ),
            glm::ivec2(
                face->glyph->bitmap_left,
                face->glyph->bitmap_top
            ),
            static_cast<unsigned int>(face->glyph->advance.x)
        };

        Characters.insert(
            std::pair<char, Character>(static_cast<char>(c), character)
        );
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, oldAlignment);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //shaders

    const char* textVertexSource = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
        out vec2 TexCoords;
        uniform mat4 projection;

        void main() {
            TexCoords = vertex.zw;
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        }
    )";

    const char* textFragmentSource = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;
        uniform sampler2D text;
        uniform vec3 textColor;

        void main() {
            float coverage = texture(text, TexCoords).r;
            color = vec4(textColor, coverage);
        }
    )";

    GLuint vertexShader = compileShader(
        GL_VERTEX_SHADER, textVertexSource
    );

    GLuint fragmentShader = compileShader(
        GL_FRAGMENT_SHADER, textFragmentSource
    );

    textProgram = glCreateProgram();
    glAttachShader(textProgram, vertexShader);
    glAttachShader(textProgram, fragmentShader);
    glLinkProgram(textProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linked =0;
    glGetProgramiv(textProgram, GL_LINK_STATUS, &linked);

    if (!linked) {
        char log[512] = {};
        glGetProgramInfoLog(textProgram, 512, nullptr, log);
        std::cout << "Text shader linking failed: " << log << '\n';
        destroyTextRenderer();
        return false;
    }

    glUseProgram(textProgram);

    glm::mat4 projection = glm::ortho(
        0.0f,(float)g_screenWidth,
        0.0f,(float)g_screenHeight
    );

    glUniformMatrix4fv(
        glGetUniformLocation(textProgram, "projection"),
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );

    glUniform1i(
        glGetUniformLocation(textProgram, "text"), 0
    );

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * 6 * 4,
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,
        4,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        0 );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    return true;
}




void RenderText(
    std::string text,
    float x,
    float y,
    float scale,
    glm::vec3 color
) {
    if (!textProgram) return;

    GLint previousProgram = 0;
    GLint previousVAO = 0;
    GLint previousVBO = 0;
    GLint previousActiveTexture = 0;
    GLint previousTexture = 0;

    glGetIntegerv(GL_CURRENT_PROGRAM, &previousProgram);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previousVAO);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &previousVBO);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &previousActiveTexture);

    glActiveTexture(GL_TEXTURE0);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);

    glUseProgram(textProgram);

    glUniform3f(
        glGetUniformLocation(textProgram, "textColor"),
        color.x, color.y, color.z
    );

    glBindVertexArray(VAO);

    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++) {
        auto it = Characters.find(*c);
        if (it == Characters.end()) continue;

        Character ch = it->second;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        if (ch.TextureID != 0) {
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                sizeof(vertices),
                vertices
            );
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(previousVAO);
    glBindBuffer(GL_ARRAY_BUFFER, previousVBO);
    glBindTexture(GL_TEXTURE_2D, previousTexture);
    glActiveTexture(previousActiveTexture);
    glUseProgram(previousProgram);
}



void drawText(
    GLint locTransform,
    GLint locTint,
    const std::string& text,
    float centerX, float centerY, float cell,
    float red, float green, float blue
) {
    if (text.empty() || Characters.empty()) return;

    auto capital = Characters.find('H');
    if (capital == Characters.end() ||
        capital->second.Size.y <= 0) return;

    float pixelX = (centerX + 1.0f) * g_screenWidth / 2.0f;
    float pixelY = (centerY + 1.0f) * g_screenHeight / 2.0f;

    float desiredHeight = 7.0f * cell * g_screenHeight / 2.0f;
    float scale = desiredHeight / capital->second.Size.y;

    float textWidth = 0.0f;

    for (char c : text) {
        auto it = Characters.find(c);
        if (it != Characters.end()) {
            textWidth += (it->second.Advance >> 6) * scale;
        }
    }

    RenderText(
        text,
        pixelX - textWidth / 2.0f,
        pixelY - desiredHeight / 2.0f,
        scale,
        glm::vec3(red, green, blue)
    );
}




void destroyTextRenderer() {
    for (auto& entry : Characters) {
        if (entry.second.TextureID != 0) {
            glDeleteTextures(1, &entry.second.TextureID);
        }
    }

    Characters.clear();

    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (textProgram) glDeleteProgram(textProgram);

    VBO = 0;
    VAO = 0;
    textProgram = 0;
}





 
void drawGameplay(GLint locTransform, GLint locTint){
    glm::mat4 identity = glm::mat4(1);
    
    drawRectangle(locTransform, locTint, identity, 0,.80f,2, .30f, .20f, .23f, .30f);

    for (int type = 0; type < 3; type++) { 
        bool available = coins >= PLAYER_COST && (type == MELEE || meleePurchased >=3);
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

            std::string priceText = std::to_string(getPlayerCost(type));
            drawText(
    locTransform, locTint,
    priceText,
    cardX[type] - .055f, .705f, .004f,
    .95f, .85f, .45f
                      );}

            if (coins < PLAYER_COST) {
                drawText(locTransform, locTint, "NOT ENOUGH COINS",-.30f, .625f, .006f, 1.0f, .65f, .35f ); 

            }

            for (int i= 0; i< MAX_PLAYERS; i++) { 
                if (!players[i].active) continue;

                float feetY= laneY[players[i].lane] - .06f;

                if(players[i].jumping) {
                    
                float u =players[i].actionTime / JUMP_DURATION;
                    feetY += 4 * JUMP_HEIGHT * u * (1 -u);


                }

                float fraction = (float)players[i].health / players[i].maxHealth;

                drawCharacter( locTransform, locTint,players[i].type, players[i].x, feetY, .40f + .60f* fraction, players[i].ducking ? .50f : 1.0f, players[i].attackTime / ATTACK_DURATION, false);

                }

                for (int i =0; i< MAX_PLAYERS; i++) {
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
                drawText(locTransform, locTint, coinText, .63f, .80f, .014f, .95f, .85f, .45f);

                drawText( locTransform, locTint, coinText,.63f,.80f,.014f,.95f,.85f,.45f);

}
                