#ifndef LEVELCAM_H_
#define LEVELCAM_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Object/2D/Static2D.h>
#include <libeng/Features/Camera/Camera.h>

using namespace eng;

//////
class LevelCam : public Level {

private:
    Camera* mCamera;

    Static2D* mBackCam1;
    Static2D* mBackCam2;
    Static2D* mBackCam3;
    Static2D* mBackCam4;
    Static2D* mBackCam5;
    Static2D* mBackCam6;

    Static2D* mAntialiasing;

public:
    LevelCam(Game* game);
    virtual ~LevelCam();

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

#endif // LEVELCAM_H_
