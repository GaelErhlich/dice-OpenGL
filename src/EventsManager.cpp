#include "EventsManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

void manageEvents(bool &isOpened) {
    //Fetch the SDL events
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
            //We can add more event, like listening for the keyboard or the mouse. See SDL_Event documentation for more details
        }
    }
}