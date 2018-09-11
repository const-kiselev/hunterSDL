#include "Point.h"

Point::Point(){
    //ctor
}

Point::~Point(){
    //dtor
}

Hunter::Hunter(){
    objRectData.x = 0;
    objRectData.y = 0;
    objRectData.w = 127;
    objRectData.h = 96;

    posOnScreen.x = 0;
    posOnScreen.y = 0;
    posOnScreen.w = objRectData.w;
    posOnScreen.h = objRectData.h;

    yVel = objRectData.h/28;
    xVel = 0;

    urgentSurface = NULL;
}

Hunter::~Hunter(){
    SDL_FreeSurface(urgentSurface);
}

int
Hunter::Draw(SDL_Surface* surf){
    Uint32 Rmask, Gmask, Bmask;
    Rmask = surf->format->Rmask;
    Gmask = surf->format->Gmask;
    Bmask = surf->format->Bmask;
    if(!urgentSurface){
        urgentSurface = IMG_Load("hunter.gif");
    }
    if (surf) SDL_BlitSurface(urgentSurface, NULL, surf,  &posOnScreen);
    return 0;
}

int
Hunter::Move(bool data){
   // data: если TRUE, то вниз, если FALSE, то вверх
    posOnScreen.y += (data?1:(-1)) * yVel;
    // Проверка на выход за пределы окна
    if( ( posOnScreen.y < 0 ) || ( posOnScreen.y + objRectData.h > WINDOW_H ) || (!Collision( posOnScreen, wall ) ) )
        posOnScreen.y -= (data?1:(-1)) * yVel;
    return 0;
}

Bullet::Bullet(int x, int y){
    objRectData.x = 0;
    objRectData.y = 0;
    objRectData.w = 8;
    objRectData.h = 4;

    posOnScreen.x = x;
    posOnScreen.y = y;
    posOnScreen.w = objRectData.w;
    posOnScreen.h = objRectData.h;

    xVel = objRectData.w / 4;
    yVel = 0;

    urgentSurface = NULL;
}

Bullet::~Bullet(){
    SDL_FreeSurface(urgentSurface);
}

int
Bullet::Draw(SDL_Surface* surf){
    Uint32 Rmask, Gmask, Bmask;
    Rmask = surf->format->Rmask;
    Gmask = surf->format->Gmask;
    Bmask = surf->format->Bmask;
    if(!urgentSurface){
        urgentSurface = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, objRectData.w, objRectData.h, 32,Rmask, Gmask, Bmask, 0xFF000000);
        Draw_FillEllipse(urgentSurface,objRectData.x+objRectData.w/2,objRectData.y+objRectData.h/2,objRectData.w/2,objRectData.h/2, SDL_MapRGB(urgentSurface->format, 255, 0,01));
    }
    if (surf) SDL_BlitSurface(urgentSurface, NULL, surf,  &posOnScreen);
    return 0;
}

int
Bullet::Move(bool data){
    posOnScreen.x += (data?1:(-1)) * xVel;
    // Проверка на выход за пределы окна
    if( ( posOnScreen.x < 0 ) || ( posOnScreen.x + objRectData.w > WINDOW_W ) || (!Collision( posOnScreen, wall ) ) )
    {
        posOnScreen.x -= (data?1:(-1)) * xVel;
        return 1;
    }
    return 0;
}

Bunny::Bunny(int x, int y){
    objRectData.x = 0;
    objRectData.y = 0;
    objRectData.w = 109;
    objRectData.h = 100;

    posOnScreen.x = x;
    posOnScreen.y = y;
    posOnScreen.w = objRectData.w;
    posOnScreen.h = objRectData.h;

    xVel = objRectData.w / 4;
    yVel = 0;

    urgentSurface = NULL;
}

Bunny::~Bunny(){
    SDL_FreeSurface(urgentSurface);
}

int
Bunny::Draw(SDL_Surface*surf){
    Uint32 Rmask, Gmask, Bmask;
    Rmask = surf->format->Rmask;
    Gmask = surf->format->Gmask;
    Bmask = surf->format->Bmask;
    if(!urgentSurface){
        urgentSurface = IMG_Load("bunny.gif");
    }
    if (surf) SDL_BlitSurface(urgentSurface, NULL, surf,  &posOnScreen);
    return 0;
}

int
Point::SetPosOnScreen(int x, int y, int w, int h){
    posOnScreen.x = x;
    posOnScreen.y = y;
    if(w) posOnScreen.w = w;
    if(h) posOnScreen.h = h;
    return 0;
}

SDL_Rect
Point::GetPosOnScreen(void){
    return posOnScreen;
}
