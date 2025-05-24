/**
 * @file render_manager.c
 * @brief This file manages the textures and surfaces to render on the screen
 */

#include <SDL2/SDL.h>
#include "../include/render_manager.h"

SDL_Renderer *globalRenderer = NULL;
int *nextItemIDAvailable = NULL;
struct itemDataNode *globalItemDataNode = NULL;

signed int render_managerInit(SDL_Renderer *renderer) {
    // @todo: Init other components, might even init the SDL_Renderer but will be done after
    nextItemIDAvailable = malloc(sizeof(int));
    *nextItemIDAvailable = 1;
    globalRenderer = malloc(sizeof(renderer));
    globalRenderer = renderer;
    globalItemDataNode = malloc(sizeof(itemDataNode));
    globalItemDataNode->next = NULL; // The first textureDataNode will contain no data, used only as the head of the node.
    return 0;
}

static int render_managerGetAvailableItemID(int sceneID) {
    int IDToReturn = *nextItemIDAvailable;
    if (IDToReturn == INT_MAX) {
        ; // @todo: search for the next available id
    }
    return IDToReturn;
}

int render_managerAddItemToDraw(const signed int sceneID, const signed int layer, SDL_Texture *texturePtr, SDL_Surface *surfacePtr, const int x, const int y, const int width, const int height) {
    itemDataNode *currentNodeCheck = globalItemDataNode; // Head of the node.
    bool conditionsNotMet = true;
    int itemID = render_managerGetAvailableItemID(sceneID);
    while(conditionsNotMet) {
        if (currentNodeCheck->next == NULL) {
            conditionsNotMet = false;
        } else if (currentNodeCheck->sceneID == sceneID && currentNodeCheck->layer > layer) {
            conditionsNotMet = false;
        } else if (currentNodeCheck->sceneID == sceneID && currentNodeCheck->layer == layer && currentNodeCheck->itemID < itemID && currentNodeCheck->next->itemID > itemID) {
            conditionsNotMet = false;
        }
    } // Out of the loop: got the node where to add this, 
    itemDataNode *newItemDataNode = malloc(sizeof(itemDataNode));
    newItemDataNode->sceneID = sceneID;
    newItemDataNode->layer = layer;
    newItemDataNode->itemID = itemID;
    newItemDataNode->texturePtr = texturePtr;
    newItemDataNode->surfacePtr = surfacePtr;
    newItemDataNode->xCoord = x;
    newItemDataNode->yCoord = y;
    newItemDataNode->width = width;
    newItemDataNode->height = height;
    newItemDataNode->next = currentNodeCheck->next;
    currentNodeCheck->next = newItemDataNode;
    return itemID;
}

signed int render_managerDrawScene() {
    SDL_Rect *currentRect;
    itemDataNode *currentItemDataNode = globalItemDataNode; // Head of the node.
    while (true) {
        currentRect->x = currentItemDataNode->xCoord;
        currentRect->y = currentItemDataNode->yCoord;
        currentRect->w = currentItemDataNode->width;
        currentRect->h = currentItemDataNode->height;
        if (currentItemDataNode->texturePtr == NULL) {
            SDL_Texture *texturePtr = SDL_CreateTextureFromSurface(globalRenderer, currentItemDataNode->surfacePtr);
            SDL_RenderCopy(globalRenderer, texturePtr, NULL, currentRect);
        } else {
            SDL_RenderCopy(globalRenderer, currentItemDataNode->texturePtr, NULL, currentRect);
        }
        if (currentItemDataNode->next == NULL) {
            break;
        } else {
            currentItemDataNode = currentItemDataNode->next;
        }
    }
    SDL_RenderPresent(globalRenderer);
    return 0;
}
