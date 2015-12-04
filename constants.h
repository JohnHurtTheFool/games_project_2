// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 constants.h v1.1

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

//=============================================================================
//                  Constants
//=============================================================================

// window
const char CLASS_NAME[] = "AI";
const char GAME_TITLE[] = "SPACESHIP CONTROL";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1270;               // width of game in pixels
const UINT GAME_HEIGHT = 800;               // height of game in pixels
const float BACKGROUND_SCALE = 0.8;

// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const RECT  COLLISION_RECTANGLE = {-30,-16,30,16};
const RECT  COLLISION_BOX_player = {-45,-10,45,10};
const RECT COLLISION_BOX_PUCK = {-32, -32, 32, 32};
const float   COLLISION_RADIUS = 29;
const int SPACESHIP_SIZE = 64;
const int LASER_HEIGHT = 4;
const int LASER_WIDTH = 2;
const int NUM_ENEMIES_INITIAL = 15;
const int MAX_PLAYER_LASERS = 600;
const int MAX_ENEMY_LASERS = 150;
const double laserDamage = 5.00;
const double kamikazeDamage = 15.00;
const int MAX_HIGH_SCORE = 5;
// graphic images
const char PUCK_IMAGE[] = "..\\games_project_2\\pictures\\homework.jpg";  // game textures
const char PADDLE_IMAGE[] =     "..\\games_project_2\\pictures\\student.jpg";      // menu texture
const char BACKGROUND_IMAGE[] =     "..\\games_project_2\\pictures\\Background.png";      // menu texture
const char SPLASH_SCREEN[] =     "..\\games_project_2\\pictures\\Splash Screen.png";      // menu texture
const char PLAYER_IMAGE[] =     "..\\games_project_2\\pictures\\player spritesheet.png";      // menu texture
const char ENEMY_IMAGE[] =     "..\\games_project_2\\pictures\\enemy spritesheet two.png";      // menu texture
const char BOSS_IMAGE[] =     "..\\games_project_2\\pictures\\bossspritesheet.png";      // menu texture
const char ENEMY_LASER[] =     "..\\games_project_2\\pictures\\red laser.png";      // menu texture
const char PLAYER_LASER[] =     "..\\games_project_2\\pictures\\blue laser.png";      // menu texture
const char SHIELD_IMAGE[] =     "..\\games_project_2\\pictures\\shield.png";      // menu texture
const char BONUS_IMAGE[] =     "..\\games_project_2\\pictures\\bonus.png";      // menu texture
const char EMP_POWERUP_IMAGE[] =     "..\\games_project_2\\pictures\\emp powerup.png";      // menu texture
const char GAME_OVER[] =     "..\\games_project_2\\pictures\\game over.png";      // menu texture
const char INSTR_IMAGE[] =     "..\\games_project_2\\pictures\\instructions.png";      // menu texture
const char EMP_IMAGE[] =     "..\\games_project_2\\pictures\\emp.png";      // menu texture
// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;       // escape key
const UCHAR ALT_KEY      = VK_MENU;         // Alt key
const UCHAR player_LEFT    = VK_LEFT;     // left arrow
const UCHAR player_RIGHT   = VK_RIGHT;    // right arrow
const UCHAR ENTER_KEY    = VK_RETURN; 
const UCHAR player_UP = VK_UP;
const UCHAR player_DOWN = VK_DOWN;
const UCHAR PLAYER_SHOOT = VK_SPACE;
const UCHAR TOGGLE_MUSIC = VK_TAB;
//

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
//const char WAVE_BANK[]  = "..\\games_project_2\\audio\\Win\\WavesExample1.xwb";
const char WAVE_BANK[]  = "..\\games_project_2\\audio\\Win\\Wave Bank.xwb";
// SOUND_BANK must be location of .xsb file.
//const char SOUND_BANK[] = "..\\games_project_2\\audio\\Win\\SoundsExample1.xsb";
const char SOUND_BANK[] = "..\\games_project_2\\audio\\Win\\Sound Bank.xsb";
// audio cues
const char BEEP1[] = "beep1";
const char BEEP2[] = "beep2";
const char BEEP3[] = "beep3";
const char BEEP4[] = "beep4";
const char HIT[]   = "hit";
const char BACKGROUND[] = "backgroundc";
const char CRASH[] = "crashc";
const char LASER[] = "laserc";
const char ASOUND[] = "a";
// Pattern Step Action
enum PATTERN_STEP_ACTION {NONE, UP, DOWN, LEFT, RIGHT, TRACK, EVADE};
enum GAME_STATES {SPLASH, GAME_PLAY,LOSE_SCREEN,NEW_LEVEL,MENU,OPTIONS,CHEAT,INSTR,HIGH_SCORES};
const char INVINCIBILITY_CHEAT[] = "FORCEFIELD";
#endif
