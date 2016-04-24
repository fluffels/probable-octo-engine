#include "ShaderManager.h"

ShaderManager::
ShaderManager():
        _shaders() {}

ShaderManager::
~ShaderManager() {
    for (auto shader : _shaders) {
        delete shader;
    }
}

Shader& ShaderManager::
get(const string& name) {
    Shader* result = nullptr;
    for (auto shader : _shaders) {
        if (shader->getName() == name) {
            result = shader;
        }
    }
    if (result == nullptr) {
        result = new Shader(name);
        _shaders.push_back(result);
    }
    return *result;
}

void ShaderManager::
updateProjectionMatrices(mat4 matrix) {
    for (auto shader : _shaders) {
        shader->updateProjectionMatrix(matrix);
    }
}

void ShaderManager::
updateViewMatrices(mat4 matrix) {
    for (auto shader : _shaders) {
        shader->updateViewMatrix(matrix);
    }
}

void ShaderManager::
updateWorldMatrices(mat4 matrix) {
    for (auto shader : _shaders) {
        shader->updateWorldMatrix(matrix);
    }
}
