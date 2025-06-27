/**
 * @file interface_handler.c
 * @brief This file manages all the interface and how they are being rendered.
 */

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
        perror("getcwd error");
        return 1;
    }
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

    printf("%s has a size of %lu\n", sceneFilePath, sceneFileSize);

    char *sceneContent = malloc((sizeof(char) * sceneFileSize) + 1);

    int sceneFileFile = open(sceneFilePath, O_RDONLY);

    if (read(sceneFileFile, sceneContent, sceneFileSize) == -1) {
        printf("Error\n");
    } else {
        printf("Think this is good\n");
    }

    cJSON *json = cJSON_Parse(sceneContent);

    printf("%s\n", cJSON_Print(json));

    cJSON_Delete(json);

    return 0;
}