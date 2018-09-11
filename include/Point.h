#ifndef POINT_H
#define POINT_H

#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>
#include <DialogBase.h>

class Point {
    public:
        Point();
        virtual ~Point();
        virtual int Draw(SDL_Surface* surf)=0;
        virtual int Move(bool data)=0;
        int SetPosOnScreen(int, int, int, int);
        SDL_Rect GetPosOnScreen(void);
    protected:
        SDL_Surface *urgentSurface;
        SDL_Rect objRectData, posOnScreen;
        bool changes;
        int xVel,yVel; //  velocity - скорость перемещения кадр/секунда
};

class Hunter: public Point {
public:
    Hunter();
    ~Hunter();
    int Draw(SDL_Surface* surf);
    int Move(bool data);
protected:
};

class Bullet: public Point {
private:
public:
    Bullet(int x, int y);
    ~Bullet();
    int Draw(SDL_Surface* surf);
    int Move(bool data);
};

class Bunny: public Point {
public:
    Bunny(int x, int y);
    ~Bunny();
    int Draw(SDL_Surface* surf);
    int Move(bool data){return 0;};
};
#endif // POINT_H
