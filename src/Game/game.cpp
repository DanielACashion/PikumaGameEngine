#include "game.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include "glm/ext/vector_float2.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstddef>
#include <glm/glm.hpp>

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

Game::Game() { Logger::Log("Game contstructor was called"); }
Game::~Game() { Logger::Log("Game destructor was called"); }

// START PUBLICS
void Game::Initialize() {
  int init_response = SDL_Init(SDL_INIT_EVERYTHING);
  if (init_response < 0) {
    Logger::Error("failed to init sdl\n");
    this->_should_run = false;
    return;
  }
  SDL_DisplayMode displaymode;
  SDL_GetCurrentDisplayMode(0, &displaymode);
  // this->window_width = displaymode.w;
  // this->window_height = displaymode.h;
  this->window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, this->window_width,
                                  this->window_height, SDL_WINDOW_BORDERLESS);
  if (!this->window) {
    this->_should_run = false;
    Logger::Error("failed to setup window");
    return;
  }
  this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!this->renderer) {
    this->_should_run = false;
    Logger::Error("failed to setup renderer");
    return;
  }
  // SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN);
  this->_should_run = true;
  SetUp();
}

void Game::Run() {
  while (this->_should_run) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destroy() {
  if (this->renderer) {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
  }
  if (this->window) {
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
  }
  if (this->tank_texture) {
    SDL_DestroyTexture(this->tank_texture);
    this->tank_texture = nullptr;
  }
  SDL_Quit();
}
// END PUBLICS

void Game::ProcessInput() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    this->_should_run = false;
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      this->_should_run = false;
      break;
    }
    break;
  }
}

int millisecs_previous_frame = 0;
void Game::Update() {
  int timeToWait =
      MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecs_previous_frame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
    return;
  }
  double delta_time =
      ((double)SDL_GetTicks() - (double)millisecs_previous_frame) / 1000.0f;
  millisecs_previous_frame = SDL_GetTicks();
  playerPosition.x += (playerVelocity.x * delta_time);
  playerPosition.y += (playerVelocity.y * delta_time);
}

void Game::SetUp() {
  // load startup
  playerPosition = glm::vec2(10.0, 20.0);
  playerVelocity = glm::vec2(100.0, 0.0);

  SDL_Surface *surface = IMG_Load("./assets/images/tank-tiger-right.png");
  if (!surface) {
    Logger::Error("failed to loadimage");
  }
  this->tank_texture = SDL_CreateTextureFromSurface(this->renderer, surface);
  SDL_FreeSurface(surface);
  surface = nullptr;
  if (!this->tank_texture) {
    this->_should_run = false;
    Logger::Error("Failed to move tank texture from surface to texture");
  }

  millisecs_previous_frame = SDL_GetTicks(); // this should be last
}

void Game::Render() {

  SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
  SDL_RenderClear(this->renderer);

  //
  SDL_Rect tank_rect = {.x = static_cast<int>(playerPosition.x),
                        .y = static_cast<int>(playerPosition.y),
                        .w = 32,
                        .h = 32};
  SDL_Rect player = {.x = 10, .y = 10, .w = 20, .h = 20};
  SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(this->renderer, &player);
  SDL_RenderCopy(this->renderer, this->tank_texture, NULL, &tank_rect);
  //

  SDL_RenderPresent(this->renderer);
}