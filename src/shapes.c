/**
 * @file shapes.c
 * @brief shapes.c contains implementations of functions declared inside shapes.h
 */


#include <SDL2/SDL.h>
#include "../include/shapes.h"


signed int drawCircleAtCoord(SDL_Renderer *renderer, int x, int y, const float radius, const int precision, const SDL_Color color) {
	const float PI = 3.14;
    const float increment = 2.0f * PI / precision; // 2 PI Rad / precision (higher, lower increment so higher precision but slower)
    SDL_Vertex vertices[precision + 2]; // Number of vertices

    x = x + radius;
    y = y + radius;
    
    // Circle center
    vertices[0].position.x = x;
    vertices[0].position.y = y;
    vertices[0].color = color;
    vertices[0].tex_coord.x = 0;
    vertices[0].tex_coord.y = 0;

    for (int i = 0; i <= precision; i++) {
        float angle = i * increment;
        vertices[i + 1].position.x = roundf(x + radius * cosf(angle));
        vertices[i + 1].position.y = roundf(y + radius * sinf(angle));
        vertices[i + 1].color = color;
        vertices[i + 1].tex_coord.x = 0;
        vertices[i + 1].tex_coord.y = 0;
    }

    // Chaque triangle est (centre, i, i+1)
    for (int i = 1; i <= precision; i++) {
        SDL_Vertex triangle[3] = { vertices[0], vertices[i], vertices[i + 1] };
        if (SDL_RenderGeometry(renderer, NULL, triangle, 3, NULL, 0) < 0) {
            return -1;
        }
    }
    return 0;
}

signed int drawSquareAtCoord(SDL_Renderer *renderer, const int x, const int y, const int size, const SDL_Color color) {
    SDL_Vertex vertexList[6];

    vertexList[0].position.x = x;
    vertexList[0].position.y = y;
    vertexList[0].color = color;
    vertexList[0].tex_coord.x = 0;
    vertexList[0].tex_coord.y = 0;

    vertexList[1].position.x = x + size;
    vertexList[1].position.y = y;
    vertexList[1].color = color;
    vertexList[1].tex_coord.x = 0;
    vertexList[1].tex_coord.y = 0;

    vertexList[2].position.x = x + size;
    vertexList[2].position.y = y + size;
    vertexList[2].color = color;
    vertexList[2].tex_coord.x = 0;
    vertexList[2].tex_coord.y = 0;

    vertexList[3] = vertexList[0];
    vertexList[4] = vertexList[2];

    vertexList[5].position.x = x;
    vertexList[5].position.y = y + size;
    vertexList[5].color = color;
    vertexList[5].tex_coord.x = 0;
    vertexList[5].tex_coord.y = 0;
    
    const int result = SDL_RenderGeometry(renderer, NULL, vertexList, 6, NULL, 0);
    if (result != 0) {
        return -1;
    } else {
        return 0;
    }
}

signed int drawRectAtCoord(SDL_Renderer *renderer, const int x, const int y, const int xsize, const int ysize, const SDL_Color color) {
    SDL_Vertex vertexList[6];

    vertexList[0].position.x = x;
    vertexList[0].position.y = y;
    vertexList[0].color = color;
    vertexList[0].tex_coord.x = 0;
    vertexList[0].tex_coord.y = 0;

    vertexList[1].position.x = x + xsize;
    vertexList[1].position.y = y;
    vertexList[1].color = color;
    vertexList[1].tex_coord.x = 0;
    vertexList[1].tex_coord.y = 0;

    vertexList[2].position.x = x + xsize;
    vertexList[2].position.y = y + ysize;
    vertexList[2].color = color;
    vertexList[2].tex_coord.x = 0;
    vertexList[2].tex_coord.y = 0;

    vertexList[3] = vertexList[0];
    vertexList[4] = vertexList[2];

    vertexList[5].position.x = x;
    vertexList[5].position.y = y + ysize;
    vertexList[5].color = color;
    vertexList[5].tex_coord.x = 0;
    vertexList[5].tex_coord.y = 0;
    
    const int result = SDL_RenderGeometry(renderer, NULL, vertexList, 6, NULL, 0);
    if (result != 0) {
        return -1;
    } else {
        return 0;
    }
}
