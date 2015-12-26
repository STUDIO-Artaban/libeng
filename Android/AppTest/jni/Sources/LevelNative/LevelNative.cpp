#include "LevelNative.h"
#include "Global.h"

#ifndef __ANDROID__
#include "OpenGLES/ES2/gl.h"
#endif

#include <libeng/Log/Log.h>
#include <libeng/Game/2D/Game2D.h>
#include <libeng/Graphic/Bounds/2D/Overlap2D.h>
#include <libeng/Tools/Tools.h>
#include <boost/format.hpp>

#define MAX_LOAD_STEP               15
#define SPHERE_TRANS_VEL            0.01f
#define SPHERE_SCALE_VEL            0.007f
#define ANTIALIASING_ROTATE_VEL     -0.038f
#ifdef __ANDROID__
#define SPHERE2_TRANS_VEL           0.009f
#else
#define SPHERE2_TRANS_VEL           0.028f
#endif
#define MAX_VOLUME_DIST             200.0f

#define PLAY_HLBP                   48
#define PAUSE_HLBP                  84
#define STOP_HLBP                   128
#define PANELS_HEIGHT               32
#define PANELS_RATIO                2

// Sound IDs
#define SOUND_ID_PAINO              2
#define SOUND_ID_ROBOT              3
#define SOUND_ID_CHIP               4

// Texture IDs
#define TEXTURE_ID_BACK             7
#define TEXTURE_ID_CONTROLS         9
#define TEXTURE_ID_TOUCH            10
#define TEXTURE_ID_SPHERE           11

//////
LevelNative::LevelNative(Game* game) : Level(game), mSphereBottom(-0.9f), mSphereTop(0.9f), mSphereRight(0.f),
    mSphereLeft(0.f), mPanelWidth(0), mPlayTop(0), mPauseTop(0), mStopTop(0), mSphereCheckTop(true),
    mSphereCheckRight(true), mPrevVolDist(0.f), mVolTouchId1(0), mVolTouchId2(0) {

    LOGV(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    ////// Inputs use & precision
    game->mAccelPrecision = 0.f;
    //mTouchPrecision = DEFAULT_TOUCH_PRECISION;
    Inputs::InputUse = (USE_INPUT_TOUCH | USE_INPUT_ACCEL);

    mScreenLeft = NULL;
    mScreenRight = NULL;
    mScreenTop = NULL;
    mScreenBottom = NULL;

    mProgress = NULL;
    mBackground = NULL;
    mSoundPanel = NULL;
    mText1 = NULL;
    mText2 = NULL;
    mText3 = NULL;
    mSphere1 = NULL;
    mSphere2 = NULL;
    mSphere3 = NULL;
    mAntialiasing = NULL;
}
LevelNative::~LevelNative() {

    LOGV(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mScreenLeft) delete mScreenLeft;
    if (mScreenRight) delete mScreenRight;
    if (mScreenTop) delete mScreenTop;
    if (mScreenBottom) delete mScreenBottom;

    if (mProgress) delete mProgress;
    if (mBackground) delete mBackground;
    if (mSoundPanel) delete mSoundPanel;
    if (mText1) delete mText1;
    if (mText2) delete mText2;
    if (mText3) delete mText3;
    if (mSphere1) delete mSphere1;
    if (mSphere2) delete mSphere2;
    if (mSphere3) delete mSphere3;
    if (mAntialiasing) delete mAntialiasing;

    if (mChips.size()) {

        for (unsigned char i = 0; i < static_cast<unsigned char>(mChips.size()); ++i)
            delete mChips[i];
        mTextures->delTexture(6);
        mChips.clear();
    }
}

void LevelNative::pause() {

    LOGV(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if (mProgress) mProgress->pause();
    if (mBackground) mBackground->pause();
    if (mSoundPanel) mSoundPanel->pause();
    if (mText1) mText1->pause();
    if (mText2) mText2->pause();
    if (mText3) mText3->pause();
    if (mSphere1) mSphere1->pause();
    if (mSphere2) mSphere2->pause();
    if (mSphere3) mSphere3->pause();
    if (mAntialiasing) mAntialiasing->pause();

    for (unsigned char i = 0; i < static_cast<unsigned char>(mChips.size()); ++i)
        mChips[i]->pause();
}

bool LevelNative::loaded(const Game* game) {

    LOGV(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - l:%d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
    switch (mLoadStep) {
        case 1: {

            // Load font texture first (needed for 'Text2D' objects below)
            Textures::loadTexture(TEXTURE_ID_FONT);

            if (!mProgress) {

                mProgress = new Progress;
                mProgress->initialize(game2DVia(game));

                Progress::Info progInfo;
                progInfo.maxPos = MAX_LOAD_STEP - 1;

                progInfo.doneRed = 0x00;
                progInfo.doneGreen = 0x00;
                progInfo.doneBlue = 0xff;

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

            if (!mBackground) {

                mBackground = new Static2D;
                mBackground->initialize(game2DVia(game));
                mBackground->start(TEXTURE_ID_BACK);

                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
                assert(mTextures->getIndex(TEXTURE_ID_BACK) == 2);

                float texCoords[8] = {0};
                texCoords[3] = 320.f / (*mTextures)[2]->height;
                texCoords[4] = 480.f / (*mTextures)[2]->width;
                texCoords[5] = texCoords[3];
                texCoords[6] = texCoords[4];
                mBackground->setTexCoords(texCoords);
                mBackground->setVertices(0, game->getScreen()->height, game->getScreen()->width, 0);
            }
            else
                mBackground->resume(TEXTURE_ID_BACK);
            break;
        }
        case 3: {

            if (!mSoundPanel) {

                mSoundPanel = new Static2D;
                mSoundPanel->initialize(game2DVia(game));
                mSoundPanel->start(TEXTURE_ID_CONTROLS);

                float texCoords[8] = {0};
                texCoords[3] = 1.f;
                texCoords[4] = 2.f;
                texCoords[5] = 1.f;
                texCoords[6] = 2.f;
                mSoundPanel->setTexCoords(texCoords);

                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
                assert(mTextures->getIndex(TEXTURE_ID_CONTROLS) == 3);
                mSoundPanel->setVertices(0, (*mTextures)[3]->height * PANELS_RATIO,
                        (*mTextures)[3]->width * 2 * PANELS_RATIO, 0);

#ifndef LIBENG_PORT_AS_LAND
                mPlayTop = game->getScreen()->height - (PLAY_HLBP * PANELS_RATIO);
                mPauseTop = game->getScreen()->height - (PAUSE_HLBP * PANELS_RATIO);
                mStopTop = game->getScreen()->height - (STOP_HLBP * PANELS_RATIO);

#else
                mPlayTop = PLAY_HLBP * PANELS_RATIO;
                mPauseTop = PAUSE_HLBP * PANELS_RATIO;
                mStopTop = STOP_HLBP * PANELS_RATIO;
#endif
                mPanelWidth = PANELS_HEIGHT * PANELS_RATIO;
            }
            else
                mSoundPanel->resume(TEXTURE_ID_CONTROLS);
            break;
        }
        case 4: {

            if (!mText1) {

                mText1 = new Text2D;
                mText1->initialize(game2DVia(game));
                mText1->start(L"abcdefghijklmnopqrstuvwxyz&àâçéèêûô\nABCDEFGHIJKLMNOPQRSTUVWXYZ€>}]*#ïîö\n"
                        "0123456789 '\"-,.:!?()/@=+$%<{[;ëùü");
                mText1->scale(0.8f, 0.8f);
                mText1->position(game->getScreen()->left, game->getScreen()->top);
            }
            else
                mText1->resume();
            break;
        }
        case 5: {

            if (!mSphere1) {

                mSphere1 = new Element2D;
                mSphere1->initialize(game2DVia(game));
                mSphere1->start(TEXTURE_ID_SPHERE);
                mSphere1->setTexCoords(FULL_TEXCOORD_BUFFER);

                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
                assert(mTextures->getIndex(TEXTURE_ID_SPHERE) == 4);
                mSphere1->setVertices((game->getScreen()->width >> 1) - ((*mTextures)[4]->width >> 1),
                        (game->getScreen()->height >> 1) + ((*mTextures)[4]->height >> 1),
                        (game->getScreen()->width >> 1) + ((*mTextures)[4]->width >> 1),
                        (game->getScreen()->height >> 1) - ((*mTextures)[4]->height >> 1));

                float radius = (mSphere1->getRight() - mSphere1->getLeft()) / 2.f;
                mSphere1->addBound(new Circle2D(mSphere1->getLeft() + radius, mSphere1->getBottom() + radius, radius));

                mSphere1->translate(mText1->getRight() + (((*mTextures)[4]->width >> 1) * game->getScreen()->ratioW), 0.f);

                mSphere1->addVelocities(2);
                mSphere1->getVelocities()[0] = SPHERE_TRANS_VEL;
                mSphere1->getVelocities()[1] = -SPHERE_SCALE_VEL;

                mSphereCheckTop = true; // Start with a positive translation velocity
            }
            else
                mSphere1->resume(TEXTURE_ID_SPHERE);
            break;
        }
        case 6: {

            if (!mAntialiasing) {

                mAntialiasing = new Element2D;
                mAntialiasing->initialize(game2DVia(game));
                mAntialiasing->start(TEXTURE_ID_ANTIALIASING);
                mAntialiasing->setTexCoords(FULL_TEXCOORD_BUFFER);

                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
                assert(mTextures->getIndex(TEXTURE_ID_ANTIALIASING) == 5);
                mAntialiasing->setVertices((game->getScreen()->width >> 1) - ((*mTextures)[5]->width >> 1),
                        (game->getScreen()->height >> 1) + ((*mTextures)[5]->height >> 1),
                        (game->getScreen()->width >> 1) + ((*mTextures)[5]->width >> 1),
                        (game->getScreen()->height >> 1) - ((*mTextures)[5]->height >> 1));

                mAntialiasing->translate(game->getScreen()->right / 2.f, game->getScreen()->top / 2.f);

                mAntialiasing->setAlpha(0.f);
            }
            else
                mAntialiasing->resume(TEXTURE_ID_ANTIALIASING);
            break;
        }
        case 7: {

            if (!mSphere2) {

                mSphere2 = new Element2D;
                mSphere2->initialize(game2DVia(game));
                mSphere2->start(TEXTURE_ID_SPHERE);
                mSphere2->setTexCoords(FULL_TEXCOORD_BUFFER);

                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
                assert(mTextures->getIndex(TEXTURE_ID_SPHERE) == 4);
                mSphere2->setVertices((game->getScreen()->width >> 1) - ((*mTextures)[4]->width >> 1),
                        (game->getScreen()->height >> 1) + ((*mTextures)[4]->height >> 1),
                        (game->getScreen()->width >> 1) + ((*mTextures)[4]->width >> 1),
                        (game->getScreen()->height >> 1) - ((*mTextures)[4]->height >> 1));

                float radius = (mSphere2->getRight() - mSphere2->getLeft()) / 2.f;
                mSphere2->addBound(new Circle2D(mSphere2->getLeft() + radius, mSphere2->getBottom() + radius, radius));

                mSphere2->addVelocities(1);
                mSphere2->getVelocities()[0] = 2.f * radius; // Diameter

                mScreenLeft = new BorderLeft(game->getScreen()->left);
                mScreenRight = new BorderRight(game->getScreen()->right);
                mScreenTop = new BorderTop(game->getScreen()->top);
                mScreenBottom = new BorderBottom(game->getScreen()->bottom);
            }
            else
                mSphere2->resume(TEXTURE_ID_SPHERE);
            break;
        }
        case 8: {

            if (!mText2) {

                mText2 = new Text2D;
                mText2->initialize(game2DVia(game));
                mText2->start(L"0000");
                mText2->position(game->getScreen()->right - (mText2->getRight() - mText2->getLeft()),
                        game->getScreen()->bottom + mText2->getTop());
            }
            else
                mText2->resume();
            break;
        }
        case 9: Player::loadSound(SOUND_ID_PAINO); break;
        case 10: Player::loadSound(SOUND_ID_ROBOT); break;
        case 11: Player::loadSound(SOUND_ID_CHIP); break;
        // WARNING: Keep sound(s) load order! Sound entries still exist when resume!

        case 12: {

            // Manage touch texture
            unsigned char textureIdx = Textures::loadTexture(TEXTURE_ID_TOUCH);

            LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
            assert(textureIdx == 6);
            mTextures->genTexture(6);

            for (unsigned char i = 0; i < static_cast<unsigned char>(mChips.size()); ++i)
                mChips[i]->resume(6);

            break;
        }
        case 13: {

            if (!mSphere3) {

                mSphere3 = new Element2D;
                mSphere3->initialize(game2DVia(game));
                mSphere3->start(TEXTURE_ID_SPHERE);
                mSphere3->setTexCoords(FULL_TEXCOORD_BUFFER);

                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
                assert(mTextures->getIndex(TEXTURE_ID_SPHERE) == 4);
                mSphere3->setVertices((game->getScreen()->width >> 1) - ((*mTextures)[4]->width >> 1),
                        (game->getScreen()->height >> 1) + ((*mTextures)[4]->height >> 1),
                        (game->getScreen()->width >> 1) + ((*mTextures)[4]->width >> 1),
                        (game->getScreen()->height >> 1) - ((*mTextures)[4]->height >> 1));

                float radius = (mSphere3->getRight() - mSphere3->getLeft()) / 2.f;
                mSphere3->addBound(new Circle2D(mSphere3->getLeft() + radius, mSphere3->getBottom() + radius, radius));

                float panelDist = mSphere3->getBottom() - mSoundPanel->getTop();
                if (panelDist > 0.f)
                    mSphere3->translate(0.f, panelDist);

                mSphere3->addVelocities(1);
                mSphere3->getVelocities()[0] = SPHERE_TRANS_VEL;
                //mSphere3->getVelocities()[1] = -SPHERE_SCALE_VEL; // Use 'mSphere1' scale velocity

                mSphereRight.mRight = game->getScreen()->right - 0.1f;
                mSphereLeft.mLeft = game->getScreen()->left + 0.1f;

                mSphereCheckRight = true; // Start with a positive translation velocity
            }
            else
                mSphere3->resume(TEXTURE_ID_SPHERE);
            break;
        }
        case 14: {

            if (!mText3) {

                mText3 = new Text2D;
                mText3->initialize(game2DVia(game));
                mText3->start(boost::str(boost::wformat(L"%f") % Player::getInstance()->getVolume(0)));
                mText3->position(mSoundPanel->getRight(), game->getScreen()->bottom + mText3->getTop());
            }
            else
                mText3->resume();
            break;
        }
        case MAX_LOAD_STEP: { // Done

            // Repeat option on track #2
            Player::getInstance()->setRepeat(1);

            delete mProgress;
            mProgress = NULL;
            break;
        }
#ifdef DEBUG
        default: {

            LOGD(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - Unknown load step: %d"), __PRETTY_FUNCTION__, __LINE__,
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
bool LevelNative::update(const Game* game) {

    ////// Update
    if (game->mTouchCount == 2) { // Change volume

        static float convertVolDist = ((game->getScreen()->width * game->getScreen()->width) +
                (game->getScreen()->height * game->getScreen()->height)) / MAX_VOLUME_DIST;

        char touchIdx1 = LIBENG_NO_DATA;
        char touchIdx2 = LIBENG_NO_DATA;
        if (mPrevVolDist > 0.f) {

            for (unsigned char i = 0; i < 2; ++i) {
                if ((game->mTouchData[i].Id == mVolTouchId1) && (game->mTouchData[i].Type != TouchInput::TOUCH_UP))
                    touchIdx1 = static_cast<char>(i);
                if ((game->mTouchData[i].Id == mVolTouchId2) && (game->mTouchData[i].Type != TouchInput::TOUCH_UP))
                    touchIdx2 = static_cast<char>(i);
            }
        }
        else {

            if (game->mTouchData[0].Type != TouchInput::TOUCH_UP) {
                mVolTouchId1 = game->mTouchData[0].Id;
                touchIdx1 = 0;
            }
            if (game->mTouchData[1].Type != TouchInput::TOUCH_UP) {
                mVolTouchId2 = game->mTouchData[1].Id;
                touchIdx2 = 1;
            }
        }

        // Check indexes
        if ((touchIdx1 != LIBENG_NO_DATA) && (touchIdx2 != LIBENG_NO_DATA)) {

            short xDiff = game->mTouchData[touchIdx1].X - game->mTouchData[touchIdx2].X;
            short yDiff = game->mTouchData[touchIdx1].Y - game->mTouchData[touchIdx2].Y;
            if (mPrevVolDist > 0.f) {

                float newVolDist = static_cast<float>((xDiff * xDiff) + (yDiff * yDiff));
                if (std::abs<float>(newVolDist - mPrevVolDist) > convertVolDist) {

                    float newVolume = (((newVolDist - mPrevVolDist) / convertVolDist) * 0.01f) +
                            Player::getInstance()->getVolume(0);
                    mPrevVolDist = newVolDist;
                    if ((newVolume > 0.05f) && (newVolume < 1.f)) {

                        // Change volume of all tracks
                        LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - Set volume: %f"), __PRETTY_FUNCTION__, __LINE__,
                                newVolume);
                        Player* player = Player::getInstance();
                        player->setVolume(0, newVolume);
                        player->setVolume(1, newVolume);
                        player->setVolume(2, newVolume);

                        mText3->update(boost::str(boost::wformat(L"%f") % player->getVolume(0)));
                    }
                }
            }
            else
                mPrevVolDist = static_cast<float>((xDiff * xDiff) + (yDiff * yDiff));
        }
    }
    else if ((mPrevVolDist > 0.f) && (!game->mTouchCount))
        mPrevVolDist = 0.f;
    // WARNING: Low performance!

    unsigned char touchCount = game->mTouchCount;
    while ((touchCount--) && (!(*(reinterpret_cast<int*>(&mPrevVolDist))))) {

        if (game->mTouchData[touchCount].Type == TouchInput::TOUCH_UP) {

#ifndef LIBENG_PORT_AS_LAND
            if ((game->mTouchData[touchCount].Y > mPlayTop) &&
                (game->mTouchData[touchCount].X < mPanelWidth)) { // Play #1
#else
            if ((game->mTouchData[touchCount].X < mPlayTop) &&
                (game->mTouchData[touchCount].Y < mPanelWidth)) { // Play #1
#endif
                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - Play #1"), __PRETTY_FUNCTION__, __LINE__);
                Player::getInstance()->play(0);
            }
#ifndef LIBENG_PORT_AS_LAND
            else if ((game->mTouchData[touchCount].Y > mPauseTop) &&
                      (game->mTouchData[touchCount].Y < mPlayTop) &&
                      (game->mTouchData[touchCount].X < mPanelWidth)) { // Pause #1
#else
            else if ((game->mTouchData[touchCount].X < mPauseTop) &&
                      (game->mTouchData[touchCount].X > mPlayTop) &&
                      (game->mTouchData[touchCount].Y < mPanelWidth)) { // Pause #1
#endif
                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - Pause #1"), __PRETTY_FUNCTION__, __LINE__);
                Player::getInstance()->pause(0);
            }
#ifndef LIBENG_PORT_AS_LAND
            else if ((game->mTouchData[touchCount].Y > mStopTop) &&
                      (game->mTouchData[touchCount].Y < mPauseTop) &&
                      (game->mTouchData[touchCount].X < mPanelWidth)) { // Stop #1
#else
            else if ((game->mTouchData[touchCount].X < mStopTop) &&
                      (game->mTouchData[touchCount].X > mPauseTop) &&
                      (game->mTouchData[touchCount].Y < mPanelWidth)) { // Stop #1
#endif
                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - Stop #1"), __PRETTY_FUNCTION__, __LINE__);
                Player::getInstance()->stop(0);
            }
#ifndef LIBENG_PORT_AS_LAND
            else if ((game->mTouchData[touchCount].Y > mPlayTop) &&
                      (game->mTouchData[touchCount].X < (mPanelWidth * 2)) &&
                      (game->mTouchData[touchCount].X > mPanelWidth)) { // Play #2
#else
            else if ((game->mTouchData[touchCount].X < mPlayTop) &&
                      (game->mTouchData[touchCount].Y < (mPanelWidth * 2)) &&
                      (game->mTouchData[touchCount].Y > mPanelWidth)) { // Play #2
#endif
                Player* player = Player::getInstance();
                if ((player->getStatus(1) == AL_INITIAL) || (player->getStatus(1) == AL_PAUSED)) {

                    LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - Play #2"), __PRETTY_FUNCTION__, __LINE__);
                    player->play(1);
                }
                //else // Replay sound from the beginning is not allowed with a repeat option!
            }
#ifndef LIBENG_PORT_AS_LAND
            else if ((game->mTouchData[touchCount].Y > mPauseTop) &&
                      (game->mTouchData[touchCount].Y < mPlayTop) &&
                      (game->mTouchData[touchCount].X < (mPanelWidth * 2)) &&
                      (game->mTouchData[touchCount].X > mPanelWidth)) { // Pause #2
#else
            else if ((game->mTouchData[touchCount].X < mPauseTop) &&
                      (game->mTouchData[touchCount].X > mPlayTop) &&
                      (game->mTouchData[touchCount].Y < (mPanelWidth * 2)) &&
                      (game->mTouchData[touchCount].Y > mPanelWidth)) { // Pause #2
#endif
                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(" - Pause #2"), __PRETTY_FUNCTION__, __LINE__);
                Player::getInstance()->pause(1);
            }
#ifndef LIBENG_PORT_AS_LAND
            else if ((game->mTouchData[touchCount].Y > mStopTop) &&
                      (game->mTouchData[touchCount].Y < mPauseTop) &&
                      (game->mTouchData[touchCount].X < (mPanelWidth * 2)) &&
                      (game->mTouchData[touchCount].X > mPanelWidth)) { // Stop #2
#else
            else if ((game->mTouchData[touchCount].X < mStopTop) &&
                      (game->mTouchData[touchCount].X > mPauseTop) &&
                      (game->mTouchData[touchCount].Y < (mPanelWidth * 2)) &&
                      (game->mTouchData[touchCount].Y > mPanelWidth)) { // Stop #2
#endif
                //mPlayer->stop(1);
                // -> Do not stop a sound with repeat option (orelse repeat option will be disabled)

                // ...instead exit
                game->setGameLevel(1); // Menu level
                return false;
            }
            else {

                Static2D* chip = new Static2D(false);
                chip->initialize(game2DVia(game));

                LOGI(LOG_LEVEL_LEVELNATIVE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
                assert(mTextures->getIndex(TEXTURE_ID_TOUCH) == 6);
                chip->start(6);
                chip->setTexCoords(FULL_TEXCOORD_BUFFER);

                short halfTexSize = (*mTextures)[6]->width >> 1; // width == height (Sphere)
#ifndef LIBENG_PORT_AS_LAND
                chip->setVertices(game->mTouchData[touchCount].X - halfTexSize, game->getScreen()->height - game->mTouchData[touchCount].Y + halfTexSize,
                        game->mTouchData[touchCount].X + halfTexSize, game->getScreen()->height - game->mTouchData[touchCount].Y - halfTexSize);
#else
                chip->setVertices(game->mTouchData[touchCount].Y - halfTexSize, game->mTouchData[touchCount].X + halfTexSize,
                        game->mTouchData[touchCount].Y + halfTexSize, game->mTouchData[touchCount].X - halfTexSize);
#endif
                mChips.push_back(chip);
                Player::getInstance()->play(2);
                mText2->update(boost::str(boost::wformat(L"%04d") % static_cast<short>(mChips.size())));
            }
        }
    }

    mSphere1->translate(0.f, mSphere1->getVelocities()[0]);
    mSphere3->translate(mSphere3->getVelocities()[0], 0.f);

    static float sphereScale = 1.f;
    sphereScale += mSphere1->getVelocities()[1];
    if (sphereScale < 0.5f) {

        sphereScale = 0.5f;
        mSphere1->getVelocities()[1] = SPHERE_SCALE_VEL;
    }
    else if (sphereScale > 1.f) {

        sphereScale = 1.f;
        mSphere1->getVelocities()[1] = -SPHERE_SCALE_VEL;
    }
    mSphere1->scale(sphereScale, sphereScale);
    mSphere3->scale(sphereScale, sphereScale);

#ifndef LIBENG_PORT_AS_LAND
    static float angle = 0.f; // 0 radian
    static float roundPi = PI_F * -2.f; // -2Pi radian
#else
    static float angle = PI_F / -2.f; // -Pi/2 radian
    static float roundPi = (PI_F * -5.f) / 2.f; // -5Pi/2 radian
#endif
    angle += ANTIALIASING_ROTATE_VEL;
    if (angle < roundPi)
#ifndef LIBENG_PORT_AS_LAND
        angle = 0.f; // 0 radian
#else
        angle = PI_F / -2.f; // -Pi/2 radian
#endif
    mAntialiasing->rotate(angle);
    if (mAntialiasing->getAlpha() < 1.f)
        mAntialiasing->setAlpha(mAntialiasing->getAlpha() + 0.002f);

    if (game->mAccelData)
#ifndef LIBENG_PORT_AS_LAND // iOS
        mSphere2->translate(game->mAccelData->X * -SPHERE2_TRANS_VEL, game->mAccelData->Y * SPHERE2_TRANS_VEL);
#else // Android
        mSphere2->translate(game->mAccelData->X * SPHERE2_TRANS_VEL, game->mAccelData->Y * -SPHERE2_TRANS_VEL);
#endif

    ////// Check collisions
    if (mSphereCheckTop) {
        if (mSphereTop.overlap(circ2DVia(mSphere1->getBounds(0)))) {

            mSphere1->getVelocities()[0] = -SPHERE_TRANS_VEL;
            mSphereCheckTop = false;
        }
    }
    else if (mSphereBottom.overlap(circ2DVia(mSphere1->getBounds(0)))) {

        mSphere1->getVelocities()[0] = SPHERE_TRANS_VEL;
        mSphereCheckTop = true;
    }

    if (mSphereCheckRight) {
        if (mSphereRight.overlap(circ2DVia(mSphere3->getBounds(0)))) {

            mSphere3->getVelocities()[0] = -SPHERE_TRANS_VEL;
            mSphereCheckRight = false;
        }
    }
    else if (mSphereLeft.overlap(circ2DVia(mSphere3->getBounds(0)))) {

        mSphere3->getVelocities()[0] = SPHERE_TRANS_VEL;
        mSphereCheckRight = true;
    }

    static float right2left = game->getScreen()->left - game->getScreen()->right - mSphere2->getVelocities()[0];
    if (circ2DVia(mSphere2->getBounds(0))->overlapDist(mScreenRight) > mSphere2->getVelocities()[0])
        mSphere2->translate(right2left, 0.f);
    else if (circ2DVia(mSphere2->getBounds(0))->overlapDist(mScreenLeft) > mSphere2->getVelocities()[0])
        mSphere2->translate(-right2left, 0.f);

    static float top2bottom = game->getScreen()->bottom - game->getScreen()->top - mSphere2->getVelocities()[0];
    if (circ2DVia(mSphere2->getBounds(0))->overlapDist(mScreenTop) > mSphere2->getVelocities()[0])
        mSphere2->translate(0.f, top2bottom);
    else if (circ2DVia(mSphere2->getBounds(0))->overlapDist(mScreenBottom) > mSphere2->getVelocities()[0])
        mSphere2->translate(0.f, -top2bottom);

    return true;
}

void LevelNative::renderLoad() const {

    glDisable(GL_BLEND);
    mProgress->render(false);
    glEnable(GL_BLEND);
}
void LevelNative::renderUpdate() const {

    glDisable(GL_BLEND);
    mBackground->render(false);
    glEnable(GL_BLEND);

    for (unsigned char i = 0; i < static_cast<unsigned char>(mChips.size()); ++i)
        mChips[i]->render(false);

    mText1->render(false);
    mText2->render(false);
    mText3->render(false);
    mSphere1->render(true);
    mSphere3->render(true);
    mAntialiasing->render(true);

    mSphere2->render(true);
    mSoundPanel->render(true);
}
