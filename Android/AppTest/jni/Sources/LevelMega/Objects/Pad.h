#ifndef PAD_H_
#define PAD_H_

#include "Global.h"

#include <libeng/Log/Log.h>
#include <libeng/Graphic/Object/2D/Static2D.h>
#include <libeng/Game/Game.h>
#include "PadCoords.h"

#define TEXTURE_ID_JOYPAD           5
#define JOYPAD_TEXTURE_WIDTH        256.0f
#define JOYPAD_TEXTURE_HEIGHT       64.0f

#define JOYPAD_RATIO                2
#define JOYPAD_2D_RATIO             2.5

// Joypad touch area
#define BACKWARD_LEFT               0
#define BACKWARD_TOP                (21 + (2 * 19)) * JOYPAD_RATIO
#define BACKWARD_RIGHT              21 * JOYPAD_RATIO
#define BACKWARD_BOTTOM             21 * JOYPAD_RATIO

#define DOWN_LEFT                   21 * JOYPAD_RATIO
#define DOWN_TOP                    21 * JOYPAD_RATIO
#define DOWN_RIGHT                  (21 + (2 * 19)) * JOYPAD_RATIO
#define DOWN_BOTTOM                 0

#define UP_LEFT                     21 * JOYPAD_RATIO
#define UP_TOP                      ((2 * 21) + (2 * 19)) * JOYPAD_RATIO
#define UP_RIGHT                    (21 + (2 * 19)) * JOYPAD_RATIO
#define UP_BOTTOM                   (21 + (2 * 19)) * JOYPAD_RATIO

#define FORWARD_LEFT                (21 + (2 * 19)) * JOYPAD_RATIO
#define FORWARD_TOP                 (21 + (2 * 19)) * JOYPAD_RATIO
#define FORWARD_RIGHT               ((2 * 21) + (2 * 19)) * JOYPAD_RATIO
#define FORWARD_BOTTOM              21 * JOYPAD_RATIO

// Buttons touch area
#define JUMP_LEFT                   BUTTON_SIZE * JOYPAD_RATIO
#define JUMP_TOP                    (BUTTON_SIZE + 40) * JOYPAD_RATIO
#define JUMP_RIGHT                  0
#define JUMP_BOTTOM                 40 * JOYPAD_RATIO

#define SHOOT_LEFT                  (BUTTON_SIZE * 2 * JOYPAD_RATIO)
#define SHOOT_TOP                   (BUTTON_SIZE + 40) * JOYPAD_RATIO
#define SHOOT_RIGHT                 BUTTON_SIZE * JOYPAD_RATIO
#define SHOOT_BOTTOM                40 * JOYPAD_RATIO

#define ACTIVATE_LEFT               (BUTTON_SIZE + (BUTTON_SIZE / 2)) * JOYPAD_RATIO
#define ACTIVATE_TOP                BUTTON_SIZE * JOYPAD_RATIO
#define ACTIVATE_RIGHT              (BUTTON_SIZE / 2) * JOYPAD_RATIO
#define ACTIVATE_BOTTOM             0
// WARNING: Jump & Shoot area are beyond Activate area

using namespace eng;

//////
class Pad {

private:
#ifdef DEBUG
    const unsigned int* mLog;
#endif
    const Screen* mScreen;
    Textures* mTextures;

    Static2D mJoypad;
    Static2D mBtnJump;
    Static2D mBtnShoot;
    Static2D mBtnActivate;

public:
    Pad();
    virtual ~Pad();

    void initialize(const Game* game);

    inline void pause() {

        mJoypad.pause();
        mBtnJump.pause();
        mBtnShoot.pause();
        mBtnActivate.pause();
    };

    //////
    void start(bool resume);
    void render() const;

};

#endif // PAD_H_
