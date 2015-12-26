#include "AppTest.h"
#include "Global.h"

#include <libeng/Log/Log.h>
#include <libeng/Storage/Storage.h>
#include <boost/thread.hpp>

#define FONT_TEXTURE_WIDTH      512
#define FONT_TEXTURE_HEIGHT     512

//////
AppTest::AppTest() : Game2D(1) {

    LOGV(LOG_LEVEL_FINGERG, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (!mGameLevel)
        mGameIntro = new Intro(Intro::LANG_FR);

    mLevelMenu = NULL;
    mLevelMega = NULL;
    mLevelNative = NULL;
    mLevelClay = NULL;
    mLevelWalk = NULL;
    mLevelCam = NULL;
    mLevelMic = NULL;

    ////// Fonts
    mFonts->addFont(0, 14, 26, FONT_TEXTURE_WIDTH, FONT_TEXTURE_HEIGHT);

    ////// Use hyperbolic buffer
    Dynamic2D::initIncreaseBuffer();

    ////// Data
    mGameData = new StoreData;
    // -> Let the framework save data into file (both variable defined)
}
AppTest::~AppTest() {

    LOGV(LOG_LEVEL_FINGERG, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Dynamic2D::freeIncreaseBuffer();
}

bool AppTest::start() { // ...in render thread

    LOGV(LOG_LEVEL_FINGERG, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (Game2D::start()) { // First call (start operation)

        // Load data from file
        if (Storage::loadFile(FILE_NAME)) {

            Storage* store = Storage::getInstance();
            if (!store->isEmpty(FILE_NAME)) {
                if (!Storage::File2Data(mGameData, (*store)[FILE_NAME])) {

                    LOGW(LOG_FORMAT(" - Failed to get data from '%s' file"), __PRETTY_FUNCTION__, __LINE__, FILE_NAME);
                    assert(NULL);
                }
            }
            else // No data stored: Create data
                mGameData->add(DATA_INT, JUMP_COUNT, static_cast<void*>(new int(0)));
        }
        else {

            LOGW(LOG_FORMAT(" - Failed to open exsiting '%s' file"), __PRETTY_FUNCTION__, __LINE__, FILE_NAME);
            assert(NULL);
        }
        return true;
    }
    else if (mGameLevel) { // Needed for pause & resume operation (restart operation)
        //if(!mGameLevel) -> Reserved to restart introduction (done in 'Game2D')

        // Use 'mGameLevel' rules (see 'mGameLevel' description in declaration)
        if (!(mGameLevel & 0x01)) // Pair?
            --mGameLevel;
    }
    return false;
}
void AppTest::pause() {

    LOGV(LOG_LEVEL_FINGERG, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Game2D::pause();

    if (mLevelMenu) mLevelMenu->pause();
    if (mLevelMega) mLevelMega->pause();
    if (mLevelNative) mLevelNative->pause();
    if (mLevelClay) mLevelClay->pause();
    if (mLevelWalk) mLevelWalk->pause();
    if (mLevelCam) mLevelCam->pause();
    if (mLevelMic) mLevelMic->pause();
}
void AppTest::destroy() {

    LOGV(LOG_LEVEL_FINGERG, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Game2D::destroy();

    if (mLevelMenu) delete mLevelMenu;
    if (mLevelMega) delete mLevelMega;
    if (mLevelNative) delete mLevelNative;
    if (mLevelClay) delete mLevelClay;
    if (mLevelWalk) delete mLevelWalk;
    if (mLevelCam) delete mLevelCam;
    if (mLevelMic) delete mLevelMic;
}

void AppTest::wait(float millis) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_FINGERG, (mLog % 100), LOG_FORMAT(" - Delay: %f milliseconds"), __PRETTY_FUNCTION__, __LINE__, millis);
#endif

    // Process any updates during the elapse time 'millis'
    // -> Call 'Game::getInputsData()' if inputs needed
    boost::this_thread::sleep(boost::posix_time::microseconds(static_cast<unsigned long>(millis * 900))); // millis * 1000
}
void AppTest::update() {

    switch (mGameLevel) {
        case 0: updateIntro(); break; // Introduction

        ////// Menu level
        case 1: {

            if (!mLevelMenu) {
                mLevelMenu = new LevelMenu(this);
                mLevelMenu->initialize();
            }
            if (!mLevelMenu->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 2: {

            if (!mLevelMenu->update(this)) {

                delete mLevelMenu;
                mLevelMenu = NULL;
            }
            break;
        }

        ////// MegaMan level
        case 3: { // Create/Initialize/Start objects | Restart objects

            if (!mLevelMega) {
                mLevelMega = new LevelMega(this);
                mLevelMega->initialize();
            }
            if (!mLevelMega->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 4: { // Update/Check collisions

            if (!mLevelMega->update(this)) {

                assert(mGameLevel == 1);
                delete mLevelMega;
                mLevelMega = NULL;
            }
            break;
        }

        ////// NativeLib level
        case 5: {

            if (!mLevelNative) {
                mLevelNative = new LevelNative(this);
                mLevelNative->initialize();
            }
            if (!mLevelNative->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 6: {

            if (!mLevelNative->update(this)) {

                assert(mGameLevel == 1);
                delete mLevelNative;
                mLevelNative = NULL;
            }
            break;
        }

        ////// ClayMan level
        case 7: {

            if (!mLevelClay) {
                mLevelClay = new LevelClay(this);
                mLevelClay->initialize();
            }
            if (!mLevelClay->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 8: {

            if (!mLevelClay->update(this)) {

                assert(mGameLevel == 1);
                delete mLevelClay;
                mLevelClay = NULL;
            }
            break;
        }

        ////// Walker level
        case 9: {

            if (!mLevelWalk) {
                mLevelWalk = new LevelWalk(this);
                mLevelWalk->initialize();
            }
            if (!mLevelWalk->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 10: {

            if (!mLevelWalk->update(this)) {

                assert(mGameLevel == 1);
                delete mLevelWalk;
                mLevelWalk = NULL;
            }
            break;
        }

        ////// Camera level
        case 11: {

            if (!mLevelCam) {
                mLevelCam = new LevelCam(this);
                mLevelCam->initialize();
            }
            if (!mLevelCam->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 12: {

            if (!mLevelCam->update(this)) {

                assert(mGameLevel == 1);
                delete mLevelCam;
                mLevelCam = NULL;
            }
            break;
        }

        ////// Mic level
        case 13: {

            if (!mLevelMic) {
                mLevelMic = new LevelMic(this);
                mLevelMic->initialize();
            }
            if (!mLevelMic->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 14: {

            if (!mLevelMic->update(this)) {

                assert(mGameLevel == 1);
                delete mLevelMic;
                mLevelMic = NULL;
            }
            break;
        }
#ifdef DEBUG
        default: {

            LOGD(LOG_LEVEL_FINGERG, 0, LOG_FORMAT(" - Unknown game level to update: %d"), __PRETTY_FUNCTION__, __LINE__,
                    mGameLevel);
            assert(NULL);
            break;
        }
#endif
    }
}

void AppTest::render() const {
    switch (mGameLevel) {

        case 0: renderIntro(); break; // Introduction
        case 1: if (mLevelMenu) mLevelMenu->renderLoad(); break;
        case 2: mLevelMenu->renderUpdate(); break;
        case 3: if (mLevelMega) mLevelMega->renderLoad(); break;
        case 4: mLevelMega->renderUpdate(); break;
        case 5: if (mLevelNative) mLevelNative->renderLoad(); break;
        case 6: mLevelNative->renderUpdate(); break;
        case 7: if (mLevelClay) mLevelClay->renderLoad(); break;
        case 8: mLevelClay->renderUpdate(); break;
        case 9: if (mLevelWalk) mLevelWalk->renderLoad(); break;
        case 10: mLevelWalk->renderUpdate(); break;
        case 11: if (mLevelCam) mLevelCam->renderLoad(); break;
        case 12: mLevelCam->renderUpdate(); break;
        case 13: if (mLevelMic) mLevelMic->renderLoad(); break;
        case 14: mLevelMic->renderUpdate(); break;

        default: assert(NULL); break;
    }
}
