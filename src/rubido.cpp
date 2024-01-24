#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_framerate.h>
#include "cboardparts.h"
#include "cmainmenu.h"
#include "cpeg.h"
#include "cselector.h"
#include "commonvars.h"


// Load the settings, if there isn't a settings file, set some initial values
void LoadSettings()
{
	char Filename[4096];

	const char *File = ".com.github.joyrider377.formula1_funkey.dat";
	char *EnvHome = getenv("HOME");
	char *EnvHomeDrive = getenv("HOMEDRIVE");
	char *EnvHomePath = getenv("HOMEPATH");

	sprintf(Filename, "%s", File);
	if (EnvHome) //linux systems normally
		sprintf(Filename, "%s/%s", EnvHome, File);
	else
		if(EnvHomeDrive && EnvHomePath) //windows systems normally
			sprintf(Filename, "%s%s/%s", EnvHomeDrive, EnvHomePath, File);

 	FILE *SettingsFile;
 	SettingsFile = fopen(Filename,"r");
 	if(SettingsFile)
 	{
		fscanf(SettingsFile,"Best0=%d\n",&BestPegsLeft[VeryEasy]);
		fscanf(SettingsFile,"Best1=%d\n",&BestPegsLeft[Easy]);
		fscanf(SettingsFile,"Best2=%d\n",&BestPegsLeft[Hard]);
		fscanf(SettingsFile,"Best3=%d\n",&BestPegsLeft[VeryHard]);
		fclose(SettingsFile);
 	}
 	else
 	{
		BestPegsLeft[VeryEasy] = 0;
		BestPegsLeft[Easy] = 0;
		BestPegsLeft[Hard] = 0;
		BestPegsLeft[VeryHard] = 0;
	}
}

// Save the settings
void SaveSettings()
{
	char Filename[4096];

	const char *File = ".com.github.joyrider377.formula1_funkey.dat";
	char *EnvHome = getenv("HOME");
	char *EnvHomeDrive = getenv("HOMEDRIVE");
	char *EnvHomePath = getenv("HOMEPATH");

	sprintf(Filename, "%s", File);
	if (EnvHome) //linux systems normally
		sprintf(Filename, "%s/%s", EnvHome, File);
	else
		if(EnvHomeDrive && EnvHomePath) //windows systems normally
			sprintf(Filename, "%s%s/%s", EnvHomeDrive, EnvHomePath, File);

 	FILE *SettingsFile;
 	SettingsFile = fopen(Filename,"w");
 	if(SettingsFile)
 	{
		fprintf(SettingsFile,"Best0=%d\n",BestPegsLeft[VeryEasy]);
		fprintf(SettingsFile,"Best1=%d\n",BestPegsLeft[Easy]);
		fprintf(SettingsFile,"Best2=%d\n",BestPegsLeft[Hard]);
		fprintf(SettingsFile,"Best3=%d\n",BestPegsLeft[VeryHard]);
		fclose(SettingsFile);
 	}
}

// procedure i made to draw some text on the screen
void WriteText(SDL_Surface* Surface,TTF_Font* FontIn,char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn)
{
	char List[100][255];
	int Lines,Teller,Chars;
	SDL_Rect DstRect;
	SDL_Surface* TmpSurface1;
	Lines = 0;
	Chars = 0;
	for (Teller=0;Teller<NrOfChars;Teller++)
	{
		if(Lines > 100)
			break;
		if((Tekst[Teller] == '\n') || (Chars==255))
		{
			List[Lines][Chars]='\0';
			Lines++;
			Chars = 0;
		}
		else
		{
		 	List[Lines][Chars]=Tekst[Teller];
		 	Chars++;
		}
	}
	List[Lines][Chars] = '\0';
	for (Teller=0;Teller <= Lines;Teller++)
	{
		if(strlen(List[Teller]) > 0)
		{
			TmpSurface1 = TTF_RenderText_Blended(FontIn,List[Teller],ColorIn);
			DstRect.x = X;
			DstRect.y = Y + (Teller) * TTF_FontLineSkip(FontIn) + (Teller*YSpacing);
			DstRect.w = TmpSurface1->w;
			DstRect.h = TmpSurface1->h;
			SDL_BlitSurface(TmpSurface1,NULL,Surface,&DstRect);
			SDL_FreeSurface(TmpSurface1);
		}
	}
}

// procedure that calculates how many moves are possible in the current board state
// we can simply do this by checking all parts and see if they can move to all directions
// the canmoveto method in CPegs is does all the checking
int MovesLeft()
{
    CPeg *BoardPart ;
    int TmpMoves=0,Y,X;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            BoardPart = CBoardParts_GetPart(BoardParts, X,Y);
            // if there is a boardpart on that X,Y Coordinate
            // check all direction if we can move to that if so increases the movesleft
            if (BoardPart)
            {
                if (CPeg_CanMoveTo(BoardPart,X+2,Y,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X,Y-2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X+2,Y-2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X+2,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y-2,false))
                    TmpMoves++;
            }
        }
    return TmpMoves;
}

// Procedure that counts how many pegs are left on the board (animphase must be lower then 2 io a red or blue peg)
int PegsLeft()
{
    CPeg *BoardPart ;
    int Pegs=0,Y,X;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            BoardPart = CBoardParts_GetPart(BoardParts,X,Y);
            if (BoardPart)
                if ((CPeg_GetType(BoardPart) == IDPeg) && (CPeg_GetAnimPhase(BoardPart) < 2))
                    Pegs++;
        }
    return Pegs;
}

// procedure that draws the board, boardparts info and a boxed message over the playfield
// and waits till the A button is pressed

void PrintForm(char *msg)
{
	PrintFormShown = true;
    SDL_Color Color = {0,0,0,0};
	boxRGBA(Buffer,16,80,221,160,255,255,255,255);
	rectangleRGBA(Buffer,16,80,221,160,0,0,0,255);
	rectangleRGBA(Buffer,17,81,220,159,0,0,0,255);
	WriteText(Buffer,font,msg,strlen(msg),21,85,2,Color);
}

// Loads the sounds in the sound array
void LoadSounds()
{
	if (GlobalSoundEnabled)
	{
		Sounds[SND_START] = Mix_LoadWAV("./rubidofs/sound/start.wav");
		Sounds[SND_SELECT] = Mix_LoadWAV("./rubidofs/sound/select.wav");
		Sounds[SND_GOOD] = Mix_LoadWAV("./rubidofs/sound/good.wav");
		Sounds[SND_WRONG] = Mix_LoadWAV("./rubidofs/sound/wrong.wav");
		Sounds[SND_WINNER] = Mix_LoadWAV("./rubidofs/sound/winner.wav");
		Sounds[SND_LOSER] = Mix_LoadWAV("./rubidofs/sound/loser.wav");
		Music = Mix_LoadMUS("./rubidofs/music/rubido.ogg");
	}
}

// unloads the sounds
void UnloadSounds()
{
	Mix_HaltMusic();
	int Teller;
	for (Teller=0;Teller<NrOfSounds;Teller++)
		if(Sounds[Teller])
			Mix_FreeChunk(Sounds[Teller]);
    Mix_FreeMusic(Music);
}

void LoadGraphics()
{
    SDL_Surface *Tmp;
    Tmp = IMG_Load("./rubidofs/graphics/background.png");
    IMGBackground = SDL_DisplayFormatAlpha(Tmp); // optimize image for screen
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/peg.png");
    IMGPeg = SDL_DisplayFormatAlpha(Tmp); // optimize image for screen
    SDL_FreeSurface(Tmp);

	   // Load the images
    Tmp = IMG_Load("./rubidofs/graphics/infoveryeasy.png");
    IMGVeryEasyInfo = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/infoeasy.png");
    IMGEasyInfo = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/infohard.png");
    IMGHardInfo = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/infoveryhard.png");
    IMGVeryHardInfo = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/veryeasy1.png");
    IMGVeryEasy1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/easy1.png");
    IMGEasy1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/hard1.png");
    IMGHard1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./rubidofs/graphics/veryhard1.png");
    IMGVeryHard1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

	Tmp = IMG_Load("./rubidofs/graphics/credits.png");
    IMGCredits = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

}

void UnLoadGraphics()
{
    if(IMGBackground)
	{
        SDL_FreeSurface(IMGBackground);
	}
    
	if(IMGPeg)
	{
        SDL_FreeSurface(IMGPeg);
	}
	
	if(IMGVeryEasy1)
	{
    	SDL_FreeSurface(IMGVeryEasy1);
	}
	
	if(IMGEasy1)
	{
    	SDL_FreeSurface(IMGEasy1);
	}
	
	if(IMGHard1)
	{
    	SDL_FreeSurface(IMGHard1);
	}
	
	if(IMGVeryHard1)
	{
    	SDL_FreeSurface(IMGVeryHard1);
	}
	
	if(IMGVeryEasyInfo)
	{
    	SDL_FreeSurface(IMGVeryEasyInfo);
	}
	
	if(IMGEasyInfo)
	{
	    SDL_FreeSurface(IMGEasyInfo);
	}

	if(IMGHardInfo)
	{
    	SDL_FreeSurface(IMGHardInfo);
	}
	
	if(IMGVeryHardInfo)
	{
    	SDL_FreeSurface(IMGVeryHardInfo);
	}

	if(IMGCredits)
	{
    	SDL_FreeSurface(IMGCredits);
	}

}

// this will ceate the initial board state, io a cross of pegs, with the middle on being empty (=animphase 6)
void InitBoard()
{
	CBoardParts_RemoveAll(BoardParts);
    int X,Y;
    for (Y=0;Y < NrOfRows;Y++)
        for (X=0;X < NrOfCols;X++)
        {
            if ((Y < 3) && (X > 2) && (X < 6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
             if ((Y > 2) && (Y <6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
            if ((Y > 5) && (X > 2) && (X <6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
        }
     CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,4,4), 6);

}

// Checks if we won the game
bool IsWinningGame()
{
    if (PegsLeft() == 1) //must be 1 peg left
    {
        if ((Difficulty == VeryHard) || (Difficulty == Easy))
        {
            if(CPeg_GetAnimPhase(CBoardParts_GetPart(BoardParts,4,4)) < 2) // must be in the middle with veryhard or easy
                return true;
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;
}

void GameInit()
{
	GameSelector = CSelector_Create(4,4);
	InitBoard();
    Moves = 0;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_START],0);

}

// The main Game Loop
void Game()
{
	if( GameState == GSGameInit)
	{
		GameInit();
		GameState -= GSInitDiff;
	}
    SDL_Event Event;
    SDL_Color Color1 = {0,0,0,255};
    char Msg[300];
    
    SDL_BlitSurface(IMGBackground,NULL,Buffer,NULL);
	// Write some info to the Buffer
	sprintf(Msg,"Moves Left:%d",MovesLeft());
    WriteText(Buffer,font,Msg,strlen(Msg),161,163,0,Color1);
    sprintf(Msg,"Moves:%d",Moves);
    WriteText(Buffer,font,Msg,strlen(Msg),161,179,0,Color1);
    sprintf(Msg,"Pegs Left:%d",PegsLeft());
    WriteText(Buffer,font,Msg,strlen(Msg),161,195,0,Color1);
    if (BestPegsLeft[Difficulty] != 0)
    {
        sprintf(Msg,"Best Pegs:%d",BestPegsLeft[Difficulty]);
        WriteText(Buffer,font,Msg,strlen(Msg),161,211,0,Color1);
    }
	CBoardParts_Draw(BoardParts,Buffer);
	CSelector_Draw(GameSelector, Buffer);

	//need to repeat showing it until it's no longer shown
	if (PrintFormShown)
	{
		// if it's the winning game play the winning sound and show the form with the winning message
		if (IsWinningGame())
		{
			sprintf(Msg,"Congratulations you have solved\nthe puzzle! Try a new difficulty!\n\nPress (A) to continue");
			PrintForm(Msg);
		}
		else // show the loser messager, play loser sound
		{
			sprintf(Msg,"You couldn't solve the puzzle!\nDon't give up, try it again!\n\nPress (A) to continue");
			PrintForm(Msg); 
		}
	}


	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_QUIT)
			GameState = GSQuit;

		if (Event.type == SDL_KEYDOWN)
		{
			switch(Event.key.keysym.sym)
			{
				case SDLK_q:
					GameState = GSQuit;
					break;
				case SDLK_l:
				case SDLK_LEFT: // move the selector to the left io currentposition.x - 1
					if (!PrintFormShown)
						CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X -1,CSelector_GetPosition(GameSelector).Y);
					break;
				case SDLK_r:
				case SDLK_RIGHT:
					if (!PrintFormShown)
						CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X +1,CSelector_GetPosition(GameSelector).Y);
					break;
				case SDLK_u:
				case SDLK_UP:
					if (!PrintFormShown)
						CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X,CSelector_GetPosition(GameSelector).Y-1);
					break;
				case SDLK_d:
				case SDLK_DOWN:
					if (!PrintFormShown)
						CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X,CSelector_GetPosition(GameSelector).Y+1);
					break;
				case SDLK_b:
				case SDLK_c: // deselect selection
					if (!PrintFormShown && GameSelector->HasSelection)
					{
						CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,CSelector_GetSelection(GameSelector).X,CSelector_GetSelection(GameSelector).Y),0);
						CSelector_DeSelect(GameSelector);
					}
					break;
				case SDLK_s:
				case SDLK_ESCAPE: // select = quit to title
					Mix_HaltChannel(-1);
					GameState = GSTitleScreenInit;
					PrintFormShown = false;
					break;
				case SDLK_a:
				case SDLK_x:
					if(PrintFormShown)
					{
						GameState=GSTitleScreenInit;
						// stop the music
						Mix_HaltChannel(-1);
						PrintFormShown = false;
					}
					else
					{
						if (GameSelector->HasSelection) // if we have a selection
						{
							// see if the selected boardpart can move to the current position
							if (CPeg_CanMoveTo(CBoardParts_GetPart(BoardParts,CSelector_GetSelection(GameSelector).X,CSelector_GetSelection(GameSelector).Y), CSelector_GetPosition(GameSelector).X,CSelector_GetPosition(GameSelector).Y,true))
							{
								if (GlobalSoundEnabled)
									Mix_PlayChannel(-1,Sounds[SND_GOOD],0);
								//if so play a sound, increase the moves, set the selected part to empty and the current part to red
								Moves++;
								CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,CSelector_GetSelection(GameSelector).X,CSelector_GetSelection(GameSelector).Y), 6);
								CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,CSelector_GetPosition(GameSelector).X,CSelector_GetPosition(GameSelector).Y), 0);
								// if no moves are left see if the best pegs left value for the current difficulty is
								// greater if so set te new value
								if (MovesLeft() == 0)
								{
									if (BestPegsLeft[Difficulty] != 0)
									{
										if (PegsLeft() < BestPegsLeft[Difficulty])
											BestPegsLeft[Difficulty] = PegsLeft();
									}
									else
										BestPegsLeft[Difficulty] = PegsLeft();
									// if it's the winning game play the winning sound and show the form with the winning message
									if (IsWinningGame())
									{
										SaveSettings();
										if (GlobalSoundEnabled)
											Mix_PlayChannel(-1,Sounds[SND_WINNER],0);
										PrintFormShown = true;								
									}
									else // show the loser messager, play loser sound
									{
										SaveSettings();
										if (GlobalSoundEnabled)
											Mix_PlayChannel(-1,Sounds[SND_LOSER],0);
										PrintFormShown = true;								
									}								
								}

							}
							else // if we can't move to the spot, play the wrong move sound, and reset the selection to a red peg (instead of blue / selected)
							{
								CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,CSelector_GetSelection(GameSelector).X,CSelector_GetSelection(GameSelector).Y),0);
								if (GlobalSoundEnabled)
									Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
							}
							CSelector_DeSelect(GameSelector); // deselect the selection
						}
						else // we didn't have a selection, set the new selection
						{
							if (CPeg_GetAnimPhase(CBoardParts_GetPart(BoardParts,CSelector_GetPosition(GameSelector).X,CSelector_GetPosition(GameSelector).Y)) == 0)
							{
								if (GlobalSoundEnabled)
									Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
								CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,CSelector_GetPosition(GameSelector).X,CSelector_GetPosition(GameSelector).Y),1);
								CSelector_Select(GameSelector);
							}

						}
					}
					break;
				default:
					break;
			}
		}
    }
}

void TitleScreenInit()
{
	Menu = CMainMenu_Create();
	
	// if a music hasn't started start the music in an endless loop (-1 as last parameter)
    if (GlobalSoundEnabled)
    {
        if (!Mix_PlayingMusic())
            Mix_PlayMusic(Music,-1);
    }
}

// main title screen loop
void TitleScreen()
{
	if(GameState == GSTitleScreenInit)
	{
		TitleScreenInit();
		GameState -= GSInitDiff;
	}
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_QUIT)
			GameState = GSQuit;

		if (Event.type == SDL_KEYDOWN)
			switch (Event.key.keysym.sym)
			{
				case SDLK_q:
					GameState = GSQuit;
					break;
				case SDLK_d:
				case SDLK_DOWN:
					CMainMenu_NextItem(Menu);
					break;
				case SDLK_u:
				case SDLK_UP:
					CMainMenu_PreviousItem(Menu);
					break;
				case SDLK_a:
				case SDLK_x:
					// set the gamestate according to the menu selection
					Mix_PlayChannel(-1,Sounds[SND_GOOD],0);
					switch(CMainMenu_GetSelection(Menu))
					{
						case 1:
							GameState = GSDifficultySelectInit;
							break;
						case 2:
							GameState = GSCreditsInit;
							break;
						case 3:
							GameState = GSQuit;
					}
					break;
				case SDLK_ESCAPE:
					GameState = GSQuit;
					break;
				default:
					break;
			}
	}
	CMainMenu_Draw(Menu, Buffer);
}

void DifficultySelectInit()
{

}

// Main difficulty select loop
void DifficultySelect()
{
	if(GameState == GSDifficultySelectInit)
	{
		DifficultySelectInit();
		GameState -= GSInitDiff;
	}

    SDL_Event Event;
    SDL_Rect Dest;

	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_QUIT)
			GameState = GSQuit;

		if (Event.type == SDL_KEYDOWN)
			switch (Event.key.keysym.sym)
			{
				case SDLK_q:
					GameState = GSQuit;
					break;
				case SDLK_s:
				case SDLK_ESCAPE:
					GameState = GSTitleScreenInit;
					break;
				case SDLK_a:
				case SDLK_x:
					GameState = GSGameInit;
					break;
				case SDLK_l:
				case SDLK_LEFT: // Change difficluly one lower if we pressed left
					if(Difficulty == VeryHard)
					{
						Difficulty = Hard;
					}
					else
						if(Difficulty == Hard)
						{
							Difficulty = Easy;
						}
						else
							if(Difficulty == Easy)
							{
								Difficulty = VeryEasy;
							}
							else
								if(Difficulty == VeryEasy)
									Difficulty = VeryHard;
					break;
				case SDLK_r:
				case SDLK_RIGHT: // change difficulty one higher
					if(Difficulty == VeryEasy)
					{
						Difficulty = Easy;
					}
					else
						if(Difficulty == Easy)
						{
							Difficulty = Hard;
						}
						else
							if(Difficulty == Hard)
							{
								Difficulty = VeryHard;
							}
							else
								if(Difficulty == VeryHard)
									Difficulty = VeryEasy;
					break;
				default:
					break;
			}
	}
	// decide what we draw to the buffer based on the difficuly
	switch(Difficulty)
	{
		case VeryEasy:
			SDL_BlitSurface(IMGVeryEasyInfo,NULL,Buffer,NULL);
			Dest.x = Buffer->w/2 - IMGVeryEasy1->w/2;
			Dest.y = 35;
			Dest.w = IMGVeryEasy1->w;
			SDL_BlitSurface(IMGVeryEasy1,NULL,Buffer,&Dest);
			break;
		case Easy:
			SDL_BlitSurface(IMGEasyInfo,NULL,Buffer,NULL);
			Dest.x = Buffer->w/2 - IMGEasy1->w/2;
			Dest.y = 35;
			Dest.w = IMGEasy1->w;
			SDL_BlitSurface(IMGEasy1,NULL,Buffer,&Dest);
			break;
		case Hard:
			SDL_BlitSurface(IMGHardInfo,NULL,Buffer,NULL);
			Dest.x = Buffer->w/2 - IMGHard1->w/2;
			Dest.y = 35;
			Dest.w = IMGHard1->w;
			SDL_BlitSurface(IMGHard1,NULL,Buffer,&Dest);
			break;
		case VeryHard:
			SDL_BlitSurface(IMGVeryHardInfo,NULL,Buffer,NULL);
			Dest.x = Buffer->w/2 - IMGVeryHard1->w/2;
			Dest.y = 35;
			Dest.w = IMGVeryHard1->w;
			SDL_BlitSurface(IMGVeryHard1,NULL,Buffer,&Dest);
			break;

	}
}

//Main Credits loop, will just show an image and wait for a button to be pressed
void Credits()
{
    SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_QUIT)
			GameState = GSQuit;

		if (Event.type == SDL_KEYDOWN)
			switch (Event.key.keysym.sym)
			{
				case SDLK_q:
					GameState = GSQuit;
					break;
				case SDLK_a:
				case SDLK_b:
				case SDLK_c:
				case SDLK_x:
					GameState = GSTitleScreenInit;
					break;
				default:
					break;
			}
	}
	SDL_BlitSurface(IMGCredits,NULL,Buffer,NULL);
}

// Start of the program, should be obvious what happens here
int main(int argv, char** args)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0)
	{
		printf("SDL Succesfully initialized\n");
		Uint32 flags = SDL_HWSURFACE;
		#ifdef FUNKEY
			flags |= SDL_FULLSCREEN;
		#endif
        Screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 32, flags);
		if(Screen)
		{
		    printf("Succesfully Set %dx%dx32\n",WINDOW_WIDTH, WINDOW_HEIGHT);
			Buffer = SDL_CreateRGBSurface(SDL_HWSURFACE, WINDOW_WIDTH, WINDOW_HEIGHT, 32, Screen->format->Rmask, Screen->format->Gmask, Screen->format->Bmask, Screen->format->Amask);
			if(Buffer)
			{
				printf("Succesfully Created %dx%dx32 Buffer\n", WINDOW_WIDTH, WINDOW_HEIGHT);
				SDL_ShowCursor(SDL_DISABLE);
				{
					if (Mix_OpenAudio(44100,AUDIO_S16,MIX_DEFAULT_CHANNELS,1024) < 0)
					{
						GlobalSoundEnabled = false;
						printf("Failed to initialise sound!\n");
						printf("%s\n",Mix_GetError());
					}
					else
					{
						printf("Audio Succesfully initialised!\n");
					}
					if (TTF_Init() == 0)
					{
						printf("Succesfully initialized TTF\n");
						font = TTF_OpenFont("./rubidofs/font/font.ttf",15);
						if (font)
						{
							printf("Succesfully Loaded fonts\n");
							TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
							BoardParts = CBoardParts_Create();
							Menu = CMainMenu_Create();
							LoadSounds();
							LoadGraphics();
							LoadSettings();
							SDL_initFramerate(&FpsManager);
							SDL_setFramerate(&FpsManager, 30);
							// Main game loop that loops untile the gamestate = GSQuit
							// and calls the procedure according to the gamestate.
							while (GameState != GSQuit)
							{
								switch(GameState)
								{
									case GSGameInit:
									case GSGame :
										Game();
										break;
									case GSTitleScreenInit:
									case GSTitleScreen:
										TitleScreen();
										break;
									case GSDifficultySelectInit:
									case GSDifficultySelect:
										DifficultySelect();
										break;
									case GSCreditsInit:
									case GSCredits:
										Credits();
										break;
									default:
										break;
								}
								SDL_BlitSurface(Buffer, NULL, Screen, NULL);								
								SDL_Flip(Screen);
								SDL_framerateDelay(&FpsManager);
							}
							CMainMenu_Destroy(Menu);
							CBoardParts_Destroy(BoardParts);
							SaveSettings();
							UnLoadGraphics();
							UnloadSounds();
							Mix_CloseAudio();
							TTF_CloseFont(font);
							font=NULL;
													
						}
						else
						{
							printf("Failed to Load fonts\n");
						}
						TTF_Quit();
					}
					else
					{
						printf("Failed to initialize TTF\n");
					}
				}
				SDL_FreeSurface(Buffer);
			}
            SDL_FreeSurface(Screen);
            Screen=NULL;
		}
		else
		{
			printf("Failed to Set Videomode %dx%dx32\n",WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		SDL_Quit();
	}
	else
	{
		printf("Couldn't initialise SDL!\n");
	}
	return 0;

}
