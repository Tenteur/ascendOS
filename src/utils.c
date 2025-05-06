#include <time.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/utils.h"

// TTF_Font *TTFSDLFont;


// This function return an array of pointer in this order: TTF_Font, ...
int utilsInit() {
    TTF_Init();
    // TTFSDLFont = malloc(1000000); // malloc is not allowed in a constant expression
    // TTFSDLFont = TTF_OpenFont("/home/vianney/VSCode/appSDL/static/Roboto-Regular.ttf", 100);
    return 0;
}
int utilsQuit() {
    TTF_Quit();
    // free(TTFSDLFont);
    return 0;
}

// Text rendering segment:
// ? Don't know if this function works flawlessly or is optimized
// ? // BUG: Check and test this function.
// ? The texture only works !
SDL_Texture *generateTextTexture(SDL_Renderer *renderer, TTF_Font *TTFSDLFont, const char *text, const SDL_Color *textColor) {
    SDL_Surface *textSurface = TTF_RenderText_Blended(TTFSDLFont, text, *textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    // if (textTexture == NULL) {
    //     printf("%s\n", SDL_GetError());
    // } else {
    //     printf("success for texture\n");
    // }
    int h, w;
    SDL_QueryTexture(textTexture, NULL, NULL, &w, &h);
    // if (result == 0) {
    //     printf("Query success\n");
    //     printf("%u, %u\n", w, h);
    // } else {
    //     printf("Query error\n");
    // }
    size_t estimated_size = w * h * 4;
    // printf("%lu Bytes\n", (long unsigned int)estimated_size);
    SDL_Texture *textTextureNewPtr = malloc(estimated_size);
    memcpy(textTextureNewPtr, textTexture, estimated_size);

    SDL_FreeSurface(textSurface);
    return textTexture;
}
// ? This function seems to work overall.
// ! TODO: Clean this function
int renderTextAtCoord(SDL_Renderer *renderer, TTF_Font *TTFSDLFont, char *text, const int x, const int y, SDL_Color *textColor, bool useCustomSize, SDL_Rect *customSizeRect) {
    SDL_Texture *textTexture = generateTextTexture(renderer, TTFSDLFont, text, textColor);
    // SDL_Surface *tempSurface = TTF_RenderText_Blended(TTFSDLFont, text, *textColor);
    // SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_Rect rectSize;
    if (useCustomSize) {
        rectSize.h = customSizeRect->h;
        // printf("%u", rectSize.h);
        rectSize.w = customSizeRect->w;
        // printf("%u", rectSize.w);
    } else if (!useCustomSize) {
        SDL_QueryTexture(textTexture, NULL, NULL, &(rectSize.w), &(rectSize.h));
        // rectSize.h = customSizeRect->h;
        // rectSize.w = customSizeRect->w;
    }
    rectSize.x = x;
    // printf("%u", rectSize.x);
    rectSize.y = y;
    // printf("%u", rectSize.y);
    int success = SDL_RenderCopy(renderer, textTexture, NULL, &rectSize);
    // if (success == 0) {
    //     printf("success!\n");
    // } else {
    //     printf("%s\n", SDL_GetError());
    // }
    // SDL_FreeSurface(tempSurface);
    // SDL_DestroyTexture(tempTexture);

    free(textTexture);
    // free(textTextureStruct);
    return 0;
}


// * all of these mostly works. ↓
// Other rendering segment:

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
