//headers
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

//screen attribs
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//fps
const int FRAMES_PER_SECOND = 20;

//dot
const int SQUARE_HEIGHT = 20;
const int SQUARE_WIDTH = 20;

const int FOO_HEIGHT = 205;
const int FOO_WIDTH = 64;

const int FOO_LEFT = 1;
const int FOO_RIGHT = 0;

//button states
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

//surfaces
//SDL_Surface *square = NULL;
SDL_Surface *foo = NULL;
SDL_Surface *fpsmessage = NULL;
SDL_Surface *screen = NULL;

//the wall
SDL_Rect wall;

//Event structure
SDL_Event event;

//font
TTF_Font *font = NULL;

//color
SDL_Color textColor = { 0, 0, 0 };

SDL_Rect clipsRight[ 4 ];
SDL_Rect clipsLeft[ 4 ];

//circle struct
struct Circle
{
       int x, y, r;
};
class Foo
{
      private:
      int offSet;
      
      int velocity;
      
      int frame;
      
      int status;
      
      public:
      Foo();
      void handle_events();
      void move();
      void show();
};

//timer
class Timer
{
      private:
      //clock time
      int startTicks;
      
      //paused
      int pausedTicks;
      
      //timer status
      bool paused;
      bool started;
      
      public:
      //init
      Timer();
      
      //actions
      void start();
      void stop();
      void pause();
      void unpause();
      
      //get time
      int get_ticks();
      
      //status
      bool is_started();
      bool is_paused();
      
};
//the button
class Button
{
      private:
      //attribs
      SDL_Rect box;
      //buttonspritesheet that will be show.
      SDL_Rect* clip;
      
      public:
      
      //init
      Button( int x, int y, int w, int h );
      
      //handles events and sets the buttons sprtie region
      void handle_events();
      
      //shows button on screen
      void show();
};

double distance( int x1, int y1, int x2, int y2 )
{
       return sqrt( pow( x2 - x1, 2) + pow( y2 - y1, 2 ) );
}

bool check_collision( Circle &A, Circle &B )
{
     if( distance( A.x, A.y, B.x, B.y ) < ( A.r + B.r ) )
         return true;
     return false;
}
bool check_collision( Circle &A, std::vector<SDL_Rect> &B )
{
     int cX, cY;
     
     for( int Bbox = 0; Bbox < B.size(); Bbox++ )
     {
          if( A.x < B[ Bbox ].x )
              cX = B[ Bbox ].x;
          else if( A.x > B[ Bbox ].x + B[ Bbox ].w )
               cX = B[ Bbox ].x + B[ Bbox ].w;
          else
              cX = A.x;
          if( A.y < B[ Bbox ].y )
              cY = B[ Bbox ].y;
          else if( A.y > B[ Bbox ].y + B[ Bbox ].h )
               cY = B[ Bbox ].y + B[ Bbox ].h;
          else
              cY = A.y;
              
          if( distance( A.x, A.y, cX, cY ) < A.r )
              return true;
     }
     return false;
}

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
     /*
     //audio init
     if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4069 ) == -1 )
         return false;*/
     //set cap
     SDL_WM_SetCaption( "Animation", NULL );
     //init fine
     return true;
}

bool load_files()
{
     //Load bg image
     foo = load_image( "foo.png" );
     
     //load font
     font = TTF_OpenFont( "lazy.ttf", 24 );
     
     //if problem
     if( foo == NULL )
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
     
     SDL_FreeSurface( foo );
     SDL_FreeSurface( fpsmessage );
     
     TTF_CloseFont( font );
     
     TTF_Quit();
     
     SDL_Quit();
}

Timer::Timer()
{
              //init
              startTicks = 0;
              pausedTicks = 0;
              paused = false;
              started = false;
}
void Timer::start()
{
     //start
     started = true;
     
     //unpause
     paused = false;
     
     //get current time
     startTicks = SDL_GetTicks();
}
void Timer::stop()
{
     //stop
     started = false;
     
     //unpause
     paused = false;
}
int Timer::get_ticks()
{
    //if timer
    if( started == true )
    {
        if( paused == true )
        {
            //return ticks
            return pausedTicks;
        }
        else
            return SDL_GetTicks() - startTicks;
    }
    return 0;
}
void Timer::pause()
{
     //if running and not paused
     if( ( started == true ) && ( paused == false ) )
     {
         paused = true;
         pausedTicks = SDL_GetTicks() - startTicks;
     }
}
void Timer::unpause()
{
     if( paused == true )
     {
         paused = false;
         startTicks = SDL_GetTicks() - pausedTicks;
         pausedTicks = 0;
     }
}
bool Timer::is_started()
{
     return started;
}
bool Timer::is_paused()
{
     return paused;
}

Foo::Foo()
{
          offSet = 0;
          velocity = 0;
          
          frame = 0;
          status = FOO_RIGHT;
}
void Foo::move()
{
     offSet += velocity;
     
     if( ( offSet < 0 ) || ( offSet + FOO_WIDTH > SCREEN_WIDTH ) )
         offSet -= velocity;
}
void Foo::show()
{
     if( velocity < 0 )
     {
         status = FOO_LEFT;
         
         frame++;
     }
     else if( velocity > 0 )
     {
          status = FOO_RIGHT;
          
          frame++;
     }
     else
         frame = 0;
         
     if( frame >= 4 )
         frame = 0;
         
     if( status == FOO_RIGHT )
         apply_surface( offSet, SCREEN_HEIGHT - FOO_HEIGHT, foo, screen, &clipsRight[frame] );
     else if( status == FOO_LEFT )
          apply_surface( offSet, SCREEN_HEIGHT - FOO_HEIGHT, foo, screen, &clipsLeft[frame] );
}
void Foo::handle_events()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_RIGHT: velocity += FOO_WIDTH / 4; break;
            case SDLK_LEFT: velocity -= FOO_WIDTH / 4; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_RIGHT: velocity -= FOO_WIDTH / 4; break;
            case SDLK_LEFT: velocity += FOO_WIDTH / 4; break;
        }
    }
}

void set_clips()
{
     //clip the sprite sheet
     clipsRight[0].x = 0;
     clipsRight[0].y = 0;
     clipsRight[0].w = FOO_WIDTH;
     clipsRight[0].h = FOO_HEIGHT;
     
     clipsRight[1].x = FOO_WIDTH;
     clipsRight[1].y = 0;
     clipsRight[1].w = FOO_WIDTH;
     clipsRight[1].h = FOO_HEIGHT;
     
     clipsRight[2].x = FOO_WIDTH * 2;
     clipsRight[2].y = 0;
     clipsRight[2].w = FOO_WIDTH;
     clipsRight[2].h = FOO_HEIGHT;
     
     clipsRight[3].x = FOO_WIDTH * 3;
     clipsRight[3].y = 0;
     clipsRight[3].w = FOO_WIDTH;
     clipsRight[3].h = FOO_HEIGHT;
     
     clipsLeft[0].x = 0;
     clipsLeft[0].y = FOO_HEIGHT;
     clipsLeft[0].w = FOO_WIDTH;
     clipsLeft[0].h = FOO_HEIGHT;
     
     clipsLeft[1].x = FOO_WIDTH;
     clipsLeft[1].y = FOO_HEIGHT;
     clipsLeft[1].w = FOO_WIDTH;
     clipsLeft[1].h = FOO_HEIGHT;
     
     clipsLeft[2].x = FOO_WIDTH * 2;
     clipsLeft[2].y = FOO_HEIGHT;
     clipsLeft[2].w = FOO_WIDTH;
     clipsLeft[2].h = FOO_HEIGHT;
     
     clipsLeft[3].x = FOO_WIDTH * 3;
     clipsLeft[3].y = FOO_HEIGHT;
     clipsLeft[3].w = FOO_WIDTH;
     clipsLeft[3].h = FOO_HEIGHT;
}
int main( int argc, char* args[] )
{
     bool quit = false;
     
     int frame = 0;
     
     bool cap = true;
     
     Timer fps;
     Timer update;
     
     
     
     if( init() == false )
         return 1;
     
     if( load_files() == false )
         return 1;
     set_clips();
     Foo walk;
     
     update.start();
     
     while( quit == false)
     {
            fps.start();
            while( SDL_PollEvent(&event) )
            {
                   walk.handle_events();
                                  
                   if( event.type == SDL_QUIT )
                       quit = true;
            }
            
            walk.move();
            //std::stringstream fpsm;
            //fpsm << "FPS: " << frame / ( fps.get_ticks() / 1000.f );
            
            SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
            walk.show();
            
            if( update.get_ticks() > 1000.f )
            {
                
                std::stringstream fpsm;
                
                fpsm << "FPS: " <<  ( frame );
                fpsmessage = TTF_RenderText_Solid(font, fpsm.str().c_str(), textColor );
                
                frame = 0;
                update.start();
            }
            apply_surface( 10, 10, fpsmessage, screen);
            if( SDL_Flip( screen ) == -1 )
                return 1;
            
            frame++;
            if( ( ( fps.get_ticks() ) < 1000 / FRAMES_PER_SECOND ) )
                SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - ( fps.get_ticks() ) );
                
            
        
     }
     
     clean_up();
     return 0;
}
