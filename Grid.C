#include "Grid.h"

Grid::
Grid(float length, unsigned resolution) :
        IndexedMesh() {
    const unsigned VERTICES_PER_ROW = resolution + 1;
    const unsigned VERTICES_PER_COL = resolution + 1;
    _vertexCount = (GLsizei) VERTICES_PER_ROW * (GLsizei) VERTICES_PER_COL;

    const unsigned VERTEX_COMPONENT_COUNT = 3;
    const unsigned VERTEX_ARRAY_SIZE = (unsigned) _vertexCount *
                                       VERTEX_COMPONENT_COUNT;
    float *vertices = new float[VERTEX_ARRAY_SIZE];

    const unsigned SQUARES = resolution * resolution;
    const unsigned TRIANGLES_PER_SQUARE = 2;
    const unsigned VERTICES_PER_TRIANGLE = 3;
    const unsigned VERTICES_PER_SQUARE = TRIANGLES_PER_SQUARE *
                                         VERTICES_PER_TRIANGLE;
    _indexCount = (GLsizei) VERTICES_PER_SQUARE * (GLsizei) SQUARES;

    const unsigned INDEX_ARRAY_SIZE = (unsigned) _indexCount;
    GLuint *indices = new GLuint[INDEX_ARRAY_SIZE];

    const float Z_DELTA = length / (float) resolution;
    const float X_DELTA = length / (float) resolution;

    unsigned index = 0;
    for (unsigned z = 0; z < VERTICES_PER_COL; z++) {
        const float Z = (float) z * Z_DELTA;

        for (unsigned x = 0; x < VERTICES_PER_ROW; x++) {
            const float X = (float) x * X_DELTA;

            vertices[index++] = X;
            vertices[index++] = 0.0f;
            vertices[index++] = Z;
        }
    }

    index = 0;
    for (unsigned z = 0; z < VERTICES_PER_COL - 1; z++) {
        for (unsigned x = 0; x < VERTICES_PER_ROW - 1; x++) {
            unsigned i = z * VERTICES_PER_COL + x;

            /* Top left square. */
            indices[index++] = i;
            indices[index++] = i + VERTICES_PER_ROW;
            indices[index++] = i + 1;

            /* Bottom right square. */
            indices[index++] = i + 1;
            indices[index++] = i + VERTICES_PER_ROW;
            indices[index++] = i + 1 + VERTICES_PER_ROW;
        }
    }

    bind();

    GLuint buffers[2];
    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE * (GLsizei) sizeof(float),
                 vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_ARRAY_SIZE * (GLsizei) sizeof(GLuint),
                 indices, GL_STATIC_DRAW);

    GLuint positionLocation = findAttribute("position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, VERTEX_COMPONENT_COUNT, GL_FLOAT,
                          GL_FALSE, 0, 0);
}
