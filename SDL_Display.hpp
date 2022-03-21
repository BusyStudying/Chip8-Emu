#pragma once
#include <cstdint>

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class SDL_Display {
public:
    SDL_Display(char const* window_title, int window_width, int window_height, int texture_width, int texture_height);
    ~SDL_Display();
    void update_display(void const* buffer, int pitch);
    bool process_input(uint8_t *keyboard);
private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};

};