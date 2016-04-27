#pragma once

#include <iostream>
#include <string>

#include <GL/gl.h>
#include <png.h>

using std::cout;
using std::endl;
using std::string;

/**
  * Encapsulates a texture file.
  */
class Texture {
public:
    /**
      * Construct a texture.
      *
      * @param filename The file name to use as a source, sans extension.
      */
    Texture(const string &filename);

    /**
      * Destructor.
      */
    virtual ~Texture();

    /**
      * Bind the texture.
      */
    virtual void bind();

protected:
    /**
      * Default constructor for subclasses that don't need to load a file
      * into a normal 2D texture.
      */
    Texture();

    /**
      * Load texture data from a PNG file.
      *
      * @param filename Name of the texture's PNG file.
      * @param width A place to return the width of the file to.
      * @param height A place to return the height of the file to.
      *
      * @return A pointer to the image data as unsigned bytes.
      */
    GLubyte *
            loadTextureFromPNG(const string &filename, int &width, int &height);

    /** Texture handle. */
    GLuint _handle;

    /** Extension for texture files. */
    static const string EXTENSION;
    /** Path to texture files. */
    static const string PATH;
};
