#include "SDL_Display.hpp"
#include <SDL2/SDL.h>


SDL_Display::SDL_Display(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

SDL_Display::~SDL_Display()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void SDL_Display::update_display(void const* buffer, int pitch)
{
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

bool SDL_Display::process_input(uint8_t* keyboard)
{
	bool quit = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				quit = true;
			} break;

			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					{
						quit = true;
					} break;

					case SDLK_x:
					{
						keyboard[0] = 1;
					} break;

					case SDLK_1:
					{
						keyboard[1] = 1;
					} break;

					case SDLK_2:
					{
						keyboard[2] = 1;
					} break;

					case SDLK_3:
					{
						keyboard[3] = 1;
					} break;

					case SDLK_q:
					{
						keyboard[4] = 1;
					} break;

					case SDLK_w:
					{
						keyboard[5] = 1;
					} break;

					case SDLK_e:
					{
						keyboard[6] = 1;
					} break;

					case SDLK_a:
					{
						keyboard[7] = 1;
					} break;

					case SDLK_s:
					{
						keyboard[8] = 1;
					} break;

					case SDLK_d:
					{
						keyboard[9] = 1;
					} break;

					case SDLK_z:
					{
						keyboard[0xA] = 1;
					} break;

					case SDLK_c:
					{
						keyboard[0xB] = 1;
					} break;

					case SDLK_4:
					{
						keyboard[0xC] = 1;
					} break;

					case SDLK_r:
					{
						keyboard[0xD] = 1;
					} break;

					case SDLK_f:
					{
						keyboard[0xE] = 1;
					} break;

					case SDLK_v:
					{
						keyboard[0xF] = 1;
					} break;
				}
			} break;

			case SDL_KEYUP:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_x:
					{
						keyboard[0] = 0;
					} break;

					case SDLK_1:
					{
						keyboard[1] = 0;
					} break;

					case SDLK_2:
					{
						keyboard[2] = 0;
					} break;

					case SDLK_3:
					{
						keyboard[3] = 0;
					} break;

					case SDLK_q:
					{
						keyboard[4] = 0;
					} break;

					case SDLK_w:
					{
						keyboard[5] = 0;
					} break;

					case SDLK_e:
					{
						keyboard[6] = 0;
					} break;

					case SDLK_a:
					{
						keyboard[7] = 0;
					} break;

					case SDLK_s:
					{
						keyboard[8] = 0;
					} break;

					case SDLK_d:
					{
						keyboard[9] = 0;
					} break;

					case SDLK_z:
					{
						keyboard[0xA] = 0;
					} break;

					case SDLK_c:
					{
						keyboard[0xB] = 0;
					} break;

					case SDLK_4:
					{
						keyboard[0xC] = 0;
					} break;

					case SDLK_r:
					{
						keyboard[0xD] = 0;
					} break;

					case SDLK_f:
					{
						keyboard[0xE] = 0;
					} break;

					case SDLK_v:
					{
						keyboard[0xF] = 0;
					} break;
				}
			} break;
		}
	}

	return quit;
}
