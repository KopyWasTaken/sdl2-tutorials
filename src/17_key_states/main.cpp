// ========================== Includes ==========================
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

// ========================== Constants and Enums ==========================
// screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

// ========================== Global Variables ==========================
// The window we are going to render to
SDL_Window* gWindow = NULL;

// The window renderer 
SDL_Renderer* gRenderer = NULL;

// globally used font
TTF_Font* gFont = NULL;

// rendered texture
LTexture gTextTexture;

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
		printf("Freed texture\n");
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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	gFont = TTF_OpenFont("media/lazy.ttf", 56);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		// Render text
		SDL_Color textColor = {0,0,0};
		if (!gTextTexture.loadFromRenderedText(" The quick brown fox jumps over the lazy dog", textColor))
		{
			printf("Failed to render text from texture!\n");
			success = false;
		}
	}
	#endif

	// load key press textures
	if (!gUpTexture.loadFromFile("media/up.png"))
	{
		printf("Failed to load up texture!\n");
		success = false;
	}
	if (!gDownTexture.loadFromFile("media/down.png"))
	{
		printf("Failed to load down texture!\n");
		success = false;
	}
	if (!gRightTexture.loadFromFile("media/right.png"))
	{
		printf("Failed to load right texture!\n");
		success = false;
	}
	if (!gLeftTexture.loadFromFile("media/left.png"))
	{
		printf("Failed to load left texture!\n");
		success = false;
	}
	if (!gPressTexture.loadFromFile("media/press.png"))
	{
		printf("Failed to load press texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	// you might think we have to free the textures that we declared as globals, but actually
	// they go out of scope and the destructor is automatically called, which is dope
	gTextTexture.free();

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

		// Current rendered texture
		LTexture* currentTexture = NULL; 

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

			// Set textuure based on current keystate
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_UP])
			{
				currentTexture = &gUpTexture;
			}
			else if (currentKeyStates[SDL_SCANCODE_DOWN])
			{
				currentTexture = &gDownTexture;
			} 
			else if (currentKeyStates[SDL_SCANCODE_LEFT])
			{
				currentTexture = &gLeftTexture;
			}
			else if (currentKeyStates[SDL_SCANCODE_RIGHT])
			{
				currentTexture = &gRightTexture;
			}		
			else 
			{
				currentTexture = &gPressTexture;
			}

			// Clear the screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			// render current texture 
			currentTexture->render(0, 0);

			// Update screen
			SDL_RenderPresent(gRenderer);
		}
	}

	// close out resources and SDL
	close();

  	return 0;
}