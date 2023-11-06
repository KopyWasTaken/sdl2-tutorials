// ========================== Includes ==========================
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

// ========================== Constants and Enums ==========================
// screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

// Button constants
const int BUTTON_WIDTH = 300; 
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION,
	BUTTON_SPRITE_MOUSE_DOWN,
	BUTTON_SPRITE_MOUSE_UP,
	BUTTON_SPRITE_TOTAL
};

// ========================== Button Wrapper Class ==========================
class LButton
{
	public:
		// initializes internal variables
		LButton();

		// sets top left position
		void setPosition(int x, int y);

		// handles mouse event
		void handleEvent(SDL_Event* e);

		// shows button sprite
		void render();
	
	private:
		// top left position
		SDL_Point mPosition;

		// currently used global sprite
		LButtonSprite mCurrentSprite; 	
};

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

		#if defined(SDL_TTF_MAJOR_VERSION)
		// Loads image from font string
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
		#endif

		// Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		// Set blending
		void setBlendMode(SDL_BlendMode blending);

		// Set alpha modulation
		void setAlpha(Uint8 alpha);

        // deallocates the texture
        void free();

        // renders texture at a given point
        void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

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

// ========================== Timer Class (with implementation) ==========================
class LTimer
{
	private:
		// The clock time when the timer started
		Uint32 mStartTicks;

		// The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		// The timer status
		bool mPaused;
		bool mStarted;

	public: 
		// inits variables
		LTimer()
		{
			// initialize the varaibles
			mStartTicks = 0;
			mPausedTicks = 0;

			mPaused = false;
			mStarted = false;
		}

		void start()
		{
			// start the timer
			mStarted = true;

			// unpause the timer
			mPaused = false;

			// get the current clock time
			mStartTicks = SDL_GetTicks();
			mPausedTicks = 0;
		}

		void stop()
		{
			// stop the timer
			mStarted = false;

			// Unpause the timer
			mPaused = false;

			// clear the tick vars
			mStartTicks = 0;
			mPausedTicks = 0;
		}

		void pause()
		{
			// if the timer is running and isn't already paused
			if (mStarted && !mPaused)
			{
				// pause the timer
				mPaused = true;

				// calculate the paused ticks
				mPausedTicks = SDL_GetTicks() - mStartTicks;
				mStartTicks = 0;
			}
		}

		void unpause()
		{
			// if the timer is paused and started
			if (mStarted && mPaused)
			{
				// unpause the timer
				mPaused = false;

				// reset the starting ticks
				mStartTicks = SDL_GetTicks() - mPausedTicks;
				
				// reset the paused ticks
				mPausedTicks = 0;
			}
		}

		// Gets the timer's time
		Uint32 getTicks()
		{
			// the actual timer time
			Uint32 time = 0;

			// if the timer is running
			if (mStarted)
			{
				// if the timer is paused
				if (mPaused)
				{
					// return the number of ticks when the timer was paused
					time = mPausedTicks;
				}
				else 
				{
					// return the currrent time minus the start time
					time = SDL_GetTicks() - mStartTicks;
				}
			}

			return time;
		}

		// Checks the status of the timer
		bool isStarted()
		{
			// timer is running and paused or unpaused
			return mStarted;
		}

		bool isPaused()
		{
			// timer is running and paused
			return mPaused;
		}
};
// ========================== Global Variables ==========================
// The window we are going to render to
SDL_Window* gWindow = NULL;

// The window renderer 
SDL_Renderer* gRenderer = NULL;

// globally used font
TTF_Font* gFont = NULL;

// Time texture and prompt textures 
LTexture gTimeTexture;

// Global key textures
LTexture gUpTexture;
LTexture gDownTexture;
LTexture gRightTexture;
LTexture gLeftTexture;
LTexture gPressTexture;

// Button texture and everything else 
LTexture gButtonSpriteSheetTexture;
SDL_Rect gButtonClips[TOTAL_BUTTONS];
LButton gButtons[TOTAL_BUTTONS];

// ========================== Button Wrapper Class Function Definitions ==========================
LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	// if mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		// get the mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		// check if mouse is in button
		bool inside = true;

		// mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		// mouse is right of button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		// mouse is above the button
		else if (y < mPosition.y)
		{
			inside = false;
		} 
		// mouse is below button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		// mouse is outside the button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		// mouse is inside the button
		else 
		{
			// set the mouse over the sprite
			switch (e->type)
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
			}
		}

	}
}

void LButton::render()
{
	// show current button sprite
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gButtonClips[mCurrentSprite]);
}

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


#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	// if there is any left over memory loaded, free it first
	free();

	// render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		// create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// get the image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		// get rid of the old surface
		SDL_FreeSurface(textSurface);
	}

	// return success
	return mTexture != NULL;
}
#endif

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	// modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	// set the blending function
	SDL_SetTextureBlendMode(mTexture, blending); 
}

void LTexture::setAlpha(Uint8 alpha)
{
	// modulate the alpha texture
	SDL_SetTextureAlphaMod(mTexture, alpha);
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

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
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
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
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
				// initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}

			// initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError()); 
				success = false;
			}

			// Initialize SDL_ttf
			if (TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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

	#if defined(SDL_TTF_MAJOR_VERSION)
	// Open the font 
	gFont = TTF_OpenFont("media/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	#endif

	return success;
}

void close()
{
	// you might think we have to free the textures that we declared as globals, but actually
	// they go out of scope and the destructor is automatically called, which is dope
	// the only reason these are listed here is because they should be freed if they 
	// weren't global resources 
	gTimeTexture.free();

	// Free the global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	// Destroy the window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL; 
	gRenderer = NULL;

	// Quit SDL Subsystems
	IMG_Quit();
	TTF_Quit();
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

		// set text color as black
		SDL_Color textColor = {0,0,0,255};

		// frames per second timer 
		LTimer fpsTimer;

		// the frames per second cap timer
		LTimer capTimer;

		// In memory text stream
		std::stringstream timeText;

		// start counting frames per second
		int countedFrames = 0;
		fpsTimer.start();

		// The main loop of the game
		while (!quit) 
		{
			// start the cap timer
			capTimer.start();

			// handle events on the queue
			while (SDL_PollEvent(&e) != 0) 
			{
				// the user requests to quit
				if (e.type == SDL_QUIT) 
				{
					quit = true;
				}
			}

			// Calculate and correct fps
			float avgFps = countedFrames / (fpsTimer.getTicks() / 1000.f);
			if (avgFps > 2000000)
			{
				avgFps = 0;
			}

			// set text to be rendered
			timeText.str("");
			timeText << "Average frames per second (with cap) " << avgFps; 

			// render text
			if (!gTimeTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
			{
				printf("Unable to render time texture!\n");
			}

			// Clear the screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			// render textures 
			gTimeTexture.render((SCREEN_WIDTH - gTimeTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTimeTexture.getHeight()) / 2);

			// Update screen
			SDL_RenderPresent(gRenderer);
			++countedFrames;

			// if the frame finished early
			int frameTicks = capTimer.getTicks();
			if (frameTicks < SCREEN_TICKS_PER_FRAME)
			{
				// wait the remaining time
				SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
			}
		}
	}

	// close out resources and SDL
	close();

  	return 0;
}