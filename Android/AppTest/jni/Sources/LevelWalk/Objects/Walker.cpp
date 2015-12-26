#include "Walker.h"
#include "Global.h"

#include "../LevelWalk.h"

#include <libeng/Log/Log.h>
#include <libeng/Gamepads/Walkpad.h>

#define WALKMAN_SCALE_RATE      (2.0f / 600.0f) // == WalkMan scale (Multiple of 2) / GalaxyTab screen height

//////
WalkMan::WalkMan() : Sprite2D(WALK_ANIM_COUNT) { LOGV(LOG_LEVEL_WALKMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__); }
WalkMan::~WalkMan() { LOGV(LOG_LEVEL_WALKMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__); }

void WalkMan::fillVerticesAnims(unsigned char animIdx) {

    LOGV(LOG_LEVEL_WALKMAN, 0, LOG_FORMAT(" - a:%d"), __PRETTY_FUNCTION__, __LINE__, animIdx);
    assert(animIdx < WALK_ANIM_COUNT);

    static short halfW = mScreen->width >> 1;
    static short halfH = mScreen->height >> 1;

    // Define centered vertices coordinates according current animation
    switch (animIdx) {
    case STAND: positionVertices(halfW, (WALK_NONE_Y2-WALK_NONE_Y0) + halfH, (WALK_NONE_X2-WALK_NONE_X0) + halfW, halfH); break;
    case RUN1:  positionVertices(halfW, (WALK_RUN1_Y2-WALK_RUN1_Y0) + halfH, (WALK_RUN1_X2-WALK_RUN1_X0) + halfW, halfH); break;
    case RUN2:  positionVertices(halfW, (WALK_RUN2_Y2-WALK_RUN2_Y0) + halfH, (WALK_RUN2_X2-WALK_RUN2_X0) + halfW, halfH); break;
    case RUN3:  positionVertices(halfW, (WALK_RUN3_Y2-WALK_RUN3_Y0) + halfH, (WALK_RUN3_X2-WALK_RUN3_X0) + halfW, halfH); break;
    case RUN4:  positionVertices(halfW, (WALK_RUN4_Y2-WALK_RUN4_Y0) + halfH, (WALK_RUN4_X2-WALK_RUN4_X0) + halfW, halfH); break;
    case RUN5:  positionVertices(halfW, (WALK_RUN5_Y2-WALK_RUN5_Y0) + halfH, (WALK_RUN5_X2-WALK_RUN5_X0) + halfW, halfH); break;
    case RUN6:  positionVertices(halfW, (WALK_RUN6_Y2-WALK_RUN6_Y0) + halfH, (WALK_RUN6_X2-WALK_RUN6_X0) + halfW, halfH); break;
    case RUN7:  positionVertices(halfW, (WALK_RUN7_Y2-WALK_RUN7_Y0) + halfH, (WALK_RUN7_X2-WALK_RUN7_X0) + halfW, halfH); break;
    case RUN8:  positionVertices(halfW, (WALK_RUN8_Y2-WALK_RUN8_Y0) + halfH, (WALK_RUN8_X2-WALK_RUN8_X0) + halfW, halfH); break;
    }

    // Place MegaMan on the middle of the screen (horizontally)
    centerVertices(mVertices[4] / 2.f, 0.f);

    std::memcpy(mVerticesAnims[animIdx], mVertices, sizeof(float) * 8);
}

void WalkMan::start(float floor) {

    LOGV(LOG_LEVEL_WALKMAN, 0, LOG_FORMAT(" - f:%f"), __PRETTY_FUNCTION__, __LINE__, floor);
    Sprite2D::start(TEXTURE_ID_WALKMAN);

    // Create & Fill animated vertices buffer
    for (unsigned char i = 0; i < WALK_ANIM_COUNT; ++i) {

        mVerticesAnims[i] = new float[8];
        fillVerticesAnims(i);
    }
    setAnim(STAND, false);

    // Change scale & Position MegaMan on the floor
    float walkScale = mScreen->height * WALKMAN_SCALE_RATE;
    scale(walkScale, walkScale);
    mTransform[TRANS_Y] = floor;
}

void WalkMan::update(const Game* game, const Level* level) {

    static unsigned char walkCounter = 0;
    static bool stopping = false;

    const Walkpad::Data* walkData = walkdataVia(walkpadVia(game->mGamePad)->getData());
    switch (walkData->status) {
        case Walkpad::WPS_WALK: {

            walk(walklevelVia(level)->mSpeedVelocity, walkCounter, walkData->direction);
            stopping = false;
            break;
        }
        case Walkpad::WPS_STOP:
        case Walkpad::WPS_TURNLEFT:
        case Walkpad::WPS_TURNRIGHT: {

            setAnim(STAND, walkData->direction);
            stopping = true;
            break;
        }
        default: {

            if (stopping)
                break;

            if (walklevelVia(level)->mSpeedVelocity)
                walk(walklevelVia(level)->mSpeedVelocity, walkCounter, walkData->direction);
            else
                setAnim(STAND, walkData->direction);
            break;
        }
    }
}
