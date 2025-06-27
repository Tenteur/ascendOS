/**
 * @file images.c
 * @brief images.c contains implementations of functions declared inside images.h
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/images.h"

signed int imagesInit() {
    printf("%u\n", IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG));
    return 0;
}

SDL_Texture *loadImageAsTexture(SDL_Renderer *renderer, const char *imagePath) {
    return IMG_LoadTexture(renderer, imagePath);
}
SDL_Surface *loadImageAsSurface(const char *imagePath) {
    return IMG_Load(imagePath);
}

signed int renderImage(SDL_Renderer *renderer, const char *imagePath, const int x, const int y) {
    SDL_Texture *imageTexture = loadImageAsTexture(renderer, imagePath);
    if (imageTexture == NULL) {
        printf("Failure!\n");
        return -1;
    }
    SDL_Rect tempRect;
    SDL_QueryTexture(imageTexture, NULL, NULL, &(tempRect.w), &(tempRect.h));
    tempRect.x = x;
    tempRect.y = y;
    SDL_RenderCopy(renderer, imageTexture, NULL, &tempRect);
    SDL_DestroyTexture(imageTexture);
    return 0;
}