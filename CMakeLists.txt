cmake_minimum_required(VERSION 3.5)
project(probable_octo_engine)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OpenGL is not installed.")
endif(NOT OPENGL_FOUND)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW is not installed.")
endif(NOT GLEW_FOUND)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
if(NOT SDL2_FOUND)
    message(ERROR " SDL2 is not installed.")
endif(NOT SDL2_FOUND)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})
add_definitions(${PNG_DEFITIONS})
if(NOT PNG_FOUND)
    message(ERROR " PNG is not installed.")
endif(NOT PNG_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.C
        MessageException.C DynamicMessageException.C CompilationError.C LinkError.C RuntimeError.C
        Mesh.C IndexedMesh.C Cube.C Origin.C Terrain.C Grid.C
        Shader.C ShaderManager.C
        Texture.C CubeMap.C
        FrameBuffer.C ColorFrameBuffer.C)
add_executable(probable_octo_engine ${SOURCE_FILES})

target_link_libraries(probable_octo_engine ${GLEW_LIBRARIES})
target_link_libraries(probable_octo_engine ${OPENGL_LIBRARIES})
target_link_libraries(probable_octo_engine ${SDL2_LIBRARY})
target_link_libraries(probable_octo_engine ${PNG_LIBRARIES})
