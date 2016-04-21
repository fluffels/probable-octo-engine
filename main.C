#include <iostream>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

#include "Cube.h"
#include "Shader.h"

using namespace std;

int main(int argc, char** argv) {
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0) {
        cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(argv[0], 0, 0, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        cerr << "Could not create window: " << SDL_GetError() << endl;
        return 2;
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        cerr << "Could not create OpenGL context: " << SDL_GetError() << endl;
        return 3;
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        cerr << "Could not initialize GLEW: " << glewGetErrorString(glewError) << endl;
        return 4;
    }

    GLenum glError = GL_NO_ERROR;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    auto shader_identity = Shader("identity");
    auto cube = Cube();

    float t = 0.0f;
    bool done = false;
    SDL_Event event;
    while (done == false) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    done = true;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.draw();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}