#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const int DVD_WIDTH = 100;
const int DVD_HEIGHT = 44;

void animate(SDL_Renderer* renderer, SDL_Texture* dvdLogo) {
    // Initial position and velocity
    int x = rand() % (SCREEN_WIDTH - DVD_WIDTH);
    int y = rand() % (SCREEN_HEIGHT - DVD_HEIGHT);
    int dx = 2;
    int dy = 2;

    bool quit = false;
    SDL_Event e;

    // Initial background color
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Set background color
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderClear(renderer);

        // Render the DVD logo
        SDL_Rect dstRect = { x, y, DVD_WIDTH, DVD_HEIGHT };
        SDL_RenderCopy(renderer, dvdLogo, NULL, &dstRect);
        SDL_RenderPresent(renderer);

        // Update position of the DVD logo
        x += dx;
        y += dy;

        // Bounce off the wall and change to a random background color when it happens
        if (x + DVD_WIDTH > SCREEN_WIDTH || x < 0) {
            dx = -dx;
            r = rand() % 256;
            g = rand() % 256;
            b = rand() % 256;
        }
        if (y + DVD_HEIGHT > SCREEN_HEIGHT || y < 0) {
            dy = -dy;
            r = rand() % 256;
            g = rand() % 256;
            b = rand() % 256;
        }

        SDL_Delay(16);
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image for PNG support
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("DVD Logo Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load DVD logo
    SDL_Surface* logoSurface = IMG_Load("dvd-logo.png");
    if (logoSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture* dvdLogo = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);

    // Seed random number
    srand(time(NULL));

    // Start animation
    animate(renderer, dvdLogo);

    // Clean up and quit SDL_image
    IMG_Quit();

    // Clean up and quit SDL
    SDL_DestroyTexture(dvdLogo);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
