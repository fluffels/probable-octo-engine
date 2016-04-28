#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>

#include "easylogging++.h"

#include "LinkError.h"

using std::cerr;
using std::endl;
using std::string;

class Mesh {
public:
    /**
      * Destructor. Deallocates memory.
      */
    virtual ~Mesh();

    /**
      * Draw the mesh.
      */
    virtual void draw();

protected:
    /**
      * Constructor. Protected so we can't instantiate an empty mesh.
      */
    Mesh();

    /**
      * Bind to the context.
      */
    void bind();

    /**
      * Helper function to find vertex attribute locations.
      *
      * @param name The attribute's name in the shader source.
      *
      * @return The attribute's location as a GLuint.
      *
      * @throws LinkError If the attribute could not be found.
      */
    GLuint findAttribute(const char *name);

    /**
      * Amount of vertices in this mesh.
      */
    GLsizei _vertexCount;

private:
    /**
      * Handle for the VAO.
      */
    GLuint _vao;
};
