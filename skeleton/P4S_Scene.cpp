#include "P4S_Scene.h"
#include <algorithm>
#include <iostream>

void P4S_Scene::init()
{
    cleanup();

    anchorTransform = physx::PxTransform(physx::PxVec3(0.0f, 12.0f, 0.0f));
    waterTransform = physx::PxTransform(physx::PxVec3(0.0f, waterLevel, 0.0f));

    physx::PxShape* anchorShape = CreateShape(physx::PxBoxGeometry(0.35f,0.35f,0.35f));
    anchorItem = new RenderItem(anchorShape, &anchorTransform, Vector4(1,0.8f,0,1));
    anchorShape->release();

    physx::PxShape* waterShape = CreateShape(physx::PxBoxGeometry(12.0f,0.025f,12.0f));
    waterItem = new RenderItem(waterShape, &waterTransform, Vector4(0.1f,0.45f,0.9f,0.55f));
    waterShape->release();

    resetParticles();

    std::cout << "PRACTICA 4 - MUELLES" << std::endl
        << "1: muelle a punto fijo | 2: dos particulas | 3: goma elastica | 4: flotacion" << std::endl
        << "+/-: cambiar k | F: impulso breve | E: Euler explicito/semi-implicito" << std::endl
        << "M: cambiar masa | V: cambiar volumen | R: reiniciar" << std::endl;
}

void P4S_Scene::resetParticles()
{
    p1.reset(new Particle(Vector3D(4,7,0), Vector3D(0,0,0), Vector3D(), 0.999, mass, true));
    p2.reset(new Particle(Vector3D(7,7,0), Vector3D(0,0,0), Vector3D(), 0.999, mass, true));

    p1->setColor(0.9f,0.2f,0.2f,1);
    p2->setColor(0.2f,0.9f,0.3f,1);

    rebuildSpring();
}

void P4S_Scene::rebuildSpring()
{
    fixedSpring.reset(new SpringToAnchorGenerator(anchor, 5.0, k, 0.8));
    pairSpring.reset(new PairSpringGenerator(p1.get(), p2.get(), 3.0, k, mode == Mode::ElasticBand,0.8));
    buoyancy.reset(new BuoyancyGenerator(waterLevel, 1.0, volume, 1000.0));
}

void P4S_Scene::update(double dt)
{
    if (dt <= 0.0 || !p1 || !p2) return;

    dt = (std::min)(dt, 0.02);

    p1->addForce(Vector3D(0,-9.8*mass,0));
    p2->addForce(Vector3D(0,-9.8*mass,0));

    if (mode == Mode::FixedSpring) 
        fixedSpring->updateForce(*p1,dt);

    else if (mode == Mode::PairSpring || mode == Mode::ElasticBand) {
        pairSpring->updateForce(*p1, dt);
        pairSpring->updateForce(*p2, dt);
    }
        
    else buoyancy->updateForce(*p1,dt);

    if (impulseTime > 0.0) {
        p1->addForce(Vector3D(0, 120.0, 0));
        impulseTime -= dt;
    }

    auto integrate = [this,dt](Particle& p) {
        if (!explicitEuler) { p.integrate(dt); return; }
        p.integrateExplicit(dt);
    };

    integrate(*p1);

    if (mode == Mode::PairSpring || mode == Mode::ElasticBand) 
        integrate(*p2);
}

void P4S_Scene::keyPress(unsigned char key, const physx::PxTransform&)
{
    switch (key) {

    case '1': 
        mode = Mode::FixedSpring; 
        anchorTransform.p=physx::PxVec3(0,12,0); 
        rebuildSpring(); resetParticles(); 
        break;

    case '2': 
        mode = Mode::PairSpring; 
        rebuildSpring(); 
        resetParticles(); 
        break;

    case '3': 
        if (mode == Mode::PairSpring)
            mode = Mode::ElasticBand;
        else if (mode == Mode::ElasticBand)
            mode = Mode::PairSpring;

        rebuildSpring();

        std::cout << (mode == Mode::ElasticBand
            ? "Goma elastica activada\n"
            : "Muelle normal activado\n");

        break;

    case '4': 
        mode = Mode::Buoyancy; 
        rebuildSpring(); 
        resetParticles(); 
        p1->setPosition(Vector3D(0,5,0));
        break;

    case '+': 
    case '=': 
        k = (std::min)(1000.0,k+10.0); 
        rebuildSpring(); 
        std::cout << "k = " << k << "\n"; break;
    case '-':
    case '_': 
        k = (std::max)(0.0, k - 10.0); 
        rebuildSpring(); 
        std::cout << "k = "<< k << "\n"; break;
    case 'f': 
    case 'F': 
        impulseTime=0.25; break;
    case 'e': 
    case 'E': 
        explicitEuler =! explicitEuler; 
        std::cout << (explicitEuler ? "Euler explicito" : "Euler semi-implicito") << "\n"; 
        break;
    case 'm':
    case 'M': 
        mass = (mass<10.0?mass+0.5:1.0); 
        resetParticles(); 
        std::cout << "masa = " << mass << "\n"; 
        break;
    case 'v': 
    case 'V': 
        volume = (volume < 0.02 ? volume + 0.002 : 0.002); 
        rebuildSpring(); 
        std::cout << "volumen = " << volume << "\n"; 
        break;
    case 'r': 
    case 'R': 
        resetParticles(); 
        break;
    default: 
        break;
    }
}

void P4S_Scene::cleanup()
{
    p1.reset(); 
    p2.reset(); 

    fixedSpring.reset(); 
    pairSpring.reset(); 
    buoyancy.reset();

    if (anchorItem) { 
        anchorItem->release(); 
        anchorItem = nullptr; 
    }

    if (waterItem) { 
        waterItem->release(); 
        waterItem = nullptr; 
    }
}
