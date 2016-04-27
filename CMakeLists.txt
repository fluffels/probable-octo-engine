cmake_minimum_required(VERSION 3.5)
project(probable_octo_engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lSDL2 -lGLEW -lGLU -lGL -lpng")

set(SOURCE_FILES main.C MessageException.C DynamicMessageException.C CompilationError.C LinkError.C Shader.C Mesh.C
        IndexedMesh.C Cube.C Origin.C ShaderManager.C Texture.C CubeMap.C)
add_executable(probable_octo_engine ${SOURCE_FILES})