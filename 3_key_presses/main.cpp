// ========================== Includes ==========================
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

// ========================== Constants and Enums ==========================
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Key Press Surface constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};


// ========================== Function Delcarations ==========================
// loads up SDL and creates window
bool init();

// loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image
SDL_Surface* loadSurface(std::string path);

// ========================== Global Variables ==========================
// The window we are going to render to
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The current surface being displayed 
SDL_Surface* gCurrentSurface = NULL;

// The images that correspond to a key press
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

// ========================== Function Definitions ==========================
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

	// Load default image 
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("media/press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Unable to load default image!\n");
		success = false;
	}
		
	// Load up image 
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("media/up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Unable to load up image!\n");
		success = false;
	}

	// Load down image 
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("media/down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Unable to load down image!\n");
		success = false;
	}

	// Load left image 
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("media/left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Unable to load left image!\n");
		success = false;
	}

	// Load right image 
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("media/right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Unable to load right image!\n");
		success = false;
	}

	return success;
}

void close()
{
	// deallocate all surfaces
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i)
	{
		SDL_FreeSurface(gKeyPressSurfaces[i]); 
		gKeyPressSurfaces[i] = NULL;
	}

	// Destroy the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL; 

	// Quit SDL Subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface(std::string path)
{
	// Load image at the specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL) 
	{
		printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
	}

	return loadedSurface;
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
		
		// Set the default image as the current surface
		gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

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

				// If the user presses a key
				else if (e.type == SDL_KEYDOWN)
				{
					// select the surface based on the key press
					switch (e.key.keysym.sym) 
					{
						case SDLK_UP:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
						break;

						case SDLK_DOWN:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
						break;

						case SDLK_LEFT:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
						break;

						case SDLK_RIGHT:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
						break;
						
						default:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
						break;

					}
				}
			}
		
			// apply the image
			SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL);

			// update the surface
			SDL_UpdateWindowSurface(gWindow);
		}
	}

	// close out resources and SDL
	close();

  	return 0;
}

