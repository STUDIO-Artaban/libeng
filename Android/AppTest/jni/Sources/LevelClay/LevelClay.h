#ifndef LEVELCLAY_H_
#define LEVELCLAY_H_

#include <libeng/Log/Log.h>
#include <libeng/Game/Level.h>
#include <libeng/Graphic/Object/2D/Static2D.h>
#include <libeng/Graphic/Progress/Progress.h>
#include <libeng/Graphic/Scrolling/Scrolling.h>
#include <libeng/Tools/Tools.h>

#ifdef __ANDROID__
#include "Objects/ClayMan.h"
#else
#include "LevelClay/Objects/ClayMan.h"
#endif

#define SCROLL_MOVE     10 // Change 'SCROLL_MOVE' & 'SCROLL_FACTOR' to obtain the best move according the scrolling

using namespace eng;

//////
class LevelClay : public Level {

private:
    Progress* mProgress;

    Scroll* mFloor;
    ClayMan* mClayMan;

    Static2D* mBackButton;
    short mBackLeft;

    short mPrevX; // x0
    short mPrevY; // y0
    Line mVelocity;

    float mSlopeUR2U;
    float mSlopeR2UR;
    float mSlopeDR2R;
    float mSlopeD2DR;

    inline void updateClayMove(short x1, short y1) {

        short dx = (x1 >= mPrevX)? x1 - mPrevX:mPrevX - x1;
        short dy = (y1 >= mPrevY)? y1 - mPrevY:mPrevY - y1;

        mClayMove.velocity = std::max<short>(dx, dy);
        if ((!dx) && (!dy))
            return; // Keep all others ClayMan move info (included direction)

#ifndef LIBENG_PORT_AS_LAND
        mClayMove.backward = (x1 > mPrevX);

        // Direction -> 0: DOWN; 1: DR; 2: RIGHT; 3: UR; 4: UP
        if (!dx) {

            mClayMove.direction = (y1 < mPrevY)? 0:4;
            return;
        }

        float slope = ((y1 < mPrevY)? -dy:dy) / static_cast<float>(dx);
        if (slope < mSlopeD2DR)
            mClayMove.direction = 0;
        else if (slope < mSlopeDR2R)
            mClayMove.direction = 1;
        else if (slope < mSlopeR2UR)
            mClayMove.direction = 2;
        else if (slope < mSlopeUR2U)
            mClayMove.direction = 3;
        else
            mClayMove.direction = 4;
#else
        mClayMove.backward = (y1 > mPrevY);

        // Direction -> 0: DOWN; 1: DR; 2: RIGHT; 3: UR; 4: UP
        if (!dy) {

            mClayMove.direction = (x1 < mPrevX)? 4:0;
            return;
        }

        float slope = ((x1 < mPrevX)? -dx:dx) / static_cast<float>(dy);
        if (slope < mSlopeD2DR)
            mClayMove.direction = 4;
        else if (slope < mSlopeDR2R)
            mClayMove.direction = 3;
        else if (slope < mSlopeR2UR)
            mClayMove.direction = 2;
        else if (slope < mSlopeUR2U)
            mClayMove.direction = 1;
        else
            mClayMove.direction = 0;
#endif
    }

public:
    LevelClay(Game* game);
    virtual ~LevelClay();

    ClayManMove mClayMove;

    //////
    void init() { }
    void pause();

    void wait(float millis) { }

protected:
    bool loaded(const Game* game);

public:
    bool update(const Game* game);

    void renderLoad() const;
    void renderUpdate() const;

};

#endif // LEVELCLAY_H_
