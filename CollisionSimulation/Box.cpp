//
//  Box.cpp
//  CollisionSimulation
//
//  Created by Sebastian Cordoba on 14/06/2024.
//

#include "Box.hpp"
#include "Sphere.hpp"

Box::Box(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
    : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), zMin(zMin), zMax(zMax) {}


bool Box::contains(const Sphere& sphere) const {
    double x = sphere.getX();
    double y = sphere.getY();
    double z = sphere.getZ();
    double radius = sphere.getRadius();
    
    return x - radius >= xMin && x + radius <= xMax &&
           y - radius >= yMin && y + radius <= yMax &&
           z - radius >= zMin && z + radius <= zMax;
}


void Box::handleCollision(Sphere& sphere) const {
    double x = sphere.getX();
    double y = sphere.getY();
    double z = sphere.getZ();
    double radius = sphere.getRadius();
    double restitution = 1.0f;

    if (x - radius < xMin) {
        sphere.setVelocity(-sphere.getVelocityX() * restitution, sphere.getVelocityY(), sphere.getVelocityZ());
        sphere.setX(xMin + radius);
    } else if (x + radius > xMax) {
        sphere.setVelocity(-sphere.getVelocityX() * restitution, sphere.getVelocityY(), sphere.getVelocityZ());
        sphere.setX(xMax - radius);
    }

    if (y - radius < yMin) {
        sphere.setVelocity(sphere.getVelocityX(), -sphere.getVelocityY() * restitution, sphere.getVelocityZ());
        sphere.setY(yMin + radius);
    } else if (y + radius > yMax) {
        sphere.setVelocity(sphere.getVelocityX(), -sphere.getVelocityY() * restitution, sphere.getVelocityZ());
        sphere.setY(yMax - radius);
    }

    if (z - radius < zMin) {
        sphere.setVelocity(sphere.getVelocityX(), sphere.getVelocityY(), -sphere.getVelocityZ() * restitution);
        sphere.setZ(zMin + radius);
    } else if (z + radius > zMax) {
        sphere.setVelocity(sphere.getVelocityX(), sphere.getVelocityY(), -sphere.getVelocityZ() * restitution);
        sphere.setZ(zMax - radius);
    }
}


