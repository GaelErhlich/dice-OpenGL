#include "EventsManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <iostream>
#include <numeric>


void manageEvents(bool &isOpened, float &upDownParameter, float &leftRightParameter, unsigned int &numberAmong7) {

    const float sensitivity = 0.05f;



    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_CLOSE:
                    isOpened = false;
                    break;
                default:
                    break;
                }
                break;
            
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                //case SDLK_DOWN
                case SDLK_ESCAPE:
                    isOpened = false;
                    break;

                case SDLK_UP:
                    upDownParameter = min(upDownParameter + sensitivity, 1.0f);
                    break;
                case SDLK_DOWN:
                    upDownParameter = max(0.0f, upDownParameter - sensitivity);
                    break;
                case SDLK_RIGHT:
                    leftRightParameter = min(leftRightParameter + sensitivity, 1.0f);
                    break;
                case SDLK_LEFT:
                    leftRightParameter = max(0.0f, leftRightParameter - sensitivity);
                    break;
                default:
                    break;
                }
                break;


            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    numberAmong7 = (numberAmong7 + 1) % 7;
                    break;
                default:
                    break;
                }
                break;
            
        }
    }
}