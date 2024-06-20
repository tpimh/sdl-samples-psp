#include <SDL.h>

#include <intraFont.h>

enum colors
{
  /* ARGB */
  RED = 0xFF0000FF,
  GREEN = 0xFF00FF00,
  BLUE = 0xFFFF0000,
  WHITE = 0xFFFFFFFF,
  LITEGRAY = 0xFFBFBFBF,
  GRAY = 0xFF7F7F7F,
  DARKGRAY = 0xFF3F3F3F,
  BLACK = 0xFF000000,
};

struct fontVertex
{
  float u, v;
  unsigned int c;
  float x, y, z;
};

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

  intraFont *font = NULL;

  intraFontInit();
  font = intraFontLoad("ltn8.pgf", 0);

  SDL_Window *window = SDL_CreateWindow(
      "window",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      480,
      272,
      0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Rect rect = {0, 0, font->texWidth, font->texHeight};
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, font->texWidth, font->texHeight);

  int running = 1;
  SDL_Event event;
  while (running)
  {
    if (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_CONTROLLERDEVICEADDED:
        SDL_GameControllerOpen(event.cdevice.which);
        break;
      case SDL_CONTROLLERBUTTONDOWN:
        if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
          running = 0;
        break;
      }
    }

    SDL_RenderClear(renderer);

    intraFontSetStyle(font, 1.0f, BLACK, WHITE, 0.0f, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(font, 240, 240, "Hello PSP!");

    SDL_UpdateTexture(texture, &rect, font->texture, rect.w * sizeof(Uint32));

    for (int i = 0; i < font->v_size; i += 6)
    {
      SDL_Rect srcRect = {
          font->v[i].u * rect.w,
          font->v[i].v * rect.h,
          (font->v[i + 2].u - font->v[i].u) * rect.w,
          (font->v[i + 2].v - font->v[i].v) * rect.h
        };
      SDL_Rect dstRect = {
          font->v[i].x,
          font->v[i].y,
          font->v[i + 2].x - font->v[i].x,
          font->v[i + 2].y - font->v[i].y
        };
      SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    }

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderCopy(renderer, texture, &rect, &rect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);

  return 0;
}
