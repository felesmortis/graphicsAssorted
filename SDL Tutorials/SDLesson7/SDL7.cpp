//headers
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

//screen attribs
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//surfaces
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;

//Event structure
SDL_Event event;

//font
TTF_Font *font = NULL;

//color
SDL_Color textColor = { 255, 255, 255 };

SDL_Surface *load_image(std::string filename)
{
            SDL_Surface* loadedimage = NULL;
            SDL_Surface* optimizedimage = NULL;
            loadedimage = IMG_Load( filename.c_str() );
            if( loadedimage != NULL )
            {
                optimizedimage = SDL_DisplayFormat( loadedimage );
                SDL_FreeSurface(loadedimage);
                
                if( optimizedimage != NULL )
                {
                    SDL_SetColorKey( optimizedimage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedimage->format, 0, 0xFF, 0xFF ) );
                }
            }
            return optimizedimage;
}

bool init()
{
     //init
     if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
     {
         return false;
     }
     
     //set up screen
     screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
     
     if( screen == NULL )
     {
         return false;
     }
     
     //ttf init
     if( TTF_Init() == -1 )
     {
         return false;
     }
     //set cap
     SDL_WM_SetCaption( "TTF Test", NULL );
     //init fine
     return true;
}

bool load_files()
{
     //Load bg image
     background = load_image( "background.png" );
     
     //load font
     font = TTF_OpenFont( "lazy.ttf", 28 );
     
     //if problem
     if( background == NULL )
     {
         return false;
     }
     if( font == NULL )
     {
         return false;
     }
     return true;
}
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
     SDL_Rect offset;
     
     offset.x = x;
     offset.y = y;
     
     SDL_BlitSurface( source, clip, destination, &offset );
} 

void clean_up()
{
     SDL_FreeSurface( background );
     SDL_FreeSurface( message );
     
     TTF_CloseFont( font );
}

int main( int argc, char* args[] )
{
     bool quit = false;
     
     if( init() == false )
         return 1;
     
     if( load_files() == false )
         return 1;
     
     message = TTF_RenderText_Solid( font, "The quick brown fox jumps over the lazy dog.", textColor );
     
     if( message == NULL )
         return 1;
     
     apply_surface( 0, 0, background, screen );
     apply_surface( 0, 150, message, screen );
     
     if( SDL_Flip(screen) == -1 )
         return 1;
     
     while( quit == false)
     {
            while( SDL_PollEvent(&event) )
            {
                   if( event.type == SDL_QUIT )
                       quit = true;
            }
     }
     clean_up();
     return 0;
}
