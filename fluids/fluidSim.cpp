#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>
#include <random>

#include <iostream>

// Shader sources
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 Color;
    uniform mat4 MVP;
    void main() {
        gl_Position = MVP * vec4(aPos, 1.0);
        Color = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 Color;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(Color, 1.0);
    }
)";

// Particle structure
struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 color;
    float life;
};

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int MAX_PARTICLES = 1000;
const float CONTAINER_SIZE = 5.0f;

// Global variables
std::vector<Particle> particles;
GLuint VBO, VAO;
GLuint shaderProgram;
glm::mat4 projection, view, model;

// Function prototypes
void initOpenGL();
void initParticles();
void updateParticles(float deltaTime);
void renderParticles();
void cleanup();

int main() {
    initOpenGL();
    initParticles();

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fluid Simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Main loop
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        updateParticles(deltaTime);
        renderParticles();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanup();
    glfwTerminate();
    return 0;
}

void initOpenGL() {
    // Initialize GLFW and create window
    // Set up OpenGL context
    // Compile and link shaders
    // Set up projection and view matrices

    // Initialize GLFW
    if (!glfwInit()) {
        // Handle initialization failure
        return;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        // Handle window creation failure
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions
    if (glewInit() != GLEW_OK) {
        // Handle GLEW initialization failure
        return;
    }

    // Compile and link shaders
    // This is a simplified version; you'd need actual shader code
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Compile shaders...

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors...

    // Set up projection and view matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // Look at point
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );

    // Use shader program and set matrices
    glUseProgram(shaderProgram);
    // Set uniform variables for matrices in shader...
}

void initParticles() {
    // Initialize particles with random positions, velocities, and colors
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-CONTAINER_SIZE / 2, CONTAINER_SIZE / 2);
    std::uniform_real_distribution<> colorDis(0.0, 1.0);

    for (int i = 0; i < MAX_PARTICLES; ++i) {
        Particle p;
        p.position = glm::vec3(dis(gen), dis(gen), dis(gen));
        p.velocity = glm::vec3(dis(gen), dis(gen), dis(gen)) * 0.1f;
        p.color = glm::vec3(colorDis(gen), colorDis(gen), colorDis(gen));
        p.life = 1.0f;
        particles.push_back(p);
    }

    // Set up VAO and VBO for particles
}

void updateParticles(float deltaTime) {
    for (auto& p : particles) {
        // Update position
        p.position += p.velocity * deltaTime;

        // Simple boundary collision
        for (int i = 0; i < 3; ++i) {
            if (std::abs(p.position[i]) > CONTAINER_SIZE / 2) {
                p.position[i] = glm::sign(p.position[i]) * CONTAINER_SIZE / 2;
                p.velocity[i] *= -0.5f; // Damping
            }
        }

        // Update color (shift from red to green to blue)
        float t = glfwGetTime() * 0.5f;
        p.color.r = (std::sin(t) + 1.0f) / 2.0f;
        p.color.g = (std::sin(t + 2.0f * M_PI / 3.0f) + 1.0f) / 2.0f;
        p.color.b = (std::sin(t + 4.0f * M_PI / 3.0f) + 1.0f) / 2.0f;

        // Simple "gravity" effect
        p.velocity.y -= 0.1f * deltaTime;
    }
}

void renderParticles() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program
    glUseProgram(shaderProgram);

    // Update MVP matrix
    glm::mat4 MVP = projection * view * model;
    GLuint MVPLoc = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

    // Update VBO data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);

    // Draw particles
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, particles.size());
    glBindVertexArray(0);
}

void cleanup() {
    // Delete VAO, VBO, and shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}