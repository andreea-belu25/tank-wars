#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace MeshRenderer
{
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);

    Mesh* CreateBottomTrapezoid(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill);
    Mesh* CreateUpTrapezoid(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill);
    Mesh* CreateCircle(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill);
    Mesh* CreateBarrel(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill);
    Mesh* CreateHealthBar(const std::string &name, glm::vec3 leftBottomCorner, float width, float height, float z, 
    glm::vec3 color, bool fill);
    
    Mesh* CreateLineMesh(const std::string &name, const std::vector<glm::vec3> &points, glm::vec3 color);
    Mesh* CreateProjectile(const std::string &name, float radius, glm::vec3 color, bool fill);
}
