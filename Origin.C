#include "Origin.h"

Origin::Origin():
        Mesh() {
    _vertexCount = 6;
    unsigned SIZE = (unsigned)_vertexCount * 3;

    GLfloat* vertices = new GLfloat[SIZE];
    GLfloat* colours = new GLfloat[SIZE];

    int i = 0;
    int j = 0;

    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;

    colours[j++] = 1.0f;
    colours[j++] = 0.0f;
    colours[j++] = 0.0f;

    vertices[i++] = 1.0f;
    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;

    colours[j++] = 1.0f;
    colours[j++] = 0.0f;
    colours[j++] = 0.0f;

    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;

    colours[j++] = 0.0f;
    colours[j++] = 1.0f;
    colours[j++] = 0.0f;

    vertices[i++] = 0.0f;
    vertices[i++] = 1.0f;
    vertices[i++] = 0.0f;

    colours[j++] = 0.0f;
    colours[j++] = 1.0f;
    colours[j++] = 0.0f;

    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;

    colours[j++] = 0.0f;
    colours[j++] = 0.0f;
    colours[j++] = 1.0f;

    vertices[i++] = 0.0f;
    vertices[i++] = 0.0f;
    vertices[i++] = 1.0f;

    colours[j++] = 0.0f;
    colours[j++] = 0.0f;
    colours[j++] = 1.0f;

    bind();

    GLuint vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, SIZE * (int)sizeof(GL_FLOAT),
                 vertices, GL_STATIC_DRAW);

    GLuint positionLoc = findAttribute("position");
    glEnableVertexAttribArray((GLuint)positionLoc);
    glVertexAttribPointer((GLuint)positionLoc, 3, GL_FLOAT, GL_FALSE,
                          0, 0);

    GLuint colourBuffer;

    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);

    glBufferData(GL_ARRAY_BUFFER, SIZE * (int)sizeof(GL_FLOAT),
                 colours, GL_STATIC_DRAW);

    GLuint colourLoc = findAttribute("colour");
    glEnableVertexAttribArray((GLuint)colourLoc);
    glVertexAttribPointer((GLuint)colourLoc, 3, GL_FLOAT, GL_FALSE,
                          0, 0);

    delete[] vertices;
    delete[] colours;
};

void Origin::draw() {
    bind();
    glDrawArrays(GL_LINES, 0, _vertexCount);
}
