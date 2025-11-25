#include "system/window.h"
#include "utilities/error.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include <assert.h>
#include <stdlib.h>

void evWindowCreateWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, const char* pTitle, EvWindow window);

EvWindow evCreateWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, const char* pTitle) {
	assert(pTitle);

    EvWindow window = (EvWindow)malloc(sizeof(EvWindow_T));
    if(!window){
        EV_ERROR("Failed to allocate EvWindow window!!.\n");
    }

    evWindowCreateWindow(x, y, width, height, pTitle, window);

    return window;
}

void evDestroyWindow(EvWindow window) {
	SDL_DestroyWindow(window->window);
    SDL_Quit();
}
bool evWindowClose(EvWindow window){
    SDL_PollEvent(&window->event);
    if(window->event.type == SDL_EVENT_QUIT) return true;
    return false;
}
void evWindowCreateWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, const char* pTitle, EvWindow window){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        EV_ERROR("Failed to SDL_Init(SDL_INIT_VIDEO)!!.\n");
    }

    SDL_PropertiesID properties = SDL_CreateProperties();
    SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_X_NUMBER, x);
    SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y);
    SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
    SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
    SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN, true);

    window->window = SDL_CreateWindowWithProperties(properties);
    if(!window->window){
        fprintf(stderr, "SDL_CreateWindowWithProperties!!.");
        exit(EXIT_FAILURE);
    }
}
