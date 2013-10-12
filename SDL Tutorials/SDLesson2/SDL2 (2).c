
//The headers
#include "SDL/SDL.h"
#include "string.h"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

SDL_Surface *load_image(std::string filename)
{
    //Temp storage for the image
    SDL_Surface* loadedImage = NULL;

    //Optimized image - final
    SDL_Surface* optimizedImage = NULL;

    //Load
    loadedImage = SDL_LoadBMP(filename.c_str());
    if(loadedImage != NULL)
    {
        //Create optimized img
        optimizedImage = SDL_DisplayFormat(loadedImage);

        //Free up old img
        SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
    //temp rect for the offset;
    SDL_Rect offset;

    //set offsets
    offset.x = x;
    offset.y = y;

    //Blit surface
    SDL_BlitSurface(source, NULL, destination, &offset)
}

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        return 1;
    }

    //set up screen
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if(screen == NULL)
        return 1;

    //Set window Caption
    SDL_SM_SetCaption("Hello World", NULL);

    //Load images
    message = load_image("hello.bmp");
    background = load("background.bmp");

    //Apply the background to the screen
    apply_surface( 0, 0, background, screen);
    apply_surface(320, 0, background, screen);
    apply_surface(0, 240, background, screen);
    apply_surface(320, 240, background, screen);

    //Apply message
    apply_surface(180, 140, message, screen);

    //Update
    if(SDL_Flip(screen) == -1)
        return 1;

    //Wait
    SDL_Delay(2000);

    //Free surface
    SDL_FreeSurface(message);
    SDL)FreeSurface(background);

    //Quit SDL
    SLD_Quit();

    //Return
    return 0;
}
