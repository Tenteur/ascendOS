/**
 * @file interface_handler.c
 * @brief This file manages all the interface and how they are being rendered.
 */

#include <SDL2/SDL.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "../include/images.h"
#include "../include/text.h"
#include "../include/shapes.h"
#include "../include/render_manager.h"
#include "../include/interface_handler.h"

char currentWorkingDirectory[250];
SDL_Renderer *interfaceRenderer;

int interface_handlerInit(SDL_Renderer *renderer) {
    if (getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory)) == NULL) {
        perror("getcwd() error");
        return 1;
    }
    printf("interface_handler Initialized !\n");
    interfaceRenderer = renderer;
    return 0;
}

xmlDoc *interface_handlerLoadScene(char sceneName[15]) {
    char sceneFilePath[300];
    char *slash = "/templates/";
    strcpy(sceneFilePath, currentWorkingDirectory);
    strcat(sceneFilePath, slash);
    strcat(sceneFilePath, sceneName);

    FILE *sceneFile = fopen(sceneFilePath, "r");
    fseek(sceneFile, 0L, SEEK_END);
    long sceneFileSize = ftell(sceneFile);
    fseek(sceneFile, 0L, SEEK_SET);
    fclose(sceneFile);

    char *sceneContent = malloc((sizeof(char) * sceneFileSize) + 1);

    int sceneFileFile = open(sceneFilePath, O_RDONLY);

    if (read(sceneFileFile, sceneContent, sceneFileSize) == -1) {
        printf("Error\n");
    }
    
    xmlDoc *document = xmlReadFile(sceneFilePath, NULL, 0);
    if (!document) return NULL;

    // interface_handlerGenerateScene(json);
    
    return document;
}

signed int interface_handlerGenerateScene(xmlDoc *document) {
    
    return 0;   
}