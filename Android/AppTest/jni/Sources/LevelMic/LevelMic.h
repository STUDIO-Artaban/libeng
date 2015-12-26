#ifndef LEVELMIC_H_
#define LEVELMIC_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Progress/Progress.h>
#include <libeng/Features/Mic/Mic.h>

using namespace eng;

//////
class LevelMic : public Level {

private:
    Progress* mMicDB;
    Mic* mMic;

public:
    LevelMic(Game* game);
    virtual ~LevelMic();

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

#endif // LEVELMIC_H_
