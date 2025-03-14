#pragma once

#include <glm/glm.hpp>

#include <string>

class Entity
{
public:
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
    std::string texturePath;

    Entity(glm::vec2 pos, glm::vec2 sz, float rot, const std::string& texture)
        : position(pos), size(sz), rotation(rot), texturePath(texture) {}
};
