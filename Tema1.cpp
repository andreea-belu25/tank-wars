#include <vector>
#include <iostream>

#include "Tema1.h"
#include "MeshRenderer.h"
#include "transform2D.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1() {
}

Tema1::~Tema1() {
}

void Tema1::Init() {
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 2);
    float squareSide = 70.0f;

    for (int x = 0; x <= resolution.x + 1; x++) {
        float y = 200.0f * sin(0.001f * x) + 60.0f * sin(0.008f * x) + 30.0f * sin(0.018f * x)
                + 15.0f * sin(0.012f * x) + 20.0f;
        heights.push_back(y);
    }

    glm::vec3 colorSquare = glm::vec3(0.235f, 0.690f, 0.204f);
    
    Mesh* square = MeshRenderer::CreateSquare("square", glm::vec3(0, 0, 1.0f), 1.0f, colorSquare, true);
    AddMeshToList(square);
    
    glm::vec3 colorProjectile = glm::vec3(1, .5f, 0);
    
    Mesh* projectile = MeshRenderer::CreateProjectile("projectile", 7.5f, colorProjectile, true);
    AddMeshToList(projectile);

    glm::vec3 colorBaseTank1 = glm::vec3(0.6863f, 0.4510f, 0.3098f);
    glm::vec3 colorUpTank1 = glm::vec3(0.8235f, 0.7059f, 0.5490f);
    glm::vec3 colorCircleTank1 = glm::vec3(0.8235f, 0.7059f, 0.5490f);
    
    glm::vec3 colorBaseTank2 = glm::vec3(0.2941f, 0.3255f, 0.1255f);
    glm::vec3 colorUpTank2 = glm::vec3(0.5647f, 0.9333f, 0.5647f);
    glm::vec3 colorCircleTank2 = glm::vec3(0.5647f, 0.9333f, 0.5647f); 

    
    glm::vec3 colorBarrel = glm::vec3(0, 0, 0);
    glm::vec3 colorHealthBarBackground = glm::vec3(1.0f,0.749f,0.0f);
    glm::vec3 colorHealthBar = glm::vec3(0.9176f, 0.8667f, 0.7922f);
    float width = 12;
    float height = 12;

    Mesh* tank1 = InitializeTank("tank1", corner, colorBaseTank1, colorUpTank1, colorCircleTank1, colorBarrel,
    colorHealthBarBackground, colorHealthBar, width, height, true);

    Mesh* tank2 = InitializeTank("tank2", corner, colorBaseTank2, colorUpTank2, colorCircleTank2, colorBarrel, 
    colorHealthBarBackground, colorHealthBar, width, height, true);
}

void Tema1::FrameStart() {
    // backgound color
    glClearColor(0.1647f, 0.7333f, 0.8353f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

Mesh* Tema1::InitializeTank(const std::string &name, glm::vec3 corner, glm::vec3 colorBase, glm::vec3 colorUp, glm::vec3 colorCircle,
                            glm::vec3 colorBarrel, glm::vec3 colorHealthBarBackground, glm::vec3 colorHealthBar, float width, float height,
                            bool fill) {
    Mesh* baseTanc = MeshRenderer::CreateBottomTrapezoid(name + "Bottom", corner, colorBase, fill);
    AddMeshToList(baseTanc);

    Mesh* upTanc = MeshRenderer::CreateUpTrapezoid(name + "Upper", corner, colorUp, fill);
    AddMeshToList(upTanc);

    Mesh* circle = MeshRenderer::CreateCircle(name + "Circle", corner, colorCircle, fill);
    AddMeshToList(circle);

    Mesh* barrel = MeshRenderer::CreateBarrel(name + "Barrel", corner, colorBarrel, fill);
    AddMeshToList(barrel);

    Mesh *healthBarBackground = MeshRenderer::CreateHealthBar(name + "HealthBarBackground", corner, width, height, 0.0f,
    colorHealthBarBackground, fill);
    AddMeshToList(healthBarBackground);

    Mesh *healthBar = MeshRenderer::CreateHealthBar(name + "HealthBar", corner, width, height, 1.0f,
    colorHealthBar, fill);
    AddMeshToList(healthBar);

    return baseTanc; 
}

void Tema1::RenderTerrain() {
    for (int i = 0; i < heights.size() - 1; i++) {
        float Ax = i;
        float Ay = heights[i];
        float Bx = i + 1;
        float By = heights[i + 1];

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Ax, Ay);
        modelMatrix *= transform2D::ShearOY((By - Ay) / (Bx - Ax));
        modelMatrix *= transform2D::Scale(Bx - Ax, std::max(By, Ay));
        modelMatrix *= transform2D::MirrorOx();

        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::UpdateProjectile(glm::vec3 &position, glm::vec3 &speed) {
    //  update position considering current position; the same for speed
    float g = 9.8f;
    float deltaTime = 0.12f;

    float newx = position.x + speed.x * deltaTime;
    float newy = position.y + speed.y * deltaTime;

    speed = glm::vec3(speed.x, speed.y - g * deltaTime, 0);
    position = glm::vec3(newx, newy, 1.0f);
}

void Tema1::DisplayTrajectory(float x0, float y0, float angle) {
    vector<glm::vec3> trajectoryPoints;

    float v0 = 100.0f;
    float vx = v0 * cos(angle);
    float vy = v0 * sin(angle);

    glm::vec3 speed(vx, vy, 0.0f);
    glm::vec3 position(x0, y0, 0.0f);

    int num_points = 400;
    for (int i = 0; i < num_points; i++) {
        trajectoryPoints.push_back(glm::vec3(position.x, position.y, .02f));
        UpdateProjectile(position, speed);

        if (y0 + position.y < 0)  //  punctul cade sub nivelul solului => iesim din bucla
            break;
    }

    modelMatrix = glm::mat3(1);
    Mesh* trajectoryMesh = MeshRenderer::CreateLineMesh("trajectoryLine", trajectoryPoints, glm::vec3(1, 1, 1));

    glLineWidth(5.0f);
    RenderMesh2D(trajectoryMesh, shaders["VertexColor"], modelMatrix);
    // glLineWidth(1.0f);
}

float Tema1::GetProjectileAngle(float Tx, float projectileAngle) {
    float Ax, Ay, By, Bx;

    // // Find the segment where Tx lies between two x-coordinates
    for (int i = 0; i < heights.size() - 1; i++) {
        Ax = i;
        Bx = i + 1;
        Ay = heights[i];
        By = heights[i + 1];
        if (Tx >= Ax && Tx <= Bx) {
            break;
        }
    }

    float t = (Tx - Ax) / (Bx - Ax);
    float Ty = Ay + t * (By - Ay);
    
    //  Vx will always be 1
    float Vy = By - Ay;
    float angle = atan(Vy);   //   tank's angle relative to terrain

    //   barrel is by default at 90 degrees on xoy + barrel rotation + tank's angle
    return projectileAngle + glm::radians(90.0f) + angle;  
}

void Tema1::RenderAndMoveTank(const std::string &name, float Tx, float projectileAngle) {
    float Ax, Ay, By, Bx;

    for (int i = 0; i < heights.size() - 1; i++) {
        Ax = i;
        Bx = i + 1;
        Ay = heights[i];
        By = heights[i + 1];
        if (Tx >= Ax && Tx <= Bx) {
            break;
        }
    }

    float t = (Tx - Ax) / (Bx - Ax);
    float Ty = Ay + t * (By - Ay);
    float Vy = By - Ay;
    float tankAngle = atan(Vy);

    float healthWidth;
    
    if (name == "tank1") {
        healthWidth = healthTank1;
    } else {
        healthWidth = healthTank2;
    }

    if (healthWidth != 0.0f) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Tx, Ty);
        modelMatrix *= transform2D::Scale(10, 10);
        modelMatrix *= transform2D::Rotate(tankAngle);

        RenderMesh2D(meshes[name + "Bottom"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes[name + "Upper"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes[name + "Circle"], shaders["VertexColor"], modelMatrix);

        healthBarBackgroundMatrix = glm::mat3(1);
        healthBarBackgroundMatrix *= transform2D::Translate(Tx - 12.0f / 2 * 10, Ty);
        healthBarBackgroundMatrix *= transform2D::Scale(10, 10);

        RenderMesh2D(meshes[name + "HealthBarBackground"], shaders["VertexColor"], healthBarBackgroundMatrix);
        
        healthBarMatrix = glm::mat3(1);
        //  x and y for Translate considering the dimensions for barrel 
        healthBarMatrix *= transform2D::Translate(-12.0f / 2 * 10, 0.0f);
        healthBarMatrix *= transform2D::Translate(Tx, Ty);
        healthBarMatrix *= transform2D::Scale(10, 10);
        healthBarMatrix *= transform2D::Scale(healthWidth / 10.0, 1.0f);

        RenderMesh2D(meshes[name + "HealthBar"], shaders["VertexColor"], healthBarMatrix);

        modelMatrix *= transform2D::Translate(0, 4);
        modelMatrix *= transform2D::Rotate(projectileAngle);
        modelMatrix *= transform2D::Translate(0, -4);

        RenderMesh2D(meshes[name + "Barrel"], shaders["VertexColor"], modelMatrix);
        
        //  start position for projectile
        glm::vec3 projectileStart = glm::vec3(0, 10, 1);

        projectileStart = transform2D::Translate(0, -4) * projectileStart;
        projectileStart = transform2D::Rotate(projectileAngle) * projectileStart; 
        projectileStart = transform2D::Translate(0, 4) * projectileStart;
        projectileStart = transform2D::Rotate(tankAngle) * projectileStart; 
        projectileStart = transform2D::Scale(10, 10) * projectileStart;
        projectileStart = transform2D::Translate(Tx, Ty) * projectileStart;

        if (name == "tank1") {
            projectileStart1 = projectileStart;
        } else {
            projectileStart2 = projectileStart;
        }
        
        float angle = GetProjectileAngle(Tx, projectileAngle);

        DisplayTrajectory(projectileStart.x, projectileStart.y, angle);
    }
}

void Tema1::RenderProjectile(glm::vec3 position) {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(position.x, position.y);
    RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
}

glm::vec3 Tema1::GetTankPosition(float Tx) {
    float Ax, Ay, By, Bx;

    for (int i = 0; i < heights.size() - 1; i++) {
        Ax = i;
        Bx = i + 1;
        Ay = heights[i];
        By = heights[i + 1];
        
        if (Tx >= Ax && Tx <= Bx)
            break;
    }

    float t = (Tx - Ax) / (Bx - Ax);
    float Ty = Ay + t * (By - Ay);
    float Vy = By - Ay;
    float tankAngle = atan(Vy);

    glm::vec3 tankPosition = glm::vec3(0.0f, 2.0f, 1.0f);
    tankPosition = transform2D::Rotate(tankAngle) * tankPosition;
    tankPosition = transform2D::Scale(10, 10) * tankPosition;
    tankPosition = transform2D::Translate(Tx, Ty) * tankPosition;
    return tankPosition;
}

void Tema1::DidProjectileCollide() {
    for (int i = 0; i < projectiles.size(); i++) {
        RenderProjectile(projectiles[i].position);

        if (projectiles[i].position.x < 0 || projectiles[i].position.x > resolution.x) {
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }
        
        if (projectiles[i].position.y < 0) {
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }
        
        UpdateProjectile(projectiles[i].position, projectiles[i].speed);

        float Tx = TxTank1;
        if (projectiles[i].tankId == 1)
            Tx = TxTank2;

        if (projectiles[i].tankId == 1 && healthTank2 <= 0)
            continue;

        if (projectiles[i].tankId == 2 && healthTank1 <= 0)
            continue;

        glm::vec3 tankPosition = GetTankPosition(Tx);
        float distanceProjectileTank = glm::length(projectiles[i].position - tankPosition);

        if (distanceProjectileTank <= 5.0f * 10) {
            if (projectiles[i].tankId == 1) {
                healthTank2 = max(0.0f, healthTank2 - 0.5f); 
            } else {
                healthTank1 = max(0.0f, healthTank1 - 0.5f);
            }

            projectiles.erase(projectiles.begin() + i);
            i--;
        }
    }
}

void Tema1::DeformTerrain() {
    for (int i = 0; i < projectiles.size(); i++) {
        glm::vec3 currentPositionProjectile = glm::vec3(projectiles[i].position.x, projectiles[i].position.y, 0);
        float r = 40.0f;
        float terrainY = heights[(int)currentPositionProjectile.x];

        if (currentPositionProjectile.y < terrainY) {
            for (float j = currentPositionProjectile.x - r; j <= currentPositionProjectile.x + r; j++) {
                if (j >= 0 && j <= resolution.x) {
                    //  deltaY distance from center to point
                    float deltaY = sqrt(r * r - (currentPositionProjectile.x - j) * (currentPositionProjectile.x - j));
                    float newY = currentPositionProjectile.y - deltaY;
                    
                    if (newY < 0) {
                        newY = 0;
                    }

                    heights[(int)j] = newY;
                }
            }

            projectiles.erase(projectiles.begin() + i);
            i--;
        }
    }
}

void Tema1::SlideTerrain() {
    for (int i = 1; i < heights.size() - 1; i++) {
        float differenceHeights;
        float threshold = 2.5f;
        float epsilon = 0.5f; 

        differenceHeights = heights[i] - heights[i - 1];
        if (differenceHeights > threshold) {
            heights[i] -= epsilon;
            heights[i - 1] += epsilon;
        }

        differenceHeights = heights[i] - heights[i + 1];
        if (differenceHeights > threshold) {
            heights[i + 1] += epsilon;
            heights[i] -= epsilon;
        }
    }
}

void Tema1::Update(float deltaTimeSeconds) {
    RenderTerrain();
    RenderAndMoveTank("tank1", TxTank1, barrelAngleTank1);
    RenderAndMoveTank("tank2", TxTank2, barrelAngleTank2);
    DidProjectileCollide();
    DeformTerrain();
    SlideTerrain();
}

void Tema1::FrameEnd() {
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods) {
    // input for the first tank (A/D keys)
    if (window->KeyHold(GLFW_KEY_A)) {
        TxTank1 -= tankSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        TxTank1 += tankSpeed * deltaTime;
    }

    if (TxTank1 >= resolution.x - 10.0f) {
        TxTank1 = resolution.x - 10.0f;

    } else if (TxTank1 <= 0) {
        TxTank1 = 0;
    }

    // orientation of the barrel
    if (window->KeyHold(GLFW_KEY_W)) {
        barrelAngleTank1 += tankRotationSpeed * deltaTime;
        if (barrelAngleTank1 > maxTurretAngle) {
            barrelAngleTank1 = maxTurretAngle;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        barrelAngleTank1 -= tankRotationSpeed * deltaTime;
        if (barrelAngleTank1 < minTurretAngle) {
            barrelAngleTank1 = minTurretAngle;
        }
    }

    // input for the second tank
    if (window->KeyHold(GLFW_KEY_L)) {
        TxTank2 -= tankSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_R)) {
        TxTank2 += tankSpeed * deltaTime;
    }

    if (TxTank2 >= resolution.x - 10.0f) {
        TxTank2 = resolution.x - 10.0f;

    } else if (TxTank2 <= 0) {
        TxTank2 = 0;
    }

    if (window->KeyHold(GLFW_KEY_U)) {
        barrelAngleTank2 += tankRotationSpeed * deltaTime;
        if (barrelAngleTank2 > maxTurretAngle) {
            barrelAngleTank2 = maxTurretAngle;
        }
    }
    if (window->KeyHold(GLFW_KEY_Y)) {
        barrelAngleTank2 -= tankRotationSpeed * deltaTime;
        if (barrelAngleTank2 < minTurretAngle) {
            barrelAngleTank2 = minTurretAngle;
        }
    }
}

void Tema1::OnKeyPress(int key, int mods) {
    //  first tank
    if (window->KeyHold(GLFW_KEY_1) && healthTank1 > 0) {
        float angle = GetProjectileAngle(TxTank1, barrelAngleTank1);
        float v0 = 100.0f;
        float vx = v0 * cos(angle);
        float vy = v0 * sin(angle);
        
        glm::vec3 speed = glm::vec3(vx, vy, 0);
        glm::vec3 position = glm::vec3(projectileStart1.x, projectileStart1.y, 1);

        Projectile currentProjectile;

        currentProjectile.position = position;
        currentProjectile.speed = speed;
        currentProjectile.tankId = 1;

        projectiles.push_back(currentProjectile);
    }

    //  second tank
    if (window->KeyHold(GLFW_KEY_2) && healthTank2 > 0) {
        float angle = GetProjectileAngle(TxTank2, barrelAngleTank2);
        float v0 = 100.0f;
        float vx = v0 * cos(angle);
        float vy = v0 * sin(angle);
        
        glm::vec3 speed = glm::vec3(vx, vy, 0);
        glm::vec3 position = glm::vec3(projectileStart2.x, projectileStart2.y, 1);

        Projectile currentProjectile;

        currentProjectile.position = position;
        currentProjectile.speed = speed;
        currentProjectile.tankId = 2;

        projectiles.push_back(currentProjectile);
    }   
}

void Tema1::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void Tema1::OnWindowResize(int width, int height) {
}
