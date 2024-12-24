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

		// Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

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
LTexture gModulatedTexture;

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

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	// modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
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
	if (!gModulatedTexture.loadFromFile("media/colors.png"))
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	// Free loaded images
	gModulatedTexture.free();

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

		// Modulation components
		Uint8 r = 255;
		Uint8 g = 255;
		Uint8 b = 255;
		
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
				else if (e.type == SDL_KEYDOWN)
				{
					// modulate the colors with key presses
					switch (e.key.keysym.sym)
					{
						// increase red
						case SDLK_q:
						r += 32;
						break;

						// increase green
						case SDLK_w:
						g += 32;
						break;

						// increase blue
						case SDLK_e:
						b += 32;
						break;

						// decrease red
						case SDLK_a:
						r -= 32;
						break;

						// decrease green
						case SDLK_s:
						g -= 32;
						break;

						// decrease blue
						case SDLK_d:
						b -= 32;
						break;
					}
				}

			}
		
			// Clear the screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			// modulate and render texture
			gModulatedTexture.setColor(r, g, b);
			gModulatedTexture.render(0, 0);

			// Update screen
			SDL_RenderPresent(gRenderer);
		}
	}

	// close out resources and SDL
	close();

  	return 0;
}