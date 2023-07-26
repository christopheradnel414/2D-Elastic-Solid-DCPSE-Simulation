# 2D Elastic Solic DCPSE Simulation
This repository contains the source code for a 2D linear elastostatics solid simulation using particle-based discretization method DCPSE (Discretization Corrected Particle Strength Exchange). This work is part of my undergraduate thesis in computational solid mechanics and this work is also published in JETS 2022 Journal titled "Discretization Corrected Particle Strength Exchange for Steady State Linear Elasticity". 

This repository mainly consists of 3 parts:
1. 2D Linear Elastostatics DCPSE Source Code (main directory)
2. Example Geometry Generation Code (geometries folder)
3. Result Plotter Code (result folder)

The main Solver is written in C++ and utilizes the Eigen (https://eigen.tuxfamily.org) library as the sparse linear algebra engine. The main solver takes into input a .txt file containing point clouds, each anotated with the constraint and condition of the nodes (inner node, dirichlet boundary node, neumann boundary node, etc) and outputs a .txt file of the simulation results containing the node displacements, stresses, and strains. This solver works by solving the Static Navier-Cauchy equations (https://en.wikipedia.org/wiki/Linear_elasticity) using DCPSE discretization method. The 2D Static Navier-Cauchy equations can be written as follows:
1. Static Navier-Cauchy for Inner Nodes:
<img width="569" alt="Screenshot 2023-07-27 at 00 42 36" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/f02865cf-ff84-4b38-97b9-4aeb8a15e7f2">
2. Static Navier-Cauchy for Neumann Nodes (Traction/Force Boundary):
<img width="717" alt="Screenshot 2023-07-27 at 00 42 46" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/138f0154-5cfb-4428-a17c-ceec1e6dcda1">
3. Static Navier-Cauchy for Dirichlet Nodes (Displacement Boundary):
<img width="161" alt="Screenshot 2023-07-27 at 00 42 58" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/34f4ac2b-c501-4d77-b175-994021ad1846">


Next, the solver will first compute the closest neighbours of each nodes by iterating through every possible node pairs and only considers node within the cutoff radius. 
