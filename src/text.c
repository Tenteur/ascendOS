/**
 * @file text.c
 * @brief text.c contains implementations of functions declared inside text.h
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/text.h"

struct fontListStruct *allFontsListStart = NULL;
/// Store the pointer of the current font in use
TTF_Font *currentFontInUse = NULL;

signed int textInit() {
    TTF_Init();
    allFontsListStart = malloc(sizeof(fontListStruct));
    strcpy(allFontsListStart->fontName, "Roboto-Regular");
    allFontsListStart->fontSize = 100;
    allFontsListStart->fontPtr = TTF_OpenFont("/usr/share/fonts/TTF/Roboto-Regular.ttf", 100);
    allFontsListStart->next = NULL;
    currentFontInUse = allFontsListStart->fontPtr;
    printf("text Initialized\n");
    return 0;
}

signed int addFontToList(char *fontName, int fontSize) { // @todo: modify so it adds the font to the very beginning (gain some perf)
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