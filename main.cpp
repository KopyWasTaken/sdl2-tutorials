// Use SDL and Standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

// screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main( int argc, char* args[])
{ 
  // The window we will render to
  SDL_Window* window = NULL;

  // The surface contained by the window
  SDL_Surface* screenSurface = NULL;

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    // Create the window
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    { 
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
      // Get the window surface
      screenSurface = SDL_GetWindowSurface(window);

      // Fill the surface white
      SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

      // Update the surface
      SDL_UpdateWindowSurface(window);

      // Hack to get the window to stay up
      SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
    }
  }

  // Destroy the window
  SDL_DestroyWindow(window);

  // Quit SDL Subsystems 
  SDL_Quit();

  return 0;
}

