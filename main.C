#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "easylogging++.h"

#include "ColorFrameBuffer.h"
#include "Cube.h"
#include "CubeMap.h"
#include "Grid.h"
#include "Origin.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Terrain.h"

using glm::normalize;
using glm::translate;
using glm::scale;
using glm::mat4;
using glm::vec3;
using glm::vec4;

struct Camera {
    vec3 at;
    vec3 eye;
    vec3 up;
};

struct Material {
    float K_a;
    float K_d;
    float K_s;
    vec3 colour;
};

const float STEP = 100.0f;
const float ANGLE_DELTA = 3.14f;

SDL_Window* window;
float window_width = 640.0f;
float window_height = 480.0f;
uint64_t frequency = 0;
uint64_t time_last_frame = 0;

ColorFrameBuffer* frame_buffer_color;

ShaderManager* shaders;
Shader* shader_colour;
Shader* shader_skybox;
Shader* shader_terrain;
Shader* shader_water;

Mesh* skybox;
Mesh* origin;
Mesh* water;
Terrain* terrain;

CubeMap* environment_map;
Texture* texture_terrain;

INITIALIZE_EASYLOGGINGPP

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
        shader_skybox->apply();
        skybox->draw();
        shader_colour->apply();
        origin->draw();
    glEnable(GL_CULL_FACE);

    shader_terrain->apply();
    terrain->draw();

    shader_water->apply();
    float time = SDL_GetTicks() / 1000.f;
    shader_water->updateUniform("time", time);
    water->draw();

    glFlush();
    SDL_GL_SwapWindow(window);

    auto time_this_frame = SDL_GetPerformanceCounter();
    auto delta = (time_this_frame - time_last_frame) / (float)frequency;
    time_last_frame = time_this_frame;
    char title[255];
    auto fps = 1.f / delta;
    sprintf(title, "FPS: %f", fps);
    SDL_SetWindowTitle(window, title);
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
    frequency = SDL_GetPerformanceFrequency();
    time_last_frame = SDL_GetPerformanceCounter();
    LOG(INFO) << "Performance counter frequency: " << frequency;


    /* Initialize OpenGL. */
    LOG(TRACE) << "Initializing OpenGL...";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_VERTEX_ARRAY);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    /* Load assets. */
    LOG(TRACE) << "Loading assets...";
    shaders = new ShaderManager();
    {
        shader_terrain = shaders->get("terrain");
        shader_terrain->apply();
        glActiveTexture(GL_TEXTURE0);
        texture_terrain = new Texture("terrain");
        shader_terrain->updateUniform("tex", 0);
        terrain = new Terrain("heightmap.png");
        shader_terrain->updateUniform("max_height", terrain->getMaxHeight());
        auto world = mat4();
        world = translate(world, vec3(-terrain->getWidth() / 2.f,
                                      -terrain->getMaxHeight() / 2.f,
                                      -terrain->getDepth() / 2.f));
        shader_terrain->updateUniform("K_a", 0.1f);
        shader_terrain->updateUniform("K_d", 0.9f);
        shader_terrain->updateWorldMatrix(world);
        LOG(INFO) << "Maximum terrain height: " << terrain->getMaxHeight();
        LOG(INFO) << "Terrain width: " << terrain->getWidth();
        LOG(INFO) << "Terrain depth: " << terrain->getDepth();
    }
    {
        shader_skybox = shaders->get("skybox");
        shader_skybox->apply();
        glActiveTexture(GL_TEXTURE2);
        environment_map = new CubeMap("terrain_");
        shader_skybox->updateUniform("cubeMap", 2);
        auto world = mat4();
        world = scale(world, vec3(terrain->getWidth(),
                                  terrain->getWidth(),
                                  terrain->getDepth()));
        skybox = new Cube();
        shader_terrain->apply();
        shader_skybox->updateWorldMatrix(world);
    }
    {
        auto world = mat4();
        world = scale(world, vec3(terrain->getWidth() / 2.f,
                                  terrain->getMaxHeight() * 2.f,
                                  terrain->getDepth() / 2.f));
        shader_colour = shaders->get("colour");
        shader_colour->apply();
        shader_colour->updateWorldMatrix(world);
        origin = new Origin();
    } {
        auto world = mat4();
        world = translate(world, vec3(-terrain->getWidth() / 2.f,
                                      -terrain->getMaxHeight() / 2.f + 25.f,
                                      -terrain->getDepth() / 2.f));
        shader_water = shaders->get("water");
        shader_water->apply();
        shader_water->updateUniform("K_a", 0.3f);
        shader_water->updateUniform("K_d", 0.0f);
        shader_water->updateWorldMatrix(world);
        water = new Grid(terrain->getDepth(), 10.f);
    }


    /* Set up light. */
    auto light = Camera();
    light.eye = vec3(1024.0f, 1024.f, 1024.f);
    light.at = vec3(0.0f, 0.0f, 0.0f);
    light.up = vec3(0.0f, 0.0f, -1.0f);
    auto light_dir = normalize(vec3(0.f, 0.25f, -1.f));
    shader_terrain->apply();
    shader_terrain->updateUniform("light_dir", light_dir);
    shader_water->apply();
    shader_water->updateUniform("light_dir", light_dir);


    /* Set up view. */
    auto camera = Camera();
    auto camera_height = terrain->getMaxHeight() * 3.f;
    camera.eye = glm::vec3(0.f, camera_height, -terrain->getDepth() / 2.f);
    camera.at = glm::vec3(0, 0, 0);
    camera.up = glm::vec3(0, 1, 0);
    auto view = glm::lookAt(camera.eye, camera.at, camera.up);
    shaders->updateViewMatrices(view);


    /* Set up projection. */
    auto proj = glm::perspective(45.f, window_width / window_height, 100.f, 25000.f);
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
    delete environment_map;
    delete frame_buffer_color;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}