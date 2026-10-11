//
// Created by vianney on 08/10/2026.
//

#include "../include/logging_handler.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static struct logHandleList *logHandleListHead = NULL;

bool initLogging_handler(void) {
    logHandleListHead = malloc(sizeof(struct logHandleList));
    if (logHandleListHead == NULL) fprintf(stderr, "Failed to initialize the LOGGING_HANDLER: malloc failed and returned NULL");
    else return 0;
    logHandleListHead->next = NULL;
    logHandleListHead->logHandleID = 0;
    return 1;
}

void addLogHandle(unsigned int logHandleID, enum logTypeHandle logCategory) {
    struct logHandleList *currentLogHandle = logHandleListHead;
    while(true) {
        if (currentLogHandle->next == NULL) break;
        else if (currentLogHandle->logHandleID < logHandleID && currentLogHandle->next->logHandleID > logHandleID) break;
        else {
            currentLogHandle = currentLogHandle->next;
        }
    }
    if (currentLogHandle->logHandleID >= logHandleID) return;
    else {
        struct logHandleList *newLogHandle = malloc(sizeof(struct logHandleList));
        newLogHandle->logHandleID = logHandleID;
        newLogHandle->next = NULL;
        newLogHandle->isShown = true;
        newLogHandle->logCategory = logCategory;
        switch (logCategory) {
            case (LOGH_INFO):
                strcpy(newLogHandle->textProperties, "37;40;3\0");
                break;
            case (LOGH_WARNING):
                strcpy(newLogHandle->textProperties, "33;40;4\0");
                break;
            case (LOGH_ERROR):
                strcpy(newLogHandle->textProperties, "31;107;1\0");
                break;
            case (LOGH_CRITICAL):
                strcpy(newLogHandle->textProperties, "91;41;1;4\0");
                break;
            default:
                strcpy(newLogHandle->textProperties, "0\0");
                break;
        }
        
    }
}

void changeLogTextProperties(unsigned int logHandleID, char *newTextProperties) {
    return;
}

void writeNewLog(unsigned int logHandleID, char *stringToLog) {
    struct logHandleList *currentLogHandle = logHandleListHead;
    while (currentLogHandle->logHandleID != logHandleID) {
        if (currentLogHandle->next == NULL) return;
        currentLogHandle = currentLogHandle->next;
    }
    if (currentLogHandle->isShown == true) {
        printf("\033[%s%sm", currentLogHandle->textProperties, stringToLog);
    }
    return;
}