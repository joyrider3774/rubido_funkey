#ifndef COMMONVARS_H
#define COMMONVARS_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_framerate.h>
#include <stdbool.h>
#include "cselector.h"
#include "cmainmenu.h"

typedef struct CBoardParts CBoardParts;
typedef struct CPeg CPeg;
typedef struct SPoint SPoint;
typedef struct CSelector CSelector;

// The diffrent difficultys
#define VeryEasy 0
#define Easy 1
#define Hard 2
#define VeryHard 3

// The diffrent gameStates possible in the game
#define GSQuit 0
#define GSGame 1 
#define GSTitleScreen 2
#define GSDifficultySelect 3 
#define GSCredits 4

#define GSInitDiff 50

#define GSGameInit (GSGame + GSInitDiff)
#define GSTitleScreenInit (GSTitleScreen + GSInitDiff)
#define GSDifficultySelectInit (GSDifficultySelect + GSInitDiff)
#define GSCreditsInit (GSCredits + GSInitDiff)

// window size
#define ORIG_WINDOW_WIDTH 240
#define ORIG_WINDOW_HEIGHT 240

//game defines
#define NrOfRows 9
#define NrOfCols 9
#define TileWidth 24
#define TileHeight 24
#define IDPeg 1
#define XOffSet 10
#define YOffSet 11
#define NrOfSounds 6
#define SND_SELECT 0
#define SND_GOOD 1
#define SND_WRONG 2
#define SND_START 3
#define SND_WINNER 4
#define SND_LOSER 5

#if defined(FUNKEY)
#define KEY_LEFT SDLK_l
#define KEY_RIGHT SDLK_r
#define KEY_UP SDLK_u
#define KEY_DOWN SDLK_d
#define KEY_A SDLK_a
#define KEY_B SDLK_b
#define KEY_X SDLK_x
#define KEY_Y SDLK_y
#define KEY_L1 SDLK_m
#define KEY_R1 SDLK_n
#define KEY_QUIT SDLK_q
#define KEY_SELECT SDLK_s
#elif defined(MIYOO)
#define KEY_UP SDLK_UP
#define KEY_DOWN SDLK_DOWN
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_A SDLK_SPACE
#define KEY_B SDLK_LCTRL
#define KEY_Y SDLK_LALT
#define KEY_X SDLK_LSHIFT
#define KEY_L1 SDLK_e
#define KEY_R1 SDLK_t
#define KEY_QUIT SDLK_ESCAPE
#define KEY_SELECT SDLK_RCTRL
#else
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_UP SDLK_UP
#define KEY_DOWN SDLK_DOWN
#define KEY_A SDLK_SPACE
#define KEY_B SDLK_b
#define KEY_X SDLK_x
#define KEY_Y SDLK_y
#define KEY_L1 SDLK_PAGEDOWN
#define KEY_R1 SDLK_PAGEUP
#define KEY_QUIT SDLK_q
#define KEY_SELECT SDLK_ESCAPE
#endif

extern SDL_Surface *IMGCredits,*IMGPeg,*IMGBackground,*IMGVeryEasy1,*IMGEasy1,*IMGHard1,*IMGVeryHard1,*IMGEasyInfo,*IMGVeryEasyInfo,*IMGHardInfo,*IMGVeryHardInfo;
extern int Difficulty;
extern Mix_Chunk *Sounds[NrOfSounds]; // array that will hold the sounds

extern CBoardParts *BoardParts; // boardparts instance that will hold all the boardparts
extern int GameState; // the game state
extern TTF_Font *font;
extern bool GlobalSoundEnabled; // used to see if the sound was opened succesfully
extern Mix_Music *Music; // the music
extern SDL_Surface *Screen; // the screen and buffer surface, buffer is used since hardware drawing will be used
extern SDL_Surface *Buffer;
extern int Moves,Volume;
extern int BestPegsLeft[4]; // array that holds the best amount of pegs left for each difficulty
extern FPSmanager FpsManager;

//game
extern CSelector *GameSelector;
extern bool PrintFormShown;

//titlescreen
extern CMainMenu* Menu;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern Uint32 FrameTicks;
extern Uint32 FrameCount;
extern Uint32 LastFps;
extern bool ShowFps;
extern bool noDelay;
extern bool fullscreen;
#endif