#ifndef WALKER_H_
#define WALKER_H_

#include <libeng/Graphic/Object/2D/Sprite2D.h>
#include "WalkAnims.h"

#define WALKPAD_TOP             100
#define WALKPAD_BOTTOM          300

#define WALKPAD_MOVE            6 // Change 'WALKPAD_MOVE' & 'WALKPAD_FACTOR' to obtain the best move according walkpad entries
#define MAX_SPEED               (WALKPAD_BOTTOM - WALKPAD_TOP) / 2 // Maximum speed provided by the walkpad (in pixel)
#define MIN_WALK_SPEED          30 // Counter limit before changing walk animation for a minimum speed (see 'walkCounter')
#define WALK_SPEED_RATIO        (MIN_WALK_SPEED * 1.0f) / (MAX_SPEED * 1.0f) // Ratio to "convert" speed into animation speed
                                                                              // -> " * 1.0" to make it a float definition
// Texture IDs
#define TEXTURE_ID_WALKMAN      23

#define WALK_TEXTURE_WIDTH      1024.0f
#define WALK_TEXTURE_HEIGHT     512.0f

// Anims index definitions
enum { STAND = 0, RUN1, RUN2, RUN3, RUN4, RUN5, RUN6, RUN7, RUN8 };
#define WALK_ANIM_COUNT         (RUN8 + 1)

static const float gs_WalkerTexCoords[WALK_ANIM_COUNT][8] = {
    { WALK_NONE_X0/WALK_TEXTURE_WIDTH, WALK_NONE_Y0/WALK_TEXTURE_HEIGHT, WALK_NONE_X0/WALK_TEXTURE_WIDTH, WALK_NONE_Y2/WALK_TEXTURE_HEIGHT,
      WALK_NONE_X2/WALK_TEXTURE_WIDTH, WALK_NONE_Y2/WALK_TEXTURE_HEIGHT, WALK_NONE_X2/WALK_TEXTURE_WIDTH, WALK_NONE_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN1_X0/WALK_TEXTURE_WIDTH, WALK_RUN1_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN1_X0/WALK_TEXTURE_WIDTH, WALK_RUN1_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN1_X2/WALK_TEXTURE_WIDTH, WALK_RUN1_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN1_X2/WALK_TEXTURE_WIDTH, WALK_RUN1_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN2_X0/WALK_TEXTURE_WIDTH, WALK_RUN2_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN2_X0/WALK_TEXTURE_WIDTH, WALK_RUN2_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN2_X2/WALK_TEXTURE_WIDTH, WALK_RUN2_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN2_X2/WALK_TEXTURE_WIDTH, WALK_RUN2_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN3_X0/WALK_TEXTURE_WIDTH, WALK_RUN3_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN3_X0/WALK_TEXTURE_WIDTH, WALK_RUN3_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN3_X2/WALK_TEXTURE_WIDTH, WALK_RUN3_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN3_X2/WALK_TEXTURE_WIDTH, WALK_RUN3_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN4_X0/WALK_TEXTURE_WIDTH, WALK_RUN4_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN4_X0/WALK_TEXTURE_WIDTH, WALK_RUN4_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN4_X2/WALK_TEXTURE_WIDTH, WALK_RUN4_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN4_X2/WALK_TEXTURE_WIDTH, WALK_RUN4_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN5_X0/WALK_TEXTURE_WIDTH, WALK_RUN5_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN5_X0/WALK_TEXTURE_WIDTH, WALK_RUN5_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN5_X2/WALK_TEXTURE_WIDTH, WALK_RUN5_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN5_X2/WALK_TEXTURE_WIDTH, WALK_RUN5_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN6_X0/WALK_TEXTURE_WIDTH, WALK_RUN6_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN6_X0/WALK_TEXTURE_WIDTH, WALK_RUN6_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN6_X2/WALK_TEXTURE_WIDTH, WALK_RUN6_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN6_X2/WALK_TEXTURE_WIDTH, WALK_RUN6_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN7_X0/WALK_TEXTURE_WIDTH, WALK_RUN7_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN7_X0/WALK_TEXTURE_WIDTH, WALK_RUN7_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN7_X2/WALK_TEXTURE_WIDTH, WALK_RUN7_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN7_X2/WALK_TEXTURE_WIDTH, WALK_RUN7_Y0/WALK_TEXTURE_HEIGHT },
    { WALK_RUN8_X0/WALK_TEXTURE_WIDTH, WALK_RUN8_Y0/WALK_TEXTURE_HEIGHT, WALK_RUN8_X0/WALK_TEXTURE_WIDTH, WALK_RUN8_Y2/WALK_TEXTURE_HEIGHT,
      WALK_RUN8_X2/WALK_TEXTURE_WIDTH, WALK_RUN8_Y2/WALK_TEXTURE_HEIGHT, WALK_RUN8_X2/WALK_TEXTURE_WIDTH, WALK_RUN8_Y0/WALK_TEXTURE_HEIGHT } };

using namespace eng;

//////
class WalkMan : public Sprite2D {

private:
    inline void setAnim(unsigned char animIdx, bool backward) {

        mCurAnim = animIdx;
        animVertices(mCurAnim);
        animTexCoords(gs_WalkerTexCoords, mCurAnim, backward);
    }
    inline void walk(short speed, unsigned char &counter, bool backward) {

        if (counter < ((MIN_WALK_SPEED + 1) - static_cast<unsigned char>(std::abs<short>(speed) * WALK_SPEED_RATIO))) {

            counter += WALKPAD_MOVE;
            return;
        }
        counter = 0;

        if (mCurAnim != RUN8)
            mCurAnim++;
        else
            mCurAnim = RUN1;

        animVertices(mCurAnim);
        animTexCoords(gs_WalkerTexCoords, mCurAnim, backward);
    }

protected:
    void fillVerticesAnims(unsigned char animIdx);

public:
    WalkMan();
    virtual ~WalkMan();

    //
    void start(float floor);

    //////
    void update(const Game* game, const Level* level);

    bool checkCollision(const Bounds* bounds, unsigned char boundsId) { return false; }
    bool checkCollision(const Object* object, unsigned char objectId) { return false; }

};

#endif // WALKER_H_
