#include <SDL2/SDL.h>
#include "Platform.h"
#include "CHIP-8.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <Scale> <Delay> <Rom>\n", *(argv + 0));
        exit(EXIT_FAILURE);
    }
    int videoScale = atoi(*(argv + 1));
    uint64_t cycleDelay = atoi(*(argv + 2));
    char const* rom = *(argv + 3);
    
    Platform platform;
    initPlatform(&platform, "CHIP-8 Emulator", VIDEO_WIDTH * videoScale,
        VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);
    CHIP8 chip8;
    initCHIP8(&chip8);
    loadGame(&chip8, rom);

    int videoPitch = sizeof(*(chip8.graphics + 0)) * VIDEO_WIDTH;
    double time_counter = 0;
    clock_t t, currT;
    t = clock();
    int quit = 0;
    while (!quit) {
        quit = ProcessInput(&platform, chip8.keypad);
        currT = clock();
        time_counter += (double)(currT - t);
        t = currT;
        if (time_counter > (double)(cycleDelay * CLOCKS_PER_SEC)) {
            printf("CYCLE RAN\n");
            Cycle(&chip8);
            updatePlatform(&platform, chip8.graphics, videoPitch);
            time_counter -= (double)(cycleDelay * CLOCKS_PER_SEC);
        }
    }
    destroyPlatform(&platform);
    return 0;
}
