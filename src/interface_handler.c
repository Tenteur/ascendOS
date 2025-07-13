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
#include "../include/interface_handler.h"
#include "../include/cJSON.h"

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

    interface_handlerGenerateScene(json);

    cJSON_Delete(json);
    
    return 0;
}

signed int interface_handlerGenerateScene(cJSON *sceneTemplateJson) {
    // Check what is the type of the root element
    cJSON *rootType = cJSON_GetObjectItemCaseSensitive(sceneTemplateJson, "type");
    char *rootValue = cJSON_GetStringValue(rootType);
    printf("%s\n", rootValue);
    if (strcmp(rootValue, "group") == 0) {
        printf("some logic to develop later\n");
    }
    if (strcmp(rootValue, "text") == 0) {
        printf("Hellklekflklekflk\n");
        char *fontName = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(sceneTemplateJson, "fontName"));
        printf("hjkh\n");
        int fontSize = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(sceneTemplateJson, "fontSize"));
        printf("%s and %u\n", fontName, fontSize);
        printf("zqdsq\n");
        if (selectFontFromList(fontName, fontSize) == -1) {
            printf("skibidi\n");
            signed int result = addFontToList(fontName, fontSize);
            printf("toilet\n");
            if (result == 0) {
                printf("adding\n");
                SDL_Color SDLYellow = {255, 255, 0, 255};
                SDL_Texture *hiTexture = SDL_CreateTexture(interfaceRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 300, 200);
                SDL_SetRenderTarget(interfaceRenderer, hiTexture);
                signed int secondResult = renderTextAtCoord(interfaceRenderer, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(sceneTemplateJson, "textContent")), 0, 0, &SDLYellow, false, NULL);
                printf("This is the result: %d\n", secondResult);
                render_managerAddItemToDraw(1, 100, hiTexture, NULL, 100, 50, 300, 200);
                SDL_SetRenderTarget(interfaceRenderer, NULL);
            }
        }
    }
    return 0;
    
}