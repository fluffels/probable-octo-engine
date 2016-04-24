#include "Shader.h"

const string Shader::VERTEX_SHADER_FILE_NAME = "vertex.glsl";
const string Shader::TESS_CONTROL_FILE_NAME = "tesselation_control.glsl";
const string Shader::TESS_EVAL_FILE_NAME = "tesselation_evaluation.glsl";
const string Shader::GEOMETRY_SHADER_FILE_NAME = "geometry.glsl";
const string Shader::FRAGMENT_SHADER_FILE_NAME = "fragment.glsl";

Shader::
Shader(const string &name) :
        _name(name),
        _programHandle(0),
        _vertexShaderHandle(0),
        _tessControlHandle(0),
        _tessEvaluationHandle(0),
        _geometryShaderHandle(0),
        _fragmentShaderHandle(0),
        _projectionLocation(-1),
        _viewLocation(-1),
        _worldLocation(-1),
        _projectionMatrix(mat4(1.0f)),
        _viewMatrix(mat4(1.0f)),
        _worldMatrix(mat4(1.0f)) {
    /* Start by defining a shader program which acts as a container. */
    _programHandle = glCreateProgram();

    string path = "./shaders/" + name + "/";
    const string VSF = path + VERTEX_SHADER_FILE_NAME;
    const string TCF = path + TESS_CONTROL_FILE_NAME;
    const string TEF = path + TESS_EVAL_FILE_NAME;
    const string GSF = path + GEOMETRY_SHADER_FILE_NAME;
    const string FSF = path + FRAGMENT_SHADER_FILE_NAME;

    initShader(_vertexShaderHandle, VSF, GL_VERTEX_SHADER);
    glAttachShader(_programHandle, _vertexShaderHandle);

    if (fileExists(TCF)) {
        initShader(_tessControlHandle, TCF, GL_TESS_CONTROL_SHADER);
        glAttachShader(_programHandle, _tessControlHandle);

        initShader(_tessEvaluationHandle, TEF, GL_TESS_EVALUATION_SHADER);
        glAttachShader(_programHandle, _tessEvaluationHandle);
    }
    else {
        cout << "[INFO] Shader '" << name << "' lacks tesselation." << endl;
    }

    if (fileExists(GSF)) {
        initShader(_geometryShaderHandle, GSF, GL_GEOMETRY_SHADER);
        glAttachShader(_programHandle, _geometryShaderHandle);
    }
    else {
        cout << "[INFO] Shader '" << name << "' lacks geometry shader." << endl;
    }

    initShader(_fragmentShaderHandle, FSF, GL_FRAGMENT_SHADER);
    glAttachShader(_programHandle, _fragmentShaderHandle);

    glLinkProgram(_programHandle);

    int status;
    glGetProgramiv(_programHandle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        int len;
        glGetProgramiv(_programHandle, GL_INFO_LOG_LENGTH, &len);

        char *log = new char[len];
        glGetProgramInfoLog(_programHandle, len, &len, log);
        string sLog(log);
        delete log;

        throw LinkError(sLog);
    }

    _projectionLocation = findUniform("projection");
    _viewLocation = findUniform("view");
    _worldLocation = findUniform("world");
}

Shader::
~Shader() {
    glDetachShader(_programHandle, _vertexShaderHandle);
    glDeleteShader(_vertexShaderHandle);

    glDetachShader(_programHandle, _fragmentShaderHandle);
    glDeleteShader(_fragmentShaderHandle);

    glDeleteProgram(_programHandle);
}

void Shader::
apply() {
    glUseProgram(_programHandle);

    glUniformMatrix4fv(_projectionLocation, 1, false, value_ptr(_projectionMatrix));
    glUniformMatrix4fv(_viewLocation, 1, false, value_ptr(_viewMatrix));
    glUniformMatrix4fv(_worldLocation, 1, false, value_ptr(_worldMatrix));
}

const string &Shader::
getName() {
    return _name;
}

void Shader::
updateProjectionMatrix(mat4 matrix) {
    _projectionMatrix = matrix;

    if (isCurrentProgram()) {
        glUniformMatrix4fv(_projectionLocation, 1, false, value_ptr(_projectionMatrix));
    }
}

void Shader::
updateViewMatrix(mat4 matrix) {
    _viewMatrix = matrix;

    if (isCurrentProgram()) {
        glUniformMatrix4fv(_viewLocation, 1, false, value_ptr(_viewMatrix));
    }
}

void Shader::
updateWorldMatrix(mat4 matrix) {
    _worldMatrix = matrix;

    if (isCurrentProgram()) {
        glUniformMatrix4fv(_worldLocation, 1, false, value_ptr(_worldMatrix));
    }
}

void Shader::
updateUniform(const char *name, float value) {
    GLint location = findUniform(name);
    glUniform1f(location, value);
}

void Shader::
updateUniform(const char *name, mat4 value) {
    GLint location = findUniform(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void Shader::
updateUniform(const char *name, vec3 value) {
    GLint location = findUniform(name);
    glUniform3fv(location, 1, value_ptr(value));
}

GLint Shader::
findUniform(const char *name) {
    GLint location = glGetUniformLocation(_programHandle, name);

    if (location == -1) {
        string sName(name);
        cout << "[WARNING] Could not locate uniform '" << sName << "'." << endl;
    }

    return location;
}

bool Shader::
fileExists(const string &file) {
    FILE *f = fopen(file.c_str(), "r");

    if (f == NULL) {
        return false;
    }
    else {
        fclose(f);
        return true;
    }
}

void Shader::
initShader(unsigned &handle, const string &file, GLenum shaderType) {
    handle = glCreateShader(shaderType);
    char *source = loadFile(file);

    glShaderSource(handle, 1, (const GLchar **) &source, NULL);
    glCompileShader(handle);

    free(source);

    int status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        int len;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

        char *log = new char[len];
        glGetShaderInfoLog(handle, len, &len, log);

        string sLog(log);
        string error = "In file '" + file + "':\n" + sLog;
        delete log;

        throw CompilationError(error);
    }
}

bool Shader::
isCurrentProgram() {
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    return _programHandle == (GLuint) currentProgram;
}

char *Shader::
loadFile(const string &fileName) {
    FILE *file = fopen(fileName.c_str(), "r");
    if (file == NULL) {
        string error = "Could not open file '" + fileName + "'.";
        throw CompilationError(error);
    }

    unsigned bufferSize = 1;
    char *buffer = (char *) malloc(bufferSize);

    unsigned long long index = 0;
    while (true) {
        char c = (char) fgetc(file);
        if (ferror(file) != 0) {
            string error = "Could not read from file '" + fileName + "'.";
            throw CompilationError(error);
        }

        if (feof(file)) {
            break;
        }
        else {
            /* There must be space at bufferSize - 2 for the new char, and at
             * bufferSize - 1 for the \0 character terminating the string. So if
             * the buffer's index has already reached bufferSize - 1 it is already
             * out of space! */
            if (index == bufferSize - 1) {
                bufferSize *= 2;
                buffer = (char *) realloc((void *) buffer, bufferSize);
            }

            buffer[index] = c;
            index++;
        }
    }
    buffer[index] = '\0';

    fclose(file);

    return buffer;
}
