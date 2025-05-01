#include <time.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/utils.h"

signed int drawCircleAtCoord(SDL_Renderer *renderer, const int x, const int y, const float radius, const int precision, const SDL_Color color) {
    const float increment = 2.0f * 3.14 / precision;
    SDL_Vertex vertices[precision + 2];

    // Centre du cercle
    vertices[0].position.x = x;
    vertices[0].position.y = y;
    vertices[0].color = color;
    vertices[0].tex_coord.x = 0;
    vertices[0].tex_coord.y = 0;

    for (int i = 0; i <= precision; i++) {
        float angle = i * increment;
        vertices[i + 1].position.x = roundf(x + radius * cosf(angle));
        vertices[i + 1].position.y = roundf(y + radius * sinf(angle));
        vertices[i + 1].color = color;
        vertices[i + 1].tex_coord.x = 0;
        vertices[i + 1].tex_coord.y = 0;
    }

    // Chaque triangle est (centre, i, i+1)
    for (int i = 1; i <= precision; i++) {
        SDL_Vertex triangle[3] = { vertices[0], vertices[i], vertices[i + 1] };
        if (SDL_RenderGeometry(renderer, NULL, triangle, 3, NULL, 0) < 0) {
            return -1;
        }
    }
    return 0;
}


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