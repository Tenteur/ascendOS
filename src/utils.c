/**
 * @file utils.c
 * @brief utils.c contains implementation of functions declared in utils.h
 * These functions are used to draw on screen and do some derivative things.
 */

#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/utils.h"

// ! // @TODO: Clean this file, this is a little mess



int utilsInit() {
    return 0;
}
int utilsQuit() {
    // Ignore removing every fonts, losing time (instead of writing other code)
    // Quitting other modules
    TTF_Quit();
    // TODO: write code to free all the fonts.
    // free(TTFSDLFont);
    return 0;
}

// Text rendering segment:

// ! // @TODO: check if font already exists (by using selectFontFromList maybe ?)



// * all of these mostly works. ↓
// Other rendering segment:




transitionProperties *transitionBackgroundColorInit(SDL_Color *startingColor, SDL_Color *endingColor, const int transitionNumberFrames) {
    transitionProperties backgroundTransition = {
        startingColor, 
        (endingColor->r - startingColor->r) / transitionNumberFrames,
        (endingColor->g - startingColor->g) / transitionNumberFrames,
        (endingColor->b - startingColor->b) / transitionNumberFrames,
        (endingColor->a - startingColor->a) / transitionNumberFrames,
        transitionNumberFrames
    };
    transitionProperties *backgroundTransitionPtr = malloc(sizeof(transitionProperties));
    *backgroundTransitionPtr = backgroundTransition;
    return backgroundTransitionPtr;
}
// Return 1 if frames still need to be rendered, 0 if finished.
bool transitionBackgroundColorNextFrame(SDL_Renderer *renderer, transitionProperties *transitionPropertiesItem) {
    if (transitionPropertiesItem->remainingFrames >= 0) {
        transitionPropertiesItem->currentSDLColor->r += transitionPropertiesItem->red;
        transitionPropertiesItem->currentSDLColor->g += transitionPropertiesItem->green;
        transitionPropertiesItem->currentSDLColor->b += transitionPropertiesItem->blue;
        transitionPropertiesItem->currentSDLColor->a += transitionPropertiesItem->alpha;
        transitionPropertiesItem->remainingFrames -= 1;
        SDL_SetRenderDrawColor(
            renderer,
            (int)round(transitionPropertiesItem->currentSDLColor->r),
            (int)round(transitionPropertiesItem->currentSDLColor->g),
            (int)round(transitionPropertiesItem->currentSDLColor->b),
            (int)round(transitionPropertiesItem->currentSDLColor->a)
        );
        return 1;
    } else {
        return 0;
    }
}
