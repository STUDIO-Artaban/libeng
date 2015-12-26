#include "Mega.h"
#include "Global.h"
#include "Pad.h"
#include "AppTest.h"

#include <libeng/Graphic/Bounds/2D/Rectangle2D.h>
#include <boost/format.hpp>

#define MEGAMAN_SCALE_RATE          (1.4f / 600.0f) // == MegaMan scale (Multiple of 2) / GalaxyTab screen height
#define MAX_VERTICAL_VELOCITY       0.03f // Maximum velocity when jumping & falling
#define MAX_JUMP_INCREASE           70 // Maximum height when jumping: MAX_JUMP_INCREASE * MAX_VERTICAL_VELOCITY (slightly
                                       // less coz using 'Dynamic2D::IncreaseBuffer' buffer)

// Buttons index definition
#define BUTTON_IDX_JUMP         0
#define BUTTON_IDX_SHOOT        1
#define BUTTON_IDX_ACTIVATE     2

//////
MegaMan::MegaMan() : Sprite2D(MEGA_ANIM_COUNT), mStatus(0), mIncIdx(0), mPrevTransY(0.f), mOverFloor(0.f), mScrollY(0.f) {

    LOGV(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    mVelocities = new float[1];
    mVelocities[0] = 0.f;
}
MegaMan::~MegaMan() { LOGV(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__); }

void MegaMan::fillVerticesAnims(unsigned char animIdx) {

    LOGV(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(" - a:%d"), __PRETTY_FUNCTION__, __LINE__, animIdx);
    assert(animIdx < MEGA_ANIM_COUNT);

    static short halfW = mScreen->width >> 1;
    static short halfH = mScreen->height >> 1;

    // Define centered vertices coordinates according current animation
    switch (animIdx) {
    case NONE:      positionVertices(halfW, (MEGA_NONE_Y2-MEGA_NONE_Y0) + halfH,           (MEGA_NONE_X2-MEGA_NONE_X0) + halfW,           halfH); break;
    case WALK1:     positionVertices(halfW, (MEGA_WALK1_Y2-MEGA_WALK1_Y0) + halfH,         (MEGA_WALK1_X2-MEGA_WALK1_X0) + halfW,         halfH); break;
    case WALK2:     positionVertices(halfW, (MEGA_WALK2_Y2-MEGA_WALK2_Y0) + halfH,         (MEGA_WALK2_X2-MEGA_WALK2_X0) + halfW,         halfH); break;
    case WALK3:     positionVertices(halfW, (MEGA_WALK3_Y2-MEGA_WALK3_Y0) + halfH,         (MEGA_WALK3_X2-MEGA_WALK3_X0) + halfW,         halfH); break;
    case WALK4:     positionVertices(halfW, (MEGA_WALK4_Y2-MEGA_WALK4_Y0) + halfH,         (MEGA_WALK4_X2-MEGA_WALK4_X0) + halfW,         halfH); break;
    case WALK5:     positionVertices(halfW, (MEGA_WALK5_Y2-MEGA_WALK5_Y0) + halfH,         (MEGA_WALK5_X2-MEGA_WALK5_X0) + halfW,         halfH); break;
    case WALK6:     positionVertices(halfW, (MEGA_WALK6_Y2-MEGA_WALK6_Y0) + halfH,         (MEGA_WALK6_X2-MEGA_WALK6_X0) + halfW,         halfH); break;
    case WALK7:     positionVertices(halfW, (MEGA_WALK7_Y2-MEGA_WALK7_Y0) + halfH,         (MEGA_WALK7_X2-MEGA_WALK7_X0) + halfW,         halfH); break;
    case WALK8:     positionVertices(halfW, (MEGA_WALK8_Y2-MEGA_WALK8_Y0) + halfH,         (MEGA_WALK8_X2-MEGA_WALK8_X0) + halfW,         halfH); break;
    case WALK9:     positionVertices(halfW, (MEGA_WALK9_Y2-MEGA_WALK9_Y0) + halfH,         (MEGA_WALK9_X2-MEGA_WALK9_X0) + halfW,         halfH); break;
    case WALK10:    positionVertices(halfW, (MEGA_WALK10_Y2-MEGA_WALK10_Y0) + halfH,       (MEGA_WALK10_X2-MEGA_WALK10_X0) + halfW,       halfH); break;
    case BLINK1:    positionVertices(halfW, (MEGA_BLINK1_Y2-MEGA_BLINK1_Y0) + halfH,       (MEGA_BLINK1_X2-MEGA_BLINK1_X0) + halfW,       halfH); break;
    case BLINK2:    positionVertices(halfW, (MEGA_BLINK2_Y2-MEGA_BLINK2_Y0) + halfH,       (MEGA_BLINK2_X2-MEGA_BLINK2_X0) + halfW,       halfH); break;
    case JUMP1:     positionVertices(halfW, (MEGA_JUMP1_Y2-MEGA_JUMP1_Y0) + halfH,         (MEGA_JUMP1_X2-MEGA_JUMP1_X0) + halfW,         halfH); break;
    case JUMP2:     positionVertices(halfW, (MEGA_JUMP2_Y2-MEGA_JUMP2_Y0) + halfH,         (MEGA_JUMP2_X2-MEGA_JUMP2_X0) + halfW,         halfH); break;
    case JUMP3:     positionVertices(halfW, (MEGA_JUMP3_Y2-MEGA_JUMP3_Y0) + halfH,         (MEGA_JUMP3_X2-MEGA_JUMP3_X0) + halfW,         halfH); break;
    case JUMP4:     positionVertices(halfW, (MEGA_JUMP4_Y2-MEGA_JUMP4_Y0) + halfH,         (MEGA_JUMP4_X2-MEGA_JUMP4_X0) + halfW,         halfH); break;
    case JUMP5:     positionVertices(halfW, (MEGA_JUMP5_Y2-MEGA_JUMP5_Y0) + halfH,         (MEGA_JUMP5_X2-MEGA_JUMP5_X0) + halfW,         halfH); break;
    case JUMP6:     positionVertices(halfW, (MEGA_JUMP6_Y2-MEGA_JUMP6_Y0) + halfH,         (MEGA_JUMP6_X2-MEGA_JUMP6_X0) + halfW,         halfH); break;
    case JUMP7:     positionVertices(halfW, (MEGA_JUMP7_Y2-MEGA_JUMP7_Y0) + halfH,         (MEGA_JUMP7_X2-MEGA_JUMP7_X0) + halfW,         halfH); break;
    case CROUCH1:   positionVertices(halfW, (MEGA_CROUCH1_Y2-MEGA_CROUCH1_Y0) + halfH,     (MEGA_CROUCH1_X2-MEGA_CROUCH1_X0) + halfW,     halfH); break;
    case CROUCH2:   positionVertices(halfW, (MEGA_CROUCH2_Y2-MEGA_CROUCH2_Y0) + halfH,     (MEGA_CROUCH2_X2-MEGA_CROUCH2_X0) + halfW,     halfH); break;
    case CR_BLINK1: positionVertices(halfW, (MEGA_CR_BLINK1_Y2-MEGA_CR_BLINK1_Y0) + halfH, (MEGA_CR_BLINK1_X2-MEGA_CR_BLINK1_X0) + halfW, halfH); break;
    case CR_BLINK2: positionVertices(halfW, (MEGA_CR_BLINK2_Y2-MEGA_CR_BLINK2_Y0) + halfH, (MEGA_CR_BLINK2_X2-MEGA_CR_BLINK2_X0) + halfW, halfH); break;
    }

    // Place MegaMan on the middle of the screen (horizontally)
    centerVertices(mVertices[4] / 2.f, 0.f);
    if (animIdx == NONE)
        mOverFloor = mVertices[1] / -2.f;

    std::memcpy(mVerticesAnims[animIdx], mVertices, sizeof(float) * 8);
}

void MegaMan::start(float floor) {

    LOGV(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Sprite2D::start(TEXTURE_ID_MEGAMAN);

    // Create & Fill animated vertices buffer
    for (unsigned char i = 0; i < MEGA_ANIM_COUNT; ++i) {

        mVerticesAnims[i] = new float[8];
        fillVerticesAnims(i);
    }

    assert(mOverFloor < 0.f);
    mScrollY = mOverFloor;
    mTransform[TRANS_Y] = mOverFloor;

    //mCurAnim = NONE;
    animVertices(NONE);
    animTexCoords(gs_MegaTexCoords, NONE, false);

    // Change scale & Position MegaMan on the floor
    float megaScale = mScreen->height * MEGAMAN_SCALE_RATE;
    scale(megaScale, megaScale);
    assignY(floor);
}

void MegaMan::jump() {

    mIncIdx -= 2;
    if (!mIncIdx) {

        mStatus &= ~(STATUS_JUMPING);
        startFalling(true);
    }
    else if (mIncIdx < INCREASE_BUFFER_SIZE) {

        mVelocities[0] = IncreaseBuffer[mIncIdx] * MAX_VERTICAL_VELOCITY;
        if (mIncIdx == INCREASE_TANGENT_IDX) {

            mCurAnim = JUMP4;
            animVertices(mCurAnim);
            animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
        }
    }
    else if (mCurAnim != JUMP3) {
        if (mIncIdx & 0x04) {

            animVertices(++mCurAnim);
            animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
        }
    }
    translateY(mVelocities[0]);
}
void MegaMan::fall() {

    if (mIncIdx < INCREASE_BUFFER_SIZE)
        mVelocities[0] = IncreaseBuffer[mIncIdx] * MAX_VERTICAL_VELOCITY;
    mIncIdx += 2;
    translateY(-mVelocities[0]);

    if ((mCurAnim != JUMP6) && (mIncIdx & 0x04)) { // ...anim should be 'JUMP4' when first call 'fall' method!

        animVertices(++mCurAnim);
        animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
    }
}

void MegaMan::update(const Game* game, const Level* level) {

    const Joypad::Data* joypadData = joyDataVia(joypadVia(game->mGamePad)->getData());
    if ((mStatus & STATUS_FALLING) || (mStatus & STATUS_JUMPING)) {

        if (mStatus & STATUS_BACKWARD) {
            if (joypadData->direction == Joypad::JPD_FORWARD) {
                mStatus &= ~(STATUS_BACKWARD);
                reverse(false);
            }
        }
        else if (joypadData->direction == Joypad::JPD_BACKWARD) {
            mStatus |= STATUS_BACKWARD;
            reverse(false);
        }

        if (mStatus & STATUS_JUMPING)
            jump(); ////// Jumping
        else
            fall(); ////// Falling
        return;
    }

    ////// Start jumping
    if (joypadData->buttonPushed[BUTTON_IDX_JUMP]) {

        LOGI(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(" - Jump"), __PRETTY_FUNCTION__, __LINE__);
        mStatus &= ~(STATUS_CROUCHED | STATUS_BLINKING);
        mStatus |= STATUS_JUMPING;

        mVelocities[0] = MAX_VERTICAL_VELOCITY;
        translateY(MAX_VERTICAL_VELOCITY);
        mIncIdx = MAX_JUMP_INCREASE;

        mCurAnim = JUMP1;
        animVertices(mCurAnim);
        animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);

        int* jumpCount = static_cast<int*>(game->getData(DATA_INT, JUMP_COUNT));
        assert(jumpCount);

        static_cast<const LevelMega*>(level)->mScore->update(boost::str(boost::wformat(L"%06u") % ++(*jumpCount)));
        Player::getInstance()->play(0);
        return;
    }

    static unsigned short counter = 0;
    switch (joypadData->direction) {

        ////// Walk
        case Joypad::JPD_FORWARD: {

            mStatus = 0;
            if ((counter & 0x0003) == 0x0003)
                walk();
            break;
        }
        case Joypad::JPD_BACKWARD: {

            mStatus = STATUS_BACKWARD;
            if ((counter & 0x0003) == 0x0003)
                walk();
            break;
        }

        ////// Crouch & Crouched blinking
        case Joypad::JPD_DOWN: {

            if (mCurAnim < CROUCH2) {

                mStatus &= ~(STATUS_BLINKING);
                if ((counter & 0x0003) == 0x0003)
                    crouch();
            }
            else
                blink(counter);
            break;
        }

        ////// Stop fall/Stop crouching/Blinking/None
        default: {

            if (mCurAnim == JUMP7) {
                if (mIncIdx & 0x04) {

                    mCurAnim = NONE;
                    animVertices(mCurAnim);
                    animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
                }
                mIncIdx += 2;
            }
            else if ((mStatus & STATUS_CROUCHED) ||
                    ((mCurAnim != NONE) && (!(mStatus & STATUS_BLINKING)))) { // Unknown current anim when no move is requested

                mStatus &= ~(STATUS_CROUCHED | STATUS_BLINKING);

                mCurAnim = NONE;
                animVertices(mCurAnim);
                animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
            }
            else
                blink(counter);
            break;
        }
    }
    ++counter;
}

bool MegaMan::checkCollision(const Bounds* bounds, unsigned char boundsId) {

    assert(boundsId == BOUND_ID_FLOOR);
    if (!(mTransform[TRANS_Y] > bBotVia(bounds)->mBottom)) {

        if (!(mStatus & STATUS_FALLING))
            return true; // Already on the floor

        stopFalling();
        assignY(bBotVia(bounds)->mBottom);
        return true;
    }
    return false;
}
bool MegaMan::checkCollision(const Object* object, unsigned char objectId) {

    assert((objectId == OBJECT_ID_PLATFORMA) || (objectId == OBJECT_ID_PLATFORMB) || (objectId == OBJECT_ID_PLATFORMC));
    bool overFloor = (elem2DVia(object)->getTop() > mOverFloor);
    if (((!overFloor) && (!(mTransform[TRANS_Y] > rect2DVia(elem2DVia(object)->getBounds(0))->mTop))) || // Near floor
            ((overFloor) && (!(rect2DVia(elem2DVia(object)->getBounds(0))->mTop < mOverFloor)))) {  // Over floor

        bool overW = (!((getRight() < rect2DVia(elem2DVia(object)->getBounds(0))->mLeft) ||
                (getLeft() > rect2DVia(elem2DVia(object)->getBounds(0))->mRight)));
        if (mStatus & STATUS_FALLING) {

            if ((overW) && (((!overFloor) && (mPrevTransY > rect2DVia(elem2DVia(object)->getBounds(0))->mTop)) ||
                    ((overFloor) && (rect2DVia(elem2DVia(object)->getBounds(0))->mOverlapY < mOverFloor)))) { // Check 'mOverlapY' that...
                stopFalling();                           // ...contains previous 'mTop' bound (see platform C move in 'LevelMega::update')
                if (!overFloor)
                    assignY(rect2DVia(elem2DVia(object)->getBounds(0))->mTop);
                //else // Do not position MegaMan but world (see 'Check collisions' section in 'LevelMega::update' method)
                return true;
            }
        }
        else if (!(mStatus & STATUS_JUMPING)) { // Not in falling or jumping status

            // Should not call this code when MegaMan is on the floor or on a platform
            // -> See 'Check collisions' section in 'LevelMega' (platform check order)
            if (overW)
                return true; // On the platform
            else
                startFalling(false);
        }
    }
    return false;
}
