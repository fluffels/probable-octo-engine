cmake_minimum_required(VERSION 3.5)
project(probable_octo_engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lSDL2 -lGLEW -lGLU -lGL -lpng")

set(SOURCE_FILES
        main.C
        MessageException.C DynamicMessageException.C CompilationError.C LinkError.C RuntimeError.C
        Mesh.C IndexedMesh.C Cube.C Origin.C Terrain.C Grid.C
        Shader.C ShaderManager.C
        Texture.C CubeMap.C
        FrameBuffer.C ColorFrameBuffer.C)
add_executable(probable_octo_engine ${SOURCE_FILES})