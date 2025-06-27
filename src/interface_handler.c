/**
 * @file interface_handler.c
 * @brief This file manages all the interface and how they are being rendered.
 */

#include <SDL2/SDL.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "../include/images.h"
#include "../include/text.h"
#include "../include/shapes.h"
#include "../include/render_manager.h"
#include "../include/cJSON.h"

char currentWorkingDirectory[250];

int interface_handlerInit() {
    if (getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory)) == NULL) {
        perror("getcwd() error");
        return 1;
    }
    printf("interface_handler Initialized !\n");
    return 0;
}

int interface_handlerLoadScene(char sceneName[15]) {
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

    cJSON *json = cJSON_Parse(sceneContent);

    printf("%s\n", cJSON_Print(json));

    cJSON_Delete(json);

    return 0;
}

signed int render_managerFuseTextures(SDL_Renderer *renderer, SDL_Texture *outputTexture, SDL_Texture *firstInputTexture, SDL_Texture *secondInputTexture, SDL_Rect *firstInputCoord, SDL_Rect *secondInputCoord, SDL_Rect *firstInputSRect, SDL_Rect *secondInputSRect) {
    signed int hasError = 0b0;
    hasError = hasError ^ SDL_SetRenderTarget(renderer, outputTexture);
    if (hasError != 0) return -1;
    hasError = hasError ^ SDL_RenderCopy(renderer, firstInputTexture, firstInputSRect, firstInputCoord);
    hasError = hasError ^ SDL_RenderCopy(renderer, secondInputTexture, secondInputSRect, secondInputCoord);
    if (hasError != 0) {
        printf("had an error during texture fusion\n");
        return -1;
    }
    hasError = hasError ^ SDL_SetRenderTarget(renderer, NULL);
    return hasError;
}