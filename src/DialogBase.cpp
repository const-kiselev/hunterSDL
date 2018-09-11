#include "DialogBase.h"


 SDL_Surface *mainSurface = NULL;
 char * mainTTFname;
 SDL_Rect wall = {0,0,WINDOW_W, WINDOW_H};


Font::Font(int xInput, int yInput, int fontSizeInput, char* fontFileNameInput, char* textInput, SDL_Color *textColorInput){
    text_color = *textColorInput;
    posInScreenSurf = {xInput, yInput, 0, 0};
    urgentSurface = NULL;
    changes = true;
    text = textInput;
    fontFileName = new char[256];
    strcpy(fontFileName, fontFileNameInput);
    fontSize = fontSizeInput;
    bool ready = true;
         fprintf(stderr, "Unable to open font\n"), ready = false;
}

Font::~Font(){
    delete fontFileName;
    SDL_FreeSurface(urgentSurface);
}

int
Font::Draw(SDL_Surface* surf){
    if(urgentSurface == NULL || changes){
        FONT = TTF_OpenFont(fontFileName, fontSize);
        SDL_FreeSurface(urgentSurface);
        urgentSurface = NULL;
        urgentSurface = TTF_RenderUTF8_Blended(FONT, text, text_color);
        changes = false;
        TTF_CloseFont(FONT);
    }
    if (surf) SDL_BlitSurface(urgentSurface, NULL, surf,  &posInScreenSurf);
    return 0;
}

DialogBase::DialogBase(){
    background=     NULL;
    urgentSurface=  NULL;
    fontPointer=    NULL;
}

DialogBase::~DialogBase(){
    //dtor
}

Run::Run(){
    bool ready = true;
    if( SDL_Init(SDL_INIT_EVERYTHING ) < 0 )
        fprintf(stderr,"Unable to init SDL: %s\n", SDL_GetError()), ready = false;
    if (TTF_Init())
        fprintf(stderr, "Unable to init SDL_TTF\n" ), ready = false;
    mainSurface = SDL_SetVideoMode(WINDOW_W, WINDOW_H_WITH_TIPS, SCRDEPTH, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !mainSurface )
        fprintf(stderr,"Unable to set video: %s\n", SDL_GetError()), ready = false;
    if (ready)
    {
        mainTTFname = new char[100];
        strcpy(mainTTFname, "marvin.ttf");
        Rmask = mainSurface->format->Rmask;
        Gmask = mainSurface->format->Gmask;
        Bmask = mainSurface->format->Bmask;
        // Задаем заголовок окна и иконку
        SDL_WM_SetCaption("РћС…РѕС‚РЅРёРє", NULL); // Дракон над городом
        // Запускаем заставку
        {Saver* MySaver = new Saver(); delete MySaver;}

            Menu MyMenu;
            MyMenu.Launch();

    }
}

int
Menu::EnterName(){
    Rmask = mainSurface->format->Rmask;
    Gmask = mainSurface->format->Gmask;
    Bmask = mainSurface->format->Bmask;
    Tips SaverTip;
    SaverTip.ChangeTipText("РёСЃРїРѕР»СЊР·СѓР№С‚Рµ Р»Р°С‚РёРЅСЃРєСѓСЋ СЂР°СЃРєР»Р°РґРєСѓ РєР»Р°РІРёР°С‚СѓСЂС‹.");
    StringInput name;

    background =SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,WINDOW_W, WINDOW_H, 32,Rmask, Gmask, Bmask, 0xFF000000);
    SDL_Surface* tmp = IMG_Load("saver.gif");
    SDL_BlitSurface(tmp,NULL, background, NULL);
    SDL_Surface* tmp2 =SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,WINDOW_W, WINDOW_H, 32,Rmask, Gmask, Bmask, 0xFF000000);
    Draw_FillRect(tmp2,WINDOW_W/2-200,WINDOW_H/2-100, 400, 200, SDL_MapRGBA(background->format, 255, 255, 255,200));
    SDL_BlitSurface(tmp2,NULL, background, NULL);
    char enterTheName[] = "Р’РІРµРґРёС‚Рµ СЃРІРѕРµ РёРјСЏ:";
    SDL_Color txtColor = {0,0,0,0};
    Font* message = new Font(WINDOW_W/2-200, WINDOW_H/2-100, 20,mainTTFname, enterTheName,&txtColor);
    message->Draw(background);
    SDL_Event event;
    bool quit = false, nameEntered = false;
    while( quit == false )
    {
        while( SDL_PollEvent( &event ) )
        {
            if( nameEntered == false )
            {
                name.handle_input(event);
                if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                    nameEntered = true;
            }
            if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN )
                quit = true;
        }


        //Apply the background
        apply_surface( 0, 0, background, mainSurface, NULL);
        name.show_centered(mainSurface);

        //Update the screen
        if( SDL_Flip( mainSurface ) == -1 )
        {
            return 1;
        }
    }
    SDL_WaitEvent(&event);
    playerName = new char[256];
    strcpy(playerName, name.GetChar());
    fprintf(stderr, "Unable %s\n", playerName );
    SDL_BlitSurface(tmp,NULL, background, NULL);
    return (int)!strlen(playerName);
}

Run::~Run(){
    SDL_FreeSurface(mainSurface);
    delete mainTTFname;
    SDL_Quit();
    TTF_Quit();
}

Saver::Saver(){
    text_color = {255,255,255,0};
    // ФОН
    SDL_Surface* tmpImgSurf = IMG_Load("saver.gif");
    SDL_BlitSurface(tmpImgSurf, NULL, mainSurface, NULL);
    // ТЕКСТ
    char ttext[50] = "РћС…РѕС‚РЅРёРє РЅР° РєСЂРѕР»РёРєР°"; // Охотник за кроликами
    fontPointer = new Font(WINDOW_W/4+10,WINDOW_H-100, 30, mainTTFname, ttext, &text_color);
    fontPointer->Draw(mainSurface);
    delete fontPointer;
    strcpy(ttext, "РљСѓСЂСЃРѕРІР°СЏ СЂР°Р±РѕС‚Р°"); // Курсовая работа
    Font * coursWork = new Font(WINDOW_W/3+10,60, 24, mainTTFname, ttext, &text_color);
    coursWork->Draw(mainSurface);
    delete coursWork;
    strcpy(ttext, "Р¤С‘РґРѕСЂРѕРІР° Р”.Рћ. Рё954"); // Федорова Д.О. И954
    coursWork = new Font(WINDOW_W-220,8, 17, mainTTFname, ttext, &text_color);
    if(mainSurface) coursWork->Draw(mainSurface);
    delete coursWork;

    Tips SaverTip;
    SaverTip.ChangeTipText("2016. Р РѕСЃСЃРёСЏ, РЎРџР±, Р‘Р“РўРЈ В«Р’РћР•РќРњР•РҐВ», РєР°С„РµРґСЂР° Рё9 /// Р—Р°РіСЂСѓР·РєР° РёРіСЂС‹..."); // Загрузка...

    SDL_Flip(mainSurface); // обновление, для отображения основной поверхности на экране
    SDL_Delay(5000);
    SDL_FreeSurface(tmpImgSurf);

}

Menu::Menu(){
    urgentSurface = NULL;
    text_color = {255,255,255,0};
    SDL_Color select_text_color = {280, 0, 0,0}, text_color_menu = {200, 200, 200, 0};

    numOfElements = 5;
    menuName = new char[20];
    playerName = new char[20];
    for(int i=0; i<numOfElements; i++)
        menuElemets[i] = new char[30];

    strcpy(menuName, "РњРµРЅСЋ"); // Меню
    strcpy(menuElemets[0], "РќРѕРІР°СЏ РёРіСЂР°"); // Новая игра
    strcpy(menuElemets[1], "РџСЂР°РІРёР»Р°"); // Правила
    strcpy(menuElemets[2], "Р РµР·СѓР»СЊС‚Р°С‚С‹"); // Результаты
    strcpy(menuElemets[3], "РЎРјРµРЅР° РёРіСЂРѕРєР°"); // Смена игрока
    strcpy(menuElemets[4], "Р’С‹Р№С‚Рё"); // Выйти
    strcpy(playerName, "");
    // ФОН
    background = IMG_Load("saver.gif");

    // Создаем оюъекты текстов для отрисовки на поверхности
    menuNameFont = new Font(0,0, 20, mainTTFname, menuName, &text_color_menu);
    playerNameFont = new Font (WINDOW_W-200, 10,20, mainTTFname, playerName, &text_color_menu);
    for(int i=0; i<numOfElements; i++){
        menuFont[i] = new Font(0,20*i+20, 18, mainTTFname, menuElemets[i], &text_color);
        menuFontSelect[i] = new Font(0,20*i+20, 18, mainTTFname, menuElemets[i], &select_text_color);
    }
    currentPosInMenu = 0; // на "Начать игру"
}

int
Menu::Launch(){
    while(EnterName());
    allRES = new Results();
    allRES->ReadResults();
    playerNameFont->ChangeText(playerName);
    Tips SaverTip;
            SaverTip.ChangeTipText("Р”Р»СЏ РїРµСЂРµРґРІРёР¶РµРЅРёСЏ РїРѕ РјРµРЅСЋ РёСЃРїРѕР»СЊР·СѓР№С‚Рµ РєР»Р°РІРёС€Рё: РІРІРµСЂС…/РІРЅРёР·.");
    EventFilter();
    return 0;
}

int
Menu::Draw(){
    if(!urgentSurface)
    {
        Rmask = mainSurface->format->Rmask;
        Gmask = mainSurface->format->Gmask;
        Bmask = mainSurface->format->Bmask;
        urgentSurface = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,WINDOW_W, WINDOW_H, 32,Rmask, Gmask, Bmask, 0);
    }
    SDL_BlitSurface(background, NULL, urgentSurface, NULL); // отрисовка фона
    playerNameFont->Draw(urgentSurface); // отрисовка имени игрока
    menuNameFont->Draw(urgentSurface); // отрисовка "меню"
    for(int i=0; i<numOfElements; i++)
        if(currentPosInMenu == i)
            menuFontSelect[i]->Draw(urgentSurface);// отрисовка выделенного элемента меню
        else
            menuFont[i]->Draw(urgentSurface); // отрисовка элемента меню
    if(mainSurface) SDL_BlitSurface(urgentSurface, NULL, mainSurface, NULL);
    return 0;
}

int
Menu::EventFilter(){
    bool nextstep = true;
    Timer fps; // таймер используемый для вычисления количества кадров в секунду
    SDL_Event event;
    SDL_EnableKeyRepeat(1,10);
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
                switch(event.key.keysym.sym){}
//                action = true;
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                case SDLK_UP:
                    currentPosInMenu--;
                    if(currentPosInMenu == -1) currentPosInMenu = numOfElements-1;
                    break;
                case SDLK_DOWN:
                    currentPosInMenu = (currentPosInMenu+1)%numOfElements;
                    break;
                case SDLK_ESCAPE:
                    nextstep = false;
                    break;
                case SDLK_RETURN:
                    if(ChooseAction()) nextstep=false;
                    break;
                }
                break;

            } // switch
        }// if
// ---------------------->> ЛОГИКА <<----------------------


// ---------------------->> ОТРИСОВКА <<----------------------
        Draw();
        if( SDL_Flip( mainSurface ) == -1 ) return 1;
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );

        //
    } // END while nextstep
    allRES->WriteResults();
    return 0;
}

int
Menu::ChooseAction(){
    if (currentPosInMenu == numOfElements-1) return 1;
    switch(currentPosInMenu)
    {
        case 0: // Новая игра
            {
                Game HunterGame;
                int tmpRes;
                tmpRes = HunterGame.Launch();
                allRES->NewResult(playerName, tmpRes);
                }
            break;
        case 1: // Правила
            {
                Rules *gameReules = new Rules;
                delete gameReules;
            }
            break;
        case 2: // Результаты
            allRES->Draw();
            break;
        case 3: // Смена игрока
            while(EnterName());
            playerNameFont->ChangeText(playerName);
            break;
    }
    Draw();
    Tips SaverTip;
    SaverTip.ChangeTipText("Р”Р»СЏ РїРµСЂРµРґРІРёР¶РµРЅРёСЏ РїРѕ РјРµРЅСЋ РёСЃРїРѕР»СЊР·СѓР№С‚Рµ РєР»Р°РІРёС€Рё: РІРІРµСЂС…/РІРЅРёР·.");

        return 0;
}

Menu::~Menu(){
    SDL_FreeSurface(background);
    SDL_FreeSurface(urgentSurface);
    for(int i=0; i<numOfElements; i++)
    {
        delete menuElemets[i];
        delete menuFont[i];
        delete menuFontSelect[i];
    }
    delete menuNameFont;
    delete menuName;
}

Tips::Tips(){
    tipText = new char[256];
    strcpy(tipText,"");
    char tmpChar[20] = "calibri.ttf";
    text_color = {255, 255, 255,0};
    objRectData = {0, WINDOW_H,WINDOW_W, WINDOW_H_WITH_TIPS-WINDOW_H};
    background = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,objRectData.w, objRectData.h, 32,Rmask, Gmask, Bmask, 0);
    Draw_FillRect(background,0,0,objRectData.w, objRectData.h,SDL_MapRGB(background->format,0,0,0));
    fontPointer = new Font(0,0, 14, tmpChar, tipText, &text_color);
}

Tips::~Tips(){
    delete tipText;
    SDL_FreeSurface(background);
    SDL_FreeSurface(urgentSurface);
    delete fontPointer;
}

int
Tips::Draw(){
    if(!urgentSurface){
        Rmask = mainSurface->format->Rmask;
        Gmask = mainSurface->format->Gmask;
        Bmask = mainSurface->format->Bmask;
        urgentSurface = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,objRectData.w, objRectData.h, 32,Rmask, Gmask, Bmask, 0);
    }
    SDL_BlitSurface(background, NULL, urgentSurface, NULL); // отрисовка фона
    fontPointer->Draw(urgentSurface);
    if(mainSurface) SDL_BlitSurface(urgentSurface, NULL, mainSurface, &objRectData);
    return 0;
}

int
Tips::ChangeTipText(char *inputText){
    strcpy(tipText, inputText);
    Draw();
    return 0;
}

Timer::Timer(){
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}
void
Timer::start(){
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}
void
Timer::stop(){
    started = false;
    paused = false;
}
int
Timer::get_ticks(){
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}
void
Timer::pause(){
    if( ( started == true ) && ( paused == false ) )
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void
Timer::unpause(){
    if( paused == true )
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}
bool
Timer::is_started(){
    return started;
}
bool
Timer::is_paused(){
    return paused;
}

StringInput::StringInput(){
    str = "";
    text = NULL;
    SDL_EnableUNICODE( SDL_ENABLE );
}

StringInput::~StringInput(){
    SDL_FreeSurface( text );
    SDL_EnableUNICODE( SDL_DISABLE );
}

void
StringInput::handle_input(SDL_Event event){
SDL_EnableKeyRepeat(1,1);
    if( event.type == SDL_KEYDOWN )
    {
        std::string temp = str;

        if( str.length() <= 10 )
        {
            if( event.key.keysym.unicode == (Uint16)' ' )
                str += (char)event.key.keysym.unicode;
            else if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) )
                str += (char)event.key.keysym.unicode;
            else if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )
                str += (char)event.key.keysym.unicode;
            else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) )
                str += (char)event.key.keysym.unicode;
        }

        if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( str.length() != 0 ) )
            str.erase( str.length() - 1 );

        if( str != temp )
        {
            SDL_FreeSurface( text );
            TTF_Font *fnt = TTF_OpenFont(mainTTFname, 20);
            text = TTF_RenderText_Blended( fnt, str.c_str(), {0, 0, 0, 0});
            TTF_CloseFont(fnt);
        }
    }
}

void
StringInput::show_centered(SDL_Surface * surf){
    if( text != NULL )
    {
        apply_surface( ( WINDOW_W - text->w ) / 2, ( WINDOW_H - text->h ) / 2, text, surf, NULL);
    }
}

void
apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ){
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool
Collision(SDL_Rect A, SDL_Rect B){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    // Вычисляем стороны фигуры А
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;
    // Вычисляем стороны фигуры В
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;
    //If any of the sides from A are outside of B
    if( bottomA <= topB ) return false;
    if( topA >= bottomB ) return false;
    if( rightA <= leftB ) return false;
    if( leftA >= rightB ) return false;
    //If none of the sides from A are outside B
    return true;
}

int
Results::NewResult(char *name, int resultInt) {
    int i;
    for(i =0; i<10; i++)
        if(arrayOfResults[i].r)
            if(resultInt>arrayOfResults[i].r)
            {
                resData tmp;
                if(i!=9) tmp = arrayOfResults[i];
                arrayOfResults[i].r = resultInt;
                strcpy(arrayOfResults[i].name, name);
                i++;
                if(i!=10) arrayOfResults[i]=tmp;
                i=10;
            }
        else {
        	arrayOfResults[i].r = resultInt;
            strcpy(arrayOfResults[i].name, name);
            //i=10;
		}
    ShowResult(name, resultInt);
    return 0;
}

bool
Results::ReadResults(){
	int i=0;
	if ((output=fopen (fileName, "rb"))==NULL)
     {
          perror ("Error open file");
          return 0;
     }
     for(i=0; i<10 && !feof(output); i++)
        fscanf(output, "%s %i", arrayOfResults[i].name, &(arrayOfResults[i].r)),
        printf ("%s %d\n", arrayOfResults[i].name, arrayOfResults[i].r);
    fclose(output);
	return 1;
}

bool
Results::WriteResults(){
	int i=0;
	if ((output=fopen (fileName, "w"))==NULL)
     {

          perror ("Error open file");
          return 0;
     }
     for(i=0; i<10 ; i++)
     	if(arrayOfResults[i].r!=0)fprintf(output, "%s %d", arrayOfResults[i].name, arrayOfResults[i].r);
    fclose(output);
	return 1;
}

void
Results::ShowResult(char* printName, int printRes){
    text_color = {255,255,255,0};
    char tmpText[10]="";
    // ФОН
    SDL_Surface* tmpImgSurf = IMG_Load("saver.gif");
    SDL_BlitSurface(tmpImgSurf, NULL, mainSurface, NULL);
    // ТЕКСТ
    char ttext[50] = "Р’Р°С€ СЂРµР·СѓР»СЊС‚Р°С‚:  "; // Охотник за кроликами
    itoa(printRes, tmpText, 10);
    strcat(ttext, tmpText);
    fontPointer = new Font(WINDOW_W/4+10,WINDOW_H/2, 30, mainTTFname, ttext, &text_color);
    fontPointer->Draw(mainSurface);
    delete fontPointer;

    Tips SaverTip;
    SaverTip.ChangeTipText("РћР¶РёРґР°Р№С‚Рµ РїРµСЂРµС…РѕРґР° РІ РјРµРЅСЋ."); // Загрузка...

    SDL_Flip(mainSurface); // обновление, для отображения основной поверхности на экране
    SDL_Delay(5000);
    SDL_FreeSurface(tmpImgSurf);
}

Results::Results(){
    fileName = new char[30];
    strcpy(fileName, "results.txt");
    strcpy(arrayOfResults[0].name, "hello");
    arrayOfResults[0].r=1;
    for(int i=1;i<10;i++)
    {
        strcpy(arrayOfResults[i].name, "");
        arrayOfResults[i].r=0;
    }
}

Results::~Results(){};

int
Results::Draw(){
    text_color = {200,200,200,0};
    char tmpText[10]="";
    // ФОН
    SDL_Surface* tmpImgSurf = IMG_Load("saver.gif");
    SDL_BlitSurface(tmpImgSurf, NULL, mainSurface, NULL);
    // ТЕКСТ
    char ttext[50] = "10 Р»СѓС‡С€РёС… СЂРµР·СѓР»СЊС‚Р°С‚РѕРІ:";
    Font * tmpFontRes = new Font(WINDOW_W/4+10,160-23, 24, mainTTFname, ttext, &text_color);
    tmpFontRes->Draw(mainSurface);
    delete tmpFontRes;

    text_color = {255,255,255,0};

    for(int i=0; i<10;i++)
    {
        strcpy(ttext, arrayOfResults[i].name);
        strcat(ttext, " ");
        itoa(arrayOfResults[i].r, tmpText, 10);
        strcat(ttext, tmpText);
        Font * tmpFontRes = new Font(WINDOW_W/3+10,160+i*23, 24, mainTTFname, ttext, &text_color);
        tmpFontRes->Draw(mainSurface);
        delete tmpFontRes;
    }
    Tips SaverTip;
    SaverTip.ChangeTipText("РћР¶РёРґР°Р№С‚Рµ РїРµСЂРµС…РѕРґР° РІ РјРµРЅСЋ."); // Загрузка...

    SDL_Flip(mainSurface); // обновление, для отображения основной поверхности на экране
    SDL_Delay(5000);
    SDL_FreeSurface(tmpImgSurf);

    return 0;
}

Rules::Rules(){
    text_color = {255,255,255,0};
    // ФОН
    SDL_Surface* tmpImgSurf = IMG_Load("saver.gif");
    SDL_BlitSurface(tmpImgSurf, NULL, mainSurface, NULL);

    char ttext[256] = "РџРѕ РїРµСЂРёРјРµС‚СЂСѓ \"РєРѕРјРЅР°С‚С‹\" РєСЂРѕР»РёРєРё СЃР»СѓС‡Р°Р№РЅС‹Рј РѕР±СЂР°Р·РѕРј РїСЂРѕРіСЂС‹Р·Р°СЋС‚ ";
    Font * tmpFontRes = new Font(50,23, 18, mainTTFname, ttext, &text_color);
    tmpFontRes->Draw(mainSurface);
    delete tmpFontRes;

    strcpy(ttext, "РЅРѕСЂРєРё Рё РєСЂР°С‚РєРѕРІСЂРµРјРµРЅРЅРѕ РѕСЃС‚Р°СЋС‚СЃСЏ РІ РЅРёС…. РЎ РїРѕРјРѕС‰СЊСЋ РєР»Р°РІРёС€ ");
    tmpFontRes = new Font(50,20*2, 18, mainTTFname, ttext, &text_color);
    tmpFontRes->Draw(mainSurface);
    delete tmpFontRes;

    strcpy(ttext, "СѓРїСЂР°РІР»РµРЅРёСЏ (СЃС‚СЂРµР»РєРё) РёРіСЂРѕРє РїРµСЂРµРґРІРёРіР°РµС‚ РѕС…РѕС‚РЅРёРєР°. РќР°Р¶Р°С‚РёРµ ");
    tmpFontRes = new Font(50,20*3, 18, mainTTFname, ttext, &text_color);
    tmpFontRes->Draw(mainSurface);
    delete tmpFontRes;

    strcpy(ttext, "РєР»Р°РІРёС€ \"РџСЂРѕР±РµР»\" СЃРѕРѕС‚РІРµС‚СЃРІСѓРµС‚ РІС‹СЃС‚СЂРµР»Сѓ. РџСѓР»СЏ Р»РµС‚РёС‚ РІ РЅР°РїСЂР°РІР»РµРЅРёРё, ");
    tmpFontRes = new Font(50,20*4, 18, mainTTFname, ttext, &text_color);
    tmpFontRes->Draw(mainSurface);
    delete tmpFontRes;

    strcpy(ttext, "РѕРїСЂРµРґРµР»СЏРµРјРѕРј РїРѕР»РѕР¶РµРЅРёРµРј СЂСѓР¶СЊСЏ. Р¦РµР»СЊ РёРіСЂС‹ вЂ” РѕС‚СЃС‚СЂРµР»РёС‚СЊ РєР°Рє");
    tmpFontRes = new Font(50,20*5, 18, mainTTFname, ttext, &text_color);
    tmpFontRes->Draw(mainSurface);
    delete tmpFontRes;

    strcpy(ttext, "РјРѕР¶РЅРѕ Р±РѕР»СЊС€Рµ РєСЂРѕР»РёРєРѕРІ Р·Р° 1 РјРёРЅСѓС‚Сѓ.");
    tmpFontRes = new Font(50,20*6, 18, mainTTFname, ttext, &text_color);
    tmpFontRes->Draw(mainSurface);
    delete tmpFontRes;

    Tips SaverTip;
    SaverTip.ChangeTipText("РћР¶РёРґР°Р№С‚Рµ РїРµСЂРµС…РѕРґР° РІ РјРµРЅСЋ."); // Загрузка...

    SDL_Flip(mainSurface); // обновление, для отображения основной поверхности на экране
    SDL_Delay(8000);
    SDL_FreeSurface(tmpImgSurf);

}

Rules::~Rules(){}
