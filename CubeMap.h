#pragma once

#include <string>

#include "Texture.h"

using std::string;

/**
  * Encapsulates a cube map texture.
  */
class CubeMap : public Texture {
public:
    /**
      * Construct a texture.
      *
      * @param prefix The prefix to use when finding the files.
      */
    CubeMap(const string &prefix);

    /**
      * Destructor.
      */
    virtual ~CubeMap();

    /**
      * Bind the texture to OpenGL's context.
      */
    virtual void bind();

private:
    /** Postfix for the positive x-axis texture. */
    const string PX;
    /** Postfix for the negative x-axis texture. */
    const string NX;
    /** Postfix for the positive y-axis texture. */
    const string PY;
    /** Postfix for the negative y-axis texture. */
    const string NY;
    /** Postfix for the positive z-axis texture. */
    const string PZ;
    /** Postfix for the negative z-axis texture. */
    const string NZ;
};

#endif
