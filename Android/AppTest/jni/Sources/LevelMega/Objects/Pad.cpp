#include "Pad.h"
#include "Global.h"

#include <libeng/Game/2D/Game2D.h>
#include <cstring>

//////
Pad::Pad() : mScreen(NULL), mBtnJump(false), mJoypad(false), mBtnShoot(false), mBtnActivate(false),
             mTextures(NULL) { // No object 'start' methods call (manage texture here)

    LOGV(LOG_LEVEL_PAD, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
#ifdef DEBUG
    mLog = NULL;
#endif
}
Pad::~Pad() {

    LOGV(LOG_LEVEL_PAD, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    unsigned char textureIdx = mTextures->getIndex(TEXTURE_ID_JOYPAD);
    if (textureIdx != TEXTURE_IDX_INVALID)
        mTextures->delTexture(textureIdx);
}

void Pad::initialize(const Game* game) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_PAD, 0, LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
    mLog = game->getLog();
#endif
    mTextures = Textures::getInstance();
    mScreen = game->getScreen();

    mJoypad.initialize(game2DVia(game));
    mBtnJump.initialize(game2DVia(game));
    mBtnShoot.initialize(game2DVia(game));
    mBtnActivate.initialize(game2DVia(game));
}

void Pad::start(bool resume) {

    LOGV(LOG_LEVEL_PAD, 0, LOG_FORMAT(" - r:%s"), __PRETTY_FUNCTION__, __LINE__, (resume)? "true":"false");

    unsigned char textureIdx = mTextures->getIndex(TEXTURE_ID_JOYPAD);
    if (textureIdx == TEXTURE_IDX_INVALID)
        textureIdx = Textures::loadTexture(TEXTURE_ID_JOYPAD); // Load texture
    else
        assert(NULL); // Should not exist (only one instance of this object use it)
    mTextures->genTexture(textureIdx);

    if (!resume) { // Start game

        mJoypad.start(textureIdx);
        mBtnJump.start(textureIdx);
        mBtnShoot.start(textureIdx);
        mBtnActivate.start(textureIdx);

        ////// Texture coordinates
        float texCoords[8];
        texCoords[0] = JOYPAD_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[1] = JOYPAD_Y0 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[2] = JOYPAD_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[3] = JOYPAD_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[4] = JOYPAD_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[5] = JOYPAD_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[6] = JOYPAD_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[7] = JOYPAD_Y0 / JOYPAD_TEXTURE_HEIGHT;
        mJoypad.setTexCoords(texCoords);

        texCoords[0] = BTN_JUMP_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[1] = BTN_JUMP_Y0 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[2] = BTN_JUMP_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[3] = BTN_JUMP_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[4] = BTN_JUMP_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[5] = BTN_JUMP_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[6] = BTN_JUMP_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[7] = BTN_JUMP_Y0 / JOYPAD_TEXTURE_HEIGHT;
        mBtnJump.setTexCoords(texCoords);

        texCoords[0] = BTN_SHOOT_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[1] = BTN_SHOOT_Y0 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[2] = BTN_SHOOT_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[3] = BTN_SHOOT_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[4] = BTN_SHOOT_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[5] = BTN_SHOOT_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[6] = BTN_SHOOT_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[7] = BTN_SHOOT_Y0 / JOYPAD_TEXTURE_HEIGHT;
        mBtnShoot.setTexCoords(texCoords);

        texCoords[0] = BTN_ACTIVATE_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[1] = BTN_ACTIVATE_Y0 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[2] = BTN_ACTIVATE_X0 / JOYPAD_TEXTURE_WIDTH;
        texCoords[3] = BTN_ACTIVATE_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[4] = BTN_ACTIVATE_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[5] = BTN_ACTIVATE_Y2 / JOYPAD_TEXTURE_HEIGHT;
        texCoords[6] = BTN_ACTIVATE_X2 / JOYPAD_TEXTURE_WIDTH;
        texCoords[7] = BTN_ACTIVATE_Y0 / JOYPAD_TEXTURE_HEIGHT;
        mBtnActivate.setTexCoords(texCoords);

        ////// Vertex coordinates (see 'Surface2D::positionVertices' method)
        // -> With [0;0]: Bottom/left coordinates; [g_ScreenWidth;g_ScreenHeight]: Top/right coordinates
        mJoypad.setVertices(0, JOYPAD_SIZE * JOYPAD_2D_RATIO, JOYPAD_SIZE * JOYPAD_2D_RATIO, 0);

        mBtnJump.setVertices(mScreen->width - JUMP_LEFT, JUMP_TOP, mScreen->width - JUMP_RIGHT, JUMP_BOTTOM);
        mBtnShoot.setVertices(mScreen->width - SHOOT_LEFT, SHOOT_TOP, mScreen->width - SHOOT_RIGHT, SHOOT_BOTTOM);
        mBtnActivate.setVertices(mScreen->width - ACTIVATE_LEFT, ACTIVATE_TOP, mScreen->width - ACTIVATE_RIGHT, ACTIVATE_BOTTOM);
    }
    else { // Resume game

        mJoypad.resume(textureIdx);

        mBtnJump.resume(textureIdx);
        mBtnShoot.resume(textureIdx);
        mBtnActivate.resume(textureIdx);
        // ...same as 'Surface2D::start' method call (just check if 'Surface2D::pause' methods have been called before)
    }
}
void Pad::render() const {

#ifdef DEBUG
    LOGV(LOG_LEVEL_PAD, (*mLog % 100), LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
#endif

    mJoypad.render(false);

    mBtnJump.render(false);
    mBtnShoot.render(false);
    mBtnActivate.render(false);
}
