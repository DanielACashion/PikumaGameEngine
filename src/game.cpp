#include "game.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <cstddef>
#include <iostream>

Game::Game() { std::cout << "Game contstructor was called \n"; }
Game::~Game() { std::cout << "Game destructor was called \n"; }
void Game::Initialize() {
  int init_response = SDL_Init(SDL_INIT_EVERYTHING);
  if (init_response < 0) {
    std::cout << "failed to init sdl\n";
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
    std::cout << "failed to setup window\n";
    return;
  }
  this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!this->renderer) {
    this->_should_run = false;
    std::cout << "failed to setup renderer\n";
    return;
  }
  // SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN);
  this->_should_run = true;
}
void Game::Run() {
  while (this->_should_run) {
    ProcessInput();
    Update();
    Render();
  }
}
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
void Game::Update() {}
void Game::Render() {
  SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
  SDL_RenderClear(this->renderer);
  SDL_RenderPresent(this->renderer);
}
void Game::Destroy() {
  if (this->renderer) {
    SDL_DestroyRenderer(this->renderer);
  }
  if (this->window) {
    SDL_DestroyWindow(this->window);
  }
  SDL_Quit();
}