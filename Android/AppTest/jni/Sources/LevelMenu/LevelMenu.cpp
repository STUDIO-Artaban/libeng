#include "LevelMenu.h"
#include "AppTest.h"

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Features/Internet/Internet.h>

#ifdef __ANDROID__
#include "LevelMega/LevelMega.h"
#else
#include "LevelMega.h"
#endif
#include <boost/format.hpp>

//////
LevelMenu::LevelMenu(Game* game) : Level(game), mMegaLeft(0), mMegaRight(0), mMegaTop(0), mMegaBottom(0),
    mNativeLeft(0), mNativeRight(0), mNativeTop(0), mNativeBottom(0), mClayLeft(0), mClayRight(0), mClayTop(0),
    mClayBottom(0), mWalkLeft(0), mWalkRight(0), mWalkTop(0), mWalkBottom(0), mMikeTop(0), mMikeBottom(0),
    mMikeRight(0), mMikeLeft(0), mCamTop(0), mCamBottom(0), mCamRight(0), mCamLeft(0) {

    LOGV(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    mAdLoaded = false;
    mAdvertising = NULL;

    ////// Inputs use & precision
    //mTouchPrecision = DEFAULT_TOUCH_PRECISION;
    Inputs::InputUse = USE_INPUT_TOUCH;

    mBackground = NULL;
    mMenu = NULL;
    mMegaChoice = NULL;
    mNativeChoice = NULL;
    mClayChoice = NULL;
    mWalkChoice = NULL;
    mCamChoice = NULL;
    mMikeChoice = NULL;
}
LevelMenu::~LevelMenu() {

    LOGV(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if(mBackground) delete mBackground;
    if(mMenu) delete mMenu;
    if(mMegaChoice) delete mMegaChoice;
    if(mNativeChoice) delete mNativeChoice;
    if(mClayChoice) delete mClayChoice;
    if(mWalkChoice) delete mWalkChoice;
    if(mCamChoice) delete mCamChoice;
    if(mMikeChoice) delete mMikeChoice;

    Advertising::freeInstance();
}

void LevelMenu::pause() {

    LOGV(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if(mBackground) mBackground->pause();
    if(mMenu) mMenu->pause();
    if(mMegaChoice) mMegaChoice->pause();
    if(mNativeChoice) mNativeChoice->pause();
    if(mClayChoice) mClayChoice->pause();
    if(mWalkChoice) mWalkChoice->pause();
    if(mCamChoice) mCamChoice->pause();
    if(mMikeChoice) mMikeChoice->pause();
}

void LevelMenu::adDisplay(bool delay) {

    if (!mAdvertising)
        return;

    if (!mAdLoaded) {

        static unsigned char counter = 0;
        if ((!delay) || (++counter == (DISPLAY_DELAY << 2))) { // Avoid to call 'Advertising::getStatus' continually

            LOGI(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(" - Check to load/display advertising"), __PRETTY_FUNCTION__, __LINE__);
            switch (Advertising::getStatus()) {
                case Advertising::STATUS_FAILED: {

                    mAdvertising->load();
                    break;
                }
                case Advertising::STATUS_LOADED: {

                    mAdLoaded = true;
                    Advertising::display(0);
                    break;
                }
            }
            counter = 0;
        }
    }
}
bool LevelMenu::adHide() {

    if (mAdvertising) {
        
        unsigned char adStatus = mAdvertising->getStatus();
        if (adStatus == Advertising::STATUS_DISPLAYED)
            mAdvertising->hide(0);
        //else if ((adStatus == Advertising::STATUS_LOADING) || (adStatus == Advertising::STATUS_DISPLAYING))
        else if (adStatus == Advertising::STATUS_DISPLAYING)
            return false;
    }
    return true;
}

bool LevelMenu::loaded(const Game* game) {

    LOGV(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (!mAdvertising) {

#ifdef __ANDROID__
    if (Internet::getInstance()->isOnline(2000))
        mAdvertising = Advertising::getInstance();
#else
    if (Internet::getInstance()->isOnline())
        mAdvertising = Advertising::getInstance();
#endif
    }
    if (mAdvertising) {

        unsigned char adStatus = Advertising::getStatus();
        assert(adStatus != Advertising::STATUS_NONE);

        mAdLoaded = false;
        if ((adStatus == Advertising::STATUS_READY) || (adStatus == Advertising::STATUS_FAILED))
            mAdvertising->load();
        else
            mAdLoaded = (adStatus > Advertising::STATUS_LOADED); // STATUS_DISPLAYING | STATUS_DISPLAYED
    }

    //////
    if (!mMenu) {

        mMenu = new Text2D;
        mMenu->initialize(game2DVia(game));
        mMenu->start(L"Sommaire");
        mMenu->position((mMenu->getLeft() - mMenu->getRight()) / 2.f, game->getScreen()->top);
    }
    else
        mMenu->resume();
    adDisplay(false);

    if (!mMegaChoice) {

        mMegaChoice = new Text2D;
        mMegaChoice->initialize(game2DVia(game));

        int* jumpCount = static_cast<int*>(game->getData(DATA_INT, JUMP_COUNT));
        assert(jumpCount);

        mMegaChoice->start(boost::str(boost::wformat(L"-= MegaMan (%u) =-") % (*jumpCount)));
        mMegaChoice->scale(2.f, 2.f); // Call it B4 positionning it
        mMegaChoice->position((mMegaChoice->getLeft() - mMegaChoice->getRight()) / 2.f, game->getScreen()->top / 2.f);

        mMegaLeft = static_cast<short>((mMegaChoice->getLeft() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
        mMegaRight = static_cast<short>((mMegaChoice->getRight() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
#ifndef LIBENG_PORT_AS_LAND
        mMegaTop = std::abs<short>(static_cast<short>((mMegaChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mMegaBottom = std::abs<short>(static_cast<short>((mMegaChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#else
        mMegaTop = game->getScreen()->height - std::abs<short>(static_cast<short>((mMegaChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mMegaBottom = game->getScreen()->height - std::abs<short>(static_cast<short>((mMegaChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#endif
    }
    else
        mMegaChoice->resume();

    if (!mNativeChoice) {

        mNativeChoice = new Text2D;
        mNativeChoice->initialize(game2DVia(game));
        mNativeChoice->start(L"-= NativeLib =-");
        mNativeChoice->scale(2.f, 2.f);
        mNativeChoice->position((mNativeChoice->getLeft() - mNativeChoice->getRight()) / 2.f, game->getScreen()->top / 4.f);

        mNativeLeft = static_cast<short>((mNativeChoice->getLeft() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
        mNativeRight = static_cast<short>((mNativeChoice->getRight() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
#ifndef LIBENG_PORT_AS_LAND
        mNativeTop = std::abs<short>(static_cast<short>((mNativeChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mNativeBottom = std::abs<short>(static_cast<short>((mNativeChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#else
        mNativeTop = game->getScreen()->height - std::abs<short>(static_cast<short>((mNativeChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mNativeBottom = game->getScreen()->height - std::abs<short>(static_cast<short>((mNativeChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#endif
    }
    else
        mNativeChoice->resume();

    if (!mClayChoice) {

        mClayChoice = new Text2D;
        mClayChoice->initialize(game2DVia(game));
        mClayChoice->start(L"-= ClayMan =-");
        mClayChoice->scale(2.f, 2.f);
        mClayChoice->position((mClayChoice->getLeft() - mClayChoice->getRight()) / 2.f, 0.f);

        mClayLeft = static_cast<short>((mClayChoice->getLeft() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
        mClayRight = static_cast<short>((mClayChoice->getRight() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
#ifndef LIBENG_PORT_AS_LAND
        mClayTop = std::abs<short>(static_cast<short>((mClayChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mClayBottom = std::abs<short>(static_cast<short>((mClayChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#else
        mClayTop = game->getScreen()->height - std::abs<short>(static_cast<short>((mClayChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mClayBottom = game->getScreen()->height - std::abs<short>(static_cast<short>((mClayChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#endif
    }
    else
        mClayChoice->resume();

    if (!mWalkChoice) {

        mWalkChoice = new Text2D;
        mWalkChoice->initialize(game2DVia(game));
        mWalkChoice->start(L"-= FingerWalk =-");
        mWalkChoice->scale(2.f, 2.f);
        mWalkChoice->position((mWalkChoice->getLeft() - mWalkChoice->getRight()) / 2.f, game->getScreen()->top / -4.f);

        mWalkLeft = static_cast<short>((mWalkChoice->getLeft() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
        mWalkRight = static_cast<short>((mWalkChoice->getRight() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
#ifndef LIBENG_PORT_AS_LAND
        mWalkTop = std::abs<short>(static_cast<short>((mWalkChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mWalkBottom = std::abs<short>(static_cast<short>((mWalkChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#else
        mWalkTop = game->getScreen()->height - std::abs<short>(static_cast<short>((mWalkChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mWalkBottom = game->getScreen()->height - std::abs<short>(static_cast<short>((mWalkChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#endif
    }
    else
        mWalkChoice->resume();

    if (!mCamChoice) {

        mCamChoice = new Text2D;
        mCamChoice->initialize(game2DVia(game));
        mCamChoice->start(L"-= Camera =-");
        mCamChoice->scale(2.f, 2.f);
        mCamChoice->position((mCamChoice->getLeft() - mCamChoice->getRight()) / 2.f,
                game->getScreen()->top - (game->getScreen()->top / 4.f));

        mCamLeft = static_cast<short>((mCamChoice->getLeft() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
        mCamRight = static_cast<short>((mCamChoice->getRight() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
#ifndef LIBENG_PORT_AS_LAND
        mCamTop = std::abs<short>(static_cast<short>((mCamChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mCamBottom = std::abs<short>(static_cast<short>((mCamChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#else
        mCamTop = game->getScreen()->height - std::abs<short>(static_cast<short>((mCamChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mCamBottom = game->getScreen()->height - std::abs<short>(static_cast<short>((mCamChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#endif
    }
    else
        mCamChoice->resume();

    if (!mMikeChoice) {

        mMikeChoice = new Text2D;
        mMikeChoice->initialize(game2DVia(game));
        mMikeChoice->start(L"-= Micro =-");
        mMikeChoice->scale(2.f, 2.f);
        mMikeChoice->position((mMikeChoice->getLeft() - mMikeChoice->getRight()) / 2.f,
                game->getScreen()->bottom + (game->getScreen()->top / 2.f));

        mMikeLeft = static_cast<short>((mMikeChoice->getLeft() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
        mMikeRight = static_cast<short>((mMikeChoice->getRight() / game->getScreen()->ratioW)) + (game->getScreen()->width >> 1);
#ifndef LIBENG_PORT_AS_LAND
        mMikeTop = std::abs<short>(static_cast<short>((mMikeChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mMikeBottom = std::abs<short>(static_cast<short>((mMikeChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#else
        mMikeTop = game->getScreen()->height - std::abs<short>(static_cast<short>((mMikeChoice->getTop() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
        mMikeBottom = game->getScreen()->height - std::abs<short>(static_cast<short>((mMikeChoice->getBottom() / \
                game->getScreen()->ratioH)) - (game->getScreen()->height >> 1));
#endif
    }
    else
        mMikeChoice->resume();

    if (!mBackground) {

        mBackground = new Back2D;
        mBackground->initialize(game2DVia(game));
        mBackground->start(TEXTURE_ID_SKY);
    }
    else
        mBackground->resume(TEXTURE_ID_SKY);

    adDisplay(false);
    return true;
}
bool LevelMenu::update(const Game* game) {

    // Advertising
    adDisplay(true);

    unsigned char touchCount = game->mTouchCount;
    while (touchCount--) {
        if (game->mTouchData[touchCount].Type == TouchInput::TOUCH_UP) {

            // Check MegaMan selection
#ifndef LIBENG_PORT_AS_LAND
            if ((game->mTouchData[touchCount].X > mMegaLeft) && (game->mTouchData[touchCount].X < mMegaRight) &&
                    (game->mTouchData[touchCount].Y > mMegaTop) && (game->mTouchData[touchCount].Y < mMegaBottom)) {
#else
            if ((game->mTouchData[touchCount].Y > mMegaLeft) && (game->mTouchData[touchCount].Y < mMegaRight) &&
                    (game->mTouchData[touchCount].X < mMegaTop) && (game->mTouchData[touchCount].X > mMegaBottom)) {
#endif
                LOGI(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(" - MegaMan selected"), __PRETTY_FUNCTION__, __LINE__);
                if (!adHide()) break;
                game->setGameLevel(3);
                return false;
            }

            // Check NativeLib selection
#ifndef LIBENG_PORT_AS_LAND
            if ((game->mTouchData[touchCount].X > mNativeLeft) && (game->mTouchData[touchCount].X < mNativeRight) &&
                    (game->mTouchData[touchCount].Y > mNativeTop) && (game->mTouchData[touchCount].Y < mNativeBottom)) {
#else
            if ((game->mTouchData[touchCount].Y > mNativeLeft) && (game->mTouchData[touchCount].Y < mNativeRight) &&
                    (game->mTouchData[touchCount].X < mNativeTop) && (game->mTouchData[touchCount].X > mNativeBottom)) {
#endif
                LOGI(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(" - NativeLib selected"), __PRETTY_FUNCTION__, __LINE__);
                if (!adHide()) break;
                game->setGameLevel(5);
                return false;
            }

            // Check ClayMan selection
#ifndef LIBENG_PORT_AS_LAND
            if ((game->mTouchData[touchCount].X > mClayLeft) && (game->mTouchData[touchCount].X < mClayRight) &&
                    (game->mTouchData[touchCount].Y > mClayTop) && (game->mTouchData[touchCount].Y < mClayBottom)) {
#else
            if ((game->mTouchData[touchCount].Y > mClayLeft) && (game->mTouchData[touchCount].Y < mClayRight) &&
                    (game->mTouchData[touchCount].X < mClayTop) && (game->mTouchData[touchCount].X > mClayBottom)) {
#endif
                LOGI(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(" - ClayMan selected"), __PRETTY_FUNCTION__, __LINE__);
                if (!adHide()) break;
                game->setGameLevel(7);
                return false;
            }

            // Check Walker selection
#ifndef LIBENG_PORT_AS_LAND
            if ((game->mTouchData[touchCount].X > mWalkLeft) && (game->mTouchData[touchCount].X < mWalkRight) &&
                    (game->mTouchData[touchCount].Y > mWalkTop) && (game->mTouchData[touchCount].Y < mWalkBottom)) {
#else
            if ((game->mTouchData[touchCount].Y > mWalkLeft) && (game->mTouchData[touchCount].Y < mWalkRight) &&
                    (game->mTouchData[touchCount].X < mWalkTop) && (game->mTouchData[touchCount].X > mWalkBottom)) {
#endif
                LOGI(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(" - Walker selected"), __PRETTY_FUNCTION__, __LINE__);
                if (!adHide()) break;
                game->setGameLevel(9);
                return false;
            }

            // Check Camera selection
#ifndef LIBENG_PORT_AS_LAND
            if ((game->mTouchData[touchCount].X > mCamLeft) && (game->mTouchData[touchCount].X < mCamRight) &&
                    (game->mTouchData[touchCount].Y > mCamTop) && (game->mTouchData[touchCount].Y < mCamBottom)) {
#else
            if ((game->mTouchData[touchCount].Y > mCamLeft) && (game->mTouchData[touchCount].Y < mCamRight) &&
                    (game->mTouchData[touchCount].X < mCamTop) && (game->mTouchData[touchCount].X > mCamBottom)) {
#endif
                LOGI(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(" - Camera selected"), __PRETTY_FUNCTION__, __LINE__);
                if (!adHide()) break;
                game->setGameLevel(11);
                return false;
            }

            // Check Mike selection
#ifndef LIBENG_PORT_AS_LAND
            if ((game->mTouchData[touchCount].X > mMikeLeft) && (game->mTouchData[touchCount].X < mMikeRight) &&
                    (game->mTouchData[touchCount].Y > mMikeTop) && (game->mTouchData[touchCount].Y < mMikeBottom)) {
#else
            if ((game->mTouchData[touchCount].Y > mMikeLeft) && (game->mTouchData[touchCount].Y < mMikeRight) &&
                    (game->mTouchData[touchCount].X < mMikeTop) && (game->mTouchData[touchCount].X > mMikeBottom)) {
#endif
                LOGI(LOG_LEVEL_LEVELMENU, 0, LOG_FORMAT(" - Mike selected"), __PRETTY_FUNCTION__, __LINE__);
                if (!adHide()) break;
                game->setGameLevel(13);
                return false;
            }
        }
    }
    return true;
}

void LevelMenu::renderUpdate() const {

    mBackground->render(false);

    mMenu->render(false);
    mMegaChoice->render(false);
    mNativeChoice->render(false);
    mClayChoice->render(false);
    mWalkChoice->render(false);
    mCamChoice->render(false);
    mMikeChoice->render(false);
}
