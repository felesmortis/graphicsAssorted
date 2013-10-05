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
SDL_Surface *upMessage = NULL;
SDL_Surface *downMessage = NULL;
SDL_Surface *leftMessage = NULL;
SDL_Surface *rightMessage = NULL;
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
     SDL_WM_SetCaption( "Event Test", NULL );
     //init fine
     return true;
}

bool load_files()
{
     //Load bg image
     background = load_image( "background.png" );
     
     //load font
     font = TTF_OpenFont( "lazy.ttf", 72 );
     
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
     
     upMessage = TTF_RenderText_Solid( font, "Up was pressed", textColor );
     downMessage = TTF_RenderText_Solid( font, "Down was pressed", textColor );
     leftMessage = TTF_RenderText_Solid( font, "Left was pressed", textColor );
     rightMessage = TTF_RenderText_Solid( font, "Right was pressed", textColor );
     
     if( upMessage == NULL )
         return 1;
     if( downMessage == NULL )
         return 1;
     if( leftMessage == NULL )
         return 1;
     if( rightMessage == NULL )
         return 1;
     
     apply_surface( 0, 0, background, screen );
     
     if( SDL_Flip(screen) == -1 )
         return 1;
     
     while( quit == false)
     {
            while( SDL_PollEvent(&event) )
            {
                   //if key
                   if( event.type == SDL_KEYDOWN )
                   {
                       switch( event.key.keysym.sym )
                       {
                       //set proper message surface
                       case SDLK_UP: message = upMessage; break;
                       case SDLK_DOWN: message = downMessage; break;
                       case SDLK_LEFT: message = leftMessage; break;
                       case SDLK_RIGHT: message = rightMessage; break;
                       }
                   }
                   if( event.type == SDL_QUIT )
                       quit = true;
            }
            if( message != NULL )
            {
                //Apply
                apply_surface( 0, 0, background, screen );
                apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 2, message, screen );
                message = NULL;
            }
            if( SDL_Flip(screen) == -1 )
                return 1;
     }
     clean_up();
     return 0;
}
