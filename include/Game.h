#ifndef GAME_H
#define GAME_H

#include "DialogBase.h"
#include "Point.h"
#include <list>
#include <iterator>

using namespace std;

class Game: public DialogBase {
    public:
        Game();
        virtual ~Game();
        int Launch();
        int Draw();
        int EventFilter();
        void ChangeScore(Font*);
    private:
        int frequency, score, gameTime;
        Point *hunterHero;
        list<Point*> listOfBullets;
        list<Point*> listOfBunnies;
};

#endif // GAME_H
