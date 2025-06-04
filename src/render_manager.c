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

static int render_managerGetAvailableItemID(int layer) {
    int IDToReturn = *nextItemIDAvailable;
    if (IDToReturn == INT_MAX) {
        ; // @todo: search for the next available id
    }
    return IDToReturn;
}

unsigned int render_managerAddItemToDraw(const signed int layer, const signed int sublayer, SDL_Texture *texturePtr, SDL_Surface *surfacePtr, const int x, const int y, const int width, const int height) {
    itemDataNode *currentNodeCheck = globalItemDataNode; // Head of the node.
    int itemID = render_managerGetAvailableItemID(layer);
    while(true) {
        if (currentNodeCheck->next == NULL) {
            break;
        } else if (currentNodeCheck->layer == layer && currentNodeCheck->layer > sublayer) {
            break;
        } else if (currentNodeCheck->layer == layer && currentNodeCheck->layer == sublayer && currentNodeCheck->itemID < itemID && currentNodeCheck->next->itemID > itemID) {
            break;
        }
    } // Out of the loop: got the node where to add this
    itemDataNode *newItemDataNode = malloc(sizeof(itemDataNode));
    newItemDataNode->layer = layer;
    newItemDataNode->sublayer = sublayer;
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
    SDL_Rect currentRect;
    itemDataNode *currentItemDataNode = globalItemDataNode->next; // Head of the node, contains no data so should not be drawn.
    while (true) {
        currentRect.x = currentItemDataNode->xCoord;
        currentRect.y = currentItemDataNode->yCoord;
        currentRect.w = currentItemDataNode->width;
        currentRect.h = currentItemDataNode->height;
        if (currentItemDataNode->texturePtr == NULL) {
            SDL_Texture *texturePtr = SDL_CreateTextureFromSurface(globalRenderer, currentItemDataNode->surfacePtr);
            SDL_RenderCopy(globalRenderer, texturePtr, NULL, &currentRect);
        } else {
            SDL_RenderCopy(globalRenderer, currentItemDataNode->texturePtr, NULL, &currentRect);
        }
        if (currentItemDataNode->next == NULL) {
            break;
        } else {
            currentItemDataNode = currentItemDataNode->next;
        }
    }
    // SDL_RenderPresent(globalRenderer);
    return 0;
}

signed int render_managerRemoveItem(int layer, int itemID, bool afterRender) {
    // @todo: add the afterRender parameter.
    itemDataNode *currentItemDataNode = globalItemDataNode;
    itemDataNode *lastItemDataNode = NULL;
    while(true) {
        if (currentItemDataNode->layer == layer && currentItemDataNode->itemID == itemID) {
            if (currentItemDataNode->texturePtr != NULL) SDL_DestroyTexture(currentItemDataNode->texturePtr);
            else SDL_FreeSurface(currentItemDataNode->surfacePtr);
            // Cleaned the texture/surface from memory. Need to move the header
            // if (currentItemDataNode == globalItemDataNode) { // @todo: Remove this condition because the item to remove cannot be the head of the NODE
            //     globalItemDataNode = currentItemDataNode->next;
            //     free(currentItemDataNode);
            // }
            lastItemDataNode->next = currentItemDataNode->next;
            free(currentItemDataNode);
            return 0;
        } else if (currentItemDataNode->next != NULL) {
            lastItemDataNode = currentItemDataNode;
            currentItemDataNode = currentItemDataNode->next;
        } else {
            return -1;
        }
    }
}
