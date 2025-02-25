#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translation matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::transpose(glm::mat3(1, 0, translateX, 0, 1, translateY, 0, 0, 1));
    }

    // Scaling matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(glm::mat3(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1));
    }

    // Rotation matrix
    inline glm::mat3 Rotate(float radians)
    {
        return glm::transpose(glm::mat3(cos(radians), -sin(radians), 0, sin(radians), cos(radians), 0, 0, 0, 1));
    }

    // Shear OY
    inline glm::mat3 ShearOY(float shearY) 
    {
        return glm::mat3(1, shearY, 0, 0, 1, 0, 0, 0, 1);
    }

    // Reflextion OX
    inline glm::mat3 MirrorOx()
    {
        return glm::mat3(1, 0, 0, 0, -1, 0, 0, 0, 1);
    }
}   // namespace transform2D
