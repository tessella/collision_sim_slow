//
//  Box.h
//  CollisionSimulation
//
//  Created by Sebastian Cordoba on 14/06/2024.
//

#ifndef Box_hpp
#define Box_hpp

#include "Sphere.hpp"

class Box {
private:
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double zMin;
    double zMax;

public:
    Box(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);
    
    double getXMin() const { return xMin; }
    double getXMax() const { return xMax; }
    double getYMin() const { return yMin; }
    double getYMax() const { return yMax; }
    double getZMin() const { return zMin; }
    double getZMax() const { return zMax; }
    
    bool contains(const Sphere& sphere) const; // This checks if the sphere is inside the box
    
    void handleCollision(Sphere& sphere) const; // This takes care of collision
};

#endif /* Box_hpp */
