#pragma once

#include "GameShared.h"
bool initTextRenderer();
void destroyTextRenderer();

void drawText(
    GLint locTransform,
    GLint locTint,
    const std::string& text,
    float centerX, float centerY, float cell,
    float red, float green, float blue );

void drawGameplay(GLint locTransform, GLint locTint);