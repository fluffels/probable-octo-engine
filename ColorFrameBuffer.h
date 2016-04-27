#pragma once

#include "FrameBuffer.h"
#include "RuntimeError.h"

#include <iostream>

#include <GL/glew.h>

#include <png++/png.hpp>

using namespace std;

/**
  * This class encapsulates a color frame buffer.
  */
class ColorFrameBuffer : public FrameBuffer {
public:
    /**
      * Constructor.
      *
      * @param width The width we want for the frame buffer.
      * @param height The height we want for the frame buffer.
      */
    ColorFrameBuffer(GLsizei width, GLsizei height);

    /**
      * Write out the color frame buffer.
      */
    void write();

private:
    /** The width of the frame buffer. */
    GLsizei _width;
    /** The height of the frame buffer. */
    GLsizei _height;
};
