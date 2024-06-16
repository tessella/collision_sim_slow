//
//  Sphere.h
//  CollisionSimulation
//
//  Created by Sebastian Cordoba on 14/06/2024.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <cmath>
#include <glm.hpp>

class Sphere {
private:
    double x;
    double y;
    double z;
    double radius;
    double velocityX;
    double velocityY;
    double velocityZ;
    double mass;

public:
    Sphere(double x, double y, double z, double radius, double mass, double velocityX, double velocityY, double velocityZ);

    double getX() const;
    double getY() const;
    double getZ() const;
    double getRadius() const;
    double getMass() const;
    double getVelocityX() const;
    double getVelocityY() const;
    double getVelocityZ() const;

    void setVelocity(double vx, double vy, double vz);
    void setX(double newX);
    void setY(double newY);
    void setZ(double newZ);

    void update(double dt);

    bool isColliding(const Sphere& other, double dt) const;
};

#endif /* Sphere_hpp */
