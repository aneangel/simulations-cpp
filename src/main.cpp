#include <iostream>
#include <vector>
#include <memory>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "ParticleFactory.h"
#include "KernelFactory.h"
#include "ForceFactory.h"
#include "BoundaryFactory.h"

std::vector<std::unique_ptr<Particle>> particles;
std::vector<std::unique_ptr<ForceModel>> forces;
std::unique_ptr<BoundaryCondition> boundary;
std::string simulationTitle = "Particle Fuild Simulation";

const float dt = 0.01f;
const int numParticles = 1000;
const float domainSize = 1.0f;
const float screenWidth = 800.0f;
const float screenHeight = 600.0f;

void initSimulation();
void updateSimulation();
void renderParticles();
void display();
void idle();


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);

    glutCreateWindow(simulationTitle.c_str());
    
    initSimulation();

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}

void initSimulation() {
    for (int i = 0; i < numParticles; i++) {
        auto particle = ParticleFactory::createParticle("water");
        particle->position = { static_cast<float>(rand()) / RAND_MAX * domainSize,
                                static_cast<float>(rand()) / RAND_MAX * domainSize,
                                0.0f };
        particle->velocity = { 0.0f, 0.0f, 0.0f };
        particles.push_back(std::move(particle));
    }

    forces.push_back(ForceFactory::createForce("gravity"));
    forces.push_back(ForceFactory::createForce("viscosity"));

    boundary = BoundaryFactory::createBoundary("solid");

    std::cout << "Simulation initalized with " << numParticles << " particles.\n";
}

void updateSimulation(float screenWidth, float screenHeight) {
    for (auto& particle : particles) {
        glm::vec3 acceleration = { 0.0f, 0.0f, 0.0f };

        for (auto& force : forces) {
            acceleration += force->computeForce(*particle);
        }

        particle->velocity += acceleration * dt;
        particle->position += particle->velocity * dt;

        if (particle->position.x < 0.0f) {
            particle->position.x = 0.0f;
            particle->velocity.x = -particle->velocity.x;
        } else if (particle->position.x > screenWidth) {
            particle->position.x = screenWidth;
            particle->velocity.x = -particle->velocity.x;
        }

        if (particle->position.y < 0.0f) {
            particle->position.y = 0.0f;
            particle->velocity.y = -particle->velocity.y;
        } else if (particle->position.y > screenHeight) {
            particle->position.y = screenHeight;
            particle->velocity.y = -particle->velocity.y;
        }
    }
}

void renderParticles() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_POINTS);
    for (const auto& particle : particles) {
        glVertex2f(particle->position.x / domainSize, particle->position.y / domainSize);
    }

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderParticles();
    glutSwapBuffers();
}

void idle() {
    updateSimulation(screenWidth, screenHeight);
    glutPostRedisplay();
}