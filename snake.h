#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include <vector>


class CSnake:public CFramedWindow
{
public:
    CSnake(CRect r, char _c = ' ');
    vector<CPoint*> bodySnake;
    CPoint foodSnake;
    CPoint endSnake;

    void paint() override;
    bool handleEvent(int key);
    void moveS(const CPoint & delta);
    void growthSnake();
    void generateNewPositon();
    void reset();
    void showHelp();

    bool collisionDetected();
    bool foodFind();
    int lastChose;
    int level = 1;
    bool isRun = false;
    bool gameOver = false;
    bool isPausa = false;
};

#endif
