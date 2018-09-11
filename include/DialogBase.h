#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <fstream>

#include "Game.h"

#define WINDOW_W 800
#define WINDOW_H 677
#define SCRDEPTH 32

#define WINDOW_H_WITH_TIPS 695

#define FRAMES_PER_SECOND 200

using namespace std;

extern SDL_Surface *mainSurface; // указатель на главную поверхность
extern char * mainTTFname;
extern SDL_Rect wall;
int Quit();
void apply_surface(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);

class Font;
///////////// DialogBase            /////////////
class DialogBase{
public:
    DialogBase();
    virtual ~DialogBase();
    virtual int Launch()=0;
    virtual int Draw()=0;
    virtual int EventFilter()=0;
protected:
    SDL_Surface *urgentSurface, *background; // актуальная поверхность, фон
    Uint32 Rmask, Gmask, Bmask;
    Font *fontPointer; // указатель на поверхность с текстом
    SDL_Color text_color;
};

// Запуск игры
class Run: public DialogBase {
public:
    Run();
    ~Run();
    int Launch(){return 0;};
    int Draw(){return 0;};
    int EventFilter(){return 0;};
private:

};
// Заставка
class Saver: public DialogBase {
public:
    Saver();
    int Launch(){return 0;};
    int Draw(){return 0;};
    int EventFilter(){return 0;};
};

class Results: public DialogBase {
public:
    Results();
    ~Results();
    int NewResult(char *name, int res);
    bool WriteResults();
    bool ReadResults();

    int Launch(){return 0;};
    int Draw();
    int EventFilter(){return 0;};
private:
    void ShowResult(char* printName, int printRes);
    struct resData {
        char name[11];
        int r;
    };
    resData arrayOfResults[10];
    FILE *output;
    char *fileName;

};

///////////// Menu:DialogBase       /////////////
class Menu: public DialogBase {
public:
    Menu();
    ~Menu();
    int Launch();
    int Draw();
    int EventFilter();
private:
    int ChooseAction(); // Выбор действия
    int currentPosInMenu, numOfElements; // текущий пункт меню, количество элементов в меню
    char * menuElemets[5], *menuName; // указатели на текст, который будет отображаться в меню в кодировке UTF8
    Font * menuFont[5], *menuNameFont, *menuFontSelect[5], *playerNameFont; // Указатели на поверхности текстовых объектов
    char *playerName;
    int EnterName();
    Results *allRES;
};

class Tips: public DialogBase {
public:
    Tips();
    ~Tips();
    int Launch(){return 0;};
    int Draw();
    int EventFilter(){return 0;};
    int ChangeTipText(char*);
private:
    char *tipText;
    SDL_Rect objRectData;
};

class Rules: public DialogBase {
public:
    Rules();
    ~Rules();
    int Launch(){return 0;};
    int Draw(){return 0;};
    int EventFilter(){return 0;};
};


// Класс, работающий с шрифтом, текстом и его отрисовкой на поверхности
class Font {
public:
    ~Font();
    Font(int xInput, int yInput, int fontSizeInput, char* fontFileName, char* textInput, SDL_Color *textColorInput);
    int Draw(SDL_Surface* surf);
    void ChangeText(char*input) {text = input; changes = true;};
private:
    char *text, *fontFileName;
    SDL_Surface * urgentSurface;
    SDL_Rect objRectData, posInScreenSurf;
    bool changes;
    TTF_Font *FONT;
    SDL_Color text_color;
    int fontSize;
};

class Timer {
private:
    int startTicks;
    int pausedTicks;

    bool paused;
    bool started;

    public:
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    int get_ticks();

    bool is_started();
    bool is_paused();
};

class StringInput {
private:
    string str;
    SDL_Surface *text;

public:
    StringInput();
    ~StringInput();
    void handle_input(SDL_Event event);
    void show_centered(SDL_Surface*);
    const char* GetChar(){return str.c_str();};
};

bool Collision(SDL_Rect A, SDL_Rect B);
#endif // DIALOGBASE_H
