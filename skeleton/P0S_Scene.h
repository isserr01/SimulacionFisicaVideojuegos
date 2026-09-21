#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"  

class P0S_Scene : public Scene {
public:
    explicit P0S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override
    {
        physx::PxShape* sphere = CreateShape(physx::PxSphereGeometry(1.0f));

        Vector3D u(3.f, 1.f, 0.f);
        Vector3D v(0.f, 4.f, 0.f);
        Vector3D w = u.cross(v);


        m_transformX = physx::PxTransform(u.normalize() * 5.0f);
        m_transformY = physx::PxTransform(v.normalize() * 5.0f);
        m_transformZ = physx::PxTransform(w.normalize() * 5.0f);

        m_renderItemX = new RenderItem(sphere, &m_transformX, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        m_renderItemY = new RenderItem(sphere, &m_transformY, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
        m_renderItemZ = new RenderItem(sphere, &m_transformZ, Vector4(0.0f, 0.0f, 1.0f, 1.0f));


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