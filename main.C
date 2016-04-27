#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "easylogging++.h"

#include "Cube.h"
#include "CubeMap.h"
#include "Origin.h"
#include "Shader.h"
#include "ShaderManager.h"

using glm::vec3;
using glm::vec4;

const float STEP = 0.1f;
const float ANGLE_DELTA = 3.14f;

float window_width = 640.0f;
float window_height = 480.0f;

INITIALIZE_EASYLOGGINGPP

struct Camera {
    vec3 at;
    vec3 eye;
    vec3 up;
};

int main(int argc, char** argv) {
    START_EASYLOGGINGPP(argc, argv);
    LOG(TRACE) << "Initializing...";

    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0) {
        LOG(ERROR) << "Could not initialize SDL: " << SDL_GetError();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(argv[0], 0, 0, (int)window_width,
                                          (int)window_height,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        LOG(ERROR) << "Could not create window: " << SDL_GetError();
        return 2;
    }

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        LOG(ERROR) << "Could not create OpenGL context: " << SDL_GetError();
        return 3;
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        LOG(ERROR) << "Could not initialize GLEW: " << glewGetErrorString(glewError);
        return 4;
    }

    glEnable(GL_DEPTH);
    glEnable(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glActiveTexture(GL_TEXTURE0);
    CubeMap* environmentMap = new CubeMap("terrain_");

    auto shaders = ShaderManager();

    auto shader_shadows = shaders.get("shadows");

    auto shader_skybox = shaders.get("skybox");
    shader_skybox->apply();
    auto skybox = Cube();

    auto shader_colour = shaders.get("colour");
    shader_colour->apply();
    auto origin = Origin();

    auto camera = Camera();
    camera.eye = glm::vec3(0, 0, 5.0f);
    camera.at = glm::vec3(0, 0, 0);
    camera.up = glm::vec3(0, 1, 0);
    auto view = glm::lookAt(camera.eye, camera.at, camera.up);
    shaders.updateViewMatrices(view);

    auto proj = glm::perspective(45.f, window_width / window_height, 0.1f,
                                 -100.f);
    shaders.updateProjectionMatrices(proj);

    float angle = 0.0f;
    bool done = false;
    SDL_Event event;
    while (!done) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        LOG(INFO) << "Exiting normally at user request.";
                        done = true;
                        break;
                    default:
                        break;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                const GLfloat X_SCALED = -(GLfloat)event.motion.xrel / window_width;
                const GLfloat Y_SCALED = -(GLfloat)event.motion.yrel / window_height;

                const GLfloat LEFT_RIGHT_ROT = X_SCALED * ANGLE_DELTA;
                const GLfloat UP_DOWN_ROT = Y_SCALED * ANGLE_DELTA;

                vec3 tempD(camera.at - camera.eye);
                vec4 d(tempD.x, tempD.y, tempD.z, 0.0f);

                vec3 right = cross(tempD, camera.up);

                mat4 rot;
                rot = rotate(rot, UP_DOWN_ROT, right);
                rot = rotate(rot, LEFT_RIGHT_ROT, camera.up);

                d = rot * d;

                camera.at.x = camera.eye.x + d.x;
                camera.at.y = camera.eye.y + d.y;
                camera.at.z = camera.eye.z + d.z;
            }
        }

        glm::vec3 direction = STEP * glm::normalize(camera.at - camera.eye);
        glm::vec3 right = STEP * glm::normalize(glm::cross(direction, camera.up));

        auto keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W]) {
            camera.eye += direction;
            camera.at += direction;
        }
        if (keys[SDL_SCANCODE_S]) {
            camera.eye -= direction;
            camera.at -= direction;
        }
        if (keys[SDL_SCANCODE_D]) {
            camera.eye += right;
            camera.at += right;
        }
        if (keys[SDL_SCANCODE_A]) {
            camera.eye -= right;
            camera.at -= right;
        }
        if (keys[SDL_SCANCODE_SPACE]) {
            camera.eye += STEP * camera.up;
            camera.at += STEP * camera.up;
        }
        if (keys[SDL_SCANCODE_LCTRL]) {
            camera.eye -= STEP * camera.up;
            camera.at -= STEP * camera.up;
        }
        view = glm::lookAt(camera.eye, camera.at, camera.up);
        shaders.updateViewMatrices(view);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_colour->apply();
        auto scaleMat = glm::mat4();
        scaleMat = glm::scale(scaleMat, glm::vec3(5.f, 5.f, 5.f));
        shader_colour->updateWorldMatrix(scaleMat);
        origin.draw();

        shader_skybox->apply();
        scaleMat = glm::mat4();
        scaleMat = glm::scale(scaleMat, glm::vec3(512.f, 512.f, 512.f));
        shader_skybox->updateWorldMatrix(scaleMat);
        skybox.draw();

        glFlush();
        SDL_GL_SwapWindow(window);

        angle += 0.01f;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}