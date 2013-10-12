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
const int FRAMES_PER_SECOND = 60;

//dot
const int SQUARE_HEIGHT = 20;
const int SQUARE_WIDTH = 20;

const int DOT_HEIGHT = 20;
const int DOT_WIDTH = 20;

//button states
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

//surfaces
//SDL_Surface *square = NULL;
SDL_Surface *dot = NULL;
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

SDL_Rect clips[ 4 ];

//circle struct
struct Circle
{
       int x, y, r;
};

//square
/*class Square
{
      private:
      //collision box
      SDL_Rect box;
      
      //velocity
      int xVel, yVel;
      
      public:
      //init
      Square();
      
      //key presses
      void handle_input();
      
      //moves
      void move();
      
      //show
      void show();
};
*/
//dot
class Dot
{
    private:
    //The area of the dot
    Circle c;
    
    //The velocity of the dot
    int xVel, yVel;
    
    public:
    //Initializes the variables
    Dot();
    
    //Takes key presses and adjusts the dot's velocity
    void handle_input();
    
    //Moves the dot
    void move( std::vector<SDL_Rect> &rects, Circle &circle );
    
    //Shows the dot on the screen
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
     SDL_WM_SetCaption( "Collision", NULL );
     //init fine
     return true;
}

bool load_files()
{
     //Load bg image
     dot = load_image( "dot.bmp" );
     
     //load font
     font = TTF_OpenFont( "lazy.ttf", 24 );
     
     //if problem
     if( dot == NULL )
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
     
     SDL_FreeSurface( dot );
     SDL_FreeSurface( fpsmessage );
     
     TTF_CloseFont( font );
     
     TTF_Quit();
     
     SDL_Quit();
}

/*Square::Square()
{
                box.x = 0;
                box.y = 0;
                
                box.w = SQUARE_WIDTH;
                box.h = SQUARE_HEIGHT;
                
                xVel = 0;
                yVel = 0;
}
void Square::move()
{
     //move left or right
     box.x += xVel;
     if( ( box.x < 0 ) || ( box.x + SQUARE_WIDTH > SCREEN_WIDTH ) || ( check_collision( box, wall ) ) )
         box.x -= xVel;
     
     box.y += yVel;
     
     if( ( box.y < 0 ) || ( box.y + SQUARE_HEIGHT > SCREEN_HEIGHT ) || (check_collision( box, wall ) ) )
         box.y -= yVel;
}
void Square::show()
{
     //apply_surface( box.x, box.y, square, screen );
}
void Square::handle_input()
{
     if( event.type == SDL_KEYDOWN )
     {
         //adj
         switch( event.key.keysym.sym )
         {
                 case SDLK_UP: yVel -= SQUARE_HEIGHT / 6; break;
                 case SDLK_DOWN: yVel += SQUARE_HEIGHT / 6; break;
                 case SDLK_LEFT: xVel -= SQUARE_HEIGHT / 6; break;
                 case SDLK_RIGHT: xVel += SQUARE_HEIGHT / 6; break;
         }
     }
     else if( event.type == SDL_KEYUP )
     {
          //adj vel
          switch( event.key.keysym.sym )
         {
                 case SDLK_UP: yVel += SQUARE_HEIGHT / 6; break;
                 case SDLK_DOWN: yVel -= SQUARE_HEIGHT / 6; break;
                 case SDLK_LEFT: xVel += SQUARE_WIDTH / 6; break;
                 case SDLK_RIGHT: xVel -= SQUARE_WIDTH / 6; break;
         }
     }
}*/

Dot::Dot()
{
    //Initialize the offsets and dimentions
    c.x = DOT_WIDTH / 2;
    c.y = DOT_WIDTH / 2;
    c.r = DOT_WIDTH / 2;
    
    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}
/*void Dot::shift_boxes()
{
     int r = 0;
     
     //dots collision boxes
     for( int set = 0; set < box.size(); set++ )
     {
          box[ set ].x = x + ( DOT_WIDTH - box[ set ].w ) / 2;
          
          box[ set ].y = y + r;
          
          r += box[ set ].h;
     }
}*/
void Dot::handle_input()
{
     if( event.type == SDL_KEYDOWN )
     {
         //adj
         switch( event.key.keysym.sym )
         {
                 case SDLK_UP: yVel -= 1; break;
                 case SDLK_DOWN: yVel += 1; break;
                 case SDLK_LEFT: xVel -= 1; break;
                 case SDLK_RIGHT: xVel += 1; break;
         }
     }
     else if( event.type == SDL_KEYUP )
     {
          //adj vel
          switch( event.key.keysym.sym )
         {
                 case SDLK_UP: yVel += 1; break;
                 case SDLK_DOWN: yVel -= 1; break;
                 case SDLK_LEFT: xVel += 1; break;
                 case SDLK_RIGHT: xVel -= 1; break;
         }
     }
}
void Dot::move( std::vector<SDL_Rect> &rects, Circle &circle )
{
     //Move the dot left or right
    c.x += xVel;

    //If the dot went too far to the left or right or has collided with the other shapes
    if( ( c.x - DOT_WIDTH / 2 < 0 ) || ( c.x + DOT_WIDTH / 2 > SCREEN_WIDTH ) || ( check_collision( c, rects ) ) || ( check_collision( c, circle ) ) )
    {
        //Move back
        c.x -= xVel;
    }

    //Move the dot up or down
    c.y += yVel;

    //If the dot went too far up or down or has collided with the other shapes
    if( ( c.y - DOT_WIDTH / 2 < 0 ) || ( c.y + DOT_WIDTH / 2 > SCREEN_HEIGHT ) || ( check_collision( c, rects ) ) || ( check_collision( c, circle ) ) )
    {
        //Move back
        c.y -= yVel;
    }
}
void Dot::show()
{
     apply_surface( c.x - c.r, c.y - c.r, dot, screen );
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
     Timer update;
     
     
     
     if( init() == false )
         return 1;
     
     if( load_files() == false )
         return 1;
     Dot myDot;
     std::vector<SDL_Rect> box( 1 );
     Circle otherDot;
     //set the wall
     box[0].x = 60;
     box[0].y = 60;
     box[0].w = 40;
     box[0].h = 40;
     
     otherDot.x = 30;
     otherDot.y = 30;
     otherDot.r = DOT_WIDTH / 2;    
     
     //generate messages
     //message = TTF_RenderText_Solid( font, "Movement", textColor );
     //update.start();
     
     update.start();
     
     while( quit == false)
     {
            fps.start();
            while( SDL_PollEvent(&event) )
            {
                   myDot.handle_input();
                                  
                   if( event.type == SDL_QUIT )
                       quit = true;
            }
            myDot.move( box, otherDot );
            //std::stringstream fpsm;
            //fpsm << "FPS: " << frame / ( fps.get_ticks() / 1000.f );
            
            SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
            
            SDL_FillRect( screen, &box[ 0 ], SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
            
            //other dot
            apply_surface( otherDot.x - otherDot.r, otherDot.y - otherDot.r, dot, screen );
            //apply_surface(10, 10, fpsmessage, screen );
            //SDL_WM_SetCaption( fpsm.str().c_str(), NULL );
            //apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( ( SCREEN_HEIGHT + message->h * 2 ) / FRAMES_PER_SECOND ) * ( frame % FRAMES_PER_SECOND ) - message->h, message, screen );
            
            myDot.show();
            
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
