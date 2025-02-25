#include "MeshRenderer.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* MeshRenderer::CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner;

    vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // draw 2 triangles; add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* MeshRenderer::CreateBottomTrapezoid(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner;

    vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-4, 0, 0), color),   // bottom left
        VertexFormat(corner + glm::vec3(4, 0, 0), color),    // bottom right
        VertexFormat(corner + glm::vec3(7, 2, 0), color),    // up right
        VertexFormat(corner + glm::vec3(-7, 2, 0), color)    // up left
    };

    Mesh* trapezoid = new Mesh(name);
    vector<unsigned int> indices;

    if (fill) {
        indices = {0, 1, 2, 0, 2, 3};
    } else {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
        indices = {0, 1, 2, 3};
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* MeshRenderer::CreateUpTrapezoid(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-7, 2, 0), color),    // bottom left
        VertexFormat(corner + glm::vec3(7, 2, 0), color),     // bottom right
        VertexFormat(corner + glm::vec3(5, 4, 0), color),     // up right
        VertexFormat(corner + glm::vec3(-5, 4, 0), color)     // up left
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices;

    if (fill) {
        // define indices to draw two triangles to form a filled trapezoid
        indices = {0, 1, 2, 0, 2, 3};
    } else {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
        // define indices for outline drawing
        indices = {0, 1, 2, 3};
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* MeshRenderer::CreateCircle(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill) {
    float xPos = 0.0f;  // center x position
    float yPos = 4.0f;  // center y position
    float radius = 2.0f;
    const int steps = 50;
    const float angle = 2 * M_PI / steps;

    vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(glm::vec3(xPos, yPos, leftBottomCorner.z), color));

    // create vertices for the circumference
    for (int i = 0; i <= steps; i++) {
        float xNew = radius * cos(angle * i);  // cos for x-coordinate
        float yNew = radius * sin(angle * i);  // sin for y-coordinate
        
        vertices.push_back(VertexFormat(glm::vec3(xPos + xNew, yPos + yNew, leftBottomCorner.z), color));
    }
    
    vector<unsigned int> indices;

    for (int i = 1; i <= steps; i++) {
        indices.push_back(0); // center vertex index
        indices.push_back(i); // current vertex index
        indices.push_back((i % steps) + 1);
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* MeshRenderer::CreateBarrel(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner + glm::vec3(0, 0, -.2f);

    vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-0.5, 4, 0), color),
        VertexFormat(corner + glm::vec3(0.5, 4, 0), color),
        VertexFormat(corner + glm::vec3(0.5, 10, 0), color),
        VertexFormat(corner + glm::vec3(-0.5, 10, 0), color)
    };

    Mesh* projectile = new Mesh(name);
    
    vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        projectile->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    projectile->InitFromData(vertices, indices);

    return projectile;
}

Mesh* MeshRenderer::CreateLineMesh(const std::string &name, const std::vector<glm::vec3> &points, glm::vec3 color) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    for (size_t i = 0; i < points.size(); ++i) {
        vertices.push_back(VertexFormat(glm::vec3(points[i].x, points[i].y, 0), color));
        indices.push_back(i);
    }

    Mesh* lineMesh = new Mesh(name);
    
    lineMesh->SetDrawMode(GL_LINE_STRIP);
    lineMesh->InitFromData(vertices, indices);

    return lineMesh;
}

Mesh* MeshRenderer::CreateProjectile(const std::string &name, float radius, glm::vec3 color, bool fill) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    int segments = 100;

    //  center circle
    if (fill) {
        vertices.push_back(VertexFormat(glm::vec3(0, 0, 0.5f), color));
    }

    // vertices around the circle
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0.5f), color));
        indices.push_back(i);
    }

    // connect the last vertex to the first to complete the loop
    indices.push_back(0);

    // create indices for a filled circle (triangles fan)
    if (fill) {
        for (int i = 1; i <= segments; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back((i % segments) + 1);
        }
    }

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(fill ? GL_TRIANGLES : GL_LINE_LOOP);
    circle->InitFromData(vertices, indices);

    return circle;
}

Mesh* MeshRenderer::CreateHealthBar(const std::string &name, glm::vec3 leftBottomCorner, float width, float height, float z,
                                    glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner;

    vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, height, z), color),
        VertexFormat(corner + glm::vec3(width, height, z), color),
        VertexFormat(corner + glm::vec3(width, height + 2, z), color),
        VertexFormat(corner + glm::vec3(0, height + 2, z), color)
    };

    Mesh* barrelLife = new Mesh(name);
    vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        barrelLife->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    barrelLife->InitFromData(vertices, indices);

    return barrelLife;
}
