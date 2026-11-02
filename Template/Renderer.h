#include <glad/glad.h>

struct Renderer{
    GLuint shaderProgram;
    GLuint VAO;
    GLuint VBO;
    GLint locTransform;
    GLint locTint;
};

bool initRenderer(Renderer& renderer);
void drawScene(const Renderer& renderer);
void destroyRenderer(Renderer& renderer);
void drawRectangle(const Renderer& renderer, const glm::mat4& parent , float x, float y, float width, float height, float angle ,float red, float green, float blue);
void drawRectangle(GLint locTransform, GLint locTint, const glm::mat4& parent,float x, float y, float width, float height,float red, float green, float blue,float angle);