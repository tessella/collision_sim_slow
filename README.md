# Slow Collision Simulation


This project explores a straightforward collision simulation of several spheres bounded in a box. 
It does not implement any fancy optimisations like Octree (for calculating which spheres are colliding 
with the target sohere, and not have to run through every single sphere in the sim), or SAT/GJK algorithm. 
Furthermore, there's unnecessary excess memory allocation which can further be refined. Whenever I get to 
making this code run smoother, I'll create a new project from the original.


For these reasons, the upper limit on the amount of spheres my laptop (M1 Macbook Air, 16gb RAM) can render and 
simulate comfortably are around 400. Anything more will cause the simulation to slow down considerably on my 
device. If your computer is beefier feel free to explore this limit yourself.

# Future work
- [ ] Add Octree optimisation for spatial partinioning.
- [ ] Incorporate either Separating Axis Theorem or Gilbert-Johnson-Keerthi for sphere collisions.
- [ ] Clean up memory allocation to use the same GLUquadric for all spheres.
- [ ] Use fixed-size array instead of vector.
- [ ] (greedy) For case of 1 sphere, implement trajectory trail and impact sites.
- [ ] Add a GUI that allows you to stop/start the simulation and change the following parameters: gravity, restitution, amount of spheres and bounding shape.
