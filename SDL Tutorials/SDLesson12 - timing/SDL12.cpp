//headers
#include <string>
#include <sstream>
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

//button states
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

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

SDL_Rect clips[ 4 ];

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
     SDL_WM_SetCaption( "Timer Test", NULL );
     //init fine
     return true;
}

bool load_files()
{
     //Load bg image
     background = load_image( "background.png" );
     
     //load font
     font = TTF_OpenFont( "lazy.ttf", 24 );
     
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
Button::Button( int x, int y, int w, int h )
{
                //button attribs
                box.x = x;
                box.y = y;
                box.w = w;
                box.h = h;
                
                //set def sprite
                clip = &clips[ CLIP_MOUSEOUT ];
}
void Button::handle_events()
{
     //mouse offsets
     int x = 0, y = 0;
     
     //if mouse moved
     if( event.type == SDL_MOUSEMOTION )
     {
         //get mouse offsets
         x = event.motion.x;
         y = event.motion.y;
         
         //if mouse is over button
         if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
         {
             clip = &clips[ CLIP_MOUSEOVER ];
         }
         else
             clip = &clips[ CLIP_MOUSEOUT ];
     }
     if( event.type == SDL_MOUSEBUTTONDOWN )
     {
         //If left
         if( event.button.button == SDL_BUTTON_LEFT )
         {
             x = event.button.x;
             y = event.button.y;
             
             //if mouse is over button
             if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
                 clip = &clips[ CLIP_MOUSEDOWN ];
         }
     }
     if( event.type == SDL_MOUSEBUTTONUP )
     {
         //If left
         if( event.button.button == SDL_BUTTON_LEFT )
         {
             x = event.button.x;
             y = event.button.y;
             
             //if mouse is over button
             if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
                 clip = &clips[ CLIP_MOUSEUP ];
         }
     }
}
void Button::show()
{
     //show the button
     //apply_surface( box.x, box.y, buttonSheet, screen, clip );
}
void set_clips()
{
     //clip the sprite sheet
     clips[ CLIP_MOUSEOVER ].x = 0;
     clips[ CLIP_MOUSEOVER ].y = 0;
     clips[ CLIP_MOUSEOVER ].w = 320;
     clips[ CLIP_MOUSEOVER ].h = 240;
     
     clips[ CLIP_MOUSEOUT ].x = 320;
     clips[ CLIP_MOUSEOUT ].y = 0;
     clips[ CLIP_MOUSEOUT ].w = 320;
     clips[ CLIP_MOUSEOUT ].h = 240;
     
     clips[ CLIP_MOUSEDOWN ].x = 0;
     clips[ CLIP_MOUSEDOWN ].y = 240;
     clips[ CLIP_MOUSEDOWN ].w = 320;
     clips[ CLIP_MOUSEDOWN ].h = 240;
     
     clips[ CLIP_MOUSEUP ].x = 320;
     clips[ CLIP_MOUSEUP ].y = 240;
     clips[ CLIP_MOUSEUP ].w = 320;
     clips[ CLIP_MOUSEUP ].h = 240;

}
int main( int argc, char* args[] )
{
     bool quit = false;
     
     int frame = 0;
     
     bool cap = true;
     
     Timer fps;
     
     if( init() == false )
         return 1;
     
     if( load_files() == false )
         return 1;
         
     
     //generate messages
     message = TTF_RenderText_Solid( font, "Testing Frame Rate", textColor );
             
     while( quit == false)
     {
            fps.start();
            while( SDL_PollEvent(&event) )
            {
                                    
                   if( event.type == SDL_KEYDOWN )
                   {
                       if( event.key.keysym.sym == SDLK_RETURN )
                       {
                           cap = (!cap);
                       }
                   }
                   else if( event.type == SDL_QUIT )
                       quit = true;
            }
            apply_surface( 0, 0, background, screen );
            apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( ( SCREEN_HEIGHT + message->h * 2 ) / FRAMES_PER_SECOND ) * ( frame % FRAMES_PER_SECOND ) - message->h, message, screen );
                    
            if( SDL_Flip( screen ) == -1 )
                return 1;
            
            frame++;
            if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
                SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
         /*
         Uint8 *keystates = SDL_GetKeyState( NULL );
         //if up
         if( keystates[ SDLK_UP ] )
         {
             apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen );
         }
         
         //if down
         if( keystates[ SDLK_DOWN ] )
             apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen );
         
         //if left
         if( keystates[ SDLK_LEFT ] )
             apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen );
         //if right
         if( keystates[ SDLK_RIGHT ] )
             apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + (SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen );
             
         if( SDL_Flip( screen ) == -1 )
             return 1;
             */
     }
     
     clean_up();
     return 0;
}
