#include "main.h"
#include "ktexture.h"
#include "state.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// Screen dims
const int SCREEN_HEIGHT = 1080;
const int SCREEN_WIDTH = 1920;

// SDL Stuff
State state;

// Dot texture
KTexture *dotTexture;

bool init() {
  // Flag indicating success or failure
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL could not intialize! SDL Error: %s\n", SDL_GetError());
  } else {
    // create the window and the renderer within the state
    state.window = NULL;
    state.renderer = NULL;
    State *statePtr = &state;
    if (SDL_CreateWindowAndRenderer("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT,
                                    0, &state.window, &state.renderer) < 0) {
      SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else {
      // Enable vsync
      SDL_SetRenderVSync(state.renderer, 1);

      // Initialize renderer color
      SDL_SetRenderDrawColor(state.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

      // Initialize PNG Loading
      int imgFlags = IMG_INIT_PNG;
      if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("SDL_Image could not initialize! SDL_image Error: %s\n",
                IMG_GetError());
        success = false;
      }
    }
  }

  return success;
}

bool loadMedia() {
  bool success = true;

  // Load the dot texture
  dotTexture = ktexture_init();

  //if (!ktexture_load_from_file(state.renderer, dotTexture, "dot.bmp")) {
  //  SDL_Log("Failed to load dot texture!\n");
  //  success = false;
  //}

  return success;
}

void close() {
  // Free the loaded images
  ktexture_free(dotTexture);

  // Destroy the window
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  state.renderer = NULL;
  state.window = NULL;

  // Quit all the SDL Subsystems
  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  if (!init()) {
    SDL_Log("Failed to initialize!\n");
  } else {
    bool quit = false;

    SDL_Event *e;

    while (!quit) {
      // Handle queue events
      while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_EVENT_QUIT) {
          quit = true;
        }
      }
    }
  }
}
