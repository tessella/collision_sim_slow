//
//  Sphere.cpp
//  CollisionSimulation
//
//  Created by Sebastian Cordoba on 14/06/2024.
//

#include "Sphere.hpp"

Sphere::Sphere(double x, double y, double z, double radius, double mass, double velocityX, double velocityY, double velocityZ)
    : x(x), y(y), z(z), radius(radius), mass(mass), velocityX(velocityX), velocityY(velocityY), velocityZ(velocityZ) {}

double Sphere::getX() const { return x; }
double Sphere::getY() const { return y; }
double Sphere::getZ() const { return z; }
double Sphere::getRadius() const { return radius; }
double Sphere::getMass() const { return mass; }
double Sphere::getVelocityX() const { return velocityX; }
double Sphere::getVelocityY() const { return velocityY; }
double Sphere::getVelocityZ() const { return velocityZ; }

void Sphere::setVelocity(double vx, double vy, double vz) {
    velocityX = vx;
    velocityY = vy;
    velocityZ = vz;
}

void Sphere::setX(double newX) { x = newX; }
void Sphere::setY(double newY) { y = newY; }
void Sphere::setZ(double newZ) { z = newZ; }

void Sphere::update(double dt) {
    
    const double gravity = -9.81; 
    velocityY += gravity * dt;
    
    x += velocityX * dt;
    y += velocityY * dt;
    z += velocityZ * dt;
}

bool Sphere::isColliding(const Sphere& other, double dt) const {
    glm::vec3 relativeVelocity(other.velocityX - velocityX, other.velocityY - velocityY, other.velocityZ - velocityZ);
    glm::vec3 relativePosition(other.x - x, other.y - y, other.z - z);

    double a = glm::dot(relativeVelocity, relativeVelocity);
    double b = 2.0 * glm::dot(relativeVelocity, relativePosition);
    double c = glm::dot(relativePosition, relativePosition) - std::pow(radius + other.radius, 2);

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }

    double t = (-b - std::sqrt(discriminant)) / (2 * a);

    return (t >= 0 && t <= dt);
}

//bool Sphere::isColliding(const Sphere& other) const {
//    double dx = x - other.x;
//    double dy = y - other.y;
//    double dz = z - other.z;
//    double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
//    return distance <= radius + other.radius;
//}
