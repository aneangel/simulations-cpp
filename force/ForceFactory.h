#ifndef FORCE_FACTORY_H
#define FORCE_FACTORY_H

#include <memory>
#include <string>
#include <stdexcept>

class ForceModel {
public:
    virtual float3 computeForce(const Particle& particle) const = 0;
    virtual ~ForceModel() = default;
};

class GravityForce : public ForceModel {
    float3 computeForce(const Particle& Particle) const override {
        return {0.0f, -9.8f, 0.0f};
    }  
};

class ViscosityForce : public ForceModel {
    float3 computeForce(const Particle& particle) const override {
        return {0.0f, 0.0f, 0.0f};
    }
};

class ForceFactory {
public:
    static std::unique_ptr<ForceModel> createForce(const std::string& type) {
        if (type == "gravity") {
            return std::make_unique<GravityForce>();
        } else if (type == "viscosity") {
            return std::make_unique<ViscosityForce>();
        } else {
            throw std::invalid_argument("Uknonw force type");
        }
    }
};

#endif