#include "Terrain.h"

Terrain::
Terrain(string heightmap) :
        IndexedMesh(),
        COMPONENTS(3),
        _vertices(NULL),
        _normals(NULL),
        _indices(NULL),
        _width(0),
        _depth(0),
        _max_height(0.f),
        _terrain_width(0.f),
        _terrain_depth(0.f),
        _hm(heightmap.c_str()) {
    construct();
}

Terrain::
Terrain(const Terrain &rhs) :
        IndexedMesh(),
        COMPONENTS(3),
        _vertices(NULL),
        _normals(NULL),
        _indices(NULL),
        _width(0),
        _depth(0),
        _max_height(0.f),
        _terrain_width(0.f),
        _terrain_depth(0.f),
        _hm(rhs._hm) {
    construct();
}

const Terrain &Terrain::
operator=(Terrain &rhs) {
    if (this != &rhs) {
        delete[] _vertices;
        delete[] _normals;
        delete[] _indices;

        this->_hm = rhs._hm;
        construct();
    }

    return *this;
}

float Terrain::
getHeight(unsigned x, unsigned z) {
    return getCoord(x, z)[Y];
}

void Terrain::
setHeight(unsigned x, unsigned z, GLfloat height) {
    getCoord(x, z)[Y] = height;
}

float Terrain::
getMaxHeight() const {
    return _max_height;
}

float Terrain::
getWidth() const {
    return _terrain_width;
}

float Terrain::
getDepth() const {
    return _terrain_depth;
}

float *Terrain::
getHeightArray() {
    GLfloat *result = new GLfloat[_vertexCount];
    GLfloat *v = getCoord(0, 0);

    for (unsigned i = 0; i < (unsigned) _vertexCount; i++) {
        result[i] = v[Y];
        v += COMPONENTS;
    }

    return result;
}

void Terrain::
construct() {
    _width = (unsigned) _hm.get_width();
    _depth = (unsigned) _hm.get_height();
    _vertexCount = (GLsizei) _depth * (GLsizei) _depth;

    const unsigned SIZE = (unsigned) _vertexCount * COMPONENTS;
    _vertices = new float[SIZE];
    _normals = new float[SIZE];

    _indexCount = (GLsizei) (_width * _depth * 6);
    _indices = new GLuint[_indexCount];

    generateVertices();
    for (int i = 0; i < 5; i++) {
        smoothVertices();
    }
    generateNormals();
    generateIndices();

    bind();

    GLuint buffers[3];
    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, SIZE * (GLsizei) sizeof(float),
                 _vertices, GL_STATIC_DRAW);

    GLuint position = findAttribute("position");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, (GLint) COMPONENTS, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, SIZE * (GLsizei) sizeof(float),
                 _normals, GL_STATIC_DRAW);

    GLuint normal = findAttribute("normal");
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, (GLint) COMPONENTS, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * (GLsizei) sizeof(GLuint),
                 _indices, GL_STATIC_DRAW);
}

float *Terrain::
getCoord(unsigned x, unsigned z) {
    return _vertices + (z * _width * COMPONENTS) + (x * COMPONENTS);
}

void Terrain::
generateVertices() {
    const float Z_DELTA = 10.0f;
    const float X_DELTA = 10.0f;

    unsigned index = 0;
    float Zcoord = 0.0f;
    float Xcoord = 0.0f;
    for (unsigned z = 0; z < _depth; z++) {
        Xcoord = 0.0f;
        for (unsigned x = 0; x < _width; x++) {
            float height = (float) _hm.get_pixel(x, z).red * (1/64.0f);
            if (height > _max_height) _max_height = height;
            _vertices[index++] = Xcoord;
            _vertices[index++] = height;
            _vertices[index++] = Zcoord;
            Xcoord += X_DELTA;
        }
        Zcoord += Z_DELTA;
    }
    _terrain_depth = Zcoord;
    _terrain_width = Xcoord;
}

void Terrain::
generateNormals() {
    for (unsigned z = 1; z < _depth - 1; z++) {
        unsigned index = (_width * z + 1) * COMPONENTS;
        float *mid = _vertices + index;
        float *top = mid + _width * COMPONENTS;
        float *left = mid - COMPONENTS;
        float *right = mid + COMPONENTS;
        float *bot = mid - _width * COMPONENTS;

        for (unsigned x = 1; x < _width - 1; x++) {
            vec3 m(mid[X], mid[Y], mid[Z]);
            vec3 t(top[X], top[Y], top[Z]);
            vec3 r(right[X], right[Y], right[Z]);
            vec3 l(left[X], left[Y], left[Z]);
            vec3 b(bot[X], bot[Y], bot[Z]);

            vec3 v0 = t - m;
            vec3 v1 = r - m;
            vec3 n = normalize(cross(v0, v1));
            vec3 v2 = l - m;
            n = n + normalize(cross(v2, v0));
            vec3 v3 = b - m;
            n = n + normalize(cross(v3, v2));
            n = n + normalize(cross(v1, v3));
            n /= 4;

            _normals[index++] = n.x;
            _normals[index++] = n.y;
            _normals[index++] = n.z;

            mid += COMPONENTS;
            top += COMPONENTS;
            right += COMPONENTS;
            left += COMPONENTS;
            bot += COMPONENTS;
        }
    }
}

void Terrain::
generateIndices() {
    unsigned index = 0;
    for (unsigned z = 0; z < _depth - 1; z++) {
        for (unsigned x = 0; x < _width - 1; x++) {
            unsigned i = z * _width + x;

            /* Top left square. */
            _indices[index++] = i;
            _indices[index++] = i + _width;
            _indices[index++] = i + 1;

            /* Bottom right square. */
            _indices[index++] = i + 1;
            _indices[index++] = i + _width;
            _indices[index++] = i + 1 + _width;
        }
    }
}

void Terrain::
smoothVertices() {
    const unsigned SAMPLE = 4;
    for (unsigned z = SAMPLE; z < _depth - SAMPLE; z++) {
        float *t = getCoord(SAMPLE, z + 1);
        float *m = getCoord(SAMPLE, z);
        float *b = getCoord(SAMPLE, z - 1);

        for (unsigned x = SAMPLE; x < _width - SAMPLE; x++) {
            float acc = m[Y] + (m - COMPONENTS)[Y] + (m + COMPONENTS)[Y];
            acc += t[Y] + (t - COMPONENTS)[Y] + (t + COMPONENTS)[Y];
            acc += b[Y] + (b - COMPONENTS)[Y] + (b + COMPONENTS)[Y];

            m[Y] = acc / 9;

            t += COMPONENTS;
            m += COMPONENTS;
            b += COMPONENTS;
        }
    }
}
