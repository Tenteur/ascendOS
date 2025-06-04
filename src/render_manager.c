/**
 * @file render_manager.c
 * @brief This file manages the textures and surfaces to render on the screen
 */

#include <SDL2/SDL.h>
#include "../include/render_manager.h"

SDL_Renderer *globalRenderer = NULL;
int *nextItemIDAvailable = NULL;
struct itemDataNode *backgroundItemDataNode = NULL;
struct itemDataNode *middlegroundItemDataNode = NULL;
struct itemDataNode *foregroundItemDataNode = NULL;
// @note These three are the heads of the nodes. They contain no data except for *next

signed int render_managerInit(SDL_Renderer *renderer) {
    // @todo: Init other components, might even init the SDL_Renderer but will be done after
    nextItemIDAvailable = malloc(sizeof(int));
    *nextItemIDAvailable = 1;
    globalRenderer = malloc(sizeof(renderer));
    globalRenderer = renderer;
    backgroundItemDataNode = malloc(sizeof(itemDataNode));
    backgroundItemDataNode->next = NULL; // The first textureDataNode will contain no data, used only as the head of the node.
    middlegroundItemDataNode = malloc(sizeof(itemDataNode));
    middlegroundItemDataNode->next = NULL; // The first textureDataNode will contain no data, used only as the head of the node.
    foregroundItemDataNode = malloc(sizeof(itemDataNode));
    foregroundItemDataNode->next = NULL; // The first textureDataNode will contain no data, used only as the head of the node.
    return 0;
}

static int render_managerGetAvailableItemID(int layer) {
    int IDToReturn = *nextItemIDAvailable;
    if (IDToReturn == INT_MAX) {
        ; // @todo: search for the next available id
    }
    // *nextItemIDAvailable = IDToReturn + 1;
    return IDToReturn;
}

unsigned int render_managerAddItemToDraw(const unsigned int layer, const signed int sublayer, SDL_Texture *texturePtr, SDL_Surface *surfacePtr, const int x, const int y, const int width, const int height) {
    itemDataNode *currentNodeCheck = NULL;
    if (layer == 0) { currentNodeCheck = backgroundItemDataNode; }
    else if (layer == 1) { currentNodeCheck = middlegroundItemDataNode; }
    else if (layer == 2) { currentNodeCheck = foregroundItemDataNode; }
    int itemID = render_managerGetAvailableItemID(layer);
    while(true) {
        if (currentNodeCheck->next == NULL) {
            break;
        } else if (currentNodeCheck->sublayer > sublayer) {
            break;
        } else if (currentNodeCheck->sublayer == sublayer && currentNodeCheck->itemID < itemID && currentNodeCheck->next->itemID > itemID) {
            break;
        }
    } // Out of the loop: got the node where to add this
    itemDataNode *newItemDataNode = malloc(sizeof(itemDataNode));
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
    itemDataNode *currentItemDataNode = NULL;
    unsigned int currentLayerDrawn;
    if (backgroundItemDataNode->next == NULL) {
        if (middlegroundItemDataNode->next == NULL) {
            if (foregroundItemDataNode->next == NULL) {
                return 0;
            } else {
                currentItemDataNode = foregroundItemDataNode->next;
                currentLayerDrawn = 2;
            }
        } else {
            currentItemDataNode = middlegroundItemDataNode->next;
            currentLayerDrawn = 1;
        }
    } else {
        currentItemDataNode = backgroundItemDataNode->next;
        currentLayerDrawn = 0;
    }

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
            currentLayerDrawn += 1;
            if (currentLayerDrawn == 1) currentItemDataNode = middlegroundItemDataNode->next;
            else if (currentLayerDrawn == 2) currentItemDataNode = foregroundItemDataNode->next;
            else break;
        } else {
            currentItemDataNode = currentItemDataNode->next;
        }
    }
    // SDL_RenderPresent(globalRenderer);
    return 0;
}

signed int render_managerRemoveItem(int layer, int itemID, bool afterRender) {
    // @todo: add the afterRender parameter.
    itemDataNode *currentItemDataNode = NULL;
    if (layer == 0) { currentItemDataNode = backgroundItemDataNode; }
    else if (layer == 1) { currentItemDataNode = middlegroundItemDataNode; }
    else if (layer == 2) { currentItemDataNode = foregroundItemDataNode; };

    itemDataNode *lastItemDataNode = NULL;
    while(true) {
        if (currentItemDataNode->itemID == itemID) {
            if (currentItemDataNode->texturePtr != NULL) SDL_DestroyTexture(currentItemDataNode->texturePtr);
            else SDL_FreeSurface(currentItemDataNode->surfacePtr);
            // Cleaned the texture/surface from memory. Need to move the header
            // if (currentItemDataNode == backgroundItemDataNode) { // @todo: Remove this condition because the item to remove cannot be the head of the NODE
            //     backgroundItemDataNode = currentItemDataNode->next;
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
