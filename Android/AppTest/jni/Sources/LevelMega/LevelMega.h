#ifndef LEVELMEGA_H_
#define LEVELMEGA_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Bounds/Overlap.h>
#include <libeng/Graphic/Scrolling/Scrolling.h>
#include <libeng/Graphic/Scrolling/Panel.h>
#include <libeng/Graphic/Text/2D/Text2D.h>
#include <libeng/Graphic/Object/2D/Element2D.h>
#include <libeng/Graphic/Progress/Progress.h>
#include <libeng/Gamepads/Joypad.h>

#ifdef __ANDROID__
#include "Objects/Pad.h"
#include "Objects/Mega.h"

#else
#include "LevelMega/Objects/Pad.h"
#include "LevelMega/Objects/Mega.h"

#endif

// Bounds IDs
#define BOUND_ID_FLOOR          0

// Object IDs
#define OBJECT_ID_PLATFORMA     0
#define OBJECT_ID_PLATFORMB     1
#define OBJECT_ID_PLATFORMC     2

// Texture ID
#define TEXTURE_ID_SKY          2

using namespace eng;

//////
class LevelMega : public Level {

private:
    BorderBottom mFloorBound; // Bottom floor bound
    BorderLeft mBoundLeft; // Left platform bound
    BorderRight mBoundRight; // Right platform bound

    Progress* mProgress;

    Scroll* mSky;
    Scroll* mMountains;
    Scroll* mFloor;

    float mFloorY; // Needed to reset vertical floor scroll position

    Element2D* mPlatformA;
    Element2D* mPlatformB;
    Element2D* mPlatformC;

    void getPlatformTexCoords(float* texCoords) const;
    void moveWorld(float y); // Move vertically

    Pad* mJoyPad;
    MegaMan* mMegaMan;

public:
    LevelMega(Game* game);
    virtual ~LevelMega();

    mutable Text2D* mScore;

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

#endif // LEVELMEGA_H_
