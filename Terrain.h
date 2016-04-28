#pragma once

#include <iostream>
#include <string>

#include <png++/png.hpp>
#include <glm/glm.hpp>

#include "easylogging++.h"

#include "Constants.h"
#include "IndexedMesh.h"
#include "RuntimeError.h"

using std::max;
using std::min;
using std::string;

using glm::cross;
using glm::normalize;
using glm::vec3;

using png::image;
using png::rgb_pixel;

/**
  * Encapsulates terrain.
  */
class Terrain : public IndexedMesh {
public:
    /**
      * Constructor, creates the mesh.
      *
      * @param heightmap The filename of a heightmap to use when generating this
      * terrain.
      */
    Terrain(string heightmap);

    /**
      * Copy constructor, unimplemented.
      */
    Terrain(const Terrain &rhs);

    /**
      * Assignment operator, unimplemented.
      */
    const Terrain &operator=(Terrain &rhs);

    /**
      * Get the height at the given heightmap coordinates.
      */
    float getHeight(unsigned x, unsigned z);

    /**
      * Get the height field as a one dimensional array.
      */
    float *getHeightArray();

    /**
      * Set the height at the given heightmap coordinates.
      * @param height The new height.
      */
    void setHeight(unsigned x, unsigned z, GLfloat height);

private:
    const unsigned COMPONENTS;

    /**
      * Helper constructor.
      */
    void construct();

    /**
      * Get the coordinate that corresponds to the given heightmap
      * coordinates.
      */
    float *getCoord(unsigned x, unsigned z);

    /**
      * Generate vertices.
      */
    void generateVertices();

    /**
      * Generate normals.
      */
    void generateNormals();

    /**
      * Generate indices.
      */
    void generateIndices();

    /**
      * Smooth out the quantized vertex data.
      */
    void smoothVertices();

    /** The vertices. */
    GLfloat *_vertices;
    /** The normals. */
    GLfloat *_normals;
    /** The indices. */
    GLuint *_indices;
    /** Width of the height map. */
    unsigned _width;
    /** Depth of the height map. */
    unsigned _depth;
    /** Height map. */
    image<rgb_pixel> _hm;
};
