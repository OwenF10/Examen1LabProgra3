#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL_mixer.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background, *character, *cuadro, *gameover, *start;
SDL_Rect rect_background, rect_character, rect_cuadro, rect_gameover, rect_start;
int velocity = 5;
bool game = true;
bool startScreen = true;

using namespace std;

int randomBetween(int min, int max)
{
    return (rand()%(max - min)) + min;
}

bool collisionCheck(SDL_Rect r1, SDL_Rect r2)
{
    if(r1.x > r2.x+r2.w)
        return false;
    if(r1.x+r1.w < r2.x)
        return false;
    if(r1.y > r2.y+r2.h)
        return false;
    if(r1.y+r1.h < r2.y)
        return false;
    return true;
}
int main( int argc, char* args[] )
{
    srand(time(0));
    double startPosX = 385;
    double startPosY = 255;
    int posX = randomBetween(25, 715);
    int posY = randomBetween(93, 456);
    int dir = 3;
    int frames = 0;
    int cant = 0;
    Mix_Chunk *gEat = Mix_LoadWAV( "awesome.wav" );

    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Snake", 100, 100, 797,/*WIDTH*/ 554,/*HEIGHT*/ SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    SDL_Init( SDL_INIT_AUDIO );
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    Mix_Music *Music= Mix_LoadMUS( "unity.wav" );
    Mix_PlayMusic(Music, -1 );

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"tablero.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0; rect_background.y = 0; rect_background.w = w; rect_background.h = h;

    start = IMG_LoadTexture(renderer,"start.png");
    SDL_QueryTexture(start, NULL, NULL, &w, &h);
    rect_start.x = 0; rect_start.y = 0; rect_start.w = w; rect_start.h = h;

    character = IMG_LoadTexture(renderer, "snake.png");
    SDL_QueryTexture(character, NULL, NULL, &w, &h);
    rect_character.x = startPosX; rect_character.y = startPosY; rect_character.w = w; rect_character.h = h;

    cuadro = IMG_LoadTexture(renderer, "cuadrito.png");
    SDL_QueryTexture(cuadro, NULL, NULL, &w, &h);
    rect_cuadro.x = posX; rect_cuadro.y = posY; rect_cuadro.w = w; rect_cuadro.h = h;

    gameover = IMG_LoadTexture(renderer, "gameover.png");
    SDL_QueryTexture(gameover, NULL, NULL, &w, &h);
    rect_gameover.x = 0; rect_gameover.y = 0; rect_gameover.w = w; rect_gameover.h = h;

    //Main Loop
    while(true)
    {
        frames++;
        while(SDL_PollEvent(&Event))
        {
            if(collisionCheck(rect_cuadro, rect_character))
            {
                Mix_PlayChannel( -1, gEat, 0 );
                velocity++;
                posX = randomBetween(25, 715);
                posY = randomBetween(93, 456);
                rect_cuadro.x = posX; rect_cuadro.y = posY;
                cant++;
            }

            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                    return 0;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_LEFT)
                    dir = 4;
                if(Event.key.keysym.sym == SDLK_RIGHT)
                    dir = 3;
                if(Event.key.keysym.sym == SDLK_UP)
                    dir = 1;
                if(Event.key.keysym.sym == SDLK_DOWN)
                    dir = 2;
                if(Event.key.keysym.sym == SDLK_RETURN && !game){
                    game = true;
                    velocity = 5;
                    rect_character.x = startPosX; rect_character.y = startPosY;
                    posX = randomBetween(25, 715);
                    posY = randomBetween(93, 456);
                    rect_cuadro.x = posX; rect_cuadro.y = posY;
                }
            }
        }
        if(frames == 100)
            {
            if(dir == 1)
            {
                rect_character.y -= velocity;
            }
            else if(dir == 2)
            {
                rect_character.y += velocity;
            }
            else if(dir == 3)
            {
                rect_character.x += velocity;
            }
            else if(dir == 4)
            {
                rect_character.x -= velocity;
            }
            frames = 0;
        }
        if(rect_character.x <= 25)
            rect_character.x = 25;
        if(rect_character.x >= 715)
            rect_character.x = 715;
        if(rect_character.y <= 93)
            rect_character.y = 93;
        if(rect_character.y >= 456)
            rect_character.y = 456;

        if((cant == 25 || rect_character.x == 25 || rect_character.x == 715 ||
            rect_character.y == 93  || rect_character.y == 456) && game)
        {
            cant = 0;
            SDL_RenderCopy(renderer, gameover, NULL, &rect_gameover);
            game = false;
        }

        else if(game)
        {
            SDL_RenderCopy(renderer, background, NULL, &rect_background);
            SDL_RenderCopy(renderer, character, NULL, &rect_character);
            SDL_RenderCopy(renderer, cuadro, NULL, &rect_cuadro);
        }

        SDL_RenderPresent(renderer);
    }
	return 0;
}
