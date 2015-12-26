#include "ClayMan.h"
#include "Global.h"
#include "../LevelClay.h"

#include <libeng/Log/Log.h>

#define MIN_WALK_SPEED      30 // Counter limit before changing walk animation for a minimum speed (see 'walkCounter')
#define WALK_SPEED_RATIO    (MIN_WALK_SPEED * 1.0f) / (CLAY_MAX_SPEED * 1.0f) // Ratio to "convert" speed into animation speed

//////
ClayMan::ClayMan() : Sprite2D(CLAY_ANIM_COUNT), mPrevDirection(0) {

    LOGV(LOG_LEVEL_CLAYMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
}
ClayMan::~ClayMan() { LOGV(LOG_LEVEL_CLAYMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__); }

void ClayMan::fillVerticesAnims(unsigned char animIdx) {

    LOGV(LOG_LEVEL_CLAYMAN, 0, LOG_FORMAT(" - a:%d"), __PRETTY_FUNCTION__, __LINE__, animIdx);
    assert(animIdx < CLAY_ANIM_COUNT);

    static short halfW = mScreen->width >> 1;
    static short halfH = mScreen->height >> 1;

    // Define centered vertices coordinates according current animation
    switch (animIdx) {
    case DOWN:   positionVertices(halfW, (CLAY_NONE_Y2-CLAY_NONE_Y0) + halfH,     (CLAY_NONE_X2-CLAY_NONE_X0) + halfW,     halfH); break;
    case DOWN1:  positionVertices(halfW, (CLAY_DOWN1_Y2-CLAY_DOWN1_Y0) + halfH,   (CLAY_DOWN1_X2-CLAY_DOWN1_X0) + halfW,   halfH); break;
    case DOWN2:  positionVertices(halfW, (CLAY_DOWN2_Y2-CLAY_DOWN2_Y0) + halfH,   (CLAY_DOWN2_X2-CLAY_DOWN2_X0) + halfW,   halfH); break;
    case DOWN3:  positionVertices(halfW, (CLAY_DOWN3_Y2-CLAY_DOWN3_Y0) + halfH,   (CLAY_DOWN3_X2-CLAY_DOWN3_X0) + halfW,   halfH); break;
    case DOWN4:  positionVertices(halfW, (CLAY_DOWN4_Y2-CLAY_DOWN4_Y0) + halfH,   (CLAY_DOWN4_X2-CLAY_DOWN4_X0) + halfW,   halfH); break;
    case DOWN5:  positionVertices(halfW, (CLAY_DOWN5_Y2-CLAY_DOWN5_Y0) + halfH,   (CLAY_DOWN5_X2-CLAY_DOWN5_X0) + halfW,   halfH); break;
    case DOWN6:  positionVertices(halfW, (CLAY_DOWN6_Y2-CLAY_DOWN6_Y0) + halfH,   (CLAY_DOWN6_X2-CLAY_DOWN6_X0) + halfW,   halfH); break;
    case DOWN7:  positionVertices(halfW, (CLAY_DOWN7_Y2-CLAY_DOWN7_Y0) + halfH,   (CLAY_DOWN7_X2-CLAY_DOWN7_X0) + halfW,   halfH); break;
    case DOWN8:  positionVertices(halfW, (CLAY_DOWN8_Y2-CLAY_DOWN8_Y0) + halfH,   (CLAY_DOWN8_X2-CLAY_DOWN8_X0) + halfW,   halfH); break;
    case DR:     positionVertices(halfW, (CLAY_DR_Y2-CLAY_DR_Y0) + halfH,         (CLAY_DR_X2-CLAY_DR_X0) + halfW,         halfH); break;
    case DR1:    positionVertices(halfW, (CLAY_DR1_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR1_X2-CLAY_DR1_X0) + halfW,       halfH); break;
    case DR2:    positionVertices(halfW, (CLAY_DR2_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR2_X2-CLAY_DR2_X0) + halfW,       halfH); break;
    case DR3:    positionVertices(halfW, (CLAY_DR3_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR3_X2-CLAY_DR3_X0) + halfW,       halfH); break;
    case DR4:    positionVertices(halfW, (CLAY_DR4_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR4_X2-CLAY_DR4_X0) + halfW,       halfH); break;
    case DR5:    positionVertices(halfW, (CLAY_DR5_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR5_X2-CLAY_DR5_X0) + halfW,       halfH); break;
    case DR6:    positionVertices(halfW, (CLAY_DR6_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR6_X2-CLAY_DR6_X0) + halfW,       halfH); break;
    case DR7:    positionVertices(halfW, (CLAY_DR7_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR7_X2-CLAY_DR7_X0) + halfW,       halfH); break;
    case DR8:    positionVertices(halfW, (CLAY_DR8_Y2-CLAY_DR_Y0) + halfH,        (CLAY_DR8_X2-CLAY_DR8_X0) + halfW,       halfH); break;
    case RIGHT:  positionVertices(halfW, (CLAY_RIGHT_Y2-CLAY_RIGHT_Y0) + halfH,   (CLAY_RIGHT_X2-CLAY_RIGHT_X0) + halfW,   halfH); break;
    case RIGHT1: positionVertices(halfW, (CLAY_RIGHT1_Y2-CLAY_RIGHT1_Y0) + halfH, (CLAY_RIGHT1_X2-CLAY_RIGHT1_X0) + halfW, halfH); break;
    case RIGHT2: positionVertices(halfW, (CLAY_RIGHT2_Y2-CLAY_RIGHT2_Y0) + halfH, (CLAY_RIGHT2_X2-CLAY_RIGHT2_X0) + halfW, halfH); break;
    case RIGHT3: positionVertices(halfW, (CLAY_RIGHT3_Y2-CLAY_RIGHT3_Y0) + halfH, (CLAY_RIGHT3_X2-CLAY_RIGHT3_X0) + halfW, halfH); break;
    case RIGHT4: positionVertices(halfW, (CLAY_RIGHT4_Y2-CLAY_RIGHT4_Y0) + halfH, (CLAY_RIGHT4_X2-CLAY_RIGHT4_X0) + halfW, halfH); break;
    case RIGHT5: positionVertices(halfW, (CLAY_RIGHT5_Y2-CLAY_RIGHT5_Y0) + halfH, (CLAY_RIGHT5_X2-CLAY_RIGHT5_X0) + halfW, halfH); break;
    case RIGHT6: positionVertices(halfW, (CLAY_RIGHT6_Y2-CLAY_RIGHT6_Y0) + halfH, (CLAY_RIGHT6_X2-CLAY_RIGHT6_X0) + halfW, halfH); break;
    case RIGHT7: positionVertices(halfW, (CLAY_RIGHT7_Y2-CLAY_RIGHT7_Y0) + halfH, (CLAY_RIGHT7_X2-CLAY_RIGHT7_X0) + halfW, halfH); break;
    case RIGHT8: positionVertices(halfW, (CLAY_RIGHT8_Y2-CLAY_RIGHT8_Y0) + halfH, (CLAY_RIGHT8_X2-CLAY_RIGHT8_X0) + halfW, halfH); break;
    case UR:     positionVertices(halfW, (CLAY_UR_Y2-CLAY_UR_Y0) + halfH,         (CLAY_UR_X2-CLAY_UR_X0) + halfW,         halfH); break;
    case UR1:    positionVertices(halfW, (CLAY_UR1_Y2-CLAY_UR1_Y0) + halfH,       (CLAY_UR1_X2-CLAY_UR1_X0) + halfW,       halfH); break;
    case UR2:    positionVertices(halfW, (CLAY_UR2_Y2-CLAY_UR2_Y0) + halfH,       (CLAY_UR2_X2-CLAY_UR2_X0) + halfW,       halfH); break;
    case UR3:    positionVertices(halfW, (CLAY_UR3_Y2-CLAY_UR3_Y0) + halfH,       (CLAY_UR3_X2-CLAY_UR3_X0) + halfW,       halfH); break;
    case UR4:    positionVertices(halfW, (CLAY_UR4_Y2-CLAY_UR4_Y0) + halfH,       (CLAY_UR4_X2-CLAY_UR4_X0) + halfW,       halfH); break;
    case UR5:    positionVertices(halfW, (CLAY_UR5_Y2-CLAY_UR5_Y0) + halfH,       (CLAY_UR5_X2-CLAY_UR5_X0) + halfW,       halfH); break;
    case UR6:    positionVertices(halfW, (CLAY_UR6_Y2-CLAY_UR6_Y0) + halfH,       (CLAY_UR6_X2-CLAY_UR6_X0) + halfW,       halfH); break;
    case UR7:    positionVertices(halfW, (CLAY_UR7_Y2-CLAY_UR7_Y0) + halfH,       (CLAY_UR7_X2-CLAY_UR7_X0) + halfW,       halfH); break;
    case UR8:    positionVertices(halfW, (CLAY_UR8_Y2-CLAY_UR8_Y0) + halfH,       (CLAY_UR8_X2-CLAY_UR8_X0) + halfW,       halfH); break;
    case UP:     positionVertices(halfW, (CLAY_UP_Y2-CLAY_UP_Y0) + halfH,         (CLAY_UP_X2-CLAY_UP_X0) + halfW,         halfH); break;
    case UP1:    positionVertices(halfW, (CLAY_UP1_Y2-CLAY_UP1_Y0) + halfH,       (CLAY_UP1_X2-CLAY_UP1_X0) + halfW,       halfH); break;
    case UP2:    positionVertices(halfW, (CLAY_UP2_Y2-CLAY_UP2_Y0) + halfH,       (CLAY_UP2_X2-CLAY_UP2_X0) + halfW,       halfH); break;
    case UP3:    positionVertices(halfW, (CLAY_UP3_Y2-CLAY_UP3_Y0) + halfH,       (CLAY_UP3_X2-CLAY_UP3_X0) + halfW,       halfH); break;
    case UP4:    positionVertices(halfW, (CLAY_UP4_Y2-CLAY_UP4_Y0) + halfH,       (CLAY_UP4_X2-CLAY_UP4_X0) + halfW,       halfH); break;
    case UP5:    positionVertices(halfW, (CLAY_UP5_Y2-CLAY_UP5_Y0) + halfH,       (CLAY_UP5_X2-CLAY_UP5_X0) + halfW,       halfH); break;
    case UP6:    positionVertices(halfW, (CLAY_UP6_Y2-CLAY_UP6_Y0) + halfH,       (CLAY_UP6_X2-CLAY_UP6_X0) + halfW,       halfH); break;
    case UP7:    positionVertices(halfW, (CLAY_UP7_Y2-CLAY_UP7_Y0) + halfH,       (CLAY_UP7_X2-CLAY_UP7_X0) + halfW,       halfH); break;
    case UP8:    positionVertices(halfW, (CLAY_UP8_Y2-CLAY_UP8_Y0) + halfH,       (CLAY_UP8_X2-CLAY_UP8_X0) + halfW,       halfH); break;
    }

    // Place ClayMan on the middle of the screen
    if (animIdx != UR6) centerVertices(mVertices[4] / 1.9f, mVertices[1] / 1.9f);
    else centerVertices(mVertices[4] / 2.1f, mVertices[1] / 1.9f);
    // BUG: 1.9f/2.1f and not 2.f due to a bug when displaying (a pixel lag in the texture)

    std::memcpy(mVerticesAnims[animIdx], mVertices, sizeof(float) * 8);
}

void ClayMan::start() {

    LOGV(LOG_LEVEL_CLAYMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Sprite2D::start(TEXTURE_ID_CLAYMAN);

    // Create & Fill animated vertices buffer
    for (unsigned char i = 0; i < CLAY_ANIM_COUNT; ++i) {

        mVerticesAnims[i] = new float[8];
        fillVerticesAnims(i);
    }

    mCurAnim = DR;
    animVertices(mCurAnim);
    animTexCoords(gs_ClayTexCoords, mCurAnim, false);
}

void ClayMan::update(const Game* game, const Level* level) {

    const ClayManMove& clayMove = static_cast<const LevelClay*>(level)->mClayMove;
    if (clayMove.velocity) {

        static unsigned char walkCounter = 0;
        if (walkCounter < ((MIN_WALK_SPEED + 1) - static_cast<unsigned char>(clayMove.velocity * WALK_SPEED_RATIO))) {

            walkCounter += SCROLL_MOVE; // Change 'SCROLL_MOVE' & 'SCROLL_FACTOR' to obtain the best move according joypad entries
            return;
        }
        else
            walkCounter = 0;

        if (mPrevDirection != clayMove.direction)
            mCurAnim += (clayMove.direction - mPrevDirection) * 9; // 9 animations

        switch (clayMove.direction) {

            // 0: DOWN; 1: DR; 2: RIGHT; 3: UR; 4: UP
            case 0: walk(DOWN1, DOWN8, clayMove.backward); break;
            case 1: walk(DR1, DR8, clayMove.backward); break;
            case 2: walk(RIGHT1, RIGHT8, clayMove.backward); break;
            case 3: walk(UR1, UR8, clayMove.backward); break;
            case 4: walk(UP1, UP8, clayMove.backward); break;
        }
        mPrevDirection = clayMove.direction; // Can only change if velocity change
    }
    else if (mCurAnim % 9) { // Check if current animation is a walking animation

        // Reset animation according direction
        switch (clayMove.direction) {

            case 0: mCurAnim = DOWN; break;
            case 1: mCurAnim = DR; break;
            case 2: mCurAnim = RIGHT; break;
            case 3: mCurAnim = UR; break;
            case 4: mCurAnim = UP; break;
        }
        animVertices(mCurAnim);
        animTexCoords(gs_ClayTexCoords, mCurAnim, clayMove.backward);
    }
}
