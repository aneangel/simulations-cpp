#ifndef BOUNDARY_FACTORY_H
#define BOUNDARY_FACTORY_H

#include <memory>
#include <string>
#include <stdexcept>

class BoundaryCondition {
public:
    virtual void apply(Particle& particle) const = 0;
    virtual ~BoundaryCondition() = default;
};

class SolidWall : public BoundaryCondition {
    void apply(Particle& particle) const override {
        if (particle.position.x < 0) particle.velocity.x = -particle.velocity.x;
    }
};

class PeriodicBoundary : public BoundaryCondition {
    void apply(Particle& particle) const override {
        if (particle.position.x > 1.0f) particle.position.x -= 1.0f;
    }
};

class BoundaryFactory {
public:
    static std::unique_ptr<BoundaryCondition> createBoundary(const std::string& type) {
        if (type == "solid") {
            return std::make_unique<SolidWall>();
        } else if (type == "periodic") {
            return std::make_unique<PeriodicBoundary>();
        } else {
            throw std::invalid_argument("Uknown boundary type ");
        }
    }
};

#endif