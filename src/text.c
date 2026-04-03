/**
 * @file text.c
 * @brief text.c contains implementations of functions declared inside text.h
 */


#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../include/text.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/render_manager.h"

fontListStruct *allFontsListStart = nullptr;
TTF_Font *currentFontInUse = nullptr;

// TODO: Verify if it was well initialized.
signed int textInit() {
    TTF_Init();
    allFontsListStart = malloc(sizeof(fontListStruct));
    strcpy(allFontsListStart->fontName, "Roboto-Regular");
    allFontsListStart->fontSize = 40;
    allFontsListStart->fontPtr = TTF_OpenFont("/usr/share/fonts/TTF/Roboto-Regular.ttf", 40);
    allFontsListStart->next = nullptr;
    currentFontInUse = allFontsListStart->fontPtr;
    printf("text Initialized\n");
    return 0;
}

// TODO: Modify so it adds the font at the beginning.
signed int addFontToList(const char *fontName, int fontSize) {
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

    fontListStruct *newFontPtr = malloc(sizeof(fontListStruct));
    strcpy(newFontPtr->fontName, fontName);
    newFontPtr->fontSize = fontSize;
    char fontPath[250];
    sprintf(fontPath, "/usr/share/fonts/TTF/%s.ttf", fontName);
    newFontPtr->fontPtr = TTF_OpenFont(fontPath, (float)fontSize);
    if (newFontPtr->fontPtr == NULL) SDL_LogCritical(1, "Error while loading font: '%s'", SDL_GetError());
    newFontPtr->next = nullptr;
    currentFont->next = newFontPtr;
    // Font added at end of list
    return 0;
}

// TODO: Write this function (boring).
signed int removeFontFromList(const char *fontName, int fontSize) {
    return 0;
}

signed int selectFontFromList(const char *fontName, int fontSize) {
    fontListStruct *currentFont = allFontsListStart;
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

// TODO: This function can be cleaned i think.
SDL_Texture *generateTextTexture(SDL_Renderer *renderer, const char *text, const SDL_FColor *textColor, unsigned int sizeX, unsigned int sizeY) {

    TTF_TextEngine *textEngine = TTF_CreateRendererTextEngine(renderer);
    TTF_Text *textText = TTF_CreateText(textEngine, currentFontInUse, text, 0);
    if (textText == NULL) SDL_LogCritical(1,"An error occurred while generating the text: %s\n", SDL_GetError());
    int w, h;
    TTF_GetTextSize(textText, &w, &h);
    if (sizeX != 0) w = sizeX;
    if (sizeY != 0) h = sizeY;
    TTF_SetTextColor(textText, textColor->r, textColor->g, textColor->b, textColor->a);
    SDL_Texture *textTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, textTexture);
    bool resu = TTF_DrawRendererText(textText, sizeX, sizeY);
    if (resu == 0) SDL_LogCritical(1,"An error occurred while rendering the text: %s\n", SDL_GetError());
    SDL_SetRenderTarget(renderer, nullptr);
    TTF_DestroyRendererTextEngine(textEngine);
    TTF_DestroyText(textText);
    return textTexture;
}

// TODO: Remove this function, it is useless.
// signed int renderTextAtCoord(SDL_Renderer *renderer, const char *text, const int x, const int y, SDL_FColor *textColor, bool useCustomSize, SDL_Rect *customSizeRect) {
//     // SDL_Texture *textTexture = generateTextTexture(renderer, text, textColor);
//     SDL_Rect rectSize;
//     if (useCustomSize) {
//         rectSize.h = customSizeRect->h;
//         rectSize.w = customSizeRect->w;
//     } else if (!useCustomSize) {
//         SDL_QueryTexture(textTexture, NULL, NULL, &(rectSize.w), &(rectSize.h));
//     }
//     rectSize.x = x;
//     rectSize.y = y;
//     SDL_RenderCopy(renderer, textTexture, NULL, &rectSize);
//     SDL_DestroyTexture(textTexture);
//     return 0;
// }