#include <cstdio>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


void create_palette_vga(SDL_Color *colors) {
  colors[15].r = colors[15].g = colors[15].b = 255;
  for (int i = 0; i < 16; i++) {
    uint8_t c = i * 16;
    colors[i].r = c;
    colors[i].g = c;
    colors[i].b = c;
  }
  for (int i = 16; i < 256; i++) {
    uint8_t c = i;
    uint8_t l = c & 0x03;
    uint8_t w = 21;
    colors[i].r = ((c & 0xc0) >> 6) * 64 + l * w;
    colors[i].g = ((c & 0x30) >> 4) * 64 + l * w;
    colors[i].b = ((c & 0x0c) >> 2) * 64 + l * w;
  }

  // print the result
  for (int i = 0; i < 256; i++) {
    printf("%03d = %02x %02x %02x  %s\n",
           i, colors[i].r, colors[i].g, colors[i].b,
           (colors[i].r == colors[i].g && colors[i].g == colors[i].b) ? "MONO" : "");
  }
}

void create_palette_greyscale(SDL_Color *colors) {
  colors[15].r = colors[15].g = colors[15].b = 255;
  for (int i = 0; i < 256; i++) {
    uint8_t c = i & 0xff;
    colors[i].r = c;
    colors[i].g = c;
    colors[i].b = c;
  }
}

int main() {
  int w = 640;
  int h = 1024;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Foo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                                        SDL_WINDOW_SHOWN);
  SDL_Surface *screen = SDL_GetWindowSurface(window);
  SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0);

  /* Fill colors with color information */
  SDL_Color colors[256];
  create_palette_vga(colors);
  SDL_SetPaletteColors(surface->format->palette, colors, 0, 256);

  //uint16_t color = 0;
  while (true) {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0) {
      switch (e.type) {
        case SDL_QUIT:
          return EXIT_SUCCESS;
      }
    }
    uint8_t *offscreen = (uint8_t *) surface->pixels;
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        offscreen[j] = i / 4;
      }
      offscreen += surface->pitch;
    }
    SDL_BlitSurface(surface, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(100);
    //color++;
  }
}
