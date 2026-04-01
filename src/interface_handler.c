/**
 * @file interface_handler.c
 * @brief This file manages all the interface and how they are being rendered.
 */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <yaml.h>
#include <stdlib.h>

#include "../include/images.h"
#include "../include/text.h"
#include "../include/shapes.h"
#include "../include/render_manager.h"
#include "../include/interface_handler.h"

char currentWorkingDirectory[250];
char *defaultFont = "Roboto-Regular";
SDL_Renderer *interfaceRenderer;

int interface_handlerInit(SDL_Renderer *renderer) {
    if (getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory)) == NULL) {
        perror("getcwd error");
        return 1;
    }
    printf("interface_handler Initialized !\n");
    interfaceRenderer = renderer;
    return 0;
}


int parse_yaml(const char *filename) {
    printf("file path: %s\n", filename);
    FILE *file = fopen(filename, "rb");
    yaml_parser_t parser;
    yaml_event_t event;
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, file);
    bool done = 0;
    // unsigned int numberOfMappings = 0;
    // unsigned int numberOfSequence = 0;

    while (!done) {
        yaml_parser_parse(&parser, &event);

        switch(event.type){
            case YAML_SCALAR_EVENT:
                if (strcmp((char *)event.data.scalar.value, "type") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (strcmp((char *)event.data.scalar.value, "rect") == 0) {
                        parse_rect(parser, event);
                    } else if (strcmp((char *)event.data.scalar.value, "text") == 0) {
                        parse_text(parser, event);
                    }
                }
                // printf("Scalar (%s)\n", event.data.scalar.value);
                break;
            // case YAML_MAPPING_START_EVENT:
            //     printf("Start of a mapping\n");
            //     break;
            // case YAML_NO_EVENT:
            //     printf("There is no events");
            //     break;
            case YAML_STREAM_END_EVENT:
                printf("stream end event \n");
                done = 1;
                break;
            // case YAML_STREAM_START_EVENT:
            //     printf("stream start event \n");
            //     break;
            // case YAML_DOCUMENT_START_EVENT:
            //     printf("document start event \n");
            //     break;
            // case YAML_DOCUMENT_END_EVENT:
            //     printf("document end event \n");
            //     break;
            // case YAML_ALIAS_EVENT:
            //     printf("alias event \n");
            //     break;
            // case YAML_SEQUENCE_START_EVENT:
            //     printf("sequence start event \n");
            //     break;
            // case YAML_SEQUENCE_END_EVENT:
            //     printf("sequence end event \n");
            //     break;
            // case YAML_MAPPING_END_EVENT:
            //     printf("mapping end event \n");
            //     break;
            default:
                break;
        }
        yaml_event_delete(&event);
    };
    
    printf("finished it\n");
    yaml_parser_delete(&parser);
    fclose(file);
    return 0;
}

unsigned int conv(const yaml_char_t *value) {
    return atoi((const char *)value);
}


int parse_rect(yaml_parser_t parser, yaml_event_t event) {    
    unsigned int posX = 0, posY = 0, sizeX = 0, sizeY = 0, colorR = 0, colorG = 0, colorB = 0;
    while(1) {
        yaml_parser_parse(&parser, &event);
        if (event.type == YAML_MAPPING_END_EVENT) {
            break;
        } else if(strcmp((char*)event.data.scalar.value, "position") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SEQUENCE_START_EVENT) {
                return -1;
            }
            yaml_parser_parse(&parser, &event);
            posX = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event);
            posY = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        } else if (strcmp((char*)event.data.scalar.value, "size") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SEQUENCE_START_EVENT) return -1;
            yaml_parser_parse(&parser, &event);
            sizeX = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event);
            sizeY = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        } else if (strcmp((char*)event.data.scalar.value, "color") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SEQUENCE_START_EVENT) return -1;
            yaml_parser_parse(&parser, &event);
            colorR = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event);
            colorG = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event);
            colorB = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        }
    }
    SDL_Texture *rectTexture = SDL_CreateTexture(interfaceRenderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_TARGET, sizeX, sizeY);
    SDL_SetRenderTarget(interfaceRenderer, rectTexture);
    SDL_FColor rectColor = {colorR, colorG, colorB, 255};
    signed int result = drawRectAtCoord(interfaceRenderer, 0, 0, sizeX, sizeY, rectColor);
    SDL_SetRenderTarget(interfaceRenderer, NULL);
    unsigned int rectItemID = render_managerAddItemToDraw(1, 0, rectTexture, NULL, posX, posY, sizeX, sizeY);
    return 0;
}

int parse_text(yaml_parser_t parser, yaml_event_t event) {
    unsigned int posX = 0, posY = 0, fontSize = 0, sizeX = 0, sizeY = 0;
    unsigned int colorR = 0, colorG = 0, colorB = 0;
    char *textToRender, *fontFamily, *text;
    while (1) {
        yaml_parser_parse(&parser, &event);
        if (event.type == YAML_MAPPING_END_EVENT) {
            break;
        } else if (strcmp((char *) event.data.scalar.value, "position") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SEQUENCE_START_EVENT) {
                return -1;
            }
            yaml_parser_parse(&parser, &event);
            posX = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event);
            posY = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        } else if (strcmp((char *) event.data.scalar.value, "fontSize") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SCALAR_EVENT) return -1;
            fontSize = conv(event.data.scalar.value);
            // yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        } else if (strcmp((char *) event.data.scalar.value, "textColor") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SEQUENCE_START_EVENT) return -1;
            yaml_parser_parse(&parser, &event);
            colorR = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event);
            colorG = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event);
            colorB = conv(event.data.scalar.value);
            yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        } else if (strcmp((char*)event.data.scalar.value, "fontFamily") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SCALAR_EVENT) return -1;
            fontFamily = (char*)event.data.scalar.value;
            // yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        } else if (strcmp((char *) event.data.scalar.value, "textContent") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SCALAR_EVENT) return -1;
            text = (char *) event.data.scalar.value;
            // yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        } else if (strcmp((char *) event.data.scalar.value, "size") == 0) {
            yaml_parser_parse(&parser, &event);
            if (event.type != YAML_SCALAR_EVENT) return -1;
            if (strcmp((char *) event.data.scalar.value, "auto") == 0) {
                sizeX = 0;
                sizeY = 0;
            }
            // else {
            //     size = event.data.scalar.value;
            // }
            // yaml_parser_parse(&parser, &event); // Needed, else it SegFault
        }
    }
    SDL_FColor textColor = {colorR, colorG, colorB, 255};
    if (selectFontFromList(fontFamily, fontSize) == -1) addFontToList(fontFamily, fontSize);
    SDL_Texture *textTexture = generateTextTexture(interfaceRenderer, text, &textColor, sizeX, sizeY);
    printf("text texture: %p", textTexture);
    // SDL_GetTextureSize()
    unsigned int textItemID = render_managerAddItemToDraw(1, 0, textTexture, NULL, posX, posY, 300, 300);
}

int interface_handlerGenerateScene() {
    return 0;
}
