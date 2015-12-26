#include "LevelMega.h"
#include "AppTest.h"

#ifndef __ANDROID__
#include "OpenGLES/ES2/gl.h"
#endif

#include <libeng/Log/Log.h>
#include <libeng/Game/2D/Game2D.h>
#include <boost/format.hpp>

#define MAX_LOAD_STEP           12
#define SCROLL_SPEED            -0.018f
#define PROGRESS_HEIGHT         32
#define FLOOR_HEIGHT            64.0f // Texture floor heihgt (in pixel)
#define MOUNTAIN_PANEL_WIDTH    256.f // Width of a mountain panel in the texture (in pixel)
#define PLATFORM_HEIGHT         50
#define PLATFORM_WIDTH          150
#define PLATFORMA_TOP_RATE      (200.0f / 600.0f) // == PlatformA top position in pixel on GalaxyTab / GalaxyTab screen height
#define PLATFORMB_TOP_RATE      (300.0f / 600.0f) // == PlatformB top position in pixel on GalaxyTab / GalaxyTab screen height
#define PLATFORMC_TOP_RATE      (450.0f / 600.0f) // == PlatformC top position in pixel on GalaxyTab / GalaxyTab screen height

#define SKY_DEPTH               0.1f
#define MOUNTAIN_DEPTH          0.3f

// Sound IDs
#define SOUND_ID_JUMP           1

// Texture IDs
#define TEXTURE_ID_MOUNTAIN     3
#define TEXTURE_ID_FLOOR        4

//////
LevelMega::LevelMega(Game* game) : Level(game), mFloorBound(0.f), mBoundLeft(0.f), mBoundRight(0.f),
    mFloorY(0.f) {

    LOGV(LOG_LEVEL_LEVELMEGA, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    ////// Inputs use & precision
    //mTouchPrecision = DEFAULT_TOUCH_PRECISION;
    Inputs::InputUse = USE_INPUT_TOUCH;

    mProgress = NULL;
    mSky = NULL;
    mMountains = NULL;
    mFloor = NULL;
    mPlatformA = NULL;
    mPlatformB = NULL;
    mPlatformC = NULL;
    mScore = NULL;
    mJoyPad = NULL;
    mMegaMan = NULL;
}
LevelMega::~LevelMega() {

    LOGV(LOG_LEVEL_LEVELMEGA, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mSky) delete mSky;
    if (mMountains) delete mMountains;
    if (mFloor) delete mFloor;
    if (mPlatformA) delete mPlatformA;
    if (mPlatformB) delete mPlatformB;
    if (mPlatformC) delete mPlatformC;
    if (mScore) delete mScore;
    if (mProgress) delete mProgress;
    if (mJoyPad) delete mJoyPad;
    if (mMegaMan) delete mMegaMan;
}

void LevelMega::pause() {

    LOGV(LOG_LEVEL_LEVELMEGA, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if (mSky) mSky->pauseScroll();
    if (mMountains) mMountains->pauseScroll();
    if (mFloor) mFloor->pauseScroll();
    if (mPlatformA) mPlatformA->pause();
    if (mPlatformB) mPlatformB->pause();
    if (mPlatformC) mPlatformC->pause();
    if (mScore) mScore->pause();
    if (mProgress) mProgress->pause();
    if (mJoyPad) mJoyPad->pause();
    if (mMegaMan) mMegaMan->pause();
}

void LevelMega::getPlatformTexCoords(float* texCoords) const {

    LOGV(LOG_LEVEL_LEVELMEGA, 0, LOG_FORMAT(" - t:%x"), __PRETTY_FUNCTION__, __LINE__, texCoords);
    assert(mFloor);

    texCoords[1] = ((*mTextures)[mFloor->getTextureIdx()]->height - FLOOR_HEIGHT) / \
            (*mTextures)[mFloor->getTextureIdx()]->height;
    texCoords[4] = 1.f / (*mTextures)[mFloor->getTextureIdx()]->width;
    texCoords[3] = ((*mTextures)[mFloor->getTextureIdx()]->height - (FLOOR_HEIGHT - PLATFORM_HEIGHT)) / \
            (*mTextures)[mFloor->getTextureIdx()]->height;
    texCoords[5] = texCoords[3];
    texCoords[6] = texCoords[4];
    texCoords[7] = texCoords[1];
}
bool LevelMega::loaded(const Game* game) {

    LOGV(LOG_LEVEL_LEVELMEGA, 0, LOG_FORMAT(" - l:%d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
    switch (mLoadStep) {
        case 1: {

            // Load font texture first (needed for 'Text2D' object below)
            Textures::loadTexture(TEXTURE_ID_FONT);

            if (!mProgress) {

                mProgress = new Progress;
                mProgress->initialize(game2DVia(game));

                Progress::Info progInfo;
                progInfo.maxPos = MAX_LOAD_STEP - 1;

                progInfo.doneRed = 0xff;
                progInfo.doneGreen = 0x00;
                progInfo.doneBlue = 0x00;

                progInfo.todoRed = 0x99;
                progInfo.todoGreen = 0x99;
                progInfo.todoBlue = 0x99;

                mProgress->start(&progInfo);
                mProgress->position(50, (game->getScreen()->height / 2) + (PROGRESS_HEIGHT / 2), game->getScreen()->width - 50,
                        (game->getScreen()->height / 2) - (PROGRESS_HEIGHT / 2));
            }
            else
                mProgress->resume();
            break;
        }
        case 2: {

            if (!mScore) {

                mScore = new Text2D;
                mScore->initialize(game2DVia(game));

                int* jumpCount = static_cast<int*>(game->getData(DATA_INT, JUMP_COUNT));
                assert(jumpCount);

                mScore->start(boost::str(boost::wformat(L"%06u") % (*jumpCount)));
                mScore->position(game->getScreen()->right - (mScore->getRight() - mScore->getLeft()), game->getScreen()->top);
            }
            else
                mScore->resume();
            break;
        }
        case 3: {        // -> See 'Surface2D::start' method

            if (!mSky) { ////// Sky

                mSky = new Scroll;
                mSky->assign(0, new Panel);
                mSky->assign(1, new Panel);
                mSky->assign(2, new Panel);

                mSky->initScroll(game2DVia(game));
                mSky->startScroll(TEXTURE_ID_SKY, true);
            }
            else
                mSky->resumeScroll(TEXTURE_ID_SKY);
            break;
        }
        case 4: { ////// Mountains

            if (!mMountains) {

                mMountains = new Scroll(7);
                for (unsigned char i = 0; i < 7; ++i)
                    mMountains->assign(i, new Panel);

                mMountains->initScroll(game2DVia(game));
                mMountains->startScroll(TEXTURE_ID_MOUNTAIN, true, 0, 7);

                float mounWidth = MOUNTAIN_PANEL_WIDTH / (*mTextures)[mMountains->getTextureIdx()]->width;
                float texCoords[8] = {0};
                texCoords[3] = 1.f;
                texCoords[4] = mounWidth;
                texCoords[5] = 1.f;
                texCoords[6] = texCoords[4];
                panelVia((*mMountains)[0])->setTexCoords(texCoords);
                for (unsigned char i = 1; i < 7; ++i) {

                    texCoords[0] = texCoords[4];
                    texCoords[2] = texCoords[0];
                    texCoords[4] += mounWidth;
                    texCoords[6] = texCoords[4];
                    panelVia((*mMountains)[i])->setTexCoords(texCoords);
                }
            }
            else
                mMountains->resumeScroll(TEXTURE_ID_MOUNTAIN);
            break;
        }
        case 5: { ////// Floor

            if (!mFloor) {

                mFloor = new Scroll;
                mFloor->assign(0, new Panel);
                mFloor->assign(1, new Panel);
                mFloor->assign(2, new Panel);

                mFloor->initScroll(game2DVia(game));
                mFloor->startScroll(TEXTURE_ID_FLOOR, true);

                // Set vertices according texture size and reposition panels at the bottom of the screen
                mFloorY = ((*mTextures)[mFloor->getTextureIdx()]->height * 2.f / \
                        (*mTextures)[mFloor->getTextureIdx()]->width) - 1.f;

                float verticesY[4];
                verticesY[0] = mFloorY;
                verticesY[1] = -1.f;
                verticesY[2] = -1.f;
                verticesY[3] = mFloorY;
                panelVia((*mFloor)[0])->setVerticesH(verticesY);
                panelVia((*mFloor)[1])->setVerticesH(verticesY);
                panelVia((*mFloor)[2])->setVerticesH(verticesY);

                // Define floor bound
                mFloorBound.mBottom = game->getScreen()->bottom + ((FLOOR_HEIGHT * 2.f *
                          game->getScreen()->top) / (*mTextures)[mFloor->getTextureIdx()]->width); // and not '->height'...
                // ...coz floor texture is not a square texture and others reason (check code lines above)

                #define FLOOR_TEXTURE_HEIGHT 128.f
                static const float texCoords[8] = { 0.f, 0.f, 0.f, (FLOOR_TEXTURE_HEIGHT - 1.f) / FLOOR_TEXTURE_HEIGHT,
                    1.f, (FLOOR_TEXTURE_HEIGHT - 1) / FLOOR_TEXTURE_HEIGHT, 1.f, 0.f };
                panelVia((*mFloor)[0])->setTexCoords(texCoords);
                panelVia((*mFloor)[1])->setTexCoords(texCoords);
                panelVia((*mFloor)[2])->setTexCoords(texCoords);
                // Needed to avoid GL_LINEAR issue
            }
            else
                mFloor->resumeScroll(TEXTURE_ID_FLOOR);
            break;
        }
        case 6: { ////// MegaMan

            if (!mMegaMan) {

                mMegaMan = new MegaMan;
                mMegaMan->initialize(game2DVia(game));
                mMegaMan->start(mFloorBound.mBottom);
            }
            else
                mMegaMan->resume(TEXTURE_ID_MEGAMAN); // == Surface2D::start(TEXTURE_ID_MEGAMAN);
            break;
        }
        case 7: { ////// Joypad

            if (!mJoyPad) {

                mJoyPad = new Pad;
                mJoyPad->initialize(game2DVia(game));
                mJoyPad->start(false);

                //
                assert(!game->mGamePad);
                game->mGamePad = new Joypad;

                JoyConfig config;
                std::memset(&config, 0, sizeof(JoyConfig));

                config.up.left = UP_LEFT;
                config.up.top = game->getScreen()->height - UP_TOP;
                config.up.right = UP_RIGHT;
                config.up.bottom = game->getScreen()->height - UP_BOTTOM;

                config.down.left = DOWN_LEFT;
                config.down.top = game->getScreen()->height - DOWN_TOP;
                config.down.right = DOWN_RIGHT;
                config.down.bottom = game->getScreen()->height - DOWN_BOTTOM;

                config.backward.left = BACKWARD_LEFT;
                config.backward.top = game->getScreen()->height - BACKWARD_TOP;
                config.backward.right = BACKWARD_RIGHT;
                config.backward.bottom = game->getScreen()->height - BACKWARD_BOTTOM;

                config.forward.left = FORWARD_LEFT;
                config.forward.top = game->getScreen()->height - FORWARD_TOP;
                config.forward.right = FORWARD_RIGHT;
                config.forward.bottom = game->getScreen()->height - FORWARD_BOTTOM;

                config.buttons[0] = new TouchArea;
                config.buttons[0]->left = game->getScreen()->width - JUMP_LEFT;
                config.buttons[0]->top = game->getScreen()->height - JUMP_TOP;
                config.buttons[0]->right = game->getScreen()->width - JUMP_RIGHT;
                config.buttons[0]->bottom = game->getScreen()->height - JUMP_BOTTOM;

                config.buttons[1] = new TouchArea;
                config.buttons[1]->left = game->getScreen()->width - SHOOT_LEFT;
                config.buttons[1]->top = game->getScreen()->height - SHOOT_TOP;
                config.buttons[1]->right = game->getScreen()->width - SHOOT_RIGHT;
                config.buttons[1]->bottom = game->getScreen()->height - SHOOT_BOTTOM;

                config.buttons[2] = new TouchArea;
                config.buttons[2]->left = game->getScreen()->width - ACTIVATE_LEFT;
                config.buttons[2]->top = game->getScreen()->height - ACTIVATE_TOP;
                config.buttons[2]->right = game->getScreen()->width - ACTIVATE_RIGHT;
                config.buttons[2]->bottom = game->getScreen()->height - ACTIVATE_BOTTOM;

#ifdef DEBUG
                static_cast<Joypad*>(game->mGamePad)->initialize(game, &config, game->getLog());
#else
                static_cast<Joypad*>(game->mGamePad)->initialize(game, &config);
#endif
            }
            else
                mJoyPad->start(true);
            break;
        }
        case 8: {

            if (!mPlatformA) {

                mPlatformA = new Element2D;
                mPlatformA->initialize(game2DVia(game));
                mPlatformA->start(TEXTURE_ID_FLOOR);

                float texCoords[8] = {0};
                getPlatformTexCoords(texCoords);
                mPlatformA->setTexCoords(texCoords);

                mPlatformA->setVertices(game->getScreen()->width,
                        static_cast<short>(PLATFORMA_TOP_RATE * game->getScreen()->height),
                        game->getScreen()->width + PLATFORM_WIDTH,
                        static_cast<short>(PLATFORMA_TOP_RATE * game->getScreen()->height) - PLATFORM_HEIGHT);
                mPlatformA->addBound(new Rectangle2D(mPlatformA->getLeft(), mPlatformA->getRight(), mPlatformA->getTop(),
                        mPlatformA->getBottom()));

                // Set previous 'mTop' bound into 'mOverlapY' (needed to check collision in 'MegaMan::checkCollision' method)
                rect2DVia(mPlatformA->getBounds(0))->mOverlapY = rect2DVia(mPlatformA->getBounds(0))->mTop;

                mBoundLeft.mLeft = 2.f * game->getScreen()->left;
                mBoundRight.mRight = 2.f * game->getScreen()->right;
            }
            else
                mPlatformA->resume(TEXTURE_ID_FLOOR);
            break;
        }
        case 9: {

            if (!mPlatformB) {

                mPlatformB = new Element2D;
                mPlatformB->initialize(game2DVia(game));
                mPlatformB->start(TEXTURE_ID_FLOOR);

                float texCoords[8] = {0};
                getPlatformTexCoords(texCoords);
                mPlatformB->setTexCoords(texCoords);

                mPlatformB->setVertices(game->getScreen()->width + PLATFORM_WIDTH,
                        static_cast<short>(PLATFORMB_TOP_RATE * game->getScreen()->height),
                        game->getScreen()->width + (2 * PLATFORM_WIDTH),
                        static_cast<short>(PLATFORMB_TOP_RATE * game->getScreen()->height) - PLATFORM_HEIGHT);
                mPlatformB->addBound(new Rectangle2D(mPlatformB->getLeft(), mPlatformB->getRight(),
                        mPlatformB->getTop(), mPlatformB->getBottom()));

                // Set previous 'mTop' bound into 'mOverlapY' (needed to check collision in 'MegaMan::checkCollision' method)
                rect2DVia(mPlatformB->getBounds(0))->mOverlapY = rect2DVia(mPlatformB->getBounds(0))->mTop;
            }
            else
                mPlatformB->resume(TEXTURE_ID_FLOOR);
            break;
        }
        case 10: {

            // Keep sound(s) load order
            Player::loadSound(SOUND_ID_JUMP);
            break;
        }
        case 11: {

            if (!mPlatformC) {

                mPlatformC = new Element2D;
                mPlatformC->initialize(game2DVia(game));
                mPlatformC->start(TEXTURE_ID_FLOOR);

                float texCoords[8] = {0};
                getPlatformTexCoords(texCoords);
                mPlatformC->setTexCoords(texCoords);

                mPlatformC->setVertices(game->getScreen()->width - PLATFORM_WIDTH,
                        static_cast<short>(PLATFORMC_TOP_RATE * game->getScreen()->height), game->getScreen()->width,
                        static_cast<short>(PLATFORMC_TOP_RATE * game->getScreen()->height) - PLATFORM_HEIGHT);
                mPlatformC->addBound(new Rectangle2D(mPlatformC->getLeft(), mPlatformC->getRight(),
                        mPlatformC->getTop(), mPlatformC->getBottom()));

                // Set previous 'mTop' bound into 'mOverlapY' (needed to check collision in 'MegaMan::checkCollision' method)
                rect2DVia(mPlatformC->getBounds(0))->mOverlapY = rect2DVia(mPlatformC->getBounds(0))->mTop;
            }
            else
                mPlatformC->resume(TEXTURE_ID_FLOOR);
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

void LevelMega::moveWorld(float y) {

    LOGV(LOG_LEVEL_LEVELMEGA, 0, LOG_FORMAT(" - y:%f"), __PRETTY_FUNCTION__, __LINE__, y);

    mMountains->scrollY(y * MOUNTAIN_DEPTH);
    mFloor->scrollY(y);

    // Set previous 'mTop' bound into 'mOverlapY' (needed to check collision in 'MegaMan::checkCollision' method)
    rect2DVia(mPlatformA->getBounds(0))->mOverlapY = rect2DVia(mPlatformA->getBounds(0))->mTop;
    mPlatformA->translate(0.f, y);
    rect2DVia(mPlatformB->getBounds(0))->mOverlapY = rect2DVia(mPlatformB->getBounds(0))->mTop;
    mPlatformB->translate(0.f, y);
    rect2DVia(mPlatformC->getBounds(0))->mOverlapY = rect2DVia(mPlatformC->getBounds(0))->mTop;
    mPlatformC->translate(0.f, y);
}

bool LevelMega::update(const Game* game) {

    ////// Update
    const Joypad::Data* joypadData = joyDataVia(joypadVia(game->mGamePad)->getData());
    if (joypadData->buttonPushed[2]) {

        delete game->mGamePad;
        game->mGamePad = NULL;

        game->setGameLevel(1); // Menu level
        return false;
    }
    switch (joypadData->direction) {
        case Joypad::JPD_FORWARD: {

            mSky->scrollX(SCROLL_SPEED * SKY_DEPTH);
            mMountains->scrollX(SCROLL_SPEED * MOUNTAIN_DEPTH);
            mFloor->scrollX(SCROLL_SPEED);

            mPlatformA->translate(SCROLL_SPEED, 0.f);
            mPlatformB->translate(SCROLL_SPEED, 0.f);
            mPlatformC->translate(SCROLL_SPEED, 0.f);
            break;
        }
        case Joypad::JPD_BACKWARD: {

            mSky->scrollX(-SCROLL_SPEED * SKY_DEPTH);
            mMountains->scrollX(-SCROLL_SPEED * MOUNTAIN_DEPTH);
            mFloor->scrollX(-SCROLL_SPEED);

            mPlatformA->translate(-SCROLL_SPEED, 0.f);
            mPlatformB->translate(-SCROLL_SPEED, 0.f);
            mPlatformC->translate(-SCROLL_SPEED, 0.f);
            break;
        }
    }
    mMegaMan->update(game, this);

    // Check if needed to move world (vertically)
    static bool scrollWorld = false; // Can use 'static' coz when start 'mMegaMan->isScrollWorld' is always false
    if (mMegaMan->isScrollWorld()) {

        if ((mMegaMan->isFalling()) || (mMegaMan->isJumping()))
            moveWorld(mMegaMan->getScrollWorld());
        //else  // MegaMan is on a platform over the floor

        scrollWorld = true;
        // => Over the floor (need to scroll the world vertically)
    }
    else // => Near the floor (no need to scroll the world vertically)
    if (scrollWorld) {

        float verticesY[] = { 1.f, -1.f, -1.f, 1.f };
        panelVia((*mMountains)[0])->setVerticesH(verticesY);
        panelVia((*mMountains)[1])->setVerticesH(verticesY);
        panelVia((*mMountains)[2])->setVerticesH(verticesY);

        verticesY[0] = verticesY[3] = mFloorY;
        panelVia((*mFloor)[0])->setVerticesH(verticesY);
        panelVia((*mFloor)[1])->setVerticesH(verticesY);
        panelVia((*mFloor)[2])->setVerticesH(verticesY);

        mPlatformA->resetY();
        mPlatformB->resetY();
        mPlatformC->resetY();

        scrollWorld = false;
        // No more vertically world scroll
    }

    ////// Check collisions
    if (!mMegaMan->checkCollision(&mFloorBound, BOUND_ID_FLOOR)) { // MegaMan should be more on the floor than on a platform

        if (mMegaMan->checkCollision(mPlatformA, OBJECT_ID_PLATFORMA)) {
            if (rect2DVia(mPlatformA->getBounds(0))->mTop > mMegaMan->getOverFloor())
                moveWorld(-rect2DVia(mPlatformA->getBounds(0))->mTop);
        }
        else
        if (mMegaMan->checkCollision(mPlatformB, OBJECT_ID_PLATFORMB)) {
            if (rect2DVia(mPlatformB->getBounds(0))->mTop > mMegaMan->getOverFloor())
                moveWorld(mMegaMan->getOverFloor() - rect2DVia(mPlatformB->getBounds(0))->mTop);
        }
        else
        if (mMegaMan->checkCollision(mPlatformC, OBJECT_ID_PLATFORMC)) {
            if (rect2DVia(mPlatformC->getBounds(0))->mTop > mMegaMan->getOverFloor())
                moveWorld(mMegaMan->getOverFloor() - rect2DVia(mPlatformC->getBounds(0))->mTop);
        }
    }

    if (mBoundLeft.overlap(rect2DVia(mPlatformA->getBounds(0))))
        mPlatformA->reset(); // Best performance here (when MegaMan goes forward)!
    else if (mBoundRight.overlap(rect2DVia(mPlatformA->getBounds(0))))
        mPlatformA->translate(3.f * game->getScreen()->left, 0.f);

    static float platformW = (PLATFORM_WIDTH * game->getScreen()->ratioW);
    mBoundLeft.mLeft += platformW;
    mBoundRight.mRight -= platformW;
    if (mBoundLeft.overlap(rect2DVia(mPlatformB->getBounds(0))))
        mPlatformB->reset(); // Best performance here (when MegaMan goes forward)!
    else if (mBoundRight.overlap(rect2DVia(mPlatformB->getBounds(0))))
        mPlatformB->translate(3.f * game->getScreen()->left, 0.f);
    mBoundLeft.mLeft -= platformW;
    mBoundRight.mRight += platformW;

    if (mBoundLeft.overlap(rect2DVia(mPlatformC->getBounds(0)))) {
        mPlatformC->reset();
        mPlatformC->translate(platformW, 0.f);
    }
    else if (mBoundRight.overlap(rect2DVia(mPlatformC->getBounds(0))))
        mPlatformC->translate(3.f * game->getScreen()->left, 0.f);
    return true;
}

void LevelMega::renderLoad() const {

    glDisable(GL_BLEND);
    mProgress->render(false);
    glEnable(GL_BLEND);
}
void LevelMega::renderUpdate() const {

    glDisable(GL_BLEND);
    mSky->renderScroll(false);
    glEnable(GL_BLEND);
    mMountains->renderScroll(false);

    glDisable(GL_BLEND);
    mPlatformA->render(true);
    mPlatformB->render(false);
    mPlatformC->render(false);
    glEnable(GL_BLEND);

    mMegaMan->render(true);
    mFloor->renderScroll(true);
    mScore->render(false);

    mJoyPad->render();
}
