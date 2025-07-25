#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "baudo_engine.h"

int main(int argc,char* argv[])
{
	int return_value=0;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window=SDL_CreateWindow("baudo-engine",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,500,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);

	SDL_GLContext context=SDL_GL_CreateContext(window);

	glewInit();

	int state=0;
	bool running=true;
	while(running)
	{
		switch(state)
		{
			case 0:
				state=baudo_engine_loop(window,argv[2],argv[1]);
				//state=baudo_engine_loop(window,0,NULL);
				break;
			case 1:
				running=false;
				break;
			default:
				return_value=state;
				running=false;
				break;
		}
	}

	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return return_value;
}
