//headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//the surfaces
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

//even structure
SDL_Event event;

SDL_Surface *load_image( std::string filename )
{
            //image
            SDL_Surface* loadedImage = NULL;
            //optimized
            SDL_Surface* optimizedImage = NULL;
            //loaded
            loadedImage = IMG_Load( filename.c_str() );
            
            //if imgae
            if( loadedImage != NULL )
            {
                //create optimized
                optimizedImage = SDL_DisplayFormat( loadedImage );
                //free old
                SDL_FreeSurface( loadedImage );
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
     image = load_image( "x.png" );
     
     if( image == NULL )
         return false;
     
     return true;
}

void clean_up()
{
     SDL_FreeSurface( image );
     
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
    apply_surface( 0, 0, image, screen );
    
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
