#include "LevelCam.h"
#include "Global.h"

#ifdef __ANDROID__
#include "Sources/LevelNative/LevelNative.h"
#else
#include "OpenGLES/ES2/gl.h"
#include "LevelNative.h"
#endif

#include <libeng/Game/2D/Game2D.h>

//////
LevelCam::LevelCam(Game* game) : Level(game) {

    LOGV(LOG_LEVEL_LEVELCAM, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    ////// Inputs use & precision
    //mTouchPrecision = DEFAULT_TOUCH_PRECISION;
    Inputs::InputUse = USE_INPUT_TOUCH;

    mCamera = Camera::getInstance();

    mBackCam1 = NULL;
    mBackCam2 = NULL;
    mBackCam3 = NULL;
    mBackCam4 = NULL;
    mBackCam5 = NULL;
    mBackCam6 = NULL;
    mAntialiasing = NULL;
}
LevelCam::~LevelCam() {

    LOGV(LOG_LEVEL_LEVELCAM, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mBackCam1) delete mBackCam1;
    if (mBackCam2) delete mBackCam2;
    if (mBackCam3) delete mBackCam3;
    if (mBackCam4) delete mBackCam4;
    if (mBackCam5) delete mBackCam5;
    if (mBackCam6) delete mBackCam6;
    if (mAntialiasing) delete mAntialiasing;
}

void LevelCam::pause() {

    LOGV(LOG_LEVEL_LEVELCAM, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if (mBackCam1) mBackCam1->pause();
    if (mBackCam2) mBackCam2->pause();
    if (mBackCam3) mBackCam3->pause();
    if (mBackCam4) mBackCam4->pause();
    if (mBackCam5) mBackCam5->pause();
    if (mBackCam6) mBackCam6->pause();
    if (mAntialiasing) mAntialiasing->pause();
}

bool LevelCam::loaded(const Game* game) {

    LOGV(LOG_LEVEL_LEVELCAM, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (!mAntialiasing) {

        mAntialiasing = new Static2D;
        mAntialiasing->initialize(game2DVia(game));
        mAntialiasing->start(TEXTURE_ID_ANTIALIASING);
        mAntialiasing->setTexCoords(FULL_TEXCOORD_BUFFER);

        assert(mTextures->getIndex(TEXTURE_ID_ANTIALIASING) == 0);
        mAntialiasing->setVertices((game->getScreen()->width >> 1) - (*mTextures)[0]->width,
                (game->getScreen()->height >> 1) + (*mTextures)[0]->height,
                (game->getScreen()->width >> 1) + (*mTextures)[0]->width,
                (game->getScreen()->height >> 1) - (*mTextures)[0]->height);
    }
    else
        mAntialiasing->resume(TEXTURE_ID_ANTIALIASING);

    float texCoords[8] = {0};
    if (!mBackCam1) {

        mBackCam1 = new Static2D(false);
        mBackCam1->initialize(game2DVia(game));

        // Start camera (one time only)
        mCamera->start(640, 480);

        mBackCam1->start(mCamera->getCamTexIdx());
#ifndef __ANDROID__
        mBackCam1->setBGRA(true);
#endif

        assert(mTextures->getIndex(TEXTURE_ID_CAM) == 1);
        texCoords[3] = 480.f / (*mTextures)[1]->height;
        texCoords[4] = 640.f / (*mTextures)[1]->width;
        texCoords[5] = texCoords[3];
        texCoords[6] = texCoords[4];
        mBackCam1->setTexCoords(texCoords);
        mBackCam1->setVertices(0, game->getScreen()->height, game->getScreen()->width / 3, game->getScreen()->height >> 1);
    }
    else {

        // Resume camera (one time only)
        //mCamera->resume();

        mBackCam1->resume(mCamera->getCamTexIdx());
    }
    if (!mBackCam2) {

        mBackCam2 = new Static2D(false);
        mBackCam2->initialize(game2DVia(game));
        mBackCam2->start(mCamera->getCamTexIdx());
#ifndef __ANDROID__
        mBackCam2->setBGRA(true);
#endif

        mBackCam2->setTexCoords(texCoords);
        mBackCam2->setVertices(game->getScreen()->width / 3, game->getScreen()->height, (game->getScreen()->width << 1) / 3,
                game->getScreen()->height >> 1);
        mBackCam2->setRed(0.5f);
        mBackCam2->setBlue(0.5f);
    }
    else
        mBackCam2->resume(mCamera->getCamTexIdx());
    if (!mBackCam3) {

        mBackCam3 = new Static2D(false);
        mBackCam3->initialize(game2DVia(game));
        mBackCam3->start(mCamera->getCamTexIdx());
#ifndef __ANDROID__
        mBackCam3->setBGRA(true);
#endif

        mBackCam3->setTexCoords(texCoords);
        mBackCam3->setVertices((game->getScreen()->width << 1) / 3, game->getScreen()->height, game->getScreen()->width,
                game->getScreen()->height >> 1);
    }
    else
        mBackCam3->resume(mCamera->getCamTexIdx());
    if (!mBackCam4) {

        mBackCam4 = new Static2D(false);
        mBackCam4->initialize(game2DVia(game));
        mBackCam4->start(mCamera->getCamTexIdx());
#ifndef __ANDROID__
        mBackCam4->setBGRA(true);
#endif

        mBackCam4->setTexCoords(texCoords);
        mBackCam4->setVertices(0, game->getScreen()->height >> 1, game->getScreen()->width / 3, 0);
        mBackCam4->setGreen(0.5f);
        mBackCam4->setBlue(0.5f);
    }
    else
        mBackCam4->resume(mCamera->getCamTexIdx());
    if (!mBackCam5) {

        mBackCam5 = new Static2D(false);
        mBackCam5->initialize(game2DVia(game));
        mBackCam5->start(mCamera->getCamTexIdx());
#ifndef __ANDROID__
        mBackCam5->setBGRA(true);
#endif

        mBackCam5->setTexCoords(texCoords);
        mBackCam5->setVertices(game->getScreen()->width / 3, game->getScreen()->height >> 1,
                (game->getScreen()->width << 1) / 3, 0);
    }
    else
        mBackCam5->resume(mCamera->getCamTexIdx());
    if (!mBackCam6) {

        mBackCam6 = new Static2D(false);
        mBackCam6->initialize(game2DVia(game));
        mBackCam6->start(mCamera->getCamTexIdx());
#ifndef __ANDROID__
        mBackCam6->setBGRA(true);
#endif

        mBackCam6->setTexCoords(texCoords);
        mBackCam6->setVertices((game->getScreen()->width << 1) / 3, game->getScreen()->height >> 1,
                game->getScreen()->width, 0);
        mBackCam6->setRed(0.5f);
        mBackCam6->setGreen(0.5f);
    }
    else
        mBackCam6->resume(mCamera->getCamTexIdx());

    return true;
}
bool LevelCam::update(const Game* game) {

    // Check back button click
    unsigned char touchCount = game->mTouchCount;
    if ((touchCount--) && (game->mTouchData[touchCount].Type == TouchInput::TOUCH_UP)) {

        // Stop camera
        mCamera->stop();

        game->setGameLevel(1); // Menu level
        return false;
    }

    // Update camera texture
    mCamera->updateTexture();

    return true;
}

void LevelCam::renderUpdate() const {

    glDisable(GL_BLEND);
    mBackCam1->render(false);
    mBackCam2->render(false);
    mBackCam3->render(false);
    mBackCam4->render(false);
    mBackCam5->render(false);
    mBackCam6->render(false);
    glEnable(GL_BLEND);

    mAntialiasing->render(false);
}
