#include "CubeMap.h"

CubeMap::
CubeMap(const string &prefix) :
        Texture(),
        PX("positive_x"),
        NX("negative_x"),
        PY("positive_y"),
        NY("negative_y"),
        PZ("positive_z"),
        NZ("negative_z") {
    glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int width = 0;
    int height = 0;

    string file = Texture::PATH + prefix + PX + EXTENSION;
    unsigned char *data = loadTextureFromPNG(file, width, height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[] data;

    file = Texture::PATH + prefix + NX + EXTENSION;
    data = loadTextureFromPNG(file, width, height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[] data;

    file = Texture::PATH + prefix + PY + EXTENSION;
    data = loadTextureFromPNG(file, width, height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[] data;

    file = Texture::PATH + prefix + NY + EXTENSION;
    data = loadTextureFromPNG(file, width, height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[] data;

    file = Texture::PATH + prefix + PZ + EXTENSION;
    data = loadTextureFromPNG(file, width, height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[] data;

    file = Texture::PATH + prefix + NZ + EXTENSION;
    data = loadTextureFromPNG(file, width, height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[] data;
}

CubeMap::
~CubeMap() {
}

void CubeMap::
bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);
}
