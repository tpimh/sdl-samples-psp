#include <SDL.h>

#include <intraFont.h>

#include <GL/gl.h>

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

  SDL_Rect rect = {0, 0, 480, 272};
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 480, 272);

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

    intraFontSetStyle(font, 1.0f, 0xFF000000, 0xFFFFFFFF, 0.0f, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(font, 240, 240, "Hello PSP!");

    void *pixels;
    int pitch;
    SDL_LockTexture(texture, &rect, &pixels, &pitch);
    glReadPixels(0, 0, 480, 272, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    SDL_UnlockTexture(texture);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);

  return 0;
}
