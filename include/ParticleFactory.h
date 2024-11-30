#ifndef PARTICLE_FACTORY_H
#define PARTICLE_FACTORY_H

#include <memory>
#include <string>
#include <stdexcept>
#include <glm/glm.hpp>

class Particle {
public:
    struct float3 { float x, y, z; };
    glm::vec3 position;
    glm::vec3 velocity;
    float density;
    float pressure;

    virtual ~Particle() = default;
    
};


class WaterParticle : public Particle {
public:
    WaterParticle() {
        density = 1000.0f;
        pressure = 0.0f;
    }
};

class OilParticle : public Particle {
public:
    OilParticle() {
        density = 800.0f;
        pressure = 0.0f;
    }
};

class ParticleFactory {
public:
    static std::unique_ptr<Particle> createParticle(const std::string& type) {
        if (type == "water") {
            return std::make_unique<WaterParticle>();
        } else if (type == "oil") {
            return std::make_unique<OilParticle>();
        } else {
            throw std::invalid_argument("Unkown particle type: " + type);
        }
    }
};


#endif // PARTICLE_FACTORY_H