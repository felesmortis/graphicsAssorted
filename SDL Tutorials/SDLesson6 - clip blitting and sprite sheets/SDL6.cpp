//headers
#include "SDL/SDL.h"
#include <string>
#include "SDL/SDL_image.h"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//surfaces
SDL_Surface *dots = NULL;
SDL_Surface *screen = NULL;

//events
SDL_Event event;

//portions to clip & blit
SDL_Rect clip[4];

SDL_Surface *load_image( std::string filename )
{
         //loaded image
         SDL_Surface* loadedImage = NULL;
         //optimized image
         SDL_Surface* optimizedImage = NULL;
         
         //load
         loadedImage = IMG_Load( filename.c_str() );
         
         //if load
         if( loadedImage != NULL )
         {
             //create optimized
             optimizedImage = SDL_DisplayFormat( loadedImage );
             
             //free
             SDL_FreeSurface( loadedImage );
             
             if( optimizedImage != NULL )
             {
                 //map the color key
                 Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );
                 
                 //Set all pixels of that color to transparent
                 SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
             }
         }
             
         return optimizedImage;    
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
     //holds offsets
     SDL_Rect offset;
     
     //get offsets
     offset.x = x;
     offset.y = y;
     
     //blit
     SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
     //init subs
     if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
         return false;
     
     //Set up screen
     screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
     
     //test
     if( screen == NULL )
         return false;
     
     //window caption
     SDL_WM_SetCaption( "Clipping", NULL );
     
     //finally
     return true;
}


bool load_files()
{
     //load
     
     dots = load_image( "dots.png" );
     if( dots == NULL )
         return false;
     return true;
}

void clean_up()
{
     SDL_FreeSurface( dots );
     
     SDL_Quit();
}

int main( int argc, char* args[] )
{
    //wait for quit
    bool quit = false;
    //init
    if( init() == false )
        return 1;
    
    //load
    if( load_files() == false )
        return 1;
    //clip for top left
    clip[0].x = 0;
    clip[0].y = 0;
    clip[0].w = 100;
    clip[0].h = 100;
    //top right
    clip[1].x = 100;
    clip[1].y = 0;
    clip[1].w = 100;
    clip[1].h = 100;
    //bottom left
    clip[2].x = 0;
    clip[2].y = 100;
    clip[2].w = 100;
    clip[2].h = 100;
    //bottom right
    clip[3].x = 100;
    clip[3].y = 100;
    clip[3].w = 100;
    clip[3].h = 100;
    
    //fill screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    //apply
    apply_surface( 0, 0, dots, screen, &clip[0] );
    apply_surface( 540, 0, dots, screen, &clip[1] );
    apply_surface( 0, 380, dots, screen, &clip[2] );
    apply_surface( 540, 380, dots, screen, &clip[3] );
    
    //update
    if( SDL_Flip( screen ) == -1 )
        return 1;
    
    while( quit == false )
    {
           //while event
           while( SDL_PollEvent( &event ) )
           {
                if( event.type == SDL_QUIT )
                    quit = true;
           }
    }
    clean_up();
    
    return 0;
}
