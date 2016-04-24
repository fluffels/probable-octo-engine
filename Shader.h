#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CompilationError.h"
#include "LinkError.h"

using glm::vec3;
using glm::mat4;
using glm::value_ptr;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Shader {
public:
    /**
      * Constructor.
      *
      * Obtains OpenGL handles, loads shader program source, compiles and
      * links.
      *
      * @param name The shader's name. This is the name of the directory that
      * will be used as a prefix when loading the shader program source.
      */
    Shader(const string &name);

    /**
      * Destructor.
      *
      * Releases OpenGL handles.
      */
    ~Shader();

    /**
      * Apply the shader.
      *
      * This will activate the shader on the graphics card's execution units
      * and load any uniforms it's using.
      */
    void apply();

    /**
     * Get the Shader's name.
     * @return A string.
     */
    const string &getName();

    /**
      * Helper function to find uniform values.
      *
      * @param name The uniform's name in the shader source.
      *
      * @return The uniform's location as a GLint.
      *
      * @throws LinkError If the uniform could not be found.
      */
    GLint findUniform(const char *name);

    /**
      * Update the shader's projection matrix.
      *
      * @param matrix The projection matrix to pass to the shader program.
      */
    void updateProjectionMatrix(mat4 matrix);

    /**
      * Update the shader's view matrix.
      *
      * @param matrix The view matrix to pass to the shader program.
      */
    void updateViewMatrix(mat4 matrix);

    /**
      * Update the shader's world matrix.
      *
      * @param matrix The world matrix to pass to the shader program.
      */
    void updateWorldMatrix(mat4 matrix);

    /**
      * Update an arbitrary uniform. Prints a warning if the uniform could
      * not be found.
      *
      * @param name The uniform's name in the shader program.
      * @param value The uniform's new value.
      */
    void updateUniform(const char *name, float value);

    void updateUniform(const char *name, mat4 value);

    void updateUniform(const char *name, vec3 value);

private:
    /** The shader's name. */
    string _name;

    /** The OpenGL handle for this shader program. */
    GLuint _programHandle;
    /** The OpenGL handle for the vertex shader. */
    GLuint _vertexShaderHandle;
    /** The OpenGL handle for the tesselation control shader.*/
    GLuint _tessControlHandle;
    /** The OpenGL handle for the tesselation evaluation shader. */
    GLuint _tessEvaluationHandle;
    /** The OpenGL handle for the geometry shader. */
    GLuint _geometryShaderHandle;
    /** The OpenGL handle for the fragment shader. */
    GLuint _fragmentShaderHandle;

    /** Shader program location of the projection matrix. */
    GLint _projectionLocation;
    /** Shader program location of the view matrix. */
    GLint _viewLocation;
    /** Shader program location of the world matrix. */
    GLint _worldLocation;

    /** The current projection matrix. */
    mat4 _projectionMatrix;
    /** The current view matrix. */
    mat4 _viewMatrix;
    /** The current world matrix. */
    mat4 _worldMatrix;

    /** Standard file name for the vertex shader. */
    const static string VERTEX_SHADER_FILE_NAME;
    /** Standard file name for the tesselation control shader. */
    const static string TESS_CONTROL_FILE_NAME;
    /** Standard file name for the tesselation evaluation shader. */
    const static string TESS_EVAL_FILE_NAME;
    /** Standard file name for the geometry shader. */
    const static string GEOMETRY_SHADER_FILE_NAME;
    /** Standard file name for the fragment shader. */
    const static string FRAGMENT_SHADER_FILE_NAME;

    /**
      * Checks if the given file exists.
      *
      * @param file The file to check for.
      *
      * @return True if the file exists, false otherwise.
      */
    bool fileExists(const string &file);

    /**
      * Helper function to initialize a shader.
      *
      * @param handle An unsigned to store the OpenGL handle in.
      * @param file The file name.
      * @param shaderType The type of shader to initialize.
      */
    void initShader(unsigned &handle, const string &file, GLenum shaderType);

    /**
      * Helper function to check whether this is the currently bound program.
      *
      * @return True if this is currently bound, false otherwise.
      */
    bool isCurrentProgram();

    /**
      * Helper function to load a file in the way required of shaders.
      *
      * @param name The name of the file to load.
      *
      * @returns The whole file as a continuous c-string.
      */
    char *loadFile(const string &name);
};
