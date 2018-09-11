#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game(){
    score = 0;
    Rmask = mainSurface->format->Rmask;
    Gmask = mainSurface->format->Gmask;
    Bmask = mainSurface->format->Bmask;

    // Фон игры, используется одна бесшовная картинка и распределяется по поверхности
    SDL_Surface* tmpImgSurf = IMG_Load("background.gif");
    SDL_Rect tmpObjRectData = {0,0,200,200};
    background = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,WINDOW_W, WINDOW_H, 32,Rmask, Gmask, Bmask, 0);
    for(int heigthI=0; heigthI<=WINDOW_H; heigthI += 200)
        for(int widthI = 0; widthI<=WINDOW_W; widthI += 200)
        {
            tmpObjRectData = {widthI,heigthI,0,0}; // x, y, w, h
            SDL_BlitSurface(tmpImgSurf, NULL, background, &tmpObjRectData);
        }
    SDL_FreeSurface(tmpImgSurf);
    tmpImgSurf = NULL;
    // Герои игры
    hunterHero = new Hunter; // Охотник
    text_color = {255,255,255,0};
}

Game::~Game(){
    SDL_FreeSurface(background);
    SDL_FreeSurface(urgentSurface);
    delete hunterHero;
    hunterHero = NULL;
    list<Point*>::iterator tmpPoint;
    for(list<Point*>::iterator itBunny = listOfBunnies.begin(); itBunny != listOfBunnies.end(); ++itBunny){
        tmpPoint = itBunny;
        tmpPoint++;
        delete *itBunny;
        listOfBunnies.erase(itBunny);
        itBunny = tmpPoint;
    }
    for(list<Point*>::iterator itBullet = listOfBullets.begin();
            itBullet != listOfBullets.end(); ++itBullet)
    {
        tmpPoint = itBullet;
        tmpPoint++;
        delete *itBullet;
        listOfBunnies.erase(itBullet);
        itBullet = tmpPoint;
    }

}

int
Game::Launch(){
    Tips SaverTip;
    //Убейте как можно больше кроликов. Клавиши вверх/вниз — перемещение охотника. Пробел — выстрел. Esc — пауза.
    SaverTip.ChangeTipText("РЈР±РµР№С‚Рµ РєР°Рє РјРѕР¶РЅРѕ Р±РѕР»СЊС€Рµ РєСЂРѕР»РёРєРѕРІ. РљР»Р°РІРёС€Рё РІРІРµСЂС…/РІРЅРёР· вЂ” РїРµСЂРµРјРµС‰РµРЅРёРµ РѕС…РѕС‚РЅРёРєР°. РџСЂРѕР±РµР» вЂ” РІС‹СЃС‚СЂРµР». Esc вЂ” РїР°СѓР·Р°.");
    return EventFilter();
}

int
Game::Draw(){
    SDL_BlitSurface(background, NULL, mainSurface, NULL);
    hunterHero->Draw(mainSurface);
    for(list<Point*>::iterator itBunny = listOfBunnies.begin();
                        itBunny != listOfBunnies.end(); ++itBunny) (*itBunny)->Draw(mainSurface);
    for(list<Point*>::iterator itBullet = listOfBullets.begin();
            itBullet != listOfBullets.end(); ++itBullet) (*itBullet)->Draw(mainSurface);
    return 0;
}

int
Game::EventFilter(){
    bool nextstep = true;
    int frame = 0, numOfBullets=0, numOfAllBunnies=0;; // счётчик кадров
    int timeInSeconds=0, maxBunnies = 2, random, currentBunniesInPeriod, TimeForTTFDraw;
    Timer fps, game; // таймер используемый для вычисления количества кадров в секунду
    SDL_Event event;
    SDL_EnableKeyRepeat(1,10);
    char ttext[10] = "0";
    char tmpText[10] = "";
    fontPointer = new Font(0,0, 19, mainTTFname, ttext, &text_color);
    Font *fontForScore = new Font(0,22, 19, mainTTFname, ttext, &text_color);
    game.start(); // таймер игры
    srand(time(NULL)); //
    int timeDelta=4;

    while(nextstep)
    {
        fps.start();
// ---------------------->> СОБЫТИЯ <<----------------------
        if(SDL_PollEvent(&event)){
            switch(event.type)
            {
            case SDL_QUIT:
                nextstep = false;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_UP:
                    hunterHero->Move(false);
                    break;
                case SDLK_DOWN:
                    hunterHero->Move(true);
                    break;
                }
//                action = true;
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                case SDLK_SPACE:
                    {
                        SDL_Rect tmp = hunterHero->GetPosOnScreen();
                        listOfBullets.push_back(new Bullet(tmp.x+tmp.w, tmp.y+43));
                        numOfBullets++;
                    }
                    break;
                case SDLK_ESCAPE:
                    nextstep = false;
                }

            } // switch
        }// if
// ---------------------->> ЛОГИКА <<----------------------
        gameTime = 60-game.get_ticks()/1000;
        for(list<Point*>::iterator itBullet = listOfBullets.begin();
            itBullet != listOfBullets.end(); ++itBullet){
            if((*itBullet)->Move(true)) // движение пули
            {
                list<Point*>::iterator tmp = itBullet;
                tmp--;
                delete *itBullet, listOfBullets.erase(itBullet);
                itBullet = tmp;
            }
            else{
                for(list<Point*>::iterator itBunny = listOfBunnies.begin();
                itBunny != listOfBunnies.end(); ++itBunny){
                    if(Collision((*itBullet)->GetPosOnScreen(), (*itBunny)->GetPosOnScreen()))
                    {
                        list<Point*>::iterator tmp = itBullet; // устанавливаем временный итератор на итератор Пуль
                        tmp--; // переходим к следующему элементу
                        delete *itBullet, listOfBullets.erase(itBullet); // удаляем пулю из списка и из памяти
                        itBullet = tmp; // присваеваем итератору пули указатель на следующую пулю
                        tmp = itBunny; // аналогично и с кроликом
                        tmp--;
                        delete *itBunny, listOfBunnies.erase(itBunny);
                        itBunny = tmp;
                        ChangeScore(fontForScore);
                        break;
                    }
                }
            }


        }
        if(!(gameTime % timeDelta) && gameTime!=timeInSeconds) maxBunnies += 2, currentBunniesInPeriod = 0, timeInSeconds = gameTime, timeDelta = 1+rand()%6;
        if(currentBunniesInPeriod < maxBunnies)
        {
            random = rand() % 4100;
            if(!(random%220))
            {
                SDL_Rect tmp = hunterHero->GetPosOnScreen();
                SDL_Rect tmpRand = {tmp.x+tmp.w + rand() % (WINDOW_W - tmp.x-tmp.w-108), rand() % (WINDOW_H-99), 109, 100};
                listOfBunnies.push_back(new Bunny(tmpRand.x, tmpRand.y));
                currentBunniesInPeriod++;
                numOfAllBunnies++;
            }

        }

        if(gameTime!=TimeForTTFDraw)
        {
            itoa(gameTime, tmpText, 10);
            fontPointer->ChangeText(tmpText);
            TimeForTTFDraw=gameTime;
            if(gameTime==0) nextstep=false;
        }

// ---------------------->> ОТРИСОВКА <<----------------------
        Draw();

        fontPointer->Draw(mainSurface); // отрисовка обратного отсчета
        fontForScore->Draw(mainSurface); // отображение количества убитых кроликов
         //Update the screen
        if( SDL_Flip( mainSurface ) == -1 ) return 1;

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        //
    } // END while nextstep
    if
    return ((score*100/numOfBullets));
}

void
Game::ChangeScore(Font* font){
    score++;
    static char tmpText2[10]="";
    itoa(score, tmpText2, 10);
    font->ChangeText(tmpText2);
}
