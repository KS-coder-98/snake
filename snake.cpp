#include <zconf.h>
#include "snake.h"
#include "winsys.h"
#include "screen.h"

CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c)
{
    bodySnake.emplace_back(CPoint(3, 3));
    bodySnake.emplace_back(CPoint(4, 3));
    bodySnake.emplace_back(CPoint(5, 3));
    foodSnake = CPoint(22, 12);
    lastChose = KEY_RIGHT;
}

void CSnake::paint() {
    CFramedWindow::paint();
    for ( auto snake : bodySnake){
        gotoyx(snake.y + geom.topleft.y, snake.x + geom.topleft.x);
        printl("X");
    }
    gotoyx(bodySnake[bodySnake.size()-1].y + geom.topleft.y, bodySnake[bodySnake.size()-1].x + geom.topleft.x);
    printl("*");
    gotoyx(foodSnake.y + geom.topleft.y, foodSnake.x + geom.topleft.x);
    printl("O");
    gotoyx(geom.topleft.y - 1, geom.topleft.x);
    printl("| Level : %d |", level);
    timeout(1000/level);
    if ( !isRun )
        showHelp();
    else if ( gameOver ){
        gotoyx(geom.topleft.y +2, geom.topleft.x + 2);
        printl("GAME OVER SCORE: %d", level);
    }

}

bool CSnake::handleEvent(int c)
{
    if (!isRun || isPausa){
        if(CFramedWindow::handleEvent(c))  // jesli strzlka to zwroci true
            return true;
    }
    if ( c == 'h' ){
        isRun = !isRun;
        paint();
        refresh();
        c = lastChose;
    }
    else if ( c == 'r' )
        reset();
    else if ( c == 'p' ){
        isPausa = !isPausa;
        paint();
        refresh();
    }
    if (!isRun || gameOver || isPausa)
        return false;
    if ( c == -1 )
        c =lastChose;
    switch (c)
    {
        case KEY_UP:
            if ( lastChose == KEY_DOWN ){
                c = KEY_DOWN;
                return false;
            }
            else{
                moveS (CPoint (0, -1));
                lastChose = KEY_UP;
            }
            return true;
        case KEY_DOWN:
            if (lastChose == KEY_UP)
                return true;
            moveS (CPoint (0, 1));
            lastChose = KEY_DOWN;
            return true;
        case KEY_RIGHT:
            if ( lastChose == KEY_LEFT )
                return true;
            lastChose = KEY_RIGHT;
            moveS (CPoint (1, 0));
            return true;
        case KEY_LEFT:
            if ( lastChose == KEY_RIGHT )
                return true;
            lastChose = KEY_LEFT;
            moveS (CPoint (-1, 0));
            return true;
    };
    return false;
}

void CSnake::moveS(const CPoint &delta) {
    endSnake = bodySnake[0];
    for (unsigned int i =0; i < bodySnake.size()-1; i++){
        bodySnake[i].x = bodySnake[i+1].x;
        bodySnake[i].y = bodySnake[i+1].y;
    }
    auto &temp = bodySnake[bodySnake.size()-1];
    temp += delta;
    for ( auto &pieceSnake : bodySnake ){
        if (pieceSnake.x >= geom.size.x - 1){
            pieceSnake.x = 1;
            break;
        }
        else if (pieceSnake.x < 1){
            pieceSnake.x = geom.size.x - 2;
            break;
        }
        else if (pieceSnake.y >= geom.size.y - 1){
            pieceSnake.y = 1;
            break;
        }
        else if (pieceSnake.y < 1){
            pieceSnake.y = geom.size.y - 2;
            break;
        }
    }
    foodFind();
    collisionDetected();
}

bool CSnake::collisionDetected() {
    for ( size_t index = 0; index < bodySnake.size(); index++ ){
        for ( size_t index1 = index + 1; index1 < bodySnake.size(); index1++ )
            if ( bodySnake[index] == bodySnake[index1] ){
                gameOver = true;
                return true;
            }
    }
    return false;
}

bool CSnake::foodFind() {
    for ( auto pieceSnake : bodySnake ){
        if (pieceSnake == foodSnake){
            generateNewPositon();
            growthSnake();
            if ( bodySnake.size() % 2 == 0 )
                level++;
            return true;
        }
    }
    return false;
}

void CSnake::growthSnake() {
        bodySnake.emplace(bodySnake.begin(), CPoint(endSnake));
}

void CSnake::generateNewPositon() {
    foodSnake.x = rand()%(geom.size.x + 1 - 2 - 1) + 1;
    foodSnake.y = rand()%(geom.size.y - 2 + 1 - 1) + 1;
}

void CSnake::reset() {
    gameOver = false;
    isRun = false;
    bodySnake.clear();
    bodySnake.emplace_back(CPoint(3, 3));
    bodySnake.emplace_back(CPoint(4, 3));
    bodySnake.emplace_back(CPoint(5, 3));
    foodSnake = CPoint(22, 12);
    lastChose = KEY_RIGHT;
    level = 1;
    paint();
    refresh();
}

void CSnake::showHelp() {
    gotoyx(geom.topleft.y + 5, geom.topleft.x + 5);
    printl("h - toggle help information");
    gotoyx(geom.topleft.y + 6, geom.topleft.x + 5);
    printl("p - toggle pause/play mode");
    gotoyx(geom.topleft.y + 7, geom.topleft.x + 5);
    printl("r - restart game");
    gotoyx(geom.topleft.y + 8, geom.topleft.x + 5);
    printl("arrows - move snake (in play mode) or");
    gotoyx(geom.topleft.y + 9, geom.topleft.x + 5);
    printl("         move window (in pause mode)");
}



