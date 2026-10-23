#include "Environment.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

ModelPart castleParts[] = { 
    {0,0,.140f,1.180f,0,.47f,.50f,.57f},
    {-.068f,0,.054f,1.300f,0,.58f,.61f,.68f},
    { .068f,0,.054f,1.300f,0,.58f,.61f,.68f},
    {-.068f,-.640f,.068f,.060f,0,.38f,.41f,.48f},
    { .068f,-.640f,.068f,.060f,0,.38f,.41f,.48f},
    {-.068f,.640f,.066f,.040f,0,.73f,.76f,.82f},
    { .068f,.640f,.066f,.040f,0,.73f,.76f,.82f},
    {-.089f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    {-.047f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    { .047f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    { .089f,.680f,.022f,.060f,0,.62f,.65f,.72f},
    {0,-.120f,.098f,.880f,0,.76f,.74f,.67f},
    {0,-.130f,.074f,.800f,0,.34f,.22f,.13f},
    {-.025f,-.130f,.004f,.780f,0,.20f,.13f,.08f},
    {0,-.130f,.004f,.780f,0,.20f,.13f,.08f},
    {.025f,-.130f,.004f,.780f,0,.20f,.13f,.08f},
    {0,.100f,.074f,.025f,0,.17f,.19f,.23f},
    {0,-.350f,.074f,.025f,0,.17f,.19f,.23f},
    {-.012f,-.130f,.009f,.045f,0,.85f,.68f,.27f},
    { .012f,-.130f,.009f,.045f,0,.85f,.68f,.27f},
    {0,.470f,.080f,.160f,0,.85f,.68f,.27f},
    {0,.470f,.060f,.130f,0,.25f,.36f,.65f},
    {0,.470f,.010f,.085f,0,.90f,.75f,.35f},
    {0,.485f,.040f,.014f,0,.90f,.75f,.35f},
    {-.068f,.350f,.017f,.100f,0,.14f,.17f,.23f},
    { .068f,.350f,.017f,.100f,0,.14f,.17f,.23f},
    {-.068f,-.300f,.017f,.100f,0,.14f,.17f,.23f},
    { .068f,-.300f,.017f,.100f,0,.14f,.17f,.23f}
};

ModelPart bannerParts[] = { {0,.012f,.055f,.024f,0,.38f,.40f,.45f},
    {0,.100f,.012f,.180f,0,.60f,.45f,.25f},
    {0,.196f,.022f,.020f,45,.85f,.68f,.27f},
    {.060f,.140f,.100f,.095f,0,.22f,.34f,.62f},
    {.060f,.184f,.100f,.007f,0,.85f,.68f,.27f},
    {.107f,.140f,.006f,.095f,0,.85f,.68f,.27f},
    {.060f,.096f,.100f,.007f,0,.85f,.68f,.27f},
    {.060f,.140f,.008f,.055f,0,.90f,.75f,.35f},
    {.060f,.148f,.040f,.008f,0,.90f,.75f,.35f} };

    ModelPart torchParts[] = {
{0,.075f,.030f,.065f,0,.25f,.27f,.31f},
    {.012f,.060f,.045f,.012f,0,.40f,.42f,.47f},
    {.025f,.095f,.014f,.090f,0,.42f,.25f,.12f},
    {.025f,.070f,.024f,.012f,0,.60f,.62f,.67f},
    {.025f,.128f,.037f,.018f,0,.30f,.32f,.36f},
    {.025f,.139f,.045f,.012f,0,.60f,.62f,.67f}

    };

    ModelPart crateParts[] = { {0,.070f,.140f,.140f,0,.43f,.27f,.13f},
    {-.025f,.070f,.004f,.115f,0,.25f,.16f,.08f},
    { .025f,.070f,.004f,.115f,0,.25f,.16f,.08f},
    {0,.132f,.140f,.016f,0,.65f,.44f,.23f},
    {0,.008f,.140f,.016f,0,.65f,.44f,.23f},
    {-.062f,.070f,.016f,.140f,0,.65f,.44f,.23f},
    { .062f,.070f,.016f,.140f,0,.65f,.44f,.23f},
    {0,.070f,.165f,.012f,45,.58f,.37f,.18f},
    {0,.070f,.165f,.012f,-45,.58f,.37f,.18f},
    {-.053f,.123f,.008f,.008f,0,.20f,.21f,.23f},
    { .053f,.123f,.008f,.008f,0,.20f,.21f,.23f},
    {-.053f,.017f,.008f,.008f,0,.20f,.21f,.23f},
    { .053f,.017f,.008f,.008f,0,.20f,.21f,.23f}  };



    ModelPart bushParts[] = { {0,.026f,.012f,.050f,0,.35f,.23f,.12f},
    {-.023f,.036f,.010f,.058f,-30,.35f,.23f,.12f},
    { .023f,.036f,.010f,.058f,30,.35f,.23f,.12f},
    {-.042f,.055f,.064f,.055f,-18,.15f,.34f,.18f},
    { .043f,.055f,.064f,.055f,18,.15f,.34f,.18f},
    {-.025f,.078f,.060f,.060f,30,.22f,.44f,.23f},
    { .025f,.078f,.060f,.060f,-30,.22f,.44f,.23f},
    {0,.095f,.060f,.060f,45,.28f,.51f,.27f},
    {0,.050f,.075f,.045f,0,.20f,.41f,.21f},
    {-.032f,.083f,.018f,.009f,-25,.40f,.62f,.32f},
    { .029f,.070f,.018f,.009f,25,.40f,.62f,.32f},
    {0,.112f,.015f,.008f,0,.45f,.66f,.35f} };


    void drawEnvironment(GLint locTransform, GLint locTint) {
        glm::mat4 identity = glm::mat4(1);

        drawRectangle(locTransform, locTint, identity,.10f, -.895f, 1.60f,.17f,.28f,.30f,.35f);

        for (int row =0; row <2; row++) {
            for (int column =0; column < 12; column++) {
                float left = -.70f + column * .14f;

                if (row == 1)
                left -= .07f;

                float right = left + .13f;

                if (left < -.70f) left = -.70f;
                if (right > .90f) right = .90f;
                if (right <=left) continue;

                bool light = (row + column) % 2 != 0;

                drawRectangle(locTransform, locTint, identity, (left + right) * .5f, -.938f + row * .078f, 
                right - left, .067f,
                light? .51f : .46f,
                light ? .54f : .49f,
                light? .60f : .55f);
        }
    }

    for (int cap = 0; cap < 10; cap++) {
        drawRectangle(locTransform, locTint, identity, -.62f + cap * .16f, -.811f, .153f, .026f, .66f, .69f ,.74f);

    }

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

       glm::mat4 parent = glm::translate(identity, glm::vec3(x, -.985f,0));

        drawParts(locTransform, locTint, parent, parts, count);

        if (decoration == 2 || decoration == 3) {

            float phase = gameTime * 7 + decoration;
            float height = .050f + .010f * (float)std::sin(phase);
            float width = .028f + .004f * (float)std::sin(phase +1);
            float lean = 8 * (float)std::sin(phase * .70f);

            glm::mat4 flame = glm::translate(parent, glm::vec3(.025f, .145f,0));

            flame = glm ::rotate (flame, glm:: radians(lean) ,glm::vec3(0,0,1));

            drawRectangle(locTransform, locTint, flame, 0,height * .5f, width, height, 1,.38f,.08f);

            drawRectangle(locTransform, locTint, flame, 0,height * .30f, width * .50f, height * .60f, 1,.80f,.20f); 
        }
    }

    for (int lane =0; lane < 3; lane++ ) {
            float red = lane == 1? .43f : .36f;
            float green = lane == 1? .47f : .40f;
            float blue = lane == 1? .43f : .36f;

            if (selectedLanes[lane]) {
                red = .55f;
                green = .65f;
                blue = .35f;
            }

            else if (hoveredLane == lane) {
                red = .55f;
                green = .59f;
                blue = .55f;
            }

            drawRectangle(locTransform, locTint, identity, .10f,laneY[lane], 1.60f, .40f, red, green, blue);}

            glm::mat4 castle = glm::translate(identity, glm::vec3(-.83f, -.10f, 0));

            drawParts(locTransform , locTint, castle, castleParts, sizeof(castleParts) / sizeof(castleParts[0]));

            for (int tower = 0; tower < 2; tower++) {
                for(int row = 0; row < 9; row++) {
                   drawRectangle( locTransform,locTint, castle , 
                    tower == 0? -.068f : .068f,
                    -.57f + row * .14f, .054f,.005f, .40f , .43f, .50f );

                   }
                }

            }


        

                
                
            
    