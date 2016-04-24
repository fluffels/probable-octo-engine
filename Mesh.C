#include "Mesh.h"

#include <iostream>

using namespace std;

Mesh::
Mesh() :
        _vertexCount(0),
        _vao(0) {
    glGenVertexArrays(1, &_vao);
    bind();
}

Mesh::
~Mesh() {
    glDeleteVertexArrays(1, &_vao);
}

void Mesh::
draw() {
    bind();
    glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
}

void Mesh::
bind() {
    glBindVertexArray(_vao);
}

GLuint Mesh::
findAttribute(const char *name) {
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    GLint location = glGetAttribLocation((GLuint) currentProgram, name);

    if (location == -1) {
        string sName(name);
        cerr << "[WARNING] Could not find attribute '" + sName + "'." << endl;
    }

    return (GLuint) location;
}

