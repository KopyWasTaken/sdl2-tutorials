#ifndef KTEXTURE_C
#define KTEXTURE_C

#include "ktexture.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <stdlib.h>

/**
 * Returns a pointer to a KTexture stored on the heap. SDL_Texture within is
 * pointing to null.
 */
KTexture *ktexture_init() {
  KTexture *toReturn = (KTexture*) malloc(sizeof(KTexture));
  toReturn->width = 0;
  toReturn->height = 0;
  toReturn->texture = NULL;
  return toReturn;
}

// load image from file
bool ktexture_load_from_file(SDL_Renderer *renderer, KTexture *kTexture,
                             char *path) {
  // Get rid of the pre-existing texture
  free(kTexture->texture);

  // the final texture
  SDL_Texture *newTexture = NULL;

  // load the image at the specified path
  SDL_Surface *loadedSurface = IMG_Load(path);
  if (loadedSurface == NULL) {
    SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path,
            IMG_GetError());
  } else {
    SDL_SetSurfaceColorKey(
        loadedSurface, SDL_TRUE,
        SDL_MapSurfaceRGB(loadedSurface, 0, 0xFF, 0xFF));

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == NULL) {
      SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path,
              SDL_GetError());
    } else {
      // Get the image dimensions
      kTexture->width = loadedSurface->w;
      kTexture->height = loadedSurface->h;
    }

    // Get rid of the old loaded surface
    SDL_DestroySurface(loadedSurface);
  }

  // return success 
  kTexture->texture = newTexture;
  return kTexture->texture != NULL;
}

// Free the texture, but I'm not sure if this leaks or not
void ktexture_free(KTexture *kTexture) {
  // free the texture if it exists
  if (kTexture->texture != NULL) {
    SDL_DestroyTexture(kTexture->texture);
    kTexture->texture = NULL;
    kTexture->width = 0;
    kTexture->height = 0;
  }
}

// set the color modulation
void ktexture_set_color(KTexture *kTexture, uint8_t red, uint8_t green,
                        uint8_t blue) {
  // Modulate the texture RGB
  SDL_SetTextureColorMod(kTexture->texture, red, green, blue);
}

// Set blending
void ktexture_set_blend_mode(KTexture *kTexture, SDL_BlendMode blending) {
  // Modulate texture alpha
  SDL_SetTextureBlendMode(kTexture->texture, blending);
}

// Set alpha modulation
void ktexture_set_alpha(KTexture *kTexture, uint8_t alpha) {
  // Modulate texture alpha
  SDL_SetTextureAlphaMod(kTexture->texture, alpha);
}

// Renders the texture at a given point
void ktexture_render(KTexture *kTexture, SDL_Renderer *renderer, int x, int y) {
  // Set rendering space and render to screen
  SDL_FRect renderQuad = {(float)x, (float)y, (float)kTexture->width,
                          (float)kTexture->height};

  // render to screen
  SDL_RenderTextureRotated(renderer, kTexture->texture, NULL, &renderQuad, 0.0,
                           NULL, SDL_FLIP_NONE);
}

#endif
