#include <SDL2/SDL.h>
typedef struct __Platform__ {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} Platform;

void initPlatform(Platform *p, char const *title, int windowWidth,
    int windowHeight, int textureWidth, int textureHeight);
void destroyPlatform(Platform *p);
void updatePlatform(Platform *p, void const *buffer, int pitch);
int ProcessInput(Platform *p, uint8_t *keys);
