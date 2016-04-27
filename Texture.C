#include "Texture.h"

using std::cerr;

const string Texture::EXTENSION = ".png";
const string Texture::PATH = "./textures/";

Texture::
Texture(const string &filename) :
        _handle(0) {
    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_2D, _handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    const string TEXTURE_FILE = PATH + filename + EXTENSION;

    GLint width = 0;
    GLint height = 0;

    GLubyte *data = loadTextureFromPNG(TEXTURE_FILE, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    delete[] data;
}

Texture::
Texture() :
        _handle(0) {
    glGenTextures(1, &_handle);
}

Texture::
~Texture() {
}

void Texture::
bind() {
    glBindTexture(GL_TEXTURE_2D, _handle);
}

GLubyte *Texture::
loadTextureFromPNG(const string &filename, int &width, int &height) {
    cerr << "[TRACE] Loading " << filename << endl;

    FILE *file = fopen(filename.c_str(), "r");
    if (file == nullptr) {
        perror("[ERROR] Could not open file");
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                 NULL, NULL);
    if (png_ptr == NULL) {
        printf("Could not initialize libPNG's read struct.\n");
        exit(-1);
    }

    png_infop png_info_ptr = png_create_info_struct(png_ptr);
    if (png_info_ptr == NULL) {
        printf("Could not initialize libPNG's info pointer.\n");
        exit(-1);
    }

    /* We jump back here if an error is encountered. */
    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("LibPNG encountered an error.\n");

        png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
        exit(-1);
    }

    png_init_io(png_ptr, file);

    png_read_png(png_ptr, png_info_ptr, 0, NULL);

    png_uint_32 png_width = 0;
    png_uint_32 png_height = 0;
    int bits = 0;
    int colour_type = 0;
    png_get_IHDR(png_ptr, png_info_ptr, &png_width, &png_height, &bits, &colour_type,
                 NULL, NULL, NULL);

    const unsigned BITS_PER_BYTE = 8;
    unsigned bytes_per_colour = (unsigned) bits / BITS_PER_BYTE;
    unsigned colours_per_pixel;

    if (colour_type == PNG_COLOR_TYPE_RGBA) {
        colours_per_pixel = 4;
    }
    else {
        cout << "[ERROR] Cannot load " << filename << ", since it is not in "
        << "RGBA format." << endl;
        exit(-1);
    }

    cout << "[INFO] Loaded " << filename << " as PNG. Width = " << png_width
    << ", height = " << png_height << ", bit depth = " << bits
    << ", colour type = " << colour_type << "." << endl;

    unsigned char *data = new unsigned char[png_width * png_height * colours_per_pixel *
                                            bytes_per_colour];

    png_bytepp row_pointers = png_get_rows(png_ptr, png_info_ptr);

    unsigned index = 0;
    for (unsigned y = 0; y < png_height; y++) {
        unsigned x = 0;
        while (x < png_width * colours_per_pixel * bytes_per_colour) {
            data[index++] = row_pointers[y][x++];
            data[index++] = row_pointers[y][x++];
            data[index++] = row_pointers[y][x++];
            data[index++] = row_pointers[y][x++];
        }
    }

    width = (int) png_width;
    height = (int) png_height;

    return data;
}

