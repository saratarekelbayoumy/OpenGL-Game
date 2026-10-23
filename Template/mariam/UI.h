#pragma once

#include "GameShared.h"

void drawText(
    GLint locTransform,
    GLint locTint,
    const std::string& text,
    float centerX, float centerY, float cell,
    float red, float green, float blue );

void drawGameplay(GLint locTransform, GLint locTint);