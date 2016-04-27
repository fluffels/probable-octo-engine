#include "ColorFrameBuffer.h"

ColorFrameBuffer::
ColorFrameBuffer(GLsizei width, GLsizei height) :
        FrameBuffer(),
        _width(width),
        _height(height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT,
                 NULL);

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);

    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        switch (status) {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                throw RuntimeError("Incomplete frame buffer attachment.");
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                throw RuntimeError("No color attachment in draw buffer.");
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                throw RuntimeError("No color attachment in read buffer.");
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                throw RuntimeError("Frame buffer is missing an attachment.");
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                throw RuntimeError("The frame buffer specifies different sampling sizes.");
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                throw RuntimeError("Not all frame buffer attachments are layered.");
            case GL_FRAMEBUFFER_UNSUPPORTED:
                throw RuntimeError("The frame buffer configuration is unsupported.");
            default:
                throw RuntimeError("Unknown frame buffer error.");
        }
    }

    unbind();
}

void ColorFrameBuffer::
write() {
    bind();

    const GLsizei DATA_SIZE = _width * _height *
                              (GLsizei) sizeof(GL_UNSIGNED_BYTE) * (GLsizei) 4;
    GLubyte *data = new GLubyte[(unsigned) DATA_SIZE];

    glReadPixels(0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    png::image<png::rgb_pixel> image((long unsigned) _width, (long unsigned) _height);
    GLsizei index = 0;
    for (GLsizei y = 0; y < _height; y++) {
        for (GLsizei x = 0; x < _width; x++) {
            image[(size_t) (_height - y - 1)][(size_t) x] =
                    png::rgb_pixel(data[index], data[index + 1], data[index + 2]);
            index += 4;
        }
    }
    image.write("screen_shot.png");

    unbind();
}

