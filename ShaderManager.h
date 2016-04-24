#pragma once

#include <string>
#include <vector>

#include "Shader.h"

using std::string;
using std::vector;

/**
 * This class manages Shader objects. Shader objects are allocated using this
 * class, which maintains a pointer to them. When a ShaderManager object goes
 * out of scope, all the Shader objects it was managing are deallocated.
 */
class ShaderManager {
public:
    /**
     * Constructor. Creates an empty ShaderManager.
     */
    ShaderManager();

    /**
     * Destructor. Frees memory.
     */
    virtual ~ShaderManager();

    /**
     * Get the shader with the specified name. If the shader has not been
     * loaded, compiled and linked, this function will do so.
     * @param name The name of the shader. Corresponds to the shaders/
     * subdirectory that contains the shader programs for this shader.
     */
    Shader& get(const string& name);

    /**
      * Update the projection matrix of all managed shaders.
      *
      * @param matrix The projection matrix to pass to the shaders.
      */
    void updateProjectionMatrices(mat4 matrix);

    /**
      * Update the view matrix of all managed shaders.
      *
      * @param matrix The view matrix to pass to the shaders.
      */
    void updateViewMatrices(mat4 matrix);


    /**
      * Update the world matrix of all managed shaders.
      *
      * @param matrix The world matrix to pass to the shaders.
      */
    void updateWorldMatrices(mat4 matrix);

private:
    /** List of shader objects managed by this ShaderManager. */
    vector<Shader*> _shaders;
};