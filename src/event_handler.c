//
// Created by vianney on 20/04/2026.
//

#include "../include/event_handler.h"

#include <stdio.h>
#include <stdlib.h>

hitboxesList *hitboxListHead = nullptr;
hitboxesList *hoverboxListHead = nullptr;
eventIDList *eventsListHead = nullptr;
itemEventsList *itemEventListHead = nullptr;

signed int event_handlerInit() {
    hitboxListHead = malloc(sizeof(hitboxesList));
    hoverboxListHead = malloc(sizeof(hitboxesList));
    eventsListHead = malloc(sizeof(eventIDList));
    itemEventListHead = malloc(sizeof(itemEventsList));
    if (hitboxListHead == nullptr || eventsListHead == nullptr || itemEventListHead == nullptr || hoverboxListHead == nullptr) return -1;
    hitboxListHead->isDownEvent = 0;
    hitboxListHead->eventType = 0;
    hitboxListHead->x1 = 0;
    hitboxListHead->y1 = 0;
    hitboxListHead->x2 = 0;
    hitboxListHead->y2 = 0;
    hitboxListHead->itemLayer = 0;
    hitboxListHead->itemEvent = malloc(sizeof(itemEventsList));
    hitboxListHead->itemEvent->itemID = -1;
    hitboxListHead->itemEvent->extraData = nullptr;
    hitboxListHead->itemEvent->functionPtr = nullptr;
    hitboxListHead->itemEvent->next = nullptr;
    hitboxListHead->next = nullptr;
    hoverboxListHead->isDownEvent = 0;
    hoverboxListHead->eventType = 0;
    hoverboxListHead->x1 = 0;
    hoverboxListHead->y1 = 0;
    hoverboxListHead->x2 = 0;
    hoverboxListHead->y2 = 0;
    hoverboxListHead->itemLayer = 0;
    hoverboxListHead->itemEvent = malloc(sizeof(itemEventsList));
    hoverboxListHead->itemEvent->itemID = -1;
    hoverboxListHead->itemEvent->extraData = nullptr;
    hoverboxListHead->itemEvent->functionPtr = nullptr;
    hoverboxListHead->itemEvent->next = nullptr;
    hoverboxListHead->next = nullptr;
    eventsListHead->next = nullptr;
    itemEventListHead->next = nullptr;
    return 0;
    printf("interface_handler Initialized !\n");
}

/**
 *
 * @param functionName The name of the function declared (used to be identified).
 * @param functionPtr The pointer of the function to use.
 * @return 0 if success, -1 if failed.
 */

signed int declareEventFunction(char functionName[30], int (*functionPtr)(...));
/**
 *
 * @param itemID The ID of the item to declare the event handler.
 * @param eventType What type of event calls the function.
 * @param functionName What function to call after the event has been fired.
 * @return 0 if success, -1 if fail
 */
signed int declareItemEvent(int itemID, int eventType, char functionName[30]) {
    itemEventsList *newItemEventList = nullptr;
    newItemEventList = malloc(sizeof(itemEventsList));
    if (newItemEventList == nullptr) return -1;
    newItemEventList->next = itemEventListHead;
    itemEventListHead = newItemEventList;
    return 0;
}


// @TODO: Add the hitbox to the start of the list, no need to go through its entirety.
signed int declareItemBox(int isDownEvent, int eventType, int isHitboxDeclared, int itemID, int itemLayer, int x1, int y1, int x2, int y2) {
    hitboxesList *currentBoxList;
    if (isHitboxDeclared == 1) currentBoxList = hitboxListHead;
    else if (isHitboxDeclared == 0) currentBoxList = hoverboxListHead;
    else return -2;
    while (1) {
        if (currentBoxList->next == nullptr) {
            currentBoxList->isDownEvent = isDownEvent;
            currentBoxList->eventType = eventType;
            currentBoxList->x1 = x1;
            currentBoxList->y1 = y1;
            currentBoxList->x2 = x2;
            currentBoxList->y2 = y2;
            if (currentBoxList->itemEvent == nullptr) currentBoxList->itemEvent = malloc(sizeof(itemEventsList));
            if (currentBoxList->itemEvent == nullptr) return -1;
            currentBoxList->itemEvent->itemID = itemID;
            currentBoxList->itemEvent->extraData = nullptr;
            currentBoxList->itemEvent->functionPtr = nullptr;
            currentBoxList->itemEvent->next = nullptr;
            currentBoxList->itemLayer = itemLayer;
            currentBoxList->next = malloc(sizeof(hitboxesList));
            if (currentBoxList->next == nullptr) return -1;
            currentBoxList->next->isDownEvent = 0;
            currentBoxList->next->eventType = 0;
            currentBoxList->next->x1 = 0;
            currentBoxList->next->y1 = 0;
            currentBoxList->next->x2 = 0;
            currentBoxList->next->y2 = 0;
            currentBoxList->next->itemLayer = 0;
            currentBoxList->next->itemEvent = malloc(sizeof(itemEventsList));
            if (currentBoxList->next->itemEvent == nullptr) return -1;
            currentBoxList->next->itemEvent->itemID = -1;
            currentBoxList->next->itemEvent->extraData = nullptr;
            currentBoxList->next->itemEvent->functionPtr = nullptr;
            currentBoxList->next->itemEvent->next = nullptr;
            currentBoxList->next->next = nullptr;
            return 0;
        } else currentBoxList = currentBoxList->next;
    }
}

signed int checkItemBox(int x, int y, int isHitboxChecked, int isDownEvent, int eventType) {
    hitboxesList *currentBoxList;
    if (isHitboxChecked == 1) currentBoxList = hitboxListHead;
    else if (isHitboxChecked == 0) currentBoxList = hoverboxListHead;
    else return -2;
    hitboxesList *bestBoxMatch = nullptr;
    while (1) {
        if ((currentBoxList->x1 <= x) && (x <= currentBoxList->x2)) {
            if ((currentBoxList->y1 <= y) && (y <= currentBoxList->y2)) {
                if ((bestBoxMatch == nullptr) || (currentBoxList->itemLayer >= bestBoxMatch->itemLayer)) {
                    bestBoxMatch = currentBoxList;
                }
            }
        }
        if (currentBoxList->next == nullptr) break;
        currentBoxList = currentBoxList->next;
    }
    if (bestBoxMatch == nullptr) return -1;
    if (bestBoxMatch->eventType == eventType) {
        if (bestBoxMatch->isDownEvent == isDownEvent || isDownEvent == 2) {
            return bestBoxMatch->itemEvent->itemID;
        }
    }
    // if (bestBoxMatch->eventType == eventType && (bestBoxMatch->isDownEvent == isDownEvent || isDownEvent == 2)) return bestBoxMatch->itemID;
    return -2; // -2 == The isDownEvent is not right.
    // if (callFunctionEvent) {
    //     itemEventsList *currentItemEventList = itemEventListHead;
    //     while (1) {
    //         if (currentHitBoxList->itemID == bestHitboxMatch->itemID) {
    //
    //         }
    //     }
    // }
}
