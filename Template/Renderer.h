#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

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


struct Renderer{
    GLuint shaderProgram;
    GLuint VAO;
    GLuint VBO;
    GLint locTransform;
    GLint locTint;
};



bool initRenderer(Renderer& renderer);
void drawScene(const Renderer& renderer);
void drawCharacter(const Renderer& renderer,int type,float x,float feetY,float damageScale,float heightScale, float attackProgress,bool grayscale);
void destroyRenderer(Renderer& renderer);
void drawRectangle(const Renderer& renderer, const glm::mat4& parent , float x, float y, float width, float height, float angle ,float red, float green, float blue);
void drawRectangle(GLint locTransform, GLint locTint, const glm::mat4& parent,float x, float y, float width, float height,float red, float green, float blue,float angle);