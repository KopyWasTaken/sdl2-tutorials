// ========================== Includes ==========================
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// ========================== Constants and Enums ==========================
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// ========================== Texture Wrapper Class ==========================
class LTexture 
{
    public:
        // initializes variables
        LTexture();

        // Deallocates memory
        ~LTexture();

        // Loads image at specified path
        bool loadFromFile(std::string path);

        // deallocates the texture
        void free();

        // renders texture at a given point
        void render(int x, int y, SDL_Rect* clip = NULL);

        // gets the image dimensions
        int getWidth();
        int getHeight();

    private:
        // the actual texture hardware
        SDL_Texture* mTexture;

        // image dimensions
        int mWidth;
        int mHeight;
};

// ========================== Global Variables ==========================
// The window we are going to render to
SDL_Window* gWindow = NULL;

// The window renderer 
SDL_Renderer* gRenderer = NULL;

// Scene sprites
SDL_Rect gSpriteClips[4];
LTexture gSpriteSheetTexture;

// ========================== Texture Wrapper Class Function Definitions ==========================
LTexture::LTexture()
{
	// initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	// deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	// get rid of the preexisting texture in case something is already loaded
	free();

	// create the texture
	SDL_Texture* newTexture = NULL;

	// first create a surface 
	SDL_Surface* newSurface= IMG_Load(path.c_str());
	if (newSurface == NULL)
	{
		printf("Could not load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError()); 
	}
	else
	{
		// color key the image we load
		SDL_SetColorKey(newSurface, SDL_TRUE, SDL_MapRGB(newSurface->format, 0, 0xFF, 0xFF));

		// create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, newSurface);
		if (newTexture == NULL) 
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}	
		else
		{
			// get the image dimensions
			mWidth = newSurface->w;
			mHeight = newSurface->h;
		}

		// get rid of the surface that we loaded
		SDL_FreeSurface(newSurface);
	}

	// return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	// free the texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	// set rendering space
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	// Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// render to screen
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

// ========================== Function Delcarations ==========================
// loads up SDL and creates window
bool init();

// loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image
SDL_Texture* loadTexture(std::string path);


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
			// initialize the renderer for the window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be initialized! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// inialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}

			// initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError()); 
				success = false;
			}
		}
	}
	
	return success;
}

bool loadMedia()
{
	// Function success
	bool success = true;

	// Load sprite sheet texture 
	if (!gSpriteSheetTexture.loadFromFile("media/dots.png"))
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else
	{
		// Set top left sprite
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 100;
		gSpriteClips[0].h = 100;

		// Set top right sprite
		gSpriteClips[1].x = 100;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 100;
		gSpriteClips[1].h = 100;

		// Set bottom left sprite
		gSpriteClips[2].x = 0;
		gSpriteClips[2].y = 100;
		gSpriteClips[2].w = 100;
		gSpriteClips[2].h = 100;

		// Set bottom right sprite
		gSpriteClips[3].x = 100;
		gSpriteClips[3].y = 100;
		gSpriteClips[3].w = 100;
		gSpriteClips[3].h = 100;
	}

	return success;
}

void close()
{
	// Free loaded images
	gSpriteSheetTexture.free();

	// Destroy the window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL; 
	gRenderer = NULL;

	// Quit SDL Subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	// the final texture
	SDL_Texture* newTexture = IMG_LoadTexture(gRenderer, path.c_str());
	if (newTexture == NULL)
	{
		printf("Could not load texture %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError()); 
	}

	return newTexture;
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
		
			// Clear the screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			// render top left sprite
			gSpriteSheetTexture.render(0, 0, &gSpriteClips[0]);

			// render top right sprite
			gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);

			// render bottom left sprite
			gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);

			// render bottom right sprite
			gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);

			// Update screen
			SDL_RenderPresent(gRenderer);
		}
	}

	// close out resources and SDL
	close();

  	return 0;
}