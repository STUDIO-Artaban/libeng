#ifndef LEVELMENU_H_
#define LEVELMENU_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Text/2D/Text2D.h>
#include <libeng/Advertising/Advertising.h>

using namespace eng;

//////
class LevelMenu : public Level {

private:
    Back2D* mBackground;
    Text2D* mMenu;

    Advertising* mAdvertising;
    bool mAdLoaded;
    void adDisplay(bool delay);
    bool adHide();

    Text2D* mMegaChoice;
    short mMegaLeft;
    short mMegaRight;
    short mMegaTop;
    short mMegaBottom;

    Text2D* mNativeChoice;
    short mNativeLeft;
    short mNativeRight;
    short mNativeTop;
    short mNativeBottom;

    Text2D* mClayChoice;
    short mClayLeft;
    short mClayRight;
    short mClayTop;
    short mClayBottom;

    Text2D* mWalkChoice;
    short mWalkLeft;
    short mWalkRight;
    short mWalkTop;
    short mWalkBottom;

    Text2D* mCamChoice;
    short mCamLeft;
    short mCamRight;
    short mCamTop;
    short mCamBottom;

    Text2D* mMikeChoice;
    short mMikeLeft;
    short mMikeRight;
    short mMikeTop;
    short mMikeBottom;

public:
    LevelMenu(Game* game);
    virtual ~LevelMenu();

    //////
    void init() { }
    void pause();

    void wait(float millis) { }

protected:
    bool loaded(const Game* game);
public:
    bool update(const Game* game);

    void renderLoad() const { }
    void renderUpdate() const;

};

#endif // LEVELMENU_H_
