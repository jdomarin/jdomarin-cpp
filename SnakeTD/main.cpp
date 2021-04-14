#include "Sprite.h"
#include "SDL_gfxPrimitives.h"
#include "Game.h"

SDL_PixelFormat *fmt;

int main ( int argc, char** argv )
{
	SDL_Rect pos;
	char message[50];
	int startTime, curTime;
	int nCycles = 0;
	int wait;
	int lastDraw;

	int timeRender = 0, timeUpdate = 0, tUpdate, tRender;

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    //SDL_Surface* screen = SDL_SetVideoMode(0, 0, 0, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_ANYFORMAT);
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE|SDL_DOUBLEBUF | SDL_ANYFORMAT);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	Mix_AllocateChannels(32);

    fmt = screen->format;
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    loadBlockTypes();
    loadTowerTypes();
    loadSpecialAttackTypes();

    int n = 1;
    if(argc > 1)
    {
        n = atoi(argv[1]);
	if(n < 1 || n >= 4) n = 1;
    }	
    Game game(n, screen);

    // program main loop
    bool done = false;
    startTime = SDL_GetTicks();
    tRender = startTime;
    while (!done)
    {
    	nCycles++;
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
			game.manageEvent(event);
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            } // end switch
        } // end of message processing

		game.update();

		curTime = SDL_GetTicks();
		tUpdate = curTime; timeUpdate += tUpdate-tRender;
		if(curTime < startTime+nCycles*20)
		{
			sprintf(message, "FPS : %.2f", 1000/((float)(curTime-lastDraw)));
			lastDraw = curTime;
        	// DRAWING STARTS HERE

	        // clear screen
	        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
			game.draw(screen);
			stringRGBA(screen, 0, 0, message, 0xff, 0xff, 0xff, 0xff);
			tRender = SDL_GetTicks();
			timeRender += tRender-tUpdate;
			SDL_Flip(screen);
			wait = startTime+nCycles*20-SDL_GetTicks();
			if(wait > 0) SDL_Delay(wait);
		}
		tRender = SDL_GetTicks();

    } // end main loop

    // free loaded bitmap

    // all is well ;)

    Mix_CloseAudio();
    printf("Temps à mettre à jour : %f\nTemps à afficher : %f\n", (float)timeUpdate/nCycles, (float)timeRender/nCycles);
    printf("Exited cleanly\n");
    return 0;
}
