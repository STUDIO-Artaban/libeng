#include "LevelMic.h"
#include "../Global.h"

#ifndef __ANDROID__
#include "OpenGLES/ES2/gl.h"
#endif

#include <libeng/Game/2D/Game2D.h>

#define MAX_MIC_DB     200

//////
LevelMic::LevelMic(Game* game) : Level(game) {

    LOGV(LOG_LEVEL_LEVELMIC, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    ////// Inputs use & precision
    //mTouchPrecision = DEFAULT_TOUCH_PRECISION;
    Inputs::InputUse = USE_INPUT_TOUCH;

    mMic = Mic::getInstance();

    mMicDB = NULL;
}
LevelMic::~LevelMic() {

    LOGV(LOG_LEVEL_LEVELMIC, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mMicDB) delete mMicDB;
}

void LevelMic::pause() {

    LOGV(LOG_LEVEL_LEVELMIC, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if (mMicDB) mMicDB->pause();
    if (mMic) mMic->stop();
}

bool LevelMic::loaded(const Game* game) {

    LOGV(LOG_LEVEL_LEVELMIC, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (!mMicDB) {

        mMicDB = new Progress;
        mMicDB->initialize(game2DVia(game));

        Progress::Info progInfo;
        progInfo.maxPos = MAX_MIC_DB;

        progInfo.doneRed = 0x00;
        progInfo.doneGreen = 0xff;
        progInfo.doneBlue = 0x00;

        progInfo.todoRed = 0x00;
        progInfo.todoGreen = 0x00;
        progInfo.todoBlue = 0x00;

        mMicDB->start(&progInfo);
        mMicDB->position(0, game->getScreen()->height, game->getScreen()->width, 0);
    }
    else
        mMicDB->resume();

    // Start mike recorder
    mMic->start();

    return true;
}
bool LevelMic::update(const Game* game) {

    // Check back button click
    unsigned char touchCount = game->mTouchCount;
    if ((touchCount--) && (game->mTouchData[touchCount].Type == TouchInput::TOUCH_UP)) {

        // Stop mike recorder
        mMic->stop();

        game->setGameLevel(1); // Menu level
        return false;
    }

    // Update mike level
    float mikeDB = mMic->getLevel();
    mMicDB->set((mikeDB > MAX_MIC_DB)? MAX_MIC_DB:static_cast<unsigned char>(mikeDB));

    return true;
}

void LevelMic::renderUpdate() const {

    glDisable(GL_BLEND);
    mMicDB->render(false);
    glEnable(GL_BLEND);
}
