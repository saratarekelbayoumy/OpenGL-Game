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
