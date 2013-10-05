//headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//the surfaces
SDL_Surface *foo = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

//even structure
SDL_Event event;

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

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
     //temp rect
     SDL_Rect offset;
     
     //get offsets
     offset.x = x;
     offset.y = y;
     
     //blit
     SDL_BlitSurface( source, NULL, destination, &offset );
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
     SDL_WM_SetCaption( "Event test", NULL );
     
     //finally
     return true;
}


bool load_files()
{
     //load
     foo = load_image( "foo.png" );
     
     if( foo == NULL )
         return false;
     background = load_image( "background.png" );
     if( background == NULL )
         return false;
     return true;
}

void clean_up()
{
     SDL_FreeSurface( foo );
     SDL_FreeSurface( background );
     
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
    
    //apply
    apply_surface( 0, 0, background, screen );
    apply_surface( 240, 190, foo, screen );
    
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
