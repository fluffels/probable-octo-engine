#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>

#include "IndexedMesh.h"

class Cube: public IndexedMesh
{
   public:
      /**
        * Constructor. Creates the cube.
        */
      Cube();
};

#endif

