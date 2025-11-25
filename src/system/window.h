#pragma once
#include <SDL3/SDL.h>

typedef struct EvWindow_T* EvWindow;

typedef struct EvWindow_T{
    SDL_Window* window;
    SDL_Event event;
} EvWindow_T;

EvWindow evCreateWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, const char* pTitle);
void evDestroyWindow(EvWindow window);
bool evWindowClose(EvWindow window);