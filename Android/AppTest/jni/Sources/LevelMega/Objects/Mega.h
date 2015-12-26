#ifndef MEGA_H_
#define MEGA_H_

#include "Global.h"
#include "MegaAnims.h"

#include <libeng/Log/Log.h>
#include <libeng/Graphic/Object/2D/Sprite2D.h>

#define TEXTURE_ID_MEGAMAN      6
#define MEGA_TEXTURE_WIDTH      1024.0f
#define MEGA_TEXTURE_HEIGHT     512.0f

// MegaMan status mask
#define STATUS_BLINKING         0x01
#define STATUS_BACKWARD         0x02
#define STATUS_CROUCHED         0x04
#define STATUS_JUMPING          0x08
#define STATUS_FALLING          0x10

// Anims index definitions
enum {

    NONE = 0, WALK1, WALK2, WALK3, WALK4, WALK5, WALK6, WALK7, WALK8, WALK9, WALK10,
    BLINK1, BLINK2,
    JUMP1, JUMP2, JUMP3, JUMP4, JUMP5, JUMP6, JUMP7,
    CROUCH1, CROUCH2,
    CR_BLINK1, CR_BLINK2

};
#define MEGA_ANIM_COUNT         (CR_BLINK2 + 1)

static const float gs_MegaTexCoords[MEGA_ANIM_COUNT][8] = {
    { MEGA_NONE_X0/MEGA_TEXTURE_WIDTH,      MEGA_NONE_Y0/MEGA_TEXTURE_HEIGHT,      MEGA_NONE_X0/MEGA_TEXTURE_WIDTH,      MEGA_NONE_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_NONE_X2/MEGA_TEXTURE_WIDTH,      MEGA_NONE_Y2/MEGA_TEXTURE_HEIGHT,      MEGA_NONE_X2/MEGA_TEXTURE_WIDTH,      MEGA_NONE_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK1_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK1_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK1_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK1_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK1_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK1_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK1_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK1_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK2_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK2_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK2_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK2_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK2_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK2_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK2_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK2_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK3_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK3_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK3_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK3_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK3_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK3_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK3_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK3_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK4_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK4_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK4_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK4_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK4_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK4_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK4_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK4_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK5_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK5_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK5_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK5_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK5_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK5_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK5_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK5_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK6_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK6_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK6_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK6_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK6_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK6_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK6_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK6_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK7_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK7_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK7_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK7_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK7_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK7_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK7_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK7_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK8_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK8_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK8_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK8_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK8_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK8_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK8_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK8_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK9_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK9_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_WALK9_X0/MEGA_TEXTURE_WIDTH,     MEGA_WALK9_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK9_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK9_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_WALK9_X2/MEGA_TEXTURE_WIDTH,     MEGA_WALK9_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_WALK10_X0/MEGA_TEXTURE_WIDTH,    MEGA_WALK10_Y0/MEGA_TEXTURE_HEIGHT,    MEGA_WALK10_X0/MEGA_TEXTURE_WIDTH,    MEGA_WALK10_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_WALK10_X2/MEGA_TEXTURE_WIDTH,    MEGA_WALK10_Y2/MEGA_TEXTURE_HEIGHT,    MEGA_WALK10_X2/MEGA_TEXTURE_WIDTH,    MEGA_WALK10_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_BLINK1_X0/MEGA_TEXTURE_WIDTH,    MEGA_BLINK1_Y0/MEGA_TEXTURE_HEIGHT,    MEGA_BLINK1_X0/MEGA_TEXTURE_WIDTH,    MEGA_BLINK1_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_BLINK1_X2/MEGA_TEXTURE_WIDTH,    MEGA_BLINK1_Y2/MEGA_TEXTURE_HEIGHT,    MEGA_BLINK1_X2/MEGA_TEXTURE_WIDTH,    MEGA_BLINK1_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_BLINK2_X0/MEGA_TEXTURE_WIDTH,    MEGA_BLINK2_Y0/MEGA_TEXTURE_HEIGHT,    MEGA_BLINK2_X0/MEGA_TEXTURE_WIDTH,    MEGA_BLINK2_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_BLINK2_X2/MEGA_TEXTURE_WIDTH,    MEGA_BLINK2_Y2/MEGA_TEXTURE_HEIGHT,    MEGA_BLINK2_X2/MEGA_TEXTURE_WIDTH,    MEGA_BLINK2_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_JUMP1_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP1_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP1_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP1_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_JUMP1_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP1_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP1_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP1_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_JUMP2_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP2_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP2_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP2_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_JUMP2_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP2_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP2_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP2_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_JUMP3_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP3_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP3_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP3_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_JUMP3_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP3_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP3_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP3_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_JUMP4_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP4_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP4_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP4_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_JUMP4_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP4_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP4_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP4_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_JUMP5_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP5_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP5_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP5_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_JUMP5_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP5_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP5_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP5_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_JUMP6_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP6_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP6_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP6_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_JUMP6_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP6_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP6_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP6_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_JUMP7_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP7_Y0/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP7_X0/MEGA_TEXTURE_WIDTH,     MEGA_JUMP7_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_JUMP7_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP7_Y2/MEGA_TEXTURE_HEIGHT,     MEGA_JUMP7_X2/MEGA_TEXTURE_WIDTH,     MEGA_JUMP7_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_CROUCH1_X0/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH1_Y0/MEGA_TEXTURE_HEIGHT,   MEGA_CROUCH1_X0/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH1_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_CROUCH1_X2/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH1_Y2/MEGA_TEXTURE_HEIGHT,   MEGA_CROUCH1_X2/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH1_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_CROUCH2_X0/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH2_Y0/MEGA_TEXTURE_HEIGHT,   MEGA_CROUCH2_X0/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH2_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_CROUCH2_X2/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH2_Y2/MEGA_TEXTURE_HEIGHT,   MEGA_CROUCH2_X2/MEGA_TEXTURE_WIDTH,   MEGA_CROUCH2_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_CR_BLINK1_X0/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK1_Y0/MEGA_TEXTURE_HEIGHT, MEGA_CR_BLINK1_X0/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK1_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_CR_BLINK1_X2/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK1_Y2/MEGA_TEXTURE_HEIGHT, MEGA_CR_BLINK1_X2/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK1_Y0/MEGA_TEXTURE_HEIGHT },
    { MEGA_CR_BLINK2_X0/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK2_Y0/MEGA_TEXTURE_HEIGHT, MEGA_CR_BLINK2_X0/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK2_Y2/MEGA_TEXTURE_HEIGHT,
      MEGA_CR_BLINK2_X2/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK2_Y2/MEGA_TEXTURE_HEIGHT, MEGA_CR_BLINK2_X2/MEGA_TEXTURE_WIDTH, MEGA_CR_BLINK2_Y0/MEGA_TEXTURE_HEIGHT } };

using namespace eng;

//////
class MegaMan : public Sprite2D {

private:
    unsigned char mStatus; // See 'MegaMan status mask' definitions
    unsigned char mIncIdx; // Hyperbolic buffer index (used when jumping & falling)

    float mPrevTransY; // Previous vertical position of MegaMan
    float mOverFloor; // Vertical bound between near & over floor area (MegaMan feet position when in middle of the screen)
    float mScrollY; // > 'mOverFloor' => Do not move MegaMan but world (to keep MegaMan on the middle of the screen)

    inline void translateY(float y) {

        if (mScrollY > mOverFloor) {
            mPrevTransY = mScrollY;
            mScrollY += y;
            if (!(mScrollY > mOverFloor)) {
                mTransform[TRANS_Y] = mScrollY;
                mScrollY = mOverFloor;
            }
        }
        else {
            mPrevTransY = mTransform[TRANS_Y];
            mTransform[TRANS_Y] += y;
            if (mTransform[TRANS_Y] > mOverFloor) {
                mScrollY = mTransform[TRANS_Y];
                mTransform[TRANS_Y] = mOverFloor;
            }
        }
    }
    inline void assignY(float y) {

        if (mScrollY > mOverFloor) mPrevTransY = mScrollY;
        else mPrevTransY = mTransform[TRANS_Y];
        if (y > mOverFloor) {
            mScrollY = y;
            mTransform[TRANS_Y] = mOverFloor;
        }
        else {
            mTransform[TRANS_Y] = y;
            mScrollY = mOverFloor;
        }
    }

    //
    inline void blink(unsigned short counter) {

        if (mStatus & STATUS_BLINKING) {

            if ((counter & 0x0007) != 0x0007) // 7 == bin(111)
                return;

            if ((mCurAnim != BLINK2) && (mCurAnim != CR_BLINK2))
                ++mCurAnim;
            else {

                mCurAnim = (!(mStatus & STATUS_CROUCHED))? NONE:CROUCH2;
                mStatus &= ~(STATUS_BLINKING);
            }
            animVertices(mCurAnim);
            animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
        }
        else if ((counter & 0x01FF) == 0x01FF) { // 511 == bin(111111111)

            if (!(mStatus & STATUS_CROUCHED))
                mCurAnim = WALK10;  // 'WALK10' == 'BLINK1' previous anim
                                    // -> Nothing for crouch management coz current anim 'CROUCH2' == 'CR_BLINK1' previous anim
            mStatus |= STATUS_BLINKING;
        }
    }
    inline void walk() {

        if (mCurAnim < WALK10) // '<' and not '!=' coz unknown current animation when start walking
            ++mCurAnim;
        else
            mCurAnim = WALK1;
        animVertices(mCurAnim);
        animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
    }
    inline void crouch() {

        LOGV(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mStatus |= STATUS_CROUCHED;

        if (mCurAnim > JUMP7) // 'JUMP7' == 'CROUCH1' previous animation
            ++mCurAnim;
        else
            mCurAnim = CROUCH1;
        animVertices(mCurAnim);
        animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
    }
    void jump();

    inline void startFalling(bool jumped) {

        LOGV(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mStatus |= STATUS_FALLING;
        mVelocities[0] = 0.f;
        mIncIdx = 0;

        if (!jumped) {

            mCurAnim = JUMP6;
            animVertices(mCurAnim);
            animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
        }
    }
    void fall();
    inline void stopFalling() {

        LOGV(LOG_LEVEL_MEGAMAN, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        assert(mStatus & STATUS_FALLING);

        mStatus &= ~(STATUS_FALLING);
        mVelocities[0] = 0.f;

        mIncIdx = 0;
        mCurAnim = JUMP7;
        // ...only usefull if no other action on the joypad is done (orelse user cannot see it coz displayed only one time)

        animVertices(mCurAnim);
        animTexCoords(gs_MegaTexCoords, mCurAnim, mStatus & STATUS_BACKWARD);
    }

protected:
    void fillVerticesAnims(unsigned char animIdx);

public:
    MegaMan();
    virtual ~MegaMan();

    inline bool isJumping() const { return (mStatus & STATUS_JUMPING); }
    inline bool isFalling() const { return (mStatus & STATUS_FALLING); }

    inline float getOverFloor() const { return mOverFloor; }
    inline bool isScrollWorld() const { return (mScrollY > mOverFloor); }
    inline float getScrollWorld() const {

        return (mPrevTransY < mOverFloor)? (mOverFloor - mScrollY):(mPrevTransY - mScrollY);
    }

    //
    void start(float floor);

    //////
    void update(const Game* game, const Level* level);

    bool checkCollision(const Bounds* bounds, unsigned char boundsId);
    bool checkCollision(const Object* object, unsigned char objectId);

};

#endif // MEGA_H_
