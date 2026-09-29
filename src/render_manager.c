/**
 * @file render_manager.c
 * @brief This file manages the textures and surfaces to render on the screen
 */

#include <SDL3/SDL.h>
#include "../include/render_manager.h"

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

// @TODO Continue migrating to a unique tree instead of 3 separate trees !!!!!


static SDL_Renderer *globalRenderer = nullptr;
static unsigned int nextItemIDAvailable = 0;
static struct treeRoot *UITreeRoot = nullptr;

signed int render_managerInit(SDL_Renderer *renderer) {
    // @TODO: Init other components, might even init the SDL_Renderer but will be done after
    nextItemIDAvailable = 1;
    globalRenderer = renderer;
    UITreeRoot = malloc(sizeof(struct treeRoot));

    UITreeRoot->backgroundBranch = malloc(sizeof(struct itemDataNode));
    UITreeRoot->backgroundBranch->firstItem = nullptr;
    UITreeRoot->backgroundBranch->lastItem = nullptr;

    UITreeRoot->middlegroundBranch = malloc(sizeof(struct itemDataNode));
    UITreeRoot->middlegroundBranch->firstItem = nullptr;
    UITreeRoot->middlegroundBranch->lastItem = nullptr;

    UITreeRoot->foregroundBranch = malloc(sizeof(struct itemDataNode));
    UITreeRoot->foregroundBranch->firstItem = nullptr;
    UITreeRoot->foregroundBranch->lastItem = nullptr;

    return 0;
}

[[maybe_unused]] static unsigned int render_managerGetAvailableItemID([[maybe_unused]]unsigned int layer) {
    unsigned int IDToReturn = nextItemIDAvailable;
    nextItemIDAvailable += 1;
    if (IDToReturn == INT_MAX) {
        ; // @todo: search for the next available id
    }
    // *nextItemIDAvailable = IDToReturn + 1;
    return IDToReturn;
}

unsigned int render_managerAddItemToDraw(unsigned int layer, SDL_Texture *texturePtr, SDL_Surface *surfacePtr, unsigned int x, unsigned int y, unsigned int xSize, unsigned int ySize) {
    struct firstBranch *lastItemDataNode = nullptr;
    if (layer == 0) lastItemDataNode = UITreeRoot->backgroundBranch;
    else if (layer == 1) lastItemDataNode = UITreeRoot->middlegroundBranch;
    else if (layer == 2) lastItemDataNode = UITreeRoot->foregroundBranch;

    struct itemDataNode *newItemDataNode = malloc(sizeof(struct itemDataNode));

    if (lastItemDataNode->firstItem == nullptr) {
        lastItemDataNode->firstItem = newItemDataNode;
    } else {
        lastItemDataNode->lastItem->next = newItemDataNode;
    }
    lastItemDataNode->lastItem = newItemDataNode;

    newItemDataNode->texturePtr = texturePtr;
    newItemDataNode->surfacePtr = surfacePtr;
    newItemDataNode->xCoord = x;
    newItemDataNode->yCoord = y;
    newItemDataNode->xSize = xSize;
    newItemDataNode->ySize = ySize;

    return 0;
}

// @TODO: If errors occurred, return -1 (so of course, i need to check if errors occurred)
signed int render_managerDrawScene() {
    SDL_FRect currentRect;
    SDL_Texture *texturePtr = nullptr;
    struct itemDataNode *currentItemDataNode = nullptr;
    unsigned int currentLayerDrawn = 0;
    
    SDL_RenderClear(globalRenderer);
    // Find first item to draw in this order: background -> middleground -> foreground

    if (UITreeRoot->backgroundBranch->firstItem != nullptr) currentItemDataNode = UITreeRoot->backgroundBranch->firstItem;
    else if (UITreeRoot->middlegroundBranch->firstItem != nullptr) currentItemDataNode = UITreeRoot->middlegroundBranch->firstItem;
    else if (UITreeRoot->foregroundBranch->firstItem != nullptr) currentItemDataNode = UITreeRoot->foregroundBranch->firstItem;
    else {
        fprintf(stderr, "Error while rendering scene: NO ITEMS TO DRAW FOUND. Not fatal, breaking.\n");
        return -1;
    }


    while (true) {
        currentRect.x = (float)currentItemDataNode->xCoord;
        currentRect.y = (float)currentItemDataNode->yCoord;
        currentRect.w = (float)currentItemDataNode->xSize;
        currentRect.h = (float)currentItemDataNode->ySize;
        if (currentItemDataNode->texturePtr != NULL) {
            SDL_RenderTexture(globalRenderer, currentItemDataNode->texturePtr, nullptr, &currentRect);
        } else if (currentItemDataNode->surfacePtr != NULL){
            texturePtr = SDL_CreateTextureFromSurface(globalRenderer, currentItemDataNode->surfacePtr);
            SDL_RenderTexture(globalRenderer, texturePtr, nullptr, &currentRect);
        } else {
            fprintf(stderr, "Error while rendering scene: SURFACE AND TEXTURE POINTERS CANNOT BOTH BE EMPTY. Not fatal, skipping.\n");
        }
        if (currentItemDataNode->next != nullptr) {
            currentItemDataNode = currentItemDataNode->next;
        } else {
            currentLayerDrawn += 1;
            if (currentLayerDrawn == 1 && UITreeRoot->middlegroundBranch->firstItem != nullptr) currentItemDataNode = UITreeRoot->middlegroundBranch->firstItem;
            else if (currentLayerDrawn == 2 && UITreeRoot->foregroundBranch->firstItem != nullptr) currentItemDataNode = UITreeRoot->foregroundBranch->firstItem;
            else break;
        }
    }
    // SDL_RenderPresent(globalRenderer);
    return 0;
}

// TODO: Also check if errors occurred and do some more checks.
// signed int render_managerRemoveItem(unsigned int layer, unsigned int itemID) {
//     itemDataNode *currentItemDataNode = NULL;
//     if (layer == 0) { currentItemDataNode = backgroundItemDataNode; }
//     else if (layer == 1) { currentItemDataNode = middlegroundItemDataNode; }
//     else if (layer == 2) { currentItemDataNode = foregroundItemDataNode; };
//
//     struct itemDataNode *lastItemDataNode = NULL;
//     while(true) {
//         if (currentItemDataNode->itemID == itemID) {
//             if (currentItemDataNode->texturePtr != NULL) SDL_DestroyTexture(currentItemDataNode->texturePtr);
//             else SDL_DestroySurface(currentItemDataNode->surfacePtr);
//             // Cleaned the texture/surface from memory. Need to move the header
//             // if (currentItemDataNode == backgroundItemDataNode) { // @todo: Remove this condition because the item to remove cannot be the head of the NODE
//             //     backgroundItemDataNode = currentItemDataNode->next;
//             //     free(currentItemDataNode);
//             // }
//             lastItemDataNode->next = currentItemDataNode->next;
//             free(currentItemDataNode);
//             return 0;
//         } else if (currentItemDataNode->next != NULL) {
//             lastItemDataNode = currentItemDataNode;
//             currentItemDataNode = currentItemDataNode->next;
//         } else {
//             return -1;
//         }
//     }
// }

// TODO: This function may not be needed.
signed int render_managerFuseTextures(SDL_Renderer *renderer, SDL_Texture *outputTexture, SDL_Texture *firstInputTexture, SDL_Texture *secondInputTexture, SDL_FRect *firstInputCoord, SDL_FRect *secondInputCoord, SDL_FRect *firstInputSRect, SDL_FRect *secondInputSRect) {
    signed int hasError = 0b0;
    hasError = hasError | SDL_SetRenderTarget(renderer, outputTexture);
    if (hasError != 0) return -1;
    hasError = hasError | SDL_RenderTexture(renderer, firstInputTexture, firstInputSRect, firstInputCoord);
    hasError = hasError | SDL_RenderTexture(renderer, secondInputTexture, secondInputSRect, secondInputCoord);
    if (hasError != 0) {
        fprintf(stderr, "Error while fusing textures\n");
        return -1;
    }
    hasError = hasError ^ SDL_SetRenderTarget(renderer, nullptr);
    return hasError;
}

// TODO: Again, check if errors occurred...
signed int render_managerDeleteScene() {
    struct itemDataNode *itemDataNodeToClear = UITreeRoot->backgroundBranch->firstItem;
    if (itemDataNodeToClear != nullptr) {
        while(1) {
            if(itemDataNodeToClear->texturePtr != nullptr) SDL_DestroyTexture(itemDataNodeToClear->texturePtr);
            else if (itemDataNodeToClear->surfacePtr != nullptr) SDL_DestroySurface(itemDataNodeToClear->surfacePtr);
            else fprintf(stderr, "Error while destroying/freeing texture/surface in backgroundBranch: NOT FOUND. Not fatal, continuing.");
            struct itemDataNode *nextDataNode = itemDataNodeToClear->next;
            free(itemDataNodeToClear);
            if (itemDataNodeToClear == UITreeRoot->middlegroundBranch->lastItem) break;
            itemDataNodeToClear = nextDataNode;
        }
    }
    itemDataNodeToClear = UITreeRoot->middlegroundBranch->firstItem;
    if (itemDataNodeToClear != nullptr) {
        while(1) {
            if(itemDataNodeToClear->texturePtr != nullptr) SDL_DestroyTexture(itemDataNodeToClear->texturePtr);
            else if (itemDataNodeToClear->surfacePtr != nullptr) SDL_DestroySurface(itemDataNodeToClear->surfacePtr);
            else fprintf(stderr, "Error while destroying/freeing texture/surface in middlegroundBranch: NOT FOUND. Not fatal, continuing.");
            struct itemDataNode *nextDataNode = itemDataNodeToClear->next;
            free(itemDataNodeToClear);
            if (itemDataNodeToClear == UITreeRoot->middlegroundBranch->lastItem) break;
            itemDataNodeToClear = nextDataNode;
        }
    }
    itemDataNodeToClear = UITreeRoot->foregroundBranch->firstItem;
    if (itemDataNodeToClear != nullptr) {
        while(1) {
            if(itemDataNodeToClear->texturePtr != nullptr) SDL_DestroyTexture(itemDataNodeToClear->texturePtr);
            else if (itemDataNodeToClear->surfacePtr != nullptr) SDL_DestroySurface(itemDataNodeToClear->surfacePtr);
            else fprintf(stderr, "Error while destroying/freeing texture/surface in foregroundBranch: NOT FOUND. Not fatal, continuing.");
            struct itemDataNode *nextDataNode = itemDataNodeToClear->next;
            free(itemDataNodeToClear);
            if (itemDataNodeToClear == UITreeRoot->middlegroundBranch->lastItem) break;
            itemDataNodeToClear = nextDataNode;
        }
    }
    // register SDL_Renderer *renderer = globalRenderer;
    // free(globalRenderer);
    // render_managerInit(renderer);
    nextItemIDAvailable = 1;
    UITreeRoot->backgroundBranch  ->lastItem    = nullptr;
    UITreeRoot->backgroundBranch  ->firstItem   = nullptr;
    UITreeRoot->middlegroundBranch->lastItem    = nullptr;
    UITreeRoot->middlegroundBranch->firstItem   = nullptr;
    UITreeRoot->foregroundBranch  ->lastItem    = nullptr;
    UITreeRoot->foregroundBranch  ->firstItem   = nullptr;
    return 0;
}