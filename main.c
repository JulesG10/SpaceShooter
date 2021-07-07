#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>
#include <SDL2/SDL.h>
#include "src/util.h"
#include "src/texture.h"

void update(float, UpdateInfo);
void draw(SDL_Renderer *);

int main(int argc, char **argv)
{

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        return 1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_CreateWindowAndRenderer(500, 500, 0, &window, &renderer) != 0)
    {
        return 1;
    }

    char *paths[4] = { "./out/assets/player/player-0.bmp", "./out/assets/player/player-1.bmp", "./out/assets/player/player-2.bmp", "./out/assets/player/player-3.bmp"};
    size_t len = 4; //sizeof(paths) / sizeof(char *);
    size_t alloc_size = sizeof(SDL_Texture *) * len;

    SDL_Texture **textures = (SDL_Texture **)malloc(alloc_size);

    memcpy(textures, load_texture(renderer, paths, len), alloc_size);

    Animation animation = create_animation(textures, (Vector2){0, 0}, (Size){50, 50}, 1, 4.0f, true, len);

    /*
    pthread_t update_thread;
    pthread_create(&update_thread, NULL, (void *)update, (void *)current_data);
    pthread_detach(update_thread);
    */

    Uint32 current_time;
    Uint32 last_time = SDL_GetTicks();
    float deltatime = 0.0f;

    time_t counter_start = time(0);
    time_t counter_current = counter_start;
    int frames = 0;

    bool running = true;
    while (running)
    {
        counter_current = time(0);
        frames++;
        if ((counter_current - counter_start) >= 1)
        {
            //printf("\rFPS: %d", frames);
            //fflush(stdout);
            frames = 0;
            counter_start = counter_current;
        }

        current_time = SDL_GetTicks();
        deltatime = current_time - last_time;
        deltatime /= 100;

        UpdateInfo updateInfo;
        updateInfo.mouse = NONE;
        updateInfo.key = SDLK_UNKNOWN;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                updateInfo.key = event.key.keysym.sym;
                break;
            case SDL_MOUSEMOTION:
                updateInfo.mouse = MOVE;

                break;
            case SDL_MOUSEBUTTONDOWN:
                updateInfo.mouse = RIGHT;
                break;
            case SDL_MOUSEWHEEL:
                if (event.button.y > 0)
                {
                    updateInfo.mouse = WHELL_UP;
                }
                else
                {
                    updateInfo.mouse = WHELL_DOWN;
                }
                break;
            }
            if (updateInfo.mouse != WHELL_UP && updateInfo.mouse != WHELL_DOWN && updateInfo.mouse != NONE)
            {
                updateInfo.mouse_position.x = event.button.x;
                updateInfo.mouse_position.y = event.button.y;
            }
        }

        update(deltatime, updateInfo);
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        //draw(renderer);
        play_animation(renderer, &animation, deltatime);
        SDL_RenderPresent(renderer);

        /*CPU fix*/
        SDL_Delay(1);
        last_time = current_time;
    }

    //pthread_join(update_thread, NULL);
    //free(textures);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


void draw(SDL_Renderer *renderer)
{
   
}

void update(float deltatime, UpdateInfo info)
{

}