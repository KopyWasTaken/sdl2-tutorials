#ifndef KTEXTURE_H
#define KTEXTURE_H

// Using SDL, SDL Image
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct KTexture_s {
		int width;
		int height;
		SDL_Texture* texture;
} KTexture;

/**
 * Returns a pointer to a KTexture stored on the heap. SDL_Texture within is pointing to null.
 */
KTexture* ktexture_init();

// load image from file 
bool ktexture_load_from_file(SDL_Renderer* renderer, KTexture* kTexture, char* path);

// Free the texure
void ktexture_free(KTexture* kTexture);

// set the color modulation
void ktexture_set_color(KTexture* kTexture, uint8_t red, uint8_t green, uint8_t blue);

// Set blending
void ktexture_set_blend_mode(KTexture* kTexture, SDL_BlendMode blending);

// Set alpha modulation
void ktexture_set_alpha(KTexture* kTexture, uint8_t alpha);

// Renders the texture at a given point
void ktexture_render(KTexture *kTexture, SDL_Renderer *renderer, int x, int y);

#endif
