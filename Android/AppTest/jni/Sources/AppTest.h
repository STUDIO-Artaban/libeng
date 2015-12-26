#ifndef FINGERG_H_
#define FINGERG_H_

#include <libeng/Game/2D/Game2D.h>

#include "LevelMenu/LevelMenu.h"
#include "LevelMega/LevelMega.h"
#include "LevelNative/LevelNative.h"
#include "LevelClay/LevelClay.h"
#include "LevelWalk/LevelWalk.h"
#include "LevelCam/LevelCam.h"
#include "LevelMic/LevelMic.h"

#define FILE_NAME       "AppTest.backup"

// Data key(s)
#define JUMP_COUNT      "JumpCount"

using namespace eng;

//////
class AppTest : public Game2D {

private:
    AppTest();
    virtual ~AppTest();

    LevelMenu* mLevelMenu;
    LevelMega* mLevelMega;
    LevelNative* mLevelNative;
    LevelClay* mLevelClay;
    LevelWalk* mLevelWalk;
    LevelCam* mLevelCam;
    LevelMic* mLevelMic;

public:
    static AppTest* getInstance() {
        if (!mThis)
            mThis = new AppTest;
        return static_cast<AppTest*>(mThis);
    }
    static void freeInstance() {
        if (mThis) {
            delete mThis;
            mThis = NULL;
        }
    }

    //////
    void init() { }

    bool start();
    void pause();
    void destroy();

    void wait(float millis);
    void update();

    void render() const;

};

#endif // FINGERG_H_
