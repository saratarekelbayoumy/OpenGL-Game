#include "Renderer.h"
#include "GameState.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

struct ModelPart{
    float x;
    float y;
    float width;
    float height;
    float angle;
    float red;
    float green;
    float blue;
};


ModelPart meleeParts[] = {
    // Legs
    {-0.023f, 0.036f, 0.026f, 0.050f, 0, 0.40f, 0.45f, 0.55f},
    { 0.023f, 0.036f, 0.026f, 0.050f, 0, 0.40f, 0.45f, 0.55f},

    // Boots
    {-0.023f, 0.011f, 0.038f, 0.022f, 0, 0.18f, 0.20f, 0.25f},
    { 0.026f, 0.011f, 0.040f, 0.022f, 0, 0.18f, 0.20f, 0.25f},

    // Tunic and armor
    {0, 0.083f, 0.080f, 0.086f, 0, 0.25f, 0.45f, 0.75f},
    {0, 0.097f, 0.070f, 0.058f, 0, 0.65f, 0.70f, 0.80f},
    {0, 0.097f, 0.024f, 0.052f, 0, 0.80f, 0.83f, 0.90f},

    // Arm and shoulders
    {-0.048f, 0.085f, 0.020f, 0.056f, 0, 0.40f, 0.45f, 0.55f},
    {-0.043f, 0.119f, 0.032f, 0.024f, 0, 0.72f, 0.76f, 0.84f},
    { 0.043f, 0.119f, 0.032f, 0.024f, 0, 0.72f, 0.76f, 0.84f},

    // Belt
    {0, 0.062f, 0.082f, 0.014f, 0, 0.25f, 0.18f, 0.12f},
    {0, 0.062f, 0.016f, 0.014f, 0, 0.85f, 0.65f, 0.25f},

    // Neck, helmet and visor
    {0, 0.132f, 0.026f, 0.024f, 0, 0.85f, 0.70f, 0.52f},
    {0, 0.157f, 0.064f, 0.058f, 0, 0.68f, 0.73f, 0.82f},
    {0.009f, 0.154f, 0.054f, 0.029f, 0, 0.42f, 0.47f, 0.56f},
    {0.013f, 0.159f, 0.045f, 0.006f, 0, 0.10f, 0.12f, 0.16f},
    {-0.010f, 0.187f, 0.026f, 0.025f, 0, 0.65f, 0.20f, 0.20f}
};


// Ranged character model
ModelPart rangedParts[] = {
    // Quiver and arrows
    {-0.054f, 0.101f, 0.027f, 0.072f, -15, 0.45f, 0.27f, 0.13f},
    {-0.065f, 0.147f, 0.004f, 0.049f, -15, 0.82f, 0.72f, 0.50f},
    {-0.054f, 0.151f, 0.004f, 0.054f, -15, 0.82f, 0.72f, 0.50f},
    {-0.065f, 0.169f, 0.014f, 0.012f, -15, 0.80f, 0.80f, 0.75f},
    {-0.054f, 0.175f, 0.014f, 0.012f, -15, 0.80f, 0.80f, 0.75f},

    // Legs and boots
    {-0.022f, 0.036f, 0.023f, 0.050f, 0, 0.30f, 0.28f, 0.18f},
    { 0.022f, 0.036f, 0.023f, 0.050f, 0, 0.30f, 0.28f, 0.18f},
    {-0.023f, 0.011f, 0.034f, 0.022f, 0, 0.25f, 0.16f, 0.10f},
    { 0.025f, 0.011f, 0.038f, 0.022f, 0, 0.25f, 0.16f, 0.10f},

    // Tunic, arm and belt
    {0, 0.086f, 0.074f, 0.080f, 0, 0.25f, 0.55f, 0.30f},
    {0, 0.049f, 0.086f, 0.022f, 0, 0.20f, 0.43f, 0.25f},
    {-0.044f, 0.090f, 0.018f, 0.052f, 0, 0.25f, 0.55f, 0.30f},
    {0, 0.069f, 0.077f, 0.012f, 0, 0.35f, 0.22f, 0.12f},
    {0.005f, 0.069f, 0.013f, 0.012f, 0, 0.75f, 0.60f, 0.25f},

    // Hood and face
    {-0.003f, 0.155f, 0.070f, 0.066f, 0, 0.18f, 0.38f, 0.22f},
    {0.009f, 0.151f, 0.044f, 0.043f, 0, 0.90f, 0.75f, 0.55f},
    {0.005f, 0.174f, 0.060f, 0.014f, 0, 0.23f, 0.47f, 0.27f},
    {0.024f, 0.155f, 0.007f, 0.005f, 0, 0.12f, 0.12f, 0.10f}
};


// Defense character model
ModelPart defenseParts[] = {
    // Legs and boots
    {-0.028f, 0.036f, 0.033f, 0.052f, 0, 0.42f, 0.45f, 0.53f},
    { 0.028f, 0.036f, 0.033f, 0.052f, 0, 0.42f, 0.45f, 0.53f},
    {-0.029f, 0.012f, 0.045f, 0.024f, 0, 0.22f, 0.24f, 0.30f},
    { 0.032f, 0.012f, 0.048f, 0.024f, 0, 0.22f, 0.24f, 0.30f},

    // Torso and arms
    {0, 0.088f, 0.110f, 0.090f, 0, 0.58f, 0.62f, 0.72f},
    {0, 0.098f, 0.082f, 0.058f, 0, 0.72f, 0.75f, 0.83f},
    {-0.061f, 0.085f, 0.024f, 0.059f, 0, 0.42f, 0.46f, 0.55f},
    { 0.061f, 0.085f, 0.024f, 0.059f, 0, 0.42f, 0.46f, 0.55f},

    // Shoulders and belt
    {-0.055f, 0.124f, 0.046f, 0.030f, 0, 0.80f, 0.82f, 0.88f},
    { 0.055f, 0.124f, 0.046f, 0.030f, 0, 0.80f, 0.82f, 0.88f},
    {0, 0.062f, 0.110f, 0.016f, 0, 0.27f, 0.24f, 0.20f},
    {-0.005f, 0.062f, 0.020f, 0.016f, 0, 0.80f, 0.65f, 0.28f},

    // Helmet
    {0, 0.159f, 0.078f, 0.062f, 0, 0.62f, 0.67f, 0.77f},
    {0, 0.184f, 0.086f, 0.015f, 0, 0.80f, 0.83f, 0.90f},
    {0.010f, 0.157f, 0.056f, 0.010f, 0, 0.12f, 0.14f, 0.20f},
    {-0.006f, 0.157f, 0.012f, 0.055f, 0, 0.80f, 0.83f, 0.90f}
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
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error ("
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "):\n" << infoLog << std::endl;
    }
    return shader;
}

void drawRectangle(const Renderer& renderer, const glm:: mat4& parent, float x, float y, float width, float height, float angle, float red, float green, float blue) {
    glm::mat4 transform = glm::translate(parent, glm::vec3(x, y, 0.0f));
    transform = glm::rotate(transform, glm::radians(angle) , glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(width, height, 1.0f));

    glUniformMatrix4fv(renderer.locTransform, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4f(renderer.locTint, red, green, blue, 1.0f);

    glBindVertexArray(renderer.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


bool initRenderer(Renderer& renderer) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    renderer.shaderProgram = glCreateProgram();
    glAttachShader(renderer.shaderProgram, vertexShader);
    glAttachShader(renderer.shaderProgram, fragmentShader);
    glLinkProgram(renderer.shaderProgram);

    int success;
    glGetProgramiv(renderer.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(renderer.shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program link error:\n" << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
                 
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,

         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &renderer.VAO);
    glGenBuffers(1, &renderer.VBO);

    glBindVertexArray(renderer.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    renderer.locTransform = glGetUniformLocation(renderer.shaderProgram, "u_Transform");
    renderer.locTint = glGetUniformLocation(renderer.shaderProgram, "u_Tint");
    return true;
}

void drawLanes(const Renderer& renderer){
    glm::mat4 identity(1.0f);
    for(int lane = 0; lane < 3; lane++){
        float red, green, blue;
        if(selectedLanes[lane]) {
            red = 0.55f;
            green = 0.65f;
            blue = 0.35f;
        } else if (hoveredLane == lane) {
            red = 0.55f;
            green = 0.59f;
            blue = 0.55f;
        }
        else if (lane == 1) {
            red = 0.43f;
            green = 0.47f;
            blue = 0.43f;
        } else {
            red = 0.36f;
            green = 0.40f;
            blue = 0.36f;
        }

        drawRectangle(renderer, identity, 0.10f, laneY[lane], 1.6f, 0.4f, 0.0f, red, green, blue);
    }
//homeBase
    drawRectangle(renderer, identity, -0.83f, -0.10f, 0.20f, 1.30f, 0.0f, 0.35f, 0.45f, 0.65f);

}

void drawCards(const Renderer& renderer){
    glm::mat4 identity(1.0f);
    drawRectangle(renderer, identity, 0.0f, 0.8f, 2.0f, 0.3f, 0.0f, 0.20f, 0.23f, 0.30f);
    for(int type = 0; type < 3; type++){
        float red, green, blue;

        if(coins < PLAYER_COST) {
            red = 0.35f;
            green = 0.35f;
            blue = 0.35f;
        } else if (hoveredCard == type) {
            red = 0.40f;
            green = 0.60f;
            blue = 0.85f;
        } else {
            red = 0.25f;
            green = 0.40f;
            blue = 0.65f;
        }
        

        drawRectangle(renderer, identity, cardX[type], 0.8f, 0.3f, 0.24f, 0.0f, red, green, blue);
    }
}



void drawCharacter(const Renderer& renderer, int type, float x,float feetY, float damageScale, float heightScale, float attackProgress, bool grayscale) {
    glm::mat4 modelTransform(1.0f);
    modelTransform = glm::translate(modelTransform, glm::vec3(x, feetY, 0.0f));
    modelTransform = glm::scale(modelTransform, glm::vec3(damageScale, damageScale * heightScale, 1.0f));

    ModelPart* parts;
    int partCount;

    if(type == MELEE) {
        parts = meleeParts;
        partCount = sizeof(meleeParts) / sizeof(meleeParts[0]);
    } else if (type == RANGED) {
        parts = rangedParts;
        partCount = sizeof(rangedParts) / sizeof(rangedParts[0]);
    } else {
        parts = defenseParts;
        partCount = sizeof(defenseParts) / sizeof(defenseParts[0]);
    } 

    for(int part = 0; part < partCount; part++) {
        float red = parts[part].red;
        float green = parts[part].green;
        float blue = parts[part].blue;

        if (grayscale) {
            float gray = (red + green + blue) / 3.0f;
            red = gray;
            green = gray;
            blue = gray;
        }

        drawRectangle(renderer, modelTransform, parts[part].x, parts[part].y, parts[part].width, parts[part].height, parts[part].angle, red, green, blue);
    }


    if (type == MELEE){

        float angle = -70.0f *(float)sin(attackProgress *PI);

        drawRectangle(renderer, modelTransform, 0.04f, 0.085f, 0.045f, 0.025f, 0, 0.60f, 0.6f, 0.6f);

        glm::mat4 weaponTransform = glm::translate(modelTransform, glm::vec3(0.065f, 0.085f, 0.0f));

        weaponTransform = glm::rotate(weaponTransform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        drawRectangle(renderer, weaponTransform, 0.0f, 0.055f, 0.015f, 0.090f, 0.0f, 0.85f, 0.85f, 0.85f);
        if (grayscale) {
            drawRectangle(renderer, weaponTransform, 0.0f, 0.01f, 0.045f, 0.012f, 0.0f, 0.50f, 0.50f, 0.50f);
        } else {
            drawRectangle(renderer, weaponTransform, 0.0f, 0.01f, 0.045f, 0.012f, 0.0f, 0.75f, 0.55f, 0.20f);
        }

        if (grayscale) {
            drawRectangle(renderer, weaponTransform, 0.0f, -0.01f, 0.012f, 0.03f, 0.0f, 0.35f, 0.35f, 0.35f);
        } else {
            drawRectangle(renderer, weaponTransform, 0.0f, -0.01f, 0.012, 0.03f, 0.0f, 0.4f, 0.25f, 0.15f);
        }
    }

    else if(type == RANGED){
        float recoil = 0.015f * (float) std:: sin(attackProgress * PI);
        if(grayscale){
            drawRectangle(renderer, modelTransform, 0.04f - recoil, 0.10f, 0.045f, 0.025f, 0, 0.55f,0.55f, 0.55f);
        }
        else{
            drawRectangle(renderer, modelTransform, 0.04f - recoil, 0.10f, 0.045f, 0.025f, 0, 0.25f,0.55f, 0.30f);
        }

        for(int part = 0; part < 2; part++){
            float y = part == 0 ? 0.125f : 0.075;
            float angle = part == 0 ? 25.0f : -25.0f;
            float red = grayscale ? 0.5f : 0.65f;
            float green = grayscale ? 0.5f : 0.40f;
            float blue = grayscale ? 0.5f : 0.20f;
        

        drawRectangle(renderer, modelTransform, 0.07f - recoil, y, 0.012f, 0.065f, angle, red, green, blue);
    }

    drawRectangle(renderer, modelTransform, 0.057f - recoil, 0.10f, 0.004f, 0.11f, 0, 0.85f, 0.85f, 0.85f);

}

else{
    drawRectangle(renderer, modelTransform, 0.045f, 0.085f, 0.08f, 0.13f, 0, 0.85f, 0.85f, 0.85f);
    if (grayscale){
        drawRectangle(renderer, modelTransform, 0.045f, 0.085f, 0.06f, 0.11f, 0, 0.45f, 0.45f, 0.45f);
        
    }else{
        drawRectangle(renderer, modelTransform, 0.045f, 0.085f, 0.06f, 0.11f, 0, 0.35f, 0.40f, 0.65f);
    }

    float red = grayscale ? 0.75f : 0.85f;
    float green = grayscale ? 0.75f : 0.70f;
    float blue = grayscale ? 0.75f : 0.30f;

    drawRectangle(renderer, modelTransform,0.045f, 0.085f, 0.010f, 0.065f,0, red, green, blue);
    drawRectangle(renderer, modelTransform,0.045f, 0.092f, 0.038f, 0.010f,0, red, green, blue);
    


}
 
}


void drawPlayers(const Renderer& renderer) {
    for (int object = 0; object < MAX_PLAYERS +3 ; object++){
        int type;
        float x;
        float feetY;
        float heightScale = 1.0f;
        float damageScale = 1.0f;
        float attackProgress = 0.0f;
        bool grayscale = false;

        if(object < 3){
            type = object;
            x = cardX[type];
            feetY = 0.71f;

            grayscale = coins < PLAYER_COST;
        }

        else{
            int i = object - 3;
            if(!players[i].active)
              continue;
            
             type = players[i].type;
             x =players[i].x;
             feetY = laneY[players[i].lane] - 0.06f;

             float healthFraction = (float)players[i].health / players[i].maxHealth;
             damageScale = 0.4f + 0.60f * healthFraction;

             attackProgress = players[i].attackTime/ ATTACK_DURATION;

             if(players[i].jumping){
                float progress = players[i].actionTime/JUMP_DURATION;
                feetY += 4 * JUMP_HEIGHT * progress * (1- progress);
                
             }

             if(players[i].ducking){
                heightScale = 0.5f;
             }
        }

        drawCharacter(renderer, type, x, feetY, damageScale, heightScale, attackProgress, grayscale);

    }

}

void drawScene(const Renderer &renderer){
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderer.shaderProgram);
    glBindVertexArray(renderer.VAO);
    drawLanes(renderer);
    drawCards(renderer);
    drawPlayers(renderer);
}


void destroyRenderer(Renderer& renderer) {
    glDeleteVertexArrays(1, &renderer.VAO);
    glDeleteBuffers(1, &renderer.VBO);
    glDeleteProgram(renderer.shaderProgram);
}








