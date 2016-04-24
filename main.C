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
using glm::vec3;
using glm::vec4;

const float STEP = 0.1f;
const float ANGLE_DELTA = 3.14f;

float window_width = 640.0f;
float window_height = 480.0f;

int main(int argc, char** argv) {
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0) {
        cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(argv[0], 0, 0, (int)window_width,
                                          (int)window_height,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        cerr << "Could not create window: " << SDL_GetError() << endl;
        return 2;
    }

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

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

    auto eye = glm::vec3(0, 0, 5.0f);
    auto at = glm::vec3(0, 0, 0);
    auto up = glm::vec3(0, 1, 0);
    auto view = glm::lookAt(eye, at, up);
    shaders.updateViewMatrices(view);

    auto proj = glm::perspective(45.f, window_width / window_height, 0.1f,
                                 -100.f);
    shaders.updateProjectionMatrices(proj);

    float angle = 0.0f;
    bool done = false;
    SDL_Event event;
    while (done == false) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        cout << "[INFO] Exiting normally at user request." << endl;
                        done = true;
                        break;
                    default:
                        break;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                cout << event.motion.xrel << " " << event.motion.yrel << endl;

                const GLfloat X_SCALED = -(GLfloat)event.motion.xrel / window_width;
                const GLfloat Y_SCALED = -(GLfloat)event.motion.yrel / window_height;

                const GLfloat LEFT_RIGHT_ROT = X_SCALED * ANGLE_DELTA;
                const GLfloat UP_DOWN_ROT = Y_SCALED * ANGLE_DELTA;

                vec3 tempD(at - eye);
                vec4 d(tempD.x, tempD.y, tempD.z, 0.0f);

                vec3 right = cross(tempD, up);

                mat4 rot;
                rot = rotate(rot, UP_DOWN_ROT, right);
                rot = rotate(rot, LEFT_RIGHT_ROT, up);

                d = rot * d;

                at.x = eye.x + d.x;
                at.y = eye.y + d.y;
                at.z = eye.z + d.z;

            }
        }

        glm::vec3 direction = STEP * glm::normalize(at - eye);
        glm::vec3 right = STEP * glm::normalize(glm::cross(direction, up));

        auto keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W]) {
            eye += direction;
            at += direction;
        }
        if (keys[SDL_SCANCODE_S]) {
            eye -= direction;
            at -= direction;
        }
        if (keys[SDL_SCANCODE_D]) {
            eye += right;
            at += right;
        }
        if (keys[SDL_SCANCODE_A]) {
            eye -= right;
            at -= right;
        }
        if (keys[SDL_SCANCODE_SPACE]) {
            eye += STEP * up;
            at += STEP * up;
        }
        if (keys[SDL_SCANCODE_LCTRL]) {
            eye -= STEP * up;
            at -= STEP * up;
        }
        auto view = glm::lookAt(eye, at, up);
        shaders.updateViewMatrices(view);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_colour->apply();
        auto scale = glm::mat4();
        scale = glm::scale(scale, glm::vec3(5.f, 5.f, 5.f));
        shader_colour->updateWorldMatrix(scale);
        origin.draw();

        shader_transform->apply();
        auto modelQuat = glm::quat();
        modelQuat = glm::rotate(modelQuat, angle, glm::vec3(0, 1, 0));
        auto modelMat = (glm::mat4)modelQuat;
        shader_transform->updateWorldMatrix((glm::mat4)modelMat);
        cube.draw();

        glFlush();
        SDL_GL_SwapWindow(window);

        angle += 0.01f;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}