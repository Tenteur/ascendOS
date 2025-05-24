/**
 * @file render_manager.c
 * @brief This file manages the textures and surfaces to render on the screen
 */

#include <SDL2/SDL.h>
#include "../include/render_manager.h"

SDL_Renderer *globalRenderer = NULL;
int *nextItemIDAvailable = NULL;
struct itemDataNode *globalItemDataNode = NULL;
// typedef struct textureDataNode {
//     int sceneID;
//     int layer;
//     int itemID;
//     SDL_Texture *texturePtr;
//     SDL_Surface *surfacePtr;
//     int xCoord;
//     int yCoord;
//     struct textureDataNode *next;
// } textureDataNode;

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

int render_managerAddItemToDraw(const signed int sceneID, const signed int layer, SDL_Texture *texturePtr, SDL_Surface *surfacePtr, const int x, const int y) {
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
    newItemDataNode->next = currentNodeCheck->next;
    currentNodeCheck->next = newItemDataNode;
    return itemID;
}
