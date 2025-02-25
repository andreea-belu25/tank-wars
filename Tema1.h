#pragma once

#include "components/simple_scene.h"

#include<vector>

using namespace std;

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;


    protected:
        glm::ivec2 resolution = window->GetResolution();
        glm::mat3 modelMatrix, healthBarBackgroundMatrix, healthBarMatrix;
        vector<float> heights;

        float TxTank1 = 0.0f;
        float TxTank2 = 10.0f;
        
        float tankSpeed = 100.0f;
        float tankRotationSpeed = 1.0f;
        
        float barrelAngleTank1 = 1.0f;
        float barrelAngleTank2 = 1.0f;
        
        const float minTurretAngle = -M_PI / 2.25f;  //  80 degrees
        const float maxTurretAngle = M_PI / 2.25f; 
        
        float initialProjectileSpeed = 60.0f;
        glm::vec3 projectileStart1 = glm::vec3(0, 0, 0);
        glm::vec3 projectileStart2 = glm::vec3(0, 0, 0);
        
        float healthTank1 = 10.0f;
        float healthTank2 = 10.0f;

        struct Projectile {
            glm::vec3 speed;
            glm::vec3 position;
            int tankId;
        };

        vector<Projectile> projectiles;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
        Mesh* InitializeTank(const std::string &name, glm::vec3 corner, glm::vec3 colorBase, glm::vec3 colorUpper, glm::vec3 colorCircle, 
        glm::vec3 colorBarrel, glm::vec3 colorHealthBarBackground, glm::vec3 colorHealthBar, float width, float height, bool fill);
        void RenderTerrain();
        void RenderAndMoveTank(const std::string &name, float Tx, float projectileAngle);
        void UpdateProjectile(glm::vec3 &position, glm::vec3 &speed);
        void DisplayTrajectory(float x, float y, float angle);
        void RenderProjectile(glm::vec3 position);
        glm::vec3 GetTankPosition(float Tx);
        void DidProjectileCollide();
        void DeformTerrain();
        void SlideTerrain();

        float GetProjectileAngle(float Tx, float projectileAngle);
    };
}   // namespace m1