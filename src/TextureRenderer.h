#pragma once

#include "Shader.h"
#include "Entity.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <string>
#include <iostream>

class TextureRenderer
{
public:
    TextureRenderer(Shader& shader) : shader(shader)
    {
        initRenderData();
    }

    ~TextureRenderer()
    {
        for (auto& [path, textureID] : textureCache)
        {
            glDeleteTextures(1, &textureID);
        }

        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteBuffers(1, &quadEBO);
    }

    GLuint getTexture(const std::string& texturePath)
    {
        if (textureCache.find(texturePath) != textureCache.end())
            return textureCache[texturePath];

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

        if (!data)
        {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return 0;
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        textureCache[texturePath] = textureID;
        return textureID;
    }

    void drawEntity(const Entity& entity)
    {
        GLuint textureID = getTexture(entity.texturePath);
        if (textureID == 0) return;

        shader.use();

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(entity.size, 1.0f));
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(entity.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(entity.position, 0.0f));
        glm::mat4 model = translate * rotation * scale;

        shader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    Shader& shader;
    GLuint quadVAO, quadVBO, quadEBO;
    std::unordered_map<std::string, GLuint> textureCache;

    void initRenderData()
    {
        constexpr float vertices[] = {
            // Positions          // Texture Coords
             0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // Top Right
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Bottom Right
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom Left
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // Top Left
        };

        constexpr unsigned int indices[] = {
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glGenBuffers(1, &quadEBO);

        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};
