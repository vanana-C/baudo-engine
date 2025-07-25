#ifndef BAUDO_ENGINE_H
#define BAUDO_ENGINE_H

#include <SDL2/SDL.h>

//int baudo_engine_loop(SDL_Window* window,uint8_t file_type,char* file_path[]);
int baudo_engine_loop(SDL_Window* window,char* vao_file,char* bem_file);

#endif
