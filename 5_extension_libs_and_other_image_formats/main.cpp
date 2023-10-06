// ========================== Includes ==========================
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// ========================== Constants and Enums ==========================
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
SDL_Surface* gPNGSurface = NULL;

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
			// initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError()); 
				success = false;
			}
			else
			{
				// Get the window surface
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}
	
	return success;
}

bool loadMedia()
{
	// Function success
	bool success = true;

	// load the PNG
	gPNGSurface = loadSurface("media/loaded.png");
	if (gPNGSurface == NULL)
	{
		printf("Failed to load PNG!");
		success = false;
	}
	return success;
}

void close()
{
	// deallocate surface
	SDL_FreeSurface(gPNGSurface);
	gPNGSurface = NULL;
	
	// Destroy the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL; 

	// Quit SDL Subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface(std::string path)
{
	// The final optimized surface
	SDL_Surface* optimizedSurface = NULL;

	// Load image at the specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) 
	{
		printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		// convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Get rid of the old loaded surface because it's still in memory
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
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
		
			// scale the image to the size of the screen
			SDL_Rect stretchedRect;
			stretchedRect.x = 0;
			stretchedRect.y = 0;
			stretchedRect.w = SCREEN_WIDTH;
			stretchedRect.h = SCREEN_HEIGHT;

			// apply the image
			SDL_BlitScaled(gPNGSurface, NULL, gScreenSurface, &stretchedRect);

			// update the surface
			SDL_UpdateWindowSurface(gWindow);
		}
	}

	// close out resources and SDL
	close();

  	return 0;
}

