#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

SDL_Surface *load_image(std::string filename)
{
      //image
            SDL_Surface* loadedImage = NULL;
            //optimized
            SDL_Surface* optimizedImage = NULL;
            
            //load
            loadedImage = IMG_Load(filename.c_str());
            
            if(loadedImage != NULL)
            {
                           //create optimized
                           optimizedImage = SDL_DisplayFormat(loadedImage);
                           
                           //free old image
                           SDL_FreeSurface(loadedImage);
            }
            return optimizedImage;
            
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
     //offsets
     SDL_Rect offset;
     
     offset.x = x;
     offset.y = y;
     
     SDL_BlitSurface(source, NULL, destination, &offset);
}
bool init()
{
     //init
     if( SDL_Init(SDL_INIT_EVERYTHING) == -1)
         return false;
     screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
     if( screen == NULL)
         return false;
     
     SDL_WM_SetCaption("PNG test", NULL);
     
     return true;
}
void clean_up()
{
     SDL_FreeSurface(image);
     
     SDL_Quit();
}

int main( int argc, char* args[])
{
    //init
    if( init() == false )
        return 1;
    
    //load
    image = load_image("look.png");
    
    //test
    if( image == NULL )
        return 1;
    
    //apply
    apply_surface( 0, 0, image, screen );
    
    //update
    if( SDL_Flip( screen ) == -1 )
        return 1;
    
    //Wait
    SDL_Delay( 2000 );
    
    clean_up();
    
    return 0;
    
}
