#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"  

class P0S_Scene : public Scene {
public:
    explicit P0S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override
    {
        physx::PxShape* X = CreateShape(physx::PxSphereGeometry(1.0f));
        physx::PxShape* Y = CreateShape(physx::PxSphereGeometry(1.0f));
        physx::PxShape* Z = CreateShape(physx::PxSphereGeometry(1.0f));

        m_transformX = physx::PxTransform(physx::PxVec3(3.0f, 1.0f, 0.0f));
        m_transformY = physx::PxTransform(physx::PxVec3(0.0f, 4.0f, 0.0f));


        //m_transformZ = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));

        m_renderItemX = new RenderItem(X, &m_transformX, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        m_renderItemY = new RenderItem(Y, &m_transformY, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

    }

    void update(double dt) override {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
        //m_transform.p.y -= static_cast<float>(9.8 * dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        if (key == 'r' || key == 'R') {
            m_transformX.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
        }
    }

    void cleanup() override {
        if (m_renderItemX) {
            m_renderItemX->release(); 
            m_renderItemX = nullptr;
        }
        if (m_renderItemY) {
            m_renderItemY->release();
            m_renderItemY = nullptr;
        }
    }

private:
    physx::PxTransform m_transformX;
    physx::PxTransform m_transformY;
    physx::PxTransform m_transformZ;
    RenderItem* m_renderItemX{ nullptr };
    RenderItem* m_renderItemY{ nullptr };
    RenderItem* m_renderItemZ{ nullptr };
};