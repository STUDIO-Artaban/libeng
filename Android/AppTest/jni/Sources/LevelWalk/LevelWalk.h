#ifndef LEVELWALK_H_
#define LEVELWALK_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Progress/Progress.h>
#include <libeng/Graphic/Scrolling/Scrolling.h>
#include <libeng/Graphic/Object/2D/Static2D.h>

#ifdef __ANDROID__
#include "Objects/Walker.h"
#else
#include "LevelWalk/Objects/Walker.h"
#endif

#define WALKPAD_FACTOR          -0.0006f // Factor to convert speed returned by the walkpad into scroll translation

using namespace eng;

//////
class LevelWalk : public Level {

private:
    Progress* mProgress;

    Static2D* mBackButton;
    short mBackLeft;
    short mBackBottom;

    Back2D* mBackground;
    Scroll* mWalkFloor;
    Static2D* mWalkPad;
    WalkMan* mWalkMan;

public:
    LevelWalk(Game* game);
    virtual ~LevelWalk();

    short mSpeedVelocity;

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

// Cast(s)
#define walklevelVia(l) static_cast<const LevelWalk*>(l)

#endif // LEVELWALK_H_
