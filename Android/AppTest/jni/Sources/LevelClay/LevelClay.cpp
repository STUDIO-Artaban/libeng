#include "LevelClay.h"

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Graphic/Scrolling/Panel.h>
#include <libeng/Tools/Tools.h>

#ifdef __ANDROID__
#include "../LevelMega/Objects/Pad.h"
#else
#include "OpenGLES/ES2/gl.h"
#include "Pad.h"
#endif

#define MAX_LOAD_STEP           5
#define TEXTURE_CHECK_SIZE      39
#define SCROLL_FACTOR           0.001f
#define BACK_BTN_TOP            ((BTN_ACTIVATE_Y2 - BTN_ACTIVATE_Y0) << 1)

// Texture IDs
#define TEXTURE_ID_CHECK1       12
#define TEXTURE_ID_CHECK2       13
#define TEXTURE_ID_CHECK3       14
#define TEXTURE_ID_CHECK4       15
#define TEXTURE_ID_CHECK5       16
#define TEXTURE_ID_CHECK6       17
#define TEXTURE_ID_CHECK7       18
#define TEXTURE_ID_CHECK8       19
#define TEXTURE_ID_CHECK9       20
#define TEXTURE_ID_CLAYMAN      21

void setCheckTexCoords(Scroll* scroll) {

    float texCoords[8] = {0};
    texCoords[3] = 16.f;
    texCoords[4] = 8.f;
    texCoords[5] = 16.f;
    texCoords[6] = 8.f;
    panelVia((*scroll)[0])->setTexCoords(texCoords);
    panelVia((*scroll)[1])->setTexCoords(texCoords);
    panelVia((*scroll)[2])->setTexCoords(texCoords);
}

//////
LevelClay::LevelClay(Game* game) : Level(game), mPrevX(0), mPrevY(0), mBackLeft(0) {

    LOGV(LOG_LEVEL_LEVELCLAY, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    mVelocity.X = 0;
    mVelocity.Y = 0;

    std::memset(&mClayMove, 0, sizeof(ClayManMove));

    mSlopeR2UR = std::tan(1.f * PI_F / 8.f);
    mSlopeUR2U = std::tan(3.f * PI_F / 8.f);
    mSlopeD2DR = std::tan(5.f * PI_F / 8.f);
    mSlopeDR2R = std::tan(7.f * PI_F / 8.f);

    ////// Inputs use & precision
    game->mTouchPrecision = 15;
    Inputs::InputUse = USE_INPUT_TOUCH;

    mProgress = NULL;
    mFloor = NULL;
    mClayMan = NULL;
    mBackButton = NULL;
}
LevelClay::~LevelClay() {

    LOGV(LOG_LEVEL_LEVELCLAY, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mProgress) delete mProgress;
    if (mFloor) delete mFloor;
    if (mClayMan) delete mClayMan;
    if (mBackButton) delete mBackButton;
}

void LevelClay::pause() {

    LOGV(LOG_LEVEL_LEVELCLAY, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if (mProgress) mProgress->pause();
    if (mFloor) mFloor->pauseScroll();
    if (mClayMan) mClayMan->pause();
    if (mBackButton) mBackButton->pause();
}

bool LevelClay::loaded(const Game* game) {

    LOGV(LOG_LEVEL_LEVELCLAY, 0, LOG_FORMAT(" - l:%d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
    switch (mLoadStep) {
        case 1: {

            if (!mProgress) {

                mProgress = new Progress;
                mProgress->initialize(game2DVia(game));

                Progress::Info progInfo;
                progInfo.maxPos = MAX_LOAD_STEP - 1;

                progInfo.doneRed = 0xff;
                progInfo.doneGreen = 0xff;
                progInfo.doneBlue = 0x00;

                progInfo.todoRed = 0x99;
                progInfo.todoGreen = 0x99;
                progInfo.todoBlue = 0x99;

                mProgress->start(&progInfo);
            }
            else
                mProgress->resume();
            break;
        }
        case 2: {

            if (!mFloor) { // Floor

                Scroll* scroll0 = new Scroll(3, false);
                scroll0->assign(0, new Panel(true));
                scroll0->assign(1, new Panel(true));
                scroll0->assign(2, new Panel(true));
                scroll0->initScroll(game2DVia(game));

                panelVia((*scroll0)[0])->Object2D::start(TEXTURE_ID_CHECK1);
                panelVia((*scroll0)[1])->Object2D::start(TEXTURE_ID_CHECK2);
                panelVia((*scroll0)[2])->Object2D::start(TEXTURE_ID_CHECK3);
                scroll0->startScroll(0, false, 0);
                setCheckTexCoords(scroll0);

                Scroll* scroll1 = new Scroll(3, false);
                scroll1->assign(0, new Panel(true));
                scroll1->assign(1, new Panel(true));
                scroll1->assign(2, new Panel(true));
                scroll1->initScroll(game2DVia(game));

                panelVia((*scroll1)[0])->Object2D::start(TEXTURE_ID_CHECK4);
                panelVia((*scroll1)[1])->Object2D::start(TEXTURE_ID_CHECK5);
                panelVia((*scroll1)[2])->Object2D::start(TEXTURE_ID_CHECK6);
                scroll1->startScroll(0, false, 1);
                setCheckTexCoords(scroll1);

                Scroll* scroll2 = new Scroll(3, false);
                scroll2->assign(0, new Panel(true));
                scroll2->assign(1, new Panel(true));
                scroll2->assign(2, new Panel(true));
                scroll2->initScroll(game2DVia(game));

                panelVia((*scroll2)[0])->Object2D::start(TEXTURE_ID_CHECK7);
                panelVia((*scroll2)[1])->Object2D::start(TEXTURE_ID_CHECK8);
                panelVia((*scroll2)[2])->Object2D::start(TEXTURE_ID_CHECK9);
                scroll2->startScroll(0, false, 2);
                setCheckTexCoords(scroll2);

                mFloor = new Scroll(3, false);
                mFloor->assign(0, scroll0);
                mFloor->assign(1, scroll1);
                mFloor->assign(2, scroll2);
                mFloor->initScroll(game2DVia(game));
                mFloor->startScroll(0, true);
            }
            else {

                panelVia((*scrollVia((*mFloor)[0]))[0])->Object2D::resume(TEXTURE_ID_CHECK1);
                panelVia((*scrollVia((*mFloor)[0]))[1])->Object2D::resume(TEXTURE_ID_CHECK2);
                panelVia((*scrollVia((*mFloor)[0]))[2])->Object2D::resume(TEXTURE_ID_CHECK3);
                panelVia((*scrollVia((*mFloor)[1]))[0])->Object2D::resume(TEXTURE_ID_CHECK4);
                panelVia((*scrollVia((*mFloor)[1]))[1])->Object2D::resume(TEXTURE_ID_CHECK5);
                panelVia((*scrollVia((*mFloor)[1]))[2])->Object2D::resume(TEXTURE_ID_CHECK6);
                panelVia((*scrollVia((*mFloor)[2]))[0])->Object2D::resume(TEXTURE_ID_CHECK7);
                panelVia((*scrollVia((*mFloor)[2]))[1])->Object2D::resume(TEXTURE_ID_CHECK8);
                panelVia((*scrollVia((*mFloor)[2]))[2])->Object2D::resume(TEXTURE_ID_CHECK9);
            }
            break;
        }
        case 3: {

            if (!mClayMan) {

                mClayMan = new ClayMan;
                mClayMan->initialize(game2DVia(game));
                mClayMan->start();
            }
            else
                mClayMan->resume(TEXTURE_ID_CLAYMAN);
            break;
        }
        case 4: {

            if (!mBackButton) {

                mBackButton = new Static2D;
                mBackButton->initialize(game2DVia(game));
                mBackButton->start(TEXTURE_ID_JOYPAD);

                float texCoords[8] = {0};
                texCoords[0] = BTN_ACTIVATE_X0 / JOYPAD_TEXTURE_WIDTH;
                texCoords[1] = BTN_ACTIVATE_Y0 / JOYPAD_TEXTURE_HEIGHT;
                texCoords[2] = BTN_ACTIVATE_X0 / JOYPAD_TEXTURE_WIDTH;
                texCoords[3] = BTN_ACTIVATE_Y2 / JOYPAD_TEXTURE_HEIGHT;
                texCoords[4] = BTN_ACTIVATE_X2 / JOYPAD_TEXTURE_WIDTH;
                texCoords[5] = BTN_ACTIVATE_Y2 / JOYPAD_TEXTURE_HEIGHT;
                texCoords[6] = BTN_ACTIVATE_X2 / JOYPAD_TEXTURE_WIDTH;
                texCoords[7] = BTN_ACTIVATE_Y0 / JOYPAD_TEXTURE_HEIGHT;
                mBackButton->setTexCoords(texCoords);

                mBackLeft = game->getScreen()->width - ((BTN_ACTIVATE_X2 - BTN_ACTIVATE_X0) << 1);
                mBackButton->setVertices(mBackLeft, BACK_BTN_TOP, game->getScreen()->width, 0);
            }
            else
                mBackButton->resume(TEXTURE_ID_JOYPAD);
            break;
        }
        case MAX_LOAD_STEP: { // Done

            delete mProgress;
            mProgress = NULL;
            break;
        }
#ifdef DEBUG
        default: {

            LOGD(LOG_LEVEL_LEVELCLAY, 0, LOG_FORMAT(" - Unknown load step: %d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
            assert(NULL);
            break;
        }
#endif
    }
    if (mLoadStep != MAX_LOAD_STEP) {

        mProgress->set(mLoadStep);
        return false;
    }
    return true;
}

bool LevelClay::update(const Game* game) {

    // Check back button click
    unsigned char touchCount = game->mTouchCount;
    if ((touchCount--) && (game->mTouchData[touchCount].Type == TouchInput::TOUCH_UP)) {

#ifndef LIBENG_PORT_AS_LAND
        if ((game->mTouchData[touchCount].X > mBackLeft) && (game->mTouchData[touchCount].Y > BACK_BTN_TOP)) {
#else
        if ((game->mTouchData[touchCount].Y > mBackLeft) && (game->mTouchData[touchCount].X < BACK_BTN_TOP)) {
#endif
            game->setGameLevel(1); // Menu level
            return false;
        }
    }

    ////// Update
    touchCount = game->mTouchCount;
    if (touchCount--) {

        switch (game->mTouchData[touchCount].Type) {
            case TouchInput::TOUCH_MOVE: {

#ifndef LIBENG_PORT_AS_LAND
                mVelocity.X = game->mTouchData[touchCount].X - mPrevX;
                mVelocity.Y = game->mTouchData[touchCount].Y - mPrevY;
#else
                mVelocity.X = game->mTouchData[touchCount].Y - mPrevY;
                mVelocity.Y = game->mTouchData[touchCount].X - mPrevX;
#endif
                if (mVelocity.X > CLAY_MAX_SPEED) mVelocity.X = CLAY_MAX_SPEED;
                else if (mVelocity.X < -CLAY_MAX_SPEED) mVelocity.X = -CLAY_MAX_SPEED;
                if (mVelocity.Y > CLAY_MAX_SPEED) mVelocity.Y = CLAY_MAX_SPEED;
                else if (mVelocity.Y < -CLAY_MAX_SPEED) mVelocity.Y = -CLAY_MAX_SPEED;

                mFloor->scrollX(mVelocity.X * SCROLL_FACTOR);
#ifndef LIBENG_PORT_AS_LAND
                mFloor->scrollY(mVelocity.Y * -SCROLL_FACTOR);
#else
                mFloor->scrollY(mVelocity.Y * SCROLL_FACTOR);
#endif

                updateClayMove(game->mTouchData[touchCount].X, game->mTouchData[touchCount].Y);

                mPrevX = game->mTouchData[touchCount].X;
                mPrevY = game->mTouchData[touchCount].Y;
                break;
            }
            case TouchInput::TOUCH_DOWN: {

                mPrevX = game->mTouchData[touchCount].X;
                mPrevY = game->mTouchData[touchCount].Y;

                mClayMove.velocity = 0;
                break;
            }
            case TouchInput::TOUCH_UP:
            case TouchInput::TOUCH_CANCEL: {

                mPrevX = mPrevY = 0;
                mVelocity.init(0, 0); // From X & Y velocity to 0
                break;
            }
        }
    }
    else if ((mVelocity.X) || (mVelocity.Y)) {

        if (mVelocity.X) mFloor->scrollX(mVelocity.X * SCROLL_FACTOR);
#ifndef LIBENG_PORT_AS_LAND
        if (mVelocity.Y) mFloor->scrollY(mVelocity.Y * -SCROLL_FACTOR);
#else
        if (mVelocity.Y) mFloor->scrollY(mVelocity.Y * SCROLL_FACTOR);
#endif

        mVelocity.next(); // Update X & Y velocity (decrease)
        mClayMove.velocity = std::max<short>(std::abs<short>(mVelocity.X), std::abs<short>(mVelocity.Y));
    }
    mClayMan->update(game, this);

    ////// Check collisions

    return true;
}

void LevelClay::renderLoad() const {

    glDisable(GL_BLEND);
    mProgress->render(false);
    glEnable(GL_BLEND);
}
void LevelClay::renderUpdate() const {

    glDisable(GL_BLEND);
    mFloor->renderScroll(false);
    glEnable(GL_BLEND);

    mClayMan->render(false);
    mBackButton->render(true);
}
