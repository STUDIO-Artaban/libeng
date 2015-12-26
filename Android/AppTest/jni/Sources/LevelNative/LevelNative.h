#ifndef LEVELNATIVE_H_
#define LEVELNATIVE_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Bounds/Overlap.h>
#include <libeng/Graphic/Text/2D/Text2D.h>
#include <libeng/Graphic/Object/2D/Static2D.h>
#include <libeng/Graphic/Object/2D/Element2D.h>
#include <libeng/Graphic/Progress/Progress.h>

// Texture IDs
#define TEXTURE_ID_ANTIALIASING     8

using namespace eng;

//////
class LevelNative : public Level {

private:
    BorderLeft* mScreenLeft;
    BorderRight* mScreenRight;
    BorderTop* mScreenTop;
    BorderBottom* mScreenBottom;

    BorderTop mSphereTop;
    BorderBottom mSphereBottom;
    bool mSphereCheckTop;
    // For 'mSphere1'

    BorderRight mSphereRight;
    BorderLeft mSphereLeft;
    bool mSphereCheckRight;
    // For 'mSphere3'

    Progress* mProgress;

    //
    Static2D* mBackground;
    Static2D* mSoundPanel;
    short mPanelWidth;

    short mPlayTop;
    short mPauseTop;
    short mStopTop;

    Text2D* mText1;
    Text2D* mText2;
    Text2D* mText3;

    float mPrevVolDist;
    unsigned int mVolTouchId1;
    unsigned int mVolTouchId2;

    Element2D* mSphere1;
    Element2D* mSphere2;
    Element2D* mSphere3;

    Element2D* mAntialiasing;
    std::vector<Static2D*> mChips;

public:
    LevelNative(Game* game);
    virtual ~LevelNative();

    //////
    void init() { }
    void pause();

    void wait(float millis) { }

protected:
    bool loaded(const Game* game);
public:
    bool update(const Game* game);

    void renderLoad() const;
    void renderUpdate() const;

};

#endif // LEVELNATIVE_H_
