#ifndef LIBWTK_SDL2_COPY_COMMAND_HPP
#define LIBWTK_SDL2_COPY_COMMAND_HPP

struct SDL_Texture;

// copy textures to the screen in a batch
struct copy_command
{
    SDL_Texture * texture;
    int x_offset;
    int y_offset;
};

#endif

