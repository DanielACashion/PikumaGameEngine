#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
  bool _should_run = false;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *tank_texture;
  void ProcessInput();
  void Update();
  void Render();
  void SetUp();

public:
  int window_width = 800;
  int window_height = 600;
  Game();
  ~Game();
  void Initialize();
  void Run();
  void Destroy();
};
#endif