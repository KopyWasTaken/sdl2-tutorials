// Use SDL and Standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

// screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Declare some functions to be filled in later
// loads up SDL and creates window
bool init();

// loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();


// Delcare some global variables here
// The window we are going to render to
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The surface that contains the image we are going to render
SDL_Surface* gXOut = NULL;


// Start function implementations
bool init()
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Create the window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{ 
		  printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		  success = false;
		}
		else
		{
		  // Get the window surface
		  gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	
	return success;
}

bool loadMedia()
{
	// Function success
	bool success = true;

	// Load splash image
	gXOut = SDL_LoadBMP("media/XOut.bmp");
	if (gXOut == NULL)
	{
		printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
		
	return success;
}

void close()
{
	// deallocate the surface
	SDL_FreeSurface(gXOut);
	gXOut = NULL;

	// Destroy the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL; 

	// Quit SDL Subsystems
	SDL_Quit();
}

int main( int argc, char* args[])
{ 
 	// start up SDL and create the window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else 
	{
		// load media 
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		
		// main loop
		bool quit = false;

		// Event handler 
		SDL_Event e;
		
		// The main loop of the game
		while (!quit) 
		{
			// handle events on the queue
			while (SDL_PollEvent(&e) != 0) 
			{
				// the user requests to quit
				if (e.type == SDL_QUIT) 
				{
					quit = true;
				}
			}
		
			// apply the image
			SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL);

			// update the surface
			SDL_UpdateWindowSurface(gWindow);
		}
	}

	// close out resources and SDL
	close();

  	return 0;
}

