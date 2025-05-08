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

struct fontListStruct *allFontsListStart = NULL;
/// Store the pointer of the current font in use
TTF_Font *currentFontInUse = NULL;

int utilsInit() {
    TTF_Init();
    allFontsListStart = malloc(sizeof(fontListStruct));
    strcpy(allFontsListStart->fontName, "Roboto-Regular");
    allFontsListStart->fontSize = 100;
    allFontsListStart->fontPtr = TTF_OpenFont("/usr/share/fonts/TTF/Roboto-Regular.ttf", 100);
    allFontsListStart->next = NULL;
    currentFontInUse = allFontsListStart->fontPtr;
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
signed int addFontToList(char *fontName, int fontSize) {
    if (strcmp(fontName, "default") == 0) {
        currentFontInUse = allFontsListStart->fontPtr;
        return 0;
    }
    TTF_Font *oldFont = currentFontInUse;
    if (selectFontFromList(fontName, fontSize) == 0) return -1;
    currentFontInUse = oldFont;
    struct fontListStruct *currentFont = allFontsListStart;
    while (true) {
        if (currentFont->next == NULL) break; 
        else {
            currentFont = currentFont->next;
        }
    } // out of the loop: Found the last font of the list

    struct fontListStruct *newFontPtr = malloc(sizeof(fontListStruct));
    strcpy(newFontPtr->fontName, fontName);
    newFontPtr->fontSize = fontSize;
    char fontPath[250];
    sprintf(fontPath, "/usr/share/fonts/TTF/%s.ttf", fontName);
    newFontPtr->fontPtr = TTF_OpenFont(fontPath, fontSize);
    newFontPtr->next = NULL;
    currentFont->next = newFontPtr;
    // Font added at end of list
    return 0;
}
signed int selectFontFromList(char *fontName, int fontSize) {
    struct fontListStruct *currentFont = allFontsListStart;
    while (true) {
        if (currentFont->fontSize == fontSize && strcmp(currentFont->fontName, fontName) == 0 ) break; 
        else if (currentFont->next == NULL) return -1;
        else {
            currentFont = currentFont->next;
        }
    } // out of the loop: found the font
    currentFontInUse = currentFont->fontPtr;
    return 0;
}

SDL_Texture *generateTextTexture(SDL_Renderer *renderer, const char *text, const SDL_Color *textColor) {
    SDL_Surface *textSurface = TTF_RenderText_Blended(currentFontInUse, text, *textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);
    return textTexture;
}
// ? This function seems to work overall.
// ! TODO: Clean this function
signed int renderTextAtCoord(SDL_Renderer *renderer, const char *text, const int x, const int y, SDL_Color *textColor, bool useCustomSize, SDL_Rect *customSizeRect) {
    SDL_Texture *textTexture = generateTextTexture(renderer, text, textColor);
    SDL_Rect rectSize;
    
    if (useCustomSize) {
        rectSize.h = customSizeRect->h;
        rectSize.w = customSizeRect->w;
    } else if (!useCustomSize) {
        SDL_QueryTexture(textTexture, NULL, NULL, &(rectSize.w), &(rectSize.h));
    }
    rectSize.x = x;
    rectSize.y = y;
    SDL_RenderCopy(renderer, textTexture, NULL, &rectSize);
    SDL_DestroyTexture(textTexture);
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
