#include <iostream>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Cube.h"
#include "Origin.h"
#include "Shader.h"
#include "ShaderManager.h"

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

    glEnable(GL_DEPTH);
    glEnable(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    auto shaders = ShaderManager();

    auto shader_transform = shaders.get("transform");
    shader_transform->apply();
    auto cube = Cube();

    auto shader_colour = shaders.get("colour");
    shader_colour->apply();
    auto origin = Origin();

    auto view = glm::lookAt(glm::vec3(0, 0, 5.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    shaders.updateViewMatrices(view);

    auto proj = glm::perspective(45.f, 640.f / 480.f, 0.1f, -100.f);
    shaders.updateProjectionMatrices(proj);

    float angle = 0.0f;
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

        auto modelQuat = glm::quat();
        modelQuat = glm::rotate(modelQuat, angle, glm::vec3(0, 1, 0));
        auto modelMat = (glm::mat4)modelQuat;
        shaders.updateWorldMatrices((glm::mat4)modelMat);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_colour->apply();
        origin.draw();

        glFlush();
        SDL_GL_SwapWindow(window);

        angle += 0.01f;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}