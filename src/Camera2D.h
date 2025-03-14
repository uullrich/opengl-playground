#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D {
public:
    Camera2D(float left, float right, float bottom, float top)
        : zoom(1.0f), rotation(0.0f), left(left), right(right), bottom(bottom), top(top)
    {
        position = glm::vec2((left + right) / 2.0f, (bottom + top) / 2.0f); // Center of screen
    }

    glm::mat4 getProjectionMatrix() const {
        float halfWidth = (right - left) / 2.0f / zoom;
        float halfHeight = (top - bottom) / 2.0f / zoom;
        return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
    }

    glm::mat4 getViewMatrix() const {
        glm::mat4 view = glm::mat4(1.0f);
        
        // Move to center first
        view = glm::translate(view, glm::vec3(-position, 0.0f));
        
        // Apply rotation around the center
        view = glm::translate(view, glm::vec3(position, 0.0f)); // Move back to world position
        view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::translate(view, glm::vec3(-position, 0.0f)); // Undo world position movement

        return view;
    }

    glm::mat4 getVPMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }

    void move(glm::vec2 delta) {
        position += delta;
    }

    void setZoom(float factor) {
        zoom *= factor;
    }

    void rotate(float degrees) {
        rotation += degrees;
    }

private:
    float left, right, bottom, top;

    glm::vec2 position;
    float zoom;
    float rotation;
};
