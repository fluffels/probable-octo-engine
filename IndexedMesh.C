#include "IndexedMesh.h"

IndexedMesh::
IndexedMesh() :
        _indexCount(0) {
}

void IndexedMesh::
draw() {
    bind();
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);
}

