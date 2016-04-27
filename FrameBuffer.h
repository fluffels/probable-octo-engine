#pragma once

#include <GL/glew.h>

/**
  * Encapsulates a frame buffer object.
  */
class FrameBuffer
{
   public:
      /**
        * Constructor.
        */
      FrameBuffer();

      /**
        * Destructor.
        */
      virtual ~FrameBuffer();

      /**
        * Bind the frame buffer.
        */
      void bind();

      /**
        * Unbind frame buffer.
        */
      void unbind();

   private:
      /** Name of the frame buffer object. */
      GLuint _id;
};
