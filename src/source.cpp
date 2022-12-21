#include <SDL3/SDL.h>
#include <iostream>
#include "Object.h"
#include <vector>

void drawFrame();
void mainLoop();
void processKeys(SDL_Event &event);
/* Sets constants */
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
object *objects[2];
unsigned int currentObjIndex = 0;
float lightSource[] = {0, 4, 0, 0};

int main(int argc, char **argv)
{

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "SDL failed to initialise: " << SDL_GetError() << "\n";
    return 1;
  }

  window = SDL_CreateWindow("SDL Example",           /* Title of the SDL window */
                            SDL_WINDOWPOS_UNDEFINED, /* Position x of the window */
                            SDL_WINDOWPOS_UNDEFINED, /* Position y of the window */
                            WIDTH,                   /* Width of the window in pixels */
                            HEIGHT,                  /* Height of the window in pixels */
                            0);                      /* Additional flag(s) */


  /* Checks if window has been created; if not, exits program */
  if (window == NULL)
  {
    std::cerr << "SDL window failed to initialise: " << SDL_GetError() << "\n";
    return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, 0x00);

  if (renderer == NULL)
  {
    std::cerr << "SDL renderer failed to initialise: " << SDL_GetError() << "\n";
    return 1;
  }

  objects[0] = new object(8, "../src/parallelepiped.txt");
  objects[1] = new object(4, "../src/tetrahedron.txt");

  mainLoop();

  /* Frees memory */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  /* Shuts down all SDL subsystems */
  SDL_Quit();

  return 0;
}
void mainLoop()
{
  SDL_Event event;
  bool quit = false;
  while (!quit)
  {
    while (SDL_PollEvent(&event))
    {
      SDL_PumpEvents();
      if (event.type == SDL_QUIT)
      {
        quit = true;
      }
      if (event.type == SDL_KEYDOWN)
      {
        processKeys(event);
      }
    }
    drawFrame();
    SDL_Delay(DELAY);
  }
}
void drawFrame()
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  std::vector<polygon *> drawConveyor;

  objects[0]->getShadePolygons(drawConveyor);
  objects[1]->getShadePolygons(drawConveyor);
  objects[0]->getPolygons(drawConveyor);
  objects[1]->getPolygons(drawConveyor);

  for (polygon *i : drawConveyor)
  {
    i->draw(renderer);
    delete i;
  }
  drawConveyor.clear();

  SDL_RenderPresent(renderer);
}
void processKeys(SDL_Event &event)
{
  switch (event.key.keysym.sym)
  {
  case SDLK_c:
    currentObjIndex = (currentObjIndex + 1) % 2;
    break;
  case SDLK_w:
    objects[currentObjIndex]->move(0, 0, 10);
    break;
  case SDLK_a:
    objects[currentObjIndex]->move(-10, 0, 0);
    break;
  case SDLK_s:
    objects[currentObjIndex]->move(0, 0, -10);
    break;
  case SDLK_d:
    objects[currentObjIndex]->move(10, 0, 0);
    break;
  case SDLK_e:
    objects[currentObjIndex]->move(0, -10, 0);
    break;
  case SDLK_f:
    objects[currentObjIndex]->move(0, 10, 0);
    break;
  case SDLK_h:
    objects[currentObjIndex]->rotate(gradToRad(-15), AxisZ);
    break;
  case SDLK_k:
    objects[currentObjIndex]->rotate(gradToRad(15), AxisZ);
    break;
  case SDLK_u:
    objects[currentObjIndex]->rotate(gradToRad(15), AxisX);
    break;
  case SDLK_j:
    objects[currentObjIndex]->rotate(gradToRad(-15), AxisX);
    break;
  case SDLK_n:
    objects[currentObjIndex]->rotate(gradToRad(15), AxisY);
    break;
  case SDLK_m:
    objects[currentObjIndex]->rotate(gradToRad(-15), AxisY);
    break;
  case SDLK_UP:
    objects[currentObjIndex]->scale(1.1, 1.1, 1.1);
    break;
  case SDLK_DOWN:
    objects[currentObjIndex]->scale(0.9, 0.9, 0.9);
    break;
  default:
    break;
  }
}
