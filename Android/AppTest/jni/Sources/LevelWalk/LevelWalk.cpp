#include "LevelWalk.h"
#include "../Global.h"

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Graphic/Scrolling/Panel.h>
#include <libeng/Gamepads/Walkpad.h>

#ifdef __ANDROID__
#include "../LevelMega/Objects/Pad.h"
#else
#include "OpenGLES/ES2/gl.h"
#include "Pad.h"
#endif

#define MAX_LOAD_STEP           7
#define PROGRESS_HEIGHT         8
#define FLOOR_HEIGHT            64.0f // Texture floor heihgt (in pixel)
#define BACK_BTN_TOP            ((BTN_ACTIVATE_Y2 - BTN_ACTIVATE_Y0) << 1)

#define WALKPAD_LEFT_L          50
#define WALKPAD_RIGHT_L         130
#define WALKPAD_LEFT_R          170
#define WALKPAD_RIGHT_R         250

// Texture IDs
#define TEXTURE_ID_WALKPAD      22
#define TEXTURE_ID_WALKFLOOR    24

//////
LevelWalk::LevelWalk(Game* game) : Level(game), mSpeedVelocity(0), mBackLeft(0), mBackBottom(0) {

    LOGV(LOG_LEVEL_LEVELWALK, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    mProgress = NULL;
    mBackground = NULL;
    mWalkFloor = NULL;
    mWalkPad = NULL;
    mWalkMan = NULL;
    mBackButton = NULL;

    ////// Inputs use & precision
    //mTouchPrecision = DEFAULT_TOUCH_PRECISION;
    Inputs::InputUse = USE_INPUT_TOUCH;
}
LevelWalk::~LevelWalk() {

    LOGV(LOG_LEVEL_LEVELWALK, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mProgress) delete mProgress;
    if (mBackground) delete mBackground;
    if (mWalkFloor) delete mWalkFloor;
    if (mWalkPad) delete mWalkPad;
    if (mWalkMan) delete mWalkMan;
    if (mBackButton) delete mBackButton;
}

void LevelWalk::pause() {

    LOGV(LOG_LEVEL_LEVELWALK, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if (mProgress) mProgress->pause();
    if (mBackground) mBackground->pause();
    if (mWalkFloor) mWalkFloor->pauseScroll();
    if (mWalkPad) mWalkPad->pause();
    if (mWalkMan) mWalkMan->pause();
    if (mBackButton) mBackButton->pause();
}

bool LevelWalk::loaded(const Game* game) {

    LOGV(LOG_LEVEL_LEVELWALK, 0, LOG_FORMAT(" (s:%d)"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
    switch (mLoadStep) {
        case 1: {

            if (!mProgress) {

                mProgress = new Progress;
                mProgress->initialize(game2DVia(game));

                Progress::Info progInfo;
                progInfo.maxPos = MAX_LOAD_STEP - 1;

                progInfo.doneRed = 0xff;
                progInfo.doneGreen = 0xff;
                progInfo.doneBlue = 0xff;

                progInfo.todoRed = 0x99;
                progInfo.todoGreen = 0x99;
                progInfo.todoBlue = 0x99;

                mProgress->start(&progInfo);
                mProgress->position(50, (game->getScreen()->height / 2) + (PROGRESS_HEIGHT / 2),
                        game->getScreen()->width - 50, (game->getScreen()->height / 2) - (PROGRESS_HEIGHT / 2));
            }
            else
                mProgress->resume();
            break;
        }
        case 2: { ////// Floor

            if (!mWalkFloor) {

                mWalkFloor = new Scroll;
                mWalkFloor->assign(0, new Panel);
                mWalkFloor->assign(1, new Panel);
                mWalkFloor->assign(2, new Panel);
                mWalkFloor->initScroll(game2DVia(game));
                mWalkFloor->startScroll(TEXTURE_ID_WALKFLOOR, true);

                // Set vertices according texture size and reposition panels at the bottom of the screen
                assert(mWalkFloor->getTextureIdx() == 1);
                float floorY = ((*mTextures)[1]->height * 2.f / (*mTextures)[1]->width) - 1.f;

                float verticesY[4];
                verticesY[0] = floorY;
                verticesY[1] = -1.f;
                verticesY[2] = -1.f;
                verticesY[3] = floorY;
                panelVia((*mWalkFloor)[0])->setVerticesH(verticesY);
                panelVia((*mWalkFloor)[1])->setVerticesH(verticesY);
                panelVia((*mWalkFloor)[2])->setVerticesH(verticesY);
            }
            else
                mWalkFloor->resumeScroll(TEXTURE_ID_WALKFLOOR);
            break;
        }
        case 3: { ////// WalkMan

            if (!mWalkMan) {

                mWalkMan = new WalkMan;
                mWalkMan->initialize(game2DVia(game));

                assert(mWalkFloor->getTextureIdx() == 1);
                mWalkMan->start(game->getScreen()->bottom + ((FLOOR_HEIGHT * 2.f * // and not '->height'...
                        game->getScreen()->top) / (*mTextures)[1]->width));
                // ...coz floor texture is not a square texture and others reason (check code lines above)
            }
            else
                mWalkMan->resume(TEXTURE_ID_WALKMAN);
            break;
        }
        case 4: { //////Walkpad

            if (!mWalkPad) {

                mWalkPad = new Static2D;
                mWalkPad->initialize(game2DVia(game));
                mWalkPad->start(TEXTURE_ID_WALKPAD);

                assert(mWalkPad->getTextureIdx() == 3);
                float texCoords[8] = {0};
                texCoords[3] = 1.f;
                texCoords[4] = 52.f / (*mTextures)[3]->width;
                texCoords[5] = 1.f;
                texCoords[6] = texCoords[4];
                mWalkPad->setTexCoords(texCoords);

#ifndef LANDSCAPE_ORIENTATION
                mWalkPad->setVertices(WALKPAD_LEFT_L, game->getScreen()->height - WALKPAD_TOP, WALKPAD_RIGHT_R,
                        game->getScreen()->height - WALKPAD_BOTTOM);
#else
                mWalkPad->setVertices(WALKPAD_LEFT_L, game->getScreen()->width - WALKPAD_TOP, WALKPAD_RIGHT_R,
                        game->getScreen()->width - WALKPAD_BOTTOM);
#endif
                assert(!game->mGamePad);
                game->mGamePad = new Walkpad;

                WalkConfig config;
                std::memset(&config, 0, sizeof(WalkConfig));

                config.leftL = WALKPAD_LEFT_L;
                config.rightL = WALKPAD_RIGHT_L;
                config.leftR = WALKPAD_LEFT_R;
                config.rightR = WALKPAD_RIGHT_R;

                config.top = WALKPAD_TOP;
                config.bottom = WALKPAD_BOTTOM;
#ifdef DEBUG
                static_cast<Walkpad*>(game->mGamePad)->initialize(game, &config, game->getLog());
#else
                static_cast<Walkpad*>(game->mGamePad)->initialize(game, &config);
#endif
            }
            else
                mWalkPad->resume(TEXTURE_ID_WALKPAD);
            break;
        }
        case 5: { ////// Background

            if (!mBackground) {

                mBackground = new Back2D;
                mBackground->initialize(game2DVia(game));
                mBackground->start(TEXTURE_ID_WALKMAN);

                assert(mWalkMan->getTextureIdx() == 2);
                float texCoords[8]; // White pixel in WalkMan texture
                texCoords[0] = WHITE_PIX_X0 / (*mTextures)[2]->width;
                texCoords[1] = WHITE_PIX_Y0 / (*mTextures)[2]->height;
                texCoords[2] = texCoords[0];
                texCoords[3] = WHITE_PIX_Y2 / (*mTextures)[2]->height;
                texCoords[4] = WHITE_PIX_X2 / (*mTextures)[2]->width;
                texCoords[5] = texCoords[3];
                texCoords[6] = texCoords[4];
                texCoords[7] = texCoords[1];
                mBackground->setTexCoords(texCoords);
            }
            else
                mBackground->resume(TEXTURE_ID_WALKMAN);
            break;
        }
        case 6: {

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
                mBackBottom = game->getScreen()->height - BACK_BTN_TOP;
                mBackButton->setVertices(mBackLeft, game->getScreen()->height, game->getScreen()->width, mBackBottom);
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

            LOGD(LOG_LEVEL_LEVELMEGA, 0, LOG_FORMAT(" - Unknown load step: %d"), __PRETTY_FUNCTION__, __LINE__,
                    mLoadStep);
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
bool LevelWalk::update(const Game* game) {

    // Check back button click
    unsigned char touchCount = game->mTouchCount;
    if ((touchCount--) && (game->mTouchData[touchCount].Type == TouchInput::TOUCH_UP)) {

#ifndef LIBENG_PORT_AS_LAND
        if ((game->mTouchData[touchCount].X > mBackLeft) && (game->mTouchData[touchCount].Y < mBackBottom)) {
#else
        if ((game->mTouchData[touchCount].Y > mBackLeft) && (game->mTouchData[touchCount].X > mBackBottom)) {
#endif
            delete game->mGamePad;
            game->mGamePad = NULL;

            game->setGameLevel(1); // Menu level
            return false;
        }
    }

    ////// Update
    const Walkpad::Data* walkpadData = walkdataVia(walkpadVia(game->mGamePad)->getData());
    switch (walkpadData->status) {
        case Walkpad::WPS_WALK: {

            if (!walkpadData->direction) {

                if (mSpeedVelocity < 0) // Reversal walk
                    mSpeedVelocity += walkpadData->speed >> 2;
                else
                    mSpeedVelocity = walkpadData->speed; // Forward
            }
            else if (mSpeedVelocity > 0) // Reversal walk
                mSpeedVelocity -= walkpadData->speed >> 2;
            else
                mSpeedVelocity = -walkpadData->speed; // Backward
            break;
        }
        default: {

            if (mSpeedVelocity > 0) mSpeedVelocity--;
            else if (mSpeedVelocity < 0) mSpeedVelocity++;
            break;
        }
    }
    mWalkFloor->scrollX(mSpeedVelocity * WALKPAD_FACTOR);
    mWalkMan->update(game, this);

    return true;
}

void LevelWalk::renderLoad() const {

    glDisable(GL_BLEND);
    mProgress->render(false);
    glEnable(GL_BLEND);
}
void LevelWalk::renderUpdate() const {

    mBackground->render(false);

    mWalkMan->render(false);
    mWalkFloor->renderScroll(true);
    mWalkPad->render(false);
    mBackButton->render(false);
}
