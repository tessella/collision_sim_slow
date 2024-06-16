//
//  main.cpp
//  CollisionSimulation
//
//  Created by Sebastian Cordoba on 14/06/2024.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "Box.hpp"
#include "Sphere.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const int numSpheres = 400;
const double FPS = 60.0;
const double dt = 1.0 / FPS;

bool firstMouse = true;
bool mouseButtonHeld = false;
float yaw = -90.0f;
float pitch = 0.0f;
double lastX = WIDTH / 2.0;
double lastY = HEIGHT / 2.0;
float fov = 90.0f;



std::vector<Sphere> spheres;
GLUquadric* quadric = nullptr; // Made a global quadric so I don't have to create an individual one for every sphere

glm::vec3 cameraPos = glm::vec3(7.0f, 7.0f, 13.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.1f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mouseButtonHeld = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        } else if (action == GLFW_RELEASE) {
            mouseButtonHeld = false;
            firstMouse = true;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mouseButtonHeld) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.3f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }
}

void renderSphere(const Sphere& sphere) {
    glPushMatrix();
    
    glTranslatef(sphere.getX(), sphere.getY(), sphere.getZ());
    gluSphere(quadric, sphere.getRadius(), 32, 32);
    
    glPopMatrix();
}

void renderBox(const Box& box) {
    
    glPushMatrix();
    glTranslatef((box.getXMin() + box.getXMax()) / 2, (box.getYMin() + box.getYMax()) / 2, (box.getZMin() + box.getZMax()) / 2);
    glScalef(box.getXMax() - box.getXMin(), box.getYMax() - box.getYMin(), box.getZMax() - box.getZMin());
    
    // Box Render:
    
    glBegin(GL_LINES);
    
    // Front face:
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    
    // Back face:
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    
    // Edges:
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();
    
    glPopMatrix();
}

void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2, double dt) {
    glm::vec3 collisionNormal = glm::normalize(glm::vec3(sphere2.getX() - sphere1.getX(), sphere2.getY() - sphere1.getY(), sphere2.getZ() - sphere1.getZ()));
    float distance = glm::distance(glm::vec3(sphere1.getX(), sphere1.getY(), sphere1.getZ()), glm::vec3(sphere2.getX(), sphere2.getY(), sphere2.getZ()));
    float penetrationDepth = sphere1.getRadius() + sphere2.getRadius() - distance;

    if (penetrationDepth > 0) {
        float restitution = 0.95f;
        float totalMass = sphere1.getMass() + sphere2.getMass();

        // Positional correction:
        float correctionFactor = 0.2f;
        float correctionDistance = correctionFactor * penetrationDepth;
        glm::vec3 correctionVector = collisionNormal * correctionDistance;

        sphere1.setX(sphere1.getX() - correctionVector.x * (sphere1.getMass() / totalMass));
        sphere1.setY(sphere1.getY() - correctionVector.y * (sphere1.getMass() / totalMass));
        sphere1.setZ(sphere1.getZ() - correctionVector.z * (sphere1.getMass() / totalMass));

        sphere2.setX(sphere2.getX() + correctionVector.x * (sphere2.getMass() / totalMass));
        sphere2.setY(sphere2.getY() + correctionVector.y * (sphere2.getMass() / totalMass));
        sphere2.setZ(sphere2.getZ() + correctionVector.z * (sphere2.getMass() / totalMass));

        // Velocity adjustment:
        glm::vec3 relativeVelocity = glm::vec3(sphere2.getVelocityX() - sphere1.getVelocityX(), sphere2.getVelocityY() - sphere1.getVelocityY(), sphere2.getVelocityZ() - sphere1.getVelocityZ());
        float impulse = -(1.0f + restitution) * glm::dot(relativeVelocity, collisionNormal) / totalMass;

        sphere1.setVelocity(sphere1.getVelocityX() - impulse * collisionNormal.x * sphere2.getMass() / totalMass,
                            sphere1.getVelocityY() - impulse * collisionNormal.y * sphere2.getMass() / totalMass,
                            sphere1.getVelocityZ() - impulse * collisionNormal.z * sphere2.getMass() / totalMass);

        sphere2.setVelocity(sphere2.getVelocityX() + impulse * collisionNormal.x * sphere1.getMass() / totalMass,
                            sphere2.getVelocityY() + impulse * collisionNormal.y * sphere1.getMass() / totalMass,
                            sphere2.getVelocityZ() + impulse * collisionNormal.z * sphere1.getMass() / totalMass);
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Collision Simulation", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // Quadric creation:
    
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    // Box and Spheres construction:
    
    Box box(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    for (int i = 0; i < numSpheres; i++) {
        double x = (double)rand() / RAND_MAX * (box.getXMax() - box.getXMin()) + box.getXMin();
        double y = (double)rand() / RAND_MAX * (box.getYMax() - box.getYMin()) + box.getYMin();
        double z = (double)rand() / RAND_MAX * (box.getZMax() - box.getZMin()) + box.getZMin();
        double radius = 0.2;
        double mass = 1.0;
        double velocityX = (double)rand() / RAND_MAX * 10.0 - 5.0; // Random velocity between -5.0 and 5.0
        double velocityY = (double)rand() / RAND_MAX * 10.0 - 5.0;
        double velocityZ = (double)rand() / RAND_MAX * 10.0 - 5.0;
        Sphere sphere(x, y, z, radius, mass, velocityX, velocityY, velocityZ);
        spheres.push_back(sphere);
    }
    
    ///////////////////////
    ///
    /// Below is a test of two balls colliding in midair, made to see if collisions work properly.
    /// Currently, this creates a bunch of bugs and I don' t know why...
    ///
    ////////////////////////
    
//    // Sphere 1
//    double x1 = -2.0;
//    double y1 = 0.0;
//    double z1 = 0.0;
//    double radius1 = 0.2;
//    double mass1 = 1.0;
//    double velocityX1 = 5.0;
//    double velocityY1 = 0.0;
//    double velocityZ1 = 0.0;
//    Sphere sphere1(x1, y1, z1, radius1, mass1, velocityX1, velocityY1, velocityZ1);
//    spheres.push_back(sphere1);
//
//    // Sphere 2
//    double x2 = 2.0;
//    double y2 = 0.0;
//    double z2 = 0.0;
//    double radius2 = 0.2;
//    double mass2 = 1.0;
//    double velocityX2 = -5.0;
//    double velocityY2 = 0.0;
//    double velocityZ2 = 0.0;
//    Sphere sphere2(x2, y2, z2, radius2, mass2, velocityX2, velocityY2, velocityZ2);
//    spheres.push_back(sphere2);
    
    
    ///////////////////////
    ///
    /// End Test section.
    ///
    ////////////////////////
    
    // Lighting and material properties:
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 0.0f, 10.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat mat_shininess[] = { 32.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    int totalFrames = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov, static_cast<double>(WIDTH) / HEIGHT, 0.1, 100.0);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
                  cameraPos.x + cameraFront.x, 
                  cameraPos.y + cameraFront.y,
                  cameraPos.z + cameraFront.z,
                  cameraUp.x, cameraUp.y, cameraUp.z);
        
        glfwSetMouseButtonCallback(window, mouse_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        
        renderBox(box);
        
        ///////////////////////
        ///
        /// Yes, the below sections is ULTRA SLOW. Completely unnecessary
        /// for each ball to check collisions with _every other ball._ However, I currently
        /// don't care and will set it as a future project to optimise this code.
        ///
        ////////////////////////

        
        // Render spheres, check collisions and update positions:
        
        for (int i = 0; i < numSpheres; i++) {
            renderSphere(spheres[i]);
        }
        
        for (int i = 0; i < numSpheres; i++) {
            for (int j = i + 1; j < numSpheres; j++) {
                handleSphereSphereCollision(spheres[i], spheres[j], dt);
            }
        }
        
        for (int i = 0; i < numSpheres; i++) {
            if (!box.contains(spheres[i])) {
                box.handleCollision(spheres[i]);
            }
        }
        
        for (int i = 0; i < numSpheres; i++) {
            spheres[i].update(dt);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        totalFrames++;

    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    double totalTimeMs = duration.count();

    double averageFrameTime = totalTimeMs / totalFrames;
    std::cout << "Total frames: " << totalFrames << std::endl;
    std::cout << "Total time: " << totalTimeMs << " milliseconds" << std::endl;
    std::cout << "Average frame time: " << averageFrameTime << " milliseconds" << std::endl;

    gluDeleteQuadric(quadric);
    glfwTerminate();
    return 0;
}
