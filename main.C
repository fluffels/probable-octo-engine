#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "easylogging++.h"

#include "ColorFrameBuffer.h"
#include "Cube.h"
#include "CubeMap.h"
#include "Origin.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Terrain.h"

using glm::translate;
using glm::scale;
using glm::vec3;
using glm::vec4;

struct Camera {
    vec3 at;
    vec3 eye;
    vec3 up;
};

const float STEP = 10.0f;
const float ANGLE_DELTA = 3.14f;

SDL_Window* window;
float window_width = 640.0f;
float window_height = 480.0f;

ColorFrameBuffer* frame_buffer_color;

ShaderManager* shaders;
Shader* shader_colour;
Shader* shader_skybox;
Shader* shader_terrain;

Mesh* skybox;
Mesh* origin;
Mesh* terrain;

CubeMap* environmentMap;

INITIALIZE_EASYLOGGINGPP

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glDisable(GL_CULL_FACE);
    {
        shader_skybox->apply();
        auto world = mat4();
        world = scale(world, vec3(512.f, 512.f, 512.f));
        shader_skybox->updateWorldMatrix(world);
        skybox->draw();
    }
    {
        shader_colour->apply();
        auto world = mat4();
        world = scale(world, vec3(1024.f, 1024.f, 1024.f));
        shader_colour->updateWorldMatrix(world);
        origin->draw();
    }
    glEnable(GL_CULL_FACE);

    {
        shader_terrain->apply();
        auto world = mat4();
        world = translate(world, vec3(-512.f, -200.f, -512.f));
        shader_terrain->updateWorldMatrix(world);
        terrain->draw();
    }

    glFlush();
    SDL_GL_SwapWindow(window);
}

int main(int argc, char** argv) {
    /* Initialize logging. */
    START_EASYLOGGINGPP(argc, argv);
    LOG(TRACE) << "Logging initialized.";


    /* Initialize SDL. */
    LOG(TRACE) << "Initializing SDL...";
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0) {
        LOG(FATAL) << "Could not initialize SDL: " << SDL_GetError();
    }
    window = SDL_CreateWindow(argv[0], 0, 0, (int)window_width,
                              (int)window_height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        LOG(FATAL) << "Could not create window: " << SDL_GetError();
    }
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);


    /* Initialize OpenGL. */
    LOG(TRACE) << "Initializing OpenGL...";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        LOG(FATAL) << SDL_GetError();
    }
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        LOG(FATAL) << "Could not initialize GLEW: " << glewGetErrorString(glewError);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_VERTEX_ARRAY);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    /* Load assets. */
    LOG(TRACE) << "Loading assets...";
    environmentMap = new CubeMap("terrain_");
    shaders = new ShaderManager();
    shader_skybox = shaders->get("skybox");
    shader_skybox->apply();
    skybox = new Cube();
    auto skyboxWorld = glm::mat4();
    skyboxWorld = translate(skyboxWorld, vec3(512.0f, 200.0f, 512.0f));
    skyboxWorld = scale(skyboxWorld, glm::vec3(512.f, 512.f, 512.f));
    shader_skybox->updateWorldMatrix(skyboxWorld);
    shader_colour = shaders->get("colour");
    shader_colour->apply();
    origin = new Origin();
    shader_terrain = shaders->get("terrain");
    shader_terrain->apply();
    terrain = new Terrain("heightmap.png");


    /* Set up camera. */
    auto camera = Camera();
    camera.eye = glm::vec3(512.0f, 200.0f, 512.0f);
    camera.at = glm::vec3(0, 0, 0);
    camera.up = glm::vec3(0, 1, 0);
    auto view = glm::lookAt(camera.eye, camera.at, camera.up);
    shaders->updateViewMatrices(view);
    auto proj = glm::perspective(45.0f, window_width / window_height, 0.1f, 1500.0f);
    shaders->updateProjectionMatrices(proj);


    /* Set up frame buffers. */
    frame_buffer_color = new ColorFrameBuffer(window_width, window_height);


    /* Main loop. */
    float angle = 0.0f;
    bool done = false;
    SDL_Event event;
    LOG(TRACE) << "Entering main loop...";
    while (!done) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        LOG(INFO) << "Exiting normally at user request...";
                        done = true;
                        break;
                    case SDLK_PRINTSCREEN:
                        LOG(INFO) << "Rendering screen shot...";
                        frame_buffer_color->bind();
                        draw();
                        frame_buffer_color->write();
                        frame_buffer_color->unbind();
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
        shaders->updateViewMatrices(view);

        draw();

        angle += 0.01f;
    }

    delete shaders;
    delete environmentMap;
    delete frame_buffer_color;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}