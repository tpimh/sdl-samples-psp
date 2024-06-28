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
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, font->texWidth, font->texHeight);

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
    intraFontPrint(font, 240, 240, "Hello_PSP!");

    SDL_UpdateTexture(texture, &rect, font->texture, rect.w * sizeof(Uint32));

    SDL_Vertex v[font->v_size];

    for (int i = 0; i < font->v_size; i++)
    {
      // printf("v[%d].u = %0.2f (tex u) ", i, font->v[i].u);
      // printf("v[%d].v = %0.2f (tex v) ", i, font->v[i].v);
      // printf("v[%d].c = %x (rgba8888) ", i, font->v[i].c);
      // printf("v[%d].x = %2.2f ", i, font->v[i].x);
      // printf("v[%d].y = %2.2f ", i, font->v[i].y);
      // printf("v[%d].z = %2.2f ", i, font->v[i].z);
      // printf("\n");

      struct fontVertex fv = font->v[i];
      SDL_Color c = {fv.c >> 24, fv.c >> 16, fv.c >> 8, fv.c};
      SDL_FPoint p = {fv.x, fv.y};
      SDL_FPoint t = {fv.u, fv.v};
      v[i] = (SDL_Vertex){p, c, t};
      SDL_RenderDrawPointF(renderer, p.x, p.y);
      SDL_RenderDrawPointF(renderer, t.x * rect.w, t.y * rect.h);
    }

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderCopy(renderer, texture, &rect, &rect);
    SDL_RenderGeometry(renderer, texture, &v[60], 60, NULL, 0);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);

  return 0;
}
