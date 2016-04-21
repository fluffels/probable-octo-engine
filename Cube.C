#include "Cube.h"

#include <GL/glew.h>

Cube::
Cube():
   IndexedMesh()
{
   _vertexCount = 8;
   unsigned SIZE = (unsigned)_vertexCount * 3;

   GLfloat* vertices = new GLfloat[SIZE];
   int i = 0;

   vertices[i++] = -1.0f;
   vertices[i++] = -1.0f;
   vertices[i++] = -1.0f;

   vertices[i++] = -1.0f;
   vertices[i++] = -1.0f;
   vertices[i++] = 1.0f;

   vertices[i++] = 1.0f;
   vertices[i++] = -1.0f;
   vertices[i++] = 1.0f;

   vertices[i++] = 1.0f;
   vertices[i++] = -1.0f;
   vertices[i++] = -1.0f;

   vertices[i++] = -1.0f;
   vertices[i++] = 1.0f;
   vertices[i++] = -1.0f;

   vertices[i++] = -1.0f;
   vertices[i++] = 1.0f;
   vertices[i++] = 1.0f;

   vertices[i++] = 1.0f;
   vertices[i++] = 1.0f;
   vertices[i++] = 1.0f;

   vertices[i++] = 1.0f;
   vertices[i++] = 1.0f;
   vertices[i++] = -1.0f;

   _indexCount = 6 * 6;
   GLuint* indices = new GLuint[_indexCount];
   i = 0;

   /* Back. */
   indices[i++] = 3;
   indices[i++] = 0;
   indices[i++] = 4;
   indices[i++] = 3;
   indices[i++] = 4;
   indices[i++] = 7;

   /* Front. */
   indices[i++] = 1;
   indices[i++] = 6;
   indices[i++] = 5;
   indices[i++] = 1;
   indices[i++] = 2;
   indices[i++] = 6;

   /* Bottom. */
   indices[i++] = 2;
   indices[i++] = 1;
   indices[i++] = 0;
   indices[i++] = 2;
   indices[i++] = 0;
   indices[i++] = 3;

   /* Left. */
   indices[i++] = 0;
   indices[i++] = 1;
   indices[i++] = 5;
   indices[i++] = 0;
   indices[i++] = 5;
   indices[i++] = 4;

   /* Top. */
   indices[i++] = 5;
   indices[i++] = 7;
   indices[i++] = 4;
   indices[i++] = 5;
   indices[i++] = 6;
   indices[i++] = 7;

   /* Right. */
   indices[i++] = 2;
   indices[i++] = 7;
   indices[i++] = 6;
   indices[i++] = 2;
   indices[i++] = 3;
   indices[i++] = 7;


   bind();

   GLuint vertexBuffer, indexBuffer;

   glGenBuffers(1, &vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

   glBufferData(GL_ARRAY_BUFFER, SIZE * (int)sizeof(GLfloat),
         vertices, GL_STATIC_DRAW);

   glGenBuffers(1, &indexBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

   glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * (int)sizeof(GLuint),
         indices, GL_STATIC_DRAW);

   GLuint positionLoc = findAttribute("position");
   glEnableVertexAttribArray((GLuint)positionLoc);
   glVertexAttribPointer((GLuint)positionLoc, 3, GL_FLOAT, GL_FALSE, 
         0, 0);

   delete[] vertices;
   delete[] indices;
}

