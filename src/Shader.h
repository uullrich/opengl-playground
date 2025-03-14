#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader
{
public:
    GLuint ID;

    Shader() 
    {
        ID = glCreateProgram();
    }

    ~Shader()
    {
        glDeleteProgram(ID);
    }

    Shader& attachFromFile(const std::string& filePath, GLenum type)
    {
        std::string code = readFile(filePath);
        return attachFromSource(code, type);
    }

    Shader& attachFromSource(const std::string& source, GLenum type)
    {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, type);
        
        glAttachShader(ID, shader);
        shaders.push_back(shader);
        return *this;
    }

    Shader& link()
    {
        glLinkProgram(ID);
        checkCompileErrors(ID, GL_PROGRAM);

        for (GLuint shader : shaders)
        {
            glDeleteShader(shader);
        }
        shaders.clear();

        return *this;
    }

    void use() const
    {
        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) const { glUniform1i(getUniformLocation(name), (int)value); }
    void setInt(const std::string& name, int value) const { glUniform1i(getUniformLocation(name), value); }
    void setFloat(const std::string& name, float value) const { glUniform1f(getUniformLocation(name), value); }
    void setVec2(const std::string& name, const glm::vec2& value) const { glUniform2fv(getUniformLocation(name), 1, &value[0]); }
    void setVec3(const std::string& name, const glm::vec3& value) const { glUniform3fv(getUniformLocation(name), 1, &value[0]); }
    void setVec4(const std::string& name, const glm::vec4& value) const { glUniform4fv(getUniformLocation(name), 1, &value[0]); }
    void setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]); }

private:
    std::vector<GLuint> shaders;
    mutable std::unordered_map<std::string, GLint> uniformCache;

    std::string readFile(const std::string& filePath) const
    {
        std::ifstream file(filePath);
        std::stringstream buffer;
        if (!file.is_open())
        {
            throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " + filePath);
        }
        buffer << file.rdbuf();
        return buffer.str();
    }

    GLint getUniformLocation(const std::string& name) const
    {
        if (uniformCache.find(name) != uniformCache.end())
            return uniformCache[name];

        GLint location = glGetUniformLocation(ID, name.c_str());
        uniformCache[name] = location;
        return location;
    }

    void checkCompileErrors(GLuint shader, GLenum type)
    {
        GLint success;
        GLchar infoLog[1024];

        if (type == GL_PROGRAM)
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
            }
        }
        else
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR (" << shaderTypeToString(type) << ")\n" << infoLog << std::endl;
            }
        }
    }

    std::string shaderTypeToString(GLenum type) const
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: return "VERTEX";
            case GL_FRAGMENT_SHADER: return "FRAGMENT";
            case GL_GEOMETRY_SHADER: return "GEOMETRY";
            default: return "UNKNOWN";
        }
    }
};