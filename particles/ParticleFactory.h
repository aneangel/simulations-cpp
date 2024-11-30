#ifndef PARTICLE_FACTORY_H
#define PARTICLE_FACTORY_H

#include <memory>
#include <string>
#include <stdexcept>

struct float3 {
    float x, y, z;

    float3 operator+(const float3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    float3& operator+=(const float3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    float3 operator*(float scalar) const {
        return {x * scalar, y *scalar, z * scalar};
    }

    float3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
};

class Particle {
public:
    struct float3 { float x, y, z; };
    float3 position;
    float3 velocity;
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