//
// Created by vianney on 12/04/2026.
//
#pragma once

#include <stdarg.h>
#include <stdlib.h>

#ifndef ASCENDOS_EVENT_HANDLER_H
#define ASCENDOS_EVENT_HANDLER_H

#endif //ASCENDOS_EVENT_HANDLER_H

/**
 * @brief All defined event types.
 * @todo Add system events
 */
enum eventTypes {
    HOVER,
    LEFTCLICK,
    MIDDLECLICK,
    RIGHTCLICK,
    TYPING,
    MANUALTRIGGER
};

typedef struct hitboxesList {
    int isDownEvent;
    int eventType;
    int x1;
    int y1;
    int x2;
    int y2;
    int itemLayer;
    struct itemEventsList *itemEvent;
    struct hitboxesList *next;
} hitboxesList;

/**
 * @brief Support for extra data for events.
 * @todo extraData (as a unsigned char[100]) but it is a struct list so we can add as many as we want with the data type we want and it is to the reader_function to determine how to use this data. The extraData is put when the event_parser don't know how to use data or where to put it (or if manually declared). The parser will manage putting the extraData in memory and cut it if extraData size is too big ( >100 ).
 */
struct extraData {
    // struct extraData *next;
    size_t dataLength;
    void *data;
};

typedef struct eventIDList {
    char functionName[30];
    int *functionPtr;
    struct eventIDList *next;
} eventIDList;

typedef struct itemEventsList {
    int itemID;
    struct extraData *extraData;
    int (*functionPtr)(...);
    struct itemEventsList *next;
} itemEventsList;


/**
 * @brief Initialize everything needed for handling events.
 * @return 0 if success, -1 if failed.
 */
signed int event_handlerInit();

/**
 * @brief declare a function that can be used when events fire.
 * @param functionName The name of the function declared (used to be identified).
 * @param functionPtr The pointer of the function to use.
 * @return 0 if success, -1 if failed.
 */
signed int declareEventFunction(char functionName[30], int (*functionPtr)(...));

/**
 * @brief delcare an event for an item.
 * @note Not used for now i think
 * @param itemID The ID of the item to declare the event handler.
 * @param eventType What type of event calls the function.
 * @param functionName What function to call after the event has been fired.
 * @return 0 if success, -1 if fail
 */
signed int declareItemEvent(int itemID, int eventType, char functionName[30]);

/**
 * @brief Simply declares an hitbox for an item based on parameters.
 * @param isDownEvent Is the event declaration only for up or down events (0 = is for up, 1 for down, 2 for all).
 * @param eventType TODO
 * @param isHitboxDeclared TODO
 * @param itemID The ID of the item to declare hitbox.
 * @param itemLayer The layer of the item (higher will have priority towards bottom ones)
 * @param x1 the top right coord of the hitbox.
 * @param y1 The top left coord of the hitbox.
 * @param x2 The bottom right coord of the hitbox.
 * @param y2 The bottom left coord of the hitbox.
 * @return A hitboxID > 1 if success, -1 or 0 based on type op failure.
 */
signed int declareItemBox(int isDownEvent, int eventType, int isHitboxDeclared, int itemID, int itemLayer, int x1, int y1, int x2, int y2);

/**
 * @brief Simply check for the hitbox based on X Y coords.
 * @param x The x coord to check for hitbox
 * @param y The y coord to check for hitbox
 * @param isHitboxChecked TODO
 * @param isDownEvent Is the event check only for up or down events.
 * @param eventType The type of the event.
 * @param callFunctionEvent 0 If should call the function of the item that got hit.
 * @return -1 if no hitbox were hit, -2 if hitbox found but could not fire event (isDownEvent), else, return the itemID.
 */
signed int checkItemBox(int x, int y, int isHitboxChecked, int isDownEvent, int eventType);